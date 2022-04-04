#include "methods.h"
#include "QRectF"
#include "drawing.h"
#include <cmath>

void standart_circle(canvas_t &scene, const figure_t &circle)
{
    QRectF rect = QRectF(circle.center.x() - circle.ra, circle.center.y() - circle.ra, 2 * circle.ra, 2 * circle.ra);
    scene->addEllipse(rect, QPen(circle.color, 1), QBrush(QColor(0,0,0,0)));
}

void standart_ellipse(canvas_t &scene, const figure_t &ellipse)
{
    QRectF rect = QRectF(ellipse.center.x() - ellipse.ra, ellipse.center.y() - ellipse.rb, 2 * ellipse.ra, 2 * ellipse.rb);
    scene->addEllipse(rect, QPen(ellipse.color, 1), QBrush(QColor(0,0,0,0)));
}

void canonical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    const double eps = 1e-6;
    double pr = circle.ra * circle.ra;

    QPointF center = circle.center;

    for (double x = 0.0; x < circle.ra / sqrt(2) || fabs(x - circle.ra * sqrt(2)) < eps; x++)
    {
        double y = sqrt(pr - x * x);
        QPointF p = QPointF(x + center.x(), y + center.y());
        if (is_draw)
            draw_reflect_circle(scene, p, center, circle.color);
    }
}

void canonical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    const double eps = 1e-6;
    double pra = ellipse.ra * ellipse.ra;
    double prb = ellipse.rb * ellipse.rb;
    double lim = ellipse.center.x() + ellipse.ra / sqrt(1 + prb / pra);

    QPointF center = ellipse.center;

    for (double x = center.x(); x < lim || fabs(x - lim) < eps; x++)
    {
        double y = round(sqrt(pra * prb - (x - center.x()) * (x - center.x()) * prb) / ellipse.ra + center.y());
        QPointF p = QPointF(x + center.x(), y + center.y());
        if (is_draw)
            draw_reflect_ellipse(scene, p, center, ellipse.color);
    }

    lim = center.y() + ellipse.rb / sqrt(1 + pra * prb);

    for (double y = lim; y >= center.y(); y--)
    {
        double x = round(sqrt(pra * prb - (y - center.y()) * (y - center.y()) * pra) / ellipse.rb + center.x());
        QPointF p = QPointF(x + center.x(), y + center.y());

        if (is_draw)
            draw_reflect_ellipse(scene, p, center, ellipse.color);
    }
}

void parametrical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double step = 1 / circle.ra;
    QPointF center = circle.center;

    for (double t = 0.0; t <= M_PI_2; t += step)
    {
        QPointF p = QPointF(center.x() + circle.ra * cos(t), center.y() + circle.ra * sin(t));
        if (is_draw)
            draw_reflect_circle(scene, p, center, circle.color);
    }
}

void parametrical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double step;
    QPointF center = ellipse.center;
    if (ellipse.ra > ellipse.rb)
        step = 1 / ellipse.ra;
    else
        step = 1 / ellipse.rb;

    for (double t = 0.0; t <= M_PI_2; t += step)
    {
        QPointF p = QPointF(center.x() + ellipse.ra * cos(t), center.y() + ellipse.rb * sin(t));
        if (is_draw)
            draw_reflect_ellipse(scene, p, center, ellipse.color);
    }
}

void bresen_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double x = 0.0;
    double y = circle.ra;
    double delta = 2 * (1 - circle.ra);

    QPointF center = circle.center;
    QPointF p = QPointF(x + center.x(), y + center.y());

    if (is_draw)
        draw_reflect_circle(scene, p, center, circle.color);

    while (x < y)
    {
        if (delta < 0)
        {
            double tmp = 2 * (delta + y) - 1;
            x++;
            if (tmp > 0)
            {
                y--;
                delta += 2 * (x - y + 1);
            }
            else
                delta += 2 * x + 1;
        }
        else
        {
            double tmp = 2 * (delta - x) - 1;
            y--;
            if (tmp < 0)
            {
                x++;
                delta += 2 * (x - y + 1);
            }
            else
                delta -= 2 * y - 1;
        }

        if (is_draw)
        {
            p = QPointF(x + center.x(), y + center.y());
            draw_reflect_circle(scene, p, center, circle.color);
        }
    }
}

void bresen_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double x = 0.0;
    double y = ellipse.rb;
    double pb = ellipse.rb * ellipse.rb;
    double pa = ellipse.ra * ellipse.ra;
    double delta = pb - pa * (2 * ellipse.rb + 1);

    QPointF center = ellipse.center;

    while (y >= 0)
    {
        QPointF p = QPointF(x + center.x(), y + center.y());
        if (is_draw)
            draw_reflect_ellipse(scene, p, center, ellipse.color);

        if (delta < 0)
        {
            double tmp = 2 * delta + pa * (2 * y - 1);
            x++;
            delta += pb * (2 * x + 1);
            if (tmp > 0)
            {
                y--;
                delta += pa * (-2 * y + 1);
            }
        }
        else if (delta == 0)
        {
            x++;
            y++;
            delta += pb * (2 * x + 1) + (1 - 2 * y) * pa;
        }
        else
        {
            double tmp = 2 * delta + pb * (-2 * x -1);
            y--;
            delta += pa * (-2 * y + 1);
            if (tmp < 0)
            {
                x++;
                delta += pb * (2 * x + 1);
            }
        }
    }
}

void middle_point_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double x = circle.ra;
    double y = 0.0;
    double delta = 1 - circle.ra;

    QPointF center = circle.center;

    while (x >= y)
    {
        QPointF p = QPointF(x + center.x(), y + center.y());
        if (is_draw)
            draw_reflect_circle(scene, p, center, circle.color);
        y++;

        if (delta > 0)
        {
            x--;
            delta -= 2 * x + 1;
        }

        delta += 2 * y + 1;
    }
}

void middle_point_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double x = 0.0;
    double y = ellipse.rb;
    double pa = ellipse.ra * ellipse.ra;
    double pb = ellipse.rb * ellipse.rb;
    double delta = pb - pa * ellipse.rb + 0.25 * pa;
    double dx = 2 * pb * x;
    double dy = 2 * pa * y;

    QPointF center = ellipse.center;

    while (dx < dy)
    {
        if (is_draw)
        {
            QPointF p = QPointF(x + center.x(), y + center.y());
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }

        x++;
        dx += 2 * pb;

        if (delta >= 0)
        {
            y--;
            dy -= 2 * pa;
            delta -= dy;
        }

        delta += dx + pb;

    }

    delta = pb * (x + 0.5) * (x + 0.5) + pa * (y - 1) * (y - 1) - pa * pb;

    while (y >= 0)
    {
        if (is_draw)
        {
            QPointF p = QPointF(x + center.x(), y + center.y());
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }

        y--;
        dy -= 2 * pa;

        if (delta <= 0)
        {
            x++;
            dx += 2 * pb;
            delta += dx;
        }

        delta -= dy - pa;

    }
}
