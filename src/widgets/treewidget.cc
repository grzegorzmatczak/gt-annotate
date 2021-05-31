#include "widgets/treewidget.h"

#include <QDebug>
#include <QWidget>
#include <QFileDialog>

constexpr auto DATASET{ "Dataset" };
constexpr auto GT_DIRECTRY{ "GtDirectory" };
constexpr auto IMAGE_DIRECTORY{ "ImageDirectory" };
constexpr auto JSON_DIRECTORY{ "JsonDirectory" };


TreeWidget::TreeWidget(const QJsonObject &a_config)
{
	Logger->trace("TreeWidget::TreeWidget()");

	m_currentDirectory = a_config[DATASET].toObject()[IMAGE_DIRECTORY].toString();
	m_currentPaintDirectory = a_config[DATASET].toObject()[GT_DIRECTRY].toString();
	m_currentJsonDirectory = a_config[DATASET].toObject()[JSON_DIRECTORY].toString();
}

QString TreeWidget::currentDir() const
{
	Logger->trace("TreeWidget::currentDir()");
	QTreeWidgetItem *current = this->currentItem();
	if (!current || !current->parent())
	{
		return "";
	}
	return current->parent()->text(0);
}

QString TreeWidget::currentFile() const
{
	Logger->trace("TreeWidget::currentFile()");
	QTreeWidgetItem *current = this->currentItem();
	if (!current || !current->parent())
	{
		return "";
	}
	return current->text(0);
}

void TreeWidget::nextFile()
{
	Logger->trace("TreeWidget::nextFile()");
	QTreeWidgetItem *currentItem = this->currentItem();
	if (!currentItem)
	{
		return;
	}
	QTreeWidgetItem *nextItem = this->itemBelow(currentItem);
	if (!nextItem)
	{
		return;
	}
	this->setCurrentItem(nextItem);
	onItemClicked();
}

void TreeWidget::previousFile() 
{
	Logger->trace("TreeWidget::previousFile()");
	QTreeWidgetItem *currentItem = this->currentItem();
	if (!currentItem)
	{
		return;
	}
	QTreeWidgetItem *previousItem = this->itemAbove(currentItem);
	if (!previousItem) 
	{
		return;
	}
	this->setCurrentItem(previousItem);
	onItemClicked();
}

void TreeWidget::onItemClicked()
{
	Logger->trace("TreeWidget::onItemClicked()");
	QString iFile = currentFile();
	QString iDir = currentDir();
	if (iFile.isEmpty() || iDir.isEmpty())
	{
		return;
	}
	emit(setCurrentPaintFolder(m_currentDirectory, m_currentPaintDirectory, m_currentJsonDirectory));
	emit(loadImage(iFile));
	

	//allDisconnnect(image_canvas);
	//int index = getImageCanvas(iFile, image_canvas);
	//updateConnect(image_canvas);
	//tabWidget->setCurrentIndex(index);
}

void TreeWidget::onOpenDirectory()
{
	this->openDirectory();
}

void TreeWidget::onOpenDialogDirectory()
{
	//statusBar()->clearMessage();
	QString openedDir = QFileDialog::getExistingDirectory(this, "Choose a directory");
	if (openedDir.isEmpty())
		return;

	this->openDirectory();
	m_currentDirectory = openedDir;
	//m_currentPaintDirectory
}

void TreeWidget::openDirectory()
{
	QTreeWidgetItem *currentTreeDir = new QTreeWidgetItem(this);
	currentTreeDir->setExpanded(true);
	currentTreeDir->setText(0, m_currentDirectory);

	QDir current_dir(m_currentDirectory);
	QStringList files = current_dir.entryList();
	static QStringList ext_img = { "png","jpg","bmp","pgm","jpeg" ,"jpe" ,"jp2" ,"pbm" ,"ppm" ,"tiff" ,"tif" };
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].size() < 4)
			continue;

		QString ext = files[i].section(".", -1, -1);
		bool is_image = false;
		for (int e = 0; e < ext_img.size(); e++)
		{
			if (ext.toLower() == ext_img[e])
			{
				is_image = true;
				break;
			}
		}
		if (!is_image)
			continue;

		if (files[i].toLower().indexOf("_mask.png") > -1)
			continue;

		QTreeWidgetItem *currentFile = new QTreeWidgetItem(currentTreeDir);
		currentFile->setText(0, files[i]);
	}
	// setWindowTitle("gt-annotate - " + m_currentDirectory);
}