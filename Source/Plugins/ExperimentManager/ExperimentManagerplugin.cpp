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
#include "TriggerTimer.h"
#include "defines.h"

Q_DECLARE_METATYPE(ExperimentManager*)

ExperimentManagerPlugin::ExperimentManagerPlugin(QObject *parent)
{
	ExperimentManagerDiagObject = new ExperimentManager_Dialog();
	ExperimentManagerObject = new ExperimentManager(ExperimentManagerDiagObject,NULL);
	strPluginInformation = PLUGIN_INFORMATION;
}

ExperimentManagerPlugin::~ExperimentManagerPlugin()
{
	delete ExperimentManagerDiagObject;
	delete ExperimentManagerObject;
}

bool ExperimentManagerPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue ExperimentManagerProto = engine.newQObject(ExperimentManagerObject);
	engine.setDefaultPrototype(qMetaTypeId<ExperimentManager*>(), ExperimentManagerProto);
	QScriptValue ExperimentManagerCtor = engine.newFunction(ExperimentManager::ctor__experimentManager, ExperimentManagerProto);
	engine.globalObject().setProperty(EXPERIMENTMANAGER_NAME, ExperimentManagerCtor);

	TriggerTimer TriggerTimerObject;//or use new(), but make sure to use delete afterwards!
	QScriptValue TriggerTimerProto = engine.newQObject(&TriggerTimerObject);
	engine.setDefaultPrototype(qMetaTypeId<TriggerTimer*>(), TriggerTimerProto);
	QScriptValue TriggerTimerCtor = engine.newFunction(TriggerTimer::ctor__triggerTimer, TriggerTimerProto);
	engine.globalObject().setProperty(TRIGGERTIMER_NAME, TriggerTimerCtor);

	ImageProcessor ImageProcessorObject;//or use new(), but make sure to use delete afterwards!
	QScriptValue ImageProcessorProto = engine.newQObject(&ImageProcessorObject);
	engine.setDefaultPrototype(qMetaTypeId<ImageProcessor*>(), ImageProcessorProto);
	QScriptValue ImageProcessorCtor = engine.newFunction(ImageProcessor::ctor__imageProcessor, ImageProcessorProto);
	engine.globalObject().setProperty(IMAGEPROCESSOR_NAME, ImageProcessorCtor);

	PrtFormatManager PrtFormatManagerObject;//or use new(), but make sure to use delete afterwards!
	QScriptValue PrtFormatManagerProto = engine.newQObject(&PrtFormatManagerObject);
	engine.setDefaultPrototype(qMetaTypeId<PrtFormatManager*>(), PrtFormatManagerProto);
	QScriptValue PrtFormatManagerCtor = engine.newFunction(PrtFormatManager::ctor__PrtFormatManager, PrtFormatManagerProto);
	engine.globalObject().setProperty(PRTFORMATMANAGER_NAME, PrtFormatManagerCtor);

	qmlWidget QmlWidgetObject;//or use new(), but make sure to use delete afterwards!
	QScriptValue QmlWidgetProto = engine.newQObject(&QmlWidgetObject);
	engine.setDefaultPrototype(qMetaTypeId<qmlWidget*>(), QmlWidgetProto);
	QScriptValue QmlWidgetCtor = engine.newFunction(qmlWidget::ctor_QmlWidget, QmlWidgetProto);
	engine.globalObject().setProperty(QMLWIDGET_NAME, QmlWidgetCtor);

	return true;
}

bool ExperimentManagerPlugin::ShowGUI()
{
	int returnVal;
	ExperimentManagerDiagObject->setWindowTitle(strPluginInformation);
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
