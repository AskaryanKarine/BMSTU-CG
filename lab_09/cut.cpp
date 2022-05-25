#include "cut.h"
#include "figure.h"
#include <cmath>

static int scalar_product(point p1, point p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}

static bool cut_line(line& res_line, const line& src, const polygon& cut)
{
    return true;
}

void cut(content& data)
{
    line visible_line;
}
