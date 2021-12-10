#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "includespdlog.h"
#include "widgets/view/view.h"

#include "data.h"
#include "dlibnetwork.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class MainWindow : public QWidget
{
	Q_OBJECT

	public:
		MainWindow();
		MainWindow(QJsonObject const& a_config, QJsonObject const& a_postprocess);
		void configure(QJsonObject const a_config, QJsonObject const a_postprocess);

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
		QThread* m_fileLoggerTrainThread;
		FileLogger *m_fileLoggerTrain;

		QThread* m_fileLoggerTestThread;
		FileLogger *m_fileLoggerTest;

		QThread* m_fileLoggerJSONThread;
		FileLogger *m_fileLoggerJSON;

		QThread* m_dlibThread;
		DlibNetwork* m_dlib;
};

#endif
