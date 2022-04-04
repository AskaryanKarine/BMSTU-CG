#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QColor>
#include <QMessageBox>
#include <QWheelEvent>
#include <iostream>
#include <QTimer>
#include <QDrag>
#include <QtGlobal>
#include "request.h"

#define DEF_X ui->graphicsView->geometry().x() / 2.0
#define DEF_Y ui->graphicsView->geometry().y() / 2.0;
#define DEF_N 100
#define DEF_R1 100
#define DEF_R2 100
#define DEF_DR1 1
#define DEF_DR2 1

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


    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    ui->pushButton_cancel->setEnabled(false);

    ui->lineEdit_figure_r2->setDisabled(true);
    ui->lineEdit_spectrum_dr2->setDisabled(true);
    ui->lineEdit_spectrum_r2->setDisabled(true);

    data.back_color = Qt::white;
    show_color(data.back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);

    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    cancel = std::stack <content_t>();
}

// информационные функции
void MainWindow::app_info_show()
{
    QMessageBox::information(NULL, "О программе","Реализация и исследование алгоритмов построения окружностей и эллипсов");
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

// чтоб рисовалось нормально
void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    QTimer::singleShot(100, this, SLOT(windowShown()));
}

void MainWindow::windowShown()
{
    request req;
    req.operation = DRAW_ALL;
    req.data = data;
    req.scene = scene;
    req.gv = ui->graphicsView;
    request_handle(req);
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
    if (event->type() == QEvent::Wheel && object == ui->graphicsView->viewport())
    {
#if QT_VERSION >= 0x060000
        QWheelEvent *wheel_event = static_cast<QWheelEvent *>(event);
        wheelEvent(wheel_event);
#endif
        return true;
    }
    return false;
}

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
    dialog.setCurrentColor(data.back_color);
    dialog.show();
    dialog.exec();
    QColor color = dialog.selectedColor();
    if (!color.isValid())
        error_message("Выберите цвет");
    else
        back_color = color;
    show_color(back_color, ui->label_bc);
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
        error_message("Выберите цвет");
    else
        line_color = color;
    show_color(line_color, ui->label_lc);
}

// функция очистки всего холста
void MainWindow::on_pushButton_clear_clicked()
{
    data.back_color = Qt::white;
    line_color = Qt::black;
    show_color(data.back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);
    data.figures.clear();
    data.spectrums.clear();
    cancel = std::stack<content_t>();
    ui->graphicsView->resetTransform();
    request req;
    req.scene = scene;
    req.gv = ui->graphicsView;
    req.operation = DRAW_ALL;
    req.data = data;
    request_handle(req);
    ui->pushButton_cancel->setEnabled(false);
}

// функция отмены действия
void MainWindow::on_pushButton_cancel_clicked()
{
    if (!cancel.empty())
    {
        data = cancel.top();
        request req;
        req.data = data;
        req.operation = DRAW_ALL;
        req.scene = scene;
        req.gv = ui->graphicsView;
        request_handle(req);
        cancel.pop();
    }
    if (cancel.empty())
        ui->pushButton_cancel->setEnabled(false);
}

// сброс масштабирования
void MainWindow::on_pushButton_reset_scale_clicked()
{
    ui->graphicsView->resetTransform();
}

// выбор фигуры
void MainWindow::on_comboBox_figure_activated(int index)
{
    if (index == 1)
    {
        ui->lineEdit_figure_r2->setDisabled(false);
        ui->lineEdit_spectrum_dr2->setDisabled(false);
        ui->lineEdit_spectrum_r2->setDisabled(false);
    }
    else
    {
        ui->lineEdit_figure_r2->setDisabled(true);
        ui->lineEdit_spectrum_dr2->setDisabled(true);
        ui->lineEdit_spectrum_r2->setDisabled(true);
    }

}

// рисование одной фигуры
void MainWindow::on_pushButton_figure_clicked() // !!!
{
    QString str_x = ui->lineEdit_figure_x->text();
    QString str_y = ui->lineEdit_figure_y->text();
    QString str_r1 = ui->lineEdit_figure_r1->text();
    QString str_r2 = ui->lineEdit_figure_r2->text();

    figure_type_t type = (figure_type_t) ui->comboBox_figure->currentIndex();

    if (!str_x.length() || !str_y.length() || !str_r1.length() || (type == ELLIPSE && !str_r2.length()))
        error_message("Ошибка ввода: неполный или пустой ввод");
    else
    {
        bool flag_x, flag_y, flag_r1, flag_r2;
        double x, y, r1, r2;
        x = str_x.toDouble(&flag_x);
        y = str_y.toDouble(&flag_y);
        r1 = str_r1.toDouble(&flag_r1);
        r2 = str_r2.toDouble(&flag_r2);

        if (!flag_x || !flag_y || !flag_r1 || (type == ELLIPSE && !flag_r2))
            error_message("Ошибка ввода: некорректный ввод");
        else
        {
            content_t *c = new content_t;
            copy(&c, &data);
            cancel.push(*c);
            ui->pushButton_cancel->setEnabled(true);
            QPointF center = QPointF(x, y);
            figure_t figure;
            figure.center = center;
            figure.type = type;
            figure.color = line_color;
            figure.method = (method_t) ui->comboBox_method->currentIndex();
            figure.ra = r1;
            if (type == ELLIPSE)
                figure.rb = r2;
            data.figures.push_back(figure);
            data.back_color = back_color;
            ui->graphicsView->setBackgroundBrush(back_color);

            request req;
            req.operation = DRAW_CIRCLE;
            if (type == ELLIPSE)
                req.operation = DRAW_ELLIPSE;
            req.figure = figure;
            req.scene = scene;
            req.gv = ui->graphicsView;
            req.back_color = back_color;
            request_handle(req);
        }
    }
}

// рисование одного спектра
void MainWindow::on_pushButton_spectrum_clicked() // !!!
{
    QString str_x = ui->lineEdit_spectrum_x->text();
    QString str_y = ui->lineEdit_spectrum_y->text();
    QString str_r1 = ui->lineEdit_spectrum_r1->text();
    QString str_r2 = ui->lineEdit_spectrum_r2->text();
    QString str_dr1 = ui->lineEdit_spectrum_dr1->text();
    QString str_dr2 = ui->lineEdit_spectrum_dr2->text();
    QString str_n = ui->lineEdit_spectrum_n->text();


    figure_type_t type = (figure_type_t) ui->comboBox_figure->currentIndex();

    if (!str_x.length() || !str_y.length() || !str_n.length() || !str_r1.length() || !str_dr1.length() || (type == ELLIPSE && (!str_r2.length() || !str_dr2.length())))
        error_message("Ошибка ввода: неполный или пустой ввод");
    else
    {
        bool flag_x, flag_y, flag_r1, flag_r2, flag_dr1, flag_dr2, flag_n;
        double x, y, r1, r2, dr1, dr2;
        int n;
        x = str_x.toDouble(&flag_x);
        y = str_y.toDouble(&flag_y);
        r1 = str_r1.toDouble(&flag_r1);
        r2 = str_r2.toDouble(&flag_r2);
        dr1 = str_dr1.toDouble(&flag_dr1);
        dr2 = str_dr2.toDouble(&flag_dr2);
        n = str_n.toInt(&flag_n);

        if (!flag_x || !flag_y || !flag_n || !flag_dr1 || !flag_r1 || (type == ELLIPSE && (!flag_r2 || !flag_dr2)))
            error_message("Ошибка ввода: некорректный ввод");
        else
        {
            content_t *c = new content_t;
            copy(&c, &data);
            cancel.push(*c);
            ui->pushButton_cancel->setEnabled(true);
            QPointF center = QPointF(x, y);
            spectrum_t spectrum;
            spectrum.center = center;
            spectrum.type = type;
            spectrum.method = (method_t) ui->comboBox_method->currentIndex();
            spectrum.color = line_color;
            spectrum.dra = dr1;
            spectrum.n = n;
            spectrum.ra = r1;
            if (type == ELLIPSE)
            {
                spectrum.drb = dr2;
                spectrum.rb = r2;
            }
            data.spectrums.push_back(spectrum);
            data.back_color = back_color;
            ui->graphicsView->setBackgroundBrush(back_color);
            request req;
            req.operation = DRAW_SPECTRUM_CIRCLE;
            if (type == ELLIPSE)
                req.operation = DRAW_SPECTRUM_ELLIPSE;
            req.scene = scene;
            req.gv = ui->graphicsView;
            req.back_color = back_color;
            req.spectrum = spectrum;
            request_handle(req);
        }
    }
}

// замеры времени
void MainWindow::on_pushButton_time_clicked()
{
    QString str_x = ui->lineEdit_spectrum_x->text();
    QString str_y = ui->lineEdit_spectrum_y->text();
    QString str_r1 = ui->lineEdit_spectrum_r1->text();
    QString str_r2 = ui->lineEdit_spectrum_r2->text();
    QString str_dr1 = ui->lineEdit_spectrum_dr1->text();
    QString str_dr2 = ui->lineEdit_spectrum_dr2->text();
    QString str_n = ui->lineEdit_spectrum_n->text();


    double x, y, r1, r2, dr1, dr2;
    int n;

    figure_type_t type = (figure_type_t) ui->comboBox_figure->currentIndex();
    bool flag_x, flag_y, flag_r1, flag_r2, flag_dr1, flag_dr2, flag_n;
    x = str_x.toDouble(&flag_x);
    y = str_y.toDouble(&flag_y);
    r1 = str_r1.toDouble(&flag_r1);
    r2 = str_r2.toDouble(&flag_r2);
    dr1 = str_dr1.toDouble(&flag_dr1);
    dr2 = str_dr2.toDouble(&flag_dr2);
    n = str_n.toInt(&flag_n);

    if (!flag_x)
        x = DEF_X;

    if (!flag_y)
        y = DEF_Y;

    if (!flag_n)
        n = DEF_N;

    if (!flag_r1)
        r1 = DEF_R1;

    if (!flag_dr1)
        dr1 = DEF_DR1;

    if (type == ELLIPSE)
    {
        if (!flag_dr2)
            dr2 = DEF_DR2;

        if (!flag_r2)
            r2 = DEF_R2;
    }

    QPointF center = QPointF(x, y);
    spectrum_t spectrum;
    spectrum.center = center;
    spectrum.type = type;
    spectrum.color = line_color;
    spectrum.dra = dr1;
    spectrum.n = n;
    spectrum.ra = r1;
    if (type == ELLIPSE)
    {
        spectrum.drb = dr2;
        spectrum.rb = r2;
    }
    request req;
    req.operation = MEASURE_TIME;
    req.scene = scene;
    req.gv = ui->graphicsView;
    req.spectrum = spectrum;
    request_handle(req);

}
