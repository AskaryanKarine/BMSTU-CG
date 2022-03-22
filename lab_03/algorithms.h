#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "mainwindow.h"

void standart_line(line_t &line, canvas_t scene);

int dda_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

int bresen_double_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

int bresen_int_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

int bresen_steps_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

int wy_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

#endif // ALGORITHMS_H
