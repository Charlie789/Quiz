#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "customtypes.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void set_models(QStandardItemModel *model, CustomTypes::RequestType request_type);
    void create_chart();

private slots:
    void on_test_push_button_clicked();
    void on_category_push_button_clicked();
    void on_add_category_push_button_clicked();
    void on_question_variant_push_button_clicked();
    void on_variant_question_push_button_clicked();
    void edit_category(const QModelIndex &topLeft, const QModelIndex, const QVector<int> = QVector<int>());
    void on_add_question_push_button_clicked();
    void on_add_question_to_db_push_button_clicked();
    void on_generate_chart_pushbutton_clicked();
    void on_chart_with_zeros_radio_clicked();
    void on_chart_without_zeros_radio_clicked();
    void on_text_reports_pushbutton_clicked();
    void on_generate_teacher_raport_pushbutton_clicked();

signals:
    void test_push_button_clicked();
    void category_push_button_clicked();
    void add_category_push_button_clicked(const QString category_name);
    void question_variant_push_button_clicked();
    void request_answers_for_question(const QString question_id);
    void category_updated(const QString category_id, const QString category_name);
    void add_question_push_button_clicked(const QList<QString> new_question);
    void with_zeros_push_button_clicked();
    void without_zeros_push_button_clicked();
    void generate_full_teacher_report();
    void generate_teacher_with_question_report();
    void generate_teacher_without_question_report();

private:
    Ui::MainWindow *ui;

    QStandardItemModel* m_category_model;
    QStandardItemModel* m_test_model;
    QStandardItemModel* m_question_model;
    QStandardItemModel* m_answer_for_question_model;
    QStandardItemModel* m_created_tests_model;
    QtCharts::QChartView* chartView;
};
#endif // MAINWINDOW_H
