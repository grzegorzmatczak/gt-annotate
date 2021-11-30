#include "widgets/view/data/treewidget.h"

#include <QDebug>
#include <QWidget>
#include <QFileDialog>

#include "includespdlog.h"

constexpr auto DATASET{ "Dataset" };
constexpr auto GT_DIRECTRY{ "GtDirectory" };
constexpr auto IMAGE_DIRECTORY{ "ImageDirectory" };
constexpr auto JSON_DIRECTORY{ "JsonDirectory" };


TreeWidget::TreeWidget(const QJsonObject &a_config)
{
	Logger->trace("TreeWidget::TreeWidget()");

	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX

	m_currentDirectory = a_config[DATASET].toObject()[IMAGE_DIRECTORY].toString();
	m_currentPaintDirectory = a_config[DATASET].toObject()[GT_DIRECTRY].toString();
	m_currentJsonDirectory = a_config[DATASET].toObject()[JSON_DIRECTORY].toString();
}

TreeWidget::TreeWidget()
{
	Logger->trace("TreeWidget::TreeWidget()");
	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX
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

void TreeWidget::onCopyFromNextFile()
{
	Logger->warn("TreeWidget::onCopyFromNextFile()");
	QTreeWidgetItem *currentItem = this->currentItem();
	if (!currentItem)
	{
		Logger->error("!currentItem");
		return;
	}
	QTreeWidgetItem *nextItem = this->itemBelow(currentItem);
	if (!nextItem) 
	{
		Logger->error("!nextItem");
		return;
	}
	QString iFile = nextItem->text(0);
	QString iDir = currentDir();
	if (iFile.isEmpty() || iDir.isEmpty())
	{
		Logger->error("iFile.isEmpty() || iDir.isEmpty()");
		return;
	}
	QString iName = iFile.split('.')[0];
	//Logger->trace("TreeWidget::onCopyFromNextFile() emit loadRois({}, {})", m_currentJsonDirectory.toStdString(), iName.toStdString());
	//emit(loadRois(m_currentJsonDirectory, iName));
	Logger->trace("TreeWidget::onCopyFromNextFile() emit loadPaint({}, {})", m_currentPaintDirectory.toStdString(), iName.toStdString());
	emit(loadPaint(m_currentPaintDirectory, iName));
}

void TreeWidget::onCopyFromPreviousFile()
{
	Logger->warn("TreeWidget::onCopyFromPreviousFile()");
	QTreeWidgetItem *currentItem = this->currentItem();
	if (!currentItem)
	{
		Logger->error("!currentItem");
		return;
	}
	QTreeWidgetItem *previousItem = this->itemAbove(currentItem);
	if (!previousItem) 
	{
		Logger->error("!previousItem");
		return;
	}
	QString iFile = previousItem->text(0);
	QString iDir = currentDir();
	if (iFile.isEmpty() || iDir.isEmpty())
	{
		Logger->error("iFile.isEmpty() || iDir.isEmpty()");
		return;
	}
	QString iName = iFile.split('.')[0];
	//Logger->trace("TreeWidget::onCopyFromPreviousFile() emit loadRois({}, {})", m_currentJsonDirectory.toStdString(), iName.toStdString());
	//emit(loadRois(m_currentJsonDirectory, iName));
	Logger->trace("TreeWidget::onCopyFromPreviousFile() emit loadPaint({}, {})", m_currentPaintDirectory.toStdString(), iName.toStdString());
	emit(loadPaint(m_currentPaintDirectory, iName));
}

void TreeWidget::onNextFile()
{
	Logger->trace("TreeWidget::onNextFile()");
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
	//emit(itemClicked());
	onItemClicked();
}

void TreeWidget::onPreviousFile() 
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
	//emit(itemClicked());
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
	QString iName = iFile.split('.')[0];
	emit(loadImage(m_currentDirectory, iName));
	emit(loadRois(m_currentJsonDirectory, iName));
	emit(loadPaint(m_currentPaintDirectory, iName));
}

void TreeWidget::onOpenDirectory()
{
	this->openDirectory();
}

void TreeWidget::onOpenDialogDirectory()
{
	QString openedDir = QFileDialog::getExistingDirectory(this, "Choose a directory");
	if (openedDir.isEmpty())
		return;

	this->openDirectory();
	m_currentDirectory = openedDir;
}

void TreeWidget::onSaveRoidAndPaint()
{	
	
	Logger->trace("TreeWidget::onSaveRoidAndPaint()");
	QString iFile = currentFile();
	QString iName = iFile.split('.')[0];
	Logger->trace("TreeWidget::onSaveRoidAndPaint() emit saveRois with ({}, {})", m_currentJsonDirectory.toStdString(), iName.toStdString());
	Logger->trace("TreeWidget::onSaveRoidAndPaint() emit savePaint with ({}, {})", m_currentPaintDirectory.toStdString(), iName.toStdString());
	
	emit(saveRois(m_currentJsonDirectory, iName));
	emit(savePaint(m_currentPaintDirectory, iName));

	QTreeWidgetItem *currentItem = this->currentItem();
	currentItem->setBackgroundColor(0, QColor(255, 0, 0, 100)); 
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