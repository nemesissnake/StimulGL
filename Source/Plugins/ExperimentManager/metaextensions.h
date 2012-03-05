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


#ifndef METAEXTENSIONS_H
#define METAEXTENSIONS_H

//#include <QObject>
//#include <QWidget>
#include <QTimer>
#include <QThread>
#include "ExperimentTimer.h"

//class TriggerTimer : public QTimer
//{
//public:
//	TriggerTimer(QWidget *parent = 0){};
//	TriggerTimer(const TriggerTimer& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
//	//~TriggerTimer(){};
//};



//class TriggerTimer : public QThread
//{
//	Q_OBJECT
//
//public:
//	TriggerTimer(QObject *parent = 0){};
//	TriggerTimer(const TriggerTimer& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
//	//~TriggerTimer(){};
//
//public slots:
//	void startTimer(int nMSec);
//	void stopTimer();
//
//};



class TriggerTimer : public QObject
{
	Q_OBJECT

signals:
	void timeout();

public:
	TriggerTimer();
	TriggerTimer(const TriggerTimer& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~TriggerTimer();

public slots:
	void startTimer(double nMSec);
	void stopTimer();

private:
	QThread _thread;
	//QMutex _mutex;
	QTimer ThreadActivationTrigger;
	ExperimentTimer eTimer;
	bool bDoStopTimer;
	double dTriggerInterval;
	double dElapsed;
	double dNextThreshold;

private slots:
	void runThreadedTimerFunction();
};

#endif // METAEXTENSIONS_H
