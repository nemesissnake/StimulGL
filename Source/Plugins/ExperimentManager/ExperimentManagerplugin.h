//ExperimentManagerplugin
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


//This file inherits the Interface description, please don't change

#ifndef ExperimentManagerPLUGIN_H
#define ExperimentManagerPLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "ExperimentManager_dialog.h"
#include "ExperimentManager.h"
#include "ImageProcessor.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ExperimentManagerPlugin : public QObject, ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)

public:
	ExperimentManagerPlugin(QObject *parent = 0);
	~ExperimentManagerPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString Test(const QString &message);
	QString GetPluginInformation(void);

private:
	QString MainPluginName;
	QString MainPluginAuthorName;
	QString MainPluginOrganizationName;
	QString MainPluginVersion;
	QString MainPluginTitle;

	short nExampleProperty;
	ExperimentManager *ExperimentManagerObject; 
	ExperimentManager_Dialog *ExperimentManagerDiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//ExperimentManagerPLUGIN_H
