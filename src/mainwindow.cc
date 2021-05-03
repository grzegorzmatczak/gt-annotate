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

}