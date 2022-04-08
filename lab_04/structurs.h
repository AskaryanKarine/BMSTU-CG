#ifndef STRUCTURS_H
#define STRUCTURS_H

#include <QPointF>
#include <QColor>
#include <QGraphicsScene>
#include <QGraphicsView>

typedef QGraphicsScene *canvas_t;
typedef QGraphicsView *gv_t;

enum method_t
{
    STANDART,
    CANONICAL,
    PARAMETRIC,
    BRESEN,
    MIDDLE_POINT,
};


enum figure_type_t
{
    CIRCLE,
    ELLIPSE,
};

struct figure_t
{
    QPoint center;
    figure_type_t type;
    double ra;
    double rb;
    method_t method;
    QColor color;
};

struct spectrum_t
{
    QPoint center;
    figure_type_t type;
    double ra;
    double rb;
    double dra;
    double drb;
    int n;
    method_t method;
    QColor color;
};

struct content_t
{
    std::vector<figure_t> figures;
    std::vector<spectrum_t> spectrums;
    QColor back_color;
};

void copy(struct content_t **a, struct content_t *b);

#endif // STRUCTURS_H
