#include "experimentlogger.h"

ExperimentLogger::ExperimentLogger(QObject *parent) : QObject(parent)
{
	//internalLoggedDataStructure = new LoggedData();
}

ExperimentLogger::~ExperimentLogger()
{
	//if (internalLoggedDataStructure)
	//{
	//	delete internalLoggedDataStructure;
	//}
	for (int i=0;i<tTimers.count();i++)
	{
		delete tTimers[i];
	}
}

bool ExperimentLogger::setLogVars(const QString &strWho, const QString &strWhere, const int &nTimerIndex)
{
	sWho.append(strWho + ",Timer(" + QString::number(nTimerIndex) + ")");
	if (nTimerIndex >= 0)
	{
		double dElapsed = tTimers.at(nTimerIndex)->getElapsedTimeInMilliSec();
		sWhen.append(QString::number(dElapsed));
	}
	else
	{
		sWhen.append("-");
	}
	sWhere.append(strWhere);
	return true;
}

bool ExperimentLogger::WriteToOutput(const QString &fileName)
{
	if (fileName.isEmpty())
		return false;

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text | QIODevice::Truncate)) 
		return false;
	QTextStream out(&file);//QDataStream
	for (int count=0; count < sWhen.count(); count++)
		out << sWho.at(count) << "\t" << sWhere.at(count) << "\t" << sWhen.at(count) << "\n";
	file.close();
	return true;
}

int ExperimentLogger::createTimer()
{
	//tTimers.append(QTime());
	////tTotalRunningTime.restart();
	//return tTimers.count()-1;

	tTimers.append(new ExperimentTimer());
	//tTotalRunningTime.restart();
	return tTimers.count()-1;


////	double start;
////	double stop;
//	double tick1;
//	double tick2;
////	// first, use std clock() function to measure elapsed time ////////////////
////	start = stop = tick1 = tick2 = (double)clock(); // start timer and remember initial ticks
////	while((stop-start)/CLOCKS_PER_SEC < 1)    // loop for 1 sec	
////	{
////		//cout << (tick2 - tick1) / CLOCKS_PER_SEC * 1000 << " ms." << endl;
////		// reset timer
////		tick1 = tick2;
////		tick2 = stop = (double)clock();
////	}
////	// pause until user input
//////	cout << "\n\nPress Enter key to run Timer class...\n\n";
////	char c;
//////	cin.get(c);
//	// second, use Timer::getElapsedTime() ////////////////////////////////////
//	ExperimentTimer *t;
//	t = new ExperimentTimer();
//	// start timer
//	t->start();
//	tick1 = tick2 = t->getElapsedTimeInMilliSec();
//	while(t->getElapsedTime() < 1)       // loop for 1 sec
//	{
//		//cout << (tick2 - tick1) << " ms." << endl;
//
//		tick1 = tick2;
//		tick2 = t->getElapsedTimeInMilliSec();
//	}
//	//cout << CLOCKS_PER_SEC << endl;
//	//cout << CLK_TCK << endl;
//	//cout << clock()/CLOCKS_PER_SEC << endl;
//	delete t;
//	return 0;
}

bool ExperimentLogger::startTimer(int nIndex)
{
	if (tTimers.count() > nIndex)
	{
		tTimers[nIndex]->start();
		return true;
	}
	return false;
}

double ExperimentLogger::restartTimer(int nIndex)
{
	if (tTimers.count() > nIndex)
	{
		double RetVal = tTimers[nIndex]->getElapsedTimeInMilliSec();
		tTimers[nIndex]->stop();
		tTimers[nIndex]->start();
		return RetVal;
	}
	return -1;
}

double ExperimentLogger::elapsedTimerTime(int nIndex)
{
	if (tTimers.count() > nIndex)
	{
		return tTimers[nIndex]->getElapsedTimeInMilliSec();
	}
	return -1;
}


