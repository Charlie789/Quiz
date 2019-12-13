#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_test_push_button_clicked()
{
    emit test_push_button_clicked();
}

void MainWindow::on_category_push_button_clicked()
{
    emit category_push_button_clicked();
}
