#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "mainwindow.h"

void standart_line(line_t &line, canvas_t scene);

void dda_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

void bresen_double_line(line_t &line, canvas_t scene, bool is_drawing, bool is_cnt_steps);

//void bresen_int_line(line_t &line, canvas_t scene, bool steps);



#endif // ALGORITHMS_H
