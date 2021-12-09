#include "widgets/data/datamemory.h"

#include "scanfiles.h"
#include "includespdlog.h"
#include "configreader.h"

constexpr auto DATASET{ "Dataset" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto DIR_CLEAN{ "DirectoryClean" };
constexpr auto DIR_GT{ "DirectoryGt" };
constexpr auto DIR_CLEAN_TRAIN{ "DirectoryCleanTrain" };
constexpr auto DIR_CLEANT_TEST{ "DirectoryCleanTest" };
constexpr auto DIR_GT_TRAIN{ "DirectoryGtTrain" };
constexpr auto DIR_GT_TEST{ "DirectoryGtTest" };
constexpr auto START_TRAIN{"StartTrain"};
constexpr auto STOP_TRAIN{"StopTrain"};
constexpr auto START_TEST{"StartTest"};
constexpr auto STOP_TEST{"StopTest"};
constexpr auto RESIZE{"Resize"};
constexpr auto INPUT_TYPE{ "InputType" };
constexpr auto OUTPUT_TYPE{ "OutputType" };
constexpr auto INPUT_PREFIX{ "InputPrefix" };
constexpr auto INITIALIZATION_FRAMES{ "InitializationFrames" };
constexpr auto ZERO_PADDING{ "ZeroPadding" };

DataMemory::DataMemory() {
	spdlog::trace("DataMemory::DataMemory()done");
}

DataMemory::~DataMemory() {}

void DataMemory::cleanData() {
	spdlog::trace("DataMemory::cleanData()");
}

void DataMemory::loadCleanData()
{
	/*
	spdlog::trace("DataMemory::loadCleanData()");

	QString configName{ "config.json" };
	ConfigReader* _configReader = new ConfigReader();
	QJsonObject jObject;
	if (!_configReader->readConfig(configName, jObject))
	{
		spdlog::error("File {} read config failed", configName.toStdString());
		return ;
	}
	delete _configReader;

	auto jDataset{ jObject[DATASET].toObject() };
	m_startGT = jDataset[START_GT].toInt();
	m_stopGT = jDataset[STOP_GT].toInt();
	spdlog::trace("DataMemory::DataMemory() m_stopGT:{}", m_stopGT);
	m_resize = jDataset[RESIZE].toBool();
	m_width = jDataset[WIDTH].toInt();
	m_height = jDataset[HEIGHT].toInt();

	QString _name = jDataset[FOLDER].toString() + jDataset[STREAM_INPUT].toString();

	m_videoFromFile.open(_name.toStdString());
	qint32 iter = 0;
	if (!m_videoFromFile.isOpened())
	{
		spdlog::error("Stream:{} not open!", _name.toStdString());
	}

	while (m_videoFromFile.isOpened()) {
		cv::Mat inputMat;
		cv::Mat inputMatResize;
		m_videoFromFile >> inputMat;
		if (inputMat.cols == 0 || inputMat.rows == 0) {
			spdlog::info("DataMemory() add:{} frames to cleanData ({}x{}x{})", iter,
				inputMat.cols, inputMat.rows, inputMat.channels());
			break;
		}
		iter++;

		if (iter >= m_stopGT) {
			spdlog::trace("DataMemory() m_stopGT:{}", m_stopGT);
			spdlog::info("DataMemory() add:{} frames to cleanData ({}x{}x{})", iter,
				inputMat.cols, inputMat.rows, inputMat.channels());
			break;
		}
		if (m_resize) {
			cv::resize(inputMat, inputMatResize, cv::Size(m_width, m_height));
		}
		else {
			cv::Rect region_of_interest = cv::Rect(0, 0, m_width, m_height);
			inputMatResize = inputMat(region_of_interest);
		}
		m_cleanData.append(inputMatResize);
	}
	spdlog::debug("DataMemory::addCleanDataFromFile() done");
	*/
}