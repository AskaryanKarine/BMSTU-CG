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

private slots:
    void app_info_show();
    void author_info_show();
    void exit_show();
    void init_points();
    void init_watch();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    struct watch_t data;
    int min_x, max_x;
    int min_y, max_y;
};
#endif // MAINWINDOW_H
