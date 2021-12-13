#include "widgets/view/painter.h"
#include "widgets/view/rect.h"

#define DEBUG

constexpr auto ROI{ "ROI" };
constexpr auto NAME{ "Name" };
constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto X{ "X" };
constexpr auto Y{ "Y" };
constexpr auto P1X{ "p1x" };
constexpr auto P1Y{ "p1y" };
constexpr auto P2X{ "p2x" };
constexpr auto P2Y{ "p2y" };
constexpr auto SIZE_ROI{ "Size" };


Painter::Painter(QJsonObject const& config, GraphicsScene *graphicsScene, GraphicsView* graphicsView, DataMemory* dataMemory)
	: m_config(config)
	, m_graphicsScene(graphicsScene)
	, m_graphicsView(graphicsView)
	, m_dataMemory(dataMemory)
{
	#ifdef _WIN32
	m_split = "\\";
	#endif // _WIN32
	#ifdef __linux__
	m_split = "/";
	#endif // _UNIX
	m_roiType = 4;
	m_paintType = 5;
	m_imageType = 6;
	m_dlibType = 10;
    m_painterSettings.configureColors(m_config);
	m_graphicsView->setPainterSettings(&m_painterSettings);
}

void Painter::onPaintOnBoard(qint32 x, qint32 y)
{
	#ifdef DEBUG
	Logger->debug("Painter::onPaintOnBoard()");
	#endif
	onPaintOnBoardInColor(x, y, m_painterSettings.m_color);
}

void Painter::onPaintBackground(qint32 x, qint32 y)
{
	#ifdef DEBUG
	Logger->debug("Painter::onPaintBackground()");
	#endif
	onPaintOnBoardInColor(x, y, m_painterSettings.m_colorBackground);
}

void Painter::onPaintOnBoardInColor(qint32 x, qint32 y, QColor color)
{
	#ifdef DEBUG
	Logger->debug("Painter::onPaintOnBoardInColor()");
	#endif
	if (m_painterSettings.m_penSize == 1)
	{
		m_paintImage.setPixelColor(x, y, color);
	}
	else if (m_painterSettings.m_penSize > 1 && m_painterSettings.m_penSize < 20)
	{
		qint32 it = m_painterSettings.m_penSize - 1;
		for (int zx = -it; zx <= it; zx++)
		{
			for (int zy = -it; zy <= it; zy++)
			{
				m_paintImage.setPixelColor(x + zx, y + zy, color);
			}
		}
	}
	else
	{
		Logger->error("Painter::onPaintOnBoard() painter not select");
	}
	m_paintPixmap->setPixmap(QPixmap::fromImage(m_paintImage));
}

void Painter::onLoadImage(int id)
{
	m_id = id;
	m_name = m_dataMemory->m_names[id];
	QString m_fileNameWithPath = m_dataMemory->m_paths[id] + m_dataMemory->m_filenames[id];
	Painter::clearScene();
	#ifdef DEBUG
	Logger->debug("Painter::onLoadImage({})",m_fileNameWithPath.toStdString());
	#endif

	QPixmap test;
	test.load(m_fileNameWithPath);
	addImageToScene(test);
}

void Painter::onLoadRois(int id)
{
	QString _fileNameWithPath = m_dataMemory->m_roiPaths[id]+m_dataMemory->m_roiNames[id];
	#ifdef DEBUG
	Logger->debug("ImageLoader::loadRois() _fileNameWithPath:{}", _fileNameWithPath.toStdString());
	#endif
	std::unique_ptr<ConfigReader> cR = std::make_unique<ConfigReader>();

	QJsonObject jObject;
	if (!cR->readConfig(_fileNameWithPath, jObject))
	{
		spdlog::error("File {} read config failed", _fileNameWithPath.toStdString());
		emit(updatStatus(m_id, "ROI not loaded"));
	}
	else
	{
		QJsonArray jROI = jObject[ROI].toArray();
		#ifdef DEBUG
		Logger->debug("ImageLoader::loadRois() jROI.size():{}", jROI.size());
		#endif
		Painter::addRoisToScene(jROI);
		//Painter::addToRoiWidget(jROI);
		emit(updatStatus(m_id, "OK"));
	}
}

void Painter::addToRoiWidget(QJsonArray array)
{
	#ifdef DEBUG
	Logger->debug("Painter::addToRoiWidget()");
	#endif
	int counter{0};
	for(auto& arr : array)
	{
		QJsonObject arrObj = arr.toObject();
		QString name = arrObj [NAME].toString();
		int x = arrObj[X].toInt();
		int y = arrObj[Y].toInt();
		int w = arrObj[WIDTH].toInt();
		int h = arrObj[HEIGHT].toInt();
		//TODO: p1x, p1y, p2x, p2y Bounding box
		int size = w*h;
		emit(addRoi(counter,name,size,"enable"));
		counter++;
	}
}

void Painter::clearScene()
{
	QList<QGraphicsItem*> items = m_graphicsScene->items(Qt::DescendingOrder);
	#ifdef DEBUG
	Logger->debug("Painter::clearScene() size:{}", items.size());
	#endif
	for (int i = 0; i < items.size(); i++)
	{
		#ifdef DEBUG
		Logger->debug("Painter::clearScene() type:{}", items[i]->type());
		#endif
		if (items[i]->type() == m_imageType || items[i]->type() == m_paintType)
		{
			GraphicsPixmapItem* cast = dynamic_cast<GraphicsPixmapItem*>(items[i]);
			m_graphicsScene->removeItem(cast);
			delete cast;
		}
	}
	m_graphicsScene->clear();
}

static cv::Mat qimage_to_mat_ref(QImage &img, int format)
{
    return cv::Mat(img.height(), img.width(), format, img.bits(), img.bytesPerLine());
}

void Painter::deleteRois()
{
	QList<QGraphicsItem*> items = m_graphicsScene->items(Qt::DescendingOrder);
	#ifdef DEBUG
	Logger->debug("Painter::deleteRois() size:{}", items.size());
	#endif
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->type() == m_roiType) // Just ROI object:
		{
			GraphicsRectItem* cast = dynamic_cast<GraphicsRectItem*>(items[i]);
			m_graphicsScene->removeItem(cast);
			delete cast;
		}
	}
	emit(clearRoiWidget());
}

void Painter::onSetCurrentPaintFolder(QString imageFolder, QString paintFolder, QString jsonDirectory)
{
	m_currentDirectory = imageFolder;
	m_currentPaintDirectory = paintFolder;
	m_currentJsonDirectory = jsonDirectory;
}

void Painter::onUpdateFile()
{
	emit(updateFileFromId(m_id));
}

void Painter::onSaveRois()
{
	
	const QString m_fileNameWithPath = m_dataMemory->m_roiPaths[m_id] + m_dataMemory->m_roiNames[m_id];
	#ifdef DEBUG
	Logger->debug("View::onSaveRois() m_fileNameWithPath:{}",m_fileNameWithPath.toStdString());
	#endif
	QJsonArray _ROIArray;
	std::vector<QJsonArray> _arrays;

	for (qint32 color = 0; color < m_painterSettings.m_colors.size(); color++)
	{
		_arrays.push_back(QJsonArray());
	}

	QList<QGraphicsItem*> items = m_graphicsScene->items(Qt::DescendingOrder);
	#ifdef DEBUG
	Logger->debug("View::onSaveRois() size:{}", items.size());
	#endif
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->type() == m_roiType)
		{
			QRectF rect = items[i]->boundingRect();
			QPointF pos = items[i]->pos();

			int x = rect.x() + 1;
			int y = rect.y() + 1;
			int width = rect.width() - 1;
			int height = rect.height() - 1;
			int sizeRoi = qAbs(width / 2) * qAbs(height / 2);

			QSize size = QSize(width, height);
			#ifdef DEBUG
			Logger->debug("items size:{}x{}x{}x{}", x, y, size.width(), size.height());
			#endif
		
			GraphicsRectItem* cast = dynamic_cast<GraphicsRectItem*>(items[i]);
			bool b_saveFlag = true;
			if(!cast->isEnabled())
			{
				b_saveFlag = false;
			}
			
			for (int i = 0 ; i < m_painterSettings.m_colors_background.size() ; i++)
			{	
			}
			if(b_saveFlag)
			{
				QJsonObject obj{ { X, x },
					{ Y, y },
					{ WIDTH, width },
					{ HEIGHT, height },
					{ NAME, cast->text() },
					{ SIZE_ROI, sizeRoi } };
				_ROIArray.append(obj);
			}
		}
	}
	#ifdef DEBUG
	qDebug() << "_ROIArray:" << _ROIArray;
	#endif
	QJsonObject json{};
	json.insert(ROI, _ROIArray);
	auto test = QJsonDocument(json).toJson(QJsonDocument::Indented);
	QFile jsonFile(QString::fromStdString(m_fileNameWithPath.toStdString()));
	jsonFile.open(QFile::WriteOnly);
	jsonFile.write(test);
	jsonFile.close();

	QString possibleError;
	QJsonArray jROI = json[ROI].toArray();
	#ifdef DEBUG
	qDebug() << "jROI:" << jROI;
	Logger->debug("View::onSaveRois() done");
	#endif
}

QImage Painter::cvMatToQImage(const cv::Mat &input)
{
	#ifdef DEBUG
	Logger->debug("View::cvMatToQImage()");
	#endif
	switch ( input.type() )
	{
		case CV_8UC1:
		{
			QImage image( input.data,
			input.cols, input.rows,
			static_cast<int>(input.step),
			QImage::Format_Grayscale8 );
			return image;
		}
	}
}

cv::Mat Painter::QImageToCvMat( const QImage &inImage, bool inCloneImageData = true )
{
	#ifdef DEBUG
	Logger->debug("View::QImageToCvMat()");
	#endif
	switch ( inImage.format() )
	{
		// 8-bit, 1 channel
		case QImage::Format_Indexed8:
		{
			#ifdef DEBUG
			Logger->debug("View::QImageToCvMat() case QImage::Format_Indexed8:");
			#endif
			cv::Mat  mat( inImage.height(), inImage.width(),
			CV_8UC1,
			const_cast<uchar*>(inImage.bits()),
			static_cast<size_t>(inImage.bytesPerLine())
			);

			return (inCloneImageData ? mat.clone() : mat);
		}
		case QImage::Format_Grayscale8:
		{
			return cv::Mat(inImage.height(), inImage.width(), CV_8UC1, 
                   const_cast<uchar*>(inImage.bits()), 
                   inImage.bytesPerLine()).clone();
		}

	}
}

void Painter::onUseNetwork()
{
	//TODO 
	// find selected roi
	// crop it from image
	// emit it via signal to dlibNetwork
	//emit(useNetwork());
	QList<QGraphicsItem*> items = m_graphicsScene->items(Qt::DescendingOrder);
	#ifdef DEBUG
	Logger->debug("View::onSaveRois() size:{}", items.size());
	#endif

	std::vector<cv::Mat> imageToNetwork;

	for (auto& item : items)
	{
		if (item->type() == m_dlibType)
		{
			QRectF rect = item->boundingRect();
			QPointF pos = item->pos();

			int x = rect.x() + 1;
			int y = rect.y() + 1;
			int width = rect.width() - 1;
			int height = rect.height() - 1;
			int sizeRoi = qAbs(width / 2) * qAbs(height / 2);
			QRect simpleRect{x,y,width,height};

			QSize size = QSize(width, height);
			#ifdef DEBUG

			QImage grayscaleImage = m_image.convertToFormat(QImage::Format_Grayscale8);
			QImage::Format format = grayscaleImage.format();
			qDebug() << "grayscaleImage format:" << format;
			Logger->debug("items size:{}x{}x{}x{}", x, y, size.width(), size.height());
			#endif
			//TODO m_image jest kolorowe:!
			QImage roiImage = grayscaleImage.copy(x,y,width,height);

			#ifdef DEBUG
			QImage::Format format2 = roiImage.format();
			qDebug() << "roiImage format:" << format2;
			#endif

			cv::Mat roiCvMat = QImageToCvMat(roiImage);

			#ifdef DEBUG
			qDebug() << "roiCvMat channels:" << roiCvMat.channels();
			qDebug() << "roiCvMat cols:" << roiCvMat.cols;
			qDebug() << "roiCvMat rows:" << roiCvMat.rows;
			#endif

			//cv::imshow("roiCvMat",roiCvMat);
			//cv::waitKey(0);
			emit(useNetwork(roiCvMat, simpleRect));
			//GraphicsRectItem* cast = dynamic_cast<GraphicsRectItem*>(item);
		}
	}
}

void Painter::onReturnUsedNetwork(cv::Mat image, QRect rect)
{
	#ifdef DEBUG
	Logger->debug("Painter::onReturnUsedNetwork()");
	#endif
	//cv::Mat& image = m_dataMemory->gt(id);
	
	#ifdef DEBUG
	Logger->debug("Painter::onReturnUsedNetwork() image ({}x{}x{})", image.cols, image.rows, image.channels());
	#endif
	if(image.empty())
	{
		Logger->error("Painter::onReturnUsedNetwork() image cant be loaded");
		return;
	}
	else
	{
		if(image.channels() > 1)
		{
			cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
		}
		
		for (qint32 color = 0; color < m_painterSettings.m_colors.size(); color++)
		{
			int ii = 0;
			for (int i = rect.x(); i < (rect.x()+rect.width()); i++, ii++)
			{
				//if(i>rect.x() && i<(rect.x()+rect.width()))
					int jj = 0;
					for (int j = rect.y(); j < (rect.y()+rect.height()); j++, jj++)
					{
						//if(i>rect.y() && i<(rect.y()+rect.height()))
							if ( m_painterSettings.m_colorIntHash[m_painterSettings.m_colors[color]] == image.at<unsigned char>(jj, ii))
							{
								onPaintColors(i, j, m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]]);
							}
					}
			}
		}
	}
	Painter::onPaintColorsFinish();
	#ifdef DEBUG
	Logger->debug("Painter::onReturnUsedNetwork() done");
	#endif
}

void Painter::onSavePaint()
{
	#ifdef DEBUG
	Logger->debug("View::onSavePaint()");
	#endif
	cv::Mat cleanData = cv::Mat(m_paintImage.height(), m_paintImage.width(), CV_8UC1, cv::Scalar(0));

	int counter{0};

	for (int i = 0; i < m_painterSettings.m_colors.size(); i++)
	{
		QString color = m_painterSettings.m_colors[i];
		for (int i = 0; i < m_paintImage.width(); i++)
		{
			for (int j = 0; j < m_paintImage.height(); j++)
			{
				QGraphicsItem* item = m_graphicsScene->itemAt(QPointF(i, j), QTransform());
				if (item->type() == m_roiType && item->isEnabled())
				{
					if (m_painterSettings.m_colorHash[color].rgb() == m_paintImage.pixel(i, j))
					{
						counter++;
						cleanData.at<unsigned char>(j,i) = m_painterSettings.m_colorIntHash[color];
					}
				}
			}
		}
	}
	m_dataMemory->m_gt[m_id] = cleanData;
	const QString m_fileNameWithPath = m_dataMemory->gtPaths(m_id) + m_dataMemory->filenames(m_id);
	cv::imwrite(m_fileNameWithPath.toStdString(), cleanData);
	#ifdef DEBUG
	Logger->debug("View::onSavePaint(): m_fileNameWithPath:{}", m_fileNameWithPath.toStdString());
	#endif
}

void Painter::onCreateRois()
{
	Painter::deleteRois();
	QJsonArray _array;
	#ifdef DEBUG
	Logger->debug("Painter::onCreateRois()");
	#endif
	for (int color = 0; color < m_painterSettings.m_colors_foreground.size(); color++)
	{
		#ifdef DEBUG
		Logger->debug("Painter::onCreateRois() color:{}", color);
		qDebug() <<m_painterSettings.m_colors_foreground[color];
		qDebug() <<m_painterSettings.m_colorHash[m_painterSettings.m_colors_foreground[color]];
		#endif
		

		cv::Mat cleanData = cv::Mat(m_paintImage.height(), m_paintImage.width(), CV_8UC1,cv::Scalar(0));
		for (int i = 0; i < m_paintImage.width(); i++)
		{
			for (int j = 0; j < m_paintImage.height(); j++)
			{
				if ( m_painterSettings.m_colorHash[m_painterSettings.m_colors_foreground[color]].rgb() == m_paintImage.pixel(i,j))
				{
					cleanData.at<unsigned char>(j,i) = 255;
				}
			}
		}
		QJsonArray contoursArray{};
		m_contour.findContours(cleanData, contoursArray, m_painterSettings.m_colors_foreground[color]);
		for(int i = 0 ; i < contoursArray.size() ; i++)
		{
			_array.append(contoursArray[i]);
		}

		QString _name = QString::number(color) + "_CrateRois.png";
		cv::imwrite(_name.toStdString(), cleanData);
	}
	Painter::addRoisToScene(_array);
	#ifdef DEBUG
	Logger->debug("Painter::onCreateRois() done");
	#endif
}

void Painter::addRoisToScene(QJsonArray contoursArray)
{
	#ifdef DEBUG
	Logger->debug("Painter::addRoisToScene(contoursArray) contoursArray.size:{}", contoursArray.size());
	#endif

	emit(clearList());
	int id{0};
	for (unsigned int i = 0; i < contoursArray.size(); i++)
	{
		QJsonObject obj = contoursArray[i].toObject();
		QString name = obj[NAME].toString();
		int size = obj[SIZE_ROI].toInt();
		QRectF tempRectToText = QRectF(obj[X].toInt(), obj[Y].toInt(), obj[WIDTH].toInt(),obj[HEIGHT].toInt());
		QColor _color = m_painterSettings.m_colorHash[name];
		
		#ifdef DEBUG
		Logger->debug("Painter::addRoisToScene()  add GraphicsRectItem:{} name:{}", i, name.toStdString());
		qDebug() << "tempRectToText:" << tempRectToText;
		qDebug() << "color:" << _color;
		#endif
		
		GraphicsRectItem * itemG = new GraphicsRectItem(_color, name, tempRectToText, m_roiType, id);
		m_graphicsScene->addItem(itemG);
		emit(addRoi(id, name, size, true));
		id++;
	}
	#ifdef DEBUG
	Logger->debug("Painter::addRoisToScene() done");
	#endif
}

void Painter::onRoiItemSelectionChanged()
{
	#ifdef DEBUG
	Logger->debug("Painter::onRoiItemSelectionChanged()");
	#endif
}

void Painter::onRoiItemChanged(QTreeWidgetItem *item, int column)
{
	#ifdef DEBUG
	Logger->debug("Painter::onRoiItemChanged(column:{})", column);
	#endif
	if (column == 3) // in 3 column we have enabled checker:
	{
		bool _state = true;
		if (item->checkState(column) == Qt::Unchecked)
		{
			_state = false;
		}

		QList<QGraphicsItem *> items = m_graphicsScene->items(Qt::DescendingOrder);
		for (int i = 0; i < items.size(); i++)
		{
			Logger->debug("View::onItemChanged() items:{}", i);
			if (items[i]->type() == m_roiType) // Just ROI object:
			{
				GraphicsRectItem* cast = dynamic_cast<GraphicsRectItem*>(items[i]);

				#ifdef DEBUG
				Logger->debug("View::onItemChanged() items:{}.type() == m_roiType", i);
				Logger->debug("View::onItemChanged() cast->id():{}", cast->id());
				Logger->debug("View::onItemChanged() item->text(3):{}", item->text(0).toStdString());
				#endif

				if(cast->id() == item->text(0).toInt())
				{
					items[i]->setEnabled(_state);
					items[i]->setVisible(_state);
				}
			}
		}
	}
}

void Painter::onAddRectToScene(QPointF startPoint, QPointF stopPoint, bool dialog, QString name)
{
	#ifdef DEBUG
	Logger->debug("Painter::onAddRectToScene()");
	#endif
	qreal x = 0;
	qreal y = 0;
	qreal width = 1;
	qreal heigt = 1;

	if (startPoint.x() < stopPoint.x() && startPoint.y() < stopPoint.y())
	{
		x = startPoint.x();
		width = qAbs(x - stopPoint.x());
		y = startPoint.y();
		heigt = qAbs(y - stopPoint.y());
		#ifdef DEBUG
		Logger->debug("View::onAddRectToScene() Bottom Right");
		#endif
	}
	else if (startPoint.x() < stopPoint.x() && startPoint.y() > stopPoint.y())
	{
		x = startPoint.x();
		width = qAbs(x - stopPoint.x());
		y = stopPoint.y();
		heigt = qAbs(y - startPoint.y());
		#ifdef DEBUG
		Logger->debug("View::onAddRectToScene() Up Right");
		#endif
	}
	else if (startPoint.x() > stopPoint.x() && startPoint.y() < stopPoint.y())
	{
		x = stopPoint.x();
		width = qAbs(x - startPoint.x());
		y = startPoint.y();
		heigt = qAbs(y - stopPoint.y());
		#ifdef DEBUG
		Logger->debug("View::onAddRectToScene() Bottom Left");
		#endif
	}
	else if (startPoint.x() > stopPoint.x() && startPoint.y() > stopPoint.y())
	{
		x = stopPoint.x();
		width = qAbs(x - startPoint.x());
		y = stopPoint.y();
		heigt = qAbs(y - startPoint.y());
		#ifdef DEBUG
		Logger->debug("View::onAddRectToScene() Up Left");
		#endif
	}

	if (dialog)
	{
		LabelDialog* dialog = new LabelDialog("");
		int dialogCode = dialog->exec();

		if (dialogCode == QDialog::Accepted)
		{
				QString tempStr = dialog->getLabelName();
				QRectF tempRectToText = QRectF(x, y, width, heigt);
				QColor color = QColor::fromRgb(0, 0, 0, 0);
				GraphicsRectItem* rectItem = new GraphicsRectItem(color, tempStr, tempRectToText, m_dlibType, 0);
				m_graphicsScene->addItem(rectItem);
			
		}
		if (dialogCode == QDialog::Rejected)
		{
			#ifdef DEBUG
			Logger->debug("View::onAddRectToScene() QDialog::Rejected");
			#endif
		}
	}
	else
	{}
	#ifdef DEBUG
	Logger->debug("Painter::onAddRectToScene() done");
	#endif
}

void Painter::addImageToScene(QPixmap image)
{
	#ifdef DEBUG
	Logger->debug("Painter::addImageToScene()");
	#endif
	m_pixmap = static_cast<GraphicsPixmapItem*>(m_graphicsScene->addPixmap(image));
	#ifdef DEBUG
	Logger->debug("Painter::addImageToScene() m_pixmap.type:{}",m_pixmap->type());
	#endif
	m_pixmap->setEnabled(true);
	m_pixmap->setVisible(true);
	m_pixmap->setOpacity(1.0);
	m_pixmap->setAcceptHoverEvents(true);
	m_pixmap->setAcceptTouchEvents(true);
	m_pixmap->setZValue(-2);
	m_pixmap->update();
	#ifdef DEBUG
	Logger->debug("Painter::addImageToScene() before configure: m_pixmap.type:{}",m_pixmap->type());
	#endif
	m_image = image.toImage();
	m_paintImage = image.toImage();

	for (int y = 0; y < m_paintImage.height(); y++)
	{
		for (int x = 0; x < m_paintImage.width(); x++)
		{
			m_paintImage.setPixelColor(x, y, QColor{ 255, 255, 255, 127 });
		}
	}
	
	QPixmap whiteBoardPixmap = QPixmap::fromImage(m_paintImage);
	m_paintPixmap = static_cast<GraphicsPixmapItem*>(m_graphicsScene->addPixmap(whiteBoardPixmap));
	m_paintPixmap->setEnabled(true);
	m_paintPixmap->setVisible(true);
	m_paintPixmap->setOpacity(0.5);
	m_paintPixmap->setAcceptHoverEvents(true);
	m_paintPixmap->setAcceptTouchEvents(true);
	m_paintPixmap->setZValue(-1);
	m_paintPixmap->update();
	#ifdef DEBUG
	Logger->debug("Painter::addImageToScene() m_paintPixmap.type:{}",m_paintPixmap->type());
	Logger->debug("Painter::addImageToScene() before configure: m_paintPixmap.type:{}",m_paintPixmap->type());
	#endif
	emit(updateView());
	#ifdef DEBUG
	Logger->debug("Painter::addImageToScene() done");
	#endif
}

void Painter::onLoadPaints(int id)
{
	#ifdef DEBUG
	Logger->debug("Painter::onLoadPaint()");
	#endif
	cv::Mat& image = m_dataMemory->gt(id);
	
	#ifdef DEBUG
	Logger->debug("Painter::onLoadPaint() image ({}x{}x{})", image.cols, image.rows, image.channels());
	#endif
	if(image.empty())
	{
		Logger->error("Painter::onLoadPaint() image cant be loaded");
		return;
	}
	else
	{
		if(image.channels() > 1)
		{
			cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
		}
		
		for (qint32 color = 0; color < m_painterSettings.m_colors.size(); color++)
		{
			for (int i = 0; i < image.cols; i++)
			{
				for (int j = 0; j < image.rows; j++)
				{
					if ( m_painterSettings.m_colorIntHash[m_painterSettings.m_colors[color]] == image.at<unsigned char>(j, i))
					{
						onPaintColors(i, j, m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]]);
					}
				}
			}
		}
	}
	Painter::onPaintColorsFinish();
	#ifdef DEBUG
	Logger->debug("Painter::onLoadPaint() done");
	#endif
}

void Painter::onPaintColors(qint32 x, qint32 y, QColor color)
{
	m_paintImage.setPixelColor(x, y, color);
}

void Painter::onPaintColorsFinish()
{
	#ifdef DEBUG
	Logger->debug("Painter::onPaintColorsFinish()");
	#endif
	m_paintPixmap->setPixmap(QPixmap::fromImage(m_paintImage));
	emit(updateView());
}

void Painter::onChangeColor(QColor color)
{
	#ifdef DEBUG
	Logger->debug("Painter::onChangeColor()");
	qDebug() << "Painter::onChangeColor:" << color;
	#endif
	m_painterSettings.m_color = color;
}

int Painter::onChangeOldColor(QString name, QColor color)
{
	#ifdef DEBUG
	Logger->debug("Painter::onChangeOldColor()");
	qDebug() << "Painter::onChangeOldColor:" << color;
	#endif
	
	// Check if color exist:
	for (qint32 i = 0; i < m_painterSettings.m_colors.size(); i++)
	{
		#ifdef DEBUG
		qDebug() << "_painterSettings.m_colorHash[m_painterSettings.m_colors[i]]" << m_painterSettings.m_colorHash[m_painterSettings.m_colors[i]];
		qDebug() << "color" << color;
		#endif
		if(m_painterSettings.m_colorHash[m_painterSettings.m_colors[i]].rgb() == color.rgb())
		{
			return false;
		}
	}
	
	QColor oldColor = m_painterSettings.m_colorHash[name];

	for (int i = 0; i < m_paintImage.height(); i++)
	{
		for (int j = 0; j < m_paintImage.width(); j++)
		{
			if (m_paintImage.pixel(j, i) == oldColor.rgb())
			{
				m_paintImage.setPixelColor(j, i, color);
			}
		}
	}
	m_painterSettings.m_color = color;
	m_painterSettings.m_colorHash[name] = color;
	
	Painter::onPaintColorsFinish();
	return true;
}

void Painter::onChangePenSize(qint32 size)
{
	#ifdef DEBUG
	Logger->debug("Painter::onChangePenSize()");
	qDebug() << "Painter::onChangePenSize:" << size;
	#endif
	
	m_painterSettings.m_penSize = size;
}

void Painter::setOpacity(qreal scaleOpacity)
{
	#ifdef DEBUG
	Logger->debug("Painter::setOpacity() set scale to:{}", scaleOpacity);
	#endif
	m_paintPixmap->setOpacity(scaleOpacity);
}

void Painter::setOpacityROI(qreal scaleOpacity)
{
	#ifdef DEBUG
	Logger->debug("Painter::setOpacityROI() set scale to:{}", scaleOpacity);
	#endif
	m_pixmap->setOpacity(scaleOpacity);
}

void Painter::setOpacityImage(qreal scaleOpacity)
{
	#ifdef DEBUG
	Logger->debug("Painter::setOpacityImage() set scale to:{}", scaleOpacity);
	#endif
	m_pixmap->setOpacity(scaleOpacity);
}
