#include "cut.h"
#include <cmath>

int find_point_code(point_code_t& code, const point& p, const figure& cut)
{
    int min_x = cut.p1.x < cut.p2.x ? cut.p1.x : cut.p2.x;
    int max_x = cut.p1.x > cut.p2.x ? cut.p1.x : cut.p2.x;
    int min_y = cut.p1.y < cut.p2.y ? cut.p1.y : cut.p2.y;
    int max_y = cut.p1.y > cut.p2.y ? cut.p1.y : cut.p2.y;

    for (size_t i = 0; i < 4; i++)
        code[i] = false;

    if (p.x < min_x)
        code[0] = true;
    else if (p.x > max_x)
        code[1] = true;
    if (p.y < min_y)
        code[2] = true;
    else if (p.y > max_y)
        code[3] = true;

    int sum = 0;
    for (size_t i = 0; i < 4; i++)
        sum += code[i];
    return sum;
}

bool log_mult(const point_code_t& T1, const point_code_t& T2)
{
    bool mult = 0;
    for (size_t i = 0; i < 4; i++)
        mult += T1[i] && T2[i];
    return mult;
}

point find_r(const point& p1, const point& p2, const point& q, const figure& cut, bool& flag)
{
    int left = cut.p1.x < cut.p2.x ? cut.p1.x : cut.p2.x;
    int right = cut.p1.x > cut.p2.x ? cut.p1.x : cut.p2.x;
    int bottom = cut.p1.y < cut.p2.y ? cut.p1.y : cut.p2.y;
    int top = cut.p1.y > cut.p2.y ? cut.p1.y : cut.p2.y;

    double m = pow(10, 30);
    if (p1.x != p2.x) {
        m = static_cast<double>(p2.y - p1.y) / (p2.x - p1.x);
        if (q.x <= left) {
            int y = static_cast<int>(round(m * (left - q.x))) + q.y;
            if (y >= bottom && y <= top)
                return { left, y };
        }
        if (q.x >= right) {
            int y = static_cast<int>(round(m * (right - q.x))) + q.y;
            if (y >= bottom && y <= top)
                return { right, y };
        }
    }
    if (qFuzzyCompare(m, 0)) {
        flag = false;
        return q;
    }
    if (q.y >= top) {
        int x = static_cast<int>(round((top - q.y) / m)) + q.x;
        if (x >= left && x <= right)
            return { x, top };
    }
    if (q.y <= bottom) {
        int x = static_cast<int>(round((bottom - q.y) / m)) + q.x;
        if (x >= left && x <= right)
            return { x, bottom };
    }
    flag = false;
    return q;
}

bool cut_line(figure& res_line, const figure& src, const figure& cut)
{
    bool flag = true;

    point_code_t T1, T2;
    int S1 = find_point_code(T1, src.p1, cut);
    int S2 = find_point_code(T2, src.p2, cut);

    point R1 = { 0, 0 }, R2 = { 0, 0 };

    if (S1 == 0 && S2 == 0) {
        R1 = src.p1;
        R2 = src.p2;
    } else {
        int p = log_mult(T1, T2);
        if (p != 0)
            flag = false;
        else {
            int i = 0;
            point Q = { 0, 0 };
            if (S1 == 0) {
                R1 = src.p1;
                Q = src.p2;
                i = 1;
            } else if (S2 == 0) {
                R1 = src.p2;
                Q = src.p1;
                i = 1;
            }
            while (i < 2) {
                i++;
                if (i == 1) {
                    Q = src.p1;
                    R1 = find_r(src.p1, src.p2, Q, cut, flag);
                    Q = src.p2;
                } else
                    R2 = find_r(src.p1, src.p2, Q, cut, flag);
            }
        }
    }
    if (flag) {
        res_line.p1 = R1;
        res_line.p2 = R2;
        return true;
    }

    return flag;
}

void cut(content& data)
{
    figure visible_line;
    for (figure line : data.lines)
        if (cut_line(visible_line, line, data.cut))
            data.visible_lines.push_back(visible_line);
}
