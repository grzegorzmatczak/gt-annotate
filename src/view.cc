#include "../include/view.h"

View::View(QJsonObject const& a_config, QFrame* parent)
	: QFrame(parent)
{
    Logger->trace("View::View()");


	View::configure(a_config);

	
}

void View::configure(QJsonObject const& a_config)
{}

QGraphicsView* View::view() const
{
	//return static_cast<QGraphicsView*>(m_graphicsView);
}