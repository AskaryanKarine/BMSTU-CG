#include "algorithms.h"
#include <cmath>

void standart_line(line_t &line, canvas_t scene)
{
    QLine l = QLine(line.start, line.end);
    scene->addLine(l, QPen(line.color));
}

void standart_spector(spectre_t &spector, canvas_t scene)
{
    int count = (int) 360 / spector.angle;
    for (int i = 0; i < count; i++)
    {
        int x = spector.center.x() + cos(M_PI * spector.angle * i / 180) * spector.radius;
        int y = spector.center.y() + sin(M_PI * spector.angle * i / 180) * spector.radius;
        QPoint cur_end = QPoint(x, y);
        QLine line = QLine(spector.center, cur_end);
        scene->addLine(line, QPen(spector.color));
    }
}
