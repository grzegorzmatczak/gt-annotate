#include "widgets/view/colorpickerwidget.h"

#include <QDebug>
#include <QPainter>

//#define DEBUG

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
    #ifdef DEBUG
	Logger->debug("ColorPickerWidget::ColorPickerWidget()");
	#endif
    this->clear();
    int counter = 0;
	QJsonArray colors = a_config[COLORS_FOREGROUND].toArray() ;
    QJsonArray colorsBackground = a_config[COLORS_BACKGROUND].toArray() ;

    for (int i = 0; i < colorsBackground.size(); i++)
    {
        colors.append(colorsBackground[i].toObject());
    }
    #ifdef DEBUG
	qDebug()<< "colors:" << colors;
    qDebug()<< "COLORS_BACKGROUND:" << a_config[COLORS_BACKGROUND].toArray();
	#endif
    
	for (int i = 0; i < colors.size(); i++)
	{
        QListWidgetItem * item = new QListWidgetItem(this);
		QJsonObject colorsIter = colors[i].toObject();
        #ifdef DEBUG
        Logger->debug("ColorPickerWidget::ColorPickerWidget() i:{}", i);
        qDebug() << "colorsIter:" << colorsIter;
        #endif
        ColorPickerLabel * label = new ColorPickerLabel(colorsIter);
		counter++;
        item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        int row = this->currentRow();
        this->addItem(item);
        #ifdef DEBUG
        Logger->debug("ColorPickerWidget::ColorPickerWidget() row:{}", row);
        Logger->debug("ColorPickerWidget::ColorPickerWidget() addItem");
        Logger->debug("ColorPickerWidget::ColorPickerWidget() setItemWidget");
        #endif

        this->setItemWidget(item, label);
    }
}

int ColorPickerWidget::addColors(QString color){return 1;}

QSize ColorPickerWidget::minimumSizeHint() const
{
  return QSize(150, 200);
}

QSize ColorPickerWidget::sizeHint() const
{
  return QSize(150, 200);
}
