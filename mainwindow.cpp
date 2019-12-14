#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_category_model(nullptr)
    , m_test_model(nullptr)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_models(QStandardItemModel* model, CustomTypes::RequestType request_type)
{
    switch(request_type){
    case CustomTypes::RequestCategory:
    {
        m_category_model = model;
        ui->category_list_view->setModel(m_category_model);
        break;
    }
    case CustomTypes::RequestTest:
    {
        m_test_model = model;
        ui->tableView->setModel(m_test_model);
        ui->tableView->setColumnWidth(0, 250);
        ui->tableView->horizontalHeader()->setStretchLastSection(true);
        break;
    }
    default:
        break;
    }

}

void MainWindow::on_test_push_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(0);
    emit test_push_button_clicked();
}

void MainWindow::on_category_push_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(1);
    emit category_push_button_clicked();
}

void MainWindow::on_add_category_push_button_clicked()
{
    emit add_category_push_button_clicked(ui->category_line_edit->text());
    ui->category_line_edit->clear();
}
