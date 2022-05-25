#ifndef STRUCTURS_H
#define STRUCTURS_H

#include <QGraphicsScene>
#include <QGraphicsView>

typedef QGraphicsScene* canvas_t;
typedef QGraphicsView* gv_t;

struct point {
    int x = 0;
    int y = 0;

    bool is_null() { return !(x > 0 && y > 0); };
    bool is_null() const { return !(x > 0 && y > 0); };
    bool operator==(const point& p) { return x == p.x && y == p.y; };
    bool operator!=(const point& p) { return !operator==(p); };
    point operator-(const point& p) const { return { x - p.x, y - p.y }; };
    point operator+(const point& p) const { return { x + p.x, y + p.y }; };
};

struct line {
    point p1;
    point p2;

    bool is_full() { return !(p1.is_null() || p2.is_null()); };
    bool is_full() const { return !(p1.is_null() || p2.is_null()); };
};

struct polygon {
    std::vector<point> points;
    std::vector<line> lines = {};
    bool is_close;

    void update_lines(const point& p)
    {
        if (lines.empty()) {
            lines.push_back({});
            lines[0].p1 = p;
        } else {
            size_t last = lines.size() - 1;
            lines[last].p2 = p;
            if (!is_close) {
                lines.push_back({});
                lines[++last].p1 = p;
            }
        }
    }
};

struct content {
    polygon cutter;
    polygon firure;
    polygon visible_figure;
    QColor line_color;
    QColor visible_color;
    QColor cut_color;
};

#endif // STRUCTURS_H
