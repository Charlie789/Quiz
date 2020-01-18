#ifndef GENERATE_TEACHER_TEST_H
#define GENERATE_TEACHER_TEST_H

#include <QObject>
#include <QStandardItemModel>
#include "customtypes.h"
#include <QTextDocument>

class GenerateTeachTest : public QObject
{
    Q_OBJECT
public:
    explicit GenerateTeachTest(QObject *parent = nullptr);

    QTextDocument document;

public slots:
    void set_model(QStandardItemModel* model, CustomTypes::RequestType request_type);
    void generate_full_report();
    void generate_teacher_with_question_report();
    void generate_teacher_without_question_report();

private:
    QStandardItemModel* m_model_teacher_test;

    void generate_document();
    QString generate_header();
    QString generate_frame();
    QString generate_teacher();
    QString generate_teacher_without_test();

};

#endif // GENERATE_TEACHER_TEST_H
