#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <vector>
#include <iostream>
#include <math.h>

#define BLACK QColor(0, 0, 0)
#define GRAY QColor(50, 50, 50)
#define BLUE QColor(0, 191, 255)
#define RED QColor(200,0, 0)
#define GREEN QColor(0, 200, 0)
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

    ui->graphicsView_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView_2->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    scene = new QGraphicsScene();
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
    data.points.push_back(QPoint(225,27));
    data.points.push_back(QPoint(95, 27));  // 36
    data.points.push_back(QPoint(99, 27));
    data.points.push_back(QPoint(99,24));
    data.points.push_back(QPoint(222, 27));  // 39
    data.points.push_back(QPoint(222, 24));
    data.points.push_back(QPoint(229, 376));
    data.points.push_back(QPoint(86, 376));  // 42
    data.points.push_back(QPoint(86, 393));
    data.points.push_back(QPoint(228, 393));
    data.points.push_back(QPoint(88, 393));  // 45
    data.points.push_back(QPoint(226, 393));
    data.points.push_back(QPoint(226, 408));
    data.points.push_back(QPoint(88, 408));  // 48
    data.points.push_back(QPoint(91, 408));
    data.points.push_back(QPoint(223, 408));
    data.points.push_back(QPoint(223, 422)); // 51
    data.points.push_back(QPoint(91, 422));
    data.points.push_back(QPoint(93, 422));
    data.points.push_back(QPoint(222, 422)); // 54
    data.points.push_back(QPoint(222, 430));
    data.points.push_back(QPoint(93, 430));
    data.points.push_back(QPoint(65, 430));  // 57
    data.points.push_back(QPoint(219, 430));
    data.points.push_back(QPoint(219, 433));
    data.points.push_back(QPoint(95, 433));  // 60
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
        data.points[i] -= QPoint(167, 226);
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
    data.connet.push_back({31, 35});
    data.connet.push_back({36, 35});
    //5ур
    data.connet.push_back({37, 38});
    data.connet.push_back({39, 40});
    data.connet.push_back({38, 40});
}
