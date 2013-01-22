//SerialPortDeviceplugin
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


#include "SerialPortDeviceplugin.h"
#include "SerialPortDevice.h"
#include "defines.h"

Q_DECLARE_METATYPE(SerialPortDevice*)
Q_DECLARE_METATYPE(SerialPortDevice)

SerialPortDevicePlugin::SerialPortDevicePlugin(QObject *parent)
{
	SerialPortDeviceObject = new SerialPortDevice(parent);//0x181b, 0x4002, parent);
	SerialPortDeviceDiagObject = new SerialPortDevice_Dialog();	
	strPluginInformation = PLUGIN_INFORMATION;
}

SerialPortDevicePlugin::~SerialPortDevicePlugin()
{
	delete SerialPortDeviceDiagObject;
	delete SerialPortDeviceObject;
}

bool SerialPortDevicePlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue SerialPortDeviceProto = engine.newQObject(SerialPortDeviceObject);
	engine.setDefaultPrototype(qMetaTypeId<SerialPortDevice*>(), SerialPortDeviceProto);
	QScriptValue SerialPortDeviceCtor = engine.newFunction(SerialPortDevice::ctor__extensionname, SerialPortDeviceProto);
	engine.globalObject().setProperty(PLUGIN_SCRIPTOBJECT_NAME, SerialPortDeviceCtor);
	int nRes = qRegisterMetaType<SerialPortDevice>(PLUGIN_SCRIPTOBJECT_NAME);
	//FinalizeScriptObjectScriptContext(engine, SerialPortDeviceObject);
	return true;
}

bool SerialPortDevicePlugin::ShowGUI()
{
	int returnVal;
	SerialPortDeviceDiagObject->setWindowTitle(strPluginInformation);
	returnVal = SerialPortDeviceDiagObject->exec();

	switch (returnVal) 
	{
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

//Q_PLUGIN_METADATA(SerialPortDeviceplugin, SerialPortDevicePlugin);
