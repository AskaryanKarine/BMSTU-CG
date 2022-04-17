#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QColorDialog>
#include <QWheelEvent>
#include "request.h"
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
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->viewport()->installEventFilter(this);

    // отключить кнопки и задержку
    ui->pushButton_cancel->setEnabled(false);
    ui->spinBox->setEnabled(false);

    // установка цветов
    show_color(back_color, ui->label_bc);
    show_color(line_color, ui->label_lc);
    show_color(fill_color, ui->label_fc);

    // начальные условия таблицы
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "x" << "y" << "№ф" << "№о");
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

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
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scene;
    cancel = std::stack <content>();
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
//        QWheelEvent *wheel_event = static_cast<QWheelEvent *>(event);
//        wheelEvent(wheel_event);
        return true;
    }
    return false;
}

// добавление точки по нажатию кнопки
void MainWindow::on_pushButton_add_point_clicked() // тут должна быть отмена действия и рисование
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
            // отправить запрос на добавление точки
            request req;
            req.data = data;
            req.fill = fill_color;
            req.is_smth = is_hole;
            req.line = line_color;
            req.p = p;
            req.scene = scene;
            req.table = ui->tableWidget;
            req.view = ui->graphicsView;
            req.oper = ADD_POINT;
            int rc = request_handle(req);
            if (rc)
                error_message("Такая точка уже введена");
            data = req.data;
            req.oper = DRAW;
            req.is_smth = false;
            request_handle(req);
//            add_point(p, line_color, fill_color, is_hole, ui->tableWidget, data);
//            drawing_points(scene, ui->graphicsView, false, p, data);

        }
    }
}

// замыкание фигуры / отверстия
void MainWindow::on_pushButton_close_clicked() // запрос на рисование, отмена
{
    int n_figures = data.n_figures;
    int n_holes = data.n_holes;
    if (!is_hole)
    {
        if (data.figures[n_figures].main_figure.size() >= 3)
            data.figures[n_figures].is_closed_figure = true;
        else
            error_message("Недостаточно точек, чтобы замкнуть фигуру");
    }
    else
    {
        if (data.figures[n_figures].holes[n_holes].points.size() >= 3)
        {
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
//    drawing_points(scene, ui->graphicsView, false, {0, 0}, data);
}

// добавление отверстия
void MainWindow::on_pushButton_add_hole_clicked() // отмена
{
    if (!data.figures[data.n_figures].is_closed_figure)
        error_message("Фигура не замкнута. Сперва замкните ее");
    else
    {
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
//    drawing_points(scene, ui->graphicsView, true, p, data);
}

// изменение точки в таблице
void MainWindow::on_tableWidget_itemChanged(QTableWidgetItem *item)
{

}

// сброс масштабирования
void MainWindow::on_pushButton_reset_scale_clicked()
{
    ui->graphicsView->resetTransform();
}

