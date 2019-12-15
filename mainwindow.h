#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "customtypes.h"

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

private slots:
    void on_test_push_button_clicked();
    void on_category_push_button_clicked();
    void on_add_category_push_button_clicked();
    void on_question_variant_push_button_clicked();
    void on_variant_question_push_button_clicked();
    void edit_category(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

signals:
    void test_push_button_clicked();
    void category_push_button_clicked();
    void add_category_push_button_clicked(const QString category_name);
    void question_variant_push_button_clicked();
    void request_answers_for_question(const QString question_id);
    void category_updated(const QString category_id, const QString category_name);

private:
    Ui::MainWindow *ui;

    QStandardItemModel* m_category_model;
    QStandardItemModel* m_test_model;
    QStandardItemModel* m_question_model;
    QStandardItemModel* m_answer_for_question_model;
};
#endif // MAINWINDOW_H
