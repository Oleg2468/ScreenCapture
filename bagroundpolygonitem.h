#ifndef BAGROUNDPOLYGONITEM_H
#define BAGROUNDPOLYGONITEM_H

#include <QObject>
#include <QGraphicsPolygonItem>

class BackgroundPolygonItem : public QObject, public QGraphicsPolygonItem
{
    Q_OBJECT
public:
    BackgroundPolygonItem(const QPolygonF &polygon = QPolygonF(), QGraphicsItem *parent = nullptr, QObject *scene = nullptr);

public slots:
    void onItemRectChanged(QRectF bounding_rect);
};

#endif // BAGROUNDPOLYGONITEM_H
