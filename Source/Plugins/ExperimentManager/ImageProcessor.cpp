//ExperimentManagerplugin
//Copyright (C) 2012  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#include "ImageProcessor.h"
#include <QImage>
#include <QPixmap>
#include "Global.h"

QScriptValue ImageProcessor::ctor__imageProcessor(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new ImageProcessor(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

ImageProcessor::ImageProcessor(QObject *parent)	: QObject(parent)
{

}

ImageProcessor::~ImageProcessor()
{

}

bool ImageProcessor::ConvertPngToDatFile(QString strSource, QString strDestination, bool bOverwrite)
{
	QFile fileSource(strSource);
	QFile fileDest(strDestination);
	QFileInfo fileDestInfo(strDestination);
	if (!fileSource.exists())
		return false;
	if (fileDest.exists() && (bOverwrite == false))
		return false;
	if (!fileDestInfo.absoluteDir().exists())
	{
		if (!QDir().mkdir(fileDestInfo.absolutePath()))
			return false;
	}
	QPixmap tmpPixmap;
	if(!tmpPixmap.load(strSource,"PNG"))
		return false;
	if (fileDest.open(QIODevice::WriteOnly))
	{
		QImage imgWrite = tmpPixmap.toImage().convertToFormat(QImage::Format_ARGB32);
		QDataStream output(&fileDest);
		quint32 magic = ARGB_FORMAT_HEADER;
		quint32 width = imgWrite.width();
		quint32 height = imgWrite.height();
		output << magic << width << height;
		for (quint32 y = 0; y < height; ++y) 
		{
			QRgb *scanLine = (QRgb *)imgWrite.scanLine(y);
			for (quint32 x = 0; x < width; ++x)
			{
				output << scanLine[x];
			}
		}
		fileDest.close();
		return true;
	}
	return false;
}

bool ImageProcessor::ConvertDatToPngFile(QString strSource, QString strDestination, bool bOverwrite)
{
	QFile fileSource(strSource);
	QFile fileDest(strDestination);
	QFileInfo fileDestInfo(strDestination);
	if (!fileSource.exists())
		return false;
	if (fileDest.exists() && (bOverwrite == false))
		return false;
	if (!fileDestInfo.absoluteDir().exists())
	{
		if (!QDir().mkdir(fileDestInfo.absolutePath()))
			return false;
	}
	if(fileSource.open(QIODevice::ReadOnly))
	{
		quint32 magic;
		quint32 width;
		quint32 height;
		quint32 x,y;
		bool bSaveResult = false;
		QRgb *scanLine;
		
		QDataStream input(&fileSource);
		//int nBytesRead = input.readRawData(magic,sizeof(magic));
		input >> magic >> width >> height;
		if(magic == ARGB_FORMAT_HEADER)
		{
			int nDataSize = width*height*sizeof(QRgb);
			QImage tmpImage(width,height,QImage::Format_ARGB32);
			for (y = 0; y < height; ++y) 
			{
				scanLine = (QRgb *)tmpImage.scanLine(y);
				for (x = 0; x < width; ++x)
				{
					input >> scanLine[x];
				}
			}
			bSaveResult = tmpImage.save(strDestination,"PNG");
			fileSource.close();
			return bSaveResult;
		}
		fileSource.close();
		return false;
	}
	return false;
}

bool ImageProcessor::ScalePngFile(QString strSource, QString strDestination, int nRatio, int nMethod, int nColorThreshold, bool bOverwrite)
{
	if (nRatio == 0)
		return false;
	bool bSaveResult = false;
	QFile fileSource(strSource);
	QFile fileDest(strDestination);
	QFileInfo fileDestInfo(strDestination);
	if (!fileSource.exists())
		return false;
	if (fileDest.exists() && (bOverwrite == false))
		return false;
	if (!fileDestInfo.absoluteDir().exists())
	{
		if (!QDir().mkdir(fileDestInfo.absolutePath()))
			return false;
	}
	QPixmap tmpPixmap;
	if(!tmpPixmap.load(strSource,"PNG"))
		return false;
	QSize newSize = tmpPixmap.size()/nRatio;
	switch (nMethod)
	{
	case 0://default (Mono colored)
		{
			tmpPixmap = tmpPixmap.scaled(newSize,Qt::KeepAspectRatio,Qt::FastTransformation);
			break;
		}
	case 1://Smooth (Grey colored)
		{
			tmpPixmap = tmpPixmap.scaled(newSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
			break;
		}
	case 2://Smooth + Convert to Mono colored (threshold = 255)
		{
			tmpPixmap = tmpPixmap.scaled(newSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
			QImage tmpImage = tmpPixmap.toImage().convertToFormat(QImage::Format_Mono, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);
			tmpPixmap = tmpPixmap.fromImage(tmpImage);
			break;	
		}
	case 3://Smooth + Convert to Mono colored (threshold = nColorThreshold)
		{
			tmpPixmap = tmpPixmap.scaled(newSize,Qt::KeepAspectRatio,Qt::SmoothTransformation);
			QImage tmpImage = tmpPixmap.toImage();//.convertToFormat(QImage::Format_Mono, Qt::MonoOnly|Qt::ThresholdDither|Qt::AvoidDither);
			QImage resultImage = tmpImage;
			resultImage.fill(255);
			for(int x=0; x<tmpImage.width(); x++)
				for(int y=0; y<tmpImage.height(); y++)
					resultImage.setPixel(x, y, qGray(tmpImage.pixel(x, y))>=nColorThreshold?qRgb(255, 255, 255):qRgb(0, 0, 0));
			tmpPixmap = tmpPixmap.fromImage(resultImage);
			//nColorThreshold
			//*and the following code is for converting images to monochrome manually by specifying threshold:*
			//
			//	QImage result = pImage;
			//result.fill(255);
			//for(int x=0; x<pImage.width(); x++)
			//	for(int y=0; y<pImage.height(); y++)
			//		result.setPixel(x, y, qGray(pImage.pixel(x, y))>threshold?qRgb(255, 255, 255):qRgb(0, 0, 0));

			//return result;

			//*The following code for converting images to monochrome by using automatic guessing for threshold:*
			//
			//	QImage result = pImage;
			//QList<int> pixelsValues;
			//for(int x=0; x<pImage.width();x++)
			//{
			//	for(int y=0; y<pImage.height();y++)
			//	{
			//		QRgb rgb = pImage.pixel(x,y);
			//		QColor color(rgb);
			//		pixelsValues<<color.lightness();
			//	}
			//}
			//qSort(pixelsValues);
			//int median2 = (pixelsValues.first() + pixelsValues.last())/2;
			//int threshold = median2;
			//for(int x=0; x<pImage.width(); x++)
			//	for(int y=0; y<pImage.height(); y++)
			//		result.setPixel(x, y, qGray(pImage.pixel(x, y))>threshold?qRgb(255, 255, 255):qRgb(0, 0, 0));
			//qDebug("threshold = %d", threshold);
			//return result;

			//*and the following code is for converting images to monochrome manually by specifying threshold:*
			//
			//	QImage result = pImage;
			//result.fill(255);
			//for(int x=0; x<pImage.width(); x++)
			//	for(int y=0; y<pImage.height(); y++)
			//		result.setPixel(x, y, qGray(pImage.pixel(x, y))>threshold?qRgb(255, 255, 255):qRgb(0, 0, 0));

			//return result;

		}
	}
	bSaveResult = tmpPixmap.save(strDestination,"PNG");
	return bSaveResult;
}