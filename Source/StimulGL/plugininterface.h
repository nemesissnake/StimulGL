//plugininterface.h
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


#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QtScript>
#include <QMessageBox>
#include "maindefines.h"


class PluginInterface
{
public:
	virtual ~PluginInterface() {}
	virtual bool ShowGUI() = 0;
	virtual QString GetPluginInformation(void) {return strPluginInformation;}
	virtual QString Test(const QString &message) {return "I'm oke(" + message + ")" + " --> " + strPluginInformation;}	
	virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
	virtual QString GetMinimalMainProgramVersion() {return MAIN_PROGRAM_FILE_VERSION_STRING;};
	
public slots:
	virtual bool IsCompatible() 
	{
		QString strMainProgramMinimalVersion = GetMinimalMainProgramVersion();
		//return (MainAppInfo::isCompatibleVersion(GetMinimalMainProgramVersion(),QString(MAIN_PROGRAM_FILE_VERSION_STRING)));			
		QStringList lstMainProgramCurrentVersion = QString(MAIN_PROGRAM_FILE_VERSION_STRING).split(".");
		QStringList lstMainProgramMinimalVersion = strMainProgramMinimalVersion.split(".");
		if ((lstMainProgramCurrentVersion.count() == 4) && (lstMainProgramMinimalVersion.count() == 4))
		{
			for (int i = 0;i<4;i++)
			{
				if(lstMainProgramCurrentVersion.at(i).toInt() > lstMainProgramMinimalVersion.at(i).toInt())
				{
					return true;//Later version
				}
				else if(lstMainProgramCurrentVersion.at(i).toInt() < lstMainProgramMinimalVersion.at(i).toInt())
				{
					return false;//Earlier version
				}
			}
			return true;//same version
		}
		return false;//wrong arguments
	};
	virtual QString GetAdditionalFileExtensions() {return QString("");};
	virtual QObject *GetScriptMetaObject(int nIndex = 0) {return NULL;};
	virtual int GetScriptMetaObjectCount() {return 0;};

protected:
	QString strPluginInformation;

};


	//////////////////////////////////////////////////////////////////////////


	class DeviceInterface: virtual public PluginInterface
	{
	public:
		virtual ~DeviceInterface() {}
	};


	//////////////////////////////////////////////////////////////////////////


	class ExtensionInterface: virtual public PluginInterface
	{
	public:
		virtual ~ExtensionInterface() {}
	};

	QT_BEGIN_NAMESPACE
		Q_DECLARE_INTERFACE(DeviceInterface,    MAIN_PROGRAM_DEVICE_INTERFACE);
		Q_DECLARE_INTERFACE(ExtensionInterface, MAIN_PROGRAM_EXTENSION_INTERFACE);
	QT_END_NAMESPACE

#endif // PLUGININTERFACE_H
