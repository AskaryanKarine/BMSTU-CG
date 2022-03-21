#include "algorithms.h"
#include <QRectF>
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

//    scene->addLine(x, y, x + 0.2, y + 0.2, pen);
    scene->addRect(x, y, 1, 1, pen, brush);
}

int dda_line(line_t &line, canvas_t scene, bool is_drawing=false, bool is_cnt_steps=false)
{
    int iX1 = round(line.start.x());
    int iY1 = round(line.start.y());
    int iX2 = round(line.end.x());
    int iY2 = round(line.end.y());

    int deltaX = abs(iX2 - iX1);
    int deltaY = abs(iY2 - iY1);

    int length = std::max(deltaX, deltaY);

    if (length == 0)
        if (is_drawing)
            draw_pix(iX1, iY1, scene, line.color);

    double dX = (line.end.x() - line.start.x()) / length;
    double dY = (line.end.y() - line.start.y()) / length;

    double x = line.start.x();
    double y = line.start.y();

    double buf_x = x, buf_y = y;
    int steps = 1;

    // Основной цикл
    for (int i = 0; i < length; i++)
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

    return steps;
}

static int sign(double x)
{
    if (x < 0)
        return -1;
    if (x > 0)
        return 1;
    return 0;
}


int bresen_double_line(line_t &line, canvas_t scene, bool is_drawing=false, bool is_cnt_steps=false)
{
    double x = round(line.start.x());
    double y = round(line.start.y());

    double dx = line.end.x() - line.start.x();
    double dy = line.end.y() - line.start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = abs(dx);
    dy = abs(dy);

    int change;
    if (dx > dy)
        change = 0;
    else
    {
        change = 1;
        std::swap(dx, dy);
    }

    double m = dy / dx;
    double e = m - 0.5;

    double buf_x = x, buf_y = y;
    int steps = 1;

    for (int i = 0; i <= dx; i++)
    {
        if (is_drawing)
            draw_pix(x, y, scene, line.color);

        if (e >= 0)
        {
            if (change == 0)
                y += sy;
            else
                x += sx;
            e--;
        }
        else
        {
            if (change == 0)
                x += sx;
            else
                y += sy;
            e += m;
        }

        if (is_cnt_steps)
        {
            if (buf_x != x && buf_y != y)
                steps++;

            buf_x = x;
            buf_y = y;
        }
    }

    return steps;
}


int bresen_int_line(line_t &line, canvas_t scene, bool is_drawing=false, bool is_cnt_steps=false)
{
    int x = round(line.start.x());
    int y = round(line.start.y());

    int dx = line.end.x() - line.start.x();
    int dy = line.end.y() - line.start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = abs(dx);
    dy = abs(dy);

    int change;
    if (dx > dy)
        change = 0;
    else
    {
        change = 1;
        std::swap(dx, dy);
    }

    int m = 2 * dy;
    int m1 = 2 * dx;
    int e = m - dx;

    int buf_x = x, buf_y = y;
    int steps = 1;

    for (int i = 0; i <= dx; i++)
    {
        if (is_drawing)
            draw_pix(x, y, scene, line.color);

        if (e >= 0)
        {
            if (change == 0)
                y += sy;
            else
                x += sx;
            e -= m1;
        }
        else
        {
            if (change == 0)
                x += sx;
            else
                y += sy;
            e += m;
        }

        if (is_cnt_steps)
        {
            if (buf_x != x && buf_y != y)
                steps++;

            buf_x = x;
            buf_y = y;
        }
    }

    return steps;
}

QColor update(QColor color, double intens)
{
    int r = color.red();
    int g = color.green();
    int b = color.blue();

    int res;
    if (255 * intens > 255)
        res = 255;
    else
        res = 255 * intens;
    if (res < 0)
        res = 0;

    QColor c = QColor(r, g, b, res);
    return c;
}


int bresen_steps_line(line_t &line, canvas_t scene, bool is_drawing=false, bool is_cnt_steps=false)
{
    double x = round(line.start.x());
    double y = round(line.start.y());

    double dx = line.end.x() - line.start.x();
    double dy = line.end.y() - line.start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = abs(dx);
    dy = abs(dy);

    int change;
    if (dx > dy)
        change = 0;
    else
    {
        change = 1;
        std::swap(dx, dy);
    }

    double m = dy / dx;
    double e = 0.5;
    QColor color1 = update(line.color, e);

    if (is_drawing)
        draw_pix(x, y, scene, color1);

    double W = 1 - m;

    double buf_x = x, buf_y = y;
    int steps = 1;

    for (int i = 0; i <= dx; i++)
    {
        if (e < W)
        {
            if (change == 0)
                x += sx;
            else
                y += sy;
            e += m;
        }
        else
        {
            x += sx;
            y += sy;
            e -= W;
        }

        color1 = update(color1, e);
        if (is_drawing)
            draw_pix(x, y, scene, color1);

        if (is_cnt_steps)
        {
            if (buf_x != x && buf_y != y)
                steps++;

            buf_x = x;
            buf_y = y;
        }
    }

    return steps;
}

int wy_line(line_t &line, canvas_t scene, bool is_drawing=false, bool is_cnt_steps=false)
{

}
