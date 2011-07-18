#include "USBHIDDeviceplugin.h"
#include "USBHIDDevice.h"

Q_DECLARE_METATYPE(USBHIDDevice*)

USBHIDDevicePlugin::USBHIDDevicePlugin(QObject *parent)
{
	USBHIDDeviceObject = new USBHIDDevice(0x181b, 0x4002, parent);
	USBHIDDeviceDiagObject = new USBHIDDevice_Dialog();	
	MainPluginName = "USBHIDDevice Plugin";
	MainPluginAuthorName = "Sven Gijsen";
	MainPluginOrganizationName = "MBIC";
	MainPluginVersion = "1.0.0";
	MainPluginTitle = MainPluginName + "(v" + MainPluginVersion + ")" + " by " + MainPluginAuthorName;	
}

USBHIDDevicePlugin::~USBHIDDevicePlugin()
{
	delete USBHIDDeviceDiagObject;
	delete USBHIDDeviceObject;
}

QString USBHIDDevicePlugin::GetPluginInformation()
{
	return MainPluginTitle;
}

bool USBHIDDevicePlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	QScriptValue USBHIDDeviceProto = engine.newQObject(USBHIDDeviceObject);
	engine.setDefaultPrototype(qMetaTypeId<USBHIDDevice*>(), USBHIDDeviceProto);
	QScriptValue USBHIDDeviceCtor = engine.newFunction(USBHIDDevice::ctor__extensionname, USBHIDDeviceProto);
	engine.globalObject().setProperty("USBHIDDevice", USBHIDDeviceCtor);
	return true;
}

QString USBHIDDevicePlugin::Test(const QString &message)
{
    return "I'm oke(" + message + ")" + " --> " + MainPluginTitle;
}

bool USBHIDDevicePlugin::ShowGUI()
{
	int returnVal;
	USBHIDDeviceDiagObject->setWindowTitle(MainPluginTitle);
	returnVal = USBHIDDeviceDiagObject->exec();

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

Q_EXPORT_PLUGIN2(USBHIDDeviceplugin, USBHIDDevicePlugin);
