#include "replyhandler.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlTableModel>

ReplyHandler::ReplyHandler(QObject *parent) : QObject(parent)
{
    std::srand(time(nullptr));
}

void ReplyHandler::reply_received(CustomTypes::RequestType request_type, QByteArray reply_array)
{
    QJsonDocument json_response = QJsonDocument::fromJson(reply_array);
    QJsonObject json_object = json_response.object();
    QJsonArray jsonArray = json_object["results"].toArray();
    QJsonObject obj = jsonArray[0].toObject();
    QJsonArray column_names = obj["columns"].toArray();
    qDebug() << column_names;
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
            QStandardItem* item = new QStandardItem(val[1].toString());
            item->setData(val[0].toString());
            m_category_model.appendRow(item);
        }
        emit model_ready(&m_category_model, CustomTypes::RequestCategory);
        break;
    }
    case CustomTypes::RequestQuestion:
    {
        m_question_model.clear();
        for (int row_index = 0; row_index < rows.count(); row_index++){
            QJsonArray val = rows[row_index].toArray();
            QList<QStandardItem *> rowData;
            rowData << new QStandardItem(val[1].toString());
            rowData << new QStandardItem(val[2].toString());
            rowData << new QStandardItem(val[3].toString());
            rowData << new QStandardItem(val[0].toString());
            rowData.at(0)->setData(val[0].toString());
            m_question_model.appendRow(rowData);
        }
        m_question_model.setHorizontalHeaderItem(0, new QStandardItem("Treść pytania"));
        m_question_model.setHorizontalHeaderItem(1, new QStandardItem("Kategoria"));
        m_question_model.setHorizontalHeaderItem(2, new QStandardItem("Nauczyciel"));
        m_question_model.setHorizontalHeaderItem(3, new QStandardItem("ID"));
        emit model_ready(&m_question_model, CustomTypes::RequestQuestion);
        break;
    }
    case CustomTypes::RequestAnswerForQuestion:
    {
        QString id[4];
        QString question_id;
        for (int row_index = 0; row_index < rows.count(); row_index++){
            QJsonArray val = rows[row_index].toArray();
            id[row_index] = val[0].toString();
            question_id = val[1].toString();

        }
        int num[4]={0},i,j;
        bool check;
        for(i=0;i<4;i++)
        {
            check=true;
            do
            {
                num[i]=std::rand() % 4;

                for(j=0;j<i;j++)
                {
                    if( num[i]==num[j]){    // checks whether number already exists in  the array
                        check=false;
                        break;
                    } else
                        check=true;
                }
            } while(check==false);
        }
        QString answer_order;
        for (int row_index = 0; row_index < rows.count(); row_index++){
            answer_order += id[num[row_index]];
            if(row_index < rows.count() - 1)
                answer_order += ",";

        }
        qDebug() << answer_order;
        emit question_variant_ready(question_id, answer_order);
        break;
    }
    case CustomTypes::RequestCreatedTestWithZerosRequest:
    {
        m_created_test_model.clear();
        for (int row_index = 0; row_index < rows.count(); row_index++){
            QJsonArray val = rows[row_index].toArray();
            QList<QStandardItem *> rowData;
            rowData << new QStandardItem(val[0].toString());
            rowData << new QStandardItem(val[1].toString());
            rowData << new QStandardItem(val[2].toString());
            m_created_test_model.appendRow(rowData);
        }
        m_created_test_model.setHorizontalHeaderItem(0, new QStandardItem("Nazwisko"));
        m_created_test_model.setHorizontalHeaderItem(1, new QStandardItem("Imię"));
        m_created_test_model.setHorizontalHeaderItem(2, new QStandardItem("Utworzone testy"));
        emit model_ready(&m_created_test_model, CustomTypes::RequestCreatedTestWithZerosRequest);
        break;
    }
    case CustomTypes::RequestCreatedTestWithoutZerosRequest:
    {
        m_created_test_model.clear();
        for (int row_index = 0; row_index < rows.count(); row_index++){
            QJsonArray val = rows[row_index].toArray();
            QList<QStandardItem *> rowData;
            rowData << new QStandardItem(val[0].toString());
            rowData << new QStandardItem(val[1].toString());
            rowData << new QStandardItem(val[2].toString());
            m_created_test_model.appendRow(rowData);
        }
        m_created_test_model.setHorizontalHeaderItem(0, new QStandardItem("Nazwisko"));
        m_created_test_model.setHorizontalHeaderItem(1, new QStandardItem("Imię"));
        m_created_test_model.setHorizontalHeaderItem(2, new QStandardItem("Utworzone testy"));
        emit model_ready(&m_created_test_model, CustomTypes::RequestCreatedTestWithoutZerosRequest);
        break;
    }
    default:
        break;
    }
}
