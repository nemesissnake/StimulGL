#include "ExperimentManagerplugin.h"
#include "ExperimentManager.h"
#include "ImageProcessor.h"

Q_DECLARE_METATYPE(ExperimentManager*)
//Q_DECLARE_METATYPE(ImageProcessor*)

ExperimentManagerPlugin::ExperimentManagerPlugin(QObject *parent)
{
	ExperimentManagerObject = new ExperimentManager(parent);
	ExperimentManagerDiagObject = new ExperimentManager_Dialog();	
	MainPluginName = "ExperimentManager Plugin";
	MainPluginAuthorName = "Sven Gijsen";
	MainPluginOrganizationName = "MBIC";
	MainPluginVersion = "1.0.0";
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
