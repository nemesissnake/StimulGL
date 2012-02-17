//parallelportplugin
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
**
****************************************************************************/

#ifndef PARALLELPORTPLUGIN_H
#define PARALLELPORTPLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "pport_dialog.h"
#include "ParallelPort.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ParallelPortPlugin : public QObject, DeviceInterface
{
    Q_OBJECT
    Q_INTERFACES(DeviceInterface)

public:
	ParallelPortPlugin(short BaseAddress = 888, QObject *parent = 0);
	~ParallelPortPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString Test(const QString &message);
	QString GetPluginInformation(void);

private:
	QString MainPluginName;
	QString MainPluginOrganizationName;
	QString MainPluginAuthorName;
	QString MainPluginVersion;
	QString MainPluginTitle;

	short nBaseAddress;
	ParallelPort *PPortObject; 
	PPort_Dialog *PPortDiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//PARALLELPORTPLUGIN_H
