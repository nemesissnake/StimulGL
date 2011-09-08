//This file defines the Interface, do not change!

#ifndef ExtensionInterface_H
#define ExtensionInterface_H

#include <QString>
#include <QtScript>
#include <QMessageBox>

	class PluginInterface
	{
	public:
		virtual ~PluginInterface() {}
		virtual QString Test(const QString &message) = 0;
		virtual QString GetPluginInformation(void) = 0;
		virtual bool ShowGUI() = 0;
		virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
	};


	//////////////////////////////////////////////////////////////////////////


	class DeviceInterface: virtual public PluginInterface
	{
		//signals:
		//	virtual void DoSignal();
	public:
		virtual ~DeviceInterface() {}
		//virtual QString Test(const QString &message) = 0;
		//virtual QString GetPluginInformation(void) = 0;
		//virtual bool ShowGUI() = 0;
		//virtual QScriptValue ExecuteScript(const QString &scriptcode) = 0;
		//virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
		//public slots:
		//virtual bool GetGUI() = 0;
	};


	//////////////////////////////////////////////////////////////////////////


	class ExtensionInterface: virtual public PluginInterface
	{
		//signals:
		//	virtual void DoSignal();
	public:
		virtual ~ExtensionInterface() {}
		//virtual QString Test(const QString &message) = 0;
		//virtual QString GetPluginInformation(void) = 0;
		//virtual bool ShowGUI() = 0;
		//virtual QScriptValue ExecuteScript(const QString &scriptcode) = 0;
		//virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
		//public slots:
		//virtual bool GetGUI() = 0;
	};

	QT_BEGIN_NAMESPACE
		Q_DECLARE_INTERFACE(DeviceInterface, "StimulGL.Plugins.DeviceInterface/1.0");
		Q_DECLARE_INTERFACE(ExtensionInterface, "StimulGL.Plugins.ExtensionInterface/1.0");
	QT_END_NAMESPACE

#endif // ExtensionInterface_H
