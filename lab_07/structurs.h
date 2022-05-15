#ifndef STRUCTURS_H
#define STRUCTURS_H

#include <QGraphicsScene>
#include <QGraphicsView>

typedef QGraphicsScene* canvas_t;
typedef QGraphicsView* gv_t;

struct point {
    int x;
    int y;
};

enum draw_type {
    CUT,
    LINE
};

struct figure {
    draw_type dt;
    point p1;
    point p2;
};

struct content {
    figure cut;
    std::vector<figure> lines;
    QColor line_color;
    QColor visible_color;
    QColor cut_color;
};

#endif // STRUCTURS_H
