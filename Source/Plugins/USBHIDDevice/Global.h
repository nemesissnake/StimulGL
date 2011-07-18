#ifndef HIDGLOBAL_H
#define HIDGLOBAL_H

#include <QThread>

//#define round(x) ((x) < LONG_MIN-0.5 || (x) > LONG_MAX+0.5 ?\
//	error() : ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define RESSCALEFACTOR 1000.0
#define INVSQRRTTWO 0.707106781
#define SQRRTTWO 1.414213562

struct strcCalibrationData {
	int Xmin;
	int Xmax;
	int Ymin;
	int Ymax;
	int XAbsOffset;
	int YAbsOffset;
	float XRelScaleFactor;
	float YRelScaleFactor;
};

enum HIDDevDialogState
{
	NotInitialized					= 0,
	ReadyToCapture					= 1,
	CaptureStarting					= 2,
	CaptureStarted					= 3,
	CaptureStopping					= 4
};
Q_DECLARE_FLAGS(HIDDevDialogStates, HIDDevDialogState)
Q_DECLARE_OPERATORS_FOR_FLAGS(HIDDevDialogStates)

class SleepThread : public QThread
{
	//public:
	//	SleepThread(QObject *parent);
	//	~SleepThread();
	//
	//private:

public:
	static void msleep(unsigned long msecs)
	{
		QThread::msleep(msecs);
	}

};

#endif