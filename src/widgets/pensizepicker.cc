#include "widgets/pensizepicker.h"

PenSizePicker::PenSizePicker(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout *penSizeLayout = new QVBoxLayout;
	penSizeLayout->setContentsMargins(0, 0, 0, 0);

	QLabel *labelPenSize = new QLabel();
	labelPenSize->setText("Pen Size:");

	QComboBox *widthPenComboBox = new QComboBox();

	widthPenComboBox->insertItem(0, "1");
	widthPenComboBox->setItemData(0, 1, Qt::DecorationRole);
	widthPenComboBox->insertItem(1, "3");
	widthPenComboBox->setItemData(1, 2, Qt::DecorationRole);
	widthPenComboBox->insertItem(2, "5");
	widthPenComboBox->setItemData(2, 3, Qt::DecorationRole);
	widthPenComboBox->insertItem(3, "7");
	widthPenComboBox->setItemData(3, 4, Qt::DecorationRole);
	widthPenComboBox->insertItem(4, "9");
	widthPenComboBox->setItemData(4, 5, Qt::DecorationRole);

	penSizeLayout->addWidget(labelPenSize);
	penSizeLayout->addWidget(widthPenComboBox);
	setLayout(penSizeLayout);

	connect(widthPenComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
		[=](int index) { 
		emit(changePenSize(widthPenComboBox->itemData(index, Qt::DecorationRole).toInt()));
		qDebug() << "widthPenComboBox:" << widthPenComboBox->itemData(index, Qt::DecorationRole); 
		});
}