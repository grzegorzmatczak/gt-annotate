#include "widgets/view/view.h"

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto X{ "x" };
constexpr auto Y{ "y" };


View::View(QJsonObject const& a_config, DataMemory* dataMemory, QFrame* parent)
	: QFrame(parent)
	, m_graphicsScene(new GraphicsScene())
	, m_graphicsView(new GraphicsView())
	, m_config(a_config)
	, m_painter(new Painter(a_config, m_graphicsScene, m_graphicsView, dataMemory))
	, m_dataMemory(dataMemory)
{
	Logger->trace("View::View()");

	
	m_graphicsView->setScale(&m_scale);
	View::setupCentralWidget();

	Logger->trace("View::View() m_painter->setupMatrix");
	View::onSetupMatrix();

	m_painter->onChangePenSize(3);
	connect(this, &View::loadImage, m_painter, &Painter::onLoadImage);
	connect(this, &View::loadPaints, m_painter, &Painter::onLoadPaints);

	View::onSetPaint();
	View::onLoadDirectory();
}

void View::setupDataWidget()
{
	m_dataWidget = new DataWidget(m_dataMemory);
	connect(this, &View::loadDirectory, m_dataWidget, &DataWidget::onLoadDirectory);
	connect( m_dataWidget, &DataWidget::loadImage, this, &View::onLoadImage);
	connect( m_dataWidget, &DataWidget::loadPaints, this, &View::onLoadPaints);
}

void View::setupCentralWidget()
{
	View::setupDataWidget();
	View::setupGraphicsView();
	View::createMenus();
	View::setupLeftToolBar();
	View::setupSliders();
	View::setupProgressBar();

	Logger->trace("View::setupCentralWidget()");
	m_vLayout = new QGridLayout;
	m_vLayout->addWidget(m_leftToolBar, 0, 0);
	m_vLayout->addWidget(m_zoomSlider, 0, 1);
	m_vLayout->addWidget(m_graphicsView, 0, 2);
	m_vLayout->addWidget(m_opacitySlider, 0, 3);
	m_vLayout->addWidget(m_opacitySliderImage, 0, 4);
	m_vLayout->addWidget(m_dataWidget, 0, 5);
	//m_vLayout->addWidget(m_opacitySlider, 0, 3);

	m_hLayout = new QGridLayout;
	m_hLayout->addLayout(m_vLayout, 0, 0);
	m_hLayout->addLayout(m_vLayoutBars, 1, 0);
	Logger->trace("View::View() setLayout");
	setLayout(m_hLayout);
}

void View::createMenus()
{
	m_menuBar = new QMenuBar(this);
	m_fileMenu = m_menuBar->addMenu(tr("&File"));
	m_fileMenu->addSeparator();
	m_fileMenu = m_menuBar->addMenu(tr("&Tool"));
}

void View::setupProgressBar()
{
	m_progressBar = new QProgressBar(this);
	m_progressBar->setMinimum(0);
	m_progressBar->setMaximum(100);

	m_statusBar = new QStatusBar(this);

	m_vLayoutBars = new QGridLayout;
	m_vLayoutBars->addWidget(m_statusBar, 0, 0);
	m_vLayoutBars->addWidget(m_progressBar, 0, 1);
}

void View::setupSliders()
{
	Logger->trace("View::setupSliders()");

	m_zoomSlider = new QSlider;
	m_zoomSlider->setMinimum(0);
	m_zoomSlider->setMaximum(500);
	m_zoomSlider->setValue(150);
	m_zoomSlider->setTickPosition(QSlider::TicksRight);

	m_opacitySlider = new QSlider;
	m_opacitySlider->setMinimum(0);
	m_opacitySlider->setMaximum(100);
	m_opacitySlider->setValue(30);
	m_opacitySlider->setTickPosition(QSlider::TicksLeft);

	m_opacitySliderROI = new QSlider;
	m_opacitySliderROI->setMinimum(0);
	m_opacitySliderROI->setMaximum(100);
	m_opacitySliderROI->setValue(30);
	m_opacitySliderROI->setTickPosition(QSlider::TicksLeft);

	m_opacitySliderImage = new QSlider;
	m_opacitySliderImage->setMinimum(0);
	m_opacitySliderImage->setMaximum(100);
	m_opacitySliderImage->setValue(30);
	m_opacitySliderImage->setTickPosition(QSlider::TicksLeft);


	connect(m_zoomSlider, &QSlider::valueChanged, this, &View::onSetupMatrix );
	// TODO: connect(m_painter, &Painter::setupMatrix, this, &View::onSetupMatrix );

	connect(m_opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity()));
	connect(m_opacitySliderROI, SIGNAL(valueChanged(int)), this, SLOT(setOpacityROI()));
	connect(m_opacitySliderImage, SIGNAL(valueChanged(int)), this, SLOT(setOpacityImage()));
}

void View::setupGraphicsView()
{
	Logger->trace("View::setupGraphicsView()");
	m_graphicsView->setRenderHint(QPainter::Antialiasing, false);
	m_graphicsView->setDragMode(QGraphicsView::NoDrag);
	m_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	m_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	m_graphicsView->setScene(m_graphicsScene);

	connect(m_graphicsScene, &GraphicsScene::paintWhiteBoard, m_painter, &Painter::onPaintOnBoard);
	connect(m_graphicsView, &GraphicsView::zoomIn, this, &View::onZoomIn);
	connect(m_graphicsView, &GraphicsView::zoomOut, this, &View::onZoomOut);
}

void View::creteAction()
{
	Logger->trace("View::creteAction()");

	action_paint = new QAction(tr("&Paint"), this);
	action_paint->setToolTip("Painting");
	action_paint->setCheckable(true);
	action_paint->setEnabled(true);

	action_move = new QAction(tr("&Move"), this);
	action_move->setToolTip("Moving");
	action_paint->setCheckable(true);

	action_ROI = new QAction(tr("&ROI"), this);
	action_ROI->setToolTip("select ROI");	

	connect(action_paint, &QAction::triggered, this, &View::onSetPaint);
	connect(action_move, &QAction::triggered, this, &View::onSetMove);
	connect(action_ROI, &QAction::triggered, this, &View::onSetROI);

	action_loadDirectory = new QAction(tr("&Load Directory"), this);
	action_loadDirectory->setStatusTip(tr("Load Directory"));
	action_loadDirectory->setToolTip("Load Directory");
	connect(action_loadDirectory, &QAction::triggered, this, &View::onLoadDirectory);
	

	action_saveWhitePixmap = new QAction(tr("&Save objects"), this);
	action_saveWhitePixmap->setToolTip("Save all roi on current images");
	connect(action_saveWhitePixmap, &QAction::triggered, m_painter, &Painter::onSavePaint);
	connect(m_painter, &Painter::updateFileFromId, m_dataWidget, &DataWidget::onUpdateFileFromId );
	View::onSetPaint();
}

void View::setupLeftToolBar() 
{
	View::creteAction();
	Logger->trace("View::setupLeftToolBar()");
	m_leftToolBar = new ToolBar();
	m_leftToolBar->setWindowFlag(Qt::FramelessWindowHint);
	m_leftToolBar->setOrientation(Qt::Vertical);
	m_leftToolBar->addWidget(m_menuBar);

	m_colorPicker = new ColorPicker(m_config);
	connect(m_colorPicker, &ColorPicker::changeColor, m_painter, &Painter::onChangeColor);
	m_leftToolBar->addWidget(m_colorPicker);
	m_leftToolBar->addSeparator();
	
	qDebug()<< "ColorPickerWidget m_config:" << m_config;
	m_colorPickerWidget = new ColorPickerWidget(m_config);
	m_leftToolBar->addWidget(m_colorPickerWidget);
	connect(m_colorPickerWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(onChangeColor2(QListWidgetItem*)));
	connect(m_colorPickerWidget, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)), this, SLOT(onChangeLabel(QListWidgetItem*, QListWidgetItem*)));

	m_penSizePicker = new PenSizePicker();
	m_leftToolBar->addWidget(m_penSizePicker);
	connect(m_penSizePicker, &PenSizePicker::changePenSize, m_painter, &Painter::onChangePenSize);

	m_leftToolBar->addSeparator();
	
	m_leftToolBar->addAction(action_paint);
	m_leftToolBar->addAction(action_move);
	m_leftToolBar->addAction(action_ROI);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addAction(action_saveWhitePixmap);
	m_leftToolBar->addSeparator();
	m_leftToolBar->addAction(action_loadDirectory);
}

void View::setOpacity()
{
	Logger->trace("View::setOpacity()");
	//TODO:
	// m_scaleOpacity not needed as class member?
	m_scaleOpacity = (m_opacitySlider->value()) / qreal(100);
	m_painter->setOpacity(m_scaleOpacity);
}

void View::setOpacityROI()
{
	Logger->trace("View::setOpacityROI()");
	m_scaleOpacityROI = (m_opacitySliderROI->value()) / qreal(100);
	m_painter->setOpacityROI(m_scaleOpacityROI);
}

void View::setOpacityImage()
{
	Logger->trace("View::setOpacityImage()");
	m_scaleOpacityImage = (m_opacitySliderImage->value()) / qreal(100);
	m_painter->setOpacityImage(m_scaleOpacityImage);
}

void View::onSetupMatrix()
{
	Logger->trace("View::onSetupMatrix()");
	m_scale = qPow(qreal(2), (m_zoomSlider->value() - 250) / qreal(50));
	Logger->trace("m_scale:{}", m_scale);
	QMatrix matrix;
	matrix.scale(m_scale, m_scale);
	m_graphicsView->setMatrix(matrix);
}

void View::onChangeColor2(QListWidgetItem* item) 
{
	Logger->trace("View::onChangeColor2()");
	ColorPickerLabel * label = static_cast<ColorPickerLabel*>(m_colorPickerWidget->itemWidget(item));
	Logger->trace("View::onChangeColor2() label.m_color:{}",label->m_color.name().toStdString());

	QString oldColor = label->m_name;
	QColor color = QColorDialog::getColor(label->m_color, this);

	if (color.isValid())
	{
		/* TODO:
		if(m_painter->onChangeOldColor(oldColor, color) );
		{
			Logger->trace("onChangeOldColor true");
			label->setNewColor(color);
		}*/
	}
}

void View::onChangeLabel(QListWidgetItem* current, QListWidgetItem* previous)
{
	Logger->trace("View::onChangeLabel()");

	if (current == NULL && previous == NULL)
	{
		Logger->error("View::onChangeLabel() return");
		return;
	}

	ColorPickerLabel * label;
	if (previous == NULL)
	{
		for (int i = 0; i < m_colorPickerWidget->count(); i++)
		{
			label = static_cast<ColorPickerLabel*>(m_colorPickerWidget->itemWidget(m_colorPickerWidget->item(i)));
			label->setSelected(false);
		}
	} 
	else 
	{
		label = static_cast<ColorPickerLabel*>(m_colorPickerWidget->itemWidget(previous));
		label->setSelected(false);
	}
	Logger->trace("View::onChangeLabel() next");
	if (current == NULL) {current = previous;}
	Logger->trace("View::onChangeLabel() label");
	label = static_cast<ColorPickerLabel*>(m_colorPickerWidget->itemWidget(current));
	label->setSelected(true);
	qDebug() << "View::onChangeLabel() change color to :" << label->m_color;
	m_painter->onChangeColor(label->m_color);
	QString str;
	QString key = label->m_name;
	QTextStream sstr(&str);
	
	sstr <<"label:" <<  label->m_name << " category: color:[" << label->m_color.name() << "]" ;
	m_statusBar->showMessage(str);
}

void View::onLoadDirectory()
{
	m_targetDirectoryPath = QFileDialog::getExistingDirectory(
		this, tr("Open Dir"), QDir::currentPath(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (m_targetDirectoryPath == "")
	{
		return;
	}
	m_targetDirectoryPath += "/";
	emit(loadDirectory(m_targetDirectoryPath)); // signal to datawidget


	//m_painter->onLoadImage(m_targetDirectoryPath, "test_images/test1.png");
	//View::renderColorsFromImage("test_images/test1.png");
}

void View::onLoadImage(QString dir, QString name)
{
	emit(loadImage(dir,name));
}

void View::onLoadPaints(int id)
{
	emit(loadPaints(id));
}

QGraphicsView* View::view() const
{
	return static_cast<QGraphicsView*>(m_graphicsView);
}

void View::onZoomIn(qint32 delta)
{
	m_zoomSlider->setValue(m_zoomSlider->value() + delta);
}

void View::onZoomOut(qint32 delta)
{
	m_zoomSlider->setValue(m_zoomSlider->value() - delta);
}

void View::onSetPaint()
{
	Logger->trace("View::setPaint()");
	action_move->setChecked(false);
	action_ROI->setChecked(false);
	m_graphicsView->setDragMode(QGraphicsView::NoDrag);
	m_graphicsScene->setMode(uiMode::Paint);
}

void View::onSetMove()
{
	Logger->trace("View::setMove()");
	action_paint->setChecked(false);
	action_ROI->setChecked(false);
	m_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
	m_graphicsScene->setMode(uiMode::MoveSelect);
}

void View::onSetROI()
{
	Logger->trace("View::setROI()");
	action_move->setChecked(false);
	action_paint->setChecked(false);
	m_graphicsView->setDragMode(QGraphicsView::RubberBandDrag);
	m_graphicsScene->setMode(uiMode::SelectROI);
}
