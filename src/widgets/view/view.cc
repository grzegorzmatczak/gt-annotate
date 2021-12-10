#include "widgets/view/view.h"

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto X{ "x" };
constexpr auto Y{ "y" };

//#define DEBUG


View::View(QJsonObject const& a_config, DataMemory* dataMemory, QFrame* parent)
	: QFrame(parent)
	, m_graphicsScene(new GraphicsScene())
	, m_graphicsView(new GraphicsView())
	, m_config(a_config)
	, m_painter(new Painter(a_config, m_graphicsScene, m_graphicsView, dataMemory))
	, m_dataMemory(dataMemory)
{
	#ifdef DEBUG
	Logger->debug("View::View()");
	#endif

	m_graphicsView->setScale(&m_scale);
	View::setupCentralWidget();
	View::onSetupMatrix();

	m_painter->onChangePenSize(3);
	connect(this, &View::loadImage, m_painter, &Painter::onLoadImage);
	connect(this, &View::loadPaints, m_painter, &Painter::onLoadPaints);
	connect(this, &View::loadRois, m_painter, &Painter::onLoadRois);
	connect(m_graphicsScene, &GraphicsScene::addRectToScene, m_painter, &Painter::onAddRectToScene);

	View::onSetPaint();
	View::onLoadDirectory();
}

void View::setupDataWidget()
{
	m_dataWidget = new DataWidget(m_dataMemory);
	m_dataWidget->setMaximumWidth(600);
	connect(this, &View::loadDirectory, m_dataWidget, &DataWidget::onLoadDirectory);
	connect( m_dataWidget, &DataWidget::loadImage, this, &View::onLoadImage);
	connect( m_dataWidget, &DataWidget::loadPaints, this, &View::onLoadPaints);
	connect( m_dataWidget, &DataWidget::loadRois, this, &View::onLoadRois);//void onLoadRois(int id);
	connect( m_dataWidget->treeWidget, &TreeWidget::loadPaint, this, &View::onLoadPaints);
}

void View::setupCentralWidget()
{
	View::setupDataWidget();
	View::setupGraphicsView();
	View::createMenus();
	View::setupLeftToolBar();
	View::setupBottomToolBar();
	View::setupSliders();
	View::setupProgressBar();

	m_vLayout = new QGridLayout;
	m_vLayout->addWidget(m_leftToolBar, 0, 0);
	m_vLayout->addWidget(m_zoomSlider, 0, 1);
	m_vLayout->addWidget(m_graphicsView, 0, 2);
	m_vLayout->addWidget(m_opacitySlider, 0, 3);
	m_vLayout->addWidget(m_opacitySliderImage, 0, 4);
	m_vLayout->addWidget(m_dataWidget, 0, 5);

	m_vLayout->addWidget(m_bottomToolBar, 1, 2);
	m_vLayout->addWidget(m_statusBar, 1, 0);
	m_progressBar->setMaximumWidth(600);
	m_vLayout->addWidget(m_progressBar, 1, 5);


	m_hLayout = new QGridLayout;
	m_hLayout->addLayout(m_vLayout, 0, 0);
	m_hLayout->addLayout(m_vLayoutBars, 1, 0);
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
	m_statusBar->setSizeGripEnabled(false);
	m_statusBar->setMaximumWidth(100);

	m_vLayoutBars = new QGridLayout;
}

void View::setupSliders()
{
	#ifdef DEBUG
	Logger->debug("View::setupSliders()");
	#endif
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
	connect(m_opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity()));
	connect(m_opacitySliderROI, SIGNAL(valueChanged(int)), this, SLOT(setOpacityROI()));
	connect(m_opacitySliderImage, SIGNAL(valueChanged(int)), this, SLOT(setOpacityImage()));
}

void View::setupGraphicsView()
{
	#ifdef DEBUG
	Logger->debug("View::setupGraphicsView()");
	#endif
	m_graphicsView->setRenderHint(QPainter::Antialiasing, false);
	m_graphicsView->setDragMode(QGraphicsView::NoDrag);
	m_graphicsView->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	m_graphicsView->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	m_graphicsView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	m_graphicsView->setScene(m_graphicsScene);

	connect(m_graphicsScene, &GraphicsScene::paintWhiteBoard, m_painter, &Painter::onPaintOnBoard);
	connect(m_graphicsScene, &GraphicsScene::paintWhiteBoardBackground, m_painter, &Painter::onPaintBackground);
	connect(m_graphicsView, &GraphicsView::zoomIn, this, &View::onZoomIn);
	connect(m_graphicsView, &GraphicsView::zoomOut, this, &View::onZoomOut);
}

void View::creteAction()
{
	#ifdef DEBUG
	Logger->debug("View::creteAction()");
	#endif

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

	action_loadDirectory = new QAction(tr("&Load Directory"), this);
	action_loadDirectory->setStatusTip(tr("Load Directory"));
	action_loadDirectory->setToolTip("Load Directory");
	connect(action_loadDirectory, &QAction::triggered, this, &View::onLoadDirectory);
	
	action_create_roi = new QAction(tr("&Create ROI"), this);
	action_create_roi->setToolTip("create all roi on current images");
	connect(action_create_roi, &QAction::triggered, m_painter, &Painter::onCreateRois);

	action_saveWhitePixmap = new QAction(tr("&Save objects"), this);
	action_saveWhitePixmap->setToolTip("Save all roi on current images");
	connect(action_saveWhitePixmap, &QAction::triggered, m_painter, &Painter::onSavePaint);
	connect(action_saveWhitePixmap, &QAction::triggered, m_painter, &Painter::onSaveRois);
	connect(action_saveWhitePixmap, &QAction::triggered, m_dataWidget, &DataWidget::open);
	connect(m_painter, &Painter::updateFileFromId, m_dataWidget, &DataWidget::onUpdateFileFromId );
	connect(m_painter, &Painter::updatStatus, m_dataWidget, &DataWidget::onUpdateStatus );
	connect(m_painter, &Painter::addRoi, m_dataWidget, &DataWidget::onAddRoi );
	connect(m_painter, &Painter::clearRoiWidget, m_dataWidget, &DataWidget::clearRoiWidget );

	connect(m_dataWidget->roiWidget, &RoiWidget::itemChanged, m_painter, &Painter::onRoiItemChanged);
	//connect(m_dataWidget->roiWidget, &RoiWidget::currentItemChanged, m_painter, &Painter::onRoiItemChanged);
	connect(m_dataWidget->roiWidget, &RoiWidget::itemSelectionChanged, m_painter, &Painter::onRoiItemSelectionChanged);
	
	
	View::onSetPaint();
}

void View::setupBottomToolBar() 
{
	View::creteBottomAction();
	m_bottomToolBar = new ToolBar();
	m_bottomToolBar->setWindowFlag(Qt::FramelessWindowHint);
	m_bottomToolBar->setOrientation(Qt::Horizontal);
	m_bottomToolBar->addAction(action_prevImage);
	m_bottomToolBar->addAction(action_nextImage);
	m_bottomToolBar->addAction(action_copyPrevImage);
	m_bottomToolBar->addAction(action_copyNextImage);
}

void View::creteBottomAction()
{
	action_nextImage = new QAction(tr("&Next Image"), this);
	action_nextImage->setToolTip("Next Image");
	connect(action_nextImage, &QAction::triggered, m_dataWidget, &DataWidget::onNextFile);

	action_prevImage = new QAction(tr("&Prev Image"), this);
	action_prevImage->setToolTip("Prev Image");
	connect(action_prevImage, &QAction::triggered, m_dataWidget, &DataWidget::onPrevFile);

	action_copyNextImage = new QAction(tr("&Copy From Next Image"), this);
	action_copyNextImage->setToolTip("Copy From Next Image");
	connect(action_copyNextImage, &QAction::triggered, m_dataWidget, &DataWidget::onCopyFromNextFile);

	action_copyPrevImage = new QAction(tr("&Copy From Prev Image"), this);
	action_copyPrevImage->setToolTip("Copy From Prev Image");
	connect(action_copyPrevImage, &QAction::triggered, m_dataWidget, &DataWidget::onCopyFromPrevFile);
}

void View::setupLeftToolBar() 
{
	#ifdef DEBUG
	Logger->debug("View::setupLeftToolBar()");
	#endif
	View::creteAction();
	m_leftToolBar = new ToolBar();
	m_leftToolBar->setWindowFlag(Qt::FramelessWindowHint);
	m_leftToolBar->setOrientation(Qt::Vertical);
	m_leftToolBar->addWidget(m_menuBar);
	m_leftToolBar->addAction(action_loadDirectory);

	m_colorPicker = new ColorPicker(m_config);
	connect(m_colorPicker, &ColorPicker::changeColor, m_painter, &Painter::onChangeColor);
	m_leftToolBar->addWidget(m_colorPicker);
	m_leftToolBar->addSeparator();
	
	
	#ifdef DEBUG
	qDebug()<< "ColorPickerWidget m_config:" << m_config;
	#endif
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
	m_leftToolBar->addSeparator();
	m_leftToolBar->addAction(action_create_roi);
	m_leftToolBar->addAction(action_saveWhitePixmap);
	m_leftToolBar->addSeparator();
}

void View::setOpacity()
{
	#ifdef DEBUG
	Logger->debug("View::setOpacity()");
	#endif
	m_scaleOpacity = (m_opacitySlider->value()) / qreal(100);
	m_painter->setOpacity(m_scaleOpacity);
}

void View::setOpacityROI()
{
	#ifdef DEBUG
	Logger->debug("View::setOpacityROI()");
	#endif
	m_scaleOpacityROI = (m_opacitySliderROI->value()) / qreal(100);
	m_painter->setOpacityROI(m_scaleOpacityROI);
}

void View::setOpacityImage()
{
	#ifdef DEBUG
	Logger->debug("View::setOpacityImage()");
	#endif
	m_scaleOpacityImage = (m_opacitySliderImage->value()) / qreal(100);
	m_painter->setOpacityImage(m_scaleOpacityImage);
}

void View::onSetupMatrix()
{
	#ifdef DEBUG
	Logger->debug("View::onSetupMatrix()");
	#endif
	m_scale = qPow(qreal(2), (m_zoomSlider->value() - 250) / qreal(50));
	#ifdef DEBUG
	Logger->debug("View::onSetupMatrix() m_scale:{}", m_scale);
	#endif
	QMatrix matrix;
	matrix.scale(m_scale, m_scale);
	m_graphicsView->setMatrix(matrix);
}

void View::onChangeColor2(QListWidgetItem* item) 
{
	#ifdef DEBUG
	Logger->debug("View::onChangeColor2()");
	#endif
	ColorPickerLabel * label = static_cast<ColorPickerLabel*>(m_colorPickerWidget->itemWidget(item));
	#ifdef DEBUG
	Logger->debug("View::onChangeColor2() label.m_color:{}",label->m_color.name().toStdString());
	#endif
	QString oldColor = label->m_name;
	QColor color = QColorDialog::getColor(label->m_color, this);

	if (color.isValid())
	{
		
		if(m_painter->onChangeOldColor(oldColor, color))
		{
			#ifdef DEBUG
			Logger->debug("View::onChangeOldColor true");
			#endif
			label->setNewColor(color);
		}
	}
}

void View::onChangeLabel(QListWidgetItem* current, QListWidgetItem* previous)
{
	#ifdef DEBUG
	Logger->debug("View::onChangeLabel()");
	#endif
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
	if (current == NULL) {current = previous;}
	label = static_cast<ColorPickerLabel*>(m_colorPickerWidget->itemWidget(current));
	label->setSelected(true);
	
	#ifdef DEBUG
	qDebug() << "View::onChangeLabel() change color to :" << label->m_color;
	#endif
	m_painter->onChangeColor(label->m_color);
	QString str;
	QString key = label->m_name;
	QTextStream sstr(&str);
	
	sstr <<"label:" <<  label->m_name << " category: color:[" << label->m_color.name() << "]" ;
	m_statusBar->showMessage(str);
}

void View::onLoadDirectory()
{
	QString targetDirectoryPath = QFileDialog::getExistingDirectory(
		this, tr("Open Dir"), QDir::currentPath(),
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if (targetDirectoryPath == "")
	{
		return;
	}
	targetDirectoryPath += "/";
	emit(loadDirectory(targetDirectoryPath)); // signal to datawidget
}

void View::onLoadImage(int id)
{
	#ifdef DEBUG
	Logger->debug("View::onLoadImage(id:{})", id);
	#endif
	emit(loadImage(id));
}

void View::onLoadPaints(int id)
{
	#ifdef DEBUG
	Logger->debug("View::onLoadPaints(id:{})", id);
	#endif
	emit(loadPaints(id));
}

void View::onLoadRois(int id)
{
	#ifdef DEBUG
	Logger->debug("View::onLoadRois(id:{})", id);
	#endif
	emit(loadRois(id));
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
	#ifdef DEBUG
	Logger->debug("View::setPaint()");
	#endif
	action_paint->setChecked(true);
	action_move->setChecked(false);
	m_graphicsView->setDragMode(QGraphicsView::NoDrag);
	m_graphicsScene->setMode(uiMode::Paint);
}

void View::onSetMove()
{
	#ifdef DEBUG
	Logger->debug("View::setMove()");
	#endif
	action_paint->setChecked(false);
	action_move->setChecked(true);
	m_graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
	m_graphicsScene->setMode(uiMode::MoveSelect);
}
