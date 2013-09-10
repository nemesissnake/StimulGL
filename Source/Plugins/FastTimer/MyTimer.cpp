#include "MyTimer.h"


// If defined, WIN32_LEAN_AND_MEAN disables timeBeginPeriod/timeEndPeriod.
#undef WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#include <sys/timeb.h>
#include <sys/types.h>

#include <iostream>

using namespace std;


MyTimer::MyTimer(QObject *parent)
	: QObject(parent)
{
	terminateTimer = false;
}

MyTimer::~MyTimer()
{
	terminateTimer = true;
}

void MyTimer::startTimer(int interval)
{
	terminateTimer = false;
	while(!terminateTimer)
	{
		SleepMSecAccurate(interval);
		//Sleep(interval);
		emit timerInvoked();
		QCoreApplication::processEvents();
	}
	terminateTimer = false;
}


void MyTimer::stopTimer()
{
	terminateTimer = true;
}

bool MyTimer::SleepMSecAccurate(double mSecs)
{
	// note: BE SURE YOU CALL timeBeginPeriod(1) at program startup!!!
	// note: BE SURE YOU CALL timeEndPeriod(1) at program exit!!!
	// note: that will require linking to winmm.lib
	// note: never use static initializers (like this) with Winamp plug-ins!

	timeBeginPeriod(1);

	LARGE_INTEGER freq;
	LARGE_INTEGER start_tick;
	if (!QueryPerformanceFrequency(&freq))// Save the performance counter frequency for later use.
		qDebug() << __FUNCTION__ << "QueryPerformanceFrequency() failed with error " << GetLastError();
	//else
	//	qDebug() << __FUNCTION__ << "QueryPerformanceFrequency(freq) gives: " << freq.QuadPart;
	if (!QueryPerformanceCounter(&start_tick))
		qDebug() << __FUNCTION__ << "QueryPerformanceCounter() failed with error " << GetLastError();
	//else
	//	qDebug() << __FUNCTION__ << "QueryPerformanceCounter(start) gives: " << start_tick.QuadPart;

	if (mSecs > 0.5)
	{
		int ticks_passed = 0;
		int ticks_left = 0;
		LARGE_INTEGER current_tick;
		int ticks_to_wait = (int)(freq.QuadPart / (1000.0 / mSecs));
		bool doneWaiting = false;

		do
		{
			if (!QueryPerformanceCounter(&current_tick))
				qDebug() << __FUNCTION__ << "QueryPerformanceCounter() failed with error " << GetLastError();
			//else
			//	qDebug() << __FUNCTION__ << "QueryPerformanceCounter(current) gives: " << current_tick.QuadPart;
			ticks_passed = (int)((__int64)current_tick.QuadPart - (__int64)start_tick.QuadPart);
			ticks_left = ticks_to_wait - ticks_passed;
			if (ticks_passed >= ticks_to_wait)
				doneWaiting = true;
			if (!doneWaiting)
			{
				// if > 0.002s left, do Sleep(1), which will actually sleep some 
				//   steady amount, probably 1-2 ms,
				//   and do so in a nice way (cpu meter drops; laptop battery spared).
				//   otherwise, do a few Sleep(0)'s, which just give up the timeslice,
				//   but don't really save cpu or battery, but do pass a tiny
				//   amount of time.
				if (ticks_left > (int)((freq.QuadPart*2)/1000.0))
					Sleep(1);
				else                        
					for (int i=0; i<10; i++) 
						Sleep(0);  // causes thread to give up its timeslice
			}
		}
		while (!doneWaiting); 
	}
	else
	{
		Sleep(0);
	}
	timeEndPeriod(1);
	return true;
}