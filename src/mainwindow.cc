#include "mainwindow.h"

#include <QtWidgets>

#define THREADS 8
//#define DEBUG

constexpr auto NAME{ "Name" };
constexpr auto GENERAL{ "General" };
constexpr auto LOG_LEVEL{ "LogLevel" };
constexpr auto DATASET{ "Dataset" };
constexpr auto CONFIG{ "Config" };
constexpr auto PREPROCESS{ "Dataset" };


MainWindow::MainWindow()
{
	#ifdef DEBUG
	Logger->debug("MainWindow::MainWindow()");
	#endif
}

MainWindow::MainWindow(QJsonObject const& a_config, QJsonObject const& a_postprocess)
{
	#ifdef DEBUG
	Logger->debug("MainWindow::MainWindow(a_config)");
	#endif
	MainWindow::configure(a_config, a_postprocess);
}

void MainWindow::configure(QJsonObject const a_config, QJsonObject const a_postprocess) 
{
	QJsonObject jDataset = a_config[DATASET].toObject();
	m_dataMemory = new DataMemory();

	createMenus();
	setupView(a_config);
	setupLayout();
	

	m_fileLoggerTrainThread = new QThread();
	m_fileLoggerTrain = new FileLogger() ;
	m_fileLoggerTrain->moveToThread(m_fileLoggerTrainThread);
	connect(m_fileLoggerTrainThread, &QThread::finished, m_fileLoggerTrain, &QObject::deleteLater);
	m_fileLoggerTrainThread->start();

	m_fileLoggerTestThread = new QThread();
	m_fileLoggerTest = new FileLogger() ;
	m_fileLoggerTest->moveToThread(m_fileLoggerTestThread);
	connect(m_fileLoggerTestThread, &QThread::finished, m_fileLoggerTest, &QObject::deleteLater);
	m_fileLoggerTestThread->start();

	m_fileLoggerJSONThread = new QThread();
	m_fileLoggerJSON = new FileLogger();
	m_fileLoggerJSON->moveToThread(m_fileLoggerJSONThread);
	connect(m_fileLoggerJSONThread, &QThread::finished, m_fileLoggerJSON, &QObject::deleteLater);
	m_fileLoggerJSONThread->start();


	m_dlibThread = new QThread();
	m_dlib = new DlibNetwork(m_dataMemory, m_fileLoggerTrain, m_fileLoggerTest, m_fileLoggerJSON);
	m_dlib->moveToThread(m_fileLoggerJSONThread);
	connect(m_dlibThread, &QThread::finished, m_dlib, &QObject::deleteLater);
	m_dlibThread->start();

	m_dlib->configure(a_config, a_postprocess["Graph_dlib"].toArray());
	connect(m_view, &View::loadDirectory, m_dlib, &DlibNetwork::onLoadDirectory);
	m_view->configure();

	connect(m_view, &View::trainNetwork, m_dlib, &DlibNetwork::onTrainNetwork);

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
