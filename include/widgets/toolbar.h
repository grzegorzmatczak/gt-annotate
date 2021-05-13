#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>

class ToolButton : public QToolButton
{
  Q_OBJECT
 public:
  ToolButton();

  // QWidget interface
 public:
  QSize minimumSizeHint() const override;
};

class ToolBar : public QToolBar
{
  Q_OBJECT
 public:
  ToolBar();
  void addAction(QAction *action);
};

#endif // TOOLBAR_H