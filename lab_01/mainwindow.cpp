#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <iostream>
#include <algorithm>
#include <QtMath>
#include <QBrush>
#include <QPen>
#include <QImage>
#include <QPainter>

#define MAX_POINTS 100
#define COEF 4
#define BLACK QColor(0, 0, 0)
#define GRAY QColor(50, 50, 50)
#define BLUE QColor(0, 191, 255)
#define RED QColor(200,0, 0)
#define GREEN QColor(0, 200, 0)
#define FIELD 0.05
#define MAX 20

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // создание сцены
    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);

    // Настройка пунктов меню
    QAction *AboutProgAction = ui->menubar->addAction("О программе");
    connect(AboutProgAction, SIGNAL(triggered()), this, SLOT(app_info_show()));
    QAction *AboutAuthorAction = ui->menubar->addAction("Об авторе");
    connect(AboutAuthorAction, SIGNAL(triggered()), this, SLOT(author_info_show()));
    QAction *ExitAction = ui->menubar->addAction(("Выход"));
    connect(ExitAction, SIGNAL(triggered()), this, SLOT(exit_show()));

    // отключение скролла
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //
    data.min_x = data.min_y = 0;
    data.max_x = data.max_y = MAX;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete [] data.arr;
    delete scene;
}

// информационные функции
void MainWindow::app_info_show()
{
    QMessageBox::information(this, "О программе","Задача на геометрические преобразования\n\n\
На плоскости задано множество из N точек.\nНайти две пересекающиеся окружности,\nсуммарная площадь которых будет минимальна.\n\
Вывести площадь области пересечения двух\nокружностей.\n");
}

void MainWindow::author_info_show()
{
    QMessageBox::information(this, "Об авторе", " Программа написана студенткой \nАскарян Каринэ из группы ИУ7-42Б");
}

void MainWindow::exit_show()
{
    QMessageBox::warning(this, "Предупреждние!", "Работа с программой будет завершена");
    qApp->quit();
}

// выводят информацию
void MainWindow::print_warning(QString str)
{
    ui->textEdit->setTextColor(QColor(194, 24, 7)); // red
    ui->textEdit->setText(str);
    ui->textEdit->setTextColor(QColor(0, 0, 0)); // black
}

void MainWindow::print_succses(QString str)
{
    ui->textEdit->setTextColor(QColor(24, 134, 45)); // green
    ui->textEdit->setText(str);
    ui->textEdit->setTextColor(QColor(0, 0, 0)); // black
}

// ввод кол-ва точек
void MainWindow::on_lineEdit_returnPressed()
{
    bool ok;
    QString text_N = ui->lineEdit->text();
    if (text_N.length() == 0)
        print_warning("Пустой ввод: Введите количество точек\n");
    else
    {
        data.N = text_N.toInt(&ok);
        if (!ok)
            print_warning("Ошибка ввода: Введите целое число точек\n");
        else
        {
            if (data.N <= 0)
                print_warning("Ошибка ввода: Введите целое число точек\n");
            else
            {
                ui->tableWidget->setColumnCount(2);
                ui->tableWidget->setRowCount(data.N);
                ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                print_succses("Ввод успешен\n");
                if (data.arr != NULL)
                    delete [] data.arr;
                data.arr = new QPointF[data.N];
                for (int i = 0; i < data.N; i++)
                {
                    data.arr[i].setX(qQNaN());
                    data.arr[i].setY(qQNaN());
                }
                on_pushButton_clear_clicked();
                ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");
            }
        }
    }
}

// функции масштабирования
// перевод в абсолютные координаты
void MainWindow::to_abs_coordinates(double x, double y, double *res_x, double *res_y)
{
    int fx = (data.max_x - data.min_x) * FIELD;
    int fy = (data.max_y - data.min_y) * FIELD;
    int nmax_x = data.max_x + fx;
    int nmin_x = data.min_x - fx;
    int nmax_y = data.max_y + fy;
    int nmin_y = data.min_y - fy;

    double xk = (double) (nmax_x - nmin_x) / ui->graphicsView->width();
    double xb = nmin_x;
    double yk = (double) (nmax_y - nmin_y) / ui->graphicsView->height();
    double yb = nmin_y;

    *res_x = x * xk + xb;
    *res_y = y * yk + yb;
}

// перевод из абсолютных координат
void MainWindow::from_abs_coordinates(double x, double y, double *res_x, double *res_y)
{
    int fx = (data.max_x - data.min_x) * FIELD;
    int fy = (data.max_y - data.min_y) * FIELD;
    int nmax_x = data.max_x + fx;
    int nmin_x = data.min_x - fx;
    int nmax_y = data.max_y + fy;
    int nmin_y = data.min_y - fy;

    double xk = (double) ui->graphicsView->width() / (nmax_x - nmin_x);
    double xb = -xk * nmin_x;
    double yk = (double) ui->graphicsView->height() / (nmax_y - nmin_y);
    double yb = -yk * nmin_y;

    *res_x = x * xk + xb;
    *res_y = y * yk + yb;
}

//функции рисования

void MainWindow::drawing_axis(QPainter *p)
{
    p->setBrush(BLACK);
    p->setPen(BLACK);

    double rx, ry;
    from_abs_coordinates(0, 0, &rx, &ry);

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

// функция отрисовки точек (без окружностей)
void MainWindow::drawing_points(int sel = -1)
{
    QImage image = QImage(ui->graphicsView->geometry().width(), ui->graphicsView->geometry().height(), QImage::Format_RGB32);
    QPainter p(&image);
    image.fill(QColor(255,255,255));
    drawing_axis(&p);

    p.setBrush(GRAY);
    p.setPen(GRAY);

    for (int i = 0; i < data.N; i++)
    {
        double rx, ry;
        from_abs_coordinates(data.arr[i].x(), data.arr[i].y(), &rx, &ry);
        p.drawEllipse(rx, ry, COEF, COEF);
    }

    if (sel >= 0)
    {
        p.setBrush(BLUE);
        p.setPen(BLUE);
        double rx, ry;
        from_abs_coordinates(data.arr[sel].x(), data.arr[sel].y(), &rx, &ry);
        p.drawEllipse(rx, ry, COEF, COEF);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}

// отрисовка точек + окружности
void MainWindow::drawing_cirles(QPointF a, QPointF b, double r1, double r2, int sel = -1)
{
    QImage image = QImage(600, 600, QImage::Format_RGB32);
    QPainter p(&image);

    image.fill(QColor(255,255,255));
    drawing_axis(&p);

    QPen pen = QPen(RED);
    pen.setWidth(COEF);
    p.setBrush(QColor(0, 0, 0, 0));
    p.setPen(pen);
    p.drawEllipse(a, r1, r1);
    p.setBrush(QColor(0, 0, 0, 0));
    pen.setColor(GREEN);
    p.setPen(pen);
    p.drawEllipse(b, r2, r2);

    p.setBrush(GRAY);
    p.setPen(GRAY);
    for (int i = 0; i < data.N; i++)
    {
        double rx, ry;
        from_abs_coordinates(data.arr[i].x(), data.arr[i].y(), &rx, &ry);
        p.drawEllipse(rx, ry, COEF, COEF);
    }

    if (sel >= 0)
    {
        p.setBrush(BLUE);
        p.setPen(BLUE);
        double rx, ry;
        from_abs_coordinates(data.arr[sel].x(), data.arr[sel].y(), &rx, &ry);
        p.drawEllipse(rx, ry, COEF, COEF);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}

// функции ввода точки
// ввод точки в таблицу
void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item) // есть заглушки
{
    bool ok;
    QString text = item->text();
    if (text.length() == 0)
        print_warning("Пустой ввод: Введите координату точки\n");
    else
    {
        double coord = text.toDouble(&ok);
        if (!ok)
            print_warning("Ошибка ввода: введите целое или действительное число\n");
        else
        {
            int row = ui->tableWidget->row(item);
            int col = ui->tableWidget->column(item);

            QTableWidgetItem *second_item;
            if (col == 0)
            {
                data.arr[row].setX(coord);
                second_item = ui->tableWidget->item(row, 1);
                if (coord > data.max_x)
                    data.max_x = coord;
                else if (coord < data.min_x)
                    data.min_x = coord;
            }
            if (col == 1)
            {
                data.arr[row].setY(coord);
                second_item = ui->tableWidget->item(row, 0);
                if (coord > data.max_y)
                    data.max_y = coord;
                else if (coord < data.min_y)
                    data.min_y = coord;
            }
            if (!second_item)
                print_warning("Невозможно построить данную точку: введите обе координаты\n");
            else
            {
                drawing_points();
            }
        }
    }
}

// ввод точки мышкой
void MainWindow::mousePressEvent(QMouseEvent *event) // есть заглушки
{
    int flag = 1;
    QRect view = ui->graphicsView->geometry();
    if (view.contains(event->pos()))
    {
        for (int i = 0; i < data.N && flag; i++)
        {
            QTableWidgetItem *item_x = ui->tableWidget->item(i, 0);
            QTableWidgetItem *item_y = ui->tableWidget->item(i, 1);
            if (!item_x && !item_y)
            {
                flag = 0;
                double ax, ay;
                to_abs_coordinates(event->pos().x() - view.x(), event->pos().y() - view.y() - menuBar()->geometry().height(), &ax, &ay);
                ax = int(ax * 100 + 0.5) / 100.0;
                ay = int(ay * 100 + 0.5) / 100.0;
                QPointF point = QPointF(ax, ay);
                if (std::find(data.arr, data.arr + data.N, point) != (data.arr + data.N))
                    print_warning("Эта точка уже существует");
                else
                {
                    cancel.push(data);
                    ui->tableWidget->blockSignals(true);
                    data.arr[i].setX(ax);
                    data.arr[i].setY(ay);
                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(data.arr[i].x())));
                    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(data.arr[i].y())));

                    if (data.arr[i].x() > data.max_x)
                        data.max_x = data.arr[i].x();
                    else if (data.arr[i].x() < data.min_x)
                        data.min_x = data.arr[i].x();

                    if (data.arr[i].y() > data.max_y)
                        data.max_y = data.arr[i].y();
                    else if (data.arr[i].y() < data.min_y)
                        data.min_y = data.arr[i].y();

                    print_succses("Ввод точки успешен");
                    drawing_points();
                    ui->tableWidget->blockSignals(false);
                }
            }
        }
        if (flag)
            print_warning("Нельзя поставить точку: превышено максимальное количество");
    }
}

// функции взаимодействия с пользователем
// выделение точки цветом
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    if (data.soluted)
        drawing_cirles(data.min_c1, data.min_c2, data.min_r1, data.min_r2, row);
    else
        drawing_points(row);
}

// удаление точки
void MainWindow::on_pushButton_del_dot_clicked()
{
    cancel.push(data);
    QTableWidgetItem *cur_item = ui->tableWidget->currentItem();
    if (cur_item)
    {
        int row = ui->tableWidget->row(cur_item);
        ui->tableWidget->removeRow(row);
        ui->tableWidget->insertRow(row);
        data.arr[row].setX(qQNaN());
        data.arr[row].setY(qQNaN());
        drawing_points();
    }
    else
        print_warning("Выберите точку");
}

// очистка всего поля
void MainWindow::on_pushButton_clear_clicked()
{
    if (data.N > 0)
    {
        ui->tableWidget->clear();
        ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");
        if (data.arr)
        {
            delete [] data.arr;
            data.arr = new QPointF[data.N];
            for (int i = 0; i < data.N; i++)
            {
                data.arr[i].setX(qQNaN());
                data.arr[i].setY(qQNaN());
            }
            data.min_x = data.min_y = 0;
            data.max_x = data.max_y = MAX;
            drawing_points();
            cancel.clear();
        }
    }
}

// отмена действия
void MainWindow::on_pushButton_cancel_clicked()
{
    if (cancel.empty())
        print_warning("Нельзя отменить действие");
    else
    {
        data = cancel.pop();
//        for (int i = 0; i < data.N; i++)
//        {
//            if (qIsNaN(data.arr[i].x()))
//                print_warning(QString("i = %1 aaa").arg(i));
//                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(data.arr[i].x())));
//            else
//                ui->tableWidget->setItem(i, 0, new QTableWidgetItem(" "));
//            if (!qIsNaN(data.arr[i].y()))
//                print_warning("aaa");
//                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(data.arr[i].y())));
//            else
//                ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString(" ")));
//        }
        if (data.soluted)
            drawing_cirles(data.min_c1, data.min_c2, data.min_r1, data.min_r2);
        else
            drawing_points();
    }
}

// функции для решения задачи

// нахождение координат центр и радиуса окружности
double MainWindow::find_center_radius(QPointF a, QPointF b, QPointF c, QPointF *center)
{
    /*
    bc = (xb*xb + yb*yb - xc*xc - yc*yc)
    ca = (xc*xc + yc*yc - xa*xa - ya*ya)
    ab = (xa*xa + ya*ya - xb*xb - yb*yb)
    d = 2(xa(yb-yc) + xb(yc-ya) + xc(ya-yb))

    x0 = ya*bc + yb*ca + yc*ab / -d
    y0 = xa*bc + xb*ca + xc*ab / d
    */

    double ab = a.x() * a.x() + a.y() * a.y() - b.x() * b.x() - b.y() * b.y();
    double bc = b.x() * b.x() + b.y() * b.y() - c.x() * c.x() - c.y() * c.y();
    double ca = c.x() * c.x() + c.y() * c.y() - a.x() * a.x() - a.y() * a.y();

    double d = 2.0 * (a.x() * (b.y() - c.y()) + b.x() * (c.y() - a.y()) + c.x() * (a.y() - b.y()));

    center->setX((a.y() * bc + b.y() * ca + c.y() * ab) / (-d));
    center->setY((a.x() * bc + b.x() * ca + c.x() * ab) / d);

    // нахождение  радиуса
    bc = sqrt((b.x() - c.x()) * (b.x() - c.x()) + (b.y() - c.y()) * (b.y() - c.y()));
    ca = sqrt((c.x() - a.x()) * (c.x() - a.x()) + (c.y() - a.y()) * (c.y() - a.y()));
    ab = sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
    double p = 0.5 * (ab + bc + ca);

    double rad = (ab * bc * ca) / (4 * sqrt(p * (p - ab) * (p - bc) * (p - ca)));
    return rad;
}

// нахождение площади между окружностями
double MainWindow::find_square_between(double r1, double r2, QPointF c1, QPointF c2)
{
    double d = sqrt((c2.x() - c1.x()) * (c2.x() - c1.x()) + (c2.y() - c1.y()) * (c2.y() - c1.y()));
    double f1 = 2 * acos((r1 * r1 - r2 * r2 + d * d) / (2 * r1 * d));
    double f2 = 2 * acos((r2 * r2 - r1 * r1 + d * d) / (2 * r2 * d));
    double s1 = (r1 * r1 * (f1 - sin(f1))) / 2.0;
    double s2 = (r2 * r2 * (f2 - sin(f2))) / 2.0;

    return s1 + s2;
}

// првоерка треугольника на вырожденность
bool MainWindow::check_triangle(QPointF a, QPointF b, QPointF c)
{
    double bc = sqrt((b.x() - c.x()) * (b.x() - c.x()) + (b.y() - c.y()) * (b.y() - c.y()));
    double ca = sqrt((c.x() - a.x()) * (c.x() - a.x()) + (c.y() - a.y()) * (c.y() - a.y()));
    double ab = sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));

    if ((ab < (bc + ca)) || (bc < (ab + ca) || ca < (ab + bc)))
        return true;
    return false;
}

// результат
void MainWindow::on_pushButton_result_clicked()
{
    QPointF a1, b1, c1, center1; // 3 точки первой окружности
    QPointF a2, b2, c2, center2; // 3 точки второй окружности
    double r1, square1;
    double r2, square2;
    double min_sum = -1.0;
    data.min_r1 = -1.0;
    data.min_r2 = -1.0;

    QPointF acenter1, acenter2, min_ac1, min_ac2;
    double ar1, ar2, min_ar1, min_ar2;
    QPointF a, b, c;

    double new_x, new_y;

    for (int i = 0; i < data.N - 2; i++)
    {
        a1 = data.arr[i];
        for (int j = i + 1; j < data.N - 1; j++)
        {
            b1 = data.arr[j];
            for (int m = j + 1; m < data.N; m++)
            {
                c1 = data.arr[m];
                if (check_triangle(a1, b1, c1))
                {
                    r1 = find_center_radius(a1, b1, c1, &center1);
                    square1 = M_PI * r1 * r1;

                    from_abs_coordinates(a1.x(), a1.y(), &new_x, &new_y);
                    a = QPointF(new_x, new_y);
                    from_abs_coordinates(b1.x(), b1.y(), &new_x, &new_y);
                    b = QPointF(new_x, new_y);
                    from_abs_coordinates(c1.x(), c1.y(), &new_x, &new_y);
                    c = QPointF(new_x, new_y);
                    ar1 = find_center_radius(a, b, c, &acenter1);

                    for (int i1 = 1; i1 < data.N - 2; i1++)
                    {
                        a2 = data.arr[i1];
                        for (int j1 = i1 + 1; j1 < data.N - 1; j1++)
                        {
                            b2 = data.arr[j1];
                            for (int m1 = j1 + 1; m1 < data.N; m1++)
                            {
                                c2 = data.arr[m1];
                                if (check_triangle(a2, b2, c2))
                                {
                                    r2 = find_center_radius(a2, b2, c2, &center2);
                                    square2 = M_PI * r2 * r2;

                                    from_abs_coordinates(a2.x(), a2.y(), &new_x, &new_y);
                                    a = QPointF(new_x, new_y);
                                    from_abs_coordinates(b2.x(), b2.y(), &new_x, &new_y);
                                    b = QPointF(new_x, new_y);
                                    from_abs_coordinates(c2.x(), c2.y(), &new_x, &new_y);
                                    c = QPointF(new_x, new_y);
                                    ar2 = find_center_radius(a, b, c, &acenter2);

                                    if (center1 != center2 && r1 != r2)
                                    {
                                        double d = (center1.x() - center2.x()) * (center1.x() - center2.x()) + (center1.y() - center2.y()) * (center1.y() - center2.y());
                                        if ((r1 + r2) > sqrt(d))
                                        {
                                            if ((min_sum < 0) || (min_sum > 0 && min_sum > (square1 + square2)))
                                            {
                                                min_sum = square1 + square2;
                                                data.min_c1 = center1;
                                                data.min_c2 = center2;
                                                data.min_r1 = r1;
                                                data.min_r2 = r2;
                                                min_ar1 = ar1;
                                                min_ar2 = ar2;
                                                min_ac1 = acenter1;
                                                min_ac2 = acenter2;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    if (data.min_r1 < 0 || data.min_r2 < 0)
        print_warning("Окружностей, удовлетоворяющих условию задачи нет");
    else
    {
        cancel.push(data);
        data.soluted = true;
        drawing_cirles(min_ac1, min_ac2, min_ar1, min_ar2);
        ui->textEdit->setText(QString("Площадь пересечения окружностей: %1").arg(find_square_between(data.min_r1, data.min_r2, data.min_c1, data.min_c2)));
        data.min_c1 = min_ac1;
        data.min_c2 = min_ac2;
        data.min_r1 = min_ar1;
        data.min_r2 = min_ar2;
    }
}
