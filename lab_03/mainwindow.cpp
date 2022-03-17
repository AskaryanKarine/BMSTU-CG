#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QColor>
#include <QMessageBox>
#include <QWheelEvent>
#include <iostream>
#include <QTimer>
#include <QDrag>
#include <QMimeData>

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


    scene = new QGraphicsScene();
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->pushButton_cancel->setEnabled(false);

    data.back = back_color;
    show_color(back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);
    min.setX(0);
    min.setY(0);
    max.setX(ui->graphicsView->geometry().width());
    max.setX(ui->graphicsView->geometry().height());

    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    cancel = std::stack <content_t>();
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    QTimer::singleShot(500, this, SLOT(windowShown()));
}

void MainWindow::windowShown()
{
    drawing_content();
}

static void copy(struct content_t **a, struct content_t *b)
{
    for (size_t i = 0; i < b->lines.size(); i++)
        (*a)->lines.push_back(b->lines[i]);
    for (size_t i = 0; i < b->specteres.size(); i++)
        (*a)->specteres.push_back(b->specteres[i]);
    (*a)->back = b->back;
}

// информационные функции
void MainWindow::app_info_show()
{
    QMessageBox::information(NULL, "О программе","Реализация и исследование алгоритмов построения отрезков");
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

// вывод предупреждений
void MainWindow::print_warning(QString str)
{
    ui->textEdit->setTextColor(QColor(194, 24, 7)); // red
    ui->textEdit->setText(str);
    ui->textEdit->setTextColor(QColor(0, 0, 0)); // black
}

// вывод успешной информации
void MainWindow::print_succses(QString str)
{
    ui->textEdit->setTextColor(QColor(24, 134, 45)); // green
    ui->textEdit->setText(str);
    ui->textEdit->setTextColor(QColor(0, 0, 0)); // black
}

// функция события колесика мыши
void MainWindow::wheelEvent(QWheelEvent* event)
{
    ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scale_factor = 1.15;
    if (event->angleDelta().y() > 0)
        ui->graphicsView->scale(scale_factor, scale_factor);
    else
        ui->graphicsView->scale(1 / scale_factor, 1 / scale_factor);
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if (object == ui->graphicsView->viewport() && event->type() == QEvent::Wheel)
        return true;
    return false;
}

// событие нажатия мыши
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && ui->graphicsView->geometry().contains(event->pos()))
        ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

// функции рисования
// функция рисования ВСЕГО
void MainWindow::drawing_content()
{
    scene->clear();
    ui->graphicsView->setBackgroundBrush(back_color);
    drawing_axes();
}

// функция рисования осей
void MainWindow::drawing_axes()
{
    QPen pen = QPen(Qt::black);

    // Oy
    scene->addLine(0, -4, 0, 100, pen);
    scene->addLine(0, 100, 4, 85, pen);
    scene->addLine(0, 100, -4, 85, pen);
    // Y
    scene->addLine(-3, 105, 0, 110, pen);
    scene->addLine(0, 110, 3, 105, pen);
    scene->addLine(0, 110, 0, 114, pen);

    // Ox
    scene->addLine(-4, 0, 100, 0, pen);
    scene->addLine(100, 0, 85, 4, pen);
    scene->addLine(100, 0, 85, -4, pen);
    // X
    scene->addLine(105, -3, 109, 3, pen);
    scene->addLine(109, -3, 105, 3, pen);

}

// рисование линий
void drawing_line(line_t &line, canvas_t &p)
{
    switch (line.method)
    {
        case STANDART:
            break;
        case DDA:
            break;
        case BRESEN_INT:
            break;
        case BRESEN_DOUBLE:
            break;
        case BRESEN_STEPS:
            break;
        case WY:
            break;
    }
}

// функция рисования спектра

// показать цвет на лейбл
void MainWindow::show_color(QColor color, QLabel *lab)
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

// выбор цвета фона
void MainWindow::on_pushButton_back_color_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(back_color);
    dialog.show();
    dialog.exec();
    QColor color = dialog.selectedColor();
    if (!color.isValid())
        print_warning("Что-то пошло не так");
    else
        back_color = color;
    show_color(back_color, ui->label_bc);
    data.back = back_color;
}

// выбор цвета линий
void MainWindow::on_pushButton_line_color_clicked()
{
    QColorDialog dialog;
    dialog.setCurrentColor(line_color);
    dialog.show();
    dialog.exec();
    QColor color = dialog.selectedColor();
    if (!color.isValid())
        print_warning("Что-то пошло не так");
    else
        line_color = color;
    show_color(line_color, ui->label_lc);
}


void MainWindow::check_max_min(QPoint &point)
{
    if (max.x() < point.x())
        max.setX(point.x());

    if (max.y() < point.y())
        max.setY(point.y());

    if (min.x() > point.x())
        min.setX(point.x());

    if (min.y() > point.y())
        min.setY(point.y());
}

// построить линию
void MainWindow::on_pushButton_line_clicked() // дописать
{
    QString str_x_start = ui->lineEdit_line_x_start->text();
    QString str_y_start = ui->lineEdit_line_y_start->text();
    QString str_x_end = ui->lineEdit_line_x_end->text();
    QString str_y_end = ui->lineEdit_line_y_end->text();

    if (str_x_end.length() == 0 || str_x_start.length() == 0 || str_y_end.length() == 0 || str_y_start.length() == 0)
        print_warning("Ошибка ввода: пустой или неполный ввод");
    else
    {
        bool flag_x_start, flag_y_start, flag_x_end, flag_y_end;
        int x_start, y_start, x_end, y_end;
        x_start = str_x_start.toInt(&flag_x_start);
        y_start = str_y_start.toInt(&flag_y_start);
        x_end = str_x_end.toInt(&flag_x_end);
        y_end = str_y_end.toInt(&flag_y_end);
        if (!flag_x_start || !flag_x_end || !flag_y_start || !flag_y_end)
            print_warning("Ошибка ввода: Некорректный ввод");
        else
        {
            QPoint start, end;
            start = QPoint(x_start, y_start);
            end = QPoint(x_end, y_end);

            if (start == end)
                print_warning("Ошибка ввода: Точки начала и конца отрезка совпадают");
            else
            {
                check_max_min(start);
                check_max_min(end);
                content_t *c = new content_t;
                copy(&c, &data);
                cancel.push(*c);
                ui->pushButton_cancel->setEnabled(true);

                line_t line;
                line.color = line_color;
                line.method = (method_t) ui->comboBox->currentIndex();
                line.start = start;
                line.end = end;
                data.lines.push_back(line);
                drawing_content();
                // функция, которая строит отрезки
            }
        }
    }
}

// поистроить спектр
void MainWindow::on_pushButton_beam_clicked() // дописать
{
    QString str_beam_x = ui->lineEdit_beam_x->text();
    QString str_beam_y = ui->lineEdit_beam_y->text();
    QString str_beam_r = ui->lineEdit_beam_r->text();
    QString str_angle = ui->lineEdit_angle->text();

    if (str_beam_x.length() == 0 || str_beam_y.length() == 0 || str_beam_r.length() == 0 || str_angle.length() == 0)
        print_warning("Ошибка ввода: пустой или неполный ввод");
    else
    {
        bool flag_beam_x, flag_beam_y, flag_beam_r, flag_angle;
        int beam_x, beam_y;
        double beam_r, angle;

        beam_x = str_beam_x.toInt(&flag_beam_x);
        beam_y = str_beam_y.toInt(&flag_beam_y);
        beam_r = str_beam_r.toDouble(&flag_beam_r);
        angle = str_angle.toDouble(&flag_angle);

        if (!beam_x || !beam_y || !beam_r || !angle)
            print_warning("Ошибка ввода: некорректный ввод");
        else
        {
            content_t *c = new content_t;
            copy(&c, &data);
            cancel.push(*c);
            ui->pushButton_cancel->setEnabled(true);

            QPoint center = QPoint(beam_x, beam_y);
            check_max_min(center);

            spec_t spectre;
            spectre.center = center;
            spectre.angle = angle;
            spectre.color = line_color;
            spectre.method = (method_t) ui->comboBox->currentIndex();
            data.specteres.push_back(spectre);

            // функция рисующая спектр
        }
    }
}

// функция очистки всего холста
void MainWindow::on_pushButton_clear_clicked()
{
    back_color = Qt::white;
    line_color = Qt::black;
    show_color(back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);
    data.lines.clear();
    data.specteres.clear();
    cancel = std::stack<content_t>();
    print_succses("Успешно очищено");
    ui->graphicsView->resetTransform();
    drawing_content();
    data.back = back_color;
    ui->pushButton_cancel->setEnabled(false);
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
}

// функция отмены действия
void MainWindow::on_pushButton_cancel_clicked() // дописать
{
    data = cancel.top();
    cancel.pop();
    drawing_content();
    if (cancel.empty())
    {
        ui->pushButton_cancel->setEnabled(false);
        ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    }
}

// изменение размера (все перестроится)
void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    ui->graphicsView->resetTransform();
    drawing_content();
//    print_succses("Вы изменили размер окна");
}

void MainWindow::on_pushButton_reset_scale_clicked()
{
    ui->graphicsView->resetTransform();
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
}
