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
    operation_t operation;
    figure_t figure;
    spectrum_t spectrum;
    content_t data;
};

void request_handle(request &req);

#endif // REQUEST_H
