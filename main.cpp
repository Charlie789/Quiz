#include "mainwindow.h"

#include "quizapplication.h"

int main(int argc, char *argv[])
{
    QuizApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
