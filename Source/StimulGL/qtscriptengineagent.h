#ifndef QTSCRIPTENGINEAGENT_H
#define QTSCRIPTENGINEAGENT_H

#include <QScriptEngineAgent>

class QTScriptEngineAgent : public QObject, public QScriptEngineAgent
{
    Q_OBJECT

public:
	QTScriptEngineAgent(QScriptEngine *parent);//QScriptEngine
	~QTScriptEngineAgent();

	void scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber);
	void scriptUnload(qint64 id);
	void contextPush();
	void contextPop();
	void functionEntry(qint64 scriptId);
	void functionExit(qint64 scriptId,const QScriptValue &returnValue);
	void positionChange(qint64 scriptId,int lineNumber, int columnNumber);
	void exceptionThrow(qint64 scriptId,const QScriptValue &exception,bool hasHandler);
	void exceptionCatch(qint64 scriptId,const QScriptValue &exception);
	bool supportsExtension(Extension extension) const;
	QVariant extension(Extension extension,	const QVariant &argument = QVariant());

signals:
	void ScriptUnloaded(qint64 id);
	void ScriptLoaded(qint64 id);
};

#endif // QTSCRIPTENGINEAGENT_H
