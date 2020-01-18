#ifndef QUIZAPPLICATION_H
#define QUIZAPPLICATION_H

#include <QObject>
#include <QApplication>
#include "restapiclient.h"
#include "mainwindow.h"
#include <QByteArray>
#include "replyhandler.h"
#include "generateteachertest.h"

class QuizApplication : public QApplication
{
    Q_OBJECT
public:
    QuizApplication(int &argc, char **argv);

private:
    MainWindow w;
    RestApiClient m_restapiclient;
    ReplyHandler m_reply_handler;
    GenerateTeachTest m_generate_teacher_test;
};

#endif // QUIZAPPLICATION_H
