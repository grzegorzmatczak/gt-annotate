#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>

#include "widgets/view/structures.h"
#include "widgets/view/paintersettings.h"


class GraphicsView : public QGraphicsView
{
	Q_OBJECT

	public:
		GraphicsView();

	public:
		bool drawing();
		bool editing();
		void setPainterSettings(PainterSettings * painterSettings);
		void setScale(qreal * scale);

	protected:
		void wheelEvent(QWheelEvent *) override;
		void mousePressEvent(QMouseEvent *e) override;

	signals:
		void updateView();
		void zoomIn(qint32 delta);
		void zoomOut(qint32 delta);

	private:
		qint32 m_mode;
		qreal* m_scale;

	private:
		QPointF m_initPos;
		QPointF m_actualPos;

	protected:
		void mouseMoveEvent(QMouseEvent *event) override;
		void paintEvent(QPaintEvent *event) override;

	private:
		QPointF m_point;
		PainterSettings * m_painterSettings;

};

#endif // GRAPHICSVIEW_H
