#ifndef PAINTER_H
#define PAINTER_H

#include <QtWidgets>
#include <QHash>
#include <QObject>
#include "utils/includespdlog.h"

#include "widgets/graphicsscene.h"
#include "widgets/graphicsview.h"
#include "imageprocessing/contour.h"



#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Painter : public QObject
{	
	Q_OBJECT
	public:
		Painter(QJsonObject const& config, GraphicsScene *graphicsScene, GraphicsView* graphicsView);
		void setOpacity(qreal scaleOpacity);
		void setOpacityROI(qreal scaleOpacity);
		void setOpacityImage(qreal scaleOpacity);

    private:
	    QJsonObject m_config;

	signals:
		void setupMatrix();

    private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;

    public slots:
		void onPaintWhiteBoard(qint32 x, qint32 y);
		void onChangeColor(QColor color);
		bool onChangeOldColor(QString name, QColor color);
		void onChangePenSize(qint32 size);
		void onLoadImage(QString imageName);
		void onSaveWhiteBoard();

    private:
		void addImageToScene(QPixmap image);
        void renderColorsFromImage(QString pathToImage);
        void onPaintColorsFinish();
        void onPaintColors(qint32 x, qint32 y, QColor color);
		void deleteRois();
		void clearScene();

	private:
		QImage m_paintImage;
		QImage m_gridImage;
		QImage m_image;
		GraphicsPixmapItem *m_paintPixmap;
		GraphicsPixmapItem *m_pixmap;
		GraphicsPixmapItem *m_gridPixmap;

	private:
		Contour m_contour;	

	private:
		int m_roiType;
		int m_whiteBoardType;
		int m_ImageType;

	public:
		PainterSettings m_painterSettings;

};

#endif // VIEW_H
