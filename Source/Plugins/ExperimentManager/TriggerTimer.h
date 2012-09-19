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

#ifndef TRIGGERTIMER_H
#define TRIGGERTIMER_H

#include <QTimer>
#include <QThread>
#include <QtScript>
#include "ExperimentTimer.h"
#include "CurrentTime.h"

/*! The enum (TriggerTimerType) can store a TriggerTimer type */
enum TriggerTimerType
{
	QPC_TriggerTimerType			= 0, //!< 0: The Query Performance Counter Timer type
	QTimer_TriggerTimerType			= 1, //!< 1: The QTimer Timer type
	QPCNew_TriggerTimerType			= 2  //!< 2: The Query Performance Counter Timer type, new implementation
};

class TriggerTimer : public QObject, protected QScriptable
{
	Q_OBJECT

	struct strcIntervalTest
	{
		//! The default constructor of the structure
		/*		strcIntervalTest()
		{
		dFirstSample = 0.0;
		dSecondSample = 0.0;
		dMinSampleSpeed = 0.0;
		}*/	
		bool bSampleSpeedDetermined;
		double dFirstSample;
		double dMinSampleSpeed;
		double dSampleSpeedToUse;
	};

signals:
	void timeout();
	void started();
	void goingAccurate(double);

public:
	TriggerTimer();
	TriggerTimer(const TriggerTimer& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~TriggerTimer();

	static QScriptValue ctor__triggerTimer(QScriptContext* context, QScriptEngine* engine);
	bool setTimerType(TriggerTimerType newTimerType);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	void startTimer(double nMSec);
	void stopTimer();
	double currentTime() {return WTF::currentTime();};
	bool setTimerType(const QString &sNewTimerType);
	QString getTimerType() const;

private:
	void resetIntervalTestResults();
	void createTimerTypeHashTable();

	QScriptEngine* currentScriptEngine;
	QHash<QString, int> timerTypeHash;
	int nThreadIdealCount;
	QThread _thread;
	QTimer ThreadActivationTrigger;
	ExperimentTimer eTimer;
	QTimer qtTimer;
	bool bDoStopTimer;
	double dTriggerInterval;
	double dElapsed;
	double dStartTime;
	double dNextThreshold;
	double dRemainingTime;
	strcIntervalTest intervalTest;
	TriggerTimerType currentTimerType;
	bool bTimerIsRunning;

private slots:
	void runThreadedTimerFunction();

};

Q_DECLARE_METATYPE(TriggerTimer*)
Q_DECLARE_FLAGS(TriggerTimerTypes, TriggerTimerType)
Q_DECLARE_OPERATORS_FOR_FLAGS(TriggerTimerTypes)

#endif // TRIGGERTIMER_H
