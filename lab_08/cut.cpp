#include "cut.h"
#include "figure.h"
#include <cmath>

int scalar_product(point p1, point p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}

bool cut_line(line& res_line, const line& src, const polygon& cut)
{
    int conv = check_convex(cut);
    size_t m = cut.lines.size();
    point D = src.p2 - src.p1;
    double lt = 0, ht = 1;
    for (size_t i = 0; i < m; i++) {
        point f = cut.lines[i].p1;
        point p2 = cut.lines[i].p2;
        point n_in = {-conv * (p2.y - f.y), conv * (p2.x - f.x)};
        point W = src.p1 - f;
        double Wsc = scalar_product(W, n_in);
        double Dsc = scalar_product(D, n_in);

        if (Dsc) {
            double t = -Wsc / Dsc;
            if (Dsc > 0) {
                if (t > 1)
                    return false;
                else if (t > lt)
                    lt = t;
            } else {
                if  (t < 0)
                    return false;
                else if (t < ht)
                    ht = t;
            }
        } else if (Wsc < 0)
            return false;
    }

    if (lt > ht)
        return false;

    point tmp = src.p2 - src.p1;
    point l_tmp = {int(tmp.x * lt), int(tmp.y * lt)};
    res_line.p1 = src.p1 + l_tmp;
    point h_tmp = {int(tmp.x * ht), int(tmp.y * ht)};
    res_line.p2 = src.p1 + h_tmp;

    return true;
}

void cut(content& data)
{
     line visible_line;
     for (line line : data.lines)
         if (cut_line(visible_line, line, data.cut))
             data.visible_lines.push_back(visible_line);
 }
