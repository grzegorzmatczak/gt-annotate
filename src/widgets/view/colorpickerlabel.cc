#include "widgets/view/colorpickerlabel.h"

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


ColorPickerLabel::ColorPickerLabel(const QJsonObject &a_config)
    : m_isSelected(false)
{
    ColorPickerLabel::configure(a_config);
    //this->setText("default");
    this->setAlignment(Qt::AlignHCenter);
}


static QColor readableColor(const QColor & color)
{
	int r, g, b;
	color.getRgb(&r, &g, &b);
    #ifdef DEBUG
        Logger->debug("ColorPickerLabel::readableColor()  rgb:{},{},{}" , r, g, b);
    #endif

    if( r<=10 && g<=10 && b<=10)
    {
        #ifdef DEBUG
        Logger->debug("ColorPickerLabel::readableColor() return QColor(255, 255, 255, 100)");
        #endif
        return QColor(255, 255, 255, 100);
    }

	if ((r*0.299 + g*0.587 + b*0.114) > 150)
    {
        #ifdef DEBUG
        Logger->debug("ColorPickerLabel::readableColor() return QColor(0, 0, 0)");
        #endif
        return QColor(0, 0, 0);
    }
    #ifdef DEBUG
    Logger->debug("ColorPickerLabel::readableColor() return QColor(255, 255, 255)");
    #endif
	return QColor(255, 255, 255);
}

void ColorPickerLabel::setNewColor(QColor color)
{
    m_color = color;
    setStyleSheet("QLabel { background-color : " + m_color.name() + "; color : " + readableColor(m_color).name() + "; font:  14px; }");
}

void ColorPickerLabel::configure(const QJsonObject &a_config) 
{
    #ifdef DEBUG
    Logger->debug("ColorPickerLabel::configure()");
    qDebug()<< "ColorPickerLabel::configure() a_config:" << a_config;
    #endif
    qint32 r = a_config[R].toInt();
    qint32 g = a_config[G].toInt();
    qint32 b = a_config[B].toInt();
    qint32 a = a_config[B].toInt();
    m_color = QColor(r, g, b);
    m_name = a_config[NAME].toString();
    this->setText(a_config[NAME].toString());
    #ifdef DEBUG
    Logger->debug("ColorPickerLabel::configure() setStyleSheet name:{}", m_color.name().toStdString());
    #endif
    setStyleSheet("QLabel { background-color : " + m_color.name() + "; color : " + readableColor(m_color).name() + "; font:  14px; }");
    #ifdef DEBUG
    Logger->debug("ColorPickerLabel::configure() done");
    #endif
}

void ColorPickerLabel::setSelected(bool s)
{
    m_isSelected = true;
    repaint();
}

void ColorPickerLabel::paintEvent(QPaintEvent *event) 
{
	QLabel::paintEvent(event);
}

QString ColorPickerLabel::getName()
{
    return "color";
}
