#include "request.h"
#include "drawing.h"

int add_point(const point &p, const QColor line, const QColor fill, const bool &is_hole, QTableWidget *table, content &data)
{
    if (!is_hole)
    {
        if (data.figures[data.n_figures].is_closed_figure)
        {
            data.figures[data.n_figures].is_closed_figure = false;
            data.n_figures++;
            data.n_holes = -1;
            figure f;
            f.fill_color = fill;
            f.line_color = line;
            f.is_closed_figure = false;
            data.figures.push_back(f);
        }
        std::vector<point> arr = data.figures[data.n_figures].main_figure;
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i].x == p.x && arr[i].y == p.y)
                return 1;

        data.figures[data.n_figures].main_figure.push_back(p);
    }
    else
    {
        std::vector<point> arr = data.figures[data.n_figures].holes[data.n_holes].points;
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i].x == p.x && arr[i].y == p.y)
                return 1;
        data.figures[data.n_figures].holes[data.n_holes].points.push_back(p);
    }

    int cnt = table->rowCount();
    table->insertRow(cnt);
    table->setItem(cnt, 0, new QTableWidgetItem(QString::number(p.x)));
    table->setItem(cnt, 1, new QTableWidgetItem(QString::number(p.y)));
    table->setItem(cnt, 2, new QTableWidgetItem(QString::number(data.n_figures)));
    table->setItem(cnt, 3, new QTableWidgetItem(QString::number(data.n_holes)));
    return 0;
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
            break;
        case ADD_POINT:
            rc = add_point(req.p, req.line, req.fill, req.is_smth, req.table, req.data);
            break;
        case DELETE_POINT:
            break;
    }
    return rc;
}
