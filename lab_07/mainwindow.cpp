#include "mainwindow.h"
#include "request.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QMouseEvent>
#include <QWidget>
#include <cmath>

#include <iostream>

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
    data.cut_color = Qt::blue;
    data.line_color = Qt::black;
    data.visible_color = QColor("#ff5500");

    show_color(data.cut_color, ui->label_cc);
    show_color(data.line_color, ui->label_lc);
    show_color(data.visible_color, ui->label_vlc);

    // работа со сценой
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->viewport()->installEventFilter(this);

    ui->radioButton_cut->setChecked(is_cut);
    ui->pushButton_cancel->setEnabled(false);

    ui->graphicsView->setMouseTracking(true);
    ui->pushButton_hand_mode->setEnabled(true);
    ui->pushButton_cursor_mode->setEnabled(false);
    is_hand = false;

    data.lines.push_back({});
    data.number_cut = 0;

    ui->pushButton_reset_scale->setToolTip("Сброс масштабирования\n✪ ω ✪");
    ui->pushButton_cursor_mode->setToolTip("Режим ввода точек мышкой.\nВнимание! Нельзя приближать\n(ﾉ◕ヮ◕)ﾉ*:･ﾟ✧");
    ui->pushButton_hand_mode->setToolTip("Режим масштабирования.\nВнимание! Нельзя вводить\nновые точки мышкой\nヽ(✿ﾟ▽ﾟ)ノ");
    ui->statusbar->setToolTip("Примите лабу, пожалуйста, я очень старалась ( •̀ ω •́ )✧");
    ui->graphicsView->setToolTip("Примите лабу, пожалуйста, я очень старалась ( •̀ ω •́ )✧");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    cancel = std::stack<content>();
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

// функции для отмены
static void copy(struct content** a, struct content* b)
{
    (*a)->cut = b->cut;
    (*a)->cut_color = b->cut_color;
    (*a)->line_color = b->line_color;
    (*a)->visible_color = b->visible_color;
    (*a)->number_cut = b->number_cut;
    for (size_t i = 0; i < b->lines.size(); i++)
        (*a)->lines.push_back(b->lines[i]);
}

void MainWindow::push_cancel()
{
    content* c = new content;
    copy(&c, &data);
    cancel.push(*c);
    ui->pushButton_cancel->setEnabled(true);
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

// работа с точками
void MainWindow::add_draw_point(const point& p)
{
    request req;
    req.operation = ADD_POINT;
    req.data = data;
    req.is_cut = ui->radioButton_cut->isChecked();
    req.p = p;
    if (ui->radioButton_cut->isChecked())
        req.number = data.number_cut + 1;
    req.scene = scene;
    req.view = ui->graphicsView;
    int rc = request_handle(req);
    if (rc == 1)
        error_message("Ошибка ввода точки: отсекатель вырожден");
    if (rc == 2)
        error_message("Ошибка ввода точки: начальная и конечная точки линии совпадают");
    if (!rc) {
        push_cancel();
        data = req.data;
        data.number_cut = req.number;
        req.operation = DRAW_ALL;
        request_handle(req);
    }
}

void MainWindow::on_pushButton_add_point_clicked()
{
    QString str_x = ui->lineEdit_x->text();
    QString str_y = ui->lineEdit_y->text();

    if (str_x.length() == 0 && str_y.length() == 0)
        error_message("Введите коориднаты точки");
    else if (str_x.length() == 0)
        error_message("Ошибка ввода: введите координату Х");
    else if (str_y.length() == 0)
        error_message("Ошибка ввода: введите координату Y");
    else {
        bool flag_x, flag_y;
        int x, y;
        x = str_x.toInt(&flag_x);
        y = str_y.toInt(&flag_y);

        if (!flag_x)
            error_message("Ошибка ввода: координата Х - число");
        else if (!flag_y)
            error_message("Ошибка ввода: координата Y - число");
        else if (x < 0 || y < 0)
            error_message("Ошибка ввода: точка находится за пределами поля");
        else {
            point p = { x, y };
            add_draw_point(p);
        }
    }
}

// событие клика мыши
void MainWindow::mousePressEvent(QMouseEvent* event)
{
    QRect view = ui->graphicsView->geometry();
    if (!is_hand) {
        if (event->pos().x() >= view.x() && event->pos().x() <= (view.x() + view.width())
            && event->pos().y() >= view.y() && event->pos().y() <= (view.y() + view.height() + menuBar()->geometry().height())) {
            process = not process;
            std::cout << "process state " << process << std::endl;
            point p = { event->pos().x() - view.x(), event->pos().y() - view.y() - menuBar()->geometry().height() };
            point lp = data.lines[data.lines.size() - 1].p1;
            Qt::KeyboardModifiers key = QApplication::queryKeyboardModifiers();
            if (key == Qt::ShiftModifier && !ui->radioButton_cut->isChecked()) {
                point d = { abs(lp.x - p.x), abs(lp.y - p.y) };
                if (d.x < d.y)
                    p.x = lp.x;
                else
                    p.y = lp.y;
            }
            add_draw_point(p);
        }
    }
}

void MainWindow::my_mouse_move_event(QMouseEvent* event)
{
    if (!process)
        return;
    QRect view = ui->graphicsView->geometry();
    if (event->pos().x() >= 0 && event->pos().y() >= 0
        && event->pos().x() <= view.width() && event->pos().y() <= view.height()) {
        point p = { event->pos().x(), event->pos().y() };
        point lp = data.lines[data.lines.size() - 1].p1;
        Qt::KeyboardModifiers key = QApplication::queryKeyboardModifiers();
        if (key == Qt::ShiftModifier && !ui->radioButton_cut->isChecked()) {
            point d = { abs(lp.x - p.x), abs(lp.y - p.y) };
            if (d.x < d.y)
                p.x = lp.x;
            else
                p.y = lp.y;
        }
        content* c = new content;
        copy(&c, &data);
        request req;
        req.operation = ADD_POINT;
        req.data = *c;
        req.is_cut = ui->radioButton_cut->isChecked();
        req.p = p;
        req.number = 2;
        req.scene = scene;
        req.view = ui->graphicsView;
        int rc = request_handle(req);
        if (!rc) {
            req.operation = DRAW_ALL;
            request_handle(req);
        }
        delete c;
    }
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::MouseMove && object == ui->graphicsView->viewport()) {
        if (!is_hand) {
            QMouseEvent* me = static_cast<QMouseEvent*>(event);
            my_mouse_move_event(me);
            return true;
        }
    }
    if (event->type() == QEvent::Wheel && object == ui->graphicsView->viewport()) {
        if (is_hand) {
            QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
            ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            double scale_factor = 1.15;
            if (wheel_event->angleDelta().y() > 0) {
                //                cnt++;
                ui->graphicsView->scale(scale_factor, scale_factor);
            } else /*if (cnt > 0)*/
            {
                //                cnt--;
                ui->graphicsView->scale(1 / scale_factor, 1 / scale_factor);
            }
        }
        return true;
    }
    return false;
}

// отмена
void MainWindow::on_pushButton_cancel_clicked()
{
    if (!cancel.empty()) {
        data = cancel.top();
        request req;
        req.data = data;
        req.operation = DRAW_ALL;
        req.scene = scene;
        req.view = ui->graphicsView;
        request_handle(req);
        cancel.pop();
        process = not process;
    }
    if (cancel.empty())
        ui->pushButton_cancel->setEnabled(false);
}

// очистка
void MainWindow::on_pushButton_clear_clicked()
{
    scene->clear();

    data.cut_color = Qt::blue;
    data.line_color = Qt::black;
    data.visible_color = QColor("#ff5500");

    show_color(data.cut_color, ui->label_cc);
    show_color(data.line_color, ui->label_lc);
    show_color(data.visible_color, ui->label_vlc);

    data.cut = { {}, {} };
    data.lines.clear();
    data.lines.push_back({});

    data.number_cut = 0;

    cancel = std::stack<content>();
    ui->pushButton_cancel->setEnabled(false);

    ui->graphicsView->resetTransform();
    ui->pushButton_hand_mode->setEnabled(true);
    ui->pushButton_cursor_mode->setEnabled(false);
    is_hand = false;
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
}

void MainWindow::on_pushButton_cut_clicked()
{
    if (!data.cut.is_full()) {
        error_message("Введите отсекатель");
        return;
    }
    if ((data.lines.size() > 1 && !data.lines[data.lines.size() - 2].is_full())
        || (data.lines.size() == 1 && !data.lines[0].is_full())) {
        error_message("Введите отрезок");
        return;
    }
    request req;
    req.data = data;
    req.operation = CUT;
    req.scene = scene;
    req.view = ui->graphicsView;
    request_handle(req);
}

void MainWindow::on_pushButton_reset_scale_clicked()
{
    ui->graphicsView->resetTransform();
}

void MainWindow::on_pushButton_cursor_mode_clicked()
{
    ui->pushButton_hand_mode->setEnabled(true);
    ui->pushButton_cursor_mode->setEnabled(false);
    is_hand = false;
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    ui->graphicsView->resetTransform();
}

void MainWindow::on_pushButton_hand_mode_clicked()
{
    ui->pushButton_hand_mode->setEnabled(false);
    ui->pushButton_cursor_mode->setEnabled(true);
    is_hand = true;
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}
