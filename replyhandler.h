#ifndef REPLYHANDLER_H
#define REPLYHANDLER_H

#include <QObject>
#include "CustomTypes.h"
#include <QByteArray>

class ReplyHandler : public QObject
{
    Q_OBJECT
public:
    explicit ReplyHandler(QObject *parent = nullptr);

signals:

public slots:
    void reply_received(CustomTypes::RequestType request_type, QByteArray reply_array);
};

#endif // REPLYHANDLER_H
