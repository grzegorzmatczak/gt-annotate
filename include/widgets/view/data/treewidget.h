#ifndef TREEWIDGET_H
#define TREEWIDGET_H

#include <QTreeWidget>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QWidget>


class TreeWidget : public QTreeWidget
{
	Q_OBJECT
	public:
		explicit TreeWidget(const QJsonObject &a_config);
		explicit TreeWidget();
		int currentId() const;
		int idFromItem(QTreeWidgetItem *item) const;

	private:
		void openDirectory();

	public slots:
		void onOpenDirectory();
		void onOpenDialogDirectory();
		void onItemClicked();
		void onCopyFromNextFile();
		void onCopyFromPrevFile();
		void onNextFile();
		void onPrevFile();
		void onSaveRoidAndPaint();

	signals:
		void clicked();
		void loadImage(int id);
		void loadRois(int id);
		void loadPaint(int id);
		void saveRois(int id);
		void savePaint(int id);
		void setCurrentPaintFolder(QString imageFolder, QString paintFolder, QString jsonDirectory);

	private:
		QString m_currentDirectory;
		QString m_currentPaintDirectory;
		QString m_currentJsonDirectory;
		QString m_split;
		
};

#endif // TREEWIDGET_H
