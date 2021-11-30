#pragma once

//#include "structures.h"
#include <QObject>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



class DataMemory : public QObject
{
	Q_OBJECT

public:
	DataMemory();
	~DataMemory();

public slots:

	void loadCleanData();
	void cleanData();

private:
	qint32 m_width;
	qint32 m_height;
	bool m_loaded{};

private:
	qint32 m_stopFrame{};
	qint32 m_startFrame{};
	qint32 m_startGT{};
	qint32 m_stopGT{};

	bool m_negativeGT{};
	bool m_negativeInput{};
	bool m_resize{};

private:
	cv::VideoCapture m_videoFromFile;
	cv::VideoCapture m_videoFromFileGT;

	QVector<cv::Mat> m_cleanData;
	QVector<cv::Mat> m_inputData;
	QVector<cv::Mat> m_gtData;

};