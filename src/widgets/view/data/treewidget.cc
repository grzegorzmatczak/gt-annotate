#include "widgets/view/data/treewidget.h"

#include <QDebug>
#include <QWidget>
#include <QFileDialog>

#include "includespdlog.h"

//#define DEBUG

constexpr auto DATASET{ "Dataset" };
constexpr auto GT_DIRECTRY{ "GtDirectory" };
constexpr auto IMAGE_DIRECTORY{ "ImageDirectory" };
constexpr auto JSON_DIRECTORY{ "JsonDirectory" };


TreeWidget::TreeWidget(const QJsonObject &a_config)
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::TreeWidget()");
	#endif

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
	#ifdef DEBUG
	Logger->debug("TreeWidget::TreeWidget()");
	#endif
	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX
}

int TreeWidget::currentId() const
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::currentId()");
	#endif

	QList<QTreeWidgetItem *> selectedItem = this->selectedItems();
	if(selectedItem.size()>0)
	{
		return selectedItem[0]->text(0).toInt();
		selectedItem[0]->setBackground(1, QColor(0, 0, 200, 125)); 
	}
	return 0;
	
}

int TreeWidget::idFromItem(QTreeWidgetItem *item) const
{
	return item->text(0).toInt();

}

void TreeWidget::onCopyFromNextFile()
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::onCopyFromNextFile()");
	#endif
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
	int id = idFromItem(nextItem);
	emit(loadPaint(id));

}

void TreeWidget::onCopyFromPrevFile()
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::onCopyFromPrevFile()");
	#endif
	QTreeWidgetItem *currentItem = this->currentItem();
	if (!currentItem)
	{
		return;
	}
	QTreeWidgetItem *nextItem = this->itemAbove(currentItem);
	if (!nextItem)
	{
		return;
	}
	int id = idFromItem(nextItem);
	emit(loadPaint(id));
}

void TreeWidget::onNextFile()
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::onNextFile()");
	#endif
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

void TreeWidget::onPrevFile() 
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::onPrevFile()");
	#endif
	QTreeWidgetItem *currentItem = this->currentItem();
	if (!currentItem)
	{
		return;
	}
	QTreeWidgetItem *nextItem = this->itemAbove(currentItem);
	if (!nextItem)
	{
		return;
	}
	this->setCurrentItem(nextItem);
	onItemClicked();
}

void TreeWidget::onItemClicked()
{
	#ifdef DEBUG
	Logger->debug("TreeWidget::onItemClicked()");
	#endif
	int id = currentId();
	emit(loadImage(id));
	emit(loadRois(id));
	emit(loadPaint(id));
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
	#ifdef DEBUG
	Logger->debug("TreeWidget::onSaveRoidAndPaint()");
	#endif
	
	int id = currentId();
	emit(saveRois(id));
	emit(savePaint(id));

	QTreeWidgetItem *currentItem = this->currentItem();
	currentItem->setBackground(0, QColor(255, 0, 0, 100)); 
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
}