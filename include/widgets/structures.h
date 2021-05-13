#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QDir>


enum uiMode
{
	None = 0,
	SelectROI = 1,
	Paint = 2,
	MoveSelect = 3
};

struct colors
{
	QString name;
	QColor color;
	qint32 gray;
};

QVector<QString> static scanAllImages(QString path)
{
	QVector<QString> temp;
	QDir directory(path);
	QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.png" << "*.PNG" << "*.JPG", QDir::Files);

	foreach (QString filename, images)
	{
		QStringList sl = filename.split(".");
		temp.push_back(sl[0]);
	}
	return temp;
}

#endif // STRUCTURES_H