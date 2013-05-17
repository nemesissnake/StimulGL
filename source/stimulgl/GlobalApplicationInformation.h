//StimulGL
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


#ifndef GLOBALAPPLICATIONINFORMATION_H
#define GLOBALAPPLICATIONINFORMATION_H

#include <QCoreApplication>
#include <QSettings>
#include "svgview.h"
#include "maindefines.h"

typedef struct strcDocContentInfoStructure
{
	QString strDocContent;
	QString strDocExtension;
	QString strDocHomeDir;
	bool bIsFile;
} DocContentInfoStructure;

typedef struct strMainAppInformation 
{
	QString sCompanyName;
	QString sInternalName;
	QString sFileVersion;
	bool bDoNotLoadScriptBindings;
	bool bOpenExtDebug;
	SvgView::RendererType rRendererType;
	bool bHQAntiAlias;
	bool bAllowMultipleInheritance;
	bool bEnableNetworkServer;
	QString sHostAddress;
	quint16 nHostPort;
	//See also serialization!!!
} MainAppInformationStructure;

class GlobalApplicationInformation : public QObject
{
	Q_OBJECT

public:
	GlobalApplicationInformation(QObject *parent = NULL);
	~GlobalApplicationInformation();

	MainAppInformationStructure getMainAppInformationStructure() {return mainAppInformation;};
	static MainAppInformationStructure getStaticMainAppInformationStructureFromSharedMemory();
	static bool copyMainAppInformationStructureToSharedMemory(const MainAppInformationStructure &mainAppInformationStructure);

	void setCompanyName(const QString &sName);
	QString getCompanyName();
	void setFileVersionString(const QString &sVersion);
	QString getFileVersionString();
	void setInternalName(const QString &sIntName);
	QString getInternalName(); 
	QString getTitle();
	bool shouldLoadScriptBindings();
	
	bool shouldEnableNetworkServer();
	QString getHostAddress();
	quint16 getHostPort();

	void initAndParseRegistrySettings();

	bool checkRegistryInformation(const QString &sName);
	QVariant getRegistryInformation(const QString &sName);
	bool setRegistryInformation(const QString &sName, const QVariant &vValue, const QString &sType);

	QVariant invokeJavaScriptConfigurationFile(const QString &sCode);
	void showJavaScriptConfigurationFile();
	

public slots:
	void parseJavaScriptConfigurationFile();

private:
	void Initialize();
	void composeJavaScriptConfigurationFile();
	//bool fetchMainAppInformationStructure();

	MainAppInformationStructure mainAppInformation;
	QSettings *AppRegistrySettings;
	QObject *webView;

public:
	enum ActiveScriptMode
	{
		NoScript				= 0x00000,
		Pending					= 0x00001,
		Executing				= 0x00002,
		Debugging				= 0x00003,
		Stopping				= 0x00004
	};
	Q_DECLARE_FLAGS(ActiveScriptModes, ActiveScriptMode)	

	enum ScriptRunMode
	{
		Execute					= 0x00000,
		Debug					= 0x00001,
		Abort					= 0x00002
	};
	Q_DECLARE_FLAGS(ScriptRunModes, ScriptRunMode)

	enum MainProgramModeFlag
	{
		Default						= 0,
		DisableAllScriptBindings	= 1,
		DisableAllPlugins			= 2,
		DisableSplash				= 4
	};
	Q_DECLARE_FLAGS(MainProgramModeFlags, MainProgramModeFlag)

	enum DocType 
	{
		DOCTYPE_UNDEFINED		= 0x00000,
		DOCTYPE_QSCRIPT			= 0x00001,
		DOCTYPE_JAVASCRIPT		= 0x00002,
		DOCTYPE_SVG				= 0x00003,
		DOCTYPE_PLUGIN_DEFINED	= 0x00004
	};
	Q_DECLARE_FLAGS(DocTypes, DocType)

	enum DocTypeStyle
	{
		DOCTYPE_STYLE_UNDEFINED		= 0,
		DOCTYPE_STYLE_ECMA			= 1,
		DOCTYPE_STYLE_PLAINTEXT		= 2,
		DOCTYPE_STYLE_XML			= 3,
		DOCTYPE_STYLE_QML			= 4
	};
	Q_DECLARE_FLAGS(DocTypeStyles, DocTypeStyle)
};

Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalApplicationInformation::ScriptRunModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalApplicationInformation::MainProgramModeFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalApplicationInformation::ActiveScriptModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalApplicationInformation::DocTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(GlobalApplicationInformation::DocTypeStyles)

QDataStream &operator<<(QDataStream &out, const MainAppInformationStructure &mainAppInformationStructure);
QDataStream &operator>>(QDataStream &in, MainAppInformationStructure &mainAppInformationStructure);

#endif // GLOBALAPPLICATIONINFORMATION_H
