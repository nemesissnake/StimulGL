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


#include "metaextensions.h"

#define THREADACTIVATIONTRIGGERTIME		5 //This is the minimal trigger period, the time resolution/accuracy can be higher!

TriggerTimer::TriggerTimer() : QObject(NULL)
{
	moveToThread(&_thread);
	_thread.start();
	ThreadActivationTrigger.setInterval(THREADACTIVATIONTRIGGERTIME);
	ThreadActivationTrigger.moveToThread(&_thread);
	bDoStopTimer = false;
	dTriggerInterval = 0;
	dElapsed = 0;
	dNextThreshold = 0;
}

TriggerTimer::~TriggerTimer()
{	
	ThreadActivationTrigger.stop();
	bDoStopTimer = true;
	eTimer.stop();	
	if( _thread.isRunning() )
	{
		_thread.terminate();//The hard way...
		_thread.wait();
	}
}

void TriggerTimer::startTimer(double dMSec)//Do not change the function name without changing the below!
{
	if(QThread::currentThread() != &_thread)//Wrong thread
	{
		//qDebug() << __FUNCTION__ << "Wrong thread";
		//QString aa = __FUNCTION__;
		QMetaObject::invokeMethod(this, "startTimer", Qt::QueuedConnection,Q_ARG(double, dMSec));//Cannot be a DirectConnection !!
	}
	else//Right thread
	{
		//qDebug() << __FUNCTION__ << "Right thread";
		eTimer.start();
		connect(&ThreadActivationTrigger, SIGNAL(timeout()), this, SLOT(runThreadedTimerFunction()));
		//emit timeout();
		bDoStopTimer = false;
		dTriggerInterval = dMSec;
		dNextThreshold = dTriggerInterval;
		ThreadActivationTrigger.start();
	}
}

void TriggerTimer::stopTimer()//Do not change the function name without changing the below!
{
	if(QThread::currentThread() != &_thread)//Wrong thread
	{
		QMetaObject::invokeMethod(this, "stopTimer", Qt::QueuedConnection);//Cannot be a DirectConnection !!
	}
	else//Right thread
	{
		ThreadActivationTrigger.stop();
		bDoStopTimer = true;
		eTimer.stop();
	}
}

void TriggerTimer::runThreadedTimerFunction()
{
	if(bDoStopTimer)
		return;
	dElapsed = eTimer.getElapsedTimeInMilliSec();
	if ((((int)(dElapsed+1)) + THREADACTIVATIONTRIGGERTIME) >= ((int)dNextThreshold))//Can we dismiss the below?
	{
		eTimer.SleepMSecAccurate(dNextThreshold - dElapsed);
		emit timeout();
		dNextThreshold = dNextThreshold + dTriggerInterval;
	}
}


//// Just a helper macro:
//#define NO_RETURN_VALUE
//
//// This does the real work:
//#define THREAD_MAGIC(targetThread, returnValue, args)     \
//	if(QThread::currentThread() != targetThread)                     \
//{                                                                \
//	QString slotName = __FUNCTION__;                             \
//	slotName.remove(QRegExp("^.*::"));                           \
//	bool ret = metaObject()->invokeMethod(this,                  \
//	qPrintable(slotName), Qt::QueuedConnection,          \
//	args.count() >=  1 ? args[0] : QGenericArgument(0),  \
//	args.count() >=  2 ? args[1] : QGenericArgument(0),  \
//	args.count() >=  3 ? args[2] : QGenericArgument(0),  \
//	args.count() >=  4 ? args[3] : QGenericArgument(0),  \
//	args.count() >=  5 ? args[4] : QGenericArgument(0),  \
//	args.count() >=  6 ? args[5] : QGenericArgument(0),  \
//	args.count() >=  7 ? args[6] : QGenericArgument(0),  \
//	args.count() >=  8 ? args[7] : QGenericArgument(0),  \
//	args.count() >=  9 ? args[8] : QGenericArgument(0),  \
//	args.count() >= 10 ? args[9] : QGenericArgument(0)); \
//	if(!ret)                                                     \
//{                                                            \
//	qFatal(qPrintable(__FUNCTION__ +                         \
//	QString(" Could not call QMetaObject::invokeMethod(). " \
//	"Check your argument list quantity and types.")));     \
//}                                                            \
//	return returnValue;                                          \
//}        
//
//#define MAKE_THREAD_SAFE_0(TargetThread, returnValue)                \
//	do {                                                         \
//	QList<QGenericArgument> args;                                \
//	THREAD_ME_LIKE_A_NEEDLE(TargetThread, returnValue, args);    \
//	} while (0);                                                  \
//
//#define THREAD_MAGIC_1(TargetThread, returnValue, ArgType1, ArgName1)         \
//	do {                                                         \
//	QList<QGenericArgument> args = QList<QGenericArgument>() <<  \
//	Q_ARG(ArgType1, ArgName1);                               \
//	THREAD_MAGIC(TargetThread, returnValue, args);    \
//	} while (0);
//\
//
//#define THREAD_MAGIC_2(TargetThread, returnValue, ArgType1, ArgName1, ArgType2, ArgName2) \
//	do {                                                         \
//	QList<QGenericArgument> args = QList<QGenericArgument>() <  \
//	Q_ARG(ArgType1, ArgName1) < <                             \
//	Q_ARG(ArgType2, ArgName2);                               \
//	THREAD_MAGIC(TargetThread, returnValue, args);    \
//	} while (0);                                                  \
//	And now the usage in one of your slots:
//
//void MyThread::mySlot(const QString &name, int count)
//{
//	THREAD_MAGIC_2(this, NO_RETURN_VALUE, QString, name, int, count);
//
//	// You are now thread safe
//}