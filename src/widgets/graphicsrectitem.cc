#include "widgets/graphicsrectitem.h"

#include <QFont>
#include <QPainter>
#include <QBrush>
#include <QStyleOptionGraphicsItem>


GraphicsRectItem::GraphicsRectItem(QColor &color, QString text, QRectF rect, int type)
{
	this->setRect(rect);
	//this->setFlags(ItemSendsGeometryChanges | ItemIsSelectable | ItemIsMovable);
	this->setFlags(ItemIsSelectable);
	m_penWidth = 0.2;
	m_rect = rect;
	m_text = (text);
	m_color = color;
	m_type = type;
	m_mouseOver = QColor(0, 0, 0, 50);
	m_hover = false;
	m_mouse = false;
	this->setZValue((int)(m_rect.x() + (int)m_rect.y()) % 2);
	m_textBrush = QBrush(m_color);
	m_textBrushBackground = QBrush(Qt::white);
}

int GraphicsRectItem::type() const
{
  return m_type;
}

void GraphicsRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget)
{
	Q_UNUSED(widget);
	QColor fillColor = m_mouseOver;
	QColor penColor = m_color;
	QBrush fillBrush = QBrush(fillColor);

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

	if (lod < 1.0)
	{
		setPainterAndText(painter, 0, -6, 25, 1);

	}
	else if (lod >= 1.0 && lod < 2.8)
	{
		setPainterAndText(painter, 0, -2, 8, 0.6);

	}
	else if (lod >= 2.8 && lod < 7.5)
	{
		setPainterAndText(painter, 0, -1, 4, 0.3);
	}
	else if (lod >= 7.5)
	{
		setPainterAndText(painter, 0, -1, 2, 0.2);
	}

	painter->setBrush(fillColor);
	painter->drawRect((rect().x()), (rect().y()), rect().width(), rect().height());
	painter->save();
}

void GraphicsRectItem::setPainterAndText(QPainter *painter, int deltax, int deltay, int pixelSize, int penWidth)
{
	QFont font = painter->font();
	font.setBold(true);
	font.setPixelSize(pixelSize);
	QPen tempPen = QPen(m_color, penWidth);
	painter->setPen(tempPen);
	painter->setBrush(m_textBrush);
	painter->setFont(font);
	painter->drawText(rect().x() + deltax, rect().y() + deltay, m_text);
	font.setBold(false);
	painter->setBrush(m_textBrushBackground);
	painter->drawText(rect().x() + deltax, rect().y() + deltay, m_text);
}
