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

signals:
    void test_push_button_clicked();
    void category_push_button_clicked();
    void add_category_push_button_clicked(const QString category_name);

private:
    Ui::MainWindow *ui;

    QStandardItemModel* m_category_model;
    QStandardItemModel* m_test_model;
};
#endif // MAINWINDOW_H
