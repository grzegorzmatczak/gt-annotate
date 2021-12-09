#include "mainwindow.h"

#include <QtWidgets>

#define THREADS 8
//#define DEBUG

constexpr auto NAME{ "Name" };
constexpr auto GENERAL{ "General" };
constexpr auto LOG_LEVEL{ "LogLevel" };
constexpr auto DATASET{ "Dataset" };


MainWindow::MainWindow()
{
	#ifdef DEBUG
	Logger->debug("MainWindow::MainWindow()");
	#endif
}

MainWindow::MainWindow(QJsonObject const& a_config)
{
	#ifdef DEBUG
	Logger->debug("MainWindow::MainWindow(a_config)");
	#endif
	MainWindow::configure(a_config);
}

void MainWindow::configure(QJsonObject const& a_config) 
{
	QJsonObject jDataset = a_config[DATASET].toObject();
	m_dataMemory = new DataMemory();

	createMenus();
	setupView(a_config);
	setupLayout();
	setupMainWidget();
}

void MainWindow::setupMainWidget() 
{
	#ifdef DEBUG
	Logger->debug("MainWindow::setupMainWidget()");
	#endif
	
	QGridLayout* mainLayout = new QGridLayout;
	m_view->setMinimumWidth(1000);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->addWidget(m_view, 0, 0);
	setLayout(mainLayout);

	resize(1600, 600);
}

void MainWindow::createMenus()
{}

void MainWindow::setupView(QJsonObject const& a_config)
{
	m_view = new View(a_config, m_dataMemory);
	m_view->setMinimumWidth(1000);
}

void MainWindow::setupLayout() 
{}
