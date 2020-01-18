#include "quizapplication.h"

QuizApplication::QuizApplication(int &argc, char **argv) :
    QApplication(argc, argv)
{
    w.showMaximized();

    connect(&m_restapiclient, &RestApiClient::reply_received, &m_reply_handler, &ReplyHandler::reply_received);
    connect(&m_reply_handler, &ReplyHandler::model_ready, &w, &MainWindow::set_models);
    connect(&m_reply_handler, &ReplyHandler::model_ready, &m_generate_teacher_test, &GenerateTeachTest::set_model);
    connect(&m_reply_handler, &ReplyHandler::question_variant_ready, &m_restapiclient, &RestApiClient::send_add_question_variant_request);

    connect(&w, &MainWindow::category_push_button_clicked, &m_restapiclient, &RestApiClient::send_category_request);
    connect(&w, &MainWindow::test_push_button_clicked, &m_restapiclient, &RestApiClient::send_test_request);
    connect(&w, &MainWindow::add_category_push_button_clicked, &m_restapiclient, &RestApiClient::send_add_category_request);
    connect(&w, &MainWindow::question_variant_push_button_clicked, &m_restapiclient, &RestApiClient::send_question_request);
    connect(&w, &MainWindow::request_answers_for_question, &m_restapiclient, &RestApiClient::send_answer_for_question_request);
    connect(&w, &MainWindow::category_updated, &m_restapiclient, &RestApiClient::send_update_category_request);
    connect(&w, &MainWindow::add_question_push_button_clicked, &m_restapiclient, &RestApiClient::send_add_question_request);
    connect(&w, &MainWindow::without_zeros_push_button_clicked, &m_restapiclient, &RestApiClient::send_number_of_created_test_without_zeros_request);
    connect(&w, &MainWindow::with_zeros_push_button_clicked, &m_restapiclient, &RestApiClient::send_number_of_created_test_with_zeros_request);
    //connect(&w, &MainWindow::generate_teacher_test_clicked, &m_generate_teacher_test, &GenerateTeachTest::generate_document);
    connect(&w, &MainWindow::generate_teacher_test_clicked, &m_restapiclient, &RestApiClient::send_created_test_with_zeros_request);
}
