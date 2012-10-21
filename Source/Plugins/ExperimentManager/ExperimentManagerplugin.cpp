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
	ExperimentManagerDiagObject = NULL;
	ExperimentManagerObject = NULL;
	TriggerTimerObject = NULL;
	ImageProcessorObject = NULL;
	PrtFormatManagerObject = NULL;
	QmlWidgetObject = NULL;
	ExperimentManagerDiagObject = new ExperimentManager_Dialog();
	ExperimentManagerObject = new ExperimentManager(ExperimentManagerDiagObject,NULL);
	strPluginInformation = PLUGIN_INFORMATION;
}

ExperimentManagerPlugin::~ExperimentManagerPlugin()
{
	if(ExperimentManagerObject)
	{
		delete ExperimentManagerObject;
		ExperimentManagerObject = NULL;
	}
	if(ExperimentManagerDiagObject)
	{
		delete ExperimentManagerDiagObject;
		ExperimentManagerDiagObject = NULL;
	}
	if(TriggerTimerObject)
	{
		delete TriggerTimerObject;
		TriggerTimerObject = NULL;
	}
	if(ImageProcessorObject)
	{
		delete ImageProcessorObject;
		ImageProcessorObject = NULL;
	}
	if(PrtFormatManagerObject)
	{
		delete PrtFormatManagerObject;
		PrtFormatManagerObject = NULL;
	}
	if(QmlWidgetObject)
	{
		delete QmlWidgetObject;
		QmlWidgetObject = NULL;
	}
}

bool ExperimentManagerPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue ExperimentManagerProto = engine.newQObject(ExperimentManagerObject);
	engine.setDefaultPrototype(qMetaTypeId<ExperimentManager*>(), ExperimentManagerProto);
	QScriptValue ExperimentManagerCtor = engine.newFunction(ExperimentManager::ctor__experimentManager, ExperimentManagerProto);
	engine.globalObject().setProperty(EXPERIMENTMANAGER_NAME, ExperimentManagerCtor);

	if(TriggerTimerObject == NULL)
		TriggerTimerObject = new TriggerTimer();
	QScriptValue TriggerTimerProto = engine.newQObject(TriggerTimerObject);
	engine.setDefaultPrototype(qMetaTypeId<TriggerTimer*>(), TriggerTimerProto);
	QScriptValue TriggerTimerCtor = engine.newFunction(TriggerTimer::ctor__triggerTimer, TriggerTimerProto);
	engine.globalObject().setProperty(TRIGGERTIMER_NAME, TriggerTimerCtor);

	if(ImageProcessorObject == NULL)
		ImageProcessorObject = new ImageProcessor();
	QScriptValue ImageProcessorProto = engine.newQObject(ImageProcessorObject);
	engine.setDefaultPrototype(qMetaTypeId<ImageProcessor*>(), ImageProcessorProto);
	QScriptValue ImageProcessorCtor = engine.newFunction(ImageProcessor::ctor__imageProcessor, ImageProcessorProto);
	engine.globalObject().setProperty(IMAGEPROCESSOR_NAME, ImageProcessorCtor);

	if(PrtFormatManagerObject == NULL)
		PrtFormatManagerObject = new PrtFormatManager();
	QScriptValue PrtFormatManagerProto = engine.newQObject(PrtFormatManagerObject);
	engine.setDefaultPrototype(qMetaTypeId<PrtFormatManager*>(), PrtFormatManagerProto);
	QScriptValue PrtFormatManagerCtor = engine.newFunction(PrtFormatManager::ctor__PrtFormatManager, PrtFormatManagerProto);
	engine.globalObject().setProperty(PRTFORMATMANAGER_NAME, PrtFormatManagerCtor);

	//if(QmlWidgetObject == NULL)
	//	QmlWidgetObject = new qmlWidget();
	//QScriptValue QmlWidgetProto = engine.newQObject(QmlWidgetObject);
	//engine.setDefaultPrototype(qMetaTypeId<qmlWidget*>(), QmlWidgetProto);
	//QScriptValue QmlWidgetCtor = engine.newFunction(qmlWidget::ctor_QmlWidget, QmlWidgetProto);
	//engine.globalObject().setProperty(QMLWIDGET_NAME, QmlWidgetCtor);

	//if(retinoWidgetObject == NULL)
	//	retinoWidgetObject = new RetinoMap_glwidget();
	//RetinoMap_glwidget retinoWidgetObject;//or use new(), but make sure to use delete afterwards!
	//QScriptValue RetinoWidgetProto = engine.newQObject(&retinoWidgetObject);
	//engine.setDefaultPrototype(qMetaTypeId<RetinoMap_glwidget*>(), RetinoWidgetProto);
	//QScriptValue RetinoWidgetCtor = engine.newFunction(RetinoMap_glwidget::ctor_RetinoWidget, RetinoWidgetProto);
	//engine.globalObject().setProperty(RETINOMAP_WIDGET_NAME, RetinoWidgetCtor);

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

QObject *ExperimentManagerPlugin::GetScriptMetaObject(int nIndex)
{
	if(nIndex<0)
		return NULL;

	switch (nIndex)
	{
	case 0:
		return (QObject *)ExperimentManagerObject->metaObject();
	case 1:
		if(TriggerTimerObject == NULL)
			TriggerTimerObject = new TriggerTimer();
		return (QObject *)TriggerTimerObject->metaObject();
	case 2:
		if(ImageProcessorObject == NULL)
			ImageProcessorObject = new ImageProcessor();
		return (QObject *)ImageProcessorObject->metaObject();
	case 3:
		if(PrtFormatManagerObject == NULL)
			PrtFormatManagerObject = new PrtFormatManager();
		return (QObject *)PrtFormatManagerObject->metaObject();
	default:
		return NULL;
	}
}

int ExperimentManagerPlugin::GetScriptMetaObjectCount() 
{
	return 4;
}

bool ExperimentManagerPlugin::ExperimentManagerEXMLDocumentHandler(const QString &docContent)
{
	DocContentInfoStructure docStruct;
	docStruct.strDocContent = docContent;
	docStruct.bIsFile = false;
	docStruct.strDocExtension = PLUGIN_EXMLDOC_EXTENSION;
	return ExecuteContent(docStruct);
}

bool ExperimentManagerPlugin::ExperimentManagerQMLDocumentHandler(const QString &docContent)
{
	DocContentInfoStructure docStruct;
	docStruct.strDocContent = docContent;
	docStruct.bIsFile = false;
	docStruct.strDocExtension = PLUGIN_QMLDOC_EXTENSION;
	return ExecuteContent(docStruct);
}

bool ExperimentManagerPlugin::ExecuteContent(const DocContentInfoStructure &docStruct)
{
	if(ExperimentManagerDiagObject)
	{
		ExperimentManagerDiagObject->setContentToExecute(docStruct);
		return ExperimentManagerDiagObject->executeDocument();
	}
	return false;
}

Q_EXPORT_PLUGIN2(experimentmanagerplugin, ExperimentManagerPlugin);
