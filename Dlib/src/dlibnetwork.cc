#include "dlibnetwork.h"
#include "dnn_instance.h"

#include <dlib/dnn.h>
#include <dlib/data_io.h>
#include "dlib/cuda/gpu_data.h"
#include "dlib/dnn/layers.h"
#include <dlib/gui_widgets.h>

#include <QDebug>
#include <QDateTime>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

// THREADS: 1/4/8/16/32
#define THREADS 4
#define DEBUG
//#define DEBUG_POSTPROCESSING

constexpr auto GENERAL{ "General" };
constexpr auto GENETIC{ "Genetic" };
constexpr auto DLIB{ "Dlib" };
constexpr auto GRAPH_TYPE{ "GraphType" };
constexpr auto DRON_TYPE{ "DronType" };
constexpr auto BOUND_TYPE{ "BoundType" };
constexpr auto RESULTS_PATH{"ResultsPath"};
constexpr auto RESULTS{"Results"};

constexpr auto CONFIG{ "Config" };
constexpr auto NAME{ "Name" };
constexpr auto DRON_NOISE{ "Noise" };
constexpr auto DRON_RAND_SEED{ "RandSeed" };
constexpr auto DRON_CONTRAST{ "Contrast" };

constexpr auto LOGS_FOLDER{ "LogsFolder" };
constexpr auto VIDEO_LOGS_FOLDER{ "VideoLogsFolder" };
constexpr auto DNN_FOLDER{ "DnnFolder" };
constexpr auto CONFIG_LINUX{ "ConfigLinux" };
constexpr auto CONFIG_WIN{ "ConfigWin" };

constexpr auto DRON_NOISE_START{ "DronNoiseStart" };
constexpr auto DRON_NOISE_STOP{ "DronNoiseStop" };
constexpr auto DRON_NOISE_DELTA{ "DronNoiseDelta" };
constexpr auto DRON_CONTRAST_START{ "DronContrastStart" };
constexpr auto DRON_CONTRAST_STOP{ "DronContrastStop" };
constexpr auto DRON_CONTRAST_DELTA{ "DronContrastDelta" };

// Graph:
constexpr auto NEXT{ "Next" };
constexpr auto PREV{ "Prev" };
constexpr auto GRAPH{ "Graph" };
constexpr auto TYPE{ "Type" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto ENCODER{ "Encoder" };

// Dlib:
constexpr auto MAX_NUM_EPOCHS{ "MaxNumEpochs" };
constexpr auto MIN_BATCH_SIZE{ "MinibatchSize" };
constexpr auto IMAGE_SIZE{ "ImageSize" };
constexpr auto QUEQE{ "Queqe" };
constexpr auto DNN_NAME{ "DnnName" };
constexpr auto USE_TWO_CUDA{ "UseTwoCUDA" };
constexpr auto LEARNING_RATE{ "LearningRate" };
constexpr auto SYNCH_NAME{ "SynchName" };

// Dataset:
constexpr auto DIR_CLEAN{ "DirectoryClean" };
constexpr auto DIR_GT{ "DirectoryGt" };
constexpr auto DIR_CLEAN_TRAIN{ "DirectoryCleanTrain" };
constexpr auto DIR_CLEANT_TEST{ "DirectoryCleanTest" };
constexpr auto DIR_GT_TRAIN{ "DirectoryGtTrain" };
constexpr auto DIR_GT_TEST{ "DirectoryGtTest" };
constexpr auto DIR_CLEAN_ROI{ "DirectoryCleanRoi" };
constexpr auto DIR_GT_ROI{ "DirectoryGtRoi" };


constexpr auto PATH_TO_DATASET{ "PathToDataset" };
constexpr auto CONFIG_NAME{ "ConfigName" };
constexpr auto DATASET_UNIX{ "DatasetLinux" };
constexpr auto DATASET_WIN32{ "DatasetWin32" };
constexpr auto INPUT_TYPE{ "InputType" };
constexpr auto OUTPUT_TYPE{ "OutputType" };


DlibNetwork::DlibNetwork(DataMemory* data, FileLogger *fileLoggerTrain, FileLogger *fileLoggerTest, FileLogger *fileLoggerJSON)
: m_dataMemory(data)
, m_fileLoggerTrain(fileLoggerTrain)
, m_fileLoggerTest(fileLoggerTest)
, m_fileLoggerJSON(fileLoggerJSON)
, m_useTwoCuda(false)
, m_currentLearningRate(0.01)
, m_averageLoss(0.1)
, m_epoch_counter(0)
{
	#ifdef DEBUG
	Logger->debug("DlibNetwork::DlibNetwork()");
	#endif
	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX
}

DlibNetwork::~DlibNetwork()
{
	#ifdef DEBUG
	Logger->debug("DlibNetwork::~DlibNetwork()");
	#endif
}

void DlibNetwork::loadFromConfig(QJsonObject const& a_config)
{
	#ifdef DNN_2LAYERS_40CON
		m_boundType = "Dnn_2Layer_40Con";
	#endif

	QJsonObject dlibConfig = a_config[DLIB].toObject();
	m_resultsPath = a_config[RESULTS].toObject()[RESULTS_PATH].toString();

	m_maxNumEpochs = dlibConfig[MAX_NUM_EPOCHS].toInt();
	m_minBatchSize = dlibConfig[MIN_BATCH_SIZE].toInt();
	m_imageSize = dlibConfig[IMAGE_SIZE].toInt();
	m_queqe = dlibConfig[QUEQE].toInt();
	m_dnnName = dlibConfig[DNN_NAME].toString();
	m_useTwoCuda = dlibConfig[USE_TWO_CUDA].toBool();
	m_learningRate = dlibConfig[LEARNING_RATE].toDouble();
	m_synchName = dlibConfig[SYNCH_NAME].toString();
	

	#ifdef _WIN32
    QJsonObject configPaths = a_config[CONFIG_WIN].toObject();
    #endif // _WIN32
    #ifdef __linux__
    QJsonObject configPaths = a_config[CONFIG_LINUX].toObject();
    #endif // __linux__

	m_logsFolder = configPaths[LOGS_FOLDER].toString();
	m_videoLogsFolder = configPaths[VIDEO_LOGS_FOLDER].toString();
	m_dnnFolder = configPaths[DNN_FOLDER].toString();

	m_logsFolderTestCase = m_logsFolder;
	m_videoLogsTestCase = m_videoLogsFolder;
	m_dnnFolderTestCase = m_dnnFolder;
	#ifdef DEBUG
	Logger->info("Dlib::configure() mini-batch size:{}", m_minBatchSize);
	#endif

}

void DlibNetwork::loadDronConfigs(QJsonArray const& a_preprocess)
{}

void DlibNetwork::onLoadDirectory(QString folderName)
{
	Logger->debug("DlibNetwork::onLoadDirectory()");
	QString pathConfig = folderName + "config.json";
	QJsonObject datasetConfig{};
	std::shared_ptr<ConfigReader> cR = std::make_shared<ConfigReader>();
	if (!cR->readConfig(pathConfig, datasetConfig))
	{
		Logger->error("DataMemory::configure() File {} not readed", (pathConfig).toStdString());
	}

	m_cleanPath = datasetConfig[DIR_CLEAN].toString();
	m_gtPath = datasetConfig[DIR_GT].toString();

	m_inputType = datasetConfig[INPUT_TYPE].toString();
	m_outputType = datasetConfig[OUTPUT_TYPE].toString();

	m_cleanTrainPath = datasetConfig[DIR_CLEAN_TRAIN].toString();
	m_gtTrainPath = datasetConfig[DIR_GT_TRAIN].toString();

	m_cleanTestPath = datasetConfig[DIR_CLEANT_TEST].toString();
	m_gtTestPath = datasetConfig[DIR_GT_TEST].toString();

	m_cleanRoiPath = datasetConfig[DIR_CLEAN_ROI].toString();
	m_gtRoiPath = datasetConfig[DIR_GT_ROI].toString();
}

void DlibNetwork::configure(QJsonObject const& a_config, QJsonArray const& a_postprocess)
{
	#ifdef DEBUG
	Logger->debug("DlibNetwork::configure()");
	#endif

	m_config = a_config;
	m_postprocess = a_postprocess;
	
	#ifdef DEBUG
	qDebug() << "DlibNetwork::configure() a_config:" << a_config << "\n";
	qDebug() << "DlibNetwork::configure() m_postprocess:" << a_postprocess << "\n";
	#endif
	DlibNetwork::loadFromConfig(a_config);
	DlibNetwork::clearData();
	
	m_nowTime = qint64(QDateTime::currentMSecsSinceEpoch());
	
	#ifdef DEBUG
	Logger->debug("DlibNetwork::configure() m_logsFolderTestCase:{}", m_logsFolderTestCase.toStdString());
	#endif
	m_fileName = m_logsFolderTestCase + "train" + "_" + QString::number(m_dronNoise) + "_" + 
			QString::number(m_dronContrast) + "_" + QString::number(m_nowTime);
	m_fileLoggerTrain->onConfigure(m_fileName + ".txt");
	m_fileName = m_logsFolderTestCase + "test" + "_" + QString::number(m_dronNoise) + "_" +
			QString::number(m_dronContrast) + "_" + QString::number(m_nowTime);
	m_fileLoggerTest->onConfigure(m_fileName + ".txt");
	m_fileName = m_logsFolderTestCase + "config" + "_" + QString::number(m_dronNoise) + "_" +
			QString::number(m_dronContrast) + "_" + QString::number(m_nowTime);
	m_fileLoggerJSON->onConfigure(m_fileName + ".json");
	m_infoName = m_logsFolderTestCase + "info" + "_" + QString::number(m_dronNoise) + "_" +
			QString::number(m_dronContrast) + "_" + QString::number(m_nowTime);

	Logger->info("Dlib::configure() file:{}", (m_fileName).toStdString());		
	#ifdef DEBUG
	Logger->debug("Dlib::configure() file:{}", (m_fileName + ".txt").toStdString());
	#endif
	


	
	#ifdef DEBUG
	Logger->debug("DlibNetwork::configure() done");
	#endif
}

void DlibNetwork::onTrainNetwork()
{
	#ifdef DEBUG
	Logger->debug("DlibNetwork::onTrainNetwork()");
	#endif

	if (!m_dataMemory->loadNamesForPreTraining())
	{
		return;
	}
	int device_num = dlib::cuda::get_num_devices();
	int device = dlib::cuda::get_device();
	std::string device_name = dlib::cuda::get_device_name(0);
	std::vector<image_info> listing = m_dataMemory->get_imageInfoRoiTrain();
	
	#ifdef DEBUG
	Logger->debug("Dlib::configure() device_num:{}", device_num);
	Logger->debug("Dlib::configure() device:{}", device);
	Logger->debug("Dlib::configure() device_name:{}", device_name);
	Logger->debug("Dlib::configure() m_dataMemory->get_imageInfoRoiTrain() ok");
	#endif

	std::vector<truth_instance>  list;

	#ifdef DEBUG
	Logger->debug("DlibCase::configure() m_maxNumEpochs:{}", m_maxNumEpochs);
	Logger->debug("DlibCase::configure() m_minBatchSize:{}", m_minBatchSize);
	Logger->debug("DlibCase::configure() m_imageSize:{}", m_imageSize);
	Logger->debug("DlibCase::configure() m_queqe:{}", m_queqe);
	Logger->debug("DlibCase::configure() m_useTwoCuda:{}", m_useTwoCuda);
	Logger->debug("DlibCase::configure() m_learningRate:{}", m_learningRate);
	Logger->debug("DlibCase::configure() m_dnnName:{}", m_dnnName.toStdString());
	Logger->debug("DlibCase::configure() m_synchName:{}", m_synchName.toStdString());
	Logger->debug("Dlib::configure() load_truth_instances ... listing.size():{}", listing.size());
	#endif

	for (int i = 0; i < listing.size(); i++)
	{
		list.push_back(this->load_truth_instances(listing[i]));
	}

	m_synchronizationNetworkFileName = m_dnnFolderTestCase + m_synchName + "_" + QString::number(m_dronNoise) + "_" + QString::number(m_dronContrast) + "_" + QString::number(m_nowTime) + ".dat";

	#ifdef DEBUG
	Logger->debug("Dlib::configure() load_truth_instances ... ok");
	Logger->debug("Dlib::configure() m_synchronizationNetworkFileName:{}", m_synchronizationNetworkFileName.toStdString());
	Logger->debug("Dlib::configure() train_segmentation_network() ...");
	#endif
	Logger->debug("Dlib::configure() m_synchronizationNetworkFileName:{}", m_synchronizationNetworkFileName.toStdString());
	int imageSize = m_config[DLIB].toObject()["ImageSize"].toInt();
	int queqe = m_config[DLIB].toObject()["Queqe"].toInt();
	QString dnnName = m_config[DLIB].toObject()["DnnName"].toString();
	QString synch_name = m_config[DLIB].toObject()["SynchName"].toString();
	QString synch_nameNet = synch_name + "_" + QString::number(123) + ".dat";
	bool useTwoCUDA = m_config[DLIB].toObject()["UseTwoCUDA"].toBool();

	m_outputNetworkFileName = m_dnnFolderTestCase + m_dnnName + "_" + QString::number(m_dronNoise) + "_" + QString::number(m_dronContrast) + "_" + QString::number(m_nowTime) + ".dnn";

	#ifdef DEBUG
	Logger->debug("Dlib::configure() m_outputNetworkFileName:{}", m_outputNetworkFileName.toStdString());
	Logger->debug("Dlib::configure() test network...");
	#endif
	Logger->debug("Dlib::configure() m_outputNetworkFileName:{}", m_outputNetworkFileName.toStdString());
	dlib::set_dnn_prefer_smallest_algorithms();

	net_type segb;
	//net_type2 segc;
	m_epoch_counter = 0;
	for(int i = 0 ; i < 2 ; i++)
	{
		segb = this->train_segmentation_network(list);

		dlib::serialize(m_outputNetworkFileName.toStdString().c_str()) << segb;
		
		//dlib::deserialize(m_outputNetworkFileName.toStdString().c_str()) >> segb;
		if (m_currentLearningRate <= 0.0000001)
		{
			break;
		}
		else
		{
			Logger->debug("m_currentLearningRate:{}",m_currentLearningRate);
		}
	}

	DlibNetwork::testNetwork("test", segb,(m_configPath+m_cleanTestPath), (m_configPath+m_gtTestPath), m_fileLoggerTest);
	#ifdef DEBUG
	Logger->debug("DlibNetwork::onTrainNetwork() done");
	#endif
}

void DlibNetwork::testNetwork(QString id, net_type segb, QString clean, QString gt, FileLogger* fileLogger)
{
	dlib::matrix<unsigned char> input_image;
	dlib::matrix<unsigned char> label_image;

	#ifdef DEBUG
	Logger->debug("DlibNetwork::testNetwork() get_files_in_directory_tree()");
	#endif

	// Find supported image files.
	const std::vector<dlib::file> files = dlib::get_files_in_directory_tree(clean.toStdString(), dlib::match_endings(".jpeg .jpg .png"));
	const std::vector<dlib::file> files_label = dlib::get_files_in_directory_tree(gt.toStdString(), dlib::match_endings(".jpeg .jpg .png"));

	dlib::rand rnd;

	std::cout << "Found " << files.size() << " " << id.toStdString() << " images, processing..." << std::endl;
	#ifdef DEBUG
	Logger->debug("DlibNetwork::loadNetwork() for loop:");
	#endif
	int num_right = 0;
	int num_wrong = 0;
	for(int i = 0 ; i < m_postprocess.size() ; i++)
	{
		if(m_postprocess[i].toObject()[NAME].toString() == ENCODER)
		{
			QJsonObject obj = m_postprocess[i].toObject();
			obj[CONFIG] = m_postprocess[i].toObject()["Config2"];
			QJsonObject config = obj[CONFIG].toObject();
			m_fileName = m_videoLogsTestCase + "mp4_" + id + "_" + QString::number(m_dronNoise) + "_" + 
								QString::number(m_dronContrast) + "_" + QString::number(m_nowTime);
			Logger->debug("Dlib::configure() video:{}", m_fileName.toStdString());
			config["Path"] = m_fileName;
			obj[CONFIG] = config;
			m_postprocess[i] = obj;
			#ifdef DEBUG
			qDebug() << "m_postprocess[i][CONFIG]:" << m_postprocess[i].toObject()[CONFIG];
			#endif

/*
			QJsonObject obj = m_postprocess[i].toObject();
			QJsonObject config = obj[CONFIG].toObject();
			
			// for video files:
			m_fileName = m_videoLogsFolder + m_graphType + m_split + m_dronType + m_split + m_boundType + m_split +
			id + "_" + QString::number(m_dronNoise) + "_" + QString::number(m_dronContrast) + "_" + QString::number(m_nowTime);
			config["Path"] = m_fileName;
			obj[CONFIG] = config;
			m_postprocess[i] = obj;*/
		}
	}

	m_graph_postprocessing.loadGraph(m_postprocess, m_blockPostprocess);

	for(int i = 0 ; i < files.size() ; i++)
	{
		#ifdef DEBUG
		//Logger->debug("DlibNetwork::loadNetwork() Load the input image:");
		#endif
		load_image(input_image, files[i].full_name());
		load_image(label_image, files_label[i].full_name());
		const auto nr = input_image.nr();
		const auto nc = input_image.nc();

		#ifdef DEBUG
		//Logger->debug("DlibNetwork::loadNetwork() instances nr:{} nc:{}", input_image.nr(), input_image.nc());
		//Logger->debug("DlibNetwork::loadNetwork() instances nr:{} nc:{}", label_image.nr(), label_image.nc());
		#endif
		
		dlib::matrix<float> instances = segb(input_image);
		cv::Mat labelMat = dlib::toMat(label_image);
		cv::Mat inputMat = dlib::toMat(input_image);

		dlib::matrix<unsigned char> instancesChar(instances.nr(), instances.nc());

		for (long r = 0; r < instances.nr(); ++r)
		{
			for (long c = 0; c < instances.nc(); ++c)
			{
				const auto& index = instances(r, c);
				if (index <= 0)
					instancesChar(r, c) = 0;
				else
					instancesChar(r, c) = 255;
			}
		}

		cv::Mat outputMat = dlib::toMat(instancesChar);

		m_outputData.clear();
		m_outputData.push_back(outputMat.clone());
		m_outputData.push_back(labelMat.clone());
		m_outputData.push_back(inputMat.clone());

		DlibNetwork::postprocessing();
	}

	struct fitness fs = DlibNetwork::finishPostProcessing();
	DlibNetwork::logPopulation(id, fs, fileLogger);

	#ifdef DEBUG
	Logger->debug("DlibNetwork::loadNetwork() for loop end:");
	#endif
}

void DlibNetwork::logPopulation(QString id, fitness fs, FileLogger * fileLogger)
{
	m_timer.stop();
	Logger->info(
		"ID:{} B:{:f} (fn:{},fp:{},tn:{},tp:{}) time:{:3.0f}[ms] loss:{:f}",
		m_epoch_counter,
		fs.fitness,
		fs.fn, fs.fp,
		fs.tn, fs.tp,
		m_timer.getTimeMilli(),
		m_averageLoss);

	QStringList list;
	list.push_back(QString::number(m_epoch_counter) + " ");
	list.push_back(QString().setNum(fs.fitness, 'f', 4) + " ");
	list.push_back(QString::number(fs.fn) + " ");
	list.push_back(QString::number(fs.fp) + " ");
	list.push_back(QString::number(fs.tn) + " ");
	list.push_back(QString::number(fs.tp) + " ");
	list.push_back(QString().setNum(m_timer.getTimeMilli(), 'f', 0) + " ");
	list.push_back(QString::number(m_averageLoss));
	list.push_back("\n");
	fileLogger->onAppendFileLogger(list);
	m_timer.start();
}

void DlibNetwork::postprocessing()
{
	// POSTPROCESSING:
	m_dataPostprocess.clear();

	#ifdef DEBUG_POSTPROCESSING
	Logger->debug("DlibNetwork::postprocessing() m_postprocess.size():{}", m_postprocess.size());
	#endif
	for (int i = 0; i < m_postprocess.size(); i++)
	{
		#ifdef DEBUG_POSTPROCESSING
		qDebug() << "DlibNetwork::postprocessing() m_postprocess[i]:" << m_postprocess[i];
		for (int z = 0; z < m_dataPostprocess.size(); z++)
		{
			for (int zz = 0; zz < m_dataPostprocess[z].size(); zz++)
			{
				Logger->debug("post [{}][{}].():{}", z, zz, m_dataPostprocess[z][zz].processing.cols);
			}
		}
		#endif

		std::vector<_postData> dataVec;
		const QJsonObject _obj = m_postprocess[i].toObject();
		const QJsonArray _prevActive = _obj[PREV].toArray();
		const QJsonArray _nextActive = _obj[NEXT].toArray();
		#ifdef DEBUG_POSTPROCESSING
		Logger->debug("DlibNetwork::postprocessing() postprocess i:{}, _prevActive.size:{}", i, _prevActive.size());
		#endif
		if (m_graph_postprocessing.checkIfLoadInputs(_prevActive, dataVec, m_outputData, i))
		{
			m_graph_postprocessing.loadInputs(_prevActive, dataVec, m_postprocess, m_dataPostprocess);				
		}
		try
		{
			#ifdef DEBUG_POSTPROCESSING
			Logger->debug("DlibNetwork::postprocessing() postProcessing: block[{}]->process", i);
			#endif
			m_blockPostprocess[i]->process((dataVec));
		}
		catch (cv::Exception& e)
		{
			const char* err_msg = e.what();
			qDebug() << "exception caught: " << err_msg;
		}
		m_dataPostprocess.push_back((dataVec));
		//m_postTime += m_blockPostprocess[i]->getElapsedTime();
	}
}

fitness DlibNetwork::finishPostProcessing()
{
	#ifdef DEBUG_POSTPROCESSING
	Logger->debug("DlibNetwork::finishPostProcessing() Calculate fitness:");
	#endif
	struct fitness fs{0,0,0,0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

	for (int i = 0; i < m_postprocess.size(); i++)
	{
		const QJsonObject _obj = m_postprocess[i].toObject();
		QString _type = _obj[TYPE].toString();
		if (_type == "Fitness")
		{
			#ifdef DEBUG_POSTPROCESSING
			Logger->debug("DlibNetwork::finishPostProcessing() Calculate Fitness endProcess:");
			#endif
			m_blockPostprocess[i]->endProcess(m_dataPostprocess[i]);
			fs = m_dataPostprocess[i][0].fs;
			#ifdef DEBUG_POSTPROCESSING
			Logger->debug("DlibNetwork::finishPostProcessing() fs:{}", fs.fitness);
			#endif
		}
		if (_type == "Encoder")
		{
			#ifdef DEBUG_POSTPROCESSING
			Logger->debug("DlibNetwork::finishPostProcessing() Calculate Encoder endProcess:");
			#endif
			m_blockPostprocess[i]->endProcess(m_dataPostprocess[i]);
		}
	}
	#ifdef DEBUG_POSTPROCESSING
	for (int z = 0; z < m_dataPostprocess.size(); z++)
	{
		for (int zz = 0; zz < m_dataPostprocess[z].size(); zz++)
		{
			Logger->debug("m_dataPostprocess [{}][{}].():{}", z, zz, m_dataPostprocess[z][zz].processing.cols);
			Logger->debug("m_dataPostprocess [{}][{}].():{}", z, zz, m_dataPostprocess[z][zz].testStr.toStdString());
		}
	}
	#endif
	return fs;
}


truth_instance DlibNetwork::load_truth_instances(const image_info& info)
{
	#ifdef DEBUG
	Logger->debug("DlibNetwork::load_truth_instances({},{})", info.image_filename, info.gt_filename);
	#endif

	dlib::matrix<unsigned char> input_image;
	dlib::matrix<unsigned char> label_image;

	load_image(input_image, info.image_filename);
	load_image(label_image, info.gt_filename);

	const auto nr = label_image.nr();
	const auto nc = label_image.nc();
	dlib::matrix<float> result(nr, nc);

	for (long r = 0; r < nr; ++r)
	{
		for (long c = 0; c < nc; ++c)
		{
			const auto& index = label_image(r, c);
			if (index == 0)
				result(r, c) = -1;
			else
				result(r, c) = index / 255.0;
		}
	}
	truth_instance ti{ input_image , result };
	#ifdef DEBUG
	Logger->debug("DlibNetwork::load_truth_instances({}x{})", nr, nc);
	#endif

	return ti;
}

void DlibNetwork::process(){}


void DlibNetwork::onSignalOk(struct fitness fs, qint32 slot)
{
}

void DlibNetwork::clearData()
{
}

net_type DlibNetwork::train_segmentation_network(const std::vector<truth_instance>& truth_images)
{
	m_timer.start();
	net_type seg_net;

	#ifdef DEBUG
		std::cout << "The net has " << seg_net.num_layers << " layers in it." << std::endl;
		std::cout << "seg_net:" << seg_net << std::endl;
	#endif

	#ifdef DEBUG
	Logger->debug("DlibNetwork::train_segmentation_network()");
	Logger->debug("truth_images.size:{}", truth_images.size());
	#endif


	int imageSize = m_imageSize;
	const std::string synchronization_file_name = m_synchronizationNetworkFileName.toStdString();
	#ifdef DEBUG
	Logger->debug("DlibNetwork::train_segmentation_network() synchronization_file_name:{}", m_synchronizationNetworkFileName.toStdString());
	#endif
	std::vector<int> cuda{ 0 };
	if (m_useTwoCuda)
	{
		#ifdef DEBUG
		Logger->debug("DlibNetwork::train_segmentation_network() useTwoCUDA");
		#endif
		cuda.push_back(1);
	}
	#ifdef DEBUG
	Logger->debug("DlibNetwork::train_segmentation_network() create seg_trainer:");
	#endif

	const double weight_decay = 0.0001;
	const double momentum = 0.9;
	dlib::dnn_trainer<net_type> seg_trainer(seg_net, dlib::sgd(weight_decay, momentum), cuda);
	#ifdef DEBUG
	Logger->debug("DlibNetwork::train_segmentation_network() setting seg_trainer:");
	#endif

	seg_trainer.set_learning_rate_shrink_factor(0.1);
	seg_trainer.set_iterations_without_progress_threshold(1000);
	seg_trainer.set_mini_batch_size(m_minBatchSize);
	seg_trainer.set_max_num_epochs(m_maxNumEpochs);
	seg_trainer.be_verbose();
	seg_trainer.set_learning_rate(m_learningRate);
	//seg_trainer.set_synchronization_file(synchronization_file_name, std::chrono::minutes(10));

	#ifdef DEBUG
	std::cout << seg_trainer << std::endl;
	std::cout << "seg_net:" << seg_net << std::endl;
	#endif

	std::ofstream out(m_infoName.toStdString()+".txt");
	out << "The net has " << seg_net.num_layers << " layers in it." << std::endl;
	out << "seg_net:\n" << seg_net<< std::endl;
	out << "seg_trainer:\n" << seg_trainer << std::endl;
	out.close();

	QJsonObject json{{"Config", m_config[DLIB].toObject()}};
	m_fileLoggerJSON->onLogJsonBest(json);

	std::vector<dlib::matrix<unsigned char>> samples;
	std::vector<dlib::matrix<float>> labels;

	// Start a bunch of threads that read images from disk and pull out random crops.  It's
	// important to be sure to feed the GPU fast enough to keep it busy.  Using multiple
	// thread for this kind of data preparation helps us do that.  Each thread puts the
	// crops into the data queue.
	dlib::pipe<seg_training_sample> data(m_queqe);

	#ifdef DEBUG
	Logger->debug("DlibNetwork::train_segmentation_network() loop:");
	#endif

	auto f = [&data, &truth_images, imageSize](time_t seed)
	{
		dlib::rand rnd(time(0) + seed);
		dlib::matrix<unsigned char> input_image;
		dlib::matrix<float> label_image;

		seg_training_sample temp;
		while (data.is_enabled())
		{
			const auto random_index = rnd.get_random_32bit_number() % truth_images.size();

			const auto& input_image = truth_images[random_index].input_image;
			const auto label_image = truth_images[random_index].label_image;

			dlib::matrix<unsigned char> inputDlib = truth_images[random_index].input_image;
			dlib::matrix<float> labelDlib = truth_images[random_index].label_image;

			cv::Mat inputMat = dlib::toMat(inputDlib);
			cv::Mat labelMat = dlib::toMat(labelDlib);

			const auto randomx = rnd.get_random_32bit_number() % (inputMat.cols - imageSize - 1);
			const auto randomy = rnd.get_random_32bit_number() % (inputMat.rows - imageSize - 1);

			cv::Rect randomRect(randomx, randomy, imageSize, imageSize);
			cv::Mat croppedInputMat = inputMat(randomRect);
			cv::Mat croppedLabelMat = labelMat(randomRect);

			dlib::cv_image<unsigned char> input_dlib_img(croppedInputMat);
			dlib::matrix<unsigned char> inputMatrixDlib;
			dlib::assign_image(inputMatrixDlib, input_dlib_img);

			dlib::cv_image<float> label_dlib_img(croppedLabelMat);
			dlib::matrix<float> labelMatrixDlib;
			dlib::assign_image(labelMatrixDlib, label_dlib_img);

			temp.input_image = inputMatrixDlib;
			temp.label_image = labelMatrixDlib;

			// Push the result to be used by the trainer.
			data.enqueue(temp);
		}
	};

	#ifdef DEBUG
	Logger->debug("DlibNetwork::train_segmentation_network() start threads... ");
	#endif

	std::thread data_loader1([f]() { f(1); });
#if(THREADS >= 4)
	std::thread data_loader2([f]() { f(2); });
	std::thread data_loader3([f]() { f(3); });
	std::thread data_loader4([f]() { f(4); });
#endif
#if(THREADS >= 8)
	std::thread data_loader5([f]() { f(5); });
	std::thread data_loader6([f]() { f(6); });
	std::thread data_loader7([f]() { f(7); });
	std::thread data_loader8([f]() { f(8); });
#endif
#if(THREADS >= 16)
	std::thread data_loader9([f]() { f(9); });
	std::thread data_loader10([f]() { f(10); });
	std::thread data_loader11([f]() { f(11); });
	std::thread data_loader12([f]() { f(12); });
	std::thread data_loader13([f]() { f(13); });
	std::thread data_loader14([f]() { f(14); });
	std::thread data_loader15([f]() { f(15); });
	std::thread data_loader16([f]() { f(16); });
#endif
#if(THREADS >= 32)
	std::thread data_loader17([f]() { f(17); });
	std::thread data_loader18([f]() { f(18); });
	std::thread data_loader19([f]() { f(19); });
	std::thread data_loader20([f]() { f(20); });
	std::thread data_loader21([f]() { f(21); });
	std::thread data_loader22([f]() { f(22); });
	std::thread data_loader23([f]() { f(23); });
	std::thread data_loader24([f]() { f(24); });
	std::thread data_loader25([f]() { f(25); });
	std::thread data_loader26([f]() { f(26); });
	std::thread data_loader27([f]() { f(27); });
	std::thread data_loader28([f]() { f(28); });
	std::thread data_loader29([f]() { f(29); });
	std::thread data_loader30([f]() { f(30); });
	std::thread data_loader31([f]() { f(31); });
	std::thread data_loader32([f]() { f(32); });
#endif

	const auto stop_data_loaders = [&]()
	{
		data.disable();
		data_loader1.join();
#if(THREADS >= 4)
		data_loader2.join();
		data_loader3.join();
		data_loader4.join();
#endif
#if(THREADS >= 8)
		data_loader5.join();
		data_loader6.join();
		data_loader7.join();
		data_loader8.join();
#endif
#if(THREADS >= 16)
		data_loader9.join();
		data_loader10.join();
		data_loader11.join();
		data_loader12.join();
		data_loader13.join();
		data_loader14.join();
		data_loader15.join();
		data_loader16.join();
#endif
#if(THREADS >= 32)
		data_loader17.join();
		data_loader18.join();
		data_loader19.join();
		data_loader20.join();
		data_loader21.join();
		data_loader22.join();
		data_loader23.join();
		data_loader24.join();
		data_loader25.join();
		data_loader26.join();
		data_loader27.join();
		data_loader28.join();
		data_loader29.join();
		data_loader30.join();
		data_loader31.join();
		data_loader32.join();
#endif
	};
	Logger->debug("start learning...");
	try
	{
		// The main training loop.  Keep making mini-batches and giving them to the trainer.
		// We will run until the learning rate has dropped by a factor of 1e-4.
		Logger->debug("seg_trainer.get_learning_rate():{}", seg_trainer.get_learning_rate());
		
		//while (seg_trainer.get_learning_rate() >= 0.00000001)
		while (seg_trainer.get_learning_rate() >= 0.00000001)
		{
			m_currentLearningRate = seg_trainer.get_learning_rate();
			m_epoch_counter++;
			samples.clear();
			labels.clear();
			// make a mini-batch
			seg_training_sample temp;
			while (samples.size() < m_minBatchSize)
			{
				data.dequeue(temp);
				samples.push_back(std::move(temp.input_image));
				labels.push_back(std::move(temp.label_image));
			}
			seg_trainer.train_one_step(samples, labels);
			if(m_epoch_counter >= 20000)
			{
				break;
			}
		}
	}
	catch (std::exception& e)
	{
		std::cout << "std::exception&:" << e.what() << std::endl;
		Logger->error("catch (std::exception&)!!");
		stop_data_loaders();
		seg_trainer.get_net();
		seg_net.clean();
		throw;
	}
	m_averageLoss = seg_trainer.get_average_loss();
	Logger->debug("stop_data_loaders() get_average_loss:{}, get_average_test_loss:{}", seg_trainer.get_average_loss(), seg_trainer.get_average_test_loss());
	// Training done, tell threads to stop and make sure to wait for them to finish before moving on.
	stop_data_loaders();
	// also wait for threaded processing to stop in the trainer.
	seg_trainer.get_net();
	seg_net.clean();
	m_timer.stop();

	#ifdef DNN_1LAYERS_20CON_05DROPOUT
		//dlib::layer<4>(seg_net).layer_details() = dlib::multiply_(0.5);
	#endif
	return seg_net;
}