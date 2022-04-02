#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <stack>
#include "structurs.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool eventFilter(QObject *object, QEvent *event);
private slots:
    void app_info_show();
    void author_info_show();
    void windowShown();
    void exit_show();
    void show_color(QColor color, QLabel *lab);
    void error_message(QString str);
    void showEvent(QShowEvent *ev);
    void wheelEvent(QWheelEvent *event);
    void on_comboBox_figure_activated(int index);
    void on_pushButton_back_color_clicked();
    void on_pushButton_line_color_clicked();
    void on_pushButton_figure_clicked();
    void on_pushButton_reset_scale_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_spectrum_clicked();

    void on_pushButton_time_clicked();

private:
    Ui::MainWindow *ui;
    canvas_t scene;
    content_t data;
    QColor line_color = Qt::black;
    QColor back_color = Qt::white;
    std::stack<content_t> cancel;
};
#endif // MAINWINDOW_H
