#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
//#include <QString>
//#include <iostream>

#define MAX_POINTS 100

//struct
//{
//    QPoint data[MAX_POINTS];
//    int count = 0;
//} points;

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
    delete [] data.arr;
    delete model;
}

void MainWindow::on_pushButton_app_info_clicked()
{
    QMessageBox::information(this, "О программе","Задача на геометрические преобразования\n\n\
На плоскости задано множество из N точек.\nНайти две пересекающиеся окружности,\nплощадь которых будет минимальна.\n\
Вывести площадь области пересечения двух\nокружностей.\n");
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

void MainWindow::on_lineEdit_returnPressed()
{
    bool ok;
    QString text_N = ui->lineEdit->text();
    if (text_N.length() == 0)
    {
        ui->textEdit->setTextColor(QColor(194, 24, 7)); // red
        ui->textEdit->setText("Пустой ввод: Введите количество точек\n");
    }
    else
    {
        data.N = text_N.toInt(&ok);
        if (!ok)
        {
            ui->textEdit->setTextColor(QColor(194, 24, 7));
            ui->textEdit->setText("Ошибка ввода: Введите целое число точек\n");
        }
        else
        {
            if (data.N <= 0)
            {
                ui->textEdit->setTextColor(QColor(194, 24, 7)); //green
                ui->textEdit->setText("Ошибка ввода: Введите целое число точек\n");
            }
            else
            {
                model = new QStandardItemModel(data.N, 2, this);
                ui->tableView->setModel(model);
                model->setHeaderData(0, Qt::Horizontal, "x");
                model->setHeaderData(1, Qt::Horizontal, "y");
                ui->textEdit->setTextColor(QColor(24, 134, 45));
                ui->textEdit->setText("Ввод успешен\n");
                data.arr = new QPointF[data.N];
            }
        }
    }

}
