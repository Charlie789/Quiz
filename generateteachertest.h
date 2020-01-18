#ifndef GENERATE_TEACHER_TEST_H
#define GENERATE_TEACHER_TEST_H

#include <QObject>
#include <QStandardItemModel>
#include "customtypes.h"

class GenerateTeachTest : public QObject
{
    Q_OBJECT
public:
    explicit GenerateTeachTest(QObject *parent = nullptr);

    void set_model(QStandardItemModel* model, CustomTypes::RequestType request_type);
    void generate_document();
    QString generate_header();
    QString generate_frame();
    QString generate_teacher();

signals:

private:
    QStandardItemModel* m_model_teacher_test;

};

#endif // GENERATE_TEACHER_TEST_H
