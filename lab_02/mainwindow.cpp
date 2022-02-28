#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <vector>
#include <iostream>
#include <math.h>
#include <QTimer>

#define BLACK QColor(0, 0, 0)
#define BLUE QColor(0, 191, 255)
#define FIELD 0.05
#define MAX 300
#define MIN -300

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QAction *AboutProgAction = ui->menubar->addAction("О программе");
    connect(AboutProgAction, SIGNAL(triggered()), this, SLOT(app_info_show()));
    QAction *AboutAuthorAction = ui->menubar->addAction("Об авторе");
    connect(AboutAuthorAction, SIGNAL(triggered()), this, SLOT(author_info_show()));
    QAction *ExitAction = ui->menubar->addAction(("Выход"));
    connect(ExitAction, SIGNAL(triggered()), this, SLOT(exit_show()));

    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    min_x = min_y = MIN;
    max_x = max_y = MAX;
    init_watch();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
}

// информационные функции
void MainWindow::app_info_show()
{
    QMessageBox::information(this, "О программе","Нарисовать исходный рисунок, \nзатем его переместить,\n\
промасштабировать, повернуть");
}

void MainWindow::author_info_show()
{
    QMessageBox::information(this, "Об авторе", " Программа написана студенткой \nАскарян Каринэ из группы ИУ7-42Б\n +7(916)888-02-20");
}

void MainWindow::exit_show()
{
    QMessageBox::warning(this, "Предупреждние!", "Работа с программой будет завершена");
    qApp->quit();
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    QTimer::singleShot(500, this, SLOT(windowShown()));
}

void MainWindow::windowShown()
{
    drawing_whatch();
}

// выводят информацию
void MainWindow::print_warning(QString str)
{
    ui->textEdit_2->setTextColor(QColor(194, 24, 7)); // red
    ui->textEdit_2->setText(str);
    ui->textEdit_2->setTextColor(QColor(0, 0, 0)); // black
}

void MainWindow::print_succses(QString str)
{
    ui->textEdit_2->setTextColor(QColor(24, 134, 45)); // green
    ui->textEdit_2->setText(str);
    ui->textEdit_2->setTextColor(QColor(0, 0, 0)); // black
}

void MainWindow::to_abs_coor(int x, int y, int *res_x, int *res_y)
{
    int fx = (max_x - min_x) * FIELD;
    int fy = (max_y - min_y) * FIELD;
    int nmax_x = max_x + fx;
    int nmin_x = min_x - fx;
    int nmax_y = max_y + fy;
    int nmin_y = min_y - fy;

    double xk = (double) (nmax_x - nmin_x) / ui->graphicsView->width();
    double xb = nmin_x;
    double yk = (double) (nmax_y - nmin_y) / ui->graphicsView->height();
    double yb = nmin_y;

    *res_x = x * xk + xb;
    *res_y = y * yk + yb;
}

// перевод из абсолютных координат
void MainWindow::from_abs_coor(int x, int y, int *res_x, int *res_y)
{
    int fx = (max_x - min_x) * FIELD;
    int fy = (max_y - min_y) * FIELD;
    int nmax_x = max_x + fx;
    int nmin_x = min_x - fx;
    int nmax_y = max_y + fy;
    int nmin_y = min_y - fy;

    double xk = (double) ui->graphicsView->width() / (nmax_x - nmin_x);
    double xb = -xk * nmin_x;
    double yk = (double) ui->graphicsView->height() / (nmax_y - nmin_y);
    double yb = -yk * nmin_y;

    *res_x = x * xk + xb;
    *res_y = y * yk + yb;
}

void MainWindow::drawing_axis(QPainter *p)
{
    p->setBrush(BLACK);
    p->setPen(BLACK);

    int rx, ry;
    from_abs_coor(0, 0, &rx, &ry);

    int h = ui->graphicsView->height()-5;
    int w = ui->graphicsView->width()-5;

    // Oy
    p->drawLine(rx, 0, rx, h);
    p->drawLine(rx, h, rx - 5,  h - 5);
    p->drawLine(rx, h, rx + 5,  h - 5);

    // Ox
    p->drawLine(0, ry, w, ry);
    p->drawLine(w, ry, w - 5,  ry - 5);
    p->drawLine(w, ry, w - 5,  ry + 5);
}

void MainWindow::init_points()
{
    data.points.push_back(QPoint(98, 128));
    data.points.push_back(QPoint(221, 128));
    data.points.push_back(QPoint(264, 179)); // 3
    data.points.push_back(QPoint(264, 278));
    data.points.push_back(QPoint(221, 326));
    data.points.push_back(QPoint(98, 326));  // 6
    data.points.push_back(QPoint(52, 278));
    data.points.push_back(QPoint(52,179));
    data.points.push_back(QPoint(98,108));   // 9
    data.points.push_back(QPoint(221, 108));
    data.points.push_back(QPoint(283,178));
    data.points.push_back(QPoint(283, 278)); // 12
    data.points.push_back(QPoint(221, 345));
    data.points.push_back(QPoint(98,345));
    data.points.push_back(QPoint(34, 278));  // 15
    data.points.push_back(QPoint(34, 178));
    data.points.push_back(QPoint(85, 80));
    data.points.push_back(QPoint(234, 80));  // 18
    data.points.push_back(QPoint(85, 374));
    data.points.push_back(QPoint(234, 374));
    data.points.push_back(QPoint(231, 80));  // 21
    data.points.push_back(QPoint(88, 80));
    data.points.push_back(QPoint(88,60));
    data.points.push_back(QPoint(231, 60));  // 24
    data.points.push_back(QPoint(229, 60));
    data.points.push_back(QPoint(89, 60));
    data.points.push_back(QPoint(229, 46));  // 27
    data.points.push_back(QPoint(89, 46));
    data.points.push_back(QPoint(92, 46));
    data.points.push_back(QPoint(225, 46));  // 30
    data.points.push_back(QPoint(225, 34));
    data.points.push_back(QPoint(92, 34));
    data.points.push_back(QPoint(95, 34));  // 33
    data.points.push_back(QPoint(222, 34));
    data.points.push_back(QPoint(222,27));
    data.points.push_back(QPoint(95, 27));  // 36
    data.points.push_back(QPoint(99, 27));
    data.points.push_back(QPoint(99,24));
    data.points.push_back(QPoint(219, 27));  // 39
    data.points.push_back(QPoint(219, 24));
    data.points.push_back(QPoint(229, 376));
    data.points.push_back(QPoint(90, 376));  // 42
    data.points.push_back(QPoint(90, 393));
    data.points.push_back(QPoint(229, 393));
    data.points.push_back(QPoint(92, 393));  // 45
    data.points.push_back(QPoint(226, 393));
    data.points.push_back(QPoint(226, 408));
    data.points.push_back(QPoint(92, 408));  // 48
    data.points.push_back(QPoint(94, 408));
    data.points.push_back(QPoint(223, 408));
    data.points.push_back(QPoint(223, 422)); // 51
    data.points.push_back(QPoint(94, 422));
    data.points.push_back(QPoint(96, 422));
    data.points.push_back(QPoint(222, 422)); // 54
    data.points.push_back(QPoint(222, 430));
    data.points.push_back(QPoint(96, 430));
    data.points.push_back(QPoint(98, 430));  // 57
    data.points.push_back(QPoint(219, 430));
    data.points.push_back(QPoint(219, 433));
    data.points.push_back(QPoint(98, 433));  // 60
    data.points.push_back(QPoint(159, 240));
    data.points.push_back(QPoint(147, 229));
    data.points.push_back(QPoint(160, 135)); // 63
    data.points.push_back(QPoint(171, 229));
    data.points.push_back(QPoint(159, 218));
    data.points.push_back(QPoint(250, 230)); // 66
    data.points.push_back(QPoint(284, 217));
    data.points.push_back(QPoint(296, 217));
    data.points.push_back(QPoint(296, 241)); // 69
    data.points.push_back(QPoint(284, 241)); // 70

    for (size_t i = 0; i < data.points.size(); i++)
    {
        data.points[i].setX(data.points[i].x() - 167);
        data.points[i].setY(data.points[i].y() - 226);
    }
//        data.points[i] -= QPoint(167, 226);
}

void MainWindow::init_watch()
{
    init_points();
    // 1 8-угольник
    data.connet.push_back({1, 2});
    data.connet.push_back({2, 3});
    data.connet.push_back({3, 4});
    data.connet.push_back({4, 5});
    data.connet.push_back({5, 6});
    data.connet.push_back({6, 7});
    data.connet.push_back({7, 8});
    data.connet.push_back({8, 1});
    // 2ой 8-угольник
    data.connet.push_back({9, 10});
    data.connet.push_back({10, 11});
    data.connet.push_back({11, 12});
    data.connet.push_back({12, 13});
    data.connet.push_back({13, 14});
    data.connet.push_back({14, 15});
    data.connet.push_back({15, 16});
    data.connet.push_back({16, 9});
    //верх корпуса
    data.connet.push_back({16, 17});
    data.connet.push_back({17, 9});
    data.connet.push_back({17, 18});
    data.connet.push_back({18, 11});
    data.connet.push_back({10, 11});
    // низ корпуса
    data.connet.push_back({14, 19});
    data.connet.push_back({19, 15});
    data.connet.push_back({19, 20});
    data.connet.push_back({13, 20});
    data.connet.push_back({20, 12});
    // колесико
    data.connet.push_back({67, 68});
    data.connet.push_back({68, 69});
    data.connet.push_back({69, 70});
    // ремешок верх
    //1 ур
    data.connet.push_back({22, 23});
    data.connet.push_back({23, 24});
    data.connet.push_back({24, 21});
    //2 ур
    data.connet.push_back({26, 28});
    data.connet.push_back({28, 27});
    data.connet.push_back({27, 25});
    //3ур
    data.connet.push_back({29, 32});
    data.connet.push_back({32, 31});
    data.connet.push_back({31, 30});
    //4ур
    data.connet.push_back({33, 36});
    data.connet.push_back({35, 34});
    data.connet.push_back({36, 35});
    //5ур
    data.connet.push_back({37, 38});
    data.connet.push_back({39, 40});
    data.connet.push_back({38, 40});
    //ремешок низ
    // 1ур
    data.connet.push_back({42, 43});
    data.connet.push_back({41, 44});
    data.connet.push_back({43, 44});
    // 2ур
    data.connet.push_back({45, 48});
    data.connet.push_back({48, 47});
    data.connet.push_back({46, 47});
    //3ур
    data.connet.push_back({49, 52});
    data.connet.push_back({50, 51});
    data.connet.push_back({52, 51});
    //4ур
    data.connet.push_back({53, 56});
    data.connet.push_back({54, 55});
    data.connet.push_back({55, 56});
    //5ур
    data.connet.push_back({57, 60});
    data.connet.push_back({58, 59});
    data.connet.push_back({60, 59});
    // нижняя стрелка
    data.connet.push_back({62, 65});
    data.connet.push_back({65, 66});
    data.connet.push_back({66, 61});
    data.connet.push_back({61, 62});
    // верхняя стрелка
    data.connet.push_back({62, 63});
    data.connet.push_back({63, 64});
    data.connet.push_back({64, 61});
}

void MainWindow::drawing_whatch()
{
    QImage image = QImage(ui->graphicsView->geometry().width(), ui->graphicsView->geometry().height(), QImage::Format_RGB32);
    QPainter p(&image);
    image.fill(QColor(255, 255, 255));
    drawing_axis(&p);
    p.setBrush(BLUE);
    p.setPen(QPen(BLUE, 2));

    for (size_t i = 0; i < data.connet.size(); i++)
    {
        int x1 = data.points[data.connet[i].p1 - 1].x();
        int y1 = data.points[data.connet[i].p1 - 1].y();

        int x2 = data.points[data.connet[i].p2 - 1].x();
        int y2 = data.points[data.connet[i].p2 - 1].y();

        int real_x1, real_y1;
        from_abs_coor(x1, y1, &real_x1, &real_y1);

        int real_x2, real_y2;
        from_abs_coor(x2, y2, &real_x2, &real_y2);

        p.drawLine(real_x1, real_y1, real_x2, real_y2);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);

}

void MainWindow::zoom(int cx, int cy, double kx, double ky)
{
    for (size_t i = 0; i < data.points.size(); i++)
        {
            int x = data.points[i].x();
            int y = data.points[i].y();
            double x1 = kx * x + (1 - kx) * cx;
            double y1 = ky * y + (1 - ky) * cy;
            data.points[i].setX(x1);
            data.points[i].setY(y1);
        }
        drawing_whatch();
}

void MainWindow::on_pushButton_scaling_clicked()
{
    QString str_x_center = ui->lineEdit_2->text();
    QString str_y_center = ui->lineEdit_3->text();
    QString str_kx = ui->lineEdit_4->text();
    QString str_ky = ui->lineEdit_5->text();

    if (str_y_center.length() == 0 || str_y_center.length() == 0 || str_kx.length() == 0 || str_ky.length() == 0)
        print_warning("Ошибка ввода: пустой ввод");
    else
    {
        bool flag_x_center, flag_y_center, flag_kx, flag_ky;
        int x_center, y_center;
        double kx, ky;
        x_center = str_x_center.toInt(&flag_x_center);
        y_center = str_y_center.toInt(&flag_y_center);
        kx = str_kx.toDouble(&flag_kx);
        ky = str_ky.toDouble(&flag_ky);
        if (!flag_x_center || !flag_y_center || !flag_kx || !flag_ky)
            print_warning("Ошибка ввода: некорректный ввод");
        else
        {
            // пушить в стек для отмены
            zoom(x_center, y_center, kx, ky);
            ui->lineEdit_2->clear();
            ui->lineEdit_3->clear();
            ui->lineEdit_4->clear();
            ui->lineEdit_5->clear();
            print_succses("Операция упешна: часы изменили свой размер");
        }

    }

}

void MainWindow::move(int dx, int dy)
{
    for (size_t i = 0; i < data.points.size(); i++)
    {
        int x1 = data.points[i].x() + dx;
        int y1 = data.points[i].y() + dy;
        data.points[i].setX(x1);
        data.points[i].setY(y1);
    }
    drawing_whatch();
}

void MainWindow::on_pushButton_shift_clicked()
{
    QString str_dx = ui->lineEdit_6->text();
    QString str_dy = ui->lineEdit_7->text();

    if (str_dx.length() == 0 || str_dy.length() == 0)
        print_warning("Ошибка ввода: пустой ввод");
    else
    {
        bool flag_dx, flag_dy;
        int dx, dy;
        dx = str_dx.toInt(&flag_dx);
        dx = str_dy.toInt(&flag_dy);
        if (!flag_dx || !flag_dy)
            print_warning("Ошибка ввода: некорректный ввод");
        else
        {
            // пушить в стек для отмены
            move(dx, dy);
            ui->lineEdit_6->clear();
            ui->lineEdit_7->clear();
            print_succses("Операция упешна: часы куда-то ушли");
        }

    }
}

void MainWindow::rotate(int cx, int cy, int angle)
{
    for (size_t i = 0; i < data.points.size(); i++)
    {
        int x = data.points[i].x();
        int y = data.points[i].y();
        double rad = (double) angle * ((double) 3.141592 / 180.0);
        double x1 = cx + (double) (x - cx) * cos(rad) + (y - cy) * sin(rad);
        double y1 = cy - (double) (x - cx) * sin(rad) + (y - cy) * cos(rad);
        data.points[i].setX(x1);
        data.points[i].setY(y1);
    }
    drawing_whatch();
}

void MainWindow::on_pushButton_turn_clicked()
{
    QString str_x_center = ui->lineEdit_8->text();
    QString str_y_center = ui->lineEdit_9->text();
    QString str_angle = ui->lineEdit_10->text();

    if (str_y_center.length() == 0 || str_y_center.length() == 0 || str_angle.length() == 0)
        print_warning("Ошибка ввода: пустой ввод");
    else
    {
        bool flag_x_center, flag_y_center, flag_angle;
        int x_center, y_center, angle;
        x_center = str_x_center.toInt(&flag_x_center);
        y_center = str_y_center.toInt(&flag_y_center);
        angle = str_angle.toDouble(&flag_angle);
        if (!flag_x_center || !flag_y_center || !flag_angle)
            print_warning("Ошибка ввода: некорректный ввод");
        else
        {
            // пушить в стек для отмены
            rotate(x_center, y_center, angle);
            ui->lineEdit_8->clear();
            ui->lineEdit_9->clear();
            ui->lineEdit_10->clear();
            print_succses("Операция упешна: вращаются обычно стрелки, а не сами часы, но ладно..");
        }

    }
}

void MainWindow::on_pushButton_clear_2_clicked()
{
    data.points.clear();
    data.connet.clear();
    init_watch();
    drawing_whatch();
    print_succses("Успешно очищено");
}
