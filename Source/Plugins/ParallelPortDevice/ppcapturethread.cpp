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
	portDev = new ParallelPort(nBaseAddress,this);
}

ppCaptureThread::~ppCaptureThread()
{
	this->stop();
	if (portDev)
	{
		delete portDev;
		portDev = NULL;
	}
}

void ppCaptureThread::stop()
{
	if (isRunning)
	{
		//this->terminate();
		abortRunning = true;
		int nRetries = 25;
		while (isRunning && (nRetries > 0))
		{
			QThread::msleep(1);
			nRetries--;
		}
		QThread::msleep(0);//Just to let the other thread completely finish
		QThread::msleep(0);//Just to let the other thread completely finish
		//if (nRetries > 0)
		//{
			//couldn't stop the thread, should we implement a false boolean return so that TerminateThread can be called?
		//}		
	}	
}

void ppCaptureThread::run()
{
	isRunning = true;
	
	short currentValue;
	short oldValue;
	QString result = "";
	//ParallelPort portDev(nBaseAddress,this);
	//portDev = new ParallelPort(nBaseAddress,this);
	//portDev->setBaseAddress()

	currentValue = (portDev->PortRead() & nMask);
	oldValue = currentValue;
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
			if (abortRunning==false)
			{
				QThread::msleep(1);
				QThread::msleep(1);			
				currentValue = (portDev->PortRead() & nMask);
			}
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
			if (abortRunning==false)
			{
				QThread::msleep(1);
				QThread::msleep(1);
				currentValue = (portDev->PortRead() & nMask);
			}
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
			if (abortRunning==false)
			{
				QThread::msleep(1);
				QThread::msleep(1);			
				currentValue = (portDev->PortRead() & nMask);
			}
		} while (abortRunning==false);
		break;
	default :
		break;	
	}
	abortRunning = false;	
	emit recieveThreadStopped(	QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
	isRunning = false;
	return;
}

