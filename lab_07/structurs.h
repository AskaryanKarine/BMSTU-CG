#ifndef STRUCTURS_H
#define STRUCTURS_H

#include <QGraphicsScene>
#include <QGraphicsView>

typedef QGraphicsScene* canvas_t;
typedef QGraphicsView* gv_t;

struct point {
    int x = -1;
    int y = -1;

    bool is_null() { return !(x > 0 && y > 0); };
    bool is_null() const { return !(x > 0 && y > 0); };
    bool operator==(const point& p) { return x == p.x && y == p.y; };
    bool operator!=(const point& p) { return !operator==(p); };
    point operator-(const point& p) const { return { x - p.x, y - p.y }; };
};

struct figure {
    point p1;
    point p2;

    bool is_full() { return !(p1.is_null() || p2.is_null()); };
    bool is_full() const { return !(p1.is_null() || p2.is_null()); };
};

struct content {
    figure cut;
    std::vector<figure> lines;
    QColor line_color;
    QColor visible_color;
    QColor cut_color;
    int number_cut = 0;
    bool process = false;
};

#endif // STRUCTURS_H