//This file inherits the Interface description, please don't change

#ifndef _extensionname_PLUGIN_H
#define _extensionname_PLUGIN_H

#include <QObject>
#include <QtGui>
#include <QString>
#include <Qlibrary>

#include "plugininterface.h"
#include "_extensionname__dialog.h"
#include "_extensionname_.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class _extensionname_Plugin : public QObject, ExtensionInterface
{
    Q_OBJECT
    Q_INTERFACES(ExtensionInterface)

public:
	_extensionname_Plugin(QObject *parent = 0);
	~_extensionname_Plugin();

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
	_extensionname_ *_extensionname_Object; 
	_extensionname__Dialog *_extensionname_DiagObject;

public slots:
    bool ShowGUI();

signals:
	void DoSignal();
};

#endif//_extensionname_PLUGIN_H
