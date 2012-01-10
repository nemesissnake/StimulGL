#include "ppgeneratethread.h"
#include "parallelport.h"

ppGenerateThread::ppGenerateThread(short baseAddress, GenerationMethod method, short outputMask, short activeValue, short inActiveValue, int activePulseTime, int repetitionTime, QObject *parent)	: QThread(parent)
{
	isRunning = false;
	abortRunning = false;
	nBaseAddress = baseAddress;
	dMethod = method;
	nOutputMask = outputMask;
	nActiveValue = activeValue;
	nInActiveValue = inActiveValue;
	nActivePulseTime =activePulseTime;
	nRepetitionTime = repetitionTime;
	portDev = new ParallelPort(nBaseAddress,this);
}

ppGenerateThread::~ppGenerateThread()
{
	this->stop();
	if (portDev)
	{
		delete portDev;
		portDev = NULL;
	}
}

void ppGenerateThread::stop()
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

void ppGenerateThread::run()
{
	isRunning = true;

	short currentValue;
	short newActiveMaskedValue;
	short newInActiveMaskedValue;
	//ParallelPort portDev(nBaseAddress,this);

	currentValue = portDev->PortRead();
	newActiveMaskedValue = ((currentValue & (255 - nOutputMask)) | (nActiveValue & nOutputMask));
	emit generateThreadStarted(	QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
	
	switch (dMethod)
	{
	case Value :	
		if (newActiveMaskedValue != currentValue)
		{
			portDev->PortWrite(newActiveMaskedValue);			
		}
		emit generateThreadTriggered(newActiveMaskedValue);
		break;	
	case Pulse:
		newInActiveMaskedValue = ((currentValue & (255 - nOutputMask)) | (nInActiveValue & nOutputMask));
		if (newInActiveMaskedValue != currentValue)
		{
			portDev->PortWrite(newInActiveMaskedValue);
			//emit generateThreadTriggered(newInActiveMaskedValue);
		}
		portDev->PortWrite(newActiveMaskedValue);
		emit generateThreadTriggered(newActiveMaskedValue);
		QThread::msleep(nActivePulseTime);
		portDev->PortWrite(newInActiveMaskedValue);
		//emit generateThreadTriggered(newInActiveMaskedValue);
		break;
	case Periodical:
		newInActiveMaskedValue = ((currentValue & (255 - nOutputMask)) | (nInActiveValue & nOutputMask));
		int nInActivePulseTime;
		if (nRepetitionTime>nActivePulseTime)
		{
			nInActivePulseTime = nRepetitionTime-nActivePulseTime;
		}
		else
		{
			nInActivePulseTime = 1;//Weird situation!!
		}
		if (newInActiveMaskedValue != currentValue)
		{
			portDev->PortWrite(newInActiveMaskedValue);
			//emit generateThreadTriggered(newInActiveMaskedValue);
		}
		do 
		{
			portDev->PortWrite(newActiveMaskedValue);
			emit generateThreadTriggered(newActiveMaskedValue);
			QThread::msleep(nActivePulseTime);
			portDev->PortWrite(newInActiveMaskedValue);
			//emit generateThreadTriggered(newInActiveMaskedValue);
			if (abortRunning) break;
			QThread::msleep(nInActivePulseTime);
		} while (abortRunning==false);
		break;
	default :
		break;	
	}

	abortRunning = false;
	emit generateThreadStopped(	QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
	isRunning = false;
	return;
}
