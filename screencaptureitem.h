#ifndef SCREENCAPTUREITEM_H
#define SCREENCAPTUREITEM_H

#include "resizegraphicsitem.h"
#include <QGraphicsRectItem>
#include <QObject>

const int CaptureItemwidth = 4;
const QColor FrameColor = QColor(0, 140, 255, 255);
const QColor TransparentColor = QColor(0, 0, 0, 0);

class ScreenCaptureItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    ScreenCaptureItem(const QRectF &rect = QRectF(), QGraphicsItem *parent = nullptr, QObject *scene = nullptr);
    ~ScreenCaptureItem();

    void addResizeItems(QGraphicsScene *scene);
    void addResizeItem(QRectF rect, ResizeGraphicsItem::ResizeMode resize_mode, QGraphicsScene *scene);

signals:
    void sizeRectChanged(QRectF rect, ResizeGraphicsItem::ResizeMode resize_mode);
    void positionRectChanged(QRectF rect);
    void posChanged(QPoint pos);
    void sizeChanged(QSize size);

public slots:
    void onResizeItemChanged(QPointF pos, ResizeGraphicsItem::ResizeMode resize_mode);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
};

#endif // SCREENCAPTUREITEM_H
