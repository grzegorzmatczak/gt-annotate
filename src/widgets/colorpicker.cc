#include "widgets/colorpicker.h"

constexpr auto NAME{ "Name" };
constexpr auto COLORS{ "Colors" };
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

ColorPicker::ColorPicker(QJsonObject const &a_config, QWidget *parent)
	: QWidget(parent)
{
	QComboBox *colorPenComboBox = new QComboBox();
	auto colors = a_config[COLORS].toArray();
	for (int i = 0; i < colors.size(); i++)
	{
		auto colorsIter = colors[i].toObject();
		colorsIter[NAME].toString();
		qint32 r = colorsIter[R].toInt();
		qint32 g = colorsIter[G].toInt();
		qint32 b = colorsIter[B].toInt();

		colorPenComboBox->insertItem(i, colorsIter[NAME].toString());
		colorPenComboBox->setItemData(i, QColor(r, g, b), Qt::DecorationRole);
	}
	QHBoxLayout *penColorLayout = new QHBoxLayout;
	penColorLayout->setContentsMargins(0, 0, 0, 0);

	QLabel *labelPenColor = new QLabel();
	labelPenColor->setText("Pen Color:");

	colorPenComboBox->setMinimumWidth(80);

	connect(colorPenComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index) {
		emit(changeColor(colorPenComboBox->itemData(index, Qt::DecorationRole).value<QColor>()));
		qDebug() << "widthPenComboBox:" << colorPenComboBox->itemData(index, Qt::DecorationRole);
	});

	QVBoxLayout *Layout = new QVBoxLayout;
	Layout->setContentsMargins(0, 0, 0, 0);
	Layout->addWidget(labelPenColor);
	Layout->addWidget(colorPenComboBox);

	setLayout(Layout);
}