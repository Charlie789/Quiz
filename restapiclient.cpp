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

    send_main_frame();
}

void RestApiClient::send_main_frame()
{
    QString json = QString("{ "
                           "\"userid\":\"htf63494\", "
                           "\"password\":\"t2sw^pjbzqvk1gr5\" "
                           "}"
                           );
    auto reply = m_manager->post(m_request_main_frame, json.toUtf8());
    reply->setProperty("request_type", RequestHello);

    qCDebug (restapi) << "ramka główna: " << json.toUtf8();

    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(reply, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::send_sql_frame(QString query, RequestType request_type)
{
    QString json = QString(query);
    auto reply = m_manager->post(m_request_sql_frame, json.toUtf8());
    reply->setProperty("request_type", request_type);

    qCDebug (restapi) << "ramka główna: " << json.toUtf8();

    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(reply, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::send_result_frame(QString job_id)
{
    QString job_endpoint = "/dbapi/v3/sql_jobs/" + job_id;
    qDebug() << job_endpoint;
    m_result_frame_qurl.setPath(job_endpoint);
    m_request_result_frame.setUrl(m_result_frame_qurl);
    auto reply = m_manager->get(m_request_result_frame);
    reply->setProperty("request_type", RequestResponse);
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
        qWarning() << "zjebałeś";
        return;
    }

    QByteArray reply_array = reply->readAll();
    qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(reply->error() == QNetworkReply::NoError){
        switch (reply->property("request_type").value<RequestType>()){
        case RequestHello:
        {
            qCDebug(restapi) << "main frame response:";
            qCDebug(restapi) << reply_array;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_array);
            QString token = jsonResponse.object()["token"].toString();
            QByteArray data = token.toLocal8Bit();
            QString headerData = "Bearer " + data;
            qDebug() << headerData;
            m_request_sql_frame.setRawHeader(QByteArray("Authorization"), headerData.toLocal8Bit());
            m_request_result_frame.setRawHeader(QByteArray("Authorization"), headerData.toLocal8Bit());
            return;
        }
        case RequestResponse:
        {
            QString job_id = reply->property("job_id").toString();
            RequestType request_type = m_result_map.take(job_id);
            switch (request_type) {
            case RequestTest:
            {
                qCDebug(restapi) << "test result response:";
                qCDebug(restapi) << reply_array;
                break;
            }
            case RequestCategory:
            {
                qCDebug(restapi) << "category result response:";
                qCDebug(restapi) << reply_array;
                break;
            }
            default:
                qCDebug(restapi) << "nieznane zapytanie sql";
                break;
            }
            return;
        }
        case RequestTest:
        {
            qCDebug(restapi) << "RequestTest response:";
            qCDebug(restapi) << reply_array;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_array);
            QString job_id = jsonResponse.object()["id"].toString();
            m_result_map.insert(job_id, reply->property("request_type").value<RequestType>());
            send_result_frame(job_id);
            return;
        }
        case RequestCategory:
        {
            qCDebug(restapi) << "RequestCategory response:";
            qCDebug(restapi) << reply_array;
            QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_array);
            QString job_id = jsonResponse.object()["id"].toString();
            m_result_map.insert(job_id, reply->property("request_type").value<RequestType>());
            send_result_frame(job_id);
            return;
        }
        default:
            qCDebug(restapi) << "nieznane żądanie z RestApi";
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
    QString query_test = "{ "
                    "\"commands\":\"SELECT * FROM TEST;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query_test, RequestTest);
}

void RestApiClient::send_category_request()
{
    QString query_category = "{ "
                    "\"commands\":\"SELECT * FROM CATEGORY;\", "
                    "\"limit\":1000, "
                    "\"separator\":\";\", "
                    "\"stop_on_error\":\"yes\" "
                    "}";
    send_sql_frame(query_category, RequestCategory);
}
