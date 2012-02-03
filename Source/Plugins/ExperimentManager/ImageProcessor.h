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
