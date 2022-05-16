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
