#include "quizapplication.h"

QuizApplication::QuizApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
    w.show();

    connect(&m_restapiclient, &RestApiClient::reply_received, &m_reply_handler, &ReplyHandler::reply_received);
    connect(&w, &MainWindow::category_push_button_clicked, &m_restapiclient, &RestApiClient::send_category_request);
    connect(&w, &MainWindow::test_push_button_clicked, &m_restapiclient, &RestApiClient::send_test_request);
    connect(&m_reply_handler, &ReplyHandler::model_ready, &w, &MainWindow::set_models);
    connect(&w, &MainWindow::add_category_push_button_clicked, &m_restapiclient, &RestApiClient::send_add_category_request);
}
