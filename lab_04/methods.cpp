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
    double r_p = circle.ra * circle.ra;

    QPointF center = circle.center;

    for (double x = 0.0; x < std::round(circle.ra / std::sqrt(2)) || fabs(x - std::round(circle.ra / std::sqrt(2))) < eps; x++)
    {
        double y = round(sqrt(r_p - x * x));
        if (is_draw)
        {
            QPointF p = QPointF(x + center.x(), y + center.y());
            draw_reflect_circle(scene, p, center, circle.color);
        }
    }
}

void canonical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double ra_p = ellipse.ra * ellipse.ra;
    double rb_p = ellipse.rb * ellipse.rb;
    double lim = round(ellipse.center.x() + ellipse.ra / std::sqrt(1 + rb_p / ra_p));

    QPointF center = ellipse.center;

    for (double x = center.x(); x <= lim /*|| fabs(x - lim) < eps*/; x++)
    {
        double y = round(sqrt(ra_p * rb_p - (x - center.x()) * (x - center.x()) * rb_p) / ellipse.ra + center.y());

        if (is_draw)
        {
            QPointF p = QPointF(x, y);
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }
    }

    lim = round(center.y() + ellipse.rb / sqrt(1 + ra_p / rb_p));

    for (double y = lim; y >= center.y(); y--)
    {
        double x = round(sqrt(ra_p * rb_p - (y - center.y()) * (y - center.y()) * ra_p) / ellipse.rb + center.x());

        if (is_draw)
        {
            QPointF p = QPointF(x, y);
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }
    }
}

void parametrical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw)
{
    double step = 1 / circle.ra;
    QPointF center = circle.center;
    double x, y;
    for (double t = 0.0; t <= M_PI_4; t += step)
    {        
        x = center.x() + circle.ra * cos(t);
        y = center.y() + circle.ra * sin(t);
        if (is_draw)
        {
            QPointF p = QPointF(x, y);
            draw_reflect_circle(scene, p, center, circle.color);
        }
    }
}

void parametrical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw)
{
    double step;
    QPointF center = ellipse.center;
    if (ellipse.ra <= ellipse.rb)
        step = 1 / ellipse.rb;
    else
        step = 1 / ellipse.ra;
    double x, y;
    for (double t = 0.0; t <= M_PI_2; t += step)
    {
        x = center.x() + ellipse.ra * cos(t);
        y = center.y() + ellipse.rb * sin(t);
        if (is_draw)
        {
            QPointF p = QPointF(x, y);
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }
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
    int x = 0;
    int y = ellipse.rb;
    double pb = ellipse.rb * ellipse.rb;
    double pa = ellipse.ra * ellipse.ra;
    double delta = round(pb - pa * (2 * ellipse.rb + 1));

    QPointF center = ellipse.center;

    while (y >= 0)
    {

        if (is_draw)
        {
            QPointF p = QPointF(x + center.x(), y + center.y());
            draw_reflect_ellipse(scene, p, center, ellipse.color);
        }

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
        else if (abs(delta) <= 1e-8)
        {
            x++;
            y--;
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
    double y = 0;
    double delta = round(1.25 - circle.ra);

    QPointF center = circle.center;

    while (x >= y)
    {

        if (is_draw)
        {
            QPointF p = QPointF(x + center.x(), y + center.y());
            draw_reflect_circle(scene, p, center, circle.color);
        }
        y++;

        if (round(delta) >= 0)
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
