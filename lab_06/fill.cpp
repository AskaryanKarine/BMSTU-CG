#include "fill.h"
#include "drawing.h"
#include <map>
#include <stack>
#include <QTime>
#include <QCoreApplication>

void sleep_feature(int sleep_time)
{
    QTime end = QTime::currentTime().addMSecs(sleep_time);
    while (QTime::currentTime() < end)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1);
}

void draw_pix(const point &point, QPainter &painter, const int &delay)
{
    if (delay)
        sleep_feature(delay);
    painter.drawPoint(point.x, point.y);
}

void draw_countor(const figure &f, QPainter &p)
{
    p.setPen(f.line_color);
    p.setBrush(f.line_color);
    size_t size = f.main_figure.size();
    for (size_t j = 0; j < size - 1; j++)
        draw_line(f.main_figure[j], f.main_figure[j + 1], p);
    if (f.is_closed_figure)
        draw_line(f.main_figure[0], f.main_figure[f.main_figure.size() - 1], p);
    for (size_t j = 0; j < f.holes.size(); j++)
    {
        for (size_t k = 0; k < f.holes[j].points.size() - 1; k++)
            draw_line(f.holes[j].points[k], f.holes[j].points[k + 1], p);
        if (f.holes[j].is_closed_hole)
            draw_line(f.holes[j].points[0], f.holes[j].points[f.holes[j].points.size() - 1], p);
    }

}

int fill(const content &data, const int &delay, canvas_t &scene, gv_t &view, std::vector<double>& time)
{
    scene->clear();
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::transparent);
    for (size_t i = 0; i < data.figures.size(); i++)
        draw_countor(data.figures[i], p);

    p.setPen(data.back_color);
    p.setBrush(data.back_color);

    std::stack<point> seeds_points;
    seeds_points.push(data.seed_point);

    using std::chrono::duration;
    using std::chrono::duration_cast;
    using std::chrono::high_resolution_clock;
    using std::chrono::microseconds;

    auto end = high_resolution_clock::now();
    auto start = high_resolution_clock::now();

    while (!seeds_points.empty())
    {
        point cur_p = seeds_points.top();
        seeds_points.pop();
        draw_pix(cur_p, p, delay);

        int rx = cur_p.x + 1;
        int lx = cur_p.x - 1;
        while (true) {
            if (rx >= view->geometry().width())
                return 1;

            QColor color = image.pixelColor(rx, cur_p.y);

            if (color != Qt::transparent) {
                rx--;
                break;
            }
            draw_pix({rx, cur_p.y}, p, delay);
            rx++;
        }

        while (true) {
            if (lx < 0)
                return 1;

            QColor color = image.pixelColor(lx, cur_p.y);

            if (color != Qt::transparent) {
                lx++;
                break;
            }
            draw_pix({lx, cur_p.y}, p, delay);
            lx--;
        }

        point new_seed_p = {};
        int y = cur_p.y - 1;

        bool flag = false;
        for (int x = lx; x <= rx; x++)
        {
            if (image.pixelColor(x, y) == Qt::transparent && image.pixelColor(x + 1, y) != Qt::transparent)
            {
                seeds_points.push({x, y});
                flag = true;
            }
            else if (image.pixelColor(x, y) == Qt::transparent)
                new_seed_p = {x, y};
        }
        if ((new_seed_p.x && new_seed_p.y) && !flag)
            seeds_points.push(new_seed_p);
        new_seed_p = {};
        y = cur_p.y + 1;
        flag = false;
        for (int x = lx; x <= rx; x++)
        {
            if (image.pixelColor(x, y) == Qt::transparent && image.pixelColor(x + 1, y) != Qt::transparent)
            {
                seeds_points.push({x, y});
                flag = true;
            }
            else if (image.pixelColor(x, y) == Qt::transparent)
                new_seed_p = {x, y};
        }
        if ((new_seed_p.x && new_seed_p.y) && !flag)
            seeds_points.push(new_seed_p);

    }

    end = high_resolution_clock::now();

    time.push_back((double)duration_cast<microseconds>(end - start).count());

    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
    return 0;
}
