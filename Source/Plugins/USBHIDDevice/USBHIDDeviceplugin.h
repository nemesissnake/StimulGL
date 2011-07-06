//This file inherits the Interface description, please don't change

#ifndef USBHIDDevicePLUGIN_H
#define USBHIDDevicePLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "USBHIDDevice_dialog.h"
#include "USBHIDDevice.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class USBHIDDevicePlugin : public QObject, DeviceInterface
{
    Q_OBJECT
    Q_INTERFACES(DeviceInterface)

public:
	USBHIDDevicePlugin(QObject *parent = 0);
	~USBHIDDevicePlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString Test(const QString &message);
	QString GetPluginInformation(void);

private:
	QString MainPluginName;
	QString MainPluginAuthorName;
	QString MainPluginOrganizationName;
	QString MainPluginVersion;
	QString MainPluginTitle;

	short nExampleProperty;
	USBHIDDevice *USBHIDDeviceObject; 
	USBHIDDevice_Dialog *USBHIDDeviceDiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//USBHIDDevicePLUGIN_H
