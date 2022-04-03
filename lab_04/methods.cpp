#include "methods.h"
#include "QRectF"
#include "drawing.h"

void standart_circle(canvas_t &scene, const figure_t &circle)
{
    QRectF rect = QRectF(circle.center.x() - circle.r1, circle.center.y() - circle.r1, 2 * circle.r1, 2 * circle.r1);
    scene->addEllipse(rect, QPen(circle.color, 1), QBrush(QColor(0,0,0,0)));
}

void standart_ellipse(canvas_t &scene, const figure_t &ellipse)
{
    QRectF rect = QRectF(ellipse.center.x() - ellipse.r1, ellipse.center.y() - ellipse.r2, 2 * ellipse.r1, 2 * ellipse.r2);
    scene->addEllipse(rect, QPen(ellipse.color, 1), QBrush(QColor(0,0,0,0)));
}
