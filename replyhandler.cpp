#include "replyhandler.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

ReplyHandler::ReplyHandler(QObject *parent) : QObject(parent)
{
}

void ReplyHandler::reply_received(CustomTypes::RequestType request_type, QByteArray reply_array)
{
    qDebug() << "Odpowiedź z testami";
    QJsonDocument json_response = QJsonDocument::fromJson(reply_array);
    QJsonObject json_object = json_response.object();
    QJsonArray jsonArray = json_object["results"].toArray();
    QJsonObject obj = jsonArray[0].toObject();
    QJsonArray column_names = obj["columns"].toArray();
    QJsonArray rows = obj["rows"].toArray();
    qDebug() <<  column_names;
    qDebug() << rows;
    QJsonArray val = rows[2].toArray();
    qDebug() << val[0];
    switch(request_type){
    case CustomTypes::RequestTest:
    {
        qDebug() << rows.count();
        break;
    }
    case CustomTypes::RequestCategory:
    {
        m_category_model.clear();
        for (int row_index = 0; row_index < rows.count(); row_index++){
            QJsonArray val = rows[row_index].toArray();
            m_category_model.appendRow(new QStandardItem(val[1].toString()));
        }
        emit model_ready(&m_category_model, CustomTypes::RequestCategory);
        break;
    }
    default:
        break;
    }
}
