#ifndef COLORPICKER_WIDGET_H
#define COLORPICKER_WIDGET_H

#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonObject>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <QListWidget>

#include "widgets/structures.h"
#include "widgets/colorpickerlabel.h"


class ColorPickerWidget : public QListWidget
{
	Q_OBJECT
	public:
		explicit ColorPickerWidget(const QJsonObject &a_config);

	private:
		int addColors(QString color);
		
};

#endif // COLORPICKER_WIDGET_H
