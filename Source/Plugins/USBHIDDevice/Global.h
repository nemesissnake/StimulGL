//USBHIDDeviceplugin
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