#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QLabel>
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

private slots:
    void app_info_show();
    void author_info_show();
    void exit_show();
    void error_message(QString str);
    void show_color(QColor color, QLabel *lab);
    void color_dialog(QColor &color);
    void on_pushButton_back_color_clicked();

    void on_pushButton_line_color_clicked();

    void on_pushButton_fill_color_clicked();

    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;
    canvas_t scene;
    QColor back_color = Qt::white;
    QColor line_color = Qt::black;
    QColor fill_color = Qt::black;
};
#endif // MAINWINDOW_H
