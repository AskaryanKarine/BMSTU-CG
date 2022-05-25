#ifndef DRAWING_H
#define DRAWING_H

#include "structurs.h"

void draw_all(const content& data, canvas_t& scene, gv_t& view);
void draw_polygon(const polygon& cut, QPainter& paint);

#endif // DRAWING_H
