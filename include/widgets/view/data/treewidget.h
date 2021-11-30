#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>

//#include "widgets/structures.h"


class TreeWidget : public QTreeWidget
{
	Q_OBJECT
	public:
		explicit TreeWidget(const QJsonObject &a_config);
		explicit TreeWidget();
        QString currentDir() const;
	    QString currentFile() const;
	private:
		void openDirectory();

	public slots:
		void onOpenDirectory();
		void onOpenDialogDirectory();
		void onItemClicked();
		void onCopyFromNextFile();
		void onCopyFromPreviousFile();
		void onNextFile();
        void onPreviousFile();
		void onSaveRoidAndPaint();

	signals:
		void loadImage(QString dir, QString name);
		void loadRois(QString dir, QString name);
		void loadPaint(QString dir, QString name);
		void saveRois(QString dir, QString name);
		void savePaint(QString dir, QString name);
		void setCurrentPaintFolder(QString imageFolder, QString paintFolder, QString jsonDirectory);

	private:
		QString m_currentDirectory;
		QString m_currentPaintDirectory;
		QString m_currentJsonDirectory;
		QString m_split;
		
};

#endif // TREEWIDGET_H
