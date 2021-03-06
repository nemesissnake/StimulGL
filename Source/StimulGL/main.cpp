//StimulGL
//Copyright (C) 2014  Sven Gijsen
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


/****************************************************************************
**
****************************************************************************/

#include <QApplication>
#include <QString>
#include <QtWidgets>
#ifndef QT_NO_OPENGL
#include <QGLFormat>
#endif
#include <iostream>
#include "mainwindow.h"
#include "mainappinfo.h"
#include "mainappexchange.h"

using namespace std;

class MainWindow;

int main(int argc, char **argv)
{
	bool bProceed = true;
	//QString sResult = QDir::toNativeSeparators(QDir(QCoreApplication::applicationDirPath()).absolutePath());
	//if(QProcessEnvironment::systemEnvironment().value(QString("QT_QPA_PLATFORM_PLUGIN_PATH"),sResult) == false)
	//{
	//	qputenv("QT_QPA_PLATFORM_PLUGIN_PATH",sResult.toLatin1());
	//}
	MainAppExchange appExchange(argc, argv, MAIN_PROGRAM_SHARED_MEM_KEY);
	GlobalApplicationInformation *globAppInformation = appExchange.getGlobalAppInformationObjectPointer();
	if (appExchange.isRunning())
	{
		appExchange.sendMessage("Log(\"New StimulGL Instance Initializing...\");");
		//if (appExchange.getSharedDataSegment("AllowMultipleInstance") == "false")
		if(globAppInformation->checkRegistryInformation(REGISTRY_ALLOWMULTIPLEINHERITANCE) == false)
			bProceed = false;
		if(globAppInformation->getRegistryInformation(REGISTRY_ALLOWMULTIPLEINHERITANCE) == false)
		{
			QStringList sFilesToOpen;
			int i;
			bool bExecuteDocument = false;
			if (argc > 2)
			{
				QString tempStr = "";
				for (i = 1; i<argc;i++)
				{
					tempStr = argv[i];
					if (tempStr == "-f" | tempStr == "-F")//path exists?
					{				
						if (i<(argc-1))//another argument available?
						{
							i = i + 1;
							sFilesToOpen.append(QString(argv[i]).split(";"));//separate multiple files (cmd) using a ';' Character!
						}
					}
					else if (tempStr == "-e" | tempStr == "-E")//Immediately execute the loaded (-f <document) document?
					{
						bExecuteDocument = true;
					}
				}
			}
			else if (argc == 2)//only path declared!
			{
				sFilesToOpen.append(QString(argv[1]).split(";"));
			}
			if(sFilesToOpen.isEmpty() == false)
			{
				for(i=0;i<sFilesToOpen.count();i++)
				{
					sFilesToOpen[i] = "\"" + QDir::cleanPath(sFilesToOpen.at(i)) + "\"";					
				}
				//StimulGL.openFiles(null,["E:/temp/1.txt","E:/temp/2.txt"]);
				QString sFileCombination = sFilesToOpen.join(",");
				appExchange.sendMessage("StimulGL.openFiles(null,[" + sFileCombination  + "]);");
			}
			appExchange.sendMessage("StimulGL.activateMainWindow();");
			if(bExecuteDocument)
				appExchange.sendMessage("StimulGL.executeActiveDocument();");
			bProceed = false;
		}
	}
	//else//First occurrence
	//{
	//	//globAppInformation->setMainAppInformationStructure();
	//}

	if(bProceed)
	{
		Q_INIT_RESOURCE(StimulGL);

		//QApplication app(argc, argv);
		QString PluginDir = MainAppInfo::pluginsDirPath();
		//app.addLibraryPath(PluginDir);
		appExchange.addLibraryPath(PluginDir);

		//only for console window, change Linker->System->SubSystem..
		//cerr << "=-=-=-=- Start of Program -=-=-=-=" << endl;
		//qDebug() <<"=-=-=-=- Start of Program -=-=-=-=";
		//qWarning() << "Usage: mainwindow [-SizeHint<color> <width>x<height>] ...";
		//QMessageBox msgBox(QMessageBox::Information,"PluginDirInfo",PluginDir,QMessageBox::Ok);
		//msgBox.exec();

		MainWindow *appWindow = new MainWindow();
		appWindow->setGlobalApplicationInformationObject(globAppInformation);
		MainAppInfo::setMainWindow(appWindow);
		GlobalApplicationInformation::MainProgramModeFlags flags = GlobalApplicationInformation::Default;
		QString tempStr = "";
		if (argc > 2)
		{
			for (int i = 1; i<argc;i++)
			{
				tempStr = argv[i];
				if (tempStr == "-f" | tempStr == "-F")//path exists?
				{				
					if (i<(argc-1))//another argument available?
					{
						i = i + 1;
						appWindow->setStartupFiles(argv[i]);//separate multiple files (cmd) using a ';' Character!
					}
				}
				else if (tempStr == "-o" | tempStr == "-O")//valid argument?
				{
					if (i<(argc-1))//another argument available?
					{
						i = i + 1;
						tempStr = argv[i];					
						flags = (flags | GlobalApplicationInformation::MainProgramModeFlag(tempStr.toInt()));
					}				
				}
				else if (tempStr == "-v" | tempStr == "-V")//verbose mode?
				{
					flags = (flags | GlobalApplicationInformation::VerboseMode);
				}
				else if (tempStr == "-e" | tempStr == "-E")//Immediately execute the loaded (-f <document) document?
				{
					flags = (flags | GlobalApplicationInformation::ExecuteDocument);
				}
			}
		}
		else if (argc == 2)//only one parameter defined!
		{
			tempStr = argv[1];
			if (tempStr == "-v" | tempStr == "-V")//verbose mode?
			{
				flags = (flags | GlobalApplicationInformation::VerboseMode);
			}
			//else if (tempStr == "-e" | tempStr == "-E")//Immediately execute the loaded (-f <document) document? --> doesn't work here because there's no file loaded, too few parameters
			//{
				//flags = (flags | GlobalApplicationInformation::ExecuteDocument);
			//}
			else
			{
				appWindow->setStartupFiles(argv[1]);
			}			
		}
		if (appExchange.isRunning())
			flags = flags | GlobalApplicationInformation::DisableNetworkServer;

		appWindow->initialize(flags);
		// connect message queue to the main window.
		QObject::connect(&appExchange, SIGNAL(messageAvailable(QString)), appWindow, SLOT(receiveExchangeMessage(QString)));
		appWindow->show();//showMaximized() creates weird behaviour!;
		appWindow->RecoverLastScreenWindowSettings();
		int nRetVal = appExchange.exec();
		delete appWindow;
		appWindow = NULL;
		return nRetVal;
	}
	return 0;
}
