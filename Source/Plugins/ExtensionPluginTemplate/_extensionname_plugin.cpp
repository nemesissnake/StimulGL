#include "_extensionname_plugin.h"
#include "_extensionname_.h"

Q_DECLARE_METATYPE(_extensionname_*)

_extensionname_Plugin::_extensionname_Plugin(QObject *parent)
{
	_extensionname_Object = new _extensionname_(parent);
	_extensionname_DiagObject = new _extensionname__Dialog();	
	MainPluginName = "_extensionname_ Plugin";
	MainPluginAuthorName = "_authorname_";
	MainPluginOrganizationName = "_organizationname_";
	MainPluginVersion = "1.0.0";
	MainPluginTitle = MainPluginName + "(v" + MainPluginVersion + ")" + " by " + MainPluginAuthorName;	
}

_extensionname_Plugin::~_extensionname_Plugin()
{
	delete _extensionname_DiagObject;
	delete _extensionname_Object;
}

QString _extensionname_Plugin::GetPluginInformation()
{
	return MainPluginTitle;
}

bool _extensionname_Plugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue _extensionname_Proto = engine.newQObject(_extensionname_Object);
	engine.setDefaultPrototype(qMetaTypeId<_extensionname_*>(), _extensionname_Proto);
	QScriptValue _extensionname_Ctor = engine.newFunction(_extensionname_::ctor__extensionname, _extensionname_Proto);
	engine.globalObject().setProperty("_extensionname_", _extensionname_Ctor);
	return true;
}

QString _extensionname_Plugin::Test(const QString &message)
{
    return "I'm oke(" + message + ")" + " --> " + MainPluginTitle;
}

bool _extensionname_Plugin::ShowGUI()
{
	int returnVal;
	_extensionname_DiagObject->setWindowTitle(MainPluginTitle);
	returnVal = _extensionname_DiagObject->exec();

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

Q_EXPORT_PLUGIN2(_extensionname_plugin, _extensionname_Plugin);
