#include "widgets/toolbar.h"


ToolButton::ToolButton() {}

QSize ToolButton::minimumSizeHint() const
{
  return QSize(150, 10);
}


ToolBar::ToolBar() {}

void ToolBar::addAction(QAction *action)
{
  ToolButton *button = new ToolButton();
  button->setDefaultAction(action);
  button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  this->addWidget(button);
}
