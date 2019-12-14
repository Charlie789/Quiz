#include "replyhandler.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlTableModel>

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
    switch(request_type){
    case CustomTypes::RequestTest:
    {
        m_test_model.clear();
        for (int row_index = 0; row_index < rows.count(); row_index++){
            QJsonArray val = rows[row_index].toArray();
            QList<QStandardItem *> rowData;
            rowData << new QStandardItem(val[1].toString());
            rowData << new QStandardItem(val[2].toString());
            m_test_model.appendRow(rowData);
        }
        m_test_model.setHorizontalHeaderItem(0, new QStandardItem("Data utworzenia"));
        m_test_model.setHorizontalHeaderItem(1, new QStandardItem("Nazwa testu"));
        emit model_ready(&m_test_model, CustomTypes::RequestTest);
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
