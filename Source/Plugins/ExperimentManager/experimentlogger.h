#ifndef EXPERIMENTLOGGER_H
#define EXPERIMENTLOGGER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTime>

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
	int restartTimer(int nIndex);
	int elapsedTimerTime(int nIndex);

	bool setLogVars(const QString &strWho, const QString &strWhere, const int &nTimerIndex);
	bool getLogVars(QString &strWho, QString &strWhen, QString &strWhere);
	bool WriteToOutput(const QString &fileName);
	void Test();

private:
	//LoggedData *internalLoggedDataStructure;
	QList<QString> sWho;
	QList<QString> sWhen;
	QList<QString> sWhere;

	QList<QTime> tTimers;
};

#endif // EXPERIMENTLOGGER_H
