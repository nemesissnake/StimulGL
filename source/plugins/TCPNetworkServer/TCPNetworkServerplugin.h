//TCPNetworkServer
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


#ifndef TCPNetworkServerPLUGIN_H
#define TCPNetworkServerPLUGIN_H

#include <QObject>
#include <QtWidgets>
#include <QString>
#include <Qlibrary>

#include "../../StimulGL/plugininterface.h"
#include "TCPNetworkServer_dialog.h"
#include "TCPNetworkServer.h"
#include "defines.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class TCPNetworkServerPlugin : public QObject, ExtensionInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID "StimulGL.Plugins.Extensions" "TCPNetworkServer.json")
    Q_INTERFACES(ExtensionInterface)

public:
	TCPNetworkServerPlugin(QObject *parent = 0);
	~TCPNetworkServerPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString GetMinimalMainProgramVersion() {return PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION;};//here we override the virtual function to set a custom value

private:

	short nExampleProperty;
	TCPNetworkServer *TCPNetworkServerObject; 
	TCPNetworkServer_Dialog *TCPNetworkServerDiagObject;

public slots:
    bool ShowGUI();
	bool IsCompatible() {return PluginInterface::IsCompatible();};// ExtensionInterface PluginInterface::IsCompatible();};
	//QStringList GetAdditionalFileExtensions() {return (QStringList() << "TCPNetworkServer files (*.TCPNetworkServer);;");};
	QObject *GetScriptMetaObject(int nIndex) {if(nIndex == 0) return (QObject *)TCPNetworkServerObject->metaObject(); else return NULL;};

signals:
	void DoSignal();
};

#endif//TCPNetworkServerPLUGIN_H
