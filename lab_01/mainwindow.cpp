#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    QApplication::quit();
}
