//PiezoStimDevice
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


#include "PiezoStimDeviceplugin.h"
#include "PiezoStimDevice.h"
#include "defines.h"

Q_DECLARE_METATYPE(PiezoStimDevice*)
Q_DECLARE_METATYPE(PiezoStimDevice)

PiezoStimDevicePlugin::PiezoStimDevicePlugin(QObject *parent)
{
	PiezoStimDeviceObject = new PiezoStimDevice(parent);
	PiezoStimDeviceDiagObject = new PiezoStimDevice_Dialog();	
	strPluginInformation = QString(PLUGIN_INTERNAL_NAME) + " Plugin" + "(v" + PLUGIN_FILE_VERSION_STRING + ")" + " by " + PLUGIN_AUTHOR_NAME;
}

PiezoStimDevicePlugin::~PiezoStimDevicePlugin()
{
	delete PiezoStimDeviceDiagObject;
	delete PiezoStimDeviceObject;
}

bool PiezoStimDevicePlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue PiezoStimDeviceProto = engine.newQObject(PiezoStimDeviceObject);
	engine.setDefaultPrototype(qMetaTypeId<PiezoStimDevice*>(), PiezoStimDeviceProto);
	QScriptValue PiezoStimDeviceCtor = engine.newFunction(PiezoStimDevice::ctor__extensionname, PiezoStimDeviceProto);
	engine.globalObject().setProperty(PLUGIN_SCRIPTOBJECT_NAME, PiezoStimDeviceCtor);
    int nRes = qRegisterMetaType<PiezoStimDevice>(PLUGIN_SCRIPTOBJECT_NAME);
	//FinalizeScriptObjectScriptContext(engine, PiezoStimDeviceObject);
	return true;
}

bool PiezoStimDevicePlugin::ShowGUI()
{
	int returnVal;
	PiezoStimDeviceDiagObject->setWindowTitle(strPluginInformation);
	returnVal = PiezoStimDeviceDiagObject->exec();

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

Q_EXPORT_PLUGIN2(PiezoStimDeviceplugin, PiezoStimDevicePlugin);
