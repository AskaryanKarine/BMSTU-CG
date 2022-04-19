#include "request.h"
#include "table.h"
#include "drawing.h"
#include "fill.h"
#include <map>

void cancel_data(const content &data, QTableWidget *table, canvas_t &scene, gv_t &view)
{
    rewrite_table(data, table);
    drawing_points(scene, view, false, {0, 0}, data);
}

void delete_point(const indexes &ind_data, content &data, QTableWidget *table, canvas_t &scene, gv_t &view)
{
    del_row(ind_data, data);
    rewrite_table(data, table);
    drawing_points(scene, view, false, {0, 0}, data);
}

int change(const indexes &ind, const point &p, content &data, QTableWidget *table, canvas_t &scene, gv_t &view)
{
    int rc = change_point(ind, p, data);
    if (!rc)
    {
        rewrite_table(data, table);
        drawing_points(scene, view, true, p, data);
    }
    return rc;
}

void fill_all(content &data, const int delay, canvas_t &scene, gv_t &view, std::vector<double>& time)
{
    for (size_t i = 0; i < data.figures.size(); i++)
        fill_one(data.figures[i], delay, scene, view, time);
}

int request_handle(request &req)
{
    int rc = 0;
    switch (req.oper)
    {
        case DRAW:
            drawing_points(req.scene, req.view, req.is_smth, req.p, req.data);
            break;
        case FILL:
            fill_all(req.data, req.delay, req.scene, req.view, req.time);
            break;
        case ADD_POINT:
            rc = add_point(req.p, req.colors_data, req.is_smth, req.table, req.data);
            break;
        case DELETE_POINT:
            delete_point(req.indexes_data, req.data, req.table, req.scene, req.view);
            break;
        case CANCEL:
            cancel_data(req.data, req.table, req.scene, req.view);
            break;
        case CHANGE_POINT:
            rc = change(req.indexes_data, req.p, req.data, req.table, req.scene, req.view);
            break;
        case SELECT:
            select_point(req.scene, req.view, req.p, req.data);
            break;
    }
    return rc;
}
