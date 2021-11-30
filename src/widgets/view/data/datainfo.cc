#include "widgets/view/data/datainfo.h"

#include "includespdlog.h"
#include "scanfiles.h"

#include <QDebug>
#include <QWidget>
#include <QListWidgetItem>


DataInfo::DataInfo()
{

}

DataInfo::DataInfo(const QJsonObject &a_config)
{
	
}

bool DataInfo::addImage(QString path, QString filename)
{
    Logger->trace("DataInfo::addImage(path:{}, filename:{})",path.toStdString(), filename.toStdString());

    cv::Mat image = cv::imread((path+filename).toStdString(),cv::IMREAD_GRAYSCALE);
    
	QListWidgetItem* item = new QListWidgetItem(filename);

	int counterElements = 0;

    if (image.empty())
    {
        Logger->error("data is empty, image:{}", (path+filename).toStdString());
    }
    else
    {
        for (int i = 0; i < image.cols; i++)
        {
            for (int j = 0; j < image.rows; j++)
            {
                if ((image.at<unsigned char>(j, i) > 0 ) && (image.at<unsigned char>(j, i) <= 255))
                {
                    counterElements++;
                }
            }
        }
    }

    data _data{ image, filename, path, counterElements, 0 };

    m_data.push_back(_data);
    return true;
}