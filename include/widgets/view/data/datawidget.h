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


    private:
        void setupModelList();
        void setupTable();
        void setupModel();
        void clearModel();
        void clearModelList();
        void onUpdateFile(int id, QString name, int elements);
        

	private:
        QTreeView* m_labelList;
        QVBoxLayout* m_rightLayout;
		QWidget* m_rightLayoutContainer;
		QGridLayout* m_gridLayout;
		QWidget* m_rightWidget;
        QTreeView* proxyView;
        TreeWidget* treeWidget;
		

        QStandardItemModel* model;
	    QStandardItemModel* modelList;

        DataMemory* m_dataMemory;
        
	public:

    public slots:
        void open();
        void onLoadDirectory(QString folderName);
        void onUpdateFileFromId(int id);
        

    signals:
        void loadImage(QString dir, QString name);
        void loadPaints(int id);
		
};

#endif // DATA_WIDGET_H
