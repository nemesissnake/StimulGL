//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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


#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QtScript>
#include <QScriptable>
#include <QColor>

//!  The ImageProcessor class. 
/*!
  The Image Processor can be used to edit Png (*.png) and custom Dat (*.dat) files.  
*/
class ImageProcessor : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "ImageProcessor");//Can't use defines here!, moc doesn't handle defines, not needed here

public:

	 /*! The enum ScalingMethod holds a Scaling Method for the ImageProcessor object */
	enum ScalingMethod
	{	
		ScalingMethod_MonoColored							= 0, /*!< default, Mono Colored. */
		ScalingMethod_SmoothGreyColored						= 1, /*!< Smoothed (Grey value Colored). */
		ScalingMethod_SmoothMonoColored						= 2, /*!< Smoothed and converted to a Mono Color values, threshold value = 255. */
		ScalingMethod_SmoothMonoColoredCustomThreshold		= 3  /*!< Smoothed and converted to a Mono Color values, custom threshold value. */
	};

	ImageProcessor(QObject *parent = NULL);
	ImageProcessor(const ImageProcessor& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~ImageProcessor();

	static QScriptValue ctor__imageProcessor(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool ConvertPngToDatFile(QString strSource, QString strDestination, bool bOverwrite = false);
	bool ConvertDatToPngFile(QString strSource, QString strDestination, bool bOverwrite = false);
	bool ScalePngFile(QString strSource, QString strDestination, int nRatio = 1, int nMethod = 0, int nColorThreshold = 255, bool bOverwrite = false, bool bSmoothFiltering = false);
	bool ScalePngFileBySize(QString strSource, QString strDestination, int nXPixels, int nYPixels, int nMethod = 0, int nColorThreshold = 255, bool bOverwrite = false, bool bSmoothFiltering = false);
	bool CreateMeanImageFromPngFiles(const QStringList &sourceImagePaths,const QString &destinationPath, const bool bOverwrite = false);
	bool ConcatenateDatFiles(const QStringList &sourceImagePaths,const QString &destinationPath, const bool bOverwrite = false);
	int SplitCDatFile(const QString &sourceImagePath,const QString &destinationPath, const QString &destPreFileName, const bool bOverwrite = false);
	//QPixmap ChangeHue(QPixmap &p);

private:
	QScriptEngine* currentScriptEngine;
	
};

Q_DECLARE_METATYPE(ImageProcessor*)

#endif // IMAGEPROCESSOR_H
