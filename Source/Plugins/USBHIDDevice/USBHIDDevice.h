//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef USBHIDDevice_H
#define USBHIDDevice_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>

#include "hidapi.h"
#include "hidcalibratedialog.h"
#include "hidcapturethread.h"

class USBHIDDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	void ExampleSignalTriggered(short);
	void CaptureThreadStarted(QString);
	void CaptureThreadStopped(QString);
	void CaptureThreadTriggered(QString);

public:
	USBHIDDevice(QObject *parent = 0);
	~USBHIDDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	void setExampleProperty( short sExampleProperty );
	short getExampleProperty() const;

	bool doMeasurement();
	bool Calibrate();

	bool StartCaptureThread();
	void StopCaptureThread();

private:
	short m_ExampleProperty;
	HIDCalibrateDialog *HIDCalDiag;
	HIDCaptureThread *HIDCapThread;

};

#endif // USBHIDDevice_H
