#include "drawing.h"
#include "fill.h"

#define COEF 2

void draw_point(const point &point, QPainter &p)
{
    p.drawRect(point.x - COEF / 2, point.y - COEF / 2, COEF, COEF);
}

void draw_line(const point &p1, const point &p2, QPainter &p)
{
    draw_point(p1, p);
    draw_point(p2, p);
    p.drawLine(p1.x, p1.y, p2.x, p2.y);
}

// функция отрисовки точек (без окружностей)
void drawing_points(canvas_t &scene, gv_t &gv, const bool &is_sel, const point &selected_p, const content &data)
{
//    scene->clear();
    QImage image = QImage(gv->geometry().width(), gv->geometry().height(), QImage::Format_RGB32);
//    QPainter p(&image);
    image.fill(data.back_color);
    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);

    image = QImage(gv->geometry().width(), gv->geometry().height(), QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter p(&image);
    figure f;
    for (size_t i = 0; i < data.figures.size(); i++)
    {
        f = data.figures[i];
        p.setPen(f.line_color);
        p.setBrush(f.line_color);
        size_t size = f.main_figure.size();
        if (size == 1)
            draw_point(f.main_figure[0], p);
        else if (size > 1)
        {
            for (size_t j = 0; j < size - 1; j++)
                draw_line(f.main_figure[j], f.main_figure[j + 1], p);
            if (f.is_closed_figure)
                draw_line(f.main_figure[0], f.main_figure[f.main_figure.size() - 1], p);
            for (size_t j = 0; j < f.holes.size(); j++)
            {
                size_t size_h = f.holes[j].points.size();
                if (size_h == 1)
                    draw_point(f.holes[j].points[0], p);
                else if (size_h > 1)
                {
                    for (size_t k = 0; k < f.holes[j].points.size() - 1; k++)
                        draw_line(f.holes[j].points[k], f.holes[j].points[k + 1], p);
                    if (f.holes[j].is_closed_hole)
                        draw_line(f.holes[j].points[0], f.holes[j].points[f.holes[j].points.size() - 1], p);
                }
            }
        }
        if (f.is_fill)
            fill_one(f, 0, scene, gv, f.fill_color);

    }
    if (is_sel)
    {
        p.setBrush(Qt::red);
        p.setPen(Qt::red);

        draw_point(selected_p, p);
    }

    pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}


