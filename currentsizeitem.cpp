#include "currentsizeitem.h"
#include <QString>
#include <QFont>
#include <QPainter>

const int offset = 20;

CurrentSizeItem::CurrentSizeItem(const QString &text/* = QString()*/, QGraphicsItem *parent/* = nullptr*/) :
    QGraphicsTextItem(text, parent)
{
    setFont(QFont("Arial"));
    setDefaultTextColor(Qt::white);
}

void CurrentSizeItem::onItemRectChanged(QRectF bounding_rect)
{
    const QSize& new_size = bounding_rect.size().toSize();
    setPlainText(QString("%1 x %2").arg(abs(new_size.width())).arg(abs(new_size.height())));
    const QPointF& new_pos = QPointF(bounding_rect.left() + bounding_rect.width() / 2 - boundingRect().size().width() / 2 , bounding_rect.bottom() + offset);
    setPos(new_pos);
}

void CurrentSizeItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QColor(0, 0, 0, 0));
    painter->setBrush(QColor(0, 140, 255, 255));
    painter->drawRoundedRect(boundingRect(), 4, 4);
    QGraphicsTextItem::paint(painter, option, widget);
}
