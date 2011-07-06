/****************************************************************************
**
**
****************************************************************************/

#ifndef PARALLELPORTPLUGIN_H
#define PARALLELPORTPLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "pport_dialog.h"
#include "ParallelPort.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ParallelPortPlugin : public QObject, DeviceInterface
{
    Q_OBJECT
    Q_INTERFACES(DeviceInterface)

public:
	ParallelPortPlugin(short BaseAddress = 888, QObject *parent = 0);
	~ParallelPortPlugin();

	bool ConfigureScriptEngine(QScriptEngine &engine);
	QString Test(const QString &message);
	QString GetPluginInformation(void);

private:
	QString MainPluginName;
	QString MainPluginOrganizationName;
	QString MainPluginAuthorName;
	QString MainPluginVersion;
	QString MainPluginTitle;

	short nBaseAddress;
	ParallelPort *PPortObject; 
	PPort_Dialog *PPortDiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//PARALLELPORTPLUGIN_H
