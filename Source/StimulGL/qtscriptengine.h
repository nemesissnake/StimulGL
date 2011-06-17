#ifndef QTSCRIPTENGINE_H
#define QTSCRIPTENGINE_H

#include <QObject>
#include <QtScript>
#if QT_VERSION >= 0x040500
#include <QtScriptTools>
#endif
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>
#include <QtGui/QApplication>
#include "qtscriptengineagent.h"
//#include "qtscriptworker.h"

class QTScriptEngine : public QObject
{
	Q_OBJECT

public:
	QTScriptEngine(QObject *parent);
	~QTScriptEngine();
	//QScriptValue evaluate(const QString &program, const QString &fileName = QString(), int lineNumber = 1);

	QScriptEngine *eng;
	QTScriptEngineAgent *agent;
	QScriptEngineDebugger *debugger;

	void ConfigureDebugger(bool state);
	QMenu *createDebuggerMenu(QWidget *parent = 0);
	QMainWindow *DebuggerStandardWindow();
	bool ImportScriptExtensions();

protected:

public slots:
	void scriptHasUnloaded(qint64 id);
	void scriptHasLoaded(qint64 id);

private:
	//QScriptValue qtscript_quit(QScriptContext *ctx, QScriptEngine *eng);
	//QScriptValue qtscript_describe(QScriptContext *ctx, QScriptEngine *eng);
	//QScriptValue importExtension(QScriptContext *context, QScriptEngine *engine);

	//void executeScript(const QString &script);
	void setAgent();

	void interactive(QScriptEngine *eng);
	bool attachedToDebugger;
	bool isRunning;
	//QTScriptWorker *worker;

signals:
	void ScriptLoaded(qint64 scriptID);
	void ScriptUnloaded(qint64 scriptID);

};

#endif // QTSCRIPTENGINE_H
