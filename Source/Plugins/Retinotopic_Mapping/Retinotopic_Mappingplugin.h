//This file inherits the Interface description, please don't change

#ifndef Retinotopic_MappingPLUGIN_H
#define Retinotopic_MappingPLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "Retinotopic_Mapping_dialog.h"
#include "Retinotopic_Mapping.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Retinotopic_MappingPlugin : public QObject, ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)

public:
	Retinotopic_MappingPlugin(QObject *parent = 0);
	~Retinotopic_MappingPlugin();

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
	Retinotopic_Mapping *Retinotopic_MappingObject; 
	Retinotopic_Mapping_Dialog *Retinotopic_MappingDiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//Retinotopic_MappingPLUGIN_H
