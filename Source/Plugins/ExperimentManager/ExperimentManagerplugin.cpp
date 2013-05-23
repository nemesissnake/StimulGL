//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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
#include "visualexperimenteditor/VisualExperimentEditor.h"
#include "ImageProcessor.h"
#include "TriggerTimer.h"
#include "ExperimentStructures.h"
#include "defines.h"

Q_DECLARE_METATYPE(ExperimentManager*)

ExperimentManagerPlugin::ExperimentManagerPlugin(QObject *parent)
{
	Q_UNUSED(parent);
	ExperimentManagerDiagObject = NULL;
	ExperimentManagerObject = NULL;
	TriggerTimerObject = NULL;
	RandomGeneratorObject = NULL;
	ImageProcessorObject = NULL;
	PrtFormatManagerObject = NULL;
	RetinotopyMapperObject = NULL;
	cExperimentStructureObject = NULL;
	cBlockStructureObject = NULL;
	cLoopStructureObject = NULL;
	Qml2ViewerObject = NULL;
	ExperimentManagerDiagObject = new ExperimentManager_Dialog();
	ExperimentManagerObject = new ExperimentManager(ExperimentManagerDiagObject,NULL);
	strPluginInformation = PLUGIN_INFORMATION;
	Q_INIT_RESOURCE(ExperimentManager);
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
	if(RandomGeneratorObject)
	{
		delete RandomGeneratorObject;
		RandomGeneratorObject = NULL;
	}	
	if(cExperimentStructureObject)
	{
		delete cExperimentStructureObject;
		cExperimentStructureObject = NULL;
	}
	if(cBlockStructureObject)
	{
		delete cBlockStructureObject;
		cBlockStructureObject = NULL;
	}
	if(cLoopStructureObject)
	{
		delete cLoopStructureObject;
		cLoopStructureObject = NULL;
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
	if(RetinotopyMapperObject)
	{
		delete RetinotopyMapperObject;
		RetinotopyMapperObject = NULL;
	}
	if(Qml2ViewerObject)
	{
		delete Qml2ViewerObject;
		Qml2ViewerObject = NULL;
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

	if(RandomGeneratorObject == NULL)
		RandomGeneratorObject = new RandomGenerator();
	QScriptValue RandomGeneratorProto = engine.newQObject(RandomGeneratorObject);
	engine.setDefaultPrototype(qMetaTypeId<RandomGenerator*>(), RandomGeneratorProto);
	QScriptValue RandomGeneratorCtor = engine.newFunction(RandomGenerator::ctor__randomGenerator, RandomGeneratorProto);
	engine.globalObject().setProperty(RANDOMGENERATOR_NAME, RandomGeneratorCtor);

	if(cExperimentStructureObject == NULL)
		cExperimentStructureObject = new cExperimentStructure();
	QScriptValue cExperimentStructureProto = engine.newQObject(cExperimentStructureObject);
	engine.setDefaultPrototype(qMetaTypeId<cExperimentStructure*>(), cExperimentStructureProto);
	QScriptValue cExperimentStructureCtor = engine.newFunction(cExperimentStructure::ctor__cExperimentStructure, cExperimentStructureProto);
	engine.globalObject().setProperty(CEXPERIMENTSTRUCTURE_NAME, cExperimentStructureCtor);
	qScriptRegisterMetaType(&engine, cExperimentStructure::experimentStructureToScriptValue, cExperimentStructure::experimentStructureFromScriptValue);

	if(cBlockStructureObject == NULL)
		cBlockStructureObject = new cBlockStructure();
	QScriptValue cBlockStructureProto = engine.newQObject(cBlockStructureObject);
	engine.setDefaultPrototype(qMetaTypeId<cBlockStructure*>(), cBlockStructureProto);
	QScriptValue cBlockStructureCtor = engine.newFunction(cBlockStructure::ctor__cBlockStructure, cBlockStructureProto);
	engine.globalObject().setProperty(CBLOCKSTRUCTURE_NAME, cBlockStructureCtor);
	qScriptRegisterMetaType(&engine, cBlockStructure::blockStructureToScriptValue, cBlockStructure::blockStructureFromScriptValue);

	if(cLoopStructureObject == NULL)
		cLoopStructureObject = new cLoopStructure();
	QScriptValue cLoopStructureProto = engine.newQObject(cLoopStructureObject);
	engine.setDefaultPrototype(qMetaTypeId<cLoopStructure*>(), cLoopStructureProto);
	QScriptValue cLoopStructureCtor = engine.newFunction(cLoopStructure::ctor__cLoopStructure, cLoopStructureProto);
	engine.globalObject().setProperty(CLOOPSTRUCTURE_NAME, cLoopStructureCtor);
	qScriptRegisterMetaType(&engine, cLoopStructure::loopStructureToScriptValue, cLoopStructure::loopStructureFromScriptValue);

	qScriptRegisterMetaType(&engine, cExperimentStructure::experimentStructureStateToScriptValue, cExperimentStructure::experimentStructureStateFromScriptValue);
	QScriptValue ctorExperimentStructureState = engine.newFunction(cExperimentStructure::createExperimentStructureStateFromScript);
	engine.globalObject().setProperty(CEXPERIMENTSTRUCTURESTATE_NAME, ctorExperimentStructureState);

	if(RetinotopyMapperObject == NULL)
		RetinotopyMapperObject = new RetinotopyMapper();
	QScriptValue RetinotopyMapperProto = engine.newQObject(RetinotopyMapperObject);
	engine.setDefaultPrototype(qMetaTypeId<RetinotopyMapper*>(), RetinotopyMapperProto);
	QScriptValue RetinotopyMapperCtor = engine.newFunction(RetinotopyMapper::ctor__RetinotopyMapper, RetinotopyMapperProto);
	engine.globalObject().setProperty(RETINOTOPYMAPPER_NAME, RetinotopyMapperCtor);

	if(Qml2ViewerObject == NULL)
		Qml2ViewerObject = new QML2Viewer();
	QScriptValue Qml2ViewerProto = engine.newQObject(Qml2ViewerObject);
	engine.setDefaultPrototype(qMetaTypeId<QML2Viewer*>(), Qml2ViewerProto);
	QScriptValue Qml2ViewerCtor = engine.newFunction(QML2Viewer::ctor_QML2Viewer, Qml2ViewerProto);
	engine.globalObject().setProperty(QML2VIEWER_NAME, Qml2ViewerCtor);

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
	case 4:
		if(RandomGeneratorObject == NULL)
			RandomGeneratorObject = new RandomGenerator();
		return (QObject *)RandomGeneratorObject->metaObject();
	case 5:
		if(cExperimentStructureObject == NULL)
			cExperimentStructureObject = new cExperimentStructure();
		return (QObject *)cExperimentStructureObject->metaObject();
	case 6:
		if(cBlockStructureObject == NULL)
			cBlockStructureObject = new cBlockStructure();
		return (QObject *)cBlockStructureObject->metaObject();
	case 7:
		if(cLoopStructureObject == NULL)
			cLoopStructureObject = new cLoopStructure();
		return (QObject *)cLoopStructureObject->metaObject();
	case 8:
		if(RetinotopyMapperObject == NULL)
			RetinotopyMapperObject = new RetinotopyMapper();
		return (QObject *)RetinotopyMapperObject->metaObject();
	case 9:
		if(Qml2ViewerObject == NULL)
			Qml2ViewerObject = new QML2Viewer();
		return (QObject *)Qml2ViewerObject->metaObject();
	default:
		return NULL;
	}
}

bool ExperimentManagerPlugin::ExperimentManagerEXMLDocumentHandler(const QString &docContent, const QString &strHomePath)
{
	DocContentInfoStructure docStruct;
	docStruct.strDocContent = docContent;
	docStruct.bIsFile = false;
	docStruct.strDocHomeDir = strHomePath;
	docStruct.strDocExtension = PLUGIN_EXMLDOC_EXTENSION;
	return ExecuteContent(docStruct);
}

bool ExperimentManagerPlugin::ExperimentManagerQMLDocumentHandler(const QString &docContent, const QString &strHomePath)
{
	DocContentInfoStructure docStruct;
	docStruct.strDocContent = docContent;
	docStruct.bIsFile = false;
	docStruct.strDocHomeDir = strHomePath;
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

int ExperimentManagerPlugin::GetAdditionalFileTypeStyle(QString strExtension) 
{
	if(strExtension.toLower() == "exml")
	{
		return GlobalApplicationInformation::DOCTYPE_STYLE_XML;
	} 
	else if(strExtension.toLower() == "qml")
	{
		return GlobalApplicationInformation::DOCTYPE_STYLE_QML;
	}	
	return GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED;
}

QString ExperimentManagerPlugin::GetAdditionalFileTypeApiName(QString strExtension) 
{
	if(strExtension.toLower() == "exml")
	{
		return "exml.api";
	} 
	else if(strExtension.toLower() == "qml")
	{
		return "";//not jet implemented
	}
	return "";
}

QWidget *ExperimentManagerPlugin::GetAdditionalFileTypeEditor(QString strExtension)
{
	return NULL;
	if(strExtension.toLower() == "exml")
	{
		if(ExperimentManagerObject == NULL)
			ExperimentManagerObject = new ExperimentManager(this);
		return ExperimentManagerObject->getVisualExperimentEditor();
	} 
	else if(strExtension.toLower() == "qml")
	{
		return NULL;//not jet implemented
	}
	return NULL;
}

bool ExperimentManagerPlugin::LoadAdditionalFile(QString strFilePath)
{
	if(ExperimentManagerObject == NULL)
		ExperimentManagerObject = new ExperimentManager(this);
	if(ExperimentManagerObject->loadExperiment(strFilePath,false,true))
	{
		return ExperimentManagerObject->showVisualExperimentEditor();
	}
	else
	{
		qDebug() << __FUNCTION__ << "::Error: Could not load the document (" << strFilePath << ")!";
	}
	return false;
}

bool ExperimentManagerPlugin::SaveAdditionalFile(QString strFilePath)
{
	return true;
}
