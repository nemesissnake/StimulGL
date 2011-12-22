#ifndef EXPERIMENTTIMER_H
#define EXPERIMENTTIMER_H

#include <QObject>

#ifdef WIN32   // Windows system specific
#include <windows.h>
#else          // Unix based system specific
#include <sys/time.h>
#endif

class ExperimentTimer : public QObject
{
	Q_OBJECT

public:
	ExperimentTimer(QObject *parent = NULL);
	ExperimentTimer(const ExperimentTimer& other ){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~ExperimentTimer();

	void   start();                             // start timer
	void   stop();                              // stop the timer
	void restart();								// restart the timer
	double getElapsedTime();                    // get elapsed time in second
	double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
	double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
	double getElapsedTimeInMicroSec();          // get elapsed time in micro-second
	static bool SleepMSecAccurate(double mSecs);

private:
	double startTimeInMicroSec;                 // starting time in micro-second
	double endTimeInMicroSec;                   // ending time in micro-second
	int    stopped;                             // stop flag 
#ifdef WIN32
	LARGE_INTEGER frequency;                    // ticks per second
	LARGE_INTEGER startCount;                   //
	LARGE_INTEGER endCount;                     //
#else
	timeval startCount;                         //
	timeval endCount;                           //
#endif
	
};

#endif // EXPERIMENTTIMER_H
