#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QGraphicsScene>
#include <stack>

struct points
{
    int N = 0;
    QPointF *arr = NULL;
    bool soluted = false;
    QPointF min_c1;
    QPointF min_c2;
    double min_r1, min_r2;
    double min_x, min_y;
    double max_x, max_y;
};

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

    void author_info_show();
    void app_info_show();
    void exit_show();
    void on_lineEdit_returnPressed();
    void on_tableWidget_cellClicked(int row, int column);
    void on_tableWidget_itemChanged(QTableWidgetItem *item);
    void mousePressEvent(QMouseEvent *event);
    void print_warning(QString str);
    void print_succses(QString str);
    void on_pushButton_del_dot_clicked();
    void on_pushButton_clear_clicked();
    void drawing_points(int sel);
    void on_pushButton_result_clicked();
    bool check_triangle(QPointF a, QPointF b, QPointF c);
    double find_center_radius(QPointF a, QPointF b, QPointF c, QPointF *center);
    void drawing_cirles(QPointF a, QPointF b, double r1, double r2, int sel);
    void on_pushButton_cancel_clicked();
    double find_square_between(double r1, double r2, QPointF c1, QPointF c2);
    void drawing_axis(QPainter *p);
    void from_abs_coor(double x, double y, double *res_x, double *res_y);
    void to_abs_coor(double x, double y, double *res_x, double *res_y);
private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    struct points data;
    std::stack <struct points> cancel;

};
#endif // MAINWINDOW_H
