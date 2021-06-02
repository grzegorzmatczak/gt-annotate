#include "imageprocessing/contour.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QJsonDocument>
#include <QDebug>
#include <QRectF>

constexpr auto NAME{ "Name" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto X{ "X" };
constexpr auto Y{ "Y" };
constexpr auto SIZE{ "Size" };

constexpr auto CONTOUR{ "Contour" };
constexpr auto CANNY_TRESHOLD{ "CannyTreshold" };
constexpr auto DILATE_COUNTER{ "DilateCounter" };
constexpr auto ERODE_COUNTER{ "ErodeCounter" };
constexpr auto MIN_AREA{ "MinArea" };
constexpr auto MAX_AREA{ "MaxArea" };


constexpr auto ROTATED_RECT{ "RotatedRect" };
constexpr auto BOUNDING_RECT{ "BoundingRect" };


Contour::Contour()
{
	configureDefault();
}

void Contour::configureDefault()
{
	m_treshCanny = 3;
	m_dilateCounter = 2;
	m_erodeCounter = 2;
	m_minArea = 4;
	m_maxArea = 40000;
}

void Contour::configure(const QJsonObject &a_config)
{
	m_treshCanny = a_config[CANNY_TRESHOLD].toInt();
	m_dilateCounter = a_config[DILATE_COUNTER].toInt();
	m_erodeCounter = a_config[ERODE_COUNTER].toInt();
	m_minArea = a_config[MIN_AREA].toInt();
	m_maxArea = a_config[MAX_AREA].toInt();
}

void Contour::CreateCannyImage(cv::Mat &opencv_img, cv::Mat &canny_output)
{
	Logger->trace("Contour::CreateCannyImage()");
	cv::dilate(opencv_img, opencv_img, cv::Mat(), cv::Point(-1, -1), m_dilateCounter, 1, 1);
	cv::erode(opencv_img, opencv_img, cv::Mat(), cv::Point(-1, -1), m_erodeCounter, 1, 1);
	cv::Canny(opencv_img, canny_output, m_treshCanny, m_treshCanny*2 );
}

void Contour::FindContours(cv::Mat & canny_output, QJsonArray & contoursArray, QString label)
{
	Logger->trace("Contour::FindContours()");
	std::vector<std::vector<cv::Point> > contours;
	std::vector<cv::Vec4i> hierarchy;
	cv::findContours(canny_output, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE );

	double area;
	std::vector<std::vector<cv::Point>> contoursBEST;
	std::vector<cv::Vec4i> hierarchyBEST;
	std::vector<cv::Point> approx;

	for (unsigned int i = 0; i < contours.size(); i++)
	{
		area = cv::contourArea(contours[i]);
		if ((area >= m_minArea) && (area <= m_maxArea))
		{
			contoursBEST.push_back(contours[i]);
			hierarchyBEST.push_back(hierarchy[i]);
		}
	}
	for (unsigned int i = 0; i < contoursBEST.size(); i++)
	{
		cv::approxPolyDP(cv::Mat(contoursBEST[i]), approx, cv::arcLength(cv::Mat(contoursBEST[i]), true) * 0.04, true);
		area = cv::contourArea(contoursBEST[i]);

		cv::Rect boundRect;
		boundRect = cv::boundingRect(contoursBEST[i]);

		/* // TODO: add support for rotatedrect:
		cv::RotatedRect;
		RotatedRect = minAreaRect( contoursBEST[i] );
		// rotated rectangle
        Point2f rect_points[4];
        minRect[i].points( rect_points );
        for ( int j = 0; j < 4; j++ )
        {
            line( drawing, rect_points[j], rect_points[(j+1)%4], color );
        }*/

		int x = boundRect.x+1;
		int y = boundRect.y+1;
		int width = boundRect.width-1;
		int height = boundRect.height-1;

		int size = qAbs(width / 2) * qAbs(height / 2);

		QJsonObject obj
		{
			{ NAME, label},
			{ X, x },
			{ Y, y },
			{ WIDTH, width },
			{ HEIGHT, height },
			{ SIZE, size}

		};
		contoursArray.append(obj);
		qDebug() << "obj:" << obj;
		qDebug() << "contoursArray:" << contoursArray;
	}
}

void Contour::CrateRois(cv::Mat &opencv_img, QString label, QJsonArray& contoursArray)
{
	Logger->trace("Contour::CrateRois()");
	cv::Mat canny_output;
	Contour::CreateCannyImage(opencv_img, canny_output);
	
	
	QString _name = label + "_canny_output.png";
	cv::imwrite(_name.toStdString(), canny_output);

	Contour::FindContours(canny_output, contoursArray, label);
	Logger->trace("Contour::CrateRois() done");
}
