#include "../include/mainwindow.h"

#include <QtWidgets>

#define THREADS 8

constexpr auto NAME{ "Name" };
constexpr auto COLORS{ "Colors" };
constexpr auto GENERAL{ "General" };
constexpr auto LOG_LEVEL{ "LogLevel" };
constexpr auto DATASET{ "Dataset" };


MainWindow::MainWindow()
{
	Logger->trace("MainWindow::MainWindow()");
}

MainWindow::MainWindow(QJsonObject const& a_config)
{
	Logger->trace("MainWindow::MainWindow()");
	MainWindow::configure(a_config);
}

void MainWindow::configure(QJsonObject const& a_config) 
{
	QJsonObject jColors = a_config[COLORS].toObject();
	QJsonObject jDataset = a_config[DATASET].toObject();

    createMenus();
    setupMainWidget();
}

void MainWindow::setupMainWidget() 
{
    Logger->trace("MainWindow::MainWindow() mainCentralLayout:");
	QGridLayout* mainCentralLayout = new QGridLayout;
	mainCentralLayout->setContentsMargins(0, 0, 0, 0);
	//mainCentralLayout->addWidget(leftToolBar, 0, 0);
	//mainCentralLayout->addWidget(rightWidget, 0, 1);

	QWidget* mainCentralWidget = new QWidget(this);
	mainCentralWidget->setLayout(mainCentralLayout);

	m_progressBar = new QProgressBar(this);
    m_progressBar->setMinimum(0);
	m_progressBar->setMaximum(100);

	Logger->trace("MainWindow::MainWindow() mainLayout:");
	QGridLayout* mainLayout = new QGridLayout;
	mainLayout->setContentsMargins(0, 0, 0, 0);
	//mainLayout->addWidget(menuBar, 0, 0);
	mainLayout->addWidget(mainCentralWidget, 1, 0);
	mainLayout->addWidget(m_progressBar, 2, 0);
	
	setLayout(mainLayout);

	resize(1600, 600);
}

void MainWindow::createMenus()
{
	menuBar = new QMenuBar(this);
	fileMenu = menuBar->addMenu(tr("&File"));
	//fileMenu->addAction(action_open);
	fileMenu->addSeparator();
	//fileMenu->addAction(action_autoSaving);
}

void MainWindow::setupView(QJsonObject const& a_config)
{
	view = new View(a_config);
    /*
	connect(this, &MainWindow::addImageToScene, view, &View::onAddImageToScene);
	connect(this, &MainWindow::setRectOpacity, view, &View::onSetRectOpacity);
	connect(this, &MainWindow::renderColors, view, &View::renderColorsFromJson);
	connect(this, &MainWindow::addRectToScene, view, &View::onAddRectToScene);
	connect(view, &View::updateLabelList, this, &MainWindow::onUpdateLabelList);
	connect(this, &MainWindow::setColor, view, &View::setColor);
	connect(view, &View::updateFile, this, &MainWindow::onUpdateFile);
	connect(view, &View::deleteList, this, &MainWindow::onDeleteList);
	connect(this, &MainWindow::updateFilename, view, &View::onUpdateFilename);
	connect(view, &View::addList, this, &MainWindow::onAddList);
	connect(this, &MainWindow::addList, this, &MainWindow::onAddList);
	connect(this, &MainWindow::clearItems, view, &View::onClearItems);*/
}