#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QObject>

class ImageProcessor : public QObject
{
	Q_OBJECT

public:
	ImageProcessor(QObject *parent);
	~ImageProcessor();

public slots:
	bool ConvertPngToDatFile(QString strSource, QString strDestination);

private:
	
};

#endif // IMAGEPROCESSOR_H
