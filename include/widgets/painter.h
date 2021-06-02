#ifndef PAINTER_H
#define PAINTER_H

#include <QtWidgets>
#include <QHash>
#include <QObject>
#include "utils/includespdlog.h"
#include "utils/configreader.h"
#include "utils/listvector.h"

#include "widgets/graphicsscene.h"
#include "widgets/graphicsview.h"
#include "widgets/graphicspixmapitem.h"
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
		void updateView();
		void addList(int id, QString label, int size, bool enabled);
		void clearList();

    private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;

    public slots:
		void onPaintOnBoard(qint32 x, qint32 y);
		void onChangeColor(QColor color);
		void onChangeOldColor(QString name, QColor color);
		void onChangePenSize(qint32 size);
		void onCreateRois();
		void onSetCurrentPaintFolder(QString imageFolder, QString paintFolder, QString jsonDirectory);
		void onSavePaint(QString dir, QString name);
		void onSaveRois(QString dir, QString name);
		void onLoadImage(QString dir, QString name);
		void onLoadPaint(QString dir, QString name);
		void onLoadRois(QString dir, QString name);

    private:
		void addImageToScene(QPixmap image);
        void onPaintColorsFinish();
        void onPaintColors(qint32 x, qint32 y, QColor color);
		void deleteRois();
		void clearScene();
		
		void addRoisToScene(QJsonArray contoursArray);

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
		int m_paintType;
		int m_imageType;

	public:
		PainterSettings m_painterSettings;
		//ListVector<listInfo> m_listVector;

	private:
		QString m_currentDirectory;
		QString m_currentPaintDirectory;
		QString m_currentJsonDirectory;
		QString m_imageName;
		QString m_name;
		QString m_split;

};

#endif // VIEW_H
