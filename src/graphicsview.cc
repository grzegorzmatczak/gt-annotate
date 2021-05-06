#include "graphicsview.h"

#ifndef QT_NO_OPENGL
#include <QtOpenGL>
#else
#include <QtWidgets>
#endif
#include <qmath.h>

class View;
#define DEBUG FALSE

GraphicsView::GraphicsView()
	: QGraphicsView()
	, m_mode(0)
{}

void GraphicsView::wheelEvent(QWheelEvent* e)
{
	#if(DEBUG)
	H_Logger->trace("GraphicsView::wheelEvent()");
	#endif
	if (e->modifiers() & Qt::ControlModifier)
	{
		if (e->delta() > 0)
			emit(zoomIn(22));
		else
			emit(zoomOut(22));

		e->accept();
	}
	else
	{
		QGraphicsView::wheelEvent(e);
	}
}

bool GraphicsView::drawing()
{
#if(DEBUG)
	H_Logger->trace("GraphicsView::drawing()");
#endif
	if (m_mode == uiMode::Paint)
		return true;
	else
		return false;
}

bool GraphicsView::editing()
{
#if(DEBUG)
	H_Logger->trace("GraphicsView::editing()");
#endif
	if (m_mode == uiMode::SelectROI)
		return true;
	else
		return false;
}

void GraphicsView::mousePressEvent(QMouseEvent* e)
{
#if(DEBUG)
	H_Logger->trace("GraphicsView::mousePressEvent()");
#endif
	m_initPos = e->pos();
#if(DEBUG)
	qDebug() << "GraphicsView::mousePressEvent" << e->pos();
#endif
	if (e->buttons() == Qt::MiddleButton)
	{
	}
	this->repaint();
	QGraphicsView::mousePressEvent(e);
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
#if(DEBUG)
	H_Logger->trace("GraphicsView::mouseMoveEvent()")
#endif
	QGraphicsView::mouseMoveEvent(event);
}