#ifndef DATA_INFO_H
#define DATA_INFO_H

#include <QJsonArray>
#include <QJsonObject>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

struct data
{
	cv::Mat image;
	QString name;
	QString path;
	int changedPixel;
	int sizeOfROI;
};

class DataInfo
{
	public:
		explicit DataInfo();
		explicit DataInfo(const QJsonObject &a_config);
		bool addImage(QString path, QString filename);

	private:
		std::vector<data> m_data;

};

#endif // DATA_WIDGET_H
