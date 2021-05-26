#include "widgets/colorpicker.h"

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

int ColorPicker::addColors(QString color)
{
	int counter = 0;
	auto colors = m_config[color].toArray();
	for (int i = 0; i < colors.size(); i++)
	{
		auto colorsIter = colors[i].toObject();
		//colorsIter[NAME].toString();
		qint32 r = colorsIter[R].toInt();
		qint32 g = colorsIter[G].toInt();
		qint32 b = colorsIter[B].toInt();
		qint32 a = colorsIter[A].toInt();

		m_colorPenComboBox->insertItem(i, colorsIter[NAME].toString());
		m_colorPenComboBox->setItemData(i, QColor(r, g, b, a), Qt::DecorationRole);

		counter++;
	}
	return counter;
}

ColorPicker::ColorPicker(QJsonObject const &a_config, QWidget *parent)
	: QWidget(parent)
	, m_config(a_config) 
{
	m_colorPenComboBox = new QComboBox();
	int colorAdded{0};

	colorAdded = ColorPicker::addColors(COLORS_FOREGROUND);
	

	QHBoxLayout *penColorLayout = new QHBoxLayout;
	penColorLayout->setContentsMargins(0, 0, 0, 0);

	QLabel *labelPenColor = new QLabel();
	labelPenColor->setText("Pen Color:");

	m_colorPenComboBox->setMinimumWidth(80);

	connect(m_colorPenComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		emit(changeColor(m_colorPenComboBox->itemData(index, Qt::DecorationRole).value<QColor>()));
		qDebug() << "colorPenComboBox:" << m_colorPenComboBox->itemData(index, Qt::DecorationRole);
	});

	QVBoxLayout *Layout = new QVBoxLayout;
	Layout->setContentsMargins(0, 0, 0, 0);
	Layout->addWidget(labelPenColor);
	Layout->addWidget(m_colorPenComboBox);

	if(colorAdded)
		{m_colorPenComboBox->setCurrentIndex(1);}

	setLayout(Layout);
}

