#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include <QHash>

#include "includespdlog.h"
#include "data.h"

#include "widgets/view/graphicsscene.h"
#include "widgets/view/graphicsview.h"
#include "widgets/view/colorpicker.h"
#include "widgets/view/pensizepicker.h"
#include "widgets/view/toolbar.h"
#include "widgets/view/paintersettings.h"
#include "widgets/view/colorpickerwidget.h"
#include "widgets/view/graphicsrectitem.h"
#include "widgets/view/painter.h"
#include "widgets/view/data/datawidget.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class View : public QFrame
{
	Q_OBJECT
	
	public:
		explicit View(QJsonObject const &a_config, DataMemory* dataMemory, QFrame *parent = nullptr);
		QGraphicsView *view() const;
		void configure(QJsonObject const& a_config, QHash<QString, int>& m_colorInthash, QHash<QString, QColor>& m_colorHash);

	public slots:
		void onSetupMatrix();
		void onZoomIn(qint32 delta);
  		void onZoomOut(qint32 delta);
		void onChangeColor2(QListWidgetItem* item);
		void onChangeLabel(QListWidgetItem* current, QListWidgetItem* previous);
		void onLoadImage(int id);
		void onLoadPaints(int id);
		void onLoadRois(int id);
		void configure();
		void onTrainNetwork();
		void onUseNetwork(cv::Mat image, QRect rect);
		void onReturnUsedNetwork(cv::Mat image, QRect rect);

	signals:
		void setModePaint();
		void setModeROI();
		void setModeMove();
		void resetScene();
		void loadDirectory(QString folderName);
		void loadImage(int id);
		void loadPaints(int id);
		void loadRois(int id);
		void trainNetwork();
		void useNetwork(cv::Mat image, QRect rect);
		//void returnUsedNetwork(cv::Mat image, QRect rect);

	private slots:
		void onSetPaint();
		void onSetMove();
		void onSetROI();
		void onLoadDirectory();
		void setOpacity();
		void setOpacityROI();
		void setOpacityImage();

	private:
		void setupProgressBar();
		void setupGraphicsView();
		void setupSliders();
		void setupLeftToolBar();
		void setupBottomToolBar();
		void setupCentralWidget();
		void creteAction();
		void creteBottomAction();
		void onSaveWhiteBoard();
		void createMenus();
		void setupDataWidget();

	private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;
		QProgressBar* m_progressBar;
		QGridLayout *m_vLayout;
		QGridLayout *m_hLayout;
		QGridLayout* m_vLayoutBars;
		QSlider *m_opacitySlider;
		QSlider *m_opacitySliderROI;
		QSlider *m_opacitySliderImage;
		QSlider *m_zoomSlider;
		ToolBar* m_leftToolBar;
		ToolBar* m_bottomToolBar;
		QHBoxLayout* m_buttonLayout;
		QToolButton* m_loadButton;
		QToolButton* m_saveGTbutton;
		QWidget* m_buttonContainer;
		QStatusBar* m_statusBar;
		QMenu* m_fileMenu;
		QMenuBar* m_menuBar;

	private:
		ColorPicker* m_colorPicker;
		ColorPickerWidget* m_colorPickerWidget;
		QTextBrowser* logViewer;
		PenSizePicker* m_penSizePicker;

	private:
		QJsonObject m_config;

	private:
		Painter* m_painter;
		DataMemory* m_dataMemory;
		DataWidget* m_dataWidget;

	private:
		QAction* action_paint;
		QAction* action_move;
		QAction* action_ROI;
		QAction* action_loadDirectory;
		QAction* action_saveWhitePixmap;
		QAction* action_create_roi;
		QAction* action_nextImage;
		QAction* action_prevImage;
		QAction* action_copyNextImage;
		QAction* action_copyPrevImage;
		QAction* action_train_network;
		QAction* action_useNetwork;


	private:
		qreal m_scaleOpacity;
		qreal m_scaleOpacityROI;
		qreal m_scaleOpacityImage;
		qreal m_scale;

};
 
#endif // VIEW_H
