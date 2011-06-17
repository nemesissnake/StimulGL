#include "Retinotopic_Mappingplugin.h"
#include "Retinotopic_Mapping.h"

Q_DECLARE_METATYPE(Retinotopic_Mapping*)

Retinotopic_MappingPlugin::Retinotopic_MappingPlugin(QObject *parent)
{
	Retinotopic_MappingDiagObject = new Retinotopic_Mapping_Dialog(NULL);
	//Retinotopic_MappingObject = new Retinotopic_Mapping(parent);//is created by the dialog itself, see below line of code
	Retinotopic_MappingObject = Retinotopic_MappingDiagObject->getRetinotopic_MappingObject();
	MainPluginName = "Retinotopic_Mapping Plugin";
	MainPluginAuthorName = "Rainer Goebel";
	MainPluginOrganizationName = "MBIC";
	MainPluginVersion = "1.0.0";
	MainPluginTitle = MainPluginName + "(v" + MainPluginVersion + ")" + " by " + MainPluginAuthorName;	
}

Retinotopic_MappingPlugin::~Retinotopic_MappingPlugin()
{
	if (Retinotopic_MappingObject)
	{
		delete Retinotopic_MappingObject;
		Retinotopic_MappingObject = NULL;
	}
	if (Retinotopic_MappingDiagObject)
	{
		delete Retinotopic_MappingDiagObject;
		Retinotopic_MappingDiagObject = NULL;
	}
}

QString Retinotopic_MappingPlugin::GetPluginInformation()
{
	return MainPluginTitle;
}

bool Retinotopic_MappingPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue Retinotopic_MappingProto = engine.newQObject(Retinotopic_MappingObject);
	engine.setDefaultPrototype(qMetaTypeId<Retinotopic_Mapping*>(), Retinotopic_MappingProto);
	QScriptValue Retinotopic_MappingCtor = engine.newFunction(Retinotopic_Mapping::ctor__extensionname, Retinotopic_MappingProto);
	engine.globalObject().setProperty("Retinotopic_Mapping", Retinotopic_MappingCtor);
	return true;
}

QString Retinotopic_MappingPlugin::Test(const QString &message)
{
    return "I'm oke(" + message + ")" + " --> " + MainPluginTitle;
}

bool Retinotopic_MappingPlugin::ShowGUI()
{
	int returnVal;
	Retinotopic_MappingDiagObject->setWindowTitle(MainPluginTitle);
	returnVal = Retinotopic_MappingDiagObject->exec();

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

Q_EXPORT_PLUGIN2(Retinotopic_Mappingplugin, Retinotopic_MappingPlugin);
