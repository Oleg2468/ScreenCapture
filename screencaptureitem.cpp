#include "screencaptureitem.h"
#include <QGraphicsScene>

 ScreenCaptureItem::ScreenCaptureItem(const QRectF &rect/* = QRectF()*/, QGraphicsItem *parent/* = nullptr*/, QObject *scene/* = nullptr*/) : QObject(scene), QGraphicsRectItem(rect, parent)
 {
 }

  ScreenCaptureItem::~ScreenCaptureItem()
 {

 }

  void ScreenCaptureItem::addResizeItem(QRectF rect, ResizeGraphicsItem::ResizeMode resize_mode, QGraphicsScene *scene)
  {
      ResizeGraphicsItem* resize_item = new ResizeGraphicsItem(resize_mode, rect, this, scene);
      resize_item->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemSendsGeometryChanges);
      resize_item->setBrush(FrameColor);
      resize_item->setPen(TransparentColor);
      connect(resize_item, &ResizeGraphicsItem::positionChanged, this, &ScreenCaptureItem::onResizeItemChanged);
      connect(this, &ScreenCaptureItem::sizeRectChanged, resize_item, &ResizeGraphicsItem::onItemRectChanged);
  }

  void ScreenCaptureItem::addResizeItems(QGraphicsScene *scene)
  {
      QRectF bounding_rect = boundingRect();
      addResizeItem(QRectF(bounding_rect.left() - 10, bounding_rect.center().y() - 20, 20, 40), ResizeGraphicsItem::Left, scene);
      addResizeItem(QRectF(bounding_rect.right() - 10, bounding_rect.center().y() - 20, 20, 40), ResizeGraphicsItem::Right, scene);
      addResizeItem(QRectF(bounding_rect.center().x() - 20, bounding_rect.top() - 10, 40, 20), ResizeGraphicsItem::Top, scene);
      addResizeItem(QRectF(bounding_rect.center().x() - 20, bounding_rect.bottom() - 10, 40, 20), ResizeGraphicsItem::Bottom, scene);
      addResizeItem(QRectF(bounding_rect.left() - 10, bounding_rect.top() - 10, 20, 20), ResizeGraphicsItem::TopLeft, scene);
      addResizeItem(QRectF(bounding_rect.right() - 10, bounding_rect.top() - 10, 20, 20), ResizeGraphicsItem::TopRight, scene);
      addResizeItem(QRectF(bounding_rect.left() - 10, bounding_rect.bottom() - 10, 20, 20), ResizeGraphicsItem::BottomLeft, scene);
      addResizeItem(QRectF(bounding_rect.right() - 10, bounding_rect.bottom() - 10, 20, 20), ResizeGraphicsItem::BottomRight, scene);
  }

  void ScreenCaptureItem::onResizeItemChanged(QPointF pos, ResizeGraphicsItem::ResizeMode resize_mode)
  {
      QRectF cur_rect = rect();
      QPointF old_top_left = cur_rect.topLeft();

      switch (resize_mode)
      {
          case ResizeGraphicsItem::TopLeft:
              cur_rect.setTopLeft(pos);
              break;
          case ResizeGraphicsItem::TopRight:
              cur_rect.setTopRight(pos);
              break;
          case ResizeGraphicsItem::BottomLeft:
              cur_rect.setBottomLeft(pos);
              break;
          case ResizeGraphicsItem::BottomRight:
              cur_rect.setBottomRight(pos);
              break;
          case ResizeGraphicsItem::Left:
              cur_rect.setLeft(pos.x());
              break;
          case ResizeGraphicsItem::Right:
              cur_rect.setRight(pos.x());
              break;
          case ResizeGraphicsItem::Top:
              cur_rect.setTop(pos.y());
              break;
          case ResizeGraphicsItem::Bottom:
              cur_rect.setBottom(pos.y());
              break;
      }
              setRect(cur_rect);

              QRectF cur_rect_to_scene = mapRectToScene(cur_rect);
              emit sizeRectChanged(cur_rect_to_scene, resize_mode);
              emit sizeChanged(cur_rect_to_scene.size().toSize());

              if (old_top_left != cur_rect.topLeft())
                      emit posChanged(cur_rect_to_scene.topLeft().toPoint());
  }

 QVariant ScreenCaptureItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene())
     {
         QRectF bounding_rect = boundingRect();
         QPointF new_pos = value.toPointF();
         QPointF item_scene_pos = bounding_rect.topLeft() + new_pos ;
         QRectF scene_rect = scene()->sceneRect();

         if (!scene_rect.contains(item_scene_pos) || !scene_rect.contains(QPointF(item_scene_pos.x() + bounding_rect.width(), item_scene_pos.y() + bounding_rect.height())))
         {
            new_pos.setX(qMin(scene_rect.right() - bounding_rect.width(), qMax(item_scene_pos.x(), scene_rect.left())));
            new_pos.setY(qMin(scene_rect.bottom() - bounding_rect.height(), qMax(item_scene_pos.y(), scene_rect.top())));

            return new_pos - bounding_rect.topLeft();
         }
     }
     else if (change == ItemPositionHasChanged)
     {
        const QRectF& bounding_rect = boundingRect();
        emit positionRectChanged(mapRectToScene(bounding_rect));
        emit posChanged(mapToScene(bounding_rect.topLeft()).toPoint());
     }
     return QGraphicsItem::itemChange(change, value);
 }
