#ifndef DLIB_NETWORK_H
#define DLIB_NETWORK_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QRandomGenerator>

#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



#include "includespdlog.h"
#include "configreader.h"
#include "filelogger.h"
#include "dnn_instance.h"
#include "data.h"
#include "postprocessing.h"
#include "graph.h"

#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include "dlib/cuda/gpu_data.h"
#include "dlib/opencv/to_open_cv.h"
#include "dlib/opencv/cv_image.h"



struct truth_instance
{
    dlib::matrix<unsigned char> input_image;
    dlib::matrix<float> label_image;
};

class DlibNetwork : public QObject {
	Q_OBJECT
  
	public:
		DlibNetwork(DataMemory* data, FileLogger *fileLoggerTrain, FileLogger *fileLoggerTest, FileLogger *fileLoggerJSON);
		~DlibNetwork();
		void process();

		truth_instance load_truth_instances(const image_info& info);
		net_type train_segmentation_network(const std::vector<truth_instance>& truth_images);

	signals:
		void dlibConfigured();
		void newConfig();
		void logJsonBest(QJsonObject json);
		void appendToFileLogger(QStringList list);
		void configureLogger(QString name, bool additionalLogs);
		void configureLoggerJSON(QString name, bool additionalLogs);

	public slots:
		void configure(QJsonObject const& a_config, QJsonArray const& a_postprocess);
		void onSignalOk(struct fitness fs, qint32 slot);
		//void loadDatasetName(QJsonObject const& a_dataset);
		void onLoadDirectory(QString folderName);
		void onTrainNetwork();
		

	private:
		void loadFromConfig(QJsonObject const& a_config);
		void iteration();
		void clearData();
		void loadDronConfigs(QJsonArray const& a_preprocess);
		void handleBestPopulation();
		void testNetwork(QString id, net_type segb, QString clean, QString gt, FileLogger* fileLogger);
		void postprocessing();
		fitness finishPostProcessing();
		void logPopulation(QString id, fitness fs, FileLogger * fileLogger);

	private:
		QRandomGenerator* m_randomGenerator;

	private:
		Graph<PostProcess, _postData> m_graph_postprocessing;
		std::vector<std::vector<_postData>> m_dataPostprocess;
		std::vector<PostProcess*> m_blockPostprocess;
		std::vector<cv::Mat> m_outputData;

	private:
		DataMemory* m_dataMemory;
		QJsonObject m_config;
		QJsonObject m_boundsGraph;
		QJsonArray m_graph;
		QJsonArray m_postprocess;
		

	private:
		int m_maxNumEpochs{};
		int m_minBatchSize{};
		int m_imageSize{};
		int m_queqe{};
		bool m_useTwoCuda{};
		double m_learningRate{};
		QString m_dnnName{};
		QString m_synchName{};

		int m_dronNoise{};
		int m_dronContrast{};

		double m_fitnessThreshold{};
		double m_bestChangeLast{};
		double m_delta{};
		double m_currentLearningRate{};
		double m_averageLoss{};
		int m_epoch_counter{};

		FileLogger *m_fileLoggerTrain;
		FileLogger *m_fileLoggerTest;
		FileLogger *m_fileLoggerJSON;
		
		cv::TickMeter m_timer;

		QString m_fileName{};
		QString m_graphType{};
		QString m_boundType{};
		QString m_dronType{};
		QString m_logsFolder{};
		QString m_videoLogsFolder{};
		QString m_dnnFolder{};
		QString m_resultsPath{};
		QString m_split{};

		QString m_synchronizationNetworkFileName{};
		QString m_outputNetworkFileName{};

		QString m_cleanPath{};
		QString m_configPath{};
		QString m_gtPath{};
		QString m_cleanTrainPath{};
		QString m_gtTrainPath{};
		QString m_cleanTestPath{};
		QString m_gtTestPath{};
		QString m_cleanRoiPath{};
		QString m_gtRoiPath{};

		QString m_inputType{};
		QString m_outputType{};
		QString m_logsFolderTestCase{};
		QString m_videoLogsTestCase{};
		QString m_dnnFolderTestCase{};
		QString m_infoName{};

		qint64 m_nowTime;
};

#endif // DLIB_NETWORK_H