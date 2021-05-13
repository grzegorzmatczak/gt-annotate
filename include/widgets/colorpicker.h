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

#include "structures.h"

class ColorPicker : public QWidget
{
	Q_OBJECT
	public:
		explicit ColorPicker(QJsonObject const &a_config, QWidget *parent = nullptr);

	signals:
		void changeColor(QColor color);
};

#endif // COLORPICKER_H