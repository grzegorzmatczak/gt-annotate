#ifndef GRAPHICSPIXMAPITEM_H
#define GRAPHICSPIXMAPITEM_H

#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QObject>

class GraphicsPixmapItem : public QGraphicsPixmapItem
{
 public:
  GraphicsPixmapItem();

 signals:

  // QGraphicsItem interface
 public:
  int type() const override;
};

#endif // GRAPHICSPIXMAPITEM_H
