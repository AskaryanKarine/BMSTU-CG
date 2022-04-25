#ifndef STRUCTURS_H
#define STRUCTURS_H

#include <QGraphicsScene>
#include <QGraphicsView>

typedef QGraphicsScene *canvas_t;
typedef QGraphicsView *gv_t;

struct point
{
    int x;
    int y;
};

struct colors
{
    QColor line;
    QColor fill;
};

struct indexes
{
    int fig;
    int hole;
    size_t index;
};

struct node
{
    double x;
    double dx;
    int dy;
};

struct hole
{
    std::vector<point> points;
    bool is_closed_hole;
};

struct figure
{
    std::vector<point> main_figure;
    std::vector<hole> holes;
    QColor line_color;
    QColor fill_color;
    bool is_closed_figure;
};

struct content
{
    std::vector<figure> figures;
    QColor back_color;
    int n_figures;
    int n_holes;
    point seed_point;
};

#endif // STRUCTURS_H
