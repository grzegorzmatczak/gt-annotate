#include "mainwindow.h"

#include <QtWidgets>

#define THREADS 8

constexpr auto NAME{ "Name" };
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
	QJsonObject jDataset = a_config[DATASET].toObject();
	m_dataMemory = new DataMemory();
	

	createMenus();
	setupView(a_config);
	setupLayout();
	setupMainWidget();
}

void MainWindow::setupMainWidget() 
{
	Logger->trace("MainWindow::MainWindow() mainLayout:");
	
	QGridLayout* mainLayout = new QGridLayout;
	m_view->setMinimumWidth(1000);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	//mainLayout->addWidget(menuBar, 0, 0);
	mainLayout->addWidget(m_view, 0, 0);
	//mainLayout->addWidget(m_dataWidget, 0, 1);
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
{
	Logger->trace("MainWindow::MainWindow() setupLayout");
}

