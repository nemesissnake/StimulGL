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


#ifndef MAINAPPINFO_H
#define MAINAPPINFO_H

#include <QCoreApplication>
#include <QDir>
#include <QString>
#include <QTime>
#include <iostream>
#include <fstream>

#include "maindefines.h"

typedef struct strcDocContentInfoStructure
{
	QString strDocContent;
	QString strDocExtension;
	QString strDocHomeDir;
	bool bIsFile;
} DocContentInfoStructure;

class MainAppInfo 
{
private:
	static QFile *mainLogFile;
	static QWidget *mainWindow;
public:

	static bool Initialize(QWidget *mainWin = NULL);
	static bool InitializeMainAppNaming();
	static QString MainProgramName()				{return MAIN_PROGRAM_INTERNAL_NAME;}
	static QString MainOrganizationName()			{return MAIN_PROGRAM_COMPANY_NAME;}
	static QString MainProgramFileVersion()			{return MAIN_PROGRAM_FILE_VERSION_STRING;}
	static QString MainProgramTitle()				{return MAIN_PROGRAM_FULL_NAME;}//(MainProgramName() + "(v" + MainProgramFileVersion() + ")");}
	static QString appDirPath()						{return QCoreApplication::applicationDirPath();}
	static QString appDocDirPath()					{return (appDirPath() + QDir::separator() + MAIN_PROGRAM_DOC_DIRNAME + QDir::separator());}
	static QString appExampleDirPath()				{return (appDirPath() + QDir::separator() + MAIN_PROGRAM_EXAMPLES_DIRNAME + QDir::separator());}
	static QString appLogFilePath()					{return (appDirPath() + QDir::separator() + MAIN_PROGRAM_LOGFILE_NAME);}
	static QString appXsdFilePath()					{return (appDirPath() + QDir::separator() + MAIN_PROGRAM_XSD_DIRNAME + QDir::separator());}
	static QString pluginsDirPath();
	static QString outputsDirPath();
	static QString qmlExtensionsPluginDirPath();
	static QWidget* getMainWindow()					{return mainWindow;};
	static QString getMainWindowLogSlotName()		{return MAIN_PROGRAM_LOG_SLOT_NAME;};
	static QString getScriptApiClassName()			{return SCRIPT_API_CLASS_NAME_TAG;};


	static QString getDefaultFileExtList()			{return QString (MAIN_PROGRAM_FILESEXTENSION_LIST);}
	static QStringList getQTScriptBindingList();
	static QString apiDirPath();
	static QString UntitledDocName()				{return MAIN_PROGRAM_UNTITLED_NAME;}
	static QString stdDateTimeFormat()				{return MAIN_PROGRAM_STANDARD_DATETIME_FORMAT;}

	static bool isCompatibleVersion(const QString &strMinimalRequiredVersion, const QString &strCurrentVersion);

	static void MyOutputHandler(QtMsgType type, const char *msg); 
	static void CloseMainLogFile();
	static bool CreateHashTableFromEnumeration(const QString &sTypeName, QHash<QString, int> &hTable, const QMetaObject metaObject);

private:
	static QDir appDebugDirPath();

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
		DOCTYPE_SVG				= 0x00002,
		DOCTYPE_PLUGIN_DEFINED	= 0x00003
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

Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::ScriptRunModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::MainProgramModeFlags)
Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::ActiveScriptModes)
Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::DocTypes)
Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::DocTypeStyles)

#endif // MAINAPPINFO_H
//#ifdef Q_WS_WIN
//#ifdef Q_WS_X11
//#ifdef Q_WS_MACX
