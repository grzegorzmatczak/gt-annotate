#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include "includespdlog.h"


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

};
 
#endif // VIEW_H