#ifndef HIDCAPTURETHREAD_H
#define HIDCAPTURETHREAD_H

//#include <QObject>
#include <QThread>
#include <QDateTime>

class HIDCaptureThread : public QThread
{
	Q_OBJECT

protected:
	void run();

public:

	//enum DetectionMethod
	//{
	//	MaskedValueChanged				= 0,
	//	MaskedValueChangedHigh			= 1,
	//	MaskedValueChangedLow			= 2
	//};
	//Q_DECLARE_FLAGS(DetectionMethods, DetectionMethod)

	HIDCaptureThread(QObject *parent);
	~HIDCaptureThread();

public slots:
	void stop();

private:
	bool isRunning;
	bool abortRunning;
	//short nBaseAddress;
	//DetectionMethod dMethod;
	//short nMask;
	//int nPostLHDelay;
	//int nPostHLDelay; 

signals:
	void receiveThreadTriggered(QString sData);
	void receiveThreadStarted(QString timeTextStamp);
	void receiveThreadStopped(QString timeTextStamp);
};

//Q_DECLARE_OPERATORS_FOR_FLAGS(HIDCaptureThread::DetectionMethods)

#endif // HIDCAPTURETHREAD_H
