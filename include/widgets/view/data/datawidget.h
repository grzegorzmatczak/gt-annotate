#ifndef DATA_WIDGET_H
#define DATA_WIDGET_H

#include "widgets/view/data/datainfo.h"
#include "data.h"
#include "widgets/view/data/treewidget.h"

#include <QWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QTreeView>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QStandardItemModel>


class DataWidget : public QWidget
{
	Q_OBJECT
	public:
		explicit DataWidget(DataMemory* dataMemory);
		TreeWidget* treeWidget;

	private:
		void setupModelList();
		void setupTable();
		void setupModel();
		void clearModel();
		void clearModelList();
		void onUpdateFile(int id, QString name, int elements, QString status);

	public slots:
		void open();
		void onLoadDirectory(QString folderName);
		void onUpdateFileFromId(int id);
		void onUpdateStatus(int id, QString status);
		void onNextFile();
		void onPrevFile();
		void onCopyFromNextFile();
		void onCopyFromPrevFile();

	signals:
		void loadImage(int id);
		void loadPaints(int id);
		void loadRois(int id);    

	private:
		QTreeView* m_labelList;
		QVBoxLayout* m_rightLayout;
		QWidget* m_rightLayoutContainer;
		QGridLayout* m_gridLayout;
		QWidget* m_rightWidget;
		QTreeView* proxyView;
		QStandardItemModel* model;
		QStandardItemModel* modelList;
		DataMemory* m_dataMemory;

};

#endif // DATA_WIDGET_H
