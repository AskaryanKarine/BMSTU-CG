#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButton_app_info_clicked();

    void on_pushButton_author_info_clicked();

    void on_pushButton_exit_clicked();

    void on_pushButton_add_dot_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
