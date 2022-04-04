#include "drawing.h"
#include "methods.h"
#include <cmath>

void drawing_axes(canvas_t &scene)
{
    QPen pen = QPen(Qt::black, 2);
    // Oy
    scene->addLine(0, -8, 0, 200, pen);
    scene->addLine(0, 200, 8, 170, pen);
    scene->addLine(0, 200, -8, 170, pen);
    // Y
    scene->addLine(-6, 210, 0, 220, pen);
    scene->addLine(0, 220, 6, 210, pen);
    scene->addLine(0, 220, 0, 228, pen);

    // Ox
    scene->addLine(-8, 0, 200, 0, pen);
    scene->addLine(200, 0, 170, 8, pen);
    scene->addLine(200, 0, 170, -8, pen);
    // X
    scene->addLine(210, -6, 218, 6, pen);
    scene->addLine(218, -6, 210, 6, pen);
}

void drawing_all(canvas_t &scene, gv_t &gv, const content_t &data)
{
    scene->clear();
    gv->setBackgroundBrush(data.back_color);
    drawing_axes(scene);

    for (size_t i = 0; i < data.figures.size(); i++)
    {
        if (data.figures[i].type == CIRCLE)
            drawing_circle(scene, data.figures[i], true);
        else
            drawing_ellipse(scene, data.figures[i], true);
    }

    for (size_t i = 0; i < data.spectrums.size(); i++)
    {
        if (data.spectrums[i].type == CIRCLE)
            drawing_spectrum_circle(scene, data.spectrums[i]);
        else
            drawing_spectrum_ellispe(scene, data.spectrums[i]);
    }
}

void drawing_circle(canvas_t &scene, const figure_t &data, const bool &is_draw)
{
    switch (data.method)
    {
        case STANDART:
            standart_circle(scene, data);
            break;
        case CANONICAL:
            canonical_circle(scene, data, is_draw);
            break;
        case PARAMETRIC:
            parametrical_circle(scene, data, is_draw);
            break;
        case BRESEN:
            bresen_circle(scene, data, is_draw);
            break;
        case MIDDLE_POINT:
            middle_point_circle(scene, data, is_draw);
            break;
    }
}

void drawing_ellipse(canvas_t &scene, const figure_t &data, const bool &is_draw)
{
    switch (data.method)
    {
        case STANDART:
            standart_ellipse(scene, data);
            break;
        case CANONICAL:
            canonical_ellipse(scene, data, is_draw);
            break;
        case PARAMETRIC:
            parametrical_ellipse(scene, data, is_draw);
            break;
        case BRESEN:
            bresen_ellipse(scene, data, is_draw);
            break;
        case MIDDLE_POINT:
            middle_point_ellipse(scene, data, is_draw);
            break;
    }
}

void drawing_spectrum_circle(canvas_t &scene, const spectrum_t &spectrum)
{
    figure_t circle;
    circle.center = spectrum.center;
    circle.color = spectrum.color;
    circle.method = spectrum.method;
    for (int i = 0; i < spectrum.n; i++)
    {
        circle.ra = spectrum.ra + i * spectrum.dra;
        drawing_circle(scene, circle, true);
    }
}

void drawing_spectrum_ellispe(canvas_t &scene, const spectrum_t &spectrum)
{
    figure_t ellispe;
    ellispe.center = spectrum.center;
    ellispe.color = spectrum.color;
    ellispe.method = spectrum.method;
    for (int i = 0; i < spectrum.n; i++)
    {
        ellispe.ra = spectrum.ra + i * spectrum.dra;
        ellispe.rb = spectrum.rb + i * spectrum.drb;
        drawing_ellipse(scene, ellispe, true);
    }
}

void draw_pix(canvas_t &scene, const double &x, const double &y, const QColor &color)
{
    QPen pen = QPen(color);
    QBrush brush = QBrush(color);
    scene->addRect(round(x), round(y), 1, 1, pen, brush);
}

void draw_reflect_circle(canvas_t &scene, const QPointF &point, const QPointF &center, const QColor &color)
{
    draw_pix(scene, point.x(), point.y(), color);
    draw_pix(scene, 2 * center.x() - point.x(), point.y(), color);
    draw_pix(scene, point.x(), 2 * center.y() - point.y(), color);
    draw_pix(scene, 2 * center.x() - point.x(), 2 * center.y() - point.y(), color);
    draw_pix(scene, point.y() + center.x() - center.y(),
             point.x() + center.y() - center.x(), color);
    draw_pix(scene, -point.y() + center.x() + center.y(),
             point.x() + center.y() - center.x(), color);
    draw_pix(scene, point.y() + center.x() - center.y(),
             -point.x() + center.y() + center.x(), color);
    draw_pix(scene, -point.y() + center.x() + center.y(),
             -point.x() + center.x() + center.y(), color);

}

void draw_reflect_ellipse(canvas_t &scene, const QPointF &point, const QPointF &center, const QColor &color)
{
    draw_pix(scene, point.x(), point.y(), color);
    draw_pix(scene, 2 * center.x() - point.x(), point.y(), color);
    draw_pix(scene, point.x(), 2 * center.y() - point.y(), color);
    draw_pix(scene, 2 * center.x() - point.x(), 2 * center.y() - point.y(), color);
}
