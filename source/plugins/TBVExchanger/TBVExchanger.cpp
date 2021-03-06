//Copyright (C) 2014  Michael Luehrs, Brain Innovation B.V. and Sven Gijsen
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

#include "TBVExchanger.h"

QScriptValue TBVExchanger::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
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

	return engine->newQObject(new TBVExchanger(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

TBVExchanger::TBVExchanger(bool autoConnect, bool autoReconnect, QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	tbvNetwIntFace = NULL;
	initialize(autoConnect,autoReconnect);
}

TBVExchanger::~TBVExchanger()
{
	//You don't have to manually disconnect signals and slots, the QObject destruction cleans them up automatically
	//disconnect(tbvNetwIntFace,&TBVNetworkInterface::executePreStep,this,&TBVExchanger::executePreStep);
	//disconnect(tbvNetwIntFace,&TBVNetworkInterface::executePostStep,this,&TBVExchanger::executePostStep);
	//disconnect(tbvNetwIntFace,&TBVNetworkInterface::executePostRun,this,&TBVExchanger::executePostRun);
	//disconnect(tbvNetwIntFace,&TBVNetworkInterface::disconnected,this,&TBVExchanger::disconnected);
	//disconnect(tbvNetwIntFace,&TBVNetworkInterface::connected,this,&TBVExchanger::connected);
	//disconnect(tbvNetwIntFace,&TBVNetworkInterface::connectionError,this,&TBVExchanger::connectionError);
	if(tbvNetwIntFace)
	{
		delete tbvNetwIntFace;
		tbvNetwIntFace = NULL;
	}
}

bool TBVExchanger::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool TBVExchanger::initialize(bool autoConnect, bool autoReconnect)
{
	if(tbvNetwIntFace == NULL)
	{
		tbvNetwIntFace = new TBVNetworkInterface(autoConnect,autoReconnect);
		//bool bResult = connect(tbvNetwIntFace,SIGNAL(executePreStep(int)),this,SLOT(executePreStep()));
		connect(tbvNetwIntFace,&TBVNetworkInterface::executePreStep,this,&TBVExchanger::executePreStep);
		connect(tbvNetwIntFace,&TBVNetworkInterface::executePostStep,this,&TBVExchanger::executePostStep);
		connect(tbvNetwIntFace,&TBVNetworkInterface::executePostRun,this,&TBVExchanger::executePostRun);
		connect(tbvNetwIntFace,&TBVNetworkInterface::disconnected,this,&TBVExchanger::disconnected);
		connect(tbvNetwIntFace,&TBVNetworkInterface::connected,this,&TBVExchanger::connected);
		connect(tbvNetwIntFace,&TBVNetworkInterface::connectionError,this,&TBVExchanger::connectionError);
	}
	return true;
}

bool TBVExchanger::activateAutoConnection()
{
	return tbvNetwIntFace->setAutoConnection(true);
}

bool TBVExchanger::deactivateAutoConnection()
{
	return tbvNetwIntFace->setAutoConnection(false);
}