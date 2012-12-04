//StimulGL
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


#ifndef GLOBALAPPLICATIONINFORMATION_H
#define GLOBALAPPLICATIONINFORMATION_H

#include <QCoreApplication>

typedef struct strMainAppInformation 
{
	QString sCompanyName;
	QString sInternalName;
	QString sFileVersion;

	//QString sDeviceInterfaceString;
	//QString sExtensionInterfaceString;
} MainAppInformationStructure;

class GlobalApplicationInformation : public QObject
{
	Q_OBJECT

public:
	GlobalApplicationInformation(QObject *parent = NULL)
	{
		parent = parent;
		mainAppInformation.sCompanyName = "";
		mainAppInformation.sFileVersion = "";
		mainAppInformation.sInternalName = "";
		//mainAppInformation.sDeviceInterfaceString = "";
		//mainAppInformation.sExtensionInterfaceString = "";
	};
	~GlobalApplicationInformation(){};

	void setCompanyName(const QString &sName)			{mainAppInformation.sCompanyName = sName;QCoreApplication::setOrganizationDomain(mainAppInformation.sCompanyName);QCoreApplication::setOrganizationName(mainAppInformation.sCompanyName);};
	QString getCompanyName()							{return mainAppInformation.sCompanyName;};
	void setFileVersionString(const QString &sVersion)	{mainAppInformation.sFileVersion = sVersion;QCoreApplication::setApplicationVersion(mainAppInformation.sFileVersion);};
	QString getFileVersionString()						{return mainAppInformation.sFileVersion;};
	void setInternalName(const QString &sIntName)		{mainAppInformation.sInternalName = sIntName;QCoreApplication::setApplicationName(mainAppInformation.sInternalName);};
	QString getInternalName()							{return mainAppInformation.sInternalName;};
	QString getTitle()									{return mainAppInformation.sInternalName +  "(v" + mainAppInformation.sFileVersion + ")";};



	//void setPluginDeviceInterfaceString(const QString &sValue) {mainAppInformation.sDeviceInterfaceString = sValue;};
	//QString getPluginDeviceInterfaceString() {return mainAppInformation.sDeviceInterfaceString;};
	//void setPluginExtensionInterfaceString(const QString &sValue) {mainAppInformation.sExtensionInterfaceString = sValue;};
	//QString getPluginExtensionInterfaceString() {return mainAppInformation.sExtensionInterfaceString;};



private:
	strMainAppInformation mainAppInformation;
};

#endif // GLOBALAPPLICATIONINFORMATION_H
