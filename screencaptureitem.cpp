#include "screencaptureitem.h"
#include <QGraphicsScene>

const int ItemMinSide = 20;
const int ItemMaxSide = 40;

const int MinSideHalf = 20 / 2;
const int MaxSideHalf = 40 / 2;

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
      QRectF cur_rect = rect();

      addResizeItem(QRectF(cur_rect.left() - MinSideHalf, cur_rect.center().y() - MaxSideHalf, ItemMinSide, ItemMaxSide), ResizeGraphicsItem::Left, scene);
      addResizeItem(QRectF(cur_rect.right() - MinSideHalf, cur_rect.center().y() - MaxSideHalf, ItemMinSide, ItemMaxSide), ResizeGraphicsItem::Right, scene);
      addResizeItem(QRectF(cur_rect.center().x() - MaxSideHalf, cur_rect.top() - MinSideHalf, ItemMaxSide, ItemMinSide), ResizeGraphicsItem::Top, scene);
      addResizeItem(QRectF(cur_rect.center().x() - MaxSideHalf, cur_rect.bottom() - MinSideHalf, ItemMaxSide, ItemMinSide), ResizeGraphicsItem::Bottom, scene);
      addResizeItem(QRectF(cur_rect.left() - MinSideHalf, cur_rect.top() - MinSideHalf, ItemMinSide, ItemMinSide), ResizeGraphicsItem::TopLeft, scene);
      addResizeItem(QRectF(cur_rect.right() - MinSideHalf, cur_rect.top() - MinSideHalf, ItemMinSide, ItemMinSide), ResizeGraphicsItem::TopRight, scene);
      addResizeItem(QRectF(cur_rect.left() - MinSideHalf, cur_rect.bottom() - MinSideHalf, ItemMinSide, ItemMinSide), ResizeGraphicsItem::BottomLeft, scene);
      addResizeItem(QRectF(cur_rect.right() - MinSideHalf, cur_rect.bottom() - MinSideHalf, ItemMinSide, ItemMinSide), ResizeGraphicsItem::BottomRight, scene);
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
         QRectF cur_rect = rect();
         QPointF new_pos = value.toPointF();
         QPointF item_scene_pos = cur_rect.topLeft() + new_pos;
         QRectF scene_rect = scene()->sceneRect();

         if (!scene_rect.contains(item_scene_pos) || !scene_rect.contains(QPointF(item_scene_pos.x() + cur_rect.width(), item_scene_pos.y() + cur_rect.height())))
         {
            new_pos.setX(qMin(scene_rect.right() - cur_rect.width(), qMax(item_scene_pos.x(), scene_rect.left())));
            new_pos.setY(qMin(scene_rect.bottom() - cur_rect.height(), qMax(item_scene_pos.y(), scene_rect.top())));

            return new_pos - cur_rect.topLeft();
         }
     }
     else if (change == ItemPositionHasChanged)
     {
        const QRectF& cur_rect = rect();
        emit positionRectChanged(mapRectToScene(cur_rect));
        emit posChanged(mapToScene(cur_rect.topLeft()).toPoint());
     }
     return QGraphicsItem::itemChange(change, value);
 }
