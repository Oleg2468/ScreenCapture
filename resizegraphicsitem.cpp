#include "resizegraphicsitem.h"
#include <QCursor>
#include <QGraphicsScene>

ResizeGraphicsItem::ResizeGraphicsItem(ResizeMode mode, const QRectF &rect/* = QRectF()*/, QGraphicsItem *parent/* = nullptr*/, QObject *scene/* = nullptr*/) :
    QObject(scene),
    QGraphicsRectItem(rect, parent),
    ResizeModeValue(mode)
{
    switch (ResizeModeValue)
    {
        case TopLeft:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case BottomLeft:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case TopRight:
            setCursor(Qt::SizeBDiagCursor);
            break;
        case BottomRight:
            setCursor(Qt::SizeFDiagCursor);
            break;
        case Right:
        case Left:
            setCursor(Qt::SizeHorCursor);
            break;
        case Top:
        case Bottom:
            setCursor(Qt::SizeVerCursor);
            break;
    }
}

void ResizeGraphicsItem::onItemRectChanged(QRectF parent_rect, ResizeMode resize_mode)
{
    if (ResizeModeValue == resize_mode)
        return;

    parent_rect = mapFromScene(parent_rect).boundingRect();
    QRectF item_rect = rect();
    auto item_height_half = item_rect.height() / 2;
    auto item_width_half = item_rect.width() / 2;

    switch (ResizeModeValue)
    {
        case TopLeft:
            item_rect.setTop(parent_rect.top() - item_height_half);
            item_rect.setBottom(parent_rect.top() + item_height_half);
            item_rect.setLeft(parent_rect.left() - item_width_half);
            item_rect.setRight(parent_rect.left() + item_width_half);
            break;
        case TopRight:
            item_rect.setTop(parent_rect.top() - item_height_half);
            item_rect.setBottom(parent_rect.top() + item_height_half);
            item_rect.setLeft(parent_rect.right() - item_width_half);
            item_rect.setRight(parent_rect.right() + item_width_half);
            break;
        case BottomLeft:
            item_rect.setTop(parent_rect.bottom() - item_height_half);
            item_rect.setBottom(parent_rect.bottom() + item_height_half);
            item_rect.setLeft(parent_rect.left() - item_width_half);
            item_rect.setRight(parent_rect.left() + item_width_half);
            break;
        case BottomRight:
            item_rect.setTop(parent_rect.bottom() - item_height_half);
            item_rect.setBottom(parent_rect.bottom() + item_height_half);
            item_rect.setLeft(parent_rect.right() - item_width_half);
            item_rect.setRight(parent_rect.right() + item_width_half);
            break;
        case Right:
            item_rect.setTop(parent_rect.center().y() - item_height_half);
            item_rect.setBottom(parent_rect.center().y() + item_height_half);
            item_rect.setLeft(parent_rect.right() - item_width_half);
            item_rect.setRight(parent_rect.right() + item_width_half);
            break;
        case Left:
            item_rect.setTop(parent_rect.center().y() - item_height_half);
            item_rect.setBottom(parent_rect.center().y() + item_height_half);
            item_rect.setLeft(parent_rect.left() - item_width_half);
            item_rect.setRight(parent_rect.left() + item_width_half);
            break;
        case Top:
            item_rect.setTop(parent_rect.top() - item_height_half);
            item_rect.setBottom(parent_rect.top() + item_height_half);
            item_rect.setLeft(parent_rect.center().x() - item_width_half);
            item_rect.setRight(parent_rect.center().x() + item_width_half);
            break;
        case Bottom:
            item_rect.setTop(parent_rect.bottom() - item_height_half);
            item_rect.setBottom(parent_rect.bottom() + item_height_half);
            item_rect.setLeft(parent_rect.center().x() - item_width_half);
            item_rect.setRight(parent_rect.center().x() + item_width_half);
            break;
    }
    setRect(item_rect);
}

QVariant ResizeGraphicsItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {        
        QPointF new_pos;

        switch (ResizeModeValue)
        {
        case ResizeMode::TopLeft:
        case ResizeMode::TopRight:
        case ResizeMode::BottomLeft:
        case ResizeMode::BottomRight:
            new_pos = value.toPointF();
            break;
        case ResizeMode::Left:
        case ResizeMode::Right:
            new_pos = QPointF(value.toPointF().x(), pos().y());
            break;
        case ResizeMode::Top:
        case ResizeMode::Bottom:
            new_pos = QPointF(pos().x(), value.toPointF().y());
            break;
        }

        emit positionChanged(rect().center() + new_pos, ResizeModeValue);
        return new_pos;
    }
    return QGraphicsRectItem::itemChange(change, value);
}
