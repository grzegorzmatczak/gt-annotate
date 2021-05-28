#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "widgets/structures.h"

class ColorPicker : public QWidget
{
	Q_OBJECT
	public:
		explicit ColorPicker(QJsonObject const &a_config, QWidget *parent = nullptr);

	private:
		int addColors(QString color);
	signals:
		void changeColor(QColor color);
	
	private:
		QJsonObject m_config;
		QComboBox *m_colorPenComboBox;
};

#endif // COLORPICKER_H
