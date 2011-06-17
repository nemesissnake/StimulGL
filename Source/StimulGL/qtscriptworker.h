#ifndef QTSCRIPTWORKER_H
#define QTSCRIPTWORKER_H

#include "mainappinfo.h"
#include <QThread>
#include <QtScript>
#if QT_VERSION >= 0x040500
#include <QtScriptTools>
#endif

#include "qtscriptengineagent.h"

class QTScriptWorker : public QThread
{
	Q_OBJECT

public:
	QTScriptWorker(QObject *parent);
	~QTScriptWorker();

	void setScript(const QString &script);

private:
	QString currentScript;
	QScriptEngine *eng;
	QScriptEngineDebugger *debugger;
	QTScriptEngineAgent *agent;

	bool ImportScriptExtensions();//QDir extensionsDir);

protected:
	virtual void run();
	
};

#endif // QTSCRIPTWORKER_H
