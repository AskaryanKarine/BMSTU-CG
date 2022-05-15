#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "structurs.h"
#include <QColor>
#include <QLabel>
#include <QMainWindow>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void app_info_show();
    void author_info_show();
    void exit_show();
    void error_message(QString str);
    void show_color(QColor color, QLabel* lab);

    void color_dialog(QColor& color);
    void on_pushButton_line_color_clicked();

    void on_pushButton_cut_color_clicked();

    void on_pushButton_visible_line_color_clicked();

private:
    Ui::MainWindow* ui;
    content data;
    std::stack<content> cancel;
    //    QColor line_color = Qt::black;
    //    QColor visible_color = Qt::red;
    //    QColor cut_color = Qt::blue;
};
#endif // MAINWINDOW_H
