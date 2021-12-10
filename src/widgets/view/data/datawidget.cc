#include "widgets/view/data/datawidget.h"

#include "includespdlog.h"
#include "scanfiles.h"

#include <QDebug>
#include <QWidget>
#include <QFile>

#define DEBUG

constexpr auto ROI{ "ROI" };

class QStandardItemModel;
class QStandardItem;
class QJsonObject;

DataWidget::DataWidget(DataMemory* dataMemory)
: m_dataMemory(dataMemory)
{
	#ifdef DEBUG
	Logger->debug("DataWidget::DataWidget()");
	#endif
    m_rightLayout = new QVBoxLayout;
	m_rightLayout->setContentsMargins(0, 0, 0, 0);
    DataWidget::setupTreeWidget();
	DataWidget::setupRoiWidget();
	m_rightLayout->addWidget(roiWidget);
    m_rightLayout->addWidget(treeWidget);
    
	m_rightLayoutContainer = new QWidget;
	m_rightLayoutContainer->setLayout(m_rightLayout);
	m_gridLayout = new QGridLayout(this);
	m_gridLayout->addWidget(m_rightLayoutContainer, 0, 1);
	this->setLayout(m_gridLayout); 
}

void DataWidget::onUpdateFile(int id, QString name, int elements, int roi, QString status)
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	item->setText(0, QString::number(id));
	item->setText(1, name);
	item->setText(2, QString::number(elements));
	item->setText(3, QString::number(roi));
	item->setText(4, status);
}

void DataWidget::onAddRoi(int id, QString label, int size, bool enable)
{
	#ifdef DEBUG
	Logger->debug("DataWidget::onAddRoi()");
	#endif
	QTreeWidgetItem * item = new QTreeWidgetItem();
	item->setText(0, QString::number(id));
	item->setText(1, label);
	item->setText(2, QString::number(size));
	item->setCheckState(3, Qt::Unchecked);
	if(enable)
		item->setCheckState(3, Qt::Checked);
	item->setText(3, "checked");
	roiWidget->addTopLevelItem(item);

	#ifdef DEBUG
	Logger->debug("DataWidget::onAddRoi() done");
	#endif
}

void DataWidget::onUpdateStatus(int id, QString status)
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	item->setText(4, status);
}

void DataWidget::onUpdateFileFromId(int id)
{
	#ifdef DEBUG
	Logger->debug("DataWidget::onUpdateFileFromId(id:{})", id);
	#endif
	m_dataMemory->countGtElements(id);
	QString status{"Update"};
	QTreeWidgetItem * item = treeWidget->currentItem();
	item->setText(4, status);
	int roiElements{0};
	QString _filenameToRoi = m_dataMemory->m_roiPaths[id] + m_dataMemory->m_roiNames[id];
	if (!QFile::exists(_filenameToRoi))
	{
		status+="(ROI not loaded)";
		item->setBackground(0, QColor(125, 0, 0, 100)); 
	}
	else
	{
		status+="(ROI ok)";
		item->setBackground(0, QColor(0, 125, 0, 100)); 
		std::unique_ptr<ConfigReader> cR = std::make_unique<ConfigReader>();

		QJsonObject jObject;
		if (!cR->readConfig(_filenameToRoi, jObject))
		{
			spdlog::error("File {} read config failed", _filenameToRoi.toStdString());
		}
		else
		{
			QJsonArray jROI = jObject[ROI].toArray();
			roiElements = jROI.size();
		}
	}
	onUpdateFile(id, m_dataMemory->filenames(id), m_dataMemory->gtElements(id), roiElements, status);
}

void DataWidget::clearTreeWidget()
{
	treeWidget->clear();
}

void DataWidget::clearRoiWidget()
{
	roiWidget->clear();
}

void DataWidget::setupTreeWidget()
{
	treeWidget = new TreeWidget();
	treeWidget->setRootIsDecorated(true);
	treeWidget->setAlternatingRowColors(true);
	treeWidget->setSortingEnabled(true);
	treeWidget->setAnimated(true);
	const QStringList data{"Id","Name","Elements","roi","Status"};
	treeWidget->setHeaderLabels(data);
	treeWidget->sortItems(1, Qt::AscendingOrder);

	connect(treeWidget, &TreeWidget::itemClicked, this, &DataWidget::open);
}

void DataWidget::setupRoiWidget()
{
	roiWidget = new RoiWidget();
	roiWidget->setRootIsDecorated(true);
	roiWidget->setAlternatingRowColors(true);
	roiWidget->setSortingEnabled(true);
	roiWidget->setAnimated(true);
	const QStringList data{"Id","Name","Size","Enabled"};
	roiWidget->setHeaderLabels(data);
	roiWidget->sortItems(0, Qt::AscendingOrder);
	roiWidget->setMaximumHeight(200);
}

void DataWidget::open()
{
	DataWidget::clearRoiWidget();
	int id = treeWidget->currentId();
	emit(loadImage(id));
	emit(loadRois(id));
	emit(loadPaints(id));
	onUpdateFileFromId(id);
}

void DataWidget::onLoadDirectory(QString folderName)
{
    #ifdef DEBUG
	Logger->debug("DataWidget::importDirImages() folderName:{}", folderName.toStdString());
	#endif
	DataWidget::clearTreeWidget();
	DataWidget::clearRoiWidget();
	
	QJsonObject paths{{"PathToDataset",folderName},{"ConfigName","config.json"}};
    QJsonObject _config{{"DatasetWin32",paths}, {"DatasetLinux",paths}};
    bool ret = m_dataMemory->configure(_config);

    for(int i = 0 ; i < m_dataMemory->getSizeClean() ; i++)
    {
		QTreeWidgetItem * item = new QTreeWidgetItem();
		QString status{"Load Image ok"};
		int roiElements{0};
		QString _filenameToRoi = m_dataMemory->m_roiPaths[i] + m_dataMemory->m_roiNames[i];
		if (!QFile::exists(_filenameToRoi))
		{
			status+="(ROI not loaded)";
			item->setBackground(0, QColor(125, 0, 0, 100)); 
		}
		else
		{
			status+="(ROI ok)";
			item->setBackground(0, QColor(0, 125, 0, 100)); 
			std::unique_ptr<ConfigReader> cR = std::make_unique<ConfigReader>();

			QJsonObject jObject;
			if (!cR->readConfig(_filenameToRoi, jObject))
			{
				spdlog::error("File {} read config failed", _filenameToRoi.toStdString());
			}
			else
			{
				QJsonArray jROI = jObject[ROI].toArray();
				roiElements = jROI.size();
			}
		}
		
		item->setText(0, QString::number(i));
		item->setSizeHint(0,QSize(4,4));
		item->setText(1, m_dataMemory->filenames(i));
		item->setText(2, QString::number(m_dataMemory->gtElements(i)));
		item->setSizeHint(2,QSize(10,10));
		item->setText(3, QString::number(roiElements));
		item->setSizeHint(3,QSize(10,10));
		item->setText(4, status);

		treeWidget->addTopLevelItem(item);
    }
}

void DataWidget::onNextFile()
{
	treeWidget->onNextFile();
	DataWidget::open();
}

void DataWidget::onPrevFile()
{
	treeWidget->onPrevFile();
	DataWidget::open();
}

void DataWidget::onCopyFromNextFile()
{
	treeWidget->onCopyFromNextFile();
}

void DataWidget::onCopyFromPrevFile()
{
	treeWidget->onCopyFromPrevFile();
}
