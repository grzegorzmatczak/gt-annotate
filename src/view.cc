#include "../include/view.h"

constexpr auto NAME{ "Name" };
constexpr auto COLORS{ "Colors" };
constexpr auto BLACK{ "Black" };
constexpr auto WHITE{ "White" };
constexpr auto SHADOW{ "Shadow" };
constexpr auto OUT_OF_SCOPE{ "OutOfScope" };
constexpr auto UNKNOWN{ "Unknown" };

constexpr auto ROI{ "ROI" };
constexpr auto GRAY_COLOR{ "GrayColor" };
constexpr auto R{ "R" };
constexpr auto G{ "G" };
constexpr auto B{ "B" };
constexpr auto A{ "A" };


View::View(QJsonObject const& a_config, QFrame* parent)
	: QFrame(parent)
	, m_penSize(3)
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

	connect(this, &View::setModePaint, m_graphicsScene, &GraphicsScene::onSetModePaint);
	connect(this, &View::setModeMove, m_graphicsScene, &GraphicsScene::onSetModeMoveSelect);
	connect(this, &View::setModeROI, m_graphicsScene, &GraphicsScene::onSetModeROI);
	connect(this, &View::resetScene, m_graphicsScene, &GraphicsScene::onResetScene);

	// To paint:
	connect(m_graphicsScene, &GraphicsScene::paintWhiteBoard, this, &View::onPaintWhiteBoard);
	connect(m_graphicsView, &GraphicsView::zoomIn, this, &View::onZoomIn);
	connect(m_graphicsView, &GraphicsView::zoomOut, this, &View::onZoomOut);
	setupMatrix();
}

void View::configure(QJsonObject const& a_config)
{
	auto _colors = a_config[COLORS].toArray();
	for (int i = 0; i < _colors.size(); i++)
	{
		auto _colorsIter = _colors[i].toObject();
		QString _name = _colorsIter[NAME].toString();
		qint32 _r = _colorsIter[R].toInt();
		qint32 _g = _colorsIter[G].toInt();
		qint32 _b = _colorsIter[B].toInt();
		qint32 _a = _colorsIter[A].toInt();
		qint32 _gray = _colorsIter[GRAY_COLOR].toInt();
		colors _colors{ _name, { _r, _g, _b, _a }, _gray };
		m_colors.push_back(_colors);
		if (_name == WHITE)
		{
			m_color = { _r, _g, _b, _a };
		}
		if (_name == BLACK) {
			m_intBlack = _gray;
			m_colorBlack = { _r, _g, _b, _a };
		}
		if (_name == WHITE) {
			m_intWhite = _gray;
			m_colorWhite = { _r, _g, _b, _a };
		}
		if (_name == SHADOW) {
			m_intShadow = _gray;
			m_colorShadow = { _r, _g, _b, _a };
		}
		if (_name == OUT_OF_SCOPE) {
			m_intOutOfScope = _gray;
			m_colorOutOfScope = { _r, _g, _b, _a };
		}
		if (_name == UNKNOWN) {
			m_intUnknown = _gray;
			m_colorUnknown = { _r, _g, _b, _a };
		}
	}
}

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

void View::onPaintWhiteBoard(qint32 x, qint32 y)
{
	if (m_penSize == 1)
	{
		m_diff.setPixelColor(x, y, m_color);
	}
	else if (m_penSize > 1 && m_penSize < 20)
	{
		qint32 it = m_penSize - 1;
		for (int zx = -it; zx <= it; zx++)
		{
			for (int zy = -it; zy <= it; zy++)
			{
				m_diff.setPixelColor(x + zx, y + zy, m_color);
			}
		}
	}

	//QPixmap whiteBoardPixmap = QPixmap::fromImage(m_diff);
	m_whitePixmap->setPixmap(QPixmap::fromImage(m_diff));
}

void View::onZoomIn(qint32 delta)
{
	m_zoomSlider->setValue(m_zoomSlider->value() + delta);
}

void View::onZoomOut(qint32 delta)
{
	m_zoomSlider->setValue(m_zoomSlider->value() - delta);
}