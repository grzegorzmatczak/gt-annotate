#include <QApplication>
#include <QStandardItemModel>

#include "../include/configreader.h"
#include "../include/includespdlog.h"

constexpr auto CONFIG{ "config.json" };
constexpr auto LOG_LEVEL{ "LogLevel" };
constexpr auto GENERAL{ "General" };


void intro();

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	spdlog::set_level(static_cast<spdlog::level::level_enum>(0));
	spdlog::set_pattern("[%Y-%m-%d] [%H:%M:%S.%e] [%t] [%^%l%$] %v");

	QString configName{ CONFIG };
	std::shared_ptr<ConfigReader> cR = std::make_shared<ConfigReader>();
	QJsonObject jObject;
	if (!cR->readConfig(configName, jObject))
	{
		spdlog::error("File {} read confif failed", configName.toStdString());
		return -66;
	}
	intro();
	qint32 messageLevel{ jObject[GENERAL].toObject()[LOG_LEVEL].toInt() };
	spdlog::info("messageLevel:{}", messageLevel);
	spdlog::set_level(static_cast<spdlog::level::level_enum>(messageLevel));

	//MainWindow window{ jObject };
	//window.show();
	return app.exec();
}

void intro() {
	spdlog::info("\n\n\t\033[1;31mgt-annotate v3.0\033[0m\n"
		"\t Author: Grzegorz Matczak\n"
		"\t 03.05.2021\n");
}