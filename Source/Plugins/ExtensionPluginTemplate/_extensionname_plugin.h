//_extensionname_
//Copyright (C) 2012  _authorname_
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


#ifndef _extensionname_PLUGIN_H
#define _extensionname_PLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "../../StimulGL/plugininterface.h"
#include "_extensionname__dialog.h"
#include "_extensionname_.h"
#include "defines.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class _extensionname_Plugin : public QObject, ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)

public:
	_extensionname_Plugin(QObject *parent = 0);
	~_extensionname_Plugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString GetMinimalMainProgramVersion() {return PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION;};//here we override the virtual function to set a custom value

private:

	short nExampleProperty;
	_extensionname_ *_extensionname_Object; 
	_extensionname__Dialog *_extensionname_DiagObject;

public slots:
    bool ShowGUI();
	bool IsCompatible() {return PluginInterface::IsCompatible();};// ExtensionInterface PluginInterface::IsCompatible();};
	//QStringList GetAdditionalFileExtensions() {return (QStringList() << "_extensionname_ files (*._extensionname_);;");};
	QObject *GetScriptMetaObject(int nIndex) {return (QObject *)_extensionname_Object->metaObject();};
	int GetScriptMetaObjectCount() {return 1;};//This plugin only defines 1 script/meta object

signals:
	void DoSignal();
};

#endif//_extensionname_PLUGIN_H
