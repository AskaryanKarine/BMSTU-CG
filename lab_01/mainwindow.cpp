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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete [] data.arr;
    delete scene;
}


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
                ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");
                print_succses("Ввод успешен\n");
                if (data.arr != NULL)
                    delete [] data.arr;
                data.arr = new QPointF[data.N];
                for (int i = 0; i < data.N; i++)
                {
                    data.arr[i].setX(qQNaN());
                    data.arr[i].setY(qQNaN());
                }
            }
        }
    }
}

// функция отрисовки
void MainWindow::drawing_points()
{
    QImage image = QImage(600, 600, QImage::Format_RGB32);
    QPainter p(&image);
    p.setBrush(QColor(0,0,0));
    p.setPen(QColor(0,0,0));
    image.fill(QColor(255,255,255));
//    pixmap.fill();
    for (int i = 0; i < data.N; i++)
    {
        if (data.arr[i].x() != qQNaN())
            p.drawEllipse(data.arr[i].x(), data.arr[i].y(), data.coef * 2, data.coef * 2);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}


void MainWindow::on_tableWidget_cellClicked(int row, int column) // заглушка
{

    // выделение точки цветом
}

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
            }
            if (col == 1)
            {
                data.arr[row].setY(coord);
                second_item = ui->tableWidget->item(row, 0);
            }
            if (!second_item)
                print_warning("Невозможно построить данную точку: введите обе координаты\n");
            else
            {
                print_succses(QString("x = %1 y = %2\n").arg(data.arr[row].x()).arg(data.arr[row].y())); // заглушка
                drawing_points();
                // сюда функцию отрисовки поля
            }
        }
    }
}

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
                // заглушка
                QPointF point = QPointF(event->pos().x() - view.x(), event->pos().y() - view.y() - menuBar()->geometry().height());
                if (std::find(data.arr, data.arr + data.N, point) != (data.arr + data.N))
                    print_warning("Эта точка уже существует");
                else
                {
                    // пересчитать координаты, рисовать точки
                    data.arr[i].setX(event->pos().x() - view.x());
                    data.arr[i].setY(event->pos().y() - view.y() - menuBar()->geometry().height());
                    ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(data.arr[i].x())));
                    ui->tableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(data.arr[i].y())));
                }
            }
        }
        if (flag)
            print_warning("Нельзя поставить точку: превышено максимальное количество");
    }
}

void MainWindow::on_pushButton_del_dot_clicked() // есть заглушка
{
    QTableWidgetItem *cur_item = ui->tableWidget->currentItem();
    if (cur_item)
    {
        int row = ui->tableWidget->row(cur_item);
        ui->tableWidget->removeRow(row);
        ui->tableWidget->insertRow(row);
        data.arr[row].setX(qQNaN());
        data.arr[row].setY(qQNaN());
        drawing_points();
        // перерисовать все поле снова
    }
    else
        print_warning("Выберите точку");
}

void MainWindow::on_pushButton_clear_clicked() // есть заглушка
{
    if (data.N > 0)
    {
        ui->tableWidget->clear();
        // очистить сцену
        if (data.arr)
        {
            delete [] data.arr;
            data.arr = new QPointF[data.N];
            for (int i = 0; i < data.N; i++)
            {
                data.arr[i].setX(qQNaN());
                data.arr[i].setY(qQNaN());
            }
            drawing_points();
        }
    }
}

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

    // semi perimetr
    bc = sqrt((b.x() - c.x()) * (b.x() - c.x()) + (b.y() - c.y()) * (b.y() - c.y()));
    ca = sqrt((c.x() - a.x()) * (c.x() - a.x()) + (c.y() - a.y()) * (c.y() - a.y()));
    ab = sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
    double p = 0.5 * (ab + bc + ca);

    double rad = (ab * bc * ca) / (4 * sqrt(p * (p - ab) * (p - bc) * (p - ca)));
    return rad;
}

bool MainWindow::check_triangle(QPointF a, QPointF b, QPointF c)
{
    double bc = sqrt((b.x() - c.x()) * (b.x() - c.x()) + (b.y() - c.y()) * (b.y() - c.y()));
    double ca = sqrt((c.x() - a.x()) * (c.x() - a.x()) + (c.y() - a.y()) * (c.y() - a.y()));
    double ab = sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));

    if ((ab < (bc + ca)) || (bc < (ab + ca) || ca < (ab + bc)))
        return true;
    return false;
}

void MainWindow::drawing_cirles(QPointF a, QPointF b, double r1, double r2)
{
    QImage image = QImage(600, 600, QImage::Format_RGB32);
    QPainter p(&image);
    image.fill(QColor(255,255,255));

    p.setBrush(QColor(200,0,0));
    p.setPen(QColor(200,0,0));
    p.drawEllipse(a, r1, r1);
    p.setBrush(QColor(0,200,0));
    p.setPen(QColor(0,200,0));
    p.drawEllipse(b, r2, r2);

    p.setBrush(QColor(0,0,0));
    p.setPen(QColor(0,0,0));
    for (int i = 0; i < data.N; i++)
    {
        if (data.arr[i].x() != qQNaN())
            p.drawEllipse(data.arr[i].x(), data.arr[i].y(), data.coef * 2, data.coef * 2);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}

void MainWindow::on_pushButton_result_clicked()
{
    QPointF a1, b1, c1, center1; // 3 точки первой окружности
    QPointF a2, b2, c2, center2; // 3 точки второй окружности
    double r1, square1;
    double r2, square2;
    double min_sum = -1.0;
    QPointF min_c1;
    QPointF min_c2;
    double min_r1, min_r2;

    if (data.N < 4)
        print_warning("Невозможно решить поставленую задачу, нужно больше точек");
    else
    {
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
                                        if (min_sum < 0)
                                        {
                                            min_sum = square1 + square2;
                                            min_c1 = center1;
                                            min_c2 = center2;
                                            min_r1 = r1;
                                            min_r2 = r2;
                                        }
                                        else if (min_sum > 0 && min_sum > (square1 + square2))
                                        {
                                            min_sum = square1 + square2;
                                            min_c1 = center1;
                                            min_c2 = center2;
                                            min_r1 = r1;
                                            min_r2 = r2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        drawing_cirles(min_c1, min_c2, min_r1, min_r2);
//        if (min_c1 == min_c2)
//            print_warning("Совпали");
        ui->textEdit->setText(QString("c1[%1, %2], r1 = %3, c2[%4,%5], r2 = %6").arg(min_c1.x()).arg(min_c1.y()).arg(min_r1).arg(min_c2.x()).arg(min_c2.y()).arg(min_r2));
    }
}





