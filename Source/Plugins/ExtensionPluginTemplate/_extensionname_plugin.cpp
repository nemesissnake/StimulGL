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


#include "_extensionname_plugin.h"
#include "_extensionname_.h"
#include "defines.h"

Q_DECLARE_METATYPE(_extensionname_*)
Q_DECLARE_METATYPE(_extensionname_)

_extensionname_Plugin::_extensionname_Plugin(QObject *parent)
{
	_extensionname_Object = new _extensionname_(parent);
	_extensionname_DiagObject = new _extensionname__Dialog();	
	strPluginInformation = QString(PLUGIN_INTERNAL_NAME) + " Plugin" + "(v" + PLUGIN_FILE_VERSION_STRING + ")" + " by " + PLUGIN_AUTHOR_NAME;
}

_extensionname_Plugin::~_extensionname_Plugin()
{
	delete _extensionname_DiagObject;
	delete _extensionname_Object;
}

bool _extensionname_Plugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue _extensionname_Proto = engine.newQObject(_extensionname_Object);
	engine.setDefaultPrototype(qMetaTypeId<_extensionname_*>(), _extensionname_Proto);
	QScriptValue _extensionname_Ctor = engine.newFunction(_extensionname_::ctor__extensionname, _extensionname_Proto);
	engine.globalObject().setProperty("_extensionname_", _extensionname_Ctor);
	int nRes = qRegisterMetaType<_extensionname_>("_extensionname_");
	return true;
}

bool _extensionname_Plugin::ShowGUI()
{
	int returnVal;
	_extensionname_DiagObject->setWindowTitle(strPluginInformation);
	returnVal = _extensionname_DiagObject->exec();

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

Q_EXPORT_PLUGIN2(_extensionname_plugin, _extensionname_Plugin);
