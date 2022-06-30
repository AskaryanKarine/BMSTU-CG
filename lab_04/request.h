#ifndef REQUEST_H
#define REQUEST_H

#include "structurs.h"

enum operation_t
{
    DRAW_CIRCLE,
    DRAW_ELLIPSE,
    DRAW_SPECTRUM_CIRCLE,
    DRAW_SPECTRUM_ELLIPSE,
    DRAW_ALL,
    MEASURE_TIME
};

struct request
{
    canvas_t scene;
    gv_t gv;
    operation_t operation;
    figure_t figure;
    spectrum_t spectrum;
    content_t data;
};

void request_handle(request &req);
void measure_time(canvas_t &scene, spectrum_t &spectrum);

#endif // REQUEST_H
