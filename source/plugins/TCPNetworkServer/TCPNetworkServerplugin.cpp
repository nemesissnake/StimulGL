//TCPNetworkServer
//Copyright (C) 2014  Sven Gijsen
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


#include "TCPNetworkServerplugin.h"
#include "TCPNetworkServer.h"
#include "defines.h"

Q_DECLARE_METATYPE(TCPNetworkServer*)
Q_DECLARE_METATYPE(TCPNetworkServer)

TCPNetworkServerPlugin::TCPNetworkServerPlugin(QObject *parent)
{
	TCPNetworkServerObject = new TCPNetworkServer(parent);
	TCPNetworkServerDiagObject = new TCPNetworkServer_Dialog();	
	strPluginInformation = QString(PLUGIN_INTERNAL_NAME) + " Plugin" + "(v" + PLUGIN_FILE_VERSION_STRING + ")" + " by " + PLUGIN_AUTHOR_NAME;
}

TCPNetworkServerPlugin::~TCPNetworkServerPlugin()
{
	delete TCPNetworkServerDiagObject;
	delete TCPNetworkServerObject;
}

bool TCPNetworkServerPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue TCPNetworkServerProto = engine.newQObject(TCPNetworkServerObject);
	engine.setDefaultPrototype(qMetaTypeId<TCPNetworkServer*>(), TCPNetworkServerProto);
	QScriptValue TCPNetworkServerCtor = engine.newFunction(TCPNetworkServer::ctor__extensionname, TCPNetworkServerProto);
	engine.globalObject().setProperty(PLUGIN_SCRIPTOBJECT_NAME, TCPNetworkServerCtor);
    //int nRes = 
		qRegisterMetaType<TCPNetworkServer>(PLUGIN_SCRIPTOBJECT_NAME);
	//FinalizeScriptObjectScriptContext(engine, TCPNetworkServerObject);
	return true;
}

bool TCPNetworkServerPlugin::ShowGUI()
{
	int returnVal;
	TCPNetworkServerDiagObject->setWindowTitle(strPluginInformation);
	returnVal = TCPNetworkServerDiagObject->exec();

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

