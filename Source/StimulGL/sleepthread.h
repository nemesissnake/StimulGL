#ifndef SLEEPTHREAD_H
#define SLEEPTHREAD_H

#include <QThread>

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

#endif // SLEEPTHREAD_H
