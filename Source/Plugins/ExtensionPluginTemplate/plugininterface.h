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


//This file defines the Interface, do not change!

#ifndef ExtensionInterface_H
#define ExtensionInterface_H

#include <QString>
#include <QtScript>
#include <QMessageBox>

	class PluginInterface
	{
	public:
		virtual ~PluginInterface() {}
		virtual QString Test(const QString &message) = 0;
		virtual QString GetPluginInformation(void) = 0;
		virtual bool ShowGUI() = 0;
		virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
	};

	class DeviceInterface: virtual public PluginInterface
	{
	public:
		virtual ~DeviceInterface() {}
	};

	class ExtensionInterface: virtual public PluginInterface
	{
	public:
		virtual ~ExtensionInterface() {}
	};

	QT_BEGIN_NAMESPACE
		Q_DECLARE_INTERFACE(DeviceInterface, "StimulGL.Plugins.DeviceInterface/1.0");
		Q_DECLARE_INTERFACE(ExtensionInterface, "StimulGL.Plugins.ExtensionInterface/1.0");
	QT_END_NAMESPACE

#endif // ExtensionInterface_H
