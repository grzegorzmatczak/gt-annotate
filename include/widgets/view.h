#ifndef VIEW_H
#define VIEW_H

#include <QtWidgets>
#include "includespdlog.h"
#include "widgets/graphicsscene.h"
#include "widgets/graphicsview.h"
#include "widgets/colorpicker.h"
#include "widgets/pensizepicker.h"
#include "widgets/toolbar.h"


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
		void onChangeColor(QColor color);
		void onChangePenSize(qint32 size);
		void onLoadDirectory();

	private:
		void addImageToScene(QPixmap image);
		void onResetScene();
		void resetView();

		void setupGraphicsView();
		void setupSliders();
		void setupLeftToolBar(QJsonObject const& a_config);
		void setupCentralWidget();
		void loadImage(QString imageName);
		

	private:
		GraphicsView *m_graphicsView;
		GraphicsScene *m_graphicsScene;

		QGridLayout *m_topLayout;
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
	private:
        QString m_targetDirectoryPath;

};
 
#endif // VIEW_H