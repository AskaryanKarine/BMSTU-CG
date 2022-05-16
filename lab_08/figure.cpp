#include "figure.h"

int add_point(const bool& is_cut, const point& p, content& data)
{
    int rc = 0;
    if (is_cut) {
        if (data.cut.is_close) {
            data.cut.is_close = false;
            data.cut.points.clear();
        }
        data.cut.points.push_back(p);
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

bool cross(const figure &f1, const figure &f2)
{
    if (f1.p2.y - f1.p1.y != 0)
    {
        double q = static_cast<double>(f1.p2.x - f1.p1.x) / (f1.p1.y - f1.p2.y);
        double sn = (f2.p1.x - f2.p2.x) + (f2.p1.y - f2.p2.y) * q;
        if (!sn)
            return false;
    }
    else
        if (!(f2.p1.y - f2.p2.y))
            return false;
    return true;
}

int vector_product(const point &v1, const point &v2)
{
    return v1.x * v2.y - v1.y * v2.x;
}

int sign(int num)
{
    if (num == 0)
        return 0;
    return (num > 0) ? 1 : -1;
}

void get_coef(point a, point c, double &d, double &k, double &b)
{
    if (c.x == a.x)
    {
        k = 1;
        b = - c.x;
        d = 0;
    }
    else
    {
        k = 1.0 * (c.y - a.y) / (c.x - a.x);
        b = c.y - (k * c.x);
        d = 1;
    }
}

bool cross(point a, point b, point c, point d)
{
    double eps = 1e6;
    double dab, kab, bab;
    double dcd, kcd, bcd;
    get_coef(a, b, dab, kab, bab);
    get_coef(c, d, dcd, kcd, bcd);

    if (abs(kab - kcd ) < eps)
        return false;
    double x = (bcd - bab) / (kab - kcd);
    double y;

    if (dcd < eps)
        y = kab * x + bab;
    else if (dab < eps)
        y = kcd * x + bcd;
    else
        y = kab * x + bab;
    int b1 = a.x;
    int b2 = b.x;
    a.x = std::max(b1, b2);
    b.x = std::min(b1, b2);
    b1 = a.y;
    b2 = b.y;
    a.y = std::max(b1, b2);
    b.y = std::min(b1, b2);

    if ((abs(b.x - x) < eps) || (abs(a.x - x) < eps) || (abs(b.y - y) < eps) || (abs(a.y - y) < eps))
        return false;
    if (b.x < x && x < a.x && b.y < y && y < a.y)
        return true;
    else
        return false;
}

bool check_cross(const cut_rect &cut)
{
    bool flag;
    size_t size = cut.points.size();
    for (size_t i = 0; i < size - 1; i++)
        for (size_t j = i + 1; j < size; j++)
        {
            if (j == size - 1)
            {
                flag = cross(cut.points[i], cut.points[i + 1], cut.points[j], cut.points[0]);
                if (flag)
                    return true;
            }
            else
            {
                flag = cross(cut.points[i], cut.points[i + 1], cut.points[j], cut.points[j + 1]);
                if (flag)
                    return true;
            }
        }
    return false;
}

int check_cut(const cut_rect &cut)
{
    size_t size = cut.points.size();
    if (size < 3)
        return false;

    point v1 = cut.points[0] - cut.points[size - 1];
    point v2 = cut.points[size - 1] - cut.points[size - 2];

    int prev = sign(vector_product(v1, v2));

    for (size_t i = 1; i < size - 2; i++)
    {
        v1 = cut.points[i] - cut.points[i - 1];
        if (i == 1)
            v2 = cut.points[i - 1] - cut.points[size - 1];
        else
            v2 = cut.points[i - 1] - cut.points[i - 2];

        int cur = sign(vector_product(v1, v2));
        if (prev != cur)
            return 0;
    }

    if (check_cross(cut))
        return 0;

    return prev;
}
