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


#include "mainappinfo.h"
#include <QTextStream>
#include <QString>

QFile * MainAppInfo::mainLogFile = NULL;//Needed to initialize the static variable!
//std::ofstream * MainAppInfo::mainLogFile = NULL;//Needed to initialize the static variable!

QDir MainAppInfo::appDebugDirPath()
{
	QDir appDebugDir = QDir(appDirPath());
	QString strAppDebugDir = appDirPath();
#if defined(Q_OS_WIN)
	if (appDebugDir.dirName().toLower() == "debug" || appDebugDir.dirName().toLower() == "release")
	{
		appDebugDir.cdUp();
		if (appDebugDir.dirName().toLower() == "win32" || appDebugDir.dirName().toLower() == "x64")//This is now unnecessary in VS2010! It is solved by changing the debugpath to the install dir.
		{
			appDebugDir.cdUp();
			appDebugDir.cdUp();
			appDebugDir.cdUp();
			appDebugDir.cd("Install");			
		}
	}
#elif defined(Q_OS_MAC)
	if (appDebugDir.dirName() == "MacOS") {
		appDebugDir.cdUp();
		appDebugDir.cdUp();
		appDebugDir.cdUp();
	}
#endif
	strAppDebugDir = appDebugDir.absolutePath();
	return strAppDebugDir;
}

QString MainAppInfo::pluginsDirPath()
{
	QDir pluginsDir = appDebugDirPath();
	pluginsDir.cd("plugins");
	#ifdef WIN64
		pluginsDir.cd("x64");
	#else
		pluginsDir.cd("Win32");
	#endif
	return pluginsDir.absolutePath();
}

QString MainAppInfo::outputsDirPath()
{
	QDir pluginsDir = appDirPath();
	if(QDir(appDirPath() + QString("/outputs/")).exists()==false)
	{
		QDir().mkdir(QString(appDirPath()) + QString("/outputs/"));
	}

	pluginsDir.cd("outputs");
	return pluginsDir.absolutePath();
}

QStringList MainAppInfo::getQTScriptBindingList()	
{
#ifdef WIN32
	return	QStringList() << "qt.core" << "qt.gui" << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon"; //strange enough these do not jet work on winXP! << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon";//<< "qt.network" does work...
#else//64 bit
	return	QStringList() << "qt.core" << "qt.gui" << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon"; //strange enough these do not jet work on winXP! << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon";
#endif
}

//QString MainAppInfo::ScriptBindingsDirPath()
//{
//	QDir bindingsDir = pluginsDirPath();
//	bindingsDir.cd("bindings");
//	return bindingsDir.absolutePath();
//}

QString MainAppInfo::apiDirPath()
{
	QDir pluginsDir = appDebugDirPath();
	pluginsDir.cd("apis");
	return pluginsDir.absolutePath();
}

void MainAppInfo::MyOutputHandler(QtMsgType type, const char *msg) 
{
	if(MainAppInfo::mainLogFile == NULL)
	{
		MainAppInfo::mainLogFile = new QFile(MainAppInfo::appLogFilePath());
	}
	if(!MainAppInfo::mainLogFile->isOpen())
	{
		MainAppInfo::mainLogFile->open(QIODevice::WriteOnly | QIODevice::Append);
		if(!MainAppInfo::mainLogFile->isOpen())
		{
            MainAppInfo::mainLogFile = NULL;
			return;
		}
	}
	QString strMessage;
	switch (type) {
	case QtDebugMsg:
		strMessage = QString("Debug: %1").arg(msg);
		break;
	case QtWarningMsg:
		strMessage = QString("Warning: %1").arg(msg);
		break;
	case QtCriticalMsg:
		strMessage = QString("Critical: %1").arg(msg);
		break;
	case QtFatalMsg:
		strMessage = QString("Fatal: %1").arg(msg);
		abort();
	}
	QTextStream ts(mainLogFile);
	ts << QDate::currentDate().toString().toAscii().data() << QString("\t") << QTime::currentTime().toString().toAscii().data() << QString("\t") << strMessage << endl;

}

void MainAppInfo::CloseMainLogFile()
{
	if(MainAppInfo::mainLogFile != NULL)
	{
		if(MainAppInfo::mainLogFile->isOpen())
		{
			MainAppInfo::mainLogFile->close();
		}
	}	
}

bool MainAppInfo::Initialize()
{
	bool bRetval = false;
	bRetval = InitializeMainAppNaming();
	return bRetval;
}

bool MainAppInfo::InitializeMainAppNaming()
{
	QCoreApplication::setOrganizationDomain(MAIN_PROGRAM_COMPANY_NAME);
	QCoreApplication::setOrganizationName(MAIN_PROGRAM_COMPANY_NAME);
	QCoreApplication::setApplicationName(MAIN_PROGRAM_INTERNAL_NAME);
	QCoreApplication::setApplicationVersion(MAIN_PROGRAM_FILE_VERSION_STRING);
	return true;
}


