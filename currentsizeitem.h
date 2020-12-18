#ifndef CURRENTSIZEITEM_H
#define CURRENTSIZEITEM_H

#include <QGraphicsTextItem>

class CurrentSizeItem : public QGraphicsTextItem
{
    Q_OBJECT
public:
    CurrentSizeItem(const QString &text  = QString(), QGraphicsItem *parent = nullptr);

public slots:
    void onItemRectChanged(QRectF bounding_rect);

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // CURRENTSIZEITEM_H
