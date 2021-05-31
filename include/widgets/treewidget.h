#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>

#include "widgets/structures.h"


class TreeWidget : public QTreeWidget
{
	Q_OBJECT
	public:
		explicit TreeWidget(const QJsonObject &a_config);
        QString currentDir() const;
	    QString currentFile() const;
        void nextFile();
        void previousFile();
        

	private:
		void openDirectory();

	public slots:
		void onOpenDirectory();
		void onOpenDialogDirectory();
		void onItemClicked();

	signals:
		void loadImage(QString imageName);
		void setCurrentPaintFolder(QString imageFolder, QString paintFolder, QString jsonDirectory);

	private:
		QString m_currentDirectory;
		QString m_currentPaintDirectory;
		QString m_currentJsonDirectory;
		
};

#endif // TREEWIDGET_H
