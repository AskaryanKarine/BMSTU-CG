#ifndef REQUEST_H
#define REQUEST_H

#include "structurs.h"
#include <QTableWidget>

enum operation
{
    DRAW,
    FILL,
    ADD_POINT,
    DELETE_POINT
};

struct request
{
    operation oper;
    content data;
    canvas_t scene;
    gv_t view;
    QTableWidget *table;
    QColor fill;
    QColor line;
    point p;
    bool is_smth;
};

int request_handle(request &req);

#endif // REQUEST_H
