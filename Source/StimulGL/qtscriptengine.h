//StimulGL
//Copyright (C) 2012  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


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