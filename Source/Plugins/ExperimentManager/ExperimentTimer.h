//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen and Michael Luehrs
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


#ifndef EXPERIMENTTIMER_H
#define EXPERIMENTTIMER_H

#include <QObject>
#include <QtScript>

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

class ExperimentTimer : public QObject
{
	Q_OBJECT

signals:
	void triggeredTimerLoopInvoked();

public:
	ExperimentTimer(QObject *parent = NULL);
	ExperimentTimer(const ExperimentTimer& other ){Q_UNUSED(other);};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~ExperimentTimer();

	static QScriptValue ctor__experimentTimer(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	void start();									// start timer
	void stop();									// stop the timer
	void restart();									// restart the timer
	double getElapsedTime();						// get elapsed time in second
	double getElapsedTimeInSec();					// get elapsed time in second (same as getElapsedTime)
	double getElapsedTimeInMilliSec();				// get elapsed time in milli seconds
	double getElapsedTimeInMicroSec();				// get elapsed time in micro-second
	static bool SleepMSecAccurate(double mSecs);
	static bool SleepMSecAccurate2(double mSecs);

	void startTriggeredTimerLoop(double nIntervalTimeMSecs);
	void stopTriggeredTimerLoop();

private:
	QScriptEngine* currentScriptEngine;

	double startTimeInMicroSec;						// starting time in micro-second
	double endTimeInMicroSec;						// ending time in micro-second
	int    stopped;									// stop flag 
#ifdef WIN32
	LARGE_INTEGER frequency;						// ticks per second
	LARGE_INTEGER startCount;                   
	LARGE_INTEGER endCount;                     
#else
	timeval startCount;                         
	timeval endCount;                           
#endif

	bool terminateTriggeredTimerLoop;
	
};

#endif // EXPERIMENTTIMER_H
