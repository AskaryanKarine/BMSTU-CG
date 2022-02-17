#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
//#include <QString>
//#include <iostream>

#define MAX_POINTS 100

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
}


void MainWindow::app_info_show()
{
    QMessageBox::information(this, "О программе","Задача на геометрические преобразования\n\n\
На плоскости задано множество из N точек.\nНайти две пересекающиеся окружности,\nплощадь которых будет минимальна.\n\
Вывести площадь области пересечения двух\nокружностей.\n");
}

void MainWindow::author_info_show()
{
    QMessageBox::information(this, "Об авторе", " Программа написана студенткой \nАскарян Каринэ из группы ИУ7-42Б");
}

void MainWindow::exit_show()
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
                ui->tableWidget->setColumnCount(2);
                ui->tableWidget->setRowCount(data.N);
                ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
                ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y");
                ui->textEdit->setTextColor(QColor(24, 134, 45));
                ui->textEdit->setText("Ввод успешен\n");
                if (data.arr != NULL)
                    delete [] data.arr;
                data.arr = new QPointF[data.N];
            }
        }
    }
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // выделение цветом
    ui->textEdit->setText(QString("click успешен, r - %1 c - %2 \n").arg(row).arg(column));
}

void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    bool ok;
    QString text = item->text();
    if (text.length() == 0)
    {
        ui->textEdit->setTextColor(QColor(194, 24, 7)); // red
        ui->textEdit->setText("Пустой ввод: Введите координату точки\n");
    }
    else
    {
        double coord = text.toDouble(&ok);
        if (!ok)
        {
            ui->textEdit->setTextColor(QColor(194, 24, 7)); // red
            ui->textEdit->setText("Ошибка ввола: введите целое или действительное число\n");
        }
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
            {
                ui->textEdit->setTextColor(QColor(194, 24, 7)); // red
                ui->textEdit->setText("Невозможно построить данную точку: введите обе координаты\n");
            }
            else
            {
                ui->textEdit->setTextColor(QColor(24, 134, 45)); // green
                ui->textEdit->setText("Ввод успешен\n");
            }
        }
    }


}
