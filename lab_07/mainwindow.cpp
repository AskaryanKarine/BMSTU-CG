#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // пункты меню
    QAction* AboutProgAction = ui->menubar->addAction("О программе");
    connect(AboutProgAction, SIGNAL(triggered()), this, SLOT(app_info_show()));
    QAction* AboutAuthorAction = ui->menubar->addAction("Об авторе");
    connect(AboutAuthorAction, SIGNAL(triggered()), this, SLOT(author_info_show()));
    QAction* ExitAction = ui->menubar->addAction(("Выход"));
    connect(ExitAction, SIGNAL(triggered()), this, SLOT(exit_show()));

    // начальные цвета
    data.cut_color = QColor("#55ffff");
    data.line_color = Qt::black;
    data.visible_color = QColor("#ff5500");

    show_color(data.cut_color, ui->label_cc);
    show_color(data.line_color, ui->label_lc);
    show_color(data.visible_color, ui->label_vlc);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// информационные функции
void MainWindow::app_info_show()
{
    QMessageBox::information(NULL, "О программе", "Реализация и исследование простого алгоритма отсечения отрезка регулярным отсекателем.\n\n\
Интерфейс этой программы был разработан с использванием ресурсов с сайта Flaticon.com\n\n\
This cover has been designed using resources from Flaticon.com");
}

void MainWindow::author_info_show()
{
    QMessageBox::information(NULL, "Об авторе", " Программа написана студенткой \nАскарян Каринэ из группы ИУ7-42Б\n +7(916)888-02-20");
}

void MainWindow::exit_show()
{
    QMessageBox msg_quit;
    msg_quit.setText("Работа с программой будет завершена");
    msg_quit.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg_quit.setIcon(QMessageBox::Warning);
    msg_quit.setDefaultButton(QMessageBox::Ok);
    msg_quit.setWindowTitle("Предупреждение!");
    int rc = msg_quit.exec();

    if (rc == QMessageBox::Ok)
        qApp->quit();
}

void MainWindow::error_message(QString str)
{
    QMessageBox::critical(NULL, "Ошибка", str);
}

// работа с цветом
void MainWindow::show_color(QColor color, QLabel* lab)
{
    QImage im = QImage(lab->geometry().width(), lab->geometry().height(), QImage::Format_RGB32);
    QPainter p(&im);
    p.setBrush(QBrush(color));
    p.setPen(Qt::black);
    QRect rect = QRect(0, 0, lab->geometry().width(), lab->geometry().height());
    p.drawRect(rect);

    QPixmap pixmap = QPixmap::fromImage(im);
    lab->clear();
    lab->setPixmap(pixmap);
}

void MainWindow::color_dialog(QColor& color)
{
    QColorDialog dialog;
    dialog.setCurrentColor(color);
    dialog.show();
    dialog.exec();
    QColor tmp = dialog.selectedColor();
    if (!tmp.isValid())
        error_message("Выберите цвет");
    else
        color = tmp;
}

void MainWindow::on_pushButton_line_color_clicked()
{
    color_dialog(data.line_color);
    show_color(data.line_color, ui->label_lc);
}

void MainWindow::on_pushButton_cut_color_clicked()
{
    color_dialog(data.cut_color);
    show_color(data.cut_color, ui->label_cc);
}

void MainWindow::on_pushButton_visible_line_color_clicked()
{
    color_dialog(data.visible_color);
    show_color(data.visible_color, ui->label_vlc);
}
