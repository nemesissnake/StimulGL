#ifndef STIMULGL_H
#define STIMULGL_H

//#include <QtCore/QCoreApplication>
//#include <QtCore/QDir>
//#include <QtCore/QString>
//
//#include <QTime>
//#include <iostream>
//#include <fstream>
//
//static std::ofstream mainLogFile;
//
//static void MyOutputHandler(QtMsgType type, const char *msg) 
//{
//	switch (type) {
//		case QtDebugMsg:
//			mainLogFile << QTime::currentTime().toString().toAscii().data() << " Debug: " << msg << std::endl;
//			break;
//		case QtCriticalMsg:
//			mainLogFile << QTime::currentTime().toString().toAscii().data() << " Critical: " << msg << std::endl;
//			break;
//		case QtWarningMsg:
//			mainLogFile << QTime::currentTime().toString().toAscii().data() << " Warning: " << msg << std::endl;
//			break;
//		case QtFatalMsg:
//			mainLogFile << QTime::currentTime().toString().toAscii().data() <<  " Fatal: " << msg << std::endl;
//			abort();
//	}
//}
//
//class MainAppInfo {
//public:
#define STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_RED	235
#define STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_GREEN	235
#define STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_BLUE	235

//	enum ActiveScriptMode
//	{
//		NoScript				= 0x00000,
//		Pending					= 0x00001,
//		Executing				= 0x00002,
//		Debugging				= 0x00003,
//		Stopping				= 0x00004
//	};
//	Q_DECLARE_FLAGS(ActiveScriptModes, ActiveScriptMode)	
//
//		enum ScriptRunMode
//	{
//		Execute					= 0x00000,
//		Debug					= 0x00001,
//		Abort					= 0x00002
//	};
//	Q_DECLARE_FLAGS(ScriptRunModes, ScriptRunMode)
//
//		enum MainProgramModeFlag
//	{
//		Default					= 0x00000,
//		DisableAllPlugins		= 0x00001,//All Plugins AND QT Script bindings!
//		DisableSplash			= 0x00002
//	};
//	Q_DECLARE_FLAGS(MainProgramModeFlags, MainProgramModeFlag)
//
//		enum DocType 
//	{
//		DOCTYPE_UNDEFINED		= 0x00000,
//		DOCTYPE_QSCRIPT			= 0x00001,
//		DOCTYPE_SVG				= 0x00002,
//		DOCTYPE_PLUGIN_WIDGET	= 0x00003
//	};
//	Q_DECLARE_FLAGS(DocTypes, DocType)
//
//		static QString MainProgramName()		{return "StimulGL";}
//	static QString MainOrganizationName()	{return "MBIC";}
//	static QString MainProgramVersion()		{return "1.0.0.0";}
//	static QString MainProgramTitle()		{return (MainProgramName() + "(v" + MainProgramVersion() + ")");}
//	static QString appDirPath()				{return QCoreApplication::applicationDirPath();}
//	static QString appLogFilePath()			{return (appDirPath() + "/logfile.txt");}
//	static QString pluginsDirPath();
//	static QString outputsDirPath();
//
//	static QString getFileExtList()			{return QString ("QT Script files (*.qs);;SVG files (*.svg *.svgz *.svg.gz);;Any files (*)");}
//	static QStringList getQTScriptBindingList();
//	static QString apiDirPath();
//	static QString UntitledDocName()		{return "Untitled";}
//	static QString stdDateTimeFormat()		{return "yyyyMMddHHmmsszzz";}
//	//static QColor getWindowBackGroundColor(){return QColor(235,235,235);}
//
//private:
//	static QDir appDebugDirPath();
//};
//
//Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::ScriptRunModes)
//Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::MainProgramModeFlags)
//Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::ActiveScriptModes)
//Q_DECLARE_OPERATORS_FOR_FLAGS(MainAppInfo::DocTypes)

#endif // STIMULGL_H


//#ifdef Q_WS_WIN
//#ifdef Q_WS_X11
//#ifdef Q_WS_MACX