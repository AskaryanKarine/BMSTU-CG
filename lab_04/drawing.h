#ifndef DRAWING_H
#define DRAWING_H

#include "structurs.h"

void drawing_all(canvas_t &scene, gv_t &gv, const content_t &data);
void drawing_circle(canvas_t &scene, gv_t &gv, const figure_t &data, const QColor &back, const bool &is_draw);
void drawing_ellipse(canvas_t &scene, gv_t &gv, const figure_t &data, const QColor &back, const bool &is_draw);
void drawing_spectrum_circle(canvas_t &scene, gv_t &gv, const spectrum_t &spectrum, const QColor &back);
void drawing_spectrum_ellispe(canvas_t &scene, gv_t &gv, const spectrum_t &spectrum, const QColor &back);
void draw_reflect_circle(canvas_t &scene, const QPointF &point, const QPointF &center, const QColor &color);
void draw_reflect_ellipse(canvas_t &scene, const QPointF &point, const QPointF &center, const QColor &color);

#endif // DRAWING_H
