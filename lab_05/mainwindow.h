#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColor>
#include <QLabel>
#include <stack>
#include <QTableWidgetItem>
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
    void exit_show();
    void error_message(QString str);
    void show_color(QColor color, QLabel *lab);
    void color_dialog(QColor &color);

    void on_pushButton_line_color_clicked();

    void on_pushButton_fill_color_clicked();

    void on_comboBox_activated(int index);

    void on_pushButton_add_point_clicked();

    void on_pushButton_close_clicked();

    void on_pushButton_add_hole_clicked();

    void on_pushButton_clear_clicked();

    void on_tableWidget_cellClicked(int row, int column);

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_reset_scale_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_del_point_clicked();

    void mousePressEvent(QMouseEvent *event);
    void on_pushButton_fill_clicked();

    void on_pushButton_hand_mode_clicked();

    void on_pushButton_cursor_mode_clicked();

private:
    Ui::MainWindow *ui;
    canvas_t scene;
    QColor back_color = Qt::white;
    QColor line_color = Qt::black;
    QColor fill_color = QColor("#ffa257");
    content data;
    std::stack<content> cancel;
    bool is_hole = false;
    int cnt = 0;
    bool is_hand = false;
};
#endif // MAINWINDOW_H
