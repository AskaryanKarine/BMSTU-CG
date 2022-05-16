#include "drawing.h"
#include "structurs.h"

#define COEF 2

void draw_point(const point& dot, QPainter& paint)
{
    paint.drawRect(dot.x - 1, dot.y - 1, COEF, COEF);
}

void draw_line(const figure& line, QPainter& paint)
{
    draw_point(line.p1, paint);
    draw_point(line.p2, paint);
    paint.drawLine(line.p1.x, line.p1.y, line.p2.x, line.p2.y);
}

void draw_cut(const figure& cut, QPainter& paint)
{
    int h = cut.p1.y - cut.p2.y;
    int w = cut.p1.x - cut.p2.x;
    draw_point(cut.p1, paint);
    draw_point(cut.p2, paint);
    draw_point({ cut.p2.x, cut.p1.y }, paint);
    draw_point({ cut.p1.x, cut.p2.y }, paint);
    paint.setBrush(QColor(0, 0, 0, 0));
    paint.drawRect(cut.p1.x, cut.p1.y, -w, -h);
}

void draw_all(const content& data, canvas_t& scene, gv_t& view)
{
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::white);

    p.setBrush(data.cut_color);
    p.setPen(data.cut_color);
    if (data.cut.is_full())
        draw_cut(data.cut, p);
    else if (!data.cut.p1.is_null())
        draw_point(data.cut.p1, p);

    p.setBrush(data.line_color);
    p.setPen(data.line_color);
    for (size_t i = 0; i < data.lines.size() - 1; i++)
        if (data.lines[i].is_full())
            draw_line(data.lines[i], p);

    figure last_fig = data.lines[data.lines.size() - 1];

    if (!last_fig.is_full() && !last_fig.p1.is_null())
        draw_point(last_fig.p1, p);

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->clear();
    scene->addPixmap(pixmap);
}
