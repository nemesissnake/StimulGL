//This file inherits the Interface description, please don't change

#ifndef ExperimentManagerPLUGIN_H
#define ExperimentManagerPLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "ExperimentManager_dialog.h"
#include "ExperimentManager.h"
#include "ImageProcessor.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class ExperimentManagerPlugin : public QObject, ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)

public:
	ExperimentManagerPlugin(QObject *parent = 0);
	~ExperimentManagerPlugin();

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
	ExperimentManager *ExperimentManagerObject; 
	ExperimentManager_Dialog *ExperimentManagerDiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//ExperimentManagerPLUGIN_H
