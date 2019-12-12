#include "quizapplication.h"

QuizApplication::QuizApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
    w.show();

    connect(&w, &MainWindow::category_push_button_clicked, &m_restapiclient, &RestApiClient::send_category_request);
    connect(&w, &MainWindow::test_push_button_clicked, &m_restapiclient, &RestApiClient::send_test_request);
}
