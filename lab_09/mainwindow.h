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

    bool eventFilter(QObject* object, QEvent* event);
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
    void on_pushButton_add_point_clicked();
    void add_draw_point(const point& p);
    void push_cancel();
    void on_pushButton_cancel_clicked();
    void on_pushButton_clear_clicked();
    void mousePressEvent(QMouseEvent* event);
    void my_mouse_move_event(QMouseEvent* event);
    void on_pushButton_cut_clicked();
    void on_pushButton_reset_scale_clicked();

    void on_pushButton_cursor_mode_clicked();

    void on_pushButton_hand_mode_clicked();

    void on_pushButton_close_cut_clicked();

    void find_parallel_point(point &p);

private:
    Ui::MainWindow* ui;
    QGraphicsScene* scene;
    content data;
    std::stack<content> cancel;
    bool is_cut = true;
    bool process = false;
    bool is_hand = false;
};
#endif // MAINWINDOW_H
