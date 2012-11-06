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

#include "../../StimulGL/plugininterface.h"
#include "ExperimentManager_dialog.h"
#include "ExperimentManager.h"
#include "ImageProcessor.h"
#include "prtformatmanager.h"
#include "TriggerTimer.h"
#include "defines.h"

class ExperimentManagerPlugin : public QObject, ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)

public:
	ExperimentManagerPlugin(QObject *parent = 0);
	~ExperimentManagerPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString GetMinimalMainProgramVersion() {return PLUGIN_MAIN_PROGRAM_MINIMAL_VERSION;};

private:
	bool ExecuteContent(const DocContentInfoStructure &docStruct);

	ExperimentManager *ExperimentManagerObject; 
	ExperimentManager_Dialog *ExperimentManagerDiagObject;
	TriggerTimer *TriggerTimerObject;
	ImageProcessor *ImageProcessorObject;
	PrtFormatManager *PrtFormatManagerObject;
	qmlWidget *QmlWidgetObject;

public slots:
    bool ShowGUI();
	bool IsCompatible() {return PluginInterface::IsCompatible();};
	QStringList GetAdditionalFileExtensions() {return (QStringList() << "Experiment files (*.exml);;" << "QML documents (*.qml);;");};
	QStringList GetAdditionalFileSlotHandlers() {return (QStringList() << "exml|ExperimentManagerEXMLDocumentHandler(const QString &, const QString &)" << "qml|ExperimentManagerQMLDocumentHandler(const QString &, const QString &)");};
	bool ExperimentManagerEXMLDocumentHandler(const QString &docContent, const QString &strHomePath);
	bool ExperimentManagerQMLDocumentHandler(const QString &docContent, const QString &strHomePath);
	QObject *GetScriptMetaObject(int nIndex);
	int GetScriptMetaObjectCount();
	int GetAdditionalFileTypeStyle(QString strExtension);
	QString GetAdditionalFileTypeApiName(QString strExtension);
};

#endif//ExperimentManagerPLUGIN_H
