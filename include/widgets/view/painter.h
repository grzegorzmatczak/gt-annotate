#ifndef PAINTER_H
#define PAINTER_H

#include <QtWidgets>
#include <QHash>
#include <QObject>
#include "includespdlog.h"
#include "configreader.h"
#include "data.h"

#include "widgets/view/graphicsscene.h"
#include "widgets/view/graphicsview.h"
#include "widgets/view/graphicspixmapitem.h"
#include "widgets/view/graphicsrectitem.h"
#include "widgets/view//labeldialog.h"
#include "imageprocessing/contour.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



class Painter : public QObject
{	
	Q_OBJECT
	public:
		Painter(QJsonObject const& config, GraphicsScene *graphicsScene, GraphicsView* graphicsView, DataMemory* dataMemory);
		void setOpacity(qreal scaleOpacity);
		void setOpacityROI(qreal scaleOpacity);
		void setOpacityImage(qreal scaleOpacity);
		int onChangeOldColor(QString name, QColor color);

	private:
		QJsonObject m_config;

	signals:
		void updateView();
		void clearList();
		void updateFileFromId(int id);
		void updatStatus(int id, QString status);
		void addRoi(int id, QString label, int size, bool enable);
		void clearRoiWidget();
		void useNetwork(cv::Mat image, QRect rect);

	private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;

	public slots:
		void onPaintOnBoard(qint32 x, qint32 y);
		void onPaintBackground(qint32 x, qint32 y);
		void onChangeColor(QColor color);
		void onChangePenSize(qint32 size);
		void onCreateRois();
		void onSetCurrentPaintFolder(QString imageFolder, QString paintFolder, QString jsonDirectory);
		void onSavePaint();
		void onSaveRois();
		void onUpdateFile();
		void onLoadImage(int id);
		void onLoadPaints(int id);
		void onLoadRois(int id);
		void onAddRectToScene(QPointF startPoint, QPointF stopPoint, bool dialog, QString name);
		void onRoiItemChanged(QTreeWidgetItem *item, int column);
		void onRoiItemSelectionChanged();
		void onUseNetwork();
		void onReturnUsedNetwork(cv::Mat image, QRect rect);

	private:
		void addImageToScene(QPixmap image);
		void onPaintColorsFinish();
		void onPaintColors(qint32 x, qint32 y, QColor color);
		void deleteRois();
		void clearScene();
		void onPaintOnBoardInColor(qint32 x, qint32 y, QColor color);
		void addRoisToScene(QJsonArray contoursArray);
		void addToRoiWidget(QJsonArray array);
		QImage cvMatToQImage(const cv::Mat &input);
		cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData);

	private:
		QImage m_paintImage;
		QImage m_gridImage;
		QImage m_image;
		GraphicsPixmapItem *m_paintPixmap;
		GraphicsPixmapItem *m_pixmap;
		GraphicsPixmapItem *m_gridPixmap;
		DataMemory* m_dataMemory;

	private:
		Contour m_contour;	

	private:
		int m_roiType;
		int m_paintType;
		int m_imageType;
		int m_dlibType;
		int m_id;

	public:
		PainterSettings m_painterSettings;
	private:
		QString m_currentDirectory;
		QString m_currentPaintDirectory;
		QString m_currentJsonDirectory;
		QString m_imageName;
		QString m_name;
		QString m_split;

};

#endif // VIEW_H
