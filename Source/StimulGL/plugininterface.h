//USBHIDDeviceplugin
//Copyright (C) 2012  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#ifndef PLUGININTERFACE_H
#define PLUGININTERFACE_H

#include <QString>
#include <QtScript>
#include <QMessageBox>
#include "maindefines.h"

//#define EXTENDSCRIPTOBJECT_FUNCTION_SHORT_SIGNATURE "ExtendScriptContext"
//#define EXTENDSCRIPTOBJECT_FUNCTION_FULL_SIGNATURE EXTENDSCRIPTOBJECT_FUNCTION_SHORT_SIGNATURE "(QScriptEngine&)"

	class PluginInterface
	{
	public:
		virtual ~PluginInterface() {}
		virtual bool ShowGUI() = 0;
		virtual QString GetPluginInformation(void) {return strPluginInformation;}
		virtual QString Test(const QString &message) {return "I'm oke(" + message + ")" + " --> " + strPluginInformation;}	
		virtual bool ConfigureScriptEngine(QScriptEngine &eng) = 0;
		virtual QString GetMinimalMainProgramVersion() {return MAIN_PROGRAM_FILE_VERSION_STRING;};
		//virtual bool FinalizeScriptObjectScriptContext(QScriptEngine &engine, QObject* object)
		//{//This function calls the ScriptObject member functions (if defined) to further setup the scripting environment
		//	if(object == NULL)
		//		return false;
		//	bool bResult = false;
		//	const QMetaObject* metaObject = object->metaObject();

		//	if (metaObject)
		//	{
		//		if (!(metaObject->indexOfMethod(QString(EXTENDSCRIPTOBJECT_FUNCTION_FULL_SIGNATURE).toLatin1()) == -1))//Is the slot present?
		//		{
		//			bool bRetVal;
		//			//Invoke the slot
		//			if(!metaObject->invokeMethod(object, QString(EXTENDSCRIPTOBJECT_FUNCTION_SHORT_SIGNATURE).toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QScriptEngine&, engine)))//, Q_ARG(QString, "sqrt"), Q_ARG(int, 42), Q_ARG(double, 9.7));
		//			{
		//				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << QString(EXTENDSCRIPTOBJECT_FUNCTION_FULL_SIGNATURE) << ")!";		
		//				return false;
		//			}
		//			if (bRetVal == false)
		//			{

		//			}
		//		}
		//		////Query the properties(only Q_PROPERTY)
		//		//QStringList properties;
		//		//QString nolist;
		//		//for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
		//		//{
		//		//	properties << QString::fromLatin1(metaObject->property(i).name());
		//		//	nolist = QString::fromLatin1(metaObject->property(i).name());
		//		//}
		//		////Query the methods(only public slots!)
		//		//QStringList methods;
		//		//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
		//		//	methods << QString::fromLatin1(metaObject->method(i).signature());
		//		////Query the enumerators(only ??)
		//		//QStringList enums;
		//		//for(int i = metaObject->enumeratorOffset(); i < metaObject->enumeratorCount(); ++i)
		//		//	enums << QString::fromLatin1(metaObject->enumerator(i).name());
		//	}
		//	else
		//	{
		//		return false;
		//	}

		//	bResult = true;
		//	return bResult;
		//}
	public slots:
		virtual bool IsCompatible() 
		{
			QString strMainProgramMinimalVersion = GetMinimalMainProgramVersion();
			//return (MainAppInfo::isCompatibleVersion(GetMinimalMainProgramVersion(),QString(MAIN_PROGRAM_FILE_VERSION_STRING)));			
			QStringList lstMainProgramCurrentVersion = QString(MAIN_PROGRAM_FILE_VERSION_STRING).split(".");
			QStringList lstMainProgramMinimalVersion = strMainProgramMinimalVersion.split(".");
			if ((lstMainProgramCurrentVersion.count() == 4) && (lstMainProgramMinimalVersion.count() == 4))
			{
				for (int i = 0;i<4;i++)
				{
					if(lstMainProgramCurrentVersion.at(i).toInt() > lstMainProgramMinimalVersion.at(i).toInt())
					{
						return true;//Later version
					}
					else if(lstMainProgramCurrentVersion.at(i).toInt() < lstMainProgramMinimalVersion.at(i).toInt())
					{
						return false;//Earlier version
					}
				}
				return true;//same version
			}
			return false;//wrong arguments
		};
		virtual QString GetAdditionalFileExtensions() {return QString("");};
	protected:
		QString strPluginInformation;
	};


	//////////////////////////////////////////////////////////////////////////


	class DeviceInterface: virtual public PluginInterface
	{
	public:
		virtual ~DeviceInterface() {}
	};


	//////////////////////////////////////////////////////////////////////////


	class ExtensionInterface: virtual public PluginInterface
	{
	public:
		virtual ~ExtensionInterface() {}
	};

	QT_BEGIN_NAMESPACE
		Q_DECLARE_INTERFACE(DeviceInterface,    MAIN_PROGRAM_DEVICE_INTERFACE);
		Q_DECLARE_INTERFACE(ExtensionInterface, MAIN_PROGRAM_EXTENSION_INTERFACE);
	QT_END_NAMESPACE

#endif // PLUGININTERFACE_H
