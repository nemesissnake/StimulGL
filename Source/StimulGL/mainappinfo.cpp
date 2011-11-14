#include "mainappinfo.h"


QDir MainAppInfo::appDebugDirPath()
{
	QDir appDebugDir = QDir(appDirPath());
#if defined(Q_OS_WIN)
	if (appDebugDir.dirName().toLower() == "debug" || appDebugDir.dirName().toLower() == "release")
		appDebugDir.cdUp();
	if (appDebugDir.dirName().toLower() == "win32" || appDebugDir.dirName().toLower() == "x64")
		appDebugDir.cdUp();
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
	return	QStringList() << "qt.core" << "qt.gui" << "qt.opengl"; //strange enough these do not jet work on winXP! << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon";//<< "qt.network" does work...
#else//64 bit
	return	QStringList() << "qt.core" << "qt.gui"; //strange enough these do not jet work on winXP! << "qt.xml" << "qt.xmlpatterns" << "qt.svg" << "qt.sql" << "qt.opengl" << "qt.uitools" << "qt.phonon";
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
