#ifndef QUIZAPPLICATION_H
#define QUIZAPPLICATION_H

#include <QObject>
#include <QApplication>
#include "restapiclient.h"
#include "mainwindow.h"

class QuizApplication : public QApplication
{
    Q_OBJECT
public:
    QuizApplication(int &argc, char **argv);

private:
    MainWindow w;
    RestApiClient m_restapiclient;
};

#endif // QUIZAPPLICATION_H
