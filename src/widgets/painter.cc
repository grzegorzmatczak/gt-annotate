#include "widgets/painter.h"

constexpr auto WIDTH{ "Width" };
constexpr auto HEIGHT{ "Height" };
constexpr auto X{ "x" };
constexpr auto Y{ "y" };


Painter::Painter(QJsonObject const& config, GraphicsScene *graphicsScene, GraphicsView* graphicsView)
	: m_config(config)
	, m_graphicsScene(graphicsScene)
	, m_graphicsView(graphicsView)
{
	m_roiType = 4;
	m_whiteBoardType = 5;
	m_ImageType = 6;
    m_painterSettings.configureColors(m_config);
	m_graphicsView->setPainterSettings(&m_painterSettings);
}

void Painter::onPaintWhiteBoard(qint32 x, qint32 y)
{
	if (m_painterSettings.m_penSize == 1)
	{
		m_paintImage.setPixelColor(x, y, m_painterSettings.m_color);
	}
	else if (m_painterSettings.m_penSize > 1 && m_painterSettings.m_penSize < 20)
	{
		qint32 it = m_painterSettings.m_penSize - 1;
		for (int zx = -it; zx <= it; zx++)
		{
			for (int zy = -it; zy <= it; zy++)
			{
				m_paintImage.setPixelColor(x + zx, y + zy, m_painterSettings.m_color);
			}
		}
	}
	m_paintPixmap->setPixmap(QPixmap::fromImage(m_paintImage));
}

void Painter::onLoadImage(QString imageName)
{
	Painter::clearScene();
	Logger->debug("Painter::onLoadImage()");
	QPixmap test;
	test.load(imageName);
	addImageToScene(test);
}

void Painter::clearScene()
{
	QList<QGraphicsItem*> items = m_graphicsScene->items(Qt::DescendingOrder);
	Logger->trace("Painter::onItemChanged() size:{}", items.size());
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->type() == m_ImageType || items[i]->type() == m_whiteBoardType)
		{
			GraphicsRectItem* cast = dynamic_cast<GraphicsRectItem*>(items[i]);
			m_graphicsScene->removeItem(cast);
			delete cast;
		}
	}
}

static cv::Mat qimage_to_mat_ref(QImage &img, int format)
{
    return cv::Mat(img.height(), img.width(), format, img.bits(), img.bytesPerLine());
}

void Painter::deleteRois()
{
	QList<QGraphicsItem*> items = m_graphicsScene->items(Qt::DescendingOrder);
	Logger->trace("Painter::onItemChanged() size:{}", items.size());
	for (int i = 0; i < items.size(); i++)
	{
		if (items[i]->type() == m_roiType) // Just ROI object:
		{
			GraphicsRectItem* cast = dynamic_cast<GraphicsRectItem*>(items[i]);
			m_graphicsScene->removeItem(cast);
			delete cast;
		}
	}
}

void Painter::onSaveWhiteBoard()
{
	Painter::deleteRois();

	Logger->trace("Painter::onSaveWhiteBoard()");
	for (int color = 0; color < m_painterSettings.m_colors.size(); color++)
	{
		Logger->trace("Painter::onSaveWhiteBoard() color:{}", color);
		qDebug() <<m_painterSettings.m_colors[color];
		qDebug() <<m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]];

		cv::Mat cleanData = cv::Mat(m_paintImage.height(), m_paintImage.width(), CV_8UC1,cv::Scalar(0));
		for (int i = 0; i < m_paintImage.width(); i++)
		{
			for (int j = 0; j < m_paintImage.height(); j++)
			{
				if ( m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]].rgb() == m_paintImage.pixel(i,j))
				{
					cleanData.at<unsigned char>(j,i) = 255;
				}
			}
		}
		QJsonArray contoursArray{};
		m_contour.CrateRois(cleanData, color, contoursArray);
		QString _name = QString::number(color) + "_CrateRois.png";
		cv::imwrite(_name.toStdString(), cleanData);
		Logger->warn("contoursArray.size:{}", contoursArray.size());
		for (unsigned int i = 0; i < contoursArray.size(); i++)
		{
			QJsonObject obj = contoursArray[i].toObject();
			QRectF tempRectToText = QRectF(obj[X].toInt(), obj[Y].toInt(), obj[WIDTH].toInt(),obj[HEIGHT].toInt());
			qDebug() << "tempRectToText:" << tempRectToText;
			QColor _color = m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]];
			QString _name = m_painterSettings.m_colors[color];
			Logger->warn("Painter::CrateRois()  add GraphicsRectItem:{} name:{}", i, _name.toStdString());
			qDebug() << "color:" << _color;
			GraphicsRectItem * itemG = new GraphicsRectItem(_color, _name, tempRectToText, m_roiType);
			m_graphicsScene->addItem(itemG);
		}
	}
}

void Painter::addImageToScene(QPixmap image)
{
	Logger->trace("Painter::addImageToScene()");
	emit(setupMatrix());
	m_pixmap = static_cast<GraphicsPixmapItem*>(m_graphicsScene->addPixmap(image));
	m_pixmap->setEnabled(true);
	m_pixmap->setVisible(true);
	m_pixmap->setOpacity(1.0);
	m_pixmap->setAcceptHoverEvents(true);
	m_pixmap->setAcceptTouchEvents(true);
	m_pixmap->setZValue(-2);
	m_pixmap->update();

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
}

void Painter::renderColorsFromImage(QString pathToImage)
{
	Logger->trace("Painter::renderColorsFromImage() pathToImage:{}", pathToImage.toStdString());
	cv::Mat image = cv::imread(pathToImage.toStdString());
	
	Logger->trace("Painter::renderColorsFromImage() image ({}x{}x{})", image.cols, image.rows, image.channels());
	if(image.empty())
	{
		Logger->error("Painter::renderColorsFromImage() image cant be loaded:{}", pathToImage.toStdString());
		return;
	}
	else
	{
		cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
		for (qint32 color = 0; color < m_painterSettings.m_colors.size(); color++)
		{
			for (int i = 0; i < image.cols; i++)
			{
				for (int j = 0; j < image.rows; j++)
				{
					if ( m_painterSettings.m_colorInthash[m_painterSettings.m_colors[color]] == image.at<unsigned char>(j, i))
					{
						onPaintColors(i, j, m_painterSettings.m_colorHash[m_painterSettings.m_colors[color]]);
					}
				}
			}
		}
	}
	Painter::onPaintColorsFinish();
}

void Painter::onPaintColors(qint32 x, qint32 y, QColor color)
{
	m_paintImage.setPixelColor(x, y, color);
}

void Painter::onPaintColorsFinish()
{
	Logger->trace("Painter::onPaintColorsFinish()");
	m_paintPixmap->setPixmap(QPixmap::fromImage(m_paintImage));
}

void Painter::onChangeColor(QColor color)
{
	qDebug() << "Painter::onChangeColor:" << color;
	m_painterSettings.m_color = color;
}

bool Painter::onChangeOldColor(QString name, QColor color)
{
	qDebug() << "Painter::onChangeOldColor:" << color;
	// Check if color exist:
	for (qint32 i = 0; i < m_painterSettings.m_colors.size(); i++)
	{
		qDebug() << "_painterSettings.m_colorHash[m_painterSettings.m_colors[i]]" << m_painterSettings.m_colorHash[m_painterSettings.m_colors[i]];
		qDebug() << "color" << color;
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
	qDebug() << "Painter::onChangePenSize:" << size;
	m_painterSettings.m_penSize = size;
}

void Painter::setOpacity(qreal scaleOpacity)
{
	Logger->trace("Painter::setOpacity() set scale to:{}", scaleOpacity);
	m_paintPixmap->setOpacity(scaleOpacity);
}

void Painter::setOpacityROI(qreal scaleOpacity)
{
	Logger->trace("Painter::setOpacityROI() set scale to:{}", scaleOpacity);
	m_pixmap->setOpacity(scaleOpacity);
}

void Painter::setOpacityImage(qreal scaleOpacity)
{
	Logger->trace("Painter::setOpacityImage() set scale to:{}", scaleOpacity);
	m_pixmap->setOpacity(scaleOpacity);
}
