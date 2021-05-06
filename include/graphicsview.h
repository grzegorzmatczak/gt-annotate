#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>

//#include "shape.h"
#include "structures.h"

class GraphicsView : public QGraphicsView
{
  Q_OBJECT
 public:
  GraphicsView();

 public:
  bool drawing();
  bool editing();

 protected:
  void wheelEvent(QWheelEvent *) override;
  void mousePressEvent(QMouseEvent *e) override;

 signals:
  void updateView();
  void zoomIn(qint32 delta);
  void zoomOut(qint32 delta);

 private:
  qint32 m_mode;

 private:
  QPointF m_initPos;
  QPointF m_actualPos;

  // QWidget interface
 protected:
  void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // GRAPHICSVIEW_H