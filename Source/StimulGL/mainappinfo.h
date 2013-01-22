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

class MainAppInfo 
{
private:
	static QFile *mainLogFile;
	static QWidget *mainWindow;

public:
	static bool Initialize(QWidget *mainWin);
	static QString appDirPath()						{return QDir(QCoreApplication::applicationDirPath()).absolutePath();}
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

	static void MyOutputHandler(QtMsgType type, const QMessageLogContext &context, const char *msg);
	static void CloseMainLogFile();
	static bool CreateHashTableFromEnumeration(const QString &sTypeName, QHash<QString, int> &hTable, const QMetaObject metaObject);

private:	
	static QDir appDebugDirPath();
};

#endif // MAINAPPINFO_H

