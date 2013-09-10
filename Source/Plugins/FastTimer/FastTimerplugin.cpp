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


#include "FastTimerplugin.h"
#include "FastTimer.h"
#include "defines.h"

Q_DECLARE_METATYPE(FastTimer*)
Q_DECLARE_METATYPE(FastTimer)

FastTimerPlugin::FastTimerPlugin(QObject *parent)
{
	FastTimerObject = new FastTimer(parent);
	FastTimerDiagObject = new FastTimer_Dialog();	
	strPluginInformation = QString(PLUGIN_INTERNAL_NAME) + " Plugin" + "(v" + PLUGIN_FILE_VERSION_STRING + ")" + " by " + PLUGIN_AUTHOR_NAME;
}

FastTimerPlugin::~FastTimerPlugin()
{
	delete FastTimerDiagObject;
	delete FastTimerObject;
}

bool FastTimerPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue FastTimerProto = engine.newQObject(FastTimerObject);
	engine.setDefaultPrototype(qMetaTypeId<FastTimer*>(), FastTimerProto);
	QScriptValue FastTimerCtor = engine.newFunction(FastTimer::ctor__extensionname, FastTimerProto);
	engine.globalObject().setProperty(PLUGIN_SCRIPTOBJECT_NAME, FastTimerCtor);
    //int nRes = 
		qRegisterMetaType<FastTimer>(PLUGIN_SCRIPTOBJECT_NAME);
	//FinalizeScriptObjectScriptContext(engine, FastTimerObject);
	return true;
}

bool FastTimerPlugin::ShowGUI()
{
	int returnVal;
	FastTimerDiagObject->setWindowTitle(strPluginInformation);
	returnVal = FastTimerDiagObject->exec();

	switch (returnVal) {
	case QMessageBox::Save:
	   // Save was clicked
	   break;
	case QMessageBox::Discard:
		// Don't Save was clicked
	    break;
	case QMessageBox::Cancel:
	    // Cancel was clicked
	    break;
	default:
	    // should never be reached
	    break;
	}		
	return true;
}

