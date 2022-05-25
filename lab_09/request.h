#ifndef REQUEST_H
#define REQUEST_H

#include "structurs.h"

enum type_operation {
    ADD_POINT,
    DRAW_ALL,
    CUT
};

struct request {
    type_operation operation;
    content data;
    bool is_cut;
    canvas_t scene;
    gv_t view;
    point p;
    int number;
};

int request_handle(request& req);

#endif // REQUEST_H
