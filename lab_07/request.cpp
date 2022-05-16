#include "request.h"
#include "cut.h"
#include "drawing.h"
#include "figure.h"

void cut_draw(content& data, canvas_t& scene, gv_t& view)
{
    cut(data);
    QImage image = QImage(view->geometry().width(), view->geometry().height(), QImage::Format_ARGB32);
    QPainter p(&image);
    image.fill(Qt::transparent);
    p.setPen(data.visible_color);
    p.setBrush(data.visible_color);
    for (size_t i = 0; i < data.visible_lines.size(); i++)
        draw_line(data.visible_lines[i], p);
    QPixmap pixmap = QPixmap::fromImage(image);
    scene->addPixmap(pixmap);
}

int request_handle(request& req)
{
    int rc = 0;
    switch (req.operation) {
    case ADD_POINT:
        rc = add_point(req.is_cut, req.p, req.number, req.data);
        break;
    case DRAW_ALL:
        draw_all(req.data, req.scene, req.view);
        break;
    case CUT:
        cut_draw(req.data, req.scene, req.view);
        break;
    }
    return rc;
}
