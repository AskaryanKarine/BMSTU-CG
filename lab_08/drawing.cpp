#include "drawing.h"
#include "structurs.h"

#define COEF 2

void draw_point(const point& dot, QPainter& paint)
{
    paint.drawRect(dot.x - 1, dot.y - 1, COEF, COEF);
}

void draw_line(const line& line, QPainter& paint)
{
    paint.drawLine(line.p1.x, line.p1.y, line.p2.x, line.p2.y);
}

void draw_cut(const polygon& cut, QPainter& paint)
{
    size_t size_point = cut.points.size();
    if (size_point < 2)
        draw_point(cut.points[0], paint);
    else
        for (size_t i = 0; i < size_point - 1; i++)
            draw_line({ cut.points[i], cut.points[i + 1] }, paint);
    if (cut.is_close)
        draw_line({cut.points[0], cut.points[size_point - 1]}, paint);
}

void draw_all(const content& data, canvas_t& scene, gv_t& view)
{
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::white);

    p.setBrush(data.cut_color);
    p.setPen(data.cut_color);
    if (data.cut.points.size() > 0)
        draw_cut(data.cut, p);
    //    else if (!data.cut.p1.is_null())
    //        draw_point(data.cut.p1, p);

    p.setBrush(data.line_color);
    p.setPen(data.line_color);
    for (size_t i = 0; i < data.lines.size() - 1; i++)
        if (data.lines[i].is_full())
            draw_line(data.lines[i], p);

    line last_fig = data.lines[data.lines.size() - 1];

    if (!last_fig.is_full() && !last_fig.p1.is_null())
        draw_point(last_fig.p1, p);

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->clear();
    scene->addPixmap(pixmap);
}
