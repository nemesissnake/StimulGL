#ifndef TESTSCRIPTOBJECT_H
#define TESTSCRIPTOBJECT_H

#include <QObject>
#include <QScriptEngine>
#include <QString>
#include <QFile>
#include <QMessageBox>

class testScriptObject : public QObject
{
	Q_OBJECT

public:
	// ctor for this class - pass in the script engine to bind to.
	testScriptObject( QScriptEngine *pEngine, QObject *parent =0);
	~testScriptObject();

	// attach ourselves to a script file - this could be done inside the ctor. I
	// have chosen to use a separate method instead.
	void runScript(const QString &strProgram);

signals:
	// test signal - we can emit this, and have some QtScript code run.
	void signal1();

public slots:
	// test slot - just displays a message box. 
	void slot1();
	// second test slot - displays the string in a combo box.
	void displayMsg(QString strMsg);

private:
	// store a pointer to the script engine.
	QScriptEngine *m_pEngine;

	// store the "this" object, so we can manually call script functions if we need to.
	QScriptValue m_thisObject;
	
};

#endif // TESTSCRIPTOBJECT_H
