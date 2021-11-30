#ifndef CONTOUR_H
#define CONTOUR_H

#include <stdio.h>

class QJsonObject;
class QJsonArray;
class QFile;
class QString;
class QJsonDocument;
class QDebug;
class QRectF;

#include "includespdlog.h"
//#include "widgets/graphicsrectitem.h"
//#include "widgets/paintersettings.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class Contour
{
	public:
		Contour();
		void configure(const QJsonObject &a_config);
		void CreateCannyImage(cv::Mat &opencv_img, cv::Mat &canny_output);
		void FindContours(cv::Mat & canny_output, QJsonArray & contoursArray, QString label);
		//void CrateRois(cv::Mat &opencv_img, QString label, QJsonArray& contoursArray);

	private:
		//PainterSettings * m_painterSettings;
		void configureDefault();

	private:
		int m_treshCanny{};
		int m_dilateCounter{};
		int m_erodeCounter{};
		int m_minArea{};
		int m_maxArea{};
};

#endif //CONTOUR_H
