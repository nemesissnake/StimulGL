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


#include <QtCore/QByteArray>
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QProcess>
#include <QMessageBox>
#include "assistant.h"

Assistant::Assistant() : proc(0)
{
}

Assistant::~Assistant()
{
    if (proc && proc->state() == QProcess::Running) {
        proc->terminate();
        proc->waitForFinished(3000);
    }
    delete proc;
}

void Assistant::showDocumentation(const QString &page, GlobalApplicationInformation *GlobAppInfo)
{
    if (!startAssistant(GlobAppInfo))
        return;

    QByteArray ba("SetSource ");
    ba.append("qthelp://com.trolltech.stimulgl/doc/");
    
    proc->write(ba + page.toLocal8Bit() + '\n');
}

bool Assistant::startAssistant(GlobalApplicationInformation *GlobAppInfo)
{
    if (!proc)
        proc = new QProcess();

    if (proc->state() != QProcess::Running) 
	{
		QString app;
		QString strHelpPackagePath = MainAppInfo::appDocDirPath() + QLatin1String(MAIN_PROGRAM_DOC_BINARYCOLLFILE);
		QFile file;
		//Does the help binary package exist?
		if(!file.exists(strHelpPackagePath))
		{
			QMessageBox::critical(0, GlobAppInfo->getInternalName(),
				QObject::tr("The help file does not exist (%1)").arg(strHelpPackagePath));
			return false;
		}
		//First we check the Qt path
		app = QLibraryInfo::location(QLibraryInfo::BinariesPath) + QDir::separator();
#if !defined(Q_OS_MAC)
		app += QLatin1String("assistant");
#if defined(Q_OS_WIN)
		app += QLatin1String(".exe");
#endif
#else
		app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");    
#endif
		if(!file.exists(app)) 
		{
			//Then we check the installation path
			app = MainAppInfo::appDirPath() + QDir::separator();
#if !defined(Q_OS_MAC)
			app += QLatin1String("assistant");
#if defined(Q_OS_WIN)
			app += QLatin1String(".exe");
#endif
#else
			app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");    
#endif
		}

        QStringList args;
        args << QLatin1String("-collectionFile") << strHelpPackagePath << QLatin1String("-enableRemoteControl");
        proc->start(app, args);

        if (!proc->waitForStarted()) 
		{
            QMessageBox::critical(0, GlobAppInfo->getInternalName(), QObject::tr("Unable to launch the help Assistant (%1)").arg(app));
            return false;
        }    
    }
    return true;
}
