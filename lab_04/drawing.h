#ifndef DRAWING_H
#define DRAWING_H

#include "structurs.h"

void drawing_all(canvas_t &scene, gv_t &gv, const content_t &data);
void drawing_circle(canvas_t &scene, const figure_t &data, const bool &is_draw);
void drawing_ellipse(canvas_t &scene, const figure_t &data, const bool &is_draw);
void drawing_spectrum_circle(canvas_t &scene, const spectrum_t &spectrum);
void drawing_spectrum_ellispe(canvas_t &scene, const spectrum_t &spectrum);
void draw_reflect_circle(canvas_t &scene, const QPointF &point, const QPointF &center, const QColor &color);
void draw_reflect_ellipse(canvas_t &scene, const QPointF &point, const QPointF &center, const QColor &color);

#endif // DRAWING_H
