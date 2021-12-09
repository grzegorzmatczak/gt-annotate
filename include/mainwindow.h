#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "includespdlog.h"
#include "widgets/view/view.h"

#include "data.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class MainWindow : public QWidget
{
	Q_OBJECT

	public:
		MainWindow();
		MainWindow(QJsonObject const& a_config);
		void configure(QJsonObject const& a_config);

	private:
		void setupMainWidget();
		void createMenus();
		void setupView(QJsonObject const& a_config);
		void setupButtons();
		void setupLayout();

	private:
		QMenu* m_fileMenu;
		QMenuBar* m_menuBar;
		DataMemory* m_dataMemory;

	private:
		View* m_view;

};

#endif
