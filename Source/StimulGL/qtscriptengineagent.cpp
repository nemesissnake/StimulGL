#include "qtscriptengineagent.h"

QTScriptEngineAgent::QTScriptEngineAgent(QScriptEngine *parent) : QScriptEngineAgent(parent) 
{	

}

QTScriptEngineAgent::~QTScriptEngineAgent()
{

}


void QTScriptEngineAgent::scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber)
{
	int a;
	a=1;
	emit ScriptLoaded(id);
}

void QTScriptEngineAgent::scriptUnload(qint64 id)
{
	int a;
	a=1;
	emit ScriptUnloaded(id);
}

void QTScriptEngineAgent::contextPush()
{
	int a;
	a=1;

}

void QTScriptEngineAgent::contextPop()
{
	int a;
	a=1;

}

void QTScriptEngineAgent::functionEntry(qint64 scriptId)
{
	QString A = QString::number(scriptId);
	int a;
	a=1;

}

void QTScriptEngineAgent::functionExit(qint64 scriptId,const QScriptValue &returnValue)
{
	int a;
	a=1;

}

void QTScriptEngineAgent::positionChange(qint64 scriptId,int lineNumber, int columnNumber)
{
	int a;
	a=1;

}

void QTScriptEngineAgent::exceptionThrow(qint64 scriptId, const QScriptValue &exception, bool hasHandler)
{
	int a;
	a=1;

}

void QTScriptEngineAgent::exceptionCatch(qint64 scriptId, const QScriptValue &exception)
{
	int a;
	a=1;

}

bool QTScriptEngineAgent::supportsExtension(Extension extension) const
{
	int a;
	a=1;
	return true;
}

QVariant QTScriptEngineAgent::extension(Extension extension, const QVariant &argument)
{
	int a;
	a=1;
	return a;
}