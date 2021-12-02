#include "widgets/view/data/datawidget.h"

#include "includespdlog.h"
#include "scanfiles.h"

#include <QDebug>
#include <QWidget>
class QStandardItemModel;
class QStandardItem;
class QJsonObject;

DataWidget::DataWidget(DataMemory* dataMemory)
: m_dataMemory(dataMemory)
{
    setupModelList();
    setupModel();

    Logger->trace("DataWidget::DataWidget()");
	m_labelList = new QTreeView();
    //m_labelList->setModel(modelList);
	m_labelList->setRootIsDecorated(true);
	m_labelList->setAlternatingRowColors(true);
	m_labelList->setSortingEnabled(true);
	m_labelList->setAnimated(true);
	m_labelList->setSelectionMode(QAbstractItemView::SingleSelection);

    m_rightLayout = new QVBoxLayout;
	m_rightLayout->setContentsMargins(0, 0, 0, 0);
	m_rightLayout->addWidget(m_labelList);
    setupTable();
    m_rightLayout->addWidget(proxyView);
    //m_rightLayout->addWidget(treeWidget);
    
	m_rightLayoutContainer = new QWidget;
	m_rightLayoutContainer->setLayout(m_rightLayout);
	m_gridLayout = new QGridLayout(this);
	m_gridLayout->addWidget(m_rightLayoutContainer, 0, 1);
	this->setLayout(m_gridLayout);
    //m_dataInfo = new DataInfo();
    
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

	//connect(modelList, &QStandardItemModel::itemChanged, this,
	//	&MainWindow::onItemChanged);
	//connect(modelList, &QStandardItemModel::itemChanged, view,
	//	&View::onItemChanged);
}

void DataWidget::setupModel()
{
	model = new QStandardItemModel(0, 3, this);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Id"));
	model->setHeaderData(1, Qt::Horizontal, QObject::tr("Name"));
	model->setHeaderData(2, Qt::Horizontal, QObject::tr("Elements"));
	//model->setHeaderData(2, Qt::Horizontal, QObject::tr("ROI size"));
	//model->setHeaderData(3, Qt::Horizontal, QObject::tr("ROI number"));
	//model->setHeaderData(4, Qt::Horizontal, QObject::tr("Status"));
}

void DataWidget::onUpdateFile(int id, QString name, int elements)
{
	QList<QStandardItem*> items = model->findItems(QString::number(id), Qt::MatchRecursive);
	Logger->trace("DataWidget::onUpdateFile() items.size():{} id:{}", items.size(), id);
	for (auto i = 0; i < items.size(); i++)
    {
		items[i]->clearData();
		qint32 row = items[i]->row();
		model->setData(model->index(row, 0), id);
		model->setData(model->index(row, 1), name);
		model->setData(model->index(row, 2), elements);
	}
	
}

void DataWidget::onUpdateFileFromId(int id)
{
	Logger->trace("DataWidget::onUpdateFileFromId(id:{})", id);
	m_dataMemory->countGtElements(id);
	onUpdateFile(id, m_dataMemory->filenames(id), m_dataMemory->gtElements(id));
	//emit(loadImage(m_dataMemory->paths(id), m_dataMemory->filenames(id)));
    //emit(loadPaints(id));
}

void DataWidget::clearModel()
{
	Logger->trace("DataWidget::clearModel()");
	qint32 count = model->rowCount();
	model->removeRows(0, count);
}

void DataWidget::clearModelList()
{
	qint32 count = modelList->rowCount();
	modelList->removeRows(0, count);
}

void DataWidget::setupTable()
{
    // TODO treeWidget needed?
    //treeWidget = new TreeWidget();

	proxyView = new QTreeView;
	proxyView->setRootIsDecorated(true);
	proxyView->setAlternatingRowColors(true);
	proxyView->setModel(model);
	proxyView->setSortingEnabled(true);
	proxyView->setAnimated(true);
	connect(proxyView, &QTreeView::clicked, this, &DataWidget::open);
}

void DataWidget::open()
{
	Logger->trace("DataWidget::open()");
    QModelIndex index = proxyView->currentIndex();
	const QModelIndex header = index.sibling(index.row(), 0);
    QVariant id = model->data(header);
    int idInt = id.toInt();
    emit(loadImage(m_dataMemory->paths(idInt), m_dataMemory->filenames(idInt)));
    emit(loadPaints(idInt));
	onUpdateFile(idInt, m_dataMemory->filenames(idInt), m_dataMemory->gtElements(idInt));
}

void DataWidget::onLoadDirectory(QString folderName)
{
	Logger->trace("DataWidget::importDirImages() folderName:{}", folderName.toStdString());
    
	clearModel();
	
	QJsonObject paths{{"PathToDataset",folderName},{"ConfigName","config.json"}};
    QJsonObject _config{{"DatasetWin32",paths}, {"DatasetLinux",paths}};
    bool ret = m_dataMemory->configure(_config);

    emit(loadImage(m_dataMemory->paths(0), m_dataMemory->filenames(0)));

    for(int i = 0 ; i < m_dataMemory->getSizeClean() ; i++)
    {
        Logger->trace("DataWidget::importDirImages() insertRow:{}", i);
        model->insertRow(0);
        model->setData(model->index(0, 0), i);
        model->setData(model->index(0, 1), m_dataMemory->filenames(i));
        model->setData(model->index(0, 2), QString::number(m_dataMemory->gtElements(i)));
    }
	
}
