#include <QApplication>
#include <QStandardItemModel>

#include "../include/configreader.h"
#include "../include/includespdlog.h"
#include "../include/mainwindow.h"

constexpr auto CONFIG{ "config.json" };
constexpr auto LOG_LEVEL{ "LogLevel" };
constexpr auto GENERAL{ "General" };


void intro();

QJsonObject readConfig();

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	Logger->set_level(static_cast<spdlog::level::level_enum>(0));
	Logger->set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");

	QJsonObject config = readConfig();
	
	intro();
	qint32 messageLevel{ config[GENERAL].toObject()[LOG_LEVEL].toInt() };
	Logger->info("messageLevel:{}", messageLevel);
	Logger->set_level(static_cast<spdlog::level::level_enum>(messageLevel));

	MainWindow window{ config };
	window.show();
	return app.exec();
}

void intro() {
	Logger->info("\n\n\t\033[1;31mgt-annotate v3.0\033[0m\n"
		"\tAuthor: Grzegorz Matczak\n"
		"\t03.05.2021\n");
}

QJsonObject readConfig()
{
	QString configName{ CONFIG };
	std::shared_ptr<ConfigReader> cR = std::make_shared<ConfigReader>();
	QJsonObject jObject;
	if (!cR->readConfig(configName, jObject))
	{
		Logger->error("File {} read confif failed", configName.toStdString());
		exit(-1);
	}
	return jObject;
}