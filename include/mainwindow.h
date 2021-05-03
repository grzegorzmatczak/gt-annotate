#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>

#include "includespdlog.h"

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

private slots:

signals:

public slots:

private:

};

#endif