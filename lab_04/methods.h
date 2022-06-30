#ifndef METHODS_H
#define METHODS_H

#include "structurs.h"


void standart_circle(canvas_t &scene, const figure_t &circle);
void standart_ellipse(canvas_t &scene, const figure_t &ellipse);
void canonical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw);
void canonical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw);
void parametrical_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw);
void parametrical_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw);
void bresen_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw);
void bresen_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw);
void middle_point_circle(canvas_t &scene, const figure_t &circle, const bool &is_draw);
void middle_point_ellipse(canvas_t &scene, const figure_t &ellipse, const bool &is_draw);

#endif // METHODS_H
