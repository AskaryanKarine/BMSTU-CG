#include "figure.h"

int add_point(const bool& is_cut, const point& p, const int& numb, content& data)
{
    int rc = 0;
    if (is_cut) {
        if (numb % 2 == 1) {
            data.cut.p1 = p;
            data.cut.p2 = { -1, -1 };
        } else {
            if (data.cut.p1 != p && (data.cut.p1.x - p.x != 0 && data.cut.p1.y - p.y != 0))
                data.cut.p2 = p;
            else
                rc = 1;
        }
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
