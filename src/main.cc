#include <QApplication>
#include <QStandardItemModel>

#include "configreader.h"
#include "includespdlog.h"
#include "mainwindow.h"

constexpr auto CONFIG{ "config.json" };
constexpr auto LOG_LEVEL{ "LogLevel" };
constexpr auto GENERAL{ "General" };


void intro(int loglevel);

QJsonObject readConfig();

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	qRegisterMetaType<QString>("QString");
	
	//Logger->set_level(static_cast<spdlog::level::level_enum>(0));
	Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");

	QJsonObject config = readConfig();
	
	
	int messageLevel{ config[GENERAL].toObject()[LOG_LEVEL].toInt() };
	//Logger->info("messageLevel:{}", messageLevel);
	Logger->set_level(static_cast<spdlog::level::level_enum>(messageLevel));
	intro(messageLevel);
	
	MainWindow window{ config };
	window.show();
	return app.exec();
}

void intro(int loglevel) {
	Logger->info("\n\n\t \033[1;31mgt-annotate v3.3\033[0m \n"
		"\t Author: Grzegorz Matczak \n"
		"\t 09.12.2021 \n"
		"\t log level:{}", loglevel);
}

QJsonObject readConfig()
{
	QString configName{ CONFIG };
	std::unique_ptr<ConfigReader> cR = std::make_unique<ConfigReader>();
	QJsonObject jObject;
	if (!cR->readConfig(configName, jObject))
	{
		Logger->error("File {} read config failed", configName.toStdString());
		exit(-1);
	}
	return jObject;
}
