#include "request.h"
#include "drawing.h"


void request_handle(request &req)
{
    switch (req.operation)
    {
        case DRAW_CIRCLE:
            drawing_circle(req.scene, req.gv, req.figure, req.back_color, true);
            break;
        case DRAW_ELLIPSE:
            drawing_ellipse(req.scene, req.gv, req.figure, req.back_color, true);
            break;
        case DRAW_SPECTRUM_CIRCLE:
            break;
        case DRAW_SPECTRUM_ELLIPSE:
            break;
        case DRAW_ALL:
            drawing_all(req.scene, req.gv, req.data);
            break;
        case MEASURE_TIME:
            break;
    }
}
