#include "algorithms.h"
#include <cmath>

void standart_line(line_t &line, canvas_t scene)
{
    QLineF l = QLineF(line.start, line.end);
    scene->addLine(l, QPen(line.color));
}

void draw_pix(double x, double y, canvas_t scene, QColor color)
{
    QPen pen = QPen(color);
    QBrush brush = QBrush(color);

    scene->addRect(x, y, 1, 1, pen, brush);
}


void dda_line(line_t &line, canvas_t scene, bool is_drawing=false, bool is_cnt_steps=false)
{
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
        if (is_drawing)
            draw_pix(iX1, iY1, scene, line.color);

    // Вычисляем приращения на каждом шаге по осям абсцисс и ординат
    double dX = (line.end.x() - line.start.x()) / length;
    double dY = (line.end.y() - line.start.y()) / length;

    // Начальные значения
    double x = line.start.x();
    double y = line.start.y();

    double buf_x = x, buf_y = y;
    int steps = 1;
    // Основной цикл
    length++;
    while (length--)
    {
        if (is_drawing)
            draw_pix(round(x), round(y), scene, line.color);

        x += dX;
        y += dY;

        if (is_cnt_steps)
        {
            if (round(x) != round(buf_x) && round(y) != round(buf_y))
                steps++;

            buf_x = x;
            buf_y = y;
        }
    }
}


//void bresen_double_line(line_t &line, canvas_t scene, bool steps=false)
//{

//}

//void bresen_int_line(line_t &line, canvas_t scene, bool steps=false)
//{

//}



