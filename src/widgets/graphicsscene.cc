#include "widgets/graphicsscene.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

GraphicsScene::GraphicsScene()
	: QGraphicsScene()
	, m_justSelect(false)
	, m_mode(uiMode::None)
{}

QRectF GraphicsScene::transformPos(QRectF pos)
{
	return pos;
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e)
{
	Logger->trace("GraphicsScene::mousePressEvent()");
	if (m_mode == uiMode::Paint && e->buttons() == Qt::LeftButton)
	{
		qreal xreal = e->scenePos().x();
		qreal yreal = e->scenePos().y();
		onPaintWhiteBoard(xreal, yreal);
		emit(updateView());
	}
	else if (m_mode == uiMode::SelectROI && e->buttons() == Qt::LeftButton)
	{
		m_startPoint = e->scenePos(); // Save start pos to save ROI on scene.
		setSelectionArea(QPainterPath());
		emit(updateView());
		m_justSelect = false;
	}
	if (m_mode == uiMode::SelectROI && e->buttons() == Qt::MiddleButton)
	{
		m_justSelect = true;
	}
	else if (m_mode == uiMode::MoveSelect && e->buttons() == Qt::LeftButton)
	{
		qreal xreal = e->scenePos().rx();
		qreal yreal = e->scenePos().ry();
		int x = static_cast<int>(xreal);
		int y = static_cast<int>(yreal);
		QPointF point = QPointF(x, y);
		e->setScenePos(point);
		QGraphicsScene::mousePressEvent(e);
	}
}

void GraphicsScene::resetScene()
{
	Logger->trace("GraphicsScene::resetScene()");
	QList<QGraphicsItem*> all = items();
	for (int i = 0; i < all.size(); i++)
	{
		QGraphicsItem* gi = all[i];
		if (gi->parentItem() == NULL)
		{
			delete gi;
		}
	}
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e)
{
	if (m_mode == uiMode::Paint && e->buttons() == Qt::LeftButton)
	{
		qreal xreal = e->scenePos().rx();
		qreal yreal = e->scenePos().ry();
		onPaintWhiteBoard(xreal, yreal);
	}
	else if (m_mode == uiMode::SelectROI)
	{
		QGraphicsScene::mouseMoveEvent(e);
		emit(updateView());
	}
	else if (m_mode == uiMode::MoveSelect)
	{
		QGraphicsScene::mouseMoveEvent(e);
	}
}

void GraphicsScene::onPaintWhiteBoard(qreal x, qreal y)
{
	int rx = static_cast<int>(x);
	int ry = static_cast<int>(y);
	emit(paintWhiteBoard(rx, ry));
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	Logger->trace("GraphicsScene::mouseReleaseEvent()");
	if (m_mode == uiMode::Paint)
	{
	}
	else if (m_mode == uiMode::SelectROI && !m_justSelect)
	{
		QPainterPath tmpPath = selectionArea();
		setSelectionArea(QPainterPath());

		if (tmpPath.isEmpty())
		{
		}
		else
		{
			emit(addRectToScene(m_startPoint, e->scenePos(), true, ""));
			emit(updateView());
		}

		QGraphicsScene::mouseReleaseEvent(e);
	}
	else if (m_mode == uiMode::MoveSelect)
	{
		QGraphicsScene::mouseReleaseEvent(e);
	}
}

void GraphicsScene::setMode(uiMode mode)
{
	Logger->trace("GraphicsScene::setMode()");
	m_mode = mode;
}

void GraphicsScene::keyPressEvent(QKeyEvent* event)
{
	if (event->key() == Qt::Key_Delete)
	{
		if (selectedItems().count() > 0)
		{
			/*
			QGraphicsItem* itemToRemove = selectedItems()[0];
			if (itemToRemove->type() == 2)
			{
				SelectText* cast = dynamic_cast<SelectText*>(itemToRemove);
				emit(deleteItem(cast->id()));
			}
			removeItem(itemToRemove);
			delete itemToRemove;*/
		}
	}
	QGraphicsScene::keyPressEvent(event);
}