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


#include "ExperimentManagerplugin.h"
#include "ExperimentManager.h"
#include "ImageProcessor.h"
#include "defines.h"

Q_DECLARE_METATYPE(ExperimentManager*)
//Q_DECLARE_METATYPE(ImageProcessor*)

ExperimentManagerPlugin::ExperimentManagerPlugin(QObject *parent)
{
	ExperimentManagerObject = new ExperimentManager(parent);
	ExperimentManagerDiagObject = new ExperimentManager_Dialog();	
	MainPluginName = QString(PLUGIN_INTERNAL_NAME) + " Plugin";
	MainPluginAuthorName = PLUGIN_AUTHOR_NAME;
	MainPluginOrganizationName = PLUGIN_COMPANY_NAME;
	MainPluginVersion = PLUGIN_FILE_VERSION_STRING;
	MainPluginTitle = MainPluginName + "(v" + MainPluginVersion + ")" + " by " + MainPluginAuthorName;	
}

ExperimentManagerPlugin::~ExperimentManagerPlugin()
{
	delete ExperimentManagerDiagObject;
	delete ExperimentManagerObject;
}

QString ExperimentManagerPlugin::GetPluginInformation()
{
	return MainPluginTitle;
}

bool ExperimentManagerPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue ExperimentManagerProto = engine.newQObject(ExperimentManagerObject);
	engine.setDefaultPrototype(qMetaTypeId<ExperimentManager*>(), ExperimentManagerProto);
	QScriptValue ExperimentManagerCtor = engine.newFunction(ExperimentManager::ctor__experimentManager, ExperimentManagerProto);
	engine.globalObject().setProperty(EXPERIMENTMANAGER_NAME, ExperimentManagerCtor);

	ImageProcessor ImageProcessorObject;//or use new(), but make sure to use delete afterwards!
	QScriptValue ImageProcessorProto = engine.newQObject(&ImageProcessorObject);
	engine.setDefaultPrototype(qMetaTypeId<ImageProcessor*>(), ImageProcessorProto);
	QScriptValue ImageProcessorCtor = engine.newFunction(ImageProcessor::ctor__imageProcessor, ImageProcessorProto);
	engine.globalObject().setProperty(IMAGEPROCESSOR_NAME, ImageProcessorCtor);

	return true;
}

QString ExperimentManagerPlugin::Test(const QString &message)
{
    return "I'm oke(" + message + ")" + " --> " + MainPluginTitle;
}

bool ExperimentManagerPlugin::ShowGUI()
{
	int returnVal;
	ExperimentManagerDiagObject->setWindowTitle(MainPluginTitle);
	returnVal = ExperimentManagerDiagObject->exec();

	switch (returnVal) {
	case QMessageBox::Save:
	   // Save was clicked
	   break;
	case QMessageBox::Discard:
		// Don't Save was clicked
	    break;
	case QMessageBox::Cancel:
	    // Cancel was clicked
	    break;
	default:
	    // should never be reached
	    break;
	}		
	return true;
}

Q_EXPORT_PLUGIN2(experimentmanagerplugin, ExperimentManagerPlugin);
