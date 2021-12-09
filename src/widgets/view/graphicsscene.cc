#include "widgets/view/graphicsscene.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

//#define DEBUG


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
	#ifdef DEBUG
	Logger->debug("GraphicsScene::mousePressEvent()");
	qDebug() << "GraphicsScene::mousePressEvent pos:" << e->pos();
	qDebug() << "GraphicsScene::mousePressEvent scenePos:" << e->scenePos();
	#endif

	QGraphicsScene::mousePressEvent(e);
	if (m_mode == uiMode::Paint && e->buttons() == Qt::LeftButton)
	{
        qreal xreal = e->scenePos().x();
		qreal yreal = e->scenePos().y();
		onPaintWhiteBoard(xreal, yreal);
		emit(updateView());
	}
	if (m_mode == uiMode::Paint && e->buttons() == Qt::RightButton)
	{
        qreal xreal = e->scenePos().x();
		qreal yreal = e->scenePos().y();
		onPaintWhiteBoardBackground(xreal, yreal);
		emit(updateView());
	}
	else if (m_mode == uiMode::SelectROI && e->buttons() == Qt::LeftButton)
	{
        qreal xreal = e->scenePos().rx()/10 * 10;
        qreal yreal = e->scenePos().ry()/10 * 10;

		m_startPoint = e->scenePos(); // Save start pos to save ROI on scene.
		m_justSelect = false;

		int x = static_cast<int>(xreal);
		int y = static_cast<int>(yreal);
		QPointF point = QPointF(x, y);
		e->setScenePos(point);
		QGraphicsScene::mousePressEvent(e);

	}
	if (m_mode == uiMode::SelectROI && e->buttons() == Qt::MiddleButton)
	{
		m_justSelect = true;
	}
	else if (m_mode == uiMode::MoveSelect && e->buttons() == Qt::LeftButton)
	{
		QGraphicsScene::mousePressEvent(e);
	}
}

void GraphicsScene::resetScene()
{
	#ifdef DEBUG
	Logger->debug("GraphicsScene::resetScene()");
	#endif
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
	#ifdef DEBUG
	Logger->debug("GraphicsScene::mouseMoveEvent()");
	#endif
	if (m_mode == uiMode::Paint && e->buttons() == Qt::LeftButton)
	{
		qreal xreal = e->scenePos().rx();
		qreal yreal = e->scenePos().ry();
		onPaintWhiteBoard(xreal, yreal);
	}
	if (m_mode == uiMode::Paint && e->buttons() == Qt::RightButton)
	{
		qreal xreal = e->scenePos().rx();
		qreal yreal = e->scenePos().ry();
		onPaintWhiteBoardBackground(xreal, yreal);
	}
	else if (m_mode == uiMode::SelectROI)
	{
		QGraphicsScene::mouseMoveEvent(e);
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

void GraphicsScene::onPaintWhiteBoardBackground(qreal x, qreal y)
{
	int rx = static_cast<int>(x);
	int ry = static_cast<int>(y);
	emit(paintWhiteBoardBackground(rx, ry));
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e)
{
	#ifdef DEBUG
	Logger->debug("GraphicsScene::mouseReleaseEvent()");
	#endif
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
	#ifdef DEBUG
	Logger->debug("GraphicsScene::setMode(mode:{})", mode);
	#endif
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
