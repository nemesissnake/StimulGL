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


/****************************************************************************
**
****************************************************************************/

#include <QApplication>
#include <QString>
#include <QtGui>
#ifndef QT_NO_OPENGL
#include <QGLFormat>
#endif
#include <iostream>
#include "mainwindow.h"
#include "mainappinfo.h"

using namespace std;

class MainWindow;

int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(StimulGL);

	QApplication app(argc, argv);
	QString PluginDir = MainAppInfo::pluginsDirPath();
	app.addLibraryPath(PluginDir);

	//only for console window, change Linker->System->SubSystem..
	//cerr << "=-=-=-=- Start of Program -=-=-=-=" << endl;
	//qDebug() <<"=-=-=-=- Start of Program -=-=-=-=";
	//qWarning() << "Usage: mainwindow [-SizeHint<color> <width>x<height>] ...";
	//QMessageBox msgBox(QMessageBox::Information,"PluginDirInfo",PluginDir,QMessageBox::Ok);
	//msgBox.exec();

	MainWindow *window;
	window = new MainWindow();
	MainAppInfo::MainProgramModeFlags flags = MainAppInfo::Default;

	if (argc > 2)
	{
		QString tempStr = "";
		for (int i = 1; i<argc;i++)
		{
			tempStr = argv[i];
			if (tempStr == "-f" | tempStr == "-F")//path exists?
			{				
				if (i<(argc-1))//another argument available?
				{
					i = i + 1;
					window->setStartupFiles(argv[i]);//separate multiple files using a ';' Character!
				}
			}
			else if (tempStr == "-o" | tempStr == "-O")//valid argument?
			{
				if (i<(argc-1))//another argument available?
				{
					i = i + 1;
					tempStr = argv[i];					
					flags = (MainAppInfo::Default | MainAppInfo::MainProgramModeFlag(tempStr.toInt()));
				}				
			}
		}
	}
	else if (argc == 2)//only path declared!
	{
		window->setStartupFiles(argv[1]);
	}
	else
	{}
	window->initialize(flags);
    window->showMaximized();
	return app.exec();
}
