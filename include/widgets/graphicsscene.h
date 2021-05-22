#ifndef GRAPHICSSCENE_H
#define GRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include <QPen>

#include "structures.h"
#include "utils/includespdlog.h"


class GraphicsScene : public QGraphicsScene
{
	Q_OBJECT
	public:
		GraphicsScene();

	public:
		void onPaintWhiteBoard(qreal x, qreal y);
		QRectF transformPos(QRectF pos);
		void resetScene();
		void setMode(uiMode mode);

	public slots:

	signals:
		void addRectToScene(QPointF startPoint, QPointF stopPoint, bool dialog, QString name);
		void updateView();
		void paintWhiteBoard(qint32 x, qint32 y);
		void handleDrawing(QPointF point);
		void handleDrawingMouseRelease(QPointF point);
		void setActualPos(QPointF point);
		void deleteItem(qint32 id);

	protected:
		void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
		void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
		void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

	private:
		QPointF m_startPoint;
		QPointF m_stopPoint;
		uiMode m_mode;

		QPointF m_initPos;
		QPointF m_actualPos;

		bool m_justSelect;

	protected:
		QPen m_pen;

	// QGraphicsScene interface
	protected:
		void keyPressEvent(QKeyEvent *event) override;
};

#endif // GRAPHICSSCENE_H