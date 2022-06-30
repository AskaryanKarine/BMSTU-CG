#include "table.h"
#include <QPolygon>
void add_row(const point &p, const int &n_figures, const int &n_holes, const size_t &i, QTableWidget *table)
{
    table->blockSignals(true);
    int cnt = table->rowCount();
    table->insertRow(cnt);
    table->setItem(cnt, 0, new QTableWidgetItem(QString::number(p.x)));
    table->setItem(cnt, 1, new QTableWidgetItem(QString::number(p.y)));
    QTableWidgetItem *item = new QTableWidgetItem(QString::number(n_figures));
    item->setFlags(Qt::ItemIsEnabled);
    table->setItem(cnt, 2, item);
    item = new QTableWidgetItem(QString::number(n_holes));
    item->setFlags(Qt::ItemIsEnabled);
    table->setItem(cnt, 3, item);
    table->setItem(cnt, 4, new QTableWidgetItem(QString::number(i)));
    table->blockSignals(false);
}

int add_point(const point &p, const colors col, const bool &is_hole, QTableWidget *table, content &data)
{
    size_t i;
    if (!is_hole)
    {
        if (data.figures[data.n_figures].is_closed_figure)
        {
//            data.figures[data.n_figures].is_closed_figure = false;
            data.n_figures++;
            data.n_holes = -1;
            figure f;
            f.fill_color = col.fill;
            f.line_color = col.line;
            f.is_closed_figure = false;
            data.figures.push_back(f);
        }
        std::vector<point> arr = data.figures[data.n_figures].main_figure;
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i].x == p.x && arr[i].y == p.y)
                return 1;

        data.figures[data.n_figures].main_figure.push_back(p);
        i = data.figures[data.n_figures].main_figure.size() - 1;
    }
    else
    {
        std::vector<point> arr = data.figures[data.n_figures].holes[data.n_holes].points;
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i].x == p.x && arr[i].y == p.y)
                return 1;
        QPolygon pol;
        for (size_t i = 0; i < data.figures[data.n_figures].main_figure.size(); i++)
        {
            QPoint point = QPoint(data.figures[data.n_figures].main_figure[i].x, data.figures[data.n_figures].main_figure[i].y);
            pol << point;
        }
        QPoint point = QPoint(data.figures[data.n_figures].main_figure[0].x, data.figures[data.n_figures].main_figure[0].y);
        pol << point;
        bool c = pol.containsPoint(QPoint(p.x, p.y), Qt::OddEvenFill);
        if (!c)
            return 2;
        data.figures[data.n_figures].holes[data.n_holes].points.push_back(p);
        i = data.figures[data.n_figures].holes[data.n_holes].points.size() - 1;
    }

    add_row(p, data.n_figures, data.n_holes, i, table);

    return 0;
}

void rewrite_table(const content &data, QTableWidget *table)
{
    table->clearContents();
    table->setRowCount(0);
    for (size_t i = 0; i < data.figures.size(); i++)
    {
        figure cur_f = data.figures[i];
        for (size_t j = 0; j < cur_f.main_figure.size(); j++)
            add_row(cur_f.main_figure[j], i, -1, j, table);
        for (size_t j = 0; j < cur_f.holes.size(); j++)
            for (size_t k = 0; k < cur_f.holes[j].points.size(); k++)
                add_row(cur_f.holes[j].points[k], i, j, k, table);
    }
}

void del_row(const indexes &ind_data, content &data)
{
    if (ind_data.hole == -1)
    {
        data.figures[ind_data.fig].main_figure.erase(data.figures[ind_data.fig].main_figure.begin() + ind_data.index);
        if (data.figures.size() != 1 && data.figures[ind_data.fig].main_figure.size() == 0)
        {
            data.figures.erase(data.figures.begin() + ind_data.fig);
            data.n_figures--;
        }
        if (data.figures[ind_data.fig].main_figure.size() == 1 || data.figures[ind_data.fig].main_figure.size() == 2)
            data.figures[ind_data.fig].is_closed_figure = false;
    }
    else
    {
        data.figures[ind_data.fig].holes[ind_data.hole].points.erase(data.figures[ind_data.fig].holes[ind_data.hole].points.begin() + ind_data.index);
        if (data.figures[ind_data.fig].holes.size() != 1 && data.figures[ind_data.fig].holes[ind_data.hole].points.size() == 0)
        {
            data.figures[ind_data.fig].holes.erase(data.figures[ind_data.fig].holes.begin() + ind_data.hole);
            if (ind_data.fig == (int) (data.figures.size() - 1))
                data.n_holes--;
        }
        if (data.figures[ind_data.fig].holes[ind_data.hole].points.size() == 1 || data.figures[ind_data.fig].holes[ind_data.hole].points.size() == 2)
            data.figures[ind_data.fig].holes[ind_data.hole].is_closed_hole = false;
    }
}

int change_point(const indexes &ind, const point &p, content &data)
{
    if (ind.hole == -1)
    {
        std::vector<point> arr = data.figures[data.n_figures].main_figure;
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i].x == p.x && arr[i].y == p.y)
                return 1;
        data.figures[ind.fig].main_figure[ind.index] = p;
    }
    else
    {
        std::vector<point> arr = data.figures[data.n_figures].holes[data.n_holes].points;
        for (size_t i = 0; i < arr.size(); i++)
            if (arr[i].x == p.x && arr[i].y == p.y)
                return 1;
        data.figures[ind.fig].holes[ind.hole].points[ind.index] = p;
    }

    return 0;
}

