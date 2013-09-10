//FastTimer
//Copyright (C) 2013  Michael Luehrs
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


#include "FastTimer.h"

QScriptValue FastTimer::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new FastTimer(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

FastTimer::FastTimer(QObject *parent) : QObject(parent)
{
	threadFinished = false;
	currentScriptEngine = NULL;

	MyTimer *privateTimer = new MyTimer();
	//QThread *workerThread = new QThread(this);


	privateTimer->moveToThread(&workerThread);
	connect(&workerThread, &QThread::finished, privateTimer, &QObject::deleteLater);
	connect(this, &FastTimer::startTimerSignal, privateTimer, &MyTimer::startTimer);
	connect(privateTimer, &MyTimer::timerInvoked, this, &FastTimer::timerInvoked);
	connect(this, &FastTimer::stopTimerSignal, privateTimer, &MyTimer::stopTimer);
	workerThread.start();
	

}

FastTimer::~FastTimer()
{
	workerThread.quit();
	workerThread.wait();
}

bool FastTimer::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

void FastTimer::startTimer(int shotTime)
{
	threadFinished = false;
	emit startTimerSignal(shotTime);
}

void FastTimer::timerInvoked()
{
	if(!threadFinished)
		emit timeout();
	
}

void FastTimer::stopTimer()
{
	threadFinished = true;
	emit stopTimerSignal();
	
}


