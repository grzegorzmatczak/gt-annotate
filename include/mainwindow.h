#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "includespdlog.h"
#include "widgets/view.h"

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

	private slots:
        
	signals:
	

	public slots:

	private:
		
		QMenu* m_fileMenu;
		QMenuBar* m_menuBar;
		
		QTreeView* m_labelList;
		QVBoxLayout* m_rightLayout;
		QWidget* m_rightLayoutContainer;
		QGridLayout* m_gridLayout;
		QWidget* m_rightWidget;

	private:
		View* m_view;


};

#endif
