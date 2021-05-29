#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include <QHash>
#include "utils/includespdlog.h"
#include "utils/listvector.h"
#include "widgets/graphicsscene.h"
#include "widgets/graphicsview.h"
#include "widgets/colorpicker.h"
#include "widgets/pensizepicker.h"
#include "widgets/toolbar.h"
#include "widgets/paintersettings.h"
#include "widgets/colorpickerwidget.h"
#include "widgets/graphicsrectitem.h"
#include "widgets/painter.h"
#include "imageprocessing/contour.h"

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
		void onSetupMatrix();
		void onZoomIn(qint32 delta);
  		void onZoomOut(qint32 delta);
		void onChangeColor2(QListWidgetItem* item);
		void onChangeLabel(QListWidgetItem* current, QListWidgetItem* previous);

	signals:
		void setModePaint();
		void setModeROI();
		void setModeMove();
		void resetScene();

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
		void setupCentralWidget();
		void creteAction();
		void onSaveWhiteBoard();
		void createMenus();

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

	private:
        QString m_targetDirectoryPath;

	private:
		QAction* action_paint;
		QAction* action_move;
		QAction* action_ROI;
		QAction* action_loadDirectory;
		QAction* action_saveWhitePixmap;

	private:
		qreal m_scaleOpacity;
		qreal m_scaleOpacityROI;
		qreal m_scaleOpacityImage;

		qreal m_scale;

};
 
#endif // VIEW_H
