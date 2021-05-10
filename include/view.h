#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include "includespdlog.h"
#include "graphicsscene.h"
#include "graphicsview.h"


class View : public QFrame
{
	Q_OBJECT
	
	public:
		explicit View(QJsonObject const &a_config, QFrame *parent = nullptr);
		QGraphicsView *view() const;

		void configure(QJsonObject const &a_config);

	public slots:
		void onPaintWhiteBoard(qint32 x, qint32 y);
		void onZoomIn(qint32 delta);
  		void onZoomOut(qint32 delta);

	signals:
		void setModePaint();
		void setModeROI();
		void setModeMove();
		void resetScene();

	private slots:
		void setupMatrix();
		void setOpacity();

	private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;

		QVBoxLayout *m_zoomSliderLayout;
		QVBoxLayout *m_opacityLayout;
		QGridLayout *m_topLayout;
		QSlider *m_opacitySlider;
		QSlider *m_zoomSlider;


	private:
		qreal m_scaleOpacity;
		qreal m_scale;

	private:
		std::vector<struct colors> m_colors;
		qint32 m_penSize{};
		QColor m_color;
	 private:
		QColor m_colorBlack;
		QColor m_colorWhite;
		QColor m_colorShadow;
		QColor m_colorOutOfScope;
		QColor m_colorUnknown;
		qint32 m_intBlack{};
		qint32 m_intWhite{};
		qint32 m_intShadow{};
		qint32 m_intOutOfScope{};
		qint32 m_intUnknown{};

	private:
		QImage m_diff;
		QImage m_image;
		QGraphicsPixmapItem *m_whitePixmap;
		QGraphicsPixmapItem *m_pixmap;

};
 
#endif // VIEW_H