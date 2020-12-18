#include "bagroundpolygonitem.h"
#include <QGraphicsScene>

BackgroundPolygonItem::BackgroundPolygonItem(const QPolygonF &polygon/* = QPolygonF()*/, QGraphicsItem *parent/* = nullptr*/, QObject *scene/* = nullptr*/):
    QObject(scene),
    QGraphicsPolygonItem(polygon, parent)
{

}

void BackgroundPolygonItem::onItemRectChanged(QRectF bounding_rect)
{
    QPolygonF new_polygon = scene()->sceneRect();
    new_polygon = new_polygon.subtracted(bounding_rect);

    setPolygon(new_polygon);
}
