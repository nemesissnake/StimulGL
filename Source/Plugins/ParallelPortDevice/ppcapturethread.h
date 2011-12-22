#ifndef PPCAPTURETHREAD_H
#define PPCAPTURETHREAD_H

#include <QThread>
#include <QDateTime>

class ParallelPort;
class ppCaptureThread : public QThread
{
	Q_OBJECT

protected:
	void run();

public:

	enum DetectionMethod
	{
		MaskedValueChanged				= 0,
		MaskedValueChangedHigh			= 1,
		MaskedValueChangedLow			= 2
	};
	Q_DECLARE_FLAGS(DetectionMethods, DetectionMethod)


	ppCaptureThread(short baseAddress, short mask, DetectionMethod method,int postLHDelay = 0, int postHLDelay = 0 ,QObject *parent = 0);
	~ppCaptureThread();

public slots:
	void stop();

private:
	bool isRunning;
	bool abortRunning;
	short nBaseAddress;
	DetectionMethod dMethod;
	short nMask;
	int nPostLHDelay;
	int nPostHLDelay; 
	ParallelPort *portDev;

signals:
	void recieveThreadTriggered(short Value);
	void recieveThreadStarted(QString timeTextStamp);
	void recieveThreadStopped(QString timeTextStamp);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ppCaptureThread::DetectionMethods)

#endif // PPCAPTURETHREAD_H
