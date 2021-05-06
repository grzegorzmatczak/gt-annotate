#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include "includespdlog.h"
#include "graphicsscene.h"
#include "graphicsview.h"


class View : public QFrame
{
  Q_OBJECT
 public:
  explicit View(QJsonObject const &a_config, QFrame *parent = nullptr);
  QGraphicsView *view() const;

  void configure(QJsonObject const &a_config);

 public slots:

 signals:

 private slots:
 void setupMatrix();
 void setOpacity();

private:
  GraphicsView *m_graphicsView;
  GraphicsScene *m_graphicsScene;

  QVBoxLayout *m_zoomSliderLayout;
  QVBoxLayout *m_opacityLayout;
  QGridLayout *m_topLayout;
  QSlider *m_opacitySlider;
  QSlider *m_zoomSlider;


private:
  qreal m_scaleOpacity;
  qreal m_scale;

private:

private:
  QImage m_diff;
  QImage m_image;
  QGraphicsPixmapItem *m_whitePixmap;
  QGraphicsPixmapItem *m_pixmap;

};
 
#endif // VIEW_H