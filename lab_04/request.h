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
    CLEAR_CANVAS,
    MEASURE_TIME
};

struct request_t
{
    operation_t operation;
    canvas_t scene;
    union
    {
//        figure_t figure;
//        spectrum_t data_spectrum;
//        content_t data_content;
    };
};
void request_handel(request_t &req);

#endif // REQUEST_H
