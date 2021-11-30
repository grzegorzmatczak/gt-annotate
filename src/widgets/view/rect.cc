#include "widgets/view/rect.h"

#include <QtWidgets>

class SelectText;

HandleItem::HandleItem(Position position)
  : handlePosition(position)
{
  setFlag(QGraphicsRectItem::ItemIsMovable);
}

void HandleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  qDebug() << "HandleItem::mouseMoveEvent";
  QPointF best = SelectText::computeTopLeftGridPoint(QPointF(event->pos().x(), event->pos().y()));
  switch (handlePosition)
  {
    case TopLeft: {
      SelectText *resizableSelectText = dynamic_cast<SelectText *>(parentItem());
      if (resizableSelectText)
      {
        QRectF boudingFrameRect = resizableSelectText->selectorFrameBounds();
        boudingFrameRect.setTop(best.y());
        boudingFrameRect.setLeft(best.x());
        resizableSelectText->setSelectorFrameBounds(boudingFrameRect.normalized());
      }
    }
    break;
    case TopRight: {
      SelectText *resizableSelectText = dynamic_cast<SelectText *>(parentItem());
      if (resizableSelectText)
      {
        QRectF boudingFrameRect = resizableSelectText->selectorFrameBounds();
        boudingFrameRect.setTop(best.y());
        boudingFrameRect.setRight(best.x());
        resizableSelectText->setSelectorFrameBounds(boudingFrameRect.normalized());
      }
    }
    break;
    case BottomRight: {
      SelectText *resizableSelectText = dynamic_cast<SelectText *>(parentItem());
      if (resizableSelectText)
      {
        QRectF boudingFrameRect = resizableSelectText->selectorFrameBounds();
        boudingFrameRect.setRight(best.x());
        boudingFrameRect.setBottom(best.y());
        resizableSelectText->setSelectorFrameBounds(boudingFrameRect.normalized());
      }
    }
    break;
    case BottomLeft: {
      SelectText *resizableSelectText = dynamic_cast<SelectText *>(parentItem());
      if (resizableSelectText)
      {
        QRectF boudingFrameRect = resizableSelectText->selectorFrameBounds();
        boudingFrameRect.setBottom(best.y());
        boudingFrameRect.setLeft(best.x());
        resizableSelectText->setSelectorFrameBounds(boudingFrameRect.normalized());
      }
    }
    break;
  }
}

QRectF SelectText::selectorFrameBounds() const
{
  return rect();
}

void SelectText::setSelectorFrameBounds(QRectF boundsRect)
{
  prepareGeometryChange();
  setRect(boundsRect);
  update();
}

void SelectText::drawHandlesIfNecessary(QPainter *painter, QSizeF size)
{
  qDebug() << "SelectText::drawHandlesIfNecessary()";
  if (isSelected())
  {
    drawHandles(painter, size);
    handlesAddedToScene = true;
    qDebug() << "SelectText::drawHandlesIfNecessary() isSelected";
  }
  else
  {
    // remove:
    qDebug() << "SelectText::drawHandlesIfNecessary() else";
    foreach (HandleItem *item, handleList)
    {
      m_scene->removeItem(item);
    }
    qDeleteAll(handleList);
    handleList.clear();
    handlesAddedToScene = false;
  }
}

void SelectText::drawHandles(QPainter *painter, QSizeF size)
{
  qDebug() << "SelectText::drawHandles()";
  if (handleList.count() == 0)
  {
    qDebug() << "SelectText::drawHandles() populate";
    // populate:
    handleList.append(new HandleItem(HandleItem::TopLeft));
    handleList.append(new HandleItem(HandleItem::TopRight));
    handleList.append(new HandleItem(HandleItem::BottomRight));
    handleList.append(new HandleItem(HandleItem::BottomLeft));
  }
  // pen:
  QPen _pen;
  _pen.setWidth(0.2);
  _pen.setColor(Qt::black);

  QPointF closestPoint1 = computeTopLeftGridPoint(rect().topLeft() + QPointF(-size.width() / 2, -size.height() / 2));
  QPointF topLeftCorner = closestPoint1;

  topLeftHandleRect = QRectF(topLeftCorner, size);
  handleList[0]->setRect(topLeftHandleRect);
  if (!handleList.isEmpty() && !handlesAddedToScene)
  {
    handleList[0]->setPen(_pen);
    handleList[0]->setBrush(QBrush(Qt::gray));
    m_scene->addItem(handleList[0]);
    handleList[0]->setParentItem(this);
  }

  QPointF closestPoint2 = computeTopLeftGridPoint(rect().topRight() + QPointF(-size.width() / 2, -size.height() / 2));
  QPointF topRightCorner = closestPoint2;
  topRightHandleRect = QRectF(topRightCorner, size);
  handleList[1]->setRect(topRightHandleRect);
  if (!handleList.isEmpty() && !handlesAddedToScene)
  {
    handleList[1]->setPen(_pen);
    handleList[1]->setBrush(QBrush(Qt::gray));
    m_scene->addItem(handleList[1]);
    handleList[1]->setParentItem(this);
  }

  // Bottomright handle
  QPointF closestPoint3 = computeTopLeftGridPoint(rect().bottomRight() + QPointF(-size.width() / 2, -size.height() / 2));
  QPointF bottomRightCorner = closestPoint3;
  bottmRightHandleRect = QRectF(bottomRightCorner, size);
  handleList[2]->setRect(bottmRightHandleRect);
  if (!handleList.isEmpty() && !handlesAddedToScene)
  {
    handleList[2]->setPen(_pen);
    handleList[2]->setBrush(QBrush(Qt::gray));
    m_scene->addItem(handleList[2]);
    handleList[2]->setParentItem(this);
  }

  QPointF closestPoint4 = computeTopLeftGridPoint(rect().bottomLeft() + QPointF(-size.width() / 2, -size.height() / 2));
  QPointF bottomLeftCorner = closestPoint4;
  bottomLeftHandleRect = QRectF(bottomLeftCorner, size);
  handleList[3]->setRect(bottomLeftHandleRect);
  if (!handleList.isEmpty() && !handlesAddedToScene)
  {
    handleList[3]->setPen(_pen);
    handleList[3]->setBrush(QBrush(Qt::gray));
    m_scene->addItem(handleList[3]);
    handleList[3]->setParentItem(this);
  }
}

SelectText::SelectText(QColor &color, QString text, QRectF tempRectToText, GraphicsScene *scene, qint32 id)
  : m_scene(scene)
{
  setRect(tempRectToText);
  setFlags(ItemSendsGeometryChanges | ItemIsSelectable | ItemIsMovable);
  m_penWidth = 0.2;
  m_tempRectToText = tempRectToText;
  m_text = (text);
  m_color = color;
  m_id = id;
  m_mouseOver = QColor(255, 0, 0, 123);
  m_hover = false;
  m_mouse = false;
  setZValue((int)(tempRectToText.x() + (int)tempRectToText.y()) % 2);
}

QString SelectText::returnName() const
{
  return m_text;
}

QPointF SelectText::computeTopLeftGridPoint(const QPointF &pointP)
{
  int gridSize = 1;
  qreal xV = qCeil(pointP.x() / gridSize) * gridSize;
  qreal yV = qCeil(pointP.y() / gridSize) * gridSize;
  return QPointF(xV, yV);
}

void SelectText::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  offset = pos() - computeTopLeftGridPoint(pos());
  QGraphicsRectItem::mousePressEvent(event);
}

void SelectText::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  m_mouse = true;
  if (this->isSelected())
  {
    update();
    QGraphicsRectItem::mouseMoveEvent(event);
  }
}

void SelectText::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  update();
  QGraphicsRectItem::mouseReleaseEvent(event);
}

void SelectText::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
  m_hover = true;
  QGraphicsRectItem::hoverMoveEvent(event);
}

QVariant SelectText::itemChange(GraphicsItemChange change, const QVariant &value)
{
  if (change == ItemPositionChange)
  {
    QPointF newPos = value.toPointF();
    if (QApplication::mouseButtons() == Qt::LeftButton)
    {
      QPointF closestPoint = computeTopLeftGridPoint(newPos);
      return closestPoint += offset;
    }
    else
      return newPos;
  }

  return QGraphicsRectItem::itemChange(change, value);
}

void SelectText::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
  Q_UNUSED(widget);
  QColor fillColor = m_mouseOver;
  QColor penColor = Qt::black;

  if (item->state & QStyle::State_Selected)
  {
    fillColor.setAlpha(50);
    penColor = Qt::white;
  }
  else if (item->state & QStyle::State_MouseOver)
  {
    fillColor.setAlpha(10);
  }
  else
  {
    fillColor.setAlpha(10);
  }

  const qreal lod = item->levelOfDetailFromTransform(painter->worldTransform());

  if (m_hover)
  {
    fillColor.setAlpha(200);
  }
  m_hover = false;
  if (m_mouse)
  {
    fillColor.setAlpha(2);
  }
  m_mouse = false;

  QPen tempPen = QPen(Qt::black, 1);

  if (lod < 1.0)
  {
    QFont font = painter->font();
    font.setPixelSize(25);
    painter->setFont(font);
    painter->drawText(rect().x() + 1, rect().y() - 12, m_text);
  }
  else if (lod >= 1.0 && lod < 2.8)
  {
    QFont font = painter->font();
    font.setPixelSize(8);
    painter->setFont(font);
    painter->drawText(rect().x() + 2, rect().y() - 4, m_text);
    tempPen = QPen(penColor, 0.6);
  }
  else if (lod >= 2.8 && lod < 7.5)
  {
    QFont font = painter->font();
    font.setPixelSize(4);
    painter->setFont(font);
    painter->drawText(rect().x() + 2, rect().y() - 2, m_text);
    tempPen = QPen(penColor, 0.3);
  }
  else if (lod >= 7.5)
  {
    QFont font = painter->font();
    font.setPixelSize(2);
    painter->setFont(font);
    painter->drawText(rect().x() + 2, rect().y() - 1, m_text);
    tempPen = QPen(penColor, 0.3);
  }


  QBrush tempBrush = QBrush(fillColor);

  painter->setPen(tempPen);
  painter->setBrush(tempBrush);

  painter->setBrush(fillColor);
  painter->drawRect((rect().x()), (rect().y()), rect().width(), rect().height());
  painter->fillRect((rect().x()), (rect().y()), rect().width(), rect().height(), tempBrush);
  painter->brush();

  painter->save();
  painter->setBrush(brush());
  // painter->drawRect(rect());
  if (lod < 1.0)
  {
    drawHandlesIfNecessary(painter, QSize(20, 20));
  }
  if (lod < 2.0)
  {
    drawHandlesIfNecessary(painter, QSize(10, 10));
  }
  else if (lod >= 2.0 && lod < 7.5)
  {
    drawHandlesIfNecessary(painter, QSize(5, 5));
  }
  else if (lod >= 7.5)
  {
    drawHandlesIfNecessary(painter, QSizeF(1, 1));
  }
  else if (lod >= 10.5)
  {
      drawHandlesIfNecessary(painter, QSizeF(0.5, 0.5));
  }
  else if (lod >= 12.5)
  {
      drawHandlesIfNecessary(painter, QSizeF(0.25, 0.25));
  }
  painter->restore();
}

int HandleItem::type() const
{
  return 1;
}

int SelectText::type() const
{
  return 2;
}
