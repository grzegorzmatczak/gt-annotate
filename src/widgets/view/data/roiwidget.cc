#include "widgets/view/data/roiwidget.h"

#include <QDebug>
#include <QWidget>
#include <QFileDialog>

#include "includespdlog.h"

#define DEBUG


RoiWidget::RoiWidget(const QJsonObject &a_config)
{
	#ifdef DEBUG
	Logger->debug("RoiWidget::RoiWidget()");
	#endif

	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX
}

RoiWidget::RoiWidget()
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

int RoiWidget::currentId() const
{
	#ifdef DEBUG
	Logger->debug("RoiWidget::currentId()");
	#endif

	QList<QTreeWidgetItem *> selectedItem = this->selectedItems();
	if(selectedItem.size()>0)
	{
		return selectedItem[0]->text(0).toInt();
		selectedItem[0]->setBackground(1, QColor(0, 0, 200, 125)); 
	}
	return 0;
	
}

int RoiWidget::idFromItem(QTreeWidgetItem *item) const
{
	return item->text(0).toInt();

}

void RoiWidget::onItemClicked()
{
	#ifdef DEBUG
	Logger->debug("RoiWidget::onItemClicked()");
	#endif
	int id = currentId();
	//emit(loadImage(id));
	//emit(loadRois(id));
	//emit(loadPaint(id));
}
