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
}

bool ExperimentLogger::setLogVars(const QString &strWho, const QString &strWhere, const int &nTimerIndex)
{
	sWho.append(strWho + ",Timer(" + QString::number(nTimerIndex) + ")");
	if (nTimerIndex >= 0)
	{
		int nElapsed = tTimers.at(nTimerIndex).elapsed();
		sWhen.append(QString::number(nElapsed));
	}
	else
	{
		sWhen.append("-");
	}
	sWhere.append(strWhere);
	return true;
}

bool ExperimentLogger::getLogVars(QString &strWho, QString &strWhen, QString &strWhere)
{
	//strWho.append(strWho);
	//strWhen.append(strWhen);
	//strWhere.append(strWhere);
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
}

int ExperimentLogger::createTimer()
{
	tTimers.append(QTime());
	//tTotalRunningTime.restart();
	return tTimers.count()-1;
}

bool ExperimentLogger::startTimer(int nIndex)
{
	if (tTimers.count() > nIndex)
	{
		tTimers[nIndex].start();
		return true;
	}
	return false;
}

int ExperimentLogger::restartTimer(int nIndex)
{
	if (tTimers.count() > nIndex)
	{
		return tTimers[nIndex].restart();
	}
	return -1;
}

int ExperimentLogger::elapsedTimerTime(int nIndex)
{
	if (tTimers.count() > nIndex)
	{
		return tTimers[nIndex].elapsed();
	}
	return -1;
}


