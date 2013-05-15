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

#include "mainappinfo.h"
#include <QTextStream>
#include <QString>
#include <QMetaEnum>
#include <QDebug>
#include <QMessageBox>

QFile *MainAppInfo::mainLogFile = NULL;//Needed to initialize the static variable!
QWidget *MainAppInfo::mainWindow = NULL;//Needed to initialize the static variable!

QDir MainAppInfo::appDebugDirPath()
{
	QDir appDebugDir = QDir(appDirPath());
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
	return appDebugDir.absolutePath();
}

QString MainAppInfo::pluginsDirPath()
{
	QDir pluginsDir = appDebugDirPath();
	pluginsDir.cd("plugins");
	if(!pluginsDir.exists())
		return appDebugDirPath().absolutePath();
	#ifdef WIN64
		pluginsDir.cd("x64");
	#else
		pluginsDir.cd("Win32");
	#endif
	//if(!pluginsDir.exists())
	//	return appDebugDirPath().absolutePath();
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

QString MainAppInfo::qmlExtensionsPluginDirPath()
{
	QString strExtPluginDirPath = appDirPath() + QString("/qml/plugins/");
	#ifdef WIN64
		strExtPluginDirPath = strExtPluginDirPath + QString("x64");
	#else
		strExtPluginDirPath = strExtPluginDirPath + QString("Win32");
	#endif

//#ifdef DEBUG
//		QString strConfiguration = "/Debug";
//#else
//		QString strConfiguration = "/Release";
//#endif
	
	if(QDir(strExtPluginDirPath).exists()==false)
	{
		QDir().mkdir(strExtPluginDirPath);
	}
	else if(QDir(strExtPluginDirPath).exists()) // + strConfiguration).exists())
	{
		strExtPluginDirPath = strExtPluginDirPath;// + strConfiguration;
	}
	return QDir(strExtPluginDirPath).absolutePath();
}

QStringList MainAppInfo::getQTScriptBindingList()	
{
#ifdef WIN32
	return	QStringList() << "qt.core" << "qt.gui" << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.webkit"; //strange enough these do not jet work on winXP! << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon";//<< "qt.network" does work...
#else//64 bit
	return	QStringList() << "qt.core" << "qt.gui" << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.webkit"; //strange enough these do not jet work on winXP! << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon";
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

void MainAppInfo::MyOutputHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) 
{
	bool bDoAbort = false;
	if(MainAppInfo::mainLogFile == NULL)
	{
		//QString aa = MainAppInfo::appLogFilePath();
		MainAppInfo::mainLogFile = new QFile(MainAppInfo::appLogFilePath());
	}
	if(!MainAppInfo::mainLogFile->isOpen())
	{
		qint64 nSize = MainAppInfo::mainLogFile->size();
		if(nSize > 41943040)//40 MegaByte
		{
			QMessageBox::StandardButton ret = QMessageBox::warning(NULL, "LogFile exceeds file size!",
				"The StimulGL LogFile(" + MainAppInfo::appLogFilePath() + ") exceeds the 40Mb file size.\n"
					"Do you want StimulGL to clear this LogFile for you?", QMessageBox::Yes | QMessageBox::No);
				if (ret == QMessageBox::Yes)
				{
					MainAppInfo::mainLogFile->open(QIODevice::WriteOnly);
				}
				else
				{
					MainAppInfo::mainLogFile->open(QIODevice::WriteOnly | QIODevice::Append);
				}
		}
		else
		{
			MainAppInfo::mainLogFile->open(QIODevice::WriteOnly | QIODevice::Append);
		}
		if(!MainAppInfo::mainLogFile->isOpen())
		{
            MainAppInfo::mainLogFile = NULL;
			return;
		}
	}
	QString strMessage;
	bool bSkipErrorForLogWindow = false;
	bSkipErrorForLogWindow = ( (msg == QString("Unknown error")) || (msg == QString("QGLShader::link: \"No errors.\" ")) );
	bSkipErrorForLogWindow = bSkipErrorForLogWindow || (msg.contains("Remove me: fixing toplevel window flags"));
	bSkipErrorForLogWindow = bSkipErrorForLogWindow || (msg.contains("warning X3206: implicit truncation of vector type"));
	switch (type) 
	{
	case QtDebugMsg:
#ifdef QT_NO_DEBUG
		strMessage = QString("Debug: %1").arg(msg);
#else
		strMessage = QString("Debug: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
#endif
		break;
	case QtWarningMsg:
#ifdef QT_NO_DEBUG
		strMessage = QString("Warning: %1").arg(msg);
#else
		strMessage = QString("Warning: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
#endif
		break;
	case QtCriticalMsg:
		strMessage = QString("Critical: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		break;
	case QtFatalMsg:
		strMessage = QString("Fatal: %1 (%2:%3, %4)").arg(msg).arg(context.file).arg(context.line).arg(context.function);
		bDoAbort = true;		
	}
	QTextStream ts(mainLogFile);
	ts << QDate::currentDate().toString().toLatin1().data() << QString("\t") << QTime::currentTime().toString().toLatin1().data() << QString("\t") << strMessage << QString("\n"); //endl macro doesn't work?
	if(MainAppInfo::getMainWindow())
	{
		if(bSkipErrorForLogWindow == false)
			QMetaObject::invokeMethod((QObject *)MainAppInfo::getMainWindow(), MainAppInfo::getMainWindowLogSlotName().toLatin1(), Qt::DirectConnection, Q_ARG(QString, strMessage));
	}	
	if (bDoAbort)
	{
		CloseMainLogFile();
		abort();
	}
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

bool MainAppInfo::SetMainWindow(QWidget *mainWin)
{
	mainWindow = mainWin;
	return true;
}

bool MainAppInfo::isCompatibleVersion(const QString &strMinimalRequiredVersion, const QString &strCurrentVersion)
{
	QStringList lstCurrentVersion = strCurrentVersion.split(".");
	QStringList lstMinimalVersion = strMinimalRequiredVersion.split(".");
	if ((lstCurrentVersion.count() == 4) && (lstMinimalVersion.count() == 4))
	{
		for (int i = 0;i<4;i++)
		{
			if(lstCurrentVersion.at(i).toInt() > lstMinimalVersion.at(i).toInt())
			{
				return true;//Later version
			}
			else if(lstCurrentVersion.at(i).toInt() < lstMinimalVersion.at(i).toInt())
			{
				return false;//Earlier version
			}
		}
		return true;//same version
	}
	return false;//wrong arguments
}

bool MainAppInfo::CreateHashTableFromEnumeration(const QString &sTypeName, QHash<QString, int> &hTable, const QMetaObject metaObject)
{
	if(!sTypeName.isEmpty())
	{
		//int test = metaObject.enumeratorCount();
		//QMetaEnum testmetaEnum = metaObject.enumerator(test-1);
		//QString teststr = testmetaEnum.name();

		QStringList tmpList = sTypeName.split("::",QString::SkipEmptyParts);
		QString sShortTypeName = tmpList.at(tmpList.count()-1);
		int enumeratorIndex = metaObject.indexOfEnumerator(sShortTypeName.toLatin1());
		if (enumeratorIndex >= 0)
		{
			QMetaEnum metaEnum = metaObject.enumerator(enumeratorIndex);
			int nKeyCount = metaEnum.keyCount();
			for (int i=0;i<nKeyCount;i++)
			{
				hTable.insert(metaEnum.key(i),metaEnum.value(i));
			}
			if (hTable.count() > 0)
				return true;
			return false;
		}
		else
		{
			qDebug() << __FUNCTION__ << "::Unknown Enumeration Type (" << sShortTypeName << ")!";	
			return false;
		}
		return true;
	}
	return false;
}
