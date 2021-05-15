#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include <QHash>
#include "includespdlog.h"
#include "widgets/graphicsscene.h"
#include "widgets/graphicsview.h"
#include "widgets/colorpicker.h"
#include "widgets/pensizepicker.h"
#include "widgets/toolbar.h"
#include "widgets/paintersettings.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class View : public QFrame
{
	Q_OBJECT
	
	public:
		explicit View(QJsonObject const &a_config, QFrame *parent = nullptr);
		QGraphicsView *view() const;

        void configure(QJsonObject const& a_config, QHash<QString, int>& m_colorInthash, QHash<QString, QColor>& m_colorHash);

	public slots:
		void onPaintWhiteBoard(qint32 x, qint32 y);
		void onZoomIn(qint32 delta);
  		void onZoomOut(qint32 delta);
		void onChangeColor(QColor color);
		void onChangePenSize(qint32 size);	

	signals:
		void setModePaint();
		void setModeROI();
		void setModeMove();
		void resetScene();

	private slots:
		void setupMatrix();
		void setOpacity();
		void onSetPaint();
		void onSetMove();
		void onSetROI();
		void onLoadDirectory();

	private:
		void addImageToScene(QPixmap image);
		void onResetScene();
		void resetView();
		void setupProgressBar();
		void setupGraphicsView();
		void setupSliders();
		void setupLeftToolBar(QJsonObject const& a_config);
		void setupCentralWidget(QJsonObject const& a_config);
		void loadImage(QString imageName);
        void renderColorsFromImage(QString pathToImage);
        void onPaintColorsFinish();
        void onPaintColors(qint32 x, qint32 y, QColor color);
		void creteAction();
		

	private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;
		QProgressBar* m_progressBar;
		QGridLayout *m_vLayout;
		QGridLayout *m_hLayout;
		QSlider *m_opacitySlider;
		QSlider *m_zoomSlider;
		ToolBar* m_leftToolBar;
		QHBoxLayout* m_buttonLayout;
		QToolButton* m_loadButton;
		QToolButton* m_saveGTbutton;
		QWidget* m_buttonContainer;

	private:
		ColorPicker* m_colorPicker;
		QTextBrowser* logViewer;
		PenSizePicker* m_penSizePicker;

	private:
        PainterSettings m_painterSettings;
		qreal m_scaleOpacity;
		qreal m_scale;



	private:
		QImage m_diff;
		QImage m_image;
		QGraphicsPixmapItem *m_whitePixmap;
		QGraphicsPixmapItem *m_pixmap;
	private:
        QString m_targetDirectoryPath;

	private:
		QAction* action_paint;
		QAction* action_move;
		QAction* action_ROI;
		QAction* action_loadDirectory;

};
 
#endif // VIEW_H
