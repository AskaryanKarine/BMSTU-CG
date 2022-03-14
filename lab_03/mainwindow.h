#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <stack>

enum method_t
{
    STANDART,
    DDA,
    BRESEN_DOUBLE,
    BRESEN_INT,
    BRESEN_STEPS,
    WY
};

struct line_t
{
    QPoint start;
    QPoint end;
    QColor color;
    method_t method;
};

struct spec_t
{
    QPoint center;
    double angle;
    double r;
    QColor color;
    method_t method;
};

struct content_t
{
    std::vector<line_t> lines;
    std::vector<spec_t> specteres;
    QColor back;
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
    void on_pushButton_back_color_clicked();
    void app_info_show();
    void author_info_show();
    void exit_show();
    void print_warning(QString str);
    void print_succses(QString str);
    void show_color(QColor color, QLabel *button);
    void on_pushButton_line_color_clicked();

    void on_pushButton_line_clicked();

    void on_pushButton_beam_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    content_t data;
    QColor back_color = Qt::white;
    QColor line_color = Qt::black;
    std::stack<content_t> cancel;

};
#endif // MAINWINDOW_H
