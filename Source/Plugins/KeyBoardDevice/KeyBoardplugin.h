//KeyBoardplugin
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


#ifndef KeyBoardPLUGIN_H
#define KeyBoardPLUGIN_H

#include <QObject>
#include <QtWidgets>
#include <QString>
#include <Qlibrary>
#include "../../StimulGL/plugininterface.h"
#include "keyboard_dialog.h"
#include "KeyBoardCapture.h"
#include "defines.h"
#include "Global.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class KeyBoardPlugin : public QObject, public DeviceInterface
{
    Q_OBJECT
	Q_PLUGIN_METADATA(IID "StimulGL.Plugins.Devices" "KeyBoardCapture.json")
    Q_INTERFACES(DeviceInterface)

public:
	KeyBoardPlugin(QObject *parent = 0);
	~KeyBoardPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString GetMinimalMainProgramVersion() {return PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION;};

private:
	int nMethod;
	KeyBoardCapture *KeyBoardObject; 
	KeyBoard_Dialog *KeyBoardDiagObject;

public slots:
    bool ShowGUI();
	bool IsCompatible() {return PluginInterface::IsCompatible();};
	QObject *GetScriptMetaObject(int nIndex) {return (QObject *)KeyBoardObject->metaObject();};
	int GetScriptMetaObjectCount() {return 1;};//This plugin only defines 1 script/meta object
};

#endif//KeyBoardPLUGIN_H
