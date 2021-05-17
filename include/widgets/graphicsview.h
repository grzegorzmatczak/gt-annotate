#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QObject>

//#include "shape.h"
#include "structures.h"
#include "includespdlog.h"
#include "widgets/paintersettings.h"

class GraphicsView : public QGraphicsView
{
	Q_OBJECT

	public:
		GraphicsView();

	public:
		bool drawing();
		bool editing();
		 void setPainterSettings(PainterSettings * painterSettings);

	protected:
		void wheelEvent(QWheelEvent *) override;
		void mousePressEvent(QMouseEvent *e) override;

	signals:
		void updateView();
		void zoomIn(qint32 delta);
		void zoomOut(qint32 delta);

	private:
		qint32 m_mode;

	private:
		QPointF m_initPos;
		QPointF m_actualPos;

	// QWidget interface
	protected:
		void mouseMoveEvent(QMouseEvent *event) override;
		void paintEvent(QPaintEvent *event) override;
	private:
		QPointF m_point;
		PainterSettings * m_painterSettings;


};

#endif // GRAPHICSVIEW_H