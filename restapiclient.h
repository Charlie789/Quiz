#ifndef RESTAPICLIENT_H
#define RESTAPICLIENT_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkConfigurationManager>
#include <QTimer>
#include <QPointer>
#include <QLoggingCategory>
#include <QMap>
#include "customtypes.h"

class RestApiClient : public QObject
{
    Q_OBJECT

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
    QMap<QString, CustomTypes::RequestType> m_result_map;

private slots:
    void send_authorization_frame();
    void send_sql_frame(QString query, CustomTypes::RequestType request_type);
    void send_result_frame(QString job_id);
    void slot_ssl_errors(const QList<QSslError> &errors);
    void reply_finished(QNetworkReply *m_reply_main_frame);
    void slot_error(QNetworkReply::NetworkError);

public slots:
    void send_test_request();
    void send_category_request();
    void send_add_category_request(const QString category_name);
    void send_question_request();
    void send_answer_for_question_request(const QString question_id);
    void send_add_question_variant_request(const QString question_id, const QString answer_order);
    void send_update_category_request(const QString category_id, const QString category_name);
    void send_add_question_request(const QList<QString> new_question);
    void send_created_test_with_zeros_request();
    void send_created_test_without_zeros_request();

signals:
    void reply_received(CustomTypes::RequestType request_type, QByteArray reply_array);
};

#endif // RESTAPICLIENT_H
