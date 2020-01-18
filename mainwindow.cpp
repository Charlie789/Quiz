#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_category_model(nullptr)
    , m_test_model(nullptr)
{
    ui->setupUi(this);
    ui->generate_chart_pushbutton->setEnabled(false);
    ui->chart_widget->setLayout(new QVBoxLayout);
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
        connect(m_category_model, &QStandardItemModel::dataChanged, this, &MainWindow::edit_category);
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
    case CustomTypes::RequestQuestion:
        m_question_model = model;
        ui->variant_question_list_view->setModel(m_question_model);
        break;
    case CustomTypes::RequestAnswerForQuestion:
        m_answer_for_question_model = model;
        for (int answer = 0; answer < m_answer_for_question_model->rowCount(); ++answer) {
            qDebug() << m_answer_for_question_model->data(m_answer_for_question_model->index(answer,0));
        }
        break;
    case CustomTypes::RequestCreatedTestWithoutZerosRequest:
    case CustomTypes::RequestCreatedTestWithZerosRequest:
        m_created_tests_model = model;
        create_chart();
        break;
    default:
        break;
    }

}

void MainWindow::create_chart()
{
    if(chartView)
        ui->chart_widget->layout()->removeWidget(chartView);
    QtCharts::QPieSeries *series = new QtCharts::QPieSeries();
    for (int row = 0; row < m_created_tests_model->rowCount(); row++){
        series->append(m_created_tests_model->data(m_created_tests_model->index(row, 0)).toString()
                       + " " + m_created_tests_model->data(m_created_tests_model->index(row, 1)).toString()
                       , m_created_tests_model->data(m_created_tests_model->index(row, 2)).toDouble());
    }

    for (int slices = 0; slices < series->slices().count(); slices++){
        QtCharts::QPieSlice *slice = series->slices().at(slices);
        if(slice->value() > 0)
            slice->setLabelVisible();
    }

    QtCharts::QChart *chart = new QtCharts::QChart();
    chart->addSeries(series);
    chart->setTitle("Wykres utworzonych pytań");
    chart->legend()->setAlignment(Qt::AlignLeft);

    chartView = new QtCharts::QChartView(chart, ui->chart_widget);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chart_widget->layout()->addWidget(chartView);
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

void MainWindow::on_question_variant_push_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(3);
    emit question_variant_push_button_clicked();
}

void MainWindow::on_variant_question_push_button_clicked()
{
    QModelIndexList selected = ui->variant_question_list_view->selectionModel()->selectedIndexes();
    QString question_id = selected.at(0).data(Qt::UserRole+1).toString();
    emit request_answers_for_question(question_id);
}

void MainWindow::edit_category(const QModelIndex &topLeft, const QModelIndex, const QVector<int>)
{
    emit category_updated(topLeft.data(Qt::UserRole + 1).toString(), topLeft.data().toString());
}

void MainWindow::on_add_question_push_button_clicked()
{
    ui->stacked_widget->setCurrentIndex(2);

}

void MainWindow::on_add_question_to_db_push_button_clicked()
{
    QList<QString> new_question;
    QModelIndexList selected = ui->category_list_view->selectionModel()->selectedIndexes();
    if (selected.isEmpty())
        return;
    new_question << selected.at(0).data(Qt::UserRole+1).toString();
    new_question << ui->question_text_edit->toPlainText();
    new_question << ui->correct_answer_line_edit->text();
    new_question << ui->wrong_answer1_line_edit->text();
    new_question << ui->wrong_answer2_line_edit->text();
    new_question << ui->wrong_answer3_line_edit->text();
    for (int i = 0; i < new_question.count(); ++i){
        if (new_question[i].isEmpty())
            return;
    }
    ui->question_text_edit->clear();
    ui->correct_answer_line_edit->clear();
    ui->wrong_answer1_line_edit->clear();
    ui->wrong_answer2_line_edit->clear();
    ui->wrong_answer3_line_edit->clear();
    emit add_question_push_button_clicked(new_question);
}

void MainWindow::on_generate_chart_pushbutton_clicked()
{
    if(ui->chart_with_zeros_radio->isChecked())
        emit with_zeros_push_button_clicked();
    if(ui->chart_without_zeros_radio->isChecked())
        emit without_zeros_push_button_clicked();
}

void MainWindow::on_chart_with_zeros_radio_clicked()
{
    if(!ui->generate_chart_pushbutton->isEnabled())
        ui->generate_chart_pushbutton->setEnabled(true);
}

void MainWindow::on_chart_without_zeros_radio_clicked()
{
    if(!ui->generate_chart_pushbutton->isEnabled())
        ui->generate_chart_pushbutton->setEnabled(true);
}
