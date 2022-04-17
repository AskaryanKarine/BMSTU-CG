#include "drawing.h"

#define COEF 4

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
void drawing_points(canvas_t &scene, gv_t &gv, const bool &is_sel, const point &selected_p, content &data)
{
    scene->clear();
    QImage image = QImage(gv->geometry().width(), gv->geometry().height(), QImage::Format_RGB32);
    QPainter p(&image);
    image.fill(data.back_color);

    figure f;
    for (size_t i = 0; i < data.figures.size(); i++)
    {
        f = data.figures[i];
        p.setPen(f.line_color);
        p.setBrush(f.line_color);
        size_t size = f.main_figure.size();
        if (size <= 1)
            draw_point(f.main_figure[0], p);
        else
        {
            for (size_t j = 0; j < size - 1; j++)
                draw_line(f.main_figure[j], f.main_figure[j + 1], p);
            if (f.is_closed_figure)
                draw_line(f.main_figure[0], f.main_figure[f.main_figure.size() - 1], p);
        }
    }
    if (is_sel)
    {
        p.setBrush(Qt::red);
        p.setPen(Qt::red);

        draw_point(selected_p, p);
    }

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}


