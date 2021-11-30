#ifndef SELECTRECT_H
#define SELECTRECT_H
#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include "widgets/view/graphicsscene.h"

class QGraphicsScene;
class GraphicsScene;

class HandleItem : public QGraphicsRectItem
{
 public:
  enum Position
  {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight

  };
  explicit HandleItem(Position position);
  // Interface:

  // QGraphicsItm interface
 protected:
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

 private:
  Position handlePosition;

  // QGraphicsItem interface
 public:
  int type() const override;
};

class SelectText : public QGraphicsRectItem
{
 public:
  SelectText(QColor &color, const QString text, QRectF tempRectToText, GraphicsScene *scene, qint32 id);
  QString returnName() const;
  qint32 id() { return m_id; }
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
  // QPainterPath shape() const override;

 private:
  QRectF m_tempRectToText;
  QString m_text;
  QColor m_color;
  qreal m_penWidth;

 public:
  QRectF selectorFrameBounds() const;
  void setSelectorFrameBounds(QRectF boundsRect);
  void drawHandlesIfNecessary(QPainter *painter, QSizeF size);

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

 protected:
  virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

 private:
  void drawHandles(QPainter *painter, QSizeF size);
  QRectF topLeftHandleRect;
  QRectF topRightHandleRect;
  QRectF bottomLeftHandleRect;
  QRectF bottmRightHandleRect;

  QList<HandleItem *> handleList;
  bool handlesAddedToScene;

  GraphicsScene *m_scene;

  QPointF offset;
  bool m_hover;
  bool m_mouse;
  QColor m_mouseOver;

 private:
  qint32 m_id;

 public:
  QPointF static computeTopLeftGridPoint(const QPointF &pointP);

  // QGraphicsItem interface
 public:
  int type() const override;

  // QGraphicsItem interface
 protected:
  void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // SELECTRECT_H
