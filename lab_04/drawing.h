#ifndef DRAWING_H
#define DRAWING_H

#include "structurs.h"

void drawing_all(canvas_t &scene, gv_t &gv, const content_t &data);
void drawing_circle(canvas_t &scene, gv_t &gv, const figure_t &data, const QColor &back, const bool &is_draw);
void drawing_ellipse(canvas_t &scene, gv_t &gv, const figure_t &data, const QColor &back, const bool &is_draw);
void draw_pix(canvas_t &scene, const double &x, const double &y, const QColor &color);

#endif // DRAWING_H
