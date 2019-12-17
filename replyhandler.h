#ifndef REPLYHANDLER_H
#define REPLYHANDLER_H

#include <QObject>
#include "customtypes.h"
#include <QByteArray>
#include <QStandardItemModel>

class ReplyHandler : public QObject
{
    Q_OBJECT
public:
    explicit ReplyHandler(QObject *parent = nullptr);

private:
    QStandardItemModel m_category_model;
    QStandardItemModel m_test_model;
    QStandardItemModel m_question_model;
    QStandardItemModel m_created_test_model;

signals:
    void model_ready(QStandardItemModel* model, CustomTypes::RequestType request_type);
    void question_variant_ready(const QString question_id, const QString answer_order);

public slots:
    void reply_received(CustomTypes::RequestType request_type, QByteArray reply_array);
};

#endif // REPLYHANDLER_H
