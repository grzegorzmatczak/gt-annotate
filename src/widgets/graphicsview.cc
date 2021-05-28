#include "widgets/graphicsview.h"

#include <QPainter>

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
	Logger->trace("GraphicsView::mousePressEvent()");
	m_initPos = e->pos();
	qDebug() << "GraphicsView::mousePressEvent" << e->pos();
	QGraphicsView::mousePressEvent(e);
}

 void GraphicsView::setPainterSettings(PainterSettings * painterSettings)
 {
	Logger->trace("GraphicsView::setPainterSettings()");
	m_painterSettings = painterSettings;
 }

void GraphicsView::setScale(qreal * scale)
{
	Logger->trace("GraphicsView::setScale()");
	m_scale = scale;
}

void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	Logger->trace("GraphicsView::mouseMoveEvent()");
	m_point = event->pos();
	viewport()->repaint();
	QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::paintEvent(QPaintEvent *event) 
{
	QGraphicsView::paintEvent(event);

	int pen_size = m_painterSettings->m_penSize;
	qreal scale = *m_scale;
	QPen pen(m_painterSettings->m_color, 1, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);

	QPainter painter_id(viewport());
	painter_id.setRenderHint(QPainter::Antialiasing, false);
	painter_id.setPen(pen);

	if (pen_size == 2)
	{
		QRectF rectangle(
						(m_point.x()) - pen_size/2 * scale, 
						(m_point.y()) - pen_size/2 * scale, 
						pen_size * scale, pen_size * scale);
		painter_id.drawRect(rectangle);
	}
	if (pen_size == 3)
	{
		QRectF rectangle(
						(m_point.x()) - (pen_size+2)/2 * scale, 
						(m_point.y()) - (pen_size+2)/2 * scale, 
						(pen_size+1) * scale, (pen_size+1) * scale);
		painter_id.drawRect(rectangle);
	}
	if (pen_size == 4)
	{
		QRectF rectangle(
						(m_point.x()) - (pen_size+3)/2 * scale, 
						(m_point.y()) - (pen_size+3)/2 * scale, 
						(pen_size+2) * scale, (pen_size+2) * scale);
		painter_id.drawRect(rectangle);
	}
	if (pen_size == 5)
	{
		QRectF rectangle(
						(m_point.x()) - (pen_size+4)/2 * scale, 
						(m_point.y()) - (pen_size+4)/2 * scale, 
						(pen_size+3) * scale, (pen_size+3) * scale);
		painter_id.drawRect(rectangle);
	}
	
	painter_id.end();
}
