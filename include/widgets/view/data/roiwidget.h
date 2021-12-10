#ifndef ROI_WIDGET_H
#define ROI_WIDGET_H

#include <QTreeWidget>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>


class RoiWidget : public QTreeWidget
{
	Q_OBJECT
	public:
		explicit RoiWidget(const QJsonObject &a_config);
		explicit RoiWidget();
		int currentId() const;
		int idFromItem(QTreeWidgetItem *item) const;

	private:

	public slots:
		void onItemClicked();
		
	signals:
		void clicked();

	private:
		QString m_split;
		
};

#endif // ROI_WIDGET_H
