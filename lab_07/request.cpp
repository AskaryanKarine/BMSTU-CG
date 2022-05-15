#include "request.h"
#include "drawing.h"
#include "figure.h"

int request_handel(request& req)
{
    int rc = 0;
    switch (req.operation) {
    case ADD_POINT:
        rc = add_point(req.is_cut, req.p, req.number, req.data);
        break;
    case DRAW_ALL:
        draw_all(req.data, req.scene, req.view);
        break;
    }
    return rc;
}
