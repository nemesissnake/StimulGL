#include "ExperimentManagerplugin.h"
#include "ExperimentManager.h"

Q_DECLARE_METATYPE(ExperimentManager*)

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
	QScriptValue ExperimentManagerCtor = engine.newFunction(ExperimentManager::ctor__extensionname, ExperimentManagerProto);
	engine.globalObject().setProperty("ExperimentManager", ExperimentManagerCtor);
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
