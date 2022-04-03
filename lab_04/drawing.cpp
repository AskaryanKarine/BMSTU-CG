#include "drawing.h"
#include "methods.h"


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
            drawing_circle(scene, gv, data.figures[i], data.back_color, true);
        else
            drawing_ellipse(scene, gv, data.figures[i], data.back_color, true);
    }

    for (size_t i = 0; i < data.spectrums.size(); i++)
    {}
}


void drawing_circle(canvas_t &scene, gv_t &gv, const figure_t &data, const QColor &back, const bool &is_draw)
{
    gv->setBackgroundBrush(back);
    switch (data.method)
    {
        case STANDART:
            standart_circle(scene, data);
            break;
        case CANONICAL:
            break;
        case PARAMETRIC:
            break;
        case BRESEN:
            break;
        case MIDDLE_POINT:
            break;
    }
}

void drawing_ellipse(canvas_t &scene, gv_t &gv, const figure_t &data, const QColor &back, const bool &is_draw)
{
    gv->setBackgroundBrush(back);
    switch (data.method)
    {
        case STANDART:
            standart_ellipse(scene, data);
            break;
        case CANONICAL:
            break;
        case PARAMETRIC:
            break;
        case BRESEN:
            break;
        case MIDDLE_POINT:
            break;
    }
}

void draw_pix(canvas_t &scene, const double &x, const double &y, const QColor &color)
{
    QPen pen = QPen(color);
    QBrush brush = QBrush(color);

    scene->addRect(x, y, 1, 1, pen, brush);
}
