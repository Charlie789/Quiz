#ifndef QUIZAPPLICATION_H
#define QUIZAPPLICATION_H

#include <QObject>
#include <QApplication>
#include "restapiclient.h"

class QuizApplication : public QApplication
{
    Q_OBJECT
public:
    QuizApplication(int &argc, char **argv);

private:
    RestApiClient m_restapiclient;
};

#endif // QUIZAPPLICATION_H
