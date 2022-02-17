#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QGraphicsScene>

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

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    struct
    {
        int N = 0;
        QPointF *arr = NULL;
        double coef = 1.0;
    } data;
};
#endif // MAINWINDOW_H
