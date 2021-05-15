#include "widgets/view.h"




View::View(QJsonObject const& a_config, QFrame* parent)
    : QFrame(parent)
    , m_graphicsScene(new GraphicsScene())
    , m_graphicsView(new GraphicsView())
{
    Logger->trace("View::View()");

    m_painterSettings.configureColors(a_config);
    View::setupCentralWidget(a_config);

    Logger->trace("View::View() setupMatrix");
    setupMatrix();
}

void View::setupCentralWidget(QJsonObject const& a_config)
{
    View::setupGraphicsView();
    View::setupLeftToolBar(a_config);
    View::setupSliders();
    View::setupProgressBar();

    Logger->trace("View::setupCentralWidget()");
    m_vLayout = new QGridLayout;
    m_vLayout->addWidget(m_leftToolBar, 0, 0);
    m_vLayout->addWidget(m_zoomSlider, 0, 1);
    m_vLayout->addWidget(m_graphicsView, 0, 2);
    m_vLayout->addWidget(m_opacitySlider, 0, 3);
    //m_topLayout->addWidget(m_progressBar, 0, 4);

    m_hLayout = new QGridLayout;
    m_hLayout->addLayout(m_vLayout, 0, 0);
    m_hLayout->addWidget(m_progressBar, 1, 0);
    Logger->trace("View::View() setLayout");
    setLayout(m_hLayout);
}

void View::setupProgressBar()
{
    m_progressBar = new QProgressBar(this);
    m_progressBar->setMinimum(0);
    m_progressBar->setMaximum(100);
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

    connect(m_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setupMatrix()));
    connect(m_opacitySlider, SIGNAL(valueChanged(int)), this, SLOT(setOpacity()));
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

    connect(m_graphicsScene, &GraphicsScene::paintWhiteBoard, this, &View::onPaintWhiteBoard);
    connect(m_graphicsView, &GraphicsView::zoomIn, this, &View::onZoomIn);
    connect(m_graphicsView, &GraphicsView::zoomOut, this, &View::onZoomOut);
}

void View::creteAction()
{
    Logger->trace("View::creteAction()");
    action_paint = new QAction(tr("&Paint"), this);
	action_move = new QAction(tr("&Move"), this);
	action_ROI = new QAction(tr("&ROI"), this);
    action_paint->setEnabled(true);
    action_paint->setText("Paint");
    action_paint->setToolTip("Rysowanie");

	action_paint->setCheckable(true);
	action_move->setCheckable(true);
	action_ROI->setCheckable(true);

    connect(action_paint, &QAction::triggered, this, &View::onSetPaint);
	connect(action_move, &QAction::triggered, this, &View::onSetMove);
	connect(action_ROI, &QAction::triggered, this, &View::onSetROI);

    action_loadDirectory = new QAction(tr("&Load Directory"), this);
	//action_loadDirectory->setShortcuts(QKeySequence::New);
	action_loadDirectory->setStatusTip(tr("Load Directory"));
    
    connect(action_loadDirectory, &QAction::triggered, this, &View::onLoadDirectory);
    View::onSetPaint();
}

void View::setupLeftToolBar(QJsonObject const& a_config) 
{
    View::creteAction();
    Logger->trace("View::setupLeftToolBar()");
    m_leftToolBar = new ToolBar();
    m_leftToolBar->setWindowFlag(Qt::FramelessWindowHint);
    m_leftToolBar->setOrientation(Qt::Vertical);
    m_leftToolBar->addSeparator();
    m_colorPicker = new ColorPicker(a_config);
    m_leftToolBar->addWidget(m_colorPicker);
    connect(m_colorPicker, &ColorPicker::changeColor, this, &View::onChangeColor);
    m_penSizePicker = new PenSizePicker();
    m_leftToolBar->addWidget(m_penSizePicker);
    connect(m_penSizePicker, &PenSizePicker::changePenSize, this, &View::onChangePenSize);

    m_leftToolBar->addSeparator();
    
	m_leftToolBar->addAction(action_paint);
	m_leftToolBar->addAction(action_move);
	m_leftToolBar->addAction(action_ROI);
    m_leftToolBar->addSeparator();
    m_leftToolBar->addAction(action_loadDirectory);
    m_leftToolBar->addSeparator();
    m_loadButton = new QToolButton;
    m_loadButton->setText(tr("Load Directory"));
    m_loadButton->setChecked(false);
    m_loadButton->setToolTip("Load Directory");
    m_leftToolBar->addWidget(m_loadButton);
    m_leftToolBar->addSeparator();
    m_saveGTbutton = new QToolButton;
    m_saveGTbutton->setText(tr("Save gt"));
    m_saveGTbutton->setChecked(false);
    m_leftToolBar->addWidget(m_saveGTbutton);
    connect(m_loadButton, SIGNAL(clicked()), this, SLOT(onLoadDirectory()));
}

void View::onLoadDirectory()
{
    m_targetDirectoryPath = QFileDialog::getExistingDirectory(
        this, tr("Open Dir"), QDir::currentPath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (m_targetDirectoryPath == "") {
        return;
    }
    m_targetDirectoryPath += "/";
    View::loadImage("/home/gm/Obrazy/temp.png");
    View::renderColorsFromImage("/home/gm/Obrazy/temp2.png");
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
    if (m_painterSettings.m_penSize == 1)
    {
        m_diff.setPixelColor(x, y, m_painterSettings.m_color);
    }
    else if (m_painterSettings.m_penSize > 1 && m_painterSettings.m_penSize < 20)
    {
        qint32 it = m_painterSettings.m_penSize - 1;
        for (int zx = -it; zx <= it; zx++)
        {
            for (int zy = -it; zy <= it; zy++)
            {
                m_diff.setPixelColor(x + zx, y + zy, m_painterSettings.m_color);
            }
        }
    }
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

void View::loadImage(QString imageName)
{
    Logger->debug("View::loadImage()");
    QPixmap test;
    test.load(imageName);
    addImageToScene(test);

}

void View::addImageToScene(QPixmap image)
{
    Logger->trace("View::addImageToScene()");
    onResetScene();
    //m_pixmap = static_cast<GraphicsPixmapItem*>(m_graphicsScene->addPixmap(image));
    m_pixmap = static_cast<QGraphicsPixmapItem*>(m_graphicsScene->addPixmap(image));
    m_image = image.toImage();
    m_diff = image.toImage();

    for (int y = 0; y < m_diff.height(); y++)
    {
        for (int x = 0; x < m_diff.width(); x++)
        {
            m_diff.setPixelColor(x, y, QColor{ 255, 255, 0, 127 });
        }
    }
    QPixmap whiteBoardPixmap = QPixmap::fromImage(m_diff);
    //m_whitePixmap = static_cast<GraphicsPixmapItem*>(m_graphicsScene->addPixmap(whiteBoardPixmap));
    m_whitePixmap = static_cast<QGraphicsPixmapItem*>(m_graphicsScene->addPixmap(whiteBoardPixmap));
    m_whitePixmap->update();

    m_pixmap->setEnabled(true);
    m_pixmap->setVisible(true);
    m_pixmap->setOpacity(1.80);
    m_pixmap->setAcceptHoverEvents(true);
    m_pixmap->setAcceptTouchEvents(true);
    m_pixmap->setZValue(-2);
    m_pixmap->update();
    View::setOpacity();
    View::resetView();
}

void View::onResetScene()
{
    qDebug() << "View::onResetScene:";
    emit(resetScene());
    View::resetView();
}

void View::resetView()
{
    Logger->trace("View::resetView()");
    setupMatrix();
}


void View::renderColorsFromImage(QString pathToImage)
{
    Logger->trace("View::renderColorsFromImage() pathToImage:{}", pathToImage.toStdString());
	cv::Mat image = cv::imread(pathToImage.toStdString());
	
    Logger->trace("View::renderColorsFromImage() image ({}x{}x{})", image.cols, image.rows, image.channels());
	if(image.empty())
	{
        Logger->error("View::renderColorsFromImage() image cant be loaded:{}", pathToImage.toStdString());
		return;
	}
	else
	{
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);

		for (qint32 color = 0; color < m_painterSettings.m_colors.size(); color++)
		{
            /*if (m_painterSettings.m_colors[color] == BLACK)
			{
				continue;
			}*/
			for (int i = 0; i < image.cols; i++)
			{
				for (int j = 0; j < image.rows; j++)
				{
                    if ( m_painterSettings.m_colorInthash[m_painterSettings.m_colors[color]] == image.at<unsigned char>(j, i))
					{
                        onPaintColors(i, j, m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]]);
					}
				}

			}
		}
        /*
		for (int i = 0; i < image.cols; i++)
		{
			for (int j = 0; j < image.rows; j++)
			{
				if ((  image.at<unsigned char>(j, i) < 255) && (image.at<unsigned char>(j, i)  > 0) )
				{
					onPaintColors(i, j, m_colorShadow);
				}
			}

        }*/
	}
	View::onPaintColorsFinish();
}

void View::onPaintColors(qint32 x, qint32 y, QColor color)
{
    m_diff.setPixelColor(x, y, color);
}

void View::onPaintColorsFinish()
{
    Logger->trace("View::onPaintColorsFinish()");
    m_whitePixmap->setPixmap(QPixmap::fromImage(m_diff));
}

void View::onChangeColor(QColor color)
{
    qDebug() << "View::onChangeColor:" << color;
    m_painterSettings.m_color = color;
}

void View::onChangePenSize(qint32 size)
{
    qDebug() << "View::onChangePenSize:" << size;
    m_painterSettings.m_penSize = size;
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