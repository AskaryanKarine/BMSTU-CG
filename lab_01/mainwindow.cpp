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

