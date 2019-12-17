#include "restapiclient.h"

#include <QSslConfiguration>
#include <QtNetwork>

Q_DECLARE_LOGGING_CATEGORY(restapi)
Q_LOGGING_CATEGORY(restapi, "pl.restapiclient")
Q_DECLARE_LOGGING_CATEGORY(rest_timeout)
Q_LOGGING_CATEGORY(rest_timeout, "pl.restapiclient.timeout")

RestApiClient::RestApiClient(QObject *parent):
    QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished,
                this, &RestApiClient::reply_finished);

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);

    QUrl m_main_frame_qurl;
    m_main_frame_qurl.setScheme("https");
    m_main_frame_qurl.setHost("dashdb-txn-sbox-yp-lon02-02.services.eu-gb.bluemix.net");
    m_main_frame_qurl.setPort(8443);
    m_main_frame_qurl.setPath("/dbapi/v3/auth/tokens");

    m_request_main_frame.setUrl(m_main_frame_qurl);
    m_request_main_frame.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_request_main_frame.setSslConfiguration(config);

    QUrl m_sql_frame_qurl;
    m_sql_frame_qurl.setScheme("https");
    m_sql_frame_qurl.setHost("dashdb-txn-sbox-yp-lon02-02.services.eu-gb.bluemix.net");
    m_sql_frame_qurl.setPort(8443);
    m_sql_frame_qurl.setPath("/dbapi/v3/sql_jobs");

    m_request_sql_frame.setUrl(m_sql_frame_qurl);
    m_request_sql_frame.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_request_sql_frame.setSslConfiguration(config);

    m_result_frame_qurl.setScheme("https");
    m_result_frame_qurl.setHost("dashdb-txn-sbox-yp-lon02-02.services.eu-gb.bluemix.net");
    m_result_frame_qurl.setPort(8443);


    m_request_result_frame.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_request_result_frame.setSslConfiguration(config);

    send_authorization_frame();
}

void RestApiClient::send_authorization_frame()
{
    QString json = QString("{ "
                           "\"userid\":\"htf63494\", "
                           "\"password\":\"t2sw^pjbzqvk1gr5\" "
                           "}"
                           );
    auto reply = m_manager->post(m_request_main_frame, json.toUtf8());
    reply->setProperty("request_type", CustomTypes::RequestHello);

    qCDebug(restapi) << "Ramka authorization: " << json.toUtf8();

    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(reply, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::send_sql_frame(QString query, CustomTypes::RequestType request_type)
{
    QString json = QString(query);
    auto reply = m_manager->post(m_request_sql_frame, json.toUtf8());
    reply->setProperty("request_type", request_type);

    qCDebug(restapi) << "Ramka sql: " << json.toUtf8();

    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(reply, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::send_result_frame(QString job_id)
{
    QString job_endpoint = "/dbapi/v3/sql_jobs/" + job_id;
    qCDebug(restapi) << job_endpoint;
    m_result_frame_qurl.setPath(job_endpoint);
    m_request_result_frame.setUrl(m_result_frame_qurl);
    auto reply = m_manager->get(m_request_result_frame);
    reply->setProperty("request_type", CustomTypes::RequestResponse);
    reply->setProperty("job_id", job_id);

    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(reply, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::slot_ssl_errors(const QList<QSslError> &errors)
{
    qCWarning(restapi) << errors;
}

void RestApiClient::reply_finished(QNetworkReply *reply)
{
    if(!reply){
        qWarning(restapi) << "brak obiektu, wysyłającego zapytanie";
        return;
    }

    QByteArray reply_array = reply->readAll();
    qCDebug(restapi) << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    CustomTypes::RequestType type_request = reply->property("request_type").value<CustomTypes::RequestType>();
    if(reply->error() == QNetworkReply::NoError){
        switch (type_request){
        case CustomTypes::RequestHello:
        {
            qCDebug(restapi) << "Odpowiedź ramki autoryzacyjnej:";
            qCDebug(restapi) << reply_array;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_array);
            QString token = jsonResponse.object()["token"].toString();
            QByteArray data = token.toLocal8Bit();
            QString headerData = "Bearer " + data;
            qCDebug(restapi) << headerData;
            m_request_sql_frame.setRawHeader(QByteArray("Authorization"), headerData.toLocal8Bit());
            m_request_result_frame.setRawHeader(QByteArray("Authorization"), headerData.toLocal8Bit());
            return;
        }
        case CustomTypes::RequestResponse:
        {
            QString job_id = reply->property("job_id").toString();
            CustomTypes::RequestType request_type = m_result_map.take(job_id);
            emit reply_received(request_type, reply_array);
            break;
        }
        case CustomTypes::RequestTest:
        case CustomTypes::RequestCategory:
        case CustomTypes::RequestQuestion:
        case CustomTypes::RequestAnswerForQuestion:
        case CustomTypes::RequestCreatedTestWithZerosRequest:
        case CustomTypes::RequestCreatedTestWithoutZerosRequest:
        {
            qCDebug(restapi) << type_request << "response:";
            qCDebug(restapi) << reply_array;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_array);
            QString job_id = jsonResponse.object()["id"].toString();
            m_result_map.insert(job_id, type_request);
            send_result_frame(job_id);
            return;
        }
        case CustomTypes::RequestAddCategory:
        {
            send_category_request();
            break;
        }
        default:
            break;
        }
    }
    if (reply->error()) {
        qCWarning(restapi) << reply->errorString();
        return;
    }
}

void RestApiClient::slot_error(QNetworkReply::NetworkError)
{
    qCWarning(restapi) << "wystąpił błąd";
}

void RestApiClient::send_test_request()
{
    QString query = "{ "
                    "\"commands\":\"SELECT * FROM TEST;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query, CustomTypes::RequestTest);
}

void RestApiClient::send_category_request()
{
    QString query = "{ "
                    "\"commands\":\"SELECT * FROM CATEGORY;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query, CustomTypes::RequestCategory);
}

void RestApiClient::send_add_category_request(const QString category_name)
{
    QString query = QString("{ "
                    "\"commands\":\"INSERT INTO CATEGORY(CATEGORY_NAME, PARENT_CATEGORY_ID) VALUES ('%1', NULL);\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}").arg(category_name);
    send_sql_frame(query, CustomTypes::RequestAddCategory);
}

void RestApiClient::send_question_request()
{
    QString query = "{ "
                    "\"commands\":\"SELECT * FROM QUESTION;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query, CustomTypes::RequestQuestion);
}

void RestApiClient::send_answer_for_question_request(const QString question_id)
{
    QString query = QString("{ "
                    "\"commands\":\"SELECT id_answer, question_id FROM ANSWER WHERE question_id = %1;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}").arg(question_id);
    send_sql_frame(query, CustomTypes::RequestAnswerForQuestion);
}

void RestApiClient::send_add_question_variant_request(const QString question_id, const QString answer_order)
{
    QString query = QString("{ "
                            "\"commands\":\"INSERT INTO QUESTION_VARIANT(QUESTION_ID, ANSWER_ORDER) VALUES (%1, '%2');\", "
                            "\"limit\":1000, "
                            "\"separator\":\";\", "
                            "\"stop_on_error\":\"yes\" "
                            "}").arg(question_id, answer_order);
    send_sql_frame(query, CustomTypes::RequestAddQuestionVariant);
}

void RestApiClient::send_update_category_request(const QString category_id, const QString category_name)
{
    QString query = QString("{ "
                            "\"commands\":\"UPDATE CATEGORY SET category_name = '%1' WHERE id_category = %2;\", "
                            "\"limit\":1000, "
                            "\"separator\":\";\", "
                            "\"stop_on_error\":\"yes\" "
                            "}").arg(category_name, category_id);
    send_sql_frame(query, CustomTypes::RequestUpdateCategory);
}

void RestApiClient::send_add_question_request(const QList<QString> new_question)
{
    QString query = QString("{ "
                            "\"commands\":\"CALL add_question('%1', '%2', '%3', '%4', '%5', %6, 1);\", "
                            "\"limit\":1000, "
                            "\"separator\":\";\", "
                            "\"stop_on_error\":\"yes\" "
                            "}").arg(new_question[1], new_question[2], new_question[3], new_question[4],
            new_question[5], new_question[0]);
    send_sql_frame(query, CustomTypes::RequestAddQuestion);
}

void RestApiClient::send_created_test_with_zeros_request()
{
    QString query = "{ "
                    "\"commands\":\"SELECT u.lname, u.fname, COUNT(q.teacher_id) as liczba_pytan FROM question AS q "
                        "RIGHT JOIN user AS u ON q.teacher_id = u.id_user "
                        "GROUP BY u.id_user, u.fname, u.lname;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query, CustomTypes::RequestCreatedTestWithZerosRequest);
}

void RestApiClient::send_created_test_without_zeros_request()
{
    QString query = "{ "
                    "\"commands\":\"SELECT u.lname, u.fname, COUNT(q.teacher_id) as liczba_pytan FROM question AS q "
                        "RIGHT JOIN teacher AS t ON q.teacher_id = t.id_teacher "
                        "LEFT JOIN user AS u ON t.id_teacher = u.id_user "
                        "GROUP BY u.id_user, u.fname, u.lname "
                        "HAVING COUNT(q.teacher_id) > 0 "
                        "ORDER BY liczba_pytan DESC;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query, CustomTypes::RequestCreatedTestWithoutZerosRequest);
}
