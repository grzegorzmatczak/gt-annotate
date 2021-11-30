#ifndef PENSIZEPICKER_H
#define PENSIZEPICKER_H

#include <QComboBox>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

#include "widgets/view/structures.h"


class PenSizePicker : public QWidget
{
	Q_OBJECT
	public:
		explicit PenSizePicker(QWidget *parent = nullptr);

	signals:
		void changePenSize(qint32 size);
};

#endif // PENSIZEPICKER_H
