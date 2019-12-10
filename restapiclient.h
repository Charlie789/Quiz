#ifndef RESTAPICLIENT_H
#define RESTAPICLIENT_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QPointer>
#include <QLoggingCategory>

class RestApiClient : public QObject
{
public:
    RestApiClient(QObject *parent = nullptr);

private:
    QPointer<QNetworkReply> m_reply_main_frame;
    QPointer<QNetworkReply> m_reply_sql_frame;
    QPointer<QNetworkReply> m_reply_result_frame;
    QNetworkAccessManager* m_manager;
    QNetworkRequest m_request_main_frame;
    QNetworkRequest m_request_sql_frame;
    QNetworkRequest m_request_result_frame;
    QNetworkConfigurationManager m_network_conf_manager;
    QUrl m_result_frame_qurl;

private slots:
    void send_main_frame();
    void send_sql_frame();
    void send_result_frame();
    void ready_read_main_frame();
    void ready_read_sql_frame();
    void ready_read_result_frame();
    void slot_ssl_errors(const QList<QSslError> &errors);
    void reply_finished(QNetworkReply *m_reply_main_frame);
    void slot_error(QNetworkReply::NetworkError);
};

#endif // RESTAPICLIENT_H
