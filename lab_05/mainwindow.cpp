#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QWheelEvent>
#include "request.h"
#include <iostream>
//#include "drawing.h"


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

    // все, что связано со сценой
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->viewport()->installEventFilter(this);

    // отключить кнопки и задержку
    ui->pushButton_cancel->setEnabled(false);
    ui->spinBox->setEnabled(false);

    // установка цветов
    show_color(back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);
    show_color(fill_color, ui->label_fc);
    ui->graphicsView->setBackgroundBrush(back_color);

    // начальные условия таблицы
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y" << "№ф" << "№о" << "i");
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setColumnHidden(4, true);


    // начальные значения для работы
    data.n_figures = 0;
    data.n_holes = -1;

    figure f;
    f.fill_color = fill_color;
    f.line_color = line_color;
    f.is_closed_figure = false;
    data.figures.push_back(f);
    data.back_color = back_color;
    data.figures[data.n_figures].line_color = line_color;
    data.figures[data.n_figures].fill_color = fill_color;

    ui->pushButton_cursor_mode->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    cancel = std::stack <content>();
}

static void copy(struct content **a, struct content *b)
{
    (*a)->back_color = b->back_color;
    (*a)->n_figures = b->n_figures;
    (*a)->n_holes = b->n_holes;
    for (size_t i = 0; i < b->figures.size(); i++)
    {
        (*a)->figures.push_back(b->figures[i]);
//        for (size_t j = 0; j < b->figures[j].main_figure.size(); j++)
//            (*a)->figures[i].main_figure
    }

}

// информационные функции
void MainWindow::app_info_show()
{
    QMessageBox::information(NULL, "О программе","Реализация и исследование алгоритмов растрового заполнения сплошных областей");
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

void MainWindow::color_dialog(QColor &color)
{
    QColorDialog dialog;
    dialog.setCurrentColor(line_color);
    dialog.show();
    dialog.exec();
    QColor tmp = dialog.selectedColor();
    if (!tmp.isValid())
        error_message("Выберите цвет");
    else
        color = tmp;
}

void MainWindow::on_pushButton_back_color_clicked()
{
    color_dialog(back_color);
    show_color(back_color, ui->label_bc);
    data.back_color = back_color;
    ui->graphicsView->setBackgroundBrush(QBrush(back_color));
}

void MainWindow::on_pushButton_line_color_clicked()
{
    color_dialog(line_color);
    show_color(line_color, ui->label_lc);
    data.figures[data.n_figures].line_color = line_color;
}

void MainWindow::on_pushButton_fill_color_clicked()
{
    color_dialog(fill_color);
    show_color(fill_color, ui->label_fc);
    data.figures[data.n_figures].fill_color = fill_color;
}

// выбор формата вывода
void MainWindow::on_comboBox_activated(int index)
{
    if (index == 1)
        ui->spinBox->setEnabled(true);
    else
        ui->spinBox->setEnabled(false);
}

bool MainWindow::eventFilter(QObject* object, QEvent* event)
{
    if (event->type() == QEvent::Wheel && object == ui->graphicsView->viewport())
    {
        if (is_hand)
        {
            QWheelEvent *wheel_event = static_cast<QWheelEvent *>(event);
            ui->graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
            double scale_factor = 1.15;
            if (wheel_event->angleDelta().y() > 0)
            {
                cnt++;
                ui->graphicsView->scale(scale_factor, scale_factor);
            }
            else if (cnt > 0)
            {
                cnt--;
                ui->graphicsView->scale(1 / scale_factor, 1 / scale_factor);
            }
        }
        return true;
    }
    return false;
}

// добавление точки по нажатию кнопки
void MainWindow::on_pushButton_add_point_clicked()
{
    QString str_x = ui->lineEdit_add_x->text();
    QString str_y = ui->lineEdit_add_y->text();

    if (str_x.length() == 0)
        error_message("Ошибка ввода: введите координату Х");
    else if (str_y.length() == 0)
        error_message("Ошибка ввода: введите координату Y");
    else
    {
        bool flag_x, flag_y;
        int x, y;
        x = str_x.toInt(&flag_x);
        y = str_y.toInt(&flag_y);

        if (!flag_x)
            error_message("Ошибка ввода: координата Х - число");
        else if (!flag_y)
            error_message("Ошибка ввода: координата Y - число");
        else
        {
            point p = {x, y};
            request req;
            req.data = data;
            req.colors_data = {line_color, fill_color};
            req.is_smth = is_hole;
            req.p = p;
            req.scene = scene;
            req.table = ui->tableWidget;
            req.view = ui->graphicsView;
            req.oper = ADD_POINT;
            int rc = request_handle(req);
            if (rc == 1)
                error_message("Такая точка уже введена");
            else if (rc == 2)
                error_message("Точка отверстия находится вне фигуры");
            else if (rc == 0)
            {
                content *c = new content;
                copy(&c, &data);
                cancel.push(*c);
                ui->pushButton_cancel->setEnabled(true);
                data = req.data;
                req.oper = DRAW;
                req.is_smth = false;
                request_handle(req);
            }
        }
    }
}

// замыкание фигуры / отверстия
void MainWindow::on_pushButton_close_clicked()
{
    int n_figures = data.n_figures;
    int n_holes = data.n_holes;
    if (!is_hole)
    {
        if (data.figures[n_figures].main_figure.size() >= 3)
        {
            content *c = new content;
            copy(&c, &data);
            cancel.push(*c);
            ui->pushButton_cancel->setEnabled(true);

            data.figures[n_figures].is_closed_figure = true;
        }
        else
            error_message("Недостаточно точек, чтобы замкнуть фигуру");
    }
    else
    {
        if (data.figures[n_figures].holes[n_holes].points.size() >= 3)
        {
            content *c = new content;
            copy(&c, &data);
            cancel.push(*c);
            ui->pushButton_cancel->setEnabled(true);

            data.figures[n_figures].holes[n_holes].is_closed_hole = true;
            is_hole = false;
            ui->pushButton_add_hole->setEnabled(true);

        }
        else
            error_message("Недостаточно точек, чтобы замкнуть отверстие");
    }
    request req;
    req.data = data;
    req.scene = scene;
    req.view = ui->graphicsView;
    req.is_smth = false;
    req.p = {0, 0};
    req.oper = DRAW;
    request_handle(req);
}

// добавление отверстия
void MainWindow::on_pushButton_add_hole_clicked()
{
    if (!data.figures[data.n_figures].is_closed_figure)
        error_message("Фигура не замкнута. Сперва замкните ее");
    else
    {
        content *c = new content;
        copy(&c, &data);
        cancel.push(*c);
        ui->pushButton_cancel->setEnabled(true);

        is_hole = true;
        ui->pushButton_add_hole->setEnabled(false);
        data.figures[data.n_figures].holes.push_back({.is_closed_hole = false});
        data.n_holes++;
    }

}

// очистка всего экрана
void MainWindow::on_pushButton_clear_clicked()
{
    scene->clear();

    back_color = Qt::white;
    line_color = Qt::black;
    fill_color = Qt::black;
    show_color(back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);
    show_color(fill_color, ui->label_fc);
    data.back_color = back_color;

    cancel = std::stack<content>();
    ui->pushButton_cancel->setEnabled(false);

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    data.figures.clear();
    data.n_figures = 0;
    data.n_holes = -1;
    figure f;
    f.fill_color = fill_color;
    f.line_color = line_color;
    f.is_closed_figure = false;
    data.figures.push_back(f);
    data.figures[data.n_figures].line_color = line_color;
    data.figures[data.n_figures].fill_color = fill_color;

    ui->graphicsView->resetTransform();
}

// выделение точки
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    point p;
    p.x = ui->tableWidget->item(row, 0)->text().toDouble();
    p.y = ui->tableWidget->item(row, 1)->text().toDouble();
    request req;
    req.data = data;
    req.scene = scene;
    req.view = ui->graphicsView;
    req.is_smth = true;
    req.p = p;
    req.oper = DRAW;
    request_handle(req);
}

// изменение точки в таблице
void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item) // отмену добавить
{
    int row = ui->tableWidget->row(item);
    bool flag_x, flag_y;
    int x = ui->tableWidget->item(row, 0)->text().toInt(&flag_x);
    int y = ui->tableWidget->item(row, 1)->text().toInt(&flag_y);
    int n_f = ui->tableWidget->item(row, 2)->text().toInt();
    int n_h = ui->tableWidget->item(row, 3)->text().toInt();
    size_t i = (size_t) ui->tableWidget->item(row, 4)->text().toInt();
    point old_p;
    if (n_h == -1)
        old_p = data.figures[n_f].main_figure[i];
    else
        old_p = data.figures[n_f].holes[n_h].points[i];

    if (!flag_x)
    {
        error_message("Координата Х должна быть целым числом");
        ui->tableWidget->blockSignals(true);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(old_p.x)));
        ui->centralwidget->blockSignals(false);
    }
    else if (!flag_y)
    {
        error_message("Координата Y должна быть целым числом");
        ui->tableWidget->blockSignals(true);
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(old_p.y)));
        ui->centralwidget->blockSignals(false);
    }
    else
    {
        point p = {x, y};
        request req;
        req.oper = CHANGE_POINT;
        req.data = data;
        req.indexes_data = {n_f, n_h, i};
        req.table = ui->tableWidget;
        req.p = p;
        req.scene = scene;
        req.view = ui->graphicsView;
        int rc = request_handle(req);
        if (rc)
        {
            error_message("Такая точка существует");
            ui->tableWidget->blockSignals(true);
            ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(old_p.x)));
            ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(old_p.y)));
            ui->centralwidget->blockSignals(false);
        }
        else
        {
            content *c = new content;
            copy(&c, &data);
            cancel.push(*c);
            ui->pushButton_cancel->setEnabled(true);
            data = req.data;
        }
    }
}

// сброс масштабирования
void MainWindow::on_pushButton_reset_scale_clicked()
{
    ui->graphicsView->resetTransform();
}

// отмена
void MainWindow::on_pushButton_cancel_clicked()
{
    if (!cancel.empty())
    {
        data = cancel.top();
        request req;
        req.data = data;
        req.table = ui->tableWidget;
        req.oper = CANCEL;
        req.scene = scene;
        req.view = ui->graphicsView;
        request_handle(req);
        cancel.pop();
    }
    if (cancel.empty())
        ui->pushButton_cancel->setEnabled(false);
}

// удаление точки
void MainWindow::on_pushButton_del_point_clicked()
{
    content *c = new content;
    copy(&c, &data);
    cancel.push(*c);
    ui->pushButton_cancel->setEnabled(true);
    QTableWidgetItem *cur_item = ui->tableWidget->currentItem();
    if (cur_item)
    {
        int row = ui->tableWidget->row(cur_item);
        int n_f = ui->tableWidget->item(row, 2)->text().toInt();
        int n_h = ui->tableWidget->item(row, 3)->text().toInt();
        size_t i = (size_t) ui->tableWidget->item(row, 4)->text().toInt();
        request req;
        req.data = data;
        req.indexes_data = {n_f, n_h, i};
        req.table = ui->tableWidget;
        req.scene = scene;
        req.view = ui->graphicsView;
        req.oper = DELETE_POINT;
        request_handle(req);
        data = req.data;
    }
        else
            error_message("Сначала выберите точку");
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QRect view = ui->graphicsView->geometry();
    if (view.contains(event->pos()))
    {
        point p = {event->pos().x() - view.x(), event->pos().y() - view.y() - menuBar()->geometry().height()};
        request req;
        req.data = data;
        req.colors_data = {line_color, fill_color};
        req.is_smth = is_hole;
        req.p = p;
        req.scene = scene;
        req.table = ui->tableWidget;
        req.view = ui->graphicsView;
        req.oper = ADD_POINT;
        if (!is_hand)
        {
            int rc = request_handle(req);
            if (rc == 1)
                error_message("Такая точка уже введена");
            else if (rc == 2)
                error_message("Точка отверстия находится вне фигуры");
            else if (rc == 0)
            {
                content *c = new content;
                copy(&c, &data);
                cancel.push(*c);
                ui->pushButton_cancel->setEnabled(true);
                data = req.data;
                req.oper = DRAW;
                req.is_smth = false;
                request_handle(req);
            }
        }
    }
}

void MainWindow::on_pushButton_fill_clicked()
{
    int delay = 0;
    if (ui->comboBox->currentIndex() == 1)
    {
        delay = ui->spinBox->value();
    }
    for (size_t i = 0; i < data.figures.size(); i++)
        if (data.figures[i].is_closed_figure)
            for (size_t j = 0; j < data.figures[i].holes.size(); j++)
                if (!data.figures[i].holes[j].is_closed_hole)
                {
                    error_message("Замкните фигуру перед заливкой");
                    return;
                }

    request req;
    req.data = data;
    req.oper = FILL;
    req.delay = delay;
    req.scene = scene;
    req.view = ui->graphicsView;
    req.colors_data = {line_color, fill_color};
    request_handle(req);
    content *c = new content;
    copy(&c, &data);
    cancel.push(*c);
    ui->pushButton_cancel->setEnabled(true);
    data = req.data;

}


void MainWindow::on_pushButton_hand_mode_clicked()
{
    ui->pushButton_hand_mode->setEnabled(false);
    ui->pushButton_cursor_mode->setEnabled(true);
    is_hand = true;
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
}


void MainWindow::on_pushButton_cursor_mode_clicked()
{
    ui->pushButton_hand_mode->setEnabled(true);
    ui->pushButton_cursor_mode->setEnabled(false);
    is_hand = false;
    ui->graphicsView->setDragMode(QGraphicsView::NoDrag);
    ui->graphicsView->resetTransform();
}

