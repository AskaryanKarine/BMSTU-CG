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
    QPointF center;
    figure_type_t type;
    double r1;
    double r2;
    method_t method;
    QColor color;
};

struct spectrum_t
{
    QPointF center;
    figure_type_t type;
    double r1;
    double r2;
    double dr1;
    double dr2;
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
