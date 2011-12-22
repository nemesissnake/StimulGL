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
