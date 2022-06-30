#ifndef DRAWING_H
#define DRAWING_H

#include "structurs.h"

void drawing_points(canvas_t &scene, gv_t &gv, const bool &is_sel, const point &selected_p, const content &data);
void draw_line(const point &p1, const point &p2, QPainter &p);
void select_point(canvas_t &scene, gv_t &gv, const point &selected_p, const content &data);

#endif // DRAWING_H
