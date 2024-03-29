#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct connection_t
{
    int p1;
    int p2;
};

struct watch_t
{
    std::vector<QPoint> points;
    std::vector<connection_t> connet;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void showEvent(QShowEvent *ev);
    void resizeEvent(QResizeEvent *event);

private slots:
    void app_info_show();
    void author_info_show();
    void exit_show();
    void init_points();
    void init_watch();
    void drawing_axis(QPainter *p);
    void to_abs_coor(int x, int y, int *res_x, int *res_y);
    void from_abs_coor(int x, int y, int *res_x, int *res_y);
    void drawing_whatch();
    void print_warning(QString str);
    void print_succses(QString str);
    void windowShown();
    void on_pushButton_scaling_clicked();
    void zoom(int cx, int cy, double kx, double ky);
    void on_pushButton_shift_clicked();
    void move(int dx, int dy);
    void on_pushButton_turn_clicked();
    void rotate(int cx, int cy, int angle);
    void on_pushButton_clear_2_clicked();

    void on_pushButton_cancel_2_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    struct watch_t data;
    int min_x, max_x;
    int min_y, max_y;
    std::stack <struct watch_t> cancel;
};
#endif // MAINWINDOW_H
