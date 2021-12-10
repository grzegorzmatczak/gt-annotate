#ifndef DATA_WIDGET_H
#define DATA_WIDGET_H

#include "widgets/view/data/datainfo.h"
#include "data.h"
#include "widgets/view/data/treewidget.h"
#include "widgets/view/data/roiwidget.h"
#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QTreeView>
#include <QVBoxLayout>
#include <QGridLayout>


class DataWidget : public QWidget
{
	Q_OBJECT
	public:
		explicit DataWidget(DataMemory* dataMemory);
		TreeWidget* treeWidget;
		RoiWidget* roiWidget;

	private:
		void setupTreeWidget();
		void setupRoiWidget();
		void onUpdateFile(int id, QString name, int elements, int roi, QString status);

	public slots:
		void open();
		void onLoadDirectory(QString folderName);
		void onUpdateFileFromId(int id);
		void onUpdateStatus(int id, QString status);
		void onNextFile();
		void onPrevFile();
		void onCopyFromNextFile();
		void onCopyFromPrevFile();
		void onAddRoi(int id, QString label, int size, bool enable);
		void clearTreeWidget();
		void clearRoiWidget();

	signals:
		void loadImage(int id);
		void loadPaints(int id);
		void loadRois(int id);    

	private:
		QVBoxLayout* m_rightLayout;
		QWidget* m_rightLayoutContainer;
		QGridLayout* m_gridLayout;
		DataMemory* m_dataMemory;

};

#endif // DATA_WIDGET_H
