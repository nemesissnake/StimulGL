#ifndef EXPERIMENTLOGGER_H
#define EXPERIMENTLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include "ExperimentTimer.h"

class ExperimentLogger : public QObject
{
	Q_OBJECT

	typedef struct{
		QList<int> nObject;
		QList<int> nTimer;
		QList<QString> sFunction;
		QList<QString> sTag;
		QList<QString> sMessage;
		QList<double> dTime;
	} LoggedData;

public:
	ExperimentLogger(QObject *parent = NULL);
	~ExperimentLogger();

	int createTimer();
	bool startTimer(int nIndex);
	double restartTimer(int nIndex);
	double elapsedTimerTime(int nIndex);

	bool setLogVars(const int &nObjectIndex, const int &nTimerIndex, const QString &strFunction = "", const QString &strTag = "", const QString &strMessage = "");
	//bool getLogVars(QString &strWho, QString &strWhen, QString &strWhere);
	bool WriteToOutput(const QString &fileName);
	void Test();

private:
	LoggedData *internalLoggedDataStructure;
	//QList<QString> sWho;
	//QList<QString> sWhen;
	//QList<QString> sWhere;

	QList<ExperimentTimer*> tTimers;
};

#endif // EXPERIMENTLOGGER_H
