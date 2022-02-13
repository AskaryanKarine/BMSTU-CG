#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QString>
#include <iostream>

#define MAX_POINTS 100

struct
{
    QPoint data[MAX_POINTS];
    int count = 0;
} points;

//QPoint points[MAX_POINTS];

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_app_info_clicked()
{
    QMessageBox::information(this, "О программе","Задача на геометрические преобразования\n\n\
На плоскости задано множество из N точек.\nНайти разбиение этого множества на два\nподмножества так, что суммарная площадь\
\nокружностей, представляющих эти множества,\nбудет минимальна, а сами окружности\nобязательно будут пересекаться. \
Вывести\nплощадь области пересечения двух окружностей.\n");
}

void MainWindow::on_pushButton_author_info_clicked()
{
    QMessageBox::information(this, "Об авторе", " Программа написана студенткой \nАскарян Каринэ из группы ИУ7-42Б");
}

void MainWindow::on_pushButton_exit_clicked()
{
    QMessageBox::warning(this, "Предупреждние!", "Работа с программой будет завершена");
    QApplication::quit();
}

void MainWindow::on_pushButton_add_dot_clicked()
{
    bool okX, okY;
    QPoint p;
    int tempX, tempY;
    QString mytext = ui->lineEdit_add->text();
    if (mytext.length() == 0)
        QMessageBox::warning(this, "Предупреждение!", "Пустой ввод\nВведите через пробел координаты точки x y");
    else
    {
        QStringList nums = mytext.split(" ");
        if (nums.length() != 2)
            QMessageBox::warning(this, "Предупреждение!", "Введены некорректные данные.\nВведите через пробел координаты точки x y");
        else
        {
            tempX = nums[0].toInt(&okX);
            tempY = nums[1].toInt(&okY);
            if (!okX || !okY)
                QMessageBox::warning(this, "Предупреждение!", "Введите два целых числа");
            else
            {
                p.setX(tempX);
                p.setY(tempY);
                points.data[points.count] = p;
                points.count++;
//                ui->label->setText(tr("x = %1 y = %2, len = %3").arg(points.data[points.count-1].x()).arg((points.data[points.count-1].y())).arg(points.count));
            }
        }
    }
//    ui->label->setText(tr("x = %1, y = %2").arg(p.x()).arg(p.y()));

}
