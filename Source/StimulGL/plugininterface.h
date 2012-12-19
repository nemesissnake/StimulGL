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
//#include "mainappinfo.h"
#include "GlobalApplicationInformation.h"

class PluginInterface
{
public:
	virtual ~PluginInterface() {}
	virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
	virtual QString GetPluginInformation(void) {return strPluginInformation;}
	
	void fetchGlobalAppInfo() {m_MainAppInfo = NULL; m_MainAppInfo = new MainAppInformationStructure(GlobalApplicationInformation::getStaticMainAppInformationStructureFromSharedMemory());};
	void deleteGlobalAppInfo() {delete m_MainAppInfo; m_MainAppInfo = NULL;};
	
public slots:
	virtual bool ShowGUI() = 0;
	virtual QString GetMinimalMainProgramVersion() {return m_MainAppInfo->sFileVersion;};
	virtual bool IsCompatible() 
	{
		QString strMainProgramMinimalVersion = GetMinimalMainProgramVersion();
		QStringList lstMainProgramCurrentVersion = QString(m_MainAppInfo->sFileVersion).split(".");
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
	virtual QStringList GetAdditionalFileExtensions() {return QStringList();};
	virtual QStringList GetAdditionalFileSlotHandlers() {return QStringList();};
	virtual QObject *GetScriptMetaObject(int nIndex = 0) {nIndex = nIndex; return NULL;};
	virtual int GetScriptMetaObjectCount() {return 0;};
	virtual int GetAdditionalFileTypeStyle(QString strExtension) {return GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED;};//should return a DocTypeStyle
	virtual QString GetAdditionalFileTypeApiName(QString strExtension) {return "";};
	
protected:
	QString strPluginInformation;

private:
	MainAppInformationStructure *m_MainAppInfo;
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
