#include "../include/view.h"

View::View(QJsonObject const& a_config, QFrame* parent)
	: QFrame(parent)
{
    Logger->trace("View::View()");


	View::configure(a_config);

	m_graphicsScene = new GraphicsScene;
	m_graphicsView = new GraphicsView;


	m_graphicsView->setRenderHint(QPainter::Antialiasing, false);
	m_graphicsView->setDragMode(QGraphicsView::NoDrag);
	m_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	m_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	m_graphicsView->setScene(m_graphicsScene);


	// Zoom slider
	m_zoomSlider = new QSlider;
	m_zoomSlider->setMinimum(0);
	m_zoomSlider->setMaximum(500);
	m_zoomSlider->setValue(150);
	m_zoomSlider->setTickPosition(QSlider::TicksRight);

	// Zoom slider layout
	m_zoomSliderLayout = new QVBoxLayout;
	m_zoomSliderLayout->addWidget(m_zoomSlider);

	m_opacitySlider = new QSlider;
	m_opacitySlider->setMinimum(0);
	m_opacitySlider->setMaximum(100);
	m_opacitySlider->setValue(30);
	m_opacitySlider->setTickPosition(QSlider::TicksLeft);

	// Zoom slider layout
	m_opacityLayout = new QVBoxLayout;
	m_opacityLayout->addWidget(m_opacitySlider);

	m_topLayout = new QGridLayout;
	m_topLayout->addLayout(m_opacityLayout, 1, 0);
	m_topLayout->addWidget(m_graphicsView, 1, 1);
	m_topLayout->addLayout(m_zoomSliderLayout, 1, 2);

	Logger->trace("View::View() setLayout");
	setLayout(m_topLayout);

	Logger->trace("View::View() setupMatrix");
	setupMatrix();


	connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
	connect(m_opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity()));

}

void View::configure(QJsonObject const& a_config)
{}

QGraphicsView* View::view() const
{
	return static_cast<QGraphicsView*>(m_graphicsView);
}

void View::setupMatrix()
{
	Logger->trace("View::setupMatrix()");
	m_scale = qPow(qreal(2), (m_zoomSlider->value() - 250) / qreal(50));
	Logger->trace("m_scale:{}", m_scale);
	QMatrix matrix;
	matrix.scale(m_scale, m_scale);

	Logger->trace("View::setupMatrix() setMatrix");
	m_graphicsView->setMatrix(matrix);
}

void View::setOpacity()
{
	Logger->trace("View::setOpacity()");
	m_scaleOpacity = (m_opacitySlider->value()) / qreal(100);
	m_whitePixmap->setOpacity(m_scaleOpacity);
}