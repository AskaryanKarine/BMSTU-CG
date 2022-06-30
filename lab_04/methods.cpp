#include "methods.h"
#include "QRectF"
#include "drawing.h"
#include <cmath>

void standart_circle(canvas_t &scene, const figure_t &circle)
{
    QRectF rect = QRectF(circle.center.x() - circle.ra, circle.center.y() - circle.ra, 2 * circle.ra, 2 * circle.ra);
    scene->addEllipse(rect, QPen(circle.color, 0.5), QBrush(QColor(0,0,0,0)));
}

void standart_ellipse(canvas_t &scene, const figure_t &ellipse)
{
    QRectF rect = QRectF(ellipse.center.x() - ellipse.ra, ellipse.center.y() - ellipse.rb, 2 * ellipse.ra, 2 * ellipse.rb);
    scene->addEllipse(rect, QPen(ellipse.color, 0.5), QBrush(QColor(0,0,0,0)));
}

void canonical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    const double eps = 1e-6;
    double r2 = circle.ra * circle.ra;

    QPoint center = circle.center;

    for (double x = 0.0; x < std::round(circle.ra / std::sqrt(2)) || fabs(x - std::round(circle.ra / std::sqrt(2))) < eps; x++)
    {
        double y = round(sqrt(r2 - x * x));
        if (is_draw)
        {
            QPoint p = QPoint(x + center.x(), y + center.y());
            draw_reflect_circle(scene, p, center, circle.color);
        }
    }
}

void canonical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double ra2 = ellipse.ra * ellipse.ra;
    double rb2 = ellipse.rb * ellipse.rb;
    double lim = round(ellipse.center.x() + ellipse.ra / std::sqrt(1 + rb2 / ra2));

    QPoint center = ellipse.center;

    for (double x = center.x(); x <= lim /*|| fabs(x - lim) < eps*/; x++)
    {
        double y = round(sqrt(ra2 * rb2 - (x - center.x()) * (x - center.x()) * rb2) / ellipse.ra + center.y());

        if (is_draw)
        {
            QPoint p = QPoint(x, y);
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }
    }

    lim = round(center.y() + ellipse.rb / sqrt(1 + ra2 / rb2));

    for (double y = lim; y >= center.y(); y--)
    {
        double x = round(sqrt(ra2 * rb2 - (y - center.y()) * (y - center.y()) * ra2) / ellipse.rb + center.x());

        if (is_draw)
        {
            QPoint p = QPoint(x, y);
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }
    }
}

void parametrical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double step = 1 / circle.ra;
    QPoint center = circle.center;
    double x, y;
    for (double t = 0.0; t <= M_PI_4; t += step)
    {        
        x = center.x() + round(circle.ra * cos(t));
        y = center.y() + round(circle.ra * sin(t));
        if (is_draw)
        {
            QPoint p = QPoint(x, y);
            draw_reflect_circle(scene, p, center, circle.color);
        }
    }
}

void parametrical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double step;
    QPoint center = ellipse.center;
    if (ellipse.ra <= ellipse.rb)
        step = 1 / ellipse.rb;
    else
        step = 1 / ellipse.ra;
    double x, y;
    for (double t = 0.0; t <= M_PI_2; t += step)
    {
        x = center.x() + round(ellipse.ra * cos(t));
        y = center.y() + round(ellipse.rb * sin(t));
        if (is_draw)
        {
            QPoint p = QPoint(x, y);
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }
    }
}

void bresen_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double x = 0.0;
    double y = circle.ra;
    double delta = 2 * (1 - circle.ra);

    QPoint center = circle.center;
    QPoint p = QPoint(x + center.x(), y + center.y());

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
            p = QPoint(x + center.x(), y + center.y());
            draw_reflect_circle(scene, p, center, circle.color);
        }
    }
}

void bresen_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double x = 0;
    double y = ellipse.rb;
    double rb2 = ellipse.rb * ellipse.rb;
    double ra2 = ellipse.ra * ellipse.ra;
    double delta = round(rb2 - ra2 * (2 * ellipse.rb - 1));

    QPoint center = ellipse.center;

    while (y >= 0)
    {

        if (is_draw)
        {
            QPoint p = QPoint(x + center.x(), y + center.y());
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }

        if (delta < 0)
        {
            double tmp = round(2 * delta + ra2 * (2 * y - 1));
            x++;
            delta += rb2 * (2 * x + 1);
            if (tmp > 0)
            {
                y--;
                delta += ra2 * (-2 * y + 1);
            }
        }
        else if (abs(delta) <= 1e-8)
        {
            x++;
            y--;
            delta += rb2 * (2 * x + 1) + (1 - 2 * y) * ra2;
        }
        else
        {
            double tmp = 2 * delta + rb2 * (-2 * x -1);
            y--;
            delta += ra2 * (-2 * y + 1);
            if (tmp < 0)
            {
                x++;
                delta += rb2 * (2 * x + 1);
            }
        }
    }
}

void middle_point_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double x = 0.0;
    double y = circle.ra;
    double delta = 1 - circle.ra;

    QPoint center = circle.center;

    while (x <= y)
    {
        if (is_draw)
        {
            QPoint p = QPoint(x + center.x(), y + center.y());
            draw_reflect_circle(scene, p, center, circle.color);
        }

        x++;

        if (delta >= 0)
        {
            y--;
            delta -= y * 2;
        }

        delta += x * 2 + 1;
    }
}

void middle_point_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double x = 0.0;
    double y = ellipse.rb;
    double ra2 = ellipse.ra * ellipse.ra;
    double rb2 = ellipse.rb * ellipse.rb;
    double delta = rb2 - ra2 * ellipse.rb + 0.25 * ra2;
    double dx = 2 * rb2 * x;
    double dy = 2 * ra2 * y;

    QPoint center = ellipse.center;

    while (dx < dy)
    {
        if (is_draw)
        {
            QPoint p = QPoint(x + center.x(), y + center.y());
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }

        x++;
        dx += 2 * rb2;

        if (delta >= 0)
        {
            y--;
            dy -= 2 * ra2;
            delta -= dy;
        }

        delta += dx + rb2;

    }

    delta = rb2 * (x + 0.5) * (x + 0.5) + ra2 * (y - 1) * (y - 1) - ra2 * rb2;

    while (y >= 0)
    {
        if (is_draw)
        {
            QPoint p = QPoint(x + center.x(), y + center.y());
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }

        y--;
        dy -= 2 * ra2;

        if (delta <= 0)
        {
            x++;
            dx += 2 * rb2;
            delta += dx;
        }

        delta -= dy - ra2;

    }
}
