#include "fill.h"
#include "drawing.h"
#include <QCoreApplication>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <algorithm>
#include <map>

void update_y_group(point p_start, point p_end, std::map<int, std::vector<node>>& y_group, int& y_max, int& y_min)
{
    if (p_start.y > p_end.y) {
        std::swap(p_start.x, p_end.x);
        std::swap(p_start.y, p_end.y);
    }

    if (p_end.y > y_max)
        y_max = p_end.y;
    if (p_start.y < y_min)
        y_min = p_start.y;

    double dy = (p_end.y - p_start.y) ? (p_end.y - p_start.y) : 1;
    double dx = (double)((-(p_end.x - p_start.x)) / dy);
    if (dy != 1) {
        y_group[p_end.y].push_back({ double(p_end.x), dx, int(dy) });
    }
}

std::map<int, std::vector<node>> make_y_group(const figure& fig, int& y_max, int& y_min)
{
    std::map<int, std::vector<node>> y_group;
    for (size_t i = 0; i < fig.main_figure.size() - 1; i++)
        update_y_group(fig.main_figure[i], fig.main_figure[i + 1], y_group, y_max, y_min);
    update_y_group(fig.main_figure[0], fig.main_figure[fig.main_figure.size() - 1], y_group, y_max, y_min);
    for (size_t i = 0; i < fig.holes.size(); i++) {
        for (size_t j = 0; j < fig.holes[i].points.size() - 1; j++)
            update_y_group(fig.holes[i].points[j], fig.holes[i].points[j + 1], y_group, y_max, y_min);
        update_y_group(fig.holes[i].points[0], fig.holes[i].points[fig.holes[i].points.size() - 1], y_group, y_max, y_min);
    }

    return y_group;
}

void check_active_edges(std::vector<node>& active_edges)
{
    size_t i = 0;
    while (i < active_edges.size()) {
        active_edges[i].x += active_edges[i].dx;
        active_edges[i].dy--;
        if (active_edges[i].dy < 1)
            active_edges.erase(active_edges.begin() + i);
        else
            i++;
    }
}

bool comp(node a1, node a2)
{
    return a1.x >= a2.x;
}

void add_actove_edges(std::map<int, std::vector<node>>& y_group, std::vector<node>& active_edges, int y)
{
    if (y_group.count(y)) {
        for (size_t i = 0; i < y_group[y].size(); i++)
            active_edges.push_back(y_group[y][i]);
    }
    std::sort(active_edges.begin(), active_edges.end(), comp);
}

void sleep_feature(int sleep_time)
{
    QTime end = QTime::currentTime().addMSecs(sleep_time);
    while (QTime::currentTime() < end)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void draw_fast_str(QPainter& p, std::vector<node>& active_edges, int& y)
{
    for (size_t i = 0; i < active_edges.size() - 1; i += 2)
        p.drawLine((active_edges[i].x), y, qRound(active_edges[i + 1].x), y);
}

void draw_str(gv_t& view, canvas_t& scene, std::vector<node>& active_edges, int& y, QColor color)
{
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::transparent);

    p.setPen(color);
    p.setBrush(color);

    for (size_t i = 0; i < active_edges.size() - 1; i += 2)
        p.drawLine((active_edges[i].x), y, qRound(active_edges[i + 1].x), y);

    QPixmap pixmap = QPixmap::fromImage(image);
    QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
    item->update();
}

void fill(const int& delay, std::map<int, std::vector<node>>& y_group, std::vector<node>& active_edges,
    int y_min, int y_max, canvas_t& scene, gv_t& view, QColor color)
{
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::transparent);
    p.setPen(color);
    p.setBrush(color);
    while (y_max > y_min) {
        check_active_edges(active_edges);
        add_actove_edges(y_group, active_edges, y_max);
        if (delay) {
            sleep_feature(delay);
            draw_str(view, scene, active_edges, y_max, color);
        } else
            draw_fast_str(p, active_edges, y_max);
        y_max--;
    }
    QPixmap pixmap = QPixmap::fromImage(image);
    QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
    item->update();
}

void draw_countor(const figure& f, canvas_t& scene, gv_t& view)
{
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::transparent);

    p.setPen(f.line_color);
    p.setBrush(f.line_color);
    size_t size = f.main_figure.size();
    for (size_t j = 0; j < size - 1; j++)
        draw_line(f.main_figure[j], f.main_figure[j + 1], p);
    draw_line(f.main_figure[0], f.main_figure[f.main_figure.size() - 1], p);
    for (size_t j = 0; j < f.holes.size(); j++) {
        for (size_t k = 0; k < f.holes[j].points.size() - 1; k++)
            draw_line(f.holes[j].points[k], f.holes[j].points[k + 1], p);
        draw_line(f.holes[j].points[0], f.holes[j].points[f.holes[j].points.size() - 1], p);
    }
    QPixmap pixmap = QPixmap::fromImage(image);
    QGraphicsPixmapItem* item = scene->addPixmap(pixmap);
    item->update();
}

void fill_one(const figure& f, const int& delay, canvas_t& scene, gv_t& view, std::vector<double>& time)
{
    int y_max = 0, y_min = 1000;
    std::map<int, std::vector<node>> y_group = make_y_group(f, y_max, y_min);
    std::vector<node> active_edges;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;
    auto cur_time = high_resolution_clock::now();
    fill(delay, y_group, active_edges, y_min, y_max, scene, view, f.fill_color);
    time.push_back((double)duration_cast<microseconds>(high_resolution_clock::now() - cur_time).count());
    draw_countor(f, scene, view);
}
