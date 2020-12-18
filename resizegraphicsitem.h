#ifndef RESIZEGRAPHICSITEM_H
#define RESIZEGRAPHICSITEM_H

#include <QObject>
#include <QGraphicsRectItem>

class ResizeGraphicsItem : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    enum ResizeMode
    {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Top,
        Bottom,
        Left,
        Right,
    };
    Q_ENUM(ResizeMode)

    ResizeGraphicsItem() = default;
    ResizeGraphicsItem(ResizeMode mode, const QRectF &rect = QRectF(), QGraphicsItem *parent = nullptr, QObject *scene = nullptr);

public slots:
    void onItemRectChanged(QRectF rect, ResizeMode resize_mode);

signals:
    void positionChanged(QPointF pos, ResizeMode resize_mode);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
    ResizeMode ResizeModeValue;
};

#endif // RESIZEGRAPHICSITEM_H
