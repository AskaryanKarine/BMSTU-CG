#include "algorithms.h"
#include <cmath>

void standart_line(line_t &line, canvas_t scene)
{
    QLineF l = QLineF(line.start, line.end);
    scene->addLine(l, QPen(line.color));
}

void standart_spectrum(spectre_t &spectrum, canvas_t scene)
{
    int count = (int) 360 / spectrum.angle;
    for (int i = 0; i < count; i++)
    {
        double x = spectrum.center.x() + cos(M_PI * spectrum.angle * i / 180) * spectrum.radius;
        double y = spectrum.center.y() + sin(M_PI * spectrum.angle * i / 180) * spectrum.radius;
        QPointF cur_end = QPointF(x, y);
        QLineF line = QLineF(spectrum.center, cur_end);
        scene->addLine(line, QPen(spectrum.color));
    }
}

void dda_line(line_t &line, canvas_t scene, bool steps=false)
{
    double h, w;
    h = 0.5;
    w = 0.5;
    QPen pen = QPen(line.color);
    QBrush brush = QBrush(line.color);
    // Целочисленные значения координат начала и конца отрезка,
    // округленные до ближайшего целого
    int iX1 = roundf(line.start.x());
    int iY1 = roundf(line.start.y());
    int iX2 = roundf(line.end.x());
    int iY2 = roundf(line.end.y());

    // Длина и высота линии
    int deltaX = abs(iX1 - iX2);
    int deltaY = abs(iY1 - iY2);

    // Считаем минимальное количество итераций, необходимое
    // для отрисовки отрезка. Выбирая максимум из длины и высоты
    // линии, обеспечиваем связность линии
    int length = std::max(deltaX, deltaY);

    // особый случай, на экране закрашивается ровно один пиксел
    if (length == 0)
        scene->addRect(iX1, iY1, h, w, pen, brush);


    // Вычисляем приращения на каждом шаге по осям абсцисс и ординат
    double dX = (line.end.x() - line.start.x()) / length;
    double dY = (line.end.y() - line.start.y()) / length;

    // Начальные значения
    double x = line.start.x();
    double y = line.start.y();

    // Основной цикл
    length++;
    while (length--)
    {
        scene->addRect(roundf(x), roundf(y), h, w, pen, brush);
        x += dX;
        y += dY;
    }
}

void dda_spectre(spectre_t &spectrum, canvas_t scene, bool is_drawing=false, bool steps=false)
{
    int count = (int) 360 / spectrum.angle;
    for (int i = 0; i < count; i++)
    {
        double x = spectrum.center.x() + cos(M_PI * spectrum.angle * i / 180) * spectrum.radius;
        double y = spectrum.center.y() + sin(M_PI * spectrum.angle * i / 180) * spectrum.radius;
        QPointF cur_end = QPointF(x, y);
        line_t line;
        line.color = spectrum.color;
        line.start = spectrum.center;
        line.end = cur_end;
        if (is_drawing)
            dda_line(line, scene);
    }
}

//void bresen_double_line(line_t &line, canvas_t scene, bool steps=false)
//{

//}

//void bresen_double_spectre(spectre_t &spectrum, canvas_t scene, bool is_drawing=false, bool steps=false)
//{
//    int count = (int) 360 / spectrum.angle;
//    for (int i = 0; i < count; i++)
//    {
//        double x = spectrum.center.x() + cos(M_PI * spectrum.angle * i / 180) * spectrum.radius;
//        double y = spectrum.center.y() + sin(M_PI * spectrum.angle * i / 180) * spectrum.radius;
//        QPointF cur_end = QPointF(x, y);
//        line_t line;
//        line.color = spectrum.color;
//        line.start = spectrum.center;
//        line.end = cur_end;
//        if (is_drawing)
//            bresen_double_line(line, scene);
//    }
//}

//void bresen_int_line(line_t &line, canvas_t scene, bool steps=false)
//{

//}

//void bresen_int_spectre(spectre_t &spectrum, canvas_t scene, bool is_drawing = false, bool steps = false)
//{
//    int count = (int) 360 / spectrum.angle;
//    for (int i = 0; i < count; i++)
//    {
//        double x = spectrum.center.x() + cos(M_PI * spectrum.angle * i / 180) * spectrum.radius;
//        double y = spectrum.center.y() + sin(M_PI * spectrum.angle * i / 180) * spectrum.radius;
//        QPointF cur_end = QPointF(x, y);
//        line_t line;
//        line.color = spectrum.color;
//        line.start = spectrum.center;
//        line.end = cur_end;
//        if (is_drawing)
//            bresen_int_line(line, scene);
//    }
//}


