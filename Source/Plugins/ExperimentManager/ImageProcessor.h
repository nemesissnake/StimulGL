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


#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>
#include <QtScript>
#include <QScriptable>

class ImageProcessor : public QObject, protected QScriptable
{
	Q_OBJECT

public:
	ImageProcessor(QObject *parent = NULL);
	ImageProcessor(const ImageProcessor& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~ImageProcessor();

	static QScriptValue ctor__imageProcessor(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool ConvertPngToDatFile(QString strSource, QString strDestination, bool bOverwrite = false);
	bool ConvertDatToPngFile(QString strSource, QString strDestination, bool bOverwrite = false);
	bool ScalePngFile(QString strSource, QString strDestination, int nRatio = 1, int nMethod = 0, int nColorThreshold = 255, bool bOverwrite = false);

private:
	
};

Q_DECLARE_METATYPE(ImageProcessor*)

#endif // IMAGEPROCESSOR_H