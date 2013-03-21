//StimulGL
//Copyright (C) 2013  Sven Gijsen
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


#include "qtscriptengineagent.h"

QTScriptEngineAgent::QTScriptEngineAgent(QScriptEngine *parent) : QScriptEngineAgent(parent) 
{	

}

QTScriptEngineAgent::~QTScriptEngineAgent()
{

}


void QTScriptEngineAgent::scriptLoad(qint64 id, const QString &program, const QString &fileName, int baseLineNumber)
{
	//int a;
	//a=1;
	emit ScriptLoaded(id);
}

void QTScriptEngineAgent::scriptUnload(qint64 id)
{
	//int a;
	//a=1;
	emit ScriptUnloaded(id);
}

void QTScriptEngineAgent::contextPush()
{
	//int a;
	//a=1;

}

void QTScriptEngineAgent::contextPop()
{
	//int a;
	//a=1;

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