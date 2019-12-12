#include "replyhandler.h"
#include <QDebug>

ReplyHandler::ReplyHandler(QObject *parent) : QObject(parent)
{

}

void ReplyHandler::reply_received(CustomTypes::RequestType request_type, QByteArray reply_array)
{
    switch(request_type){
    case CustomTypes::RequestTest:
    {
        qDebug() << "Odpowiedź z testami";
        qDebug() << reply_array;
        break;
    }
    case CustomTypes::RequestCategory:
    {
        qDebug() << "Odpowiedź z kategoriami";
        qDebug() << reply_array;
        break;
    }
    default:
        break;
    }
}
