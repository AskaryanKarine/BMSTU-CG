#include "figure.h"

int add_point(const bool& is_cut, const point& p, content& data)
{
    int rc = 0;
    if (is_cut) {
        if (data.cut.is_close) {
            data.cut.is_close = false;
            data.cut.points.clear();
            data.cut.lines.clear();
        }
        data.cut.points.push_back(p);
        data.cut.update_lines(p);
    } else {
        size_t last_id = data.lines.size() - 1;
        if (data.lines[last_id].p1.is_null())
            data.lines[last_id].p1 = p;
        else {
            if (data.lines[last_id].p1 == p)
                rc = 2;
            else {
                data.lines[last_id].p2 = p;
                data.lines.push_back({});
            }
        }
    }
    return rc;
}

int vector_product(const point &v1, const point &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

int sign(int num)
{
    if (num == 0)
        return 0;
    return (num >= 0) ? 1 : -1;
}


bool check_cross(const polygon &cut)
{

    for (size_t i = 0; i < cut.lines.size() - 1; i++)
    {
        point p1 = cut.lines[i].p1;
        point p2 = cut.lines[i].p2;
        for (size_t j = i + 1; j < cut.lines.size(); j++) {
            point p3 = cut.lines[j].p1;
            point p4 = cut.lines[j].p2;
            int v1 = vector_product(p3 - p4, p3 - p1);
            int v2 = vector_product(p3 - p4, p3 - p2);
            int v3 = vector_product(p1 - p2, p1 - p3);
            int v4 = vector_product(p1 - p2, p1 - p4);
            if (v1 * v2 < 0 && v3 * v4 < 0)
                return true;
        }
    }
    return false;
}

point get_vector(line l)
{
    return {l.p2.x - l.p1.x, l.p2.y - l.p1.y};
}

int check_convex(const polygon &cut)
{
    size_t size = cut.points.size();
    if (size < 3)
        return false;

    point v1 = get_vector(cut.lines[cut.lines.size() - 1]);
    point v2 = get_vector(cut.lines[0]);

    int prev = sign(vector_product(v1, v2));

    for (size_t i = 0; i < cut.lines.size() - 1; i++)
    {
        v1 = get_vector(cut.lines[i]);
        v2 = get_vector(cut.lines[i + 1]);

        int cur = sign(vector_product(v1, v2));
        // если случай с двумя отрезками на одной прямой валиден
        if (cur == 0 || prev == 0)
            continue;
        //
        if (prev != cur)
            return 0;
        prev = cur;
    }

    return prev;
}
