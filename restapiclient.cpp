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
    m_request_main_frame.setSslConfiguration(config);

    QUrl m_main_frame_qurl;
    m_main_frame_qurl.setScheme("https");
    m_main_frame_qurl.setHost("dashdb-txn-sbox-yp-lon02-02.services.eu-gb.bluemix.net");
    m_main_frame_qurl.setPort(8443);
    m_main_frame_qurl.setPath("/dbapi/v3/auth/tokens");

    m_request_main_frame.setUrl(m_main_frame_qurl);
    m_request_main_frame.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QUrl m_sql_frame_qurl;
    m_sql_frame_qurl.setScheme("https");
    m_sql_frame_qurl.setHost("dashdb-txn-sbox-yp-lon02-02.services.eu-gb.bluemix.net");
    m_sql_frame_qurl.setPort(8443);
    m_sql_frame_qurl.setPath("/dbapi/v3/sql_jobs");

    m_request_sql_frame.setUrl(m_sql_frame_qurl);
    m_request_sql_frame.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    m_request_sql_frame.setSslConfiguration(config);


    send_main_frame();
}

void RestApiClient::send_main_frame()
{
    QString json = QString("{ "
                           "\"userid\":\"htf63494\", "
                           "\"password\":\"t2sw^pjbzqvk1gr5\" "
                           "}"
                           );
    m_reply_main_frame = m_manager->post(m_request_main_frame, json.toUtf8());

    qCDebug (restapi) << "ramka główna: " << json.toUtf8();

    connect(m_reply_main_frame, &QNetworkReply::finished, this, &RestApiClient::ready_read_main_frame);
    connect(m_reply_main_frame, &QNetworkReply::finished, m_reply_main_frame, &QNetworkReply::deleteLater);
    connect(m_reply_main_frame, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(m_reply_main_frame, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::send_sql_frame()
{
    QString json = QString("{ "
                           "\"commands\":\"SELECT * FROM TEST;\", "
                           "\"limit\":1000, "
                           "\"separator\":\";\", "
                           "\"stop_on_error\":\"yes\" "
                           "}"
                           );
    m_reply_sql_frame = m_manager->post(m_request_sql_frame, json.toUtf8());
    m_reply_sql_frame = m_manager->post(m_request_sql_frame, json.toUtf8());

    qCDebug (restapi) << "ramka główna: " << json.toUtf8();

    connect(m_reply_sql_frame, &QNetworkReply::finished, this, &RestApiClient::ready_read_sql_frame);
    connect(m_reply_sql_frame, &QNetworkReply::finished, m_reply_sql_frame, &QNetworkReply::deleteLater);
    connect(m_reply_sql_frame, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error),
            this, &RestApiClient::slot_error);
    connect(m_reply_sql_frame, &QNetworkReply::sslErrors,
            this, &RestApiClient::slot_ssl_errors);
}

void RestApiClient::ready_read_main_frame()
{
    if(m_reply_main_frame->error()== QNetworkReply::OperationCanceledError){
        return;
    }

    QByteArray reply_array = m_reply_main_frame->readAll();
    qDebug() << m_reply_main_frame->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(m_reply_main_frame->error() == QNetworkReply::NoError){
        qCDebug(restapi) << "main frame response:";
        qCDebug(restapi) << reply_array;
        QJsonDocument jsonResponse = QJsonDocument::fromJson(reply_array);
        QString token = jsonResponse.object()["token"].toString();
        QByteArray data = token.toLocal8Bit();
        QString headerData = "Bearer " + data;
        qDebug() << headerData;
        m_request_sql_frame.setRawHeader(QByteArray("Authorization"), headerData.toLocal8Bit());
        qDebug() << "sdsdsd";
        qDebug() << m_request_sql_frame.rawHeader("Authorization");
        send_sql_frame();
        return;
    }

    qCDebug(restapi) << "Błąd pobierania danych main_frame:" << m_reply_main_frame->errorString() << m_reply_main_frame->error();
}

void RestApiClient::ready_read_sql_frame()
{
    if(m_reply_sql_frame->error()== QNetworkReply::OperationCanceledError){
        return;
    }

    QByteArray reply_array = m_reply_sql_frame->readAll();
    qDebug() << m_reply_sql_frame->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    if(m_reply_sql_frame->error() == QNetworkReply::NoError){
        qCDebug(restapi) << "sql response:";
        qCDebug(restapi) << reply_array;
        return;
    }

    qCDebug(restapi) << "Błąd pobierania danych main_frame:" << m_reply_sql_frame->errorString() << m_reply_sql_frame->error();
}

void RestApiClient::slot_ssl_errors(const QList<QSslError> &errors)
{
    qCWarning(restapi) << errors;
}

void RestApiClient::reply_finished(QNetworkReply *reply)
{
    if (reply->error()) {
        qCWarning(restapi) << reply->errorString();
        return;
    }
}

void RestApiClient::slot_error(QNetworkReply::NetworkError)
{
    qCWarning(restapi) << "wystąpił błąd";
}
