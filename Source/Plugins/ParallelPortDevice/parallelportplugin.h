//parallelportplugin
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


#ifndef PARALLELPORTPLUGIN_H
#define PARALLELPORTPLUGIN_H

#include <QObject>
#include <QtWidgets>
#include <QString>
#include <Qlibrary>
#include "../../StimulGL/plugininterface.h"
#include "pport_dialog.h"
#include "ParallelPort.h"
#include "defines.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ParallelPortPlugin : public QObject, public DeviceInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID "StimulGL.Plugins.Devices" "ParallelPortDevice.json")
    Q_INTERFACES(DeviceInterface)

public:
	ParallelPortPlugin(short BaseAddress = 888, QObject *parent = 0);
	~ParallelPortPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString GetMinimalMainProgramVersion() {return PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION;};//here we override the virtual function to set a custom value
	
private:
	short nBaseAddress;
	ParallelPort *PPortObject; 
	PPort_Dialog *PPortDiagObject;

public slots:
    bool ShowGUI();
	bool IsCompatible() {return PluginInterface::IsCompatible();};
	QObject *GetScriptMetaObject(int nIndex) {return (QObject *)PPortObject->metaObject();};
	int GetScriptMetaObjectCount() {return 1;};//This plugin only defines 1 script/meta object

};

#endif//PARALLELPORTPLUGIN_H
