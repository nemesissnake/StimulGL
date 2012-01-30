#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(QObject *parent)
	: QObject(parent)
{

}

ImageProcessor::~ImageProcessor()
{

}

bool ImageProcessor::ConvertPngToDatFile(QString strSource, QString strDestination)
{
	//if (expSnapshot.currExpTrigger > lastTriggerNumber)
	//{
	//	QFile file;
	//	QString outputDir = MainAppInfo::outputsDirPath();
	//	outputDir = outputDir + RETINOMAP_WIDGET_OUTPUT_SUBFOLDER;
	//	if(QDir(outputDir).exists()==false)
	//	{
	//		QDir().mkdir(outputDir);
	//	}
	//	outputDir = outputDir + getLastLoggedObjectStateTime(Experiment_SubObject_Started) + "/";
	//	if(QDir(outputDir).exists()==false)
	//	{
	//		QDir().mkdir(outputDir);
	//	}
	//	QString fileName = outputDir + QString::number(nRetinoID) + "_" + QString::number(expSnapshot.currExpBlock) + QString("_") + QString::number(expSnapshot.currExpTrial) + QString("_") + QString::number(expSnapshot.currExpTrigger);

	//	switch (retinoOutputType)
	//	{
	//	case RetinoMap_OutputType_Frame:
	//		if (retinoOutputFormat == RetinoMap_OutputFormat_PNG)
	//		{
	//			fileName = fileName + QString(".png");
	//			file.setFileName(fileName);
	//			if (file.open(QIODevice::WriteOnly))
	//			{
	//				StimulusResultImageFrame->save(&file, "PNG");
	//				file.close();
	//			}
	//			break;
	//		} 
	//		else if (retinoOutputFormat == RetinoMap_OutputFormat_DAT)
	//		{
	//			fileName = fileName + QString(".dat");
	//			file.setFileName(fileName);
	//			if (file.open(QIODevice::WriteOnly))
	//			{
	//				QImage imgWrite = StimulusResultImageFrame->toImage().convertToFormat(QImage::Format_ARGB32);
	//				QDataStream output(&file);
	//				quint32 magic = ARGB_FORMAT_HEADER;
	//				quint32 width = imgWrite.width();
	//				quint32 height = imgWrite.height();
	//				output << magic << width << height;
	//				for (quint32 y = 0; y < height; ++y) 
	//				{
	//					QRgb *scanLine = (QRgb *)imgWrite.scanLine(y);
	//					for (quint32 x = 0; x < width; ++x)
	//					{
	//						output << scanLine[x];
	//					}
	//				}
	//				//bool bResult = 
	//				//output.status() == QDataStream::Ok;
	//				file.close();
	//			}
	//			break;
	//		}
	//	case RetinoMap_OutputType_Mask:
	//		if (retinoOutputFormat == RetinoMap_OutputFormat_PNG)
	//		{
	//			fileName = fileName + QString(".png");
	//			file.setFileName(fileName);
	//			if (file.open(QIODevice::WriteOnly))
	//			{
	//				StimulusActivationMap->save(&file, "PNG");
	//				file.close();
	//			}
	//			break;
	//		} 
	//		else if (retinoOutputFormat == RetinoMap_OutputFormat_DAT)
	//		{
	//			fileName = fileName + QString(".dat");
	//			file.setFileName(fileName);
	//			if (file.open(QIODevice::WriteOnly))
	//			{
	//				QImage imgWrite = StimulusActivationMap->toImage().convertToFormat(QImage::Format_ARGB32);
	//				QDataStream output(&file);
	//				quint32 magic = ARGB_FORMAT_HEADER;
	//				quint32 width = imgWrite.width();
	//				quint32 height = imgWrite.height();
	//				output << magic << width << height;
	//				for (quint32 y = 0; y < height; ++y) 
	//				{
	//					QRgb *scanLine = (QRgb *)imgWrite.scanLine(y);
	//					for (quint32 x = 0; x < width; ++x)
	//					{
	//						output << scanLine[x];
	//					}
	//				}
	//				//bool bResult = 
	//				//output.status() == QDataStream::Ok;
	//				file.close();
	//			}
	//			break;
	//		}
	//	}
	//	lastTriggerNumber++;
	//}
	return true;
}