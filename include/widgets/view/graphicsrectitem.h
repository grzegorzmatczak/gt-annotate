#ifndef GRAPHICS_RECT_ITEM_H
#define GRAPHICS_RECT_ITEM_H

#include <QColor>
#include <QGraphicsItem>
#include <QGraphicsRectItem>

#include "widgets/view/graphicsscene.h"

#include "widgets/view/structures.h"


class GraphicsRectItem : public QGraphicsRectItem
{
	public:
		GraphicsRectItem(QColor &color, QString text, QRectF rect, int type);

	public: // override:
		int type() const override;
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;
		QString text(){return m_text;}

	private:
		void setPainterAndText(QPainter *painter, int deltax, int deltay, int pixelSize, int penWidth);
		
	private:
		QRectF m_rect;
		QString m_text;
		QColor m_color;
		qreal m_penWidth;
		QPointF offset;
		bool m_hover{};
		bool m_mouse{};
		QColor m_mouseOver;
		int m_type{};
		QBrush m_textBrush;
		QBrush m_textBrushBackground;
		QPen m_pen;
};

#endif // GRAPHICS_RECT_ITEM_H
