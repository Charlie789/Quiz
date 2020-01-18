#ifndef GENERATE_TEACHER_TEST_H
#define GENERATE_TEACHER_TEST_H

#include <QObject>

class GenerateTeachTest : public QObject
{
    Q_OBJECT
public:
    explicit GenerateTeachTest(QObject *parent = nullptr);

    void generate_document();
    QString generate_header();
    QString generate_frame();
    QString generate_teacher();

signals:

};

#endif // GENERATE_TEACHER_TEST_H
