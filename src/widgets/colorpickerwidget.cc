#include "widgets/colorpickerwidget.h"

#include <QDebug>
#include <QPainter>

constexpr auto NAME{ "Name" };
constexpr auto COLORS_FOREGROUND{ "ColorsForeground" };
constexpr auto COLORS_BACKGROUND{ "ColorsBackground" };
constexpr auto BLACK{ "Black" };
constexpr auto WHITE{ "White" };
constexpr auto SHADOW{ "Shadow" };
constexpr auto OUT_OF_SCOPE{ "OutOfScope" };
constexpr auto UNKNOWN{ "Unknown" };

constexpr auto ROI{ "ROI" };
constexpr auto GRAY_COLOR{ "GrayColor" };
constexpr auto R{ "R" };
constexpr auto G{ "G" };
constexpr auto B{ "B" };
constexpr auto A{ "A" };


ColorPickerWidget::ColorPickerWidget(const QJsonObject &a_config)
{
    this->clear();
    int counter = 0;
	QJsonArray colors = a_config[COLORS_FOREGROUND].toArray() ;
    QJsonArray colorsBack = a_config[COLORS_BACKGROUND].toArray() ;


    for (int i = 0; i < colorsBack.size(); i++)
    {
        colors.append(colorsBack[i].toObject());
    }

    qDebug()<< "colors:" << colors;
    qDebug()<< "COLORS_BACKGROUND:" << a_config[COLORS_BACKGROUND].toArray();
	for (int i = 0; i < colors.size(); i++)
	{
        QListWidgetItem * item = new QListWidgetItem(this);
        Logger->warn("ColorPickerWidget::ColorPickerWidget() i:{}", i);
		QJsonObject colorsIter = colors[i].toObject();
        qDebug() << "colorsIter:" << colorsIter;
        ColorPickerLabel * label = new ColorPickerLabel(colorsIter);
		counter++;
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        int row = this->currentRow();
        Logger->warn("ColorPickerWidget::ColorPickerWidget() row:{}", row);
        Logger->warn("ColorPickerWidget::ColorPickerWidget() addItem");
        this->addItem(item);
        Logger->warn("ColorPickerWidget::ColorPickerWidget() setItemWidget");
        this->setItemWidget(item, label);
    }
}

int ColorPickerWidget::addColors(QString color)
{

}

