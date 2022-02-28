#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

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
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    struct watch_t data;
    int min_x, max_x;
    int min_y, max_y;
};
#endif // MAINWINDOW_H
