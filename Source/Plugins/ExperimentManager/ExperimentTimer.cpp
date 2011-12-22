#include "ExperimentTimer.h"
//#include <stdlib.h>

ExperimentTimer::ExperimentTimer(QObject *parent)
	: QObject(parent)
{
#ifdef WIN32
	QueryPerformanceFrequency(&frequency);
	//long double PCFreq0 = long double(frequency.QuadPart)/1000.0;
	//double PCFreq1 = double(frequency.QuadPart)/1000.0;
	startCount.QuadPart = 0;
	endCount.QuadPart = 0;
#else
	startCount.tv_sec = startCount.tv_usec = 0;
	endCount.tv_sec = endCount.tv_usec = 0;
#endif
	stopped = 0;
	startTimeInMicroSec = 0;
	endTimeInMicroSec = 0;

	//LARGE_INTEGER ticksPerSecond;
	//LARGE_INTEGER tick;   // A point in time
	//LARGE_INTEGER time;   // For converting tick into real time

	//// get the high resolution counter's accuracy
	//QueryPerformanceFrequency(&ticksPerSecond);

	//// what time is it?
	//QueryPerformanceCounter(&tick);

	//// convert the tick number into the number of seconds
	//// since the system was started...
	//time.QuadPart = tick.QuadPart/ticksPerSecond.QuadPart;

	////get the number of hours
	//int hours = time.QuadPart/3600;

	////get the number of minutes
	//time.QuadPart = time.QuadPart - (hours * 3600);
	//int minutes = time.QuadPart/60;

	////get the number of seconds
	//int seconds = time.QuadPart - (minutes * 60);

	//QString result = "The system was started ";
	//result += QString::number(hours) + QString(" hours ");
	//result += QString::number(minutes) + " minutes ";
	//result += QString::number(seconds) + " and ";
	//result += QString::number(tick.QuadPart % ticksPerSecond.QuadPart);
	//result += "/" + QString::number(ticksPerSecond.QuadPart) + "=" + QString::number((tick.QuadPart % ticksPerSecond.QuadPart)/ticksPerSecond.QuadPart);
	//result += " seconds ago.";

	//result = result;
}

ExperimentTimer::~ExperimentTimer()
{

}

void ExperimentTimer::restart()
{
	stop();
	start();
}

void ExperimentTimer::start()
{
	stopped = 0; // reset stop flag
#ifdef WIN32
	QueryPerformanceCounter(&startCount);
#else
	gettimeofday(&startCount, NULL);
#endif
}

void ExperimentTimer::stop()
{
	stopped = 1; // set timer stopped flag
#ifdef WIN32
	QueryPerformanceCounter(&endCount);
#else
	gettimeofday(&endCount, NULL);
#endif
}

double ExperimentTimer::getElapsedTimeInMicroSec()
{
#ifdef WIN32
	if(!stopped)
		QueryPerformanceCounter(&endCount);	
	startTimeInMicroSec = startCount.QuadPart * (1000000.0 / frequency.QuadPart);
	endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart);
#else
	if(!stopped)
		gettimeofday(&endCount, NULL);

	startTimeInMicroSec = (startCount.tv_sec * 1000000.0) + startCount.tv_usec;
	endTimeInMicroSec = (endCount.tv_sec * 1000000.0) + endCount.tv_usec;
#endif
	//double dRetVal = endTimeInMicroSec - startTimeInMicroSec;
	//qint64 a = dRetVal;
	//__int64 b = dRetVal;
	//long double c =	(startCount.QuadPart * (1000000.0 / frequency.QuadPart)) - (endTimeInMicroSec = endCount.QuadPart * (1000000.0 / frequency.QuadPart));

	//QString str;
	//str.sprintf("took %.020Lf ms", c);
	//QString str1;
	//str1.sprintf("start %I64d end %I64d took %.020Lf ms", startTimeInMicroSec, endTimeInMicroSec, c);
	return endTimeInMicroSec - startTimeInMicroSec;
}

double ExperimentTimer::getElapsedTimeInMilliSec()
{
	return this->getElapsedTimeInMicroSec() * 0.001;
}

double ExperimentTimer::getElapsedTimeInSec()
{
	return this->getElapsedTimeInMicroSec() * 0.000001;
}

double ExperimentTimer::getElapsedTime()
{
	return this->getElapsedTimeInSec();
}

bool ExperimentTimer::SleepMSecAccurate(double mSecs)
{
	// note: BE SURE YOU CALL timeBeginPeriod(1) at program startup!!!
	// note: BE SURE YOU CALL timeEndPeriod(1) at program exit!!!
	// note: that will require linking to winmm.lib
	// note: never use static initializers (like this) with Winamp plug-ins!
	//static LARGE_INTEGER m_prev_end_of_frame; 
	//m_prev_end_of_frame.QuadPart = 0;
	//int max_fps = 60;
	LARGE_INTEGER freq;
	LARGE_INTEGER start_tick;
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start_tick);	
	
	if (mSecs > 0.5)
	{
		int ticks_passed = 0;
		int ticks_left = 0;
		LARGE_INTEGER current_tick;
		int ticks_to_wait = (int)(freq.QuadPart / (1000.0 / mSecs));
		bool doneWaiting = false;

		do
		{
			QueryPerformanceCounter(&current_tick);
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
	return true;
}
