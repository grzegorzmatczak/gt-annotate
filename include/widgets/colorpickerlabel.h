#ifndef COLORPICKER_LABEL_H 
#define COLORPICKER_LABEL_H

#include <QLabel>
#include <QJsonObject>

#include "widgets/structures.h"


class ColorPickerLabel : public QLabel {
	Q_OBJECT

public:
	ColorPickerLabel(const QJsonObject &a_config);
	
	void configure(const QJsonObject &a_config);
	void setSelected(bool s);
	QString getName();
	void setNewColor(QColor color);

public : // override
	void paintEvent(QPaintEvent *event) override;

public:
	QColor m_color;
	QString m_name;

private:
	bool m_isSelected{};
};

#endif //COLORPICKER_LABEL_H