//DefaultQMLPlugin
//Copyright (C) 2013  Sven Gijsen
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

#ifndef TIMEMODEL_H
#define TIMEMODEL_H

#include <qdebug.h>
#include <qdatetime.h>
#include <qbasictimer.h>
#include <qapplication.h>

// Implements a "TimeModel" class with hour and minute properties
// that change on-the-minute yet efficiently sleep the rest
// of the time.
class MinuteTimer : public QObject
{
	Q_OBJECT

public:
	MinuteTimer(QObject *parent);
	void start();
	void stop();
	int hour() const;
	int minute() const;

signals:
	void timeChanged();

protected:
	void timerEvent(QTimerEvent *);

private:
	QTime time;
	QBasicTimer timer;
};

class TimeModel : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int hour READ hour NOTIFY timeChanged)
	Q_PROPERTY(int minute READ minute NOTIFY timeChanged)

public:
	TimeModel(QObject *parent=0);
	~TimeModel();
	int minute() const;
	int hour() const;

signals:
	void timeChanged();

private:
	QTime t;
	static MinuteTimer *timer;
	static int instances;
};

#endif // TIMEMODEL_H
