#include "ppcapturethread.h"
#include "parallelport.h"

ppCaptureThread::ppCaptureThread(short baseAddress, short mask, DetectionMethod method,int postLHDelay, int postHLDelay , QObject *parent)	: QThread(parent)
{
	isRunning = false;
	abortRunning = false;
	nBaseAddress = baseAddress;
	dMethod = method;
	nMask = mask;
	nPostLHDelay = postLHDelay;
	nPostHLDelay = postHLDelay;
}

ppCaptureThread::~ppCaptureThread()
{


}

void ppCaptureThread::stop()
{
	abortRunning = true;
}

void ppCaptureThread::run()
{
	short currentValue;
	short oldValue;
	QString result = "";
	ParallelPort portDev(nBaseAddress,this);

	currentValue = (portDev.PortRead() & nMask);
	oldValue = currentValue;
	isRunning = true;
	emit recieveThreadStarted(QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
	switch (dMethod)
	{
	case MaskedValueChanged :
		do 
		{
			if (oldValue != currentValue)//any change within masked measurement
			{
				//result = QString::number(currentValue);
				emit recieveThreadTriggered(currentValue);
				if (nPostLHDelay > 0)
				{
					if (currentValue > 0)//low->high was triggered
					{
						QThread::msleep(nPostLHDelay);
					}
				}
				if (nPostHLDelay > 0)
				{
					if (currentValue == 0)//high->low was triggered
					{
						QThread::msleep(nPostHLDelay);
					}
				}
				oldValue = currentValue;
			}
			currentValue = (portDev.PortRead() & nMask);
		} while (abortRunning==false);
		break;	
	case MaskedValueChangedHigh :
		do 
		{
			if (oldValue != currentValue)
			{
				if (currentValue == nMask)//low->high was triggered
				{
					//result = QString::number(currentValue);
					emit recieveThreadTriggered(currentValue);	
					if (nPostLHDelay > 0) QThread::msleep(nPostLHDelay);
				}
				oldValue = currentValue;
			}
			currentValue = (portDev.PortRead() & nMask);
		} while (abortRunning==false);
		break;
	case MaskedValueChangedLow :
		do 
		{
			if (oldValue != currentValue)
			{
				if (currentValue == 0)//high->low was triggered
				{
					//result = QString::number(currentValue);
					emit recieveThreadTriggered(currentValue);	
					if (nPostHLDelay > 0) QThread::msleep(nPostHLDelay);
				}
				oldValue = currentValue;
			}
			currentValue = (portDev.PortRead() & nMask);
		} while (abortRunning==false);
		break;
	default :
		break;	
	}

	abortRunning = false;
	isRunning = false;
	emit recieveThreadStopped(	QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
	return;
}

