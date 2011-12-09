#ifndef EXPERIMENTLOGGER_H
#define EXPERIMENTLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
//#include <QTime>
#include "ExperimentTimer.h"

class ExperimentLogger : public QObject
{
	Q_OBJECT

	//typedef struct{
	//	QList<QString> sWho;
	//	QList<QString> sWhen;
	//	QList<QString> sWhere;
	//} LoggedData;

public:
	ExperimentLogger(QObject *parent = NULL);
	~ExperimentLogger();

	int createTimer();
	bool startTimer(int nIndex);
	double restartTimer(int nIndex);
	double elapsedTimerTime(int nIndex);

	bool setLogVars(const QString &strWho, const QString &strWhere, const int &nTimerIndex);
	//bool getLogVars(QString &strWho, QString &strWhen, QString &strWhere);
	bool WriteToOutput(const QString &fileName);
	void Test();

private:
	//LoggedData *internalLoggedDataStructure;
	QList<QString> sWho;
	QList<QString> sWhen;
	QList<QString> sWhere;

	QList<ExperimentTimer*> tTimers;//QTime> tTimers;
};

#endif // EXPERIMENTLOGGER_H
