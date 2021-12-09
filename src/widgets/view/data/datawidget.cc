#include "widgets/view/data/datawidget.h"

#include "includespdlog.h"
#include "scanfiles.h"

#include <QDebug>
#include <QWidget>
#include <QFile>

//#define DEBUG

class QStandardItemModel;
class QStandardItem;
class QJsonObject;

DataWidget::DataWidget(DataMemory* dataMemory)
: m_dataMemory(dataMemory)
{
	#ifdef DEBUG
	Logger->debug("DataWidget::DataWidget()");
	#endif
    setupModelList();
    setupModel();

	m_labelList = new QTreeView();
	m_labelList->setRootIsDecorated(true);
	m_labelList->setAlternatingRowColors(true);
	m_labelList->setSortingEnabled(true);
	m_labelList->setAnimated(true);
	m_labelList->setSelectionMode(QAbstractItemView::SingleSelection);

    m_rightLayout = new QVBoxLayout;
	m_rightLayout->setContentsMargins(0, 0, 0, 0);
	m_rightLayout->addWidget(m_labelList);
    setupTable();
    m_rightLayout->addWidget(treeWidget);
    
	m_rightLayoutContainer = new QWidget;
	m_rightLayoutContainer->setLayout(m_rightLayout);
	m_gridLayout = new QGridLayout(this);
	m_gridLayout->addWidget(m_rightLayoutContainer, 0, 1);
	this->setLayout(m_gridLayout); 
}

void DataWidget::setupModelList()
{
	modelList = new QStandardItemModel(0, 4, this);
	modelList->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
	modelList->setHeaderData(1, Qt::Horizontal, QObject::tr("Label"));
	modelList->setHeaderData(2, Qt::Horizontal, QObject::tr("Size"));
	modelList->setHeaderData(3, Qt::Horizontal, QObject::tr("enabled"));

	QStandardItem* poListItem = new QStandardItem;
	poListItem->setCheckable(true);
	poListItem->setCheckState(Qt::Unchecked);
}

void DataWidget::setupModel()
{
	model = new QStandardItemModel(0, 4, this);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
	model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
	model->setHeaderData(2, Qt::Horizontal, QObject::tr("Elements"));
	model->setHeaderData(3, Qt::Horizontal, QObject::tr("Status"));
}

void DataWidget::onUpdateFile(int id, QString name, int elements, QString status)
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	item->setText(0, QString::number(id));
	item->setText(1, m_dataMemory->filenames(id));
	item->setText(2, QString::number(m_dataMemory->gtElements(id)));
	item->setText(3, status);
}

void DataWidget::onUpdateStatus(int id, QString status)
{
	QTreeWidgetItem * item = treeWidget->currentItem();
	item->setText(3, status);
}

void DataWidget::onUpdateFileFromId(int id)
{
	#ifdef DEBUG
	Logger->debug("DataWidget::onUpdateFileFromId(id:{})", id);
	#endif
	m_dataMemory->countGtElements(id);
	QString status{"Update"};
	QTreeWidgetItem * item = treeWidget->currentItem();
	item->setText(3, status);

	if (!QFile::exists(m_dataMemory->m_roiPaths[id] + m_dataMemory->m_roiNames[id]))
	{
		status+="(ROI not loaded)";
		item->setBackground(0, QColor(125, 0, 0, 100)); 
	}
	else
	{
		status+="(ROI ok)";
		item->setBackground(0, QColor(0, 125, 0, 100)); 
	}
	onUpdateFile(id, m_dataMemory->filenames(id), m_dataMemory->gtElements(id), status);
}

void DataWidget::clearModel()
{
	treeWidget->clear();
}

void DataWidget::clearModelList()
{
	qint32 count = modelList->rowCount();
	modelList->removeRows(0, count);
}

void DataWidget::setupTable()
{
	treeWidget = new TreeWidget();
	treeWidget->setRootIsDecorated(true);
	treeWidget->setAlternatingRowColors(true);
	treeWidget->setSortingEnabled(true);
	treeWidget->setAnimated(true);
	const QStringList data{"Id","Name","Elements","Status"};
	treeWidget->setHeaderLabels(data);
	treeWidget->sortItems(1, Qt::AscendingOrder);

	connect(treeWidget, &TreeWidget::itemClicked, this, &DataWidget::open);
}

void DataWidget::open()
{
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
	clearModel();
	
	QJsonObject paths{{"PathToDataset",folderName},{"ConfigName","config.json"}};
    QJsonObject _config{{"DatasetWin32",paths}, {"DatasetLinux",paths}};
    bool ret = m_dataMemory->configure(_config);

    for(int i = 0 ; i < m_dataMemory->getSizeClean() ; i++)
    {
		QTreeWidgetItem * item = new QTreeWidgetItem();
		QString status{"Load Image ok"};
		if (!QFile::exists(m_dataMemory->m_roiPaths[i] + m_dataMemory->m_roiNames[i]))
		{
			status+="(ROI not loaded)";
			item->setBackground(0, QColor(125, 0, 0, 100)); 
		}
		else
		{
			status+="(ROI ok)";
			item->setBackground(0, QColor(0, 125, 0, 100)); 
		}
		
		item->setText(0, QString::number(i));
		item->setText(1, m_dataMemory->filenames(i));
		item->setText(2, QString::number(m_dataMemory->gtElements(i)));
		item->setText(3, status);

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
