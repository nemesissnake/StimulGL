//_extensionname_
//Copyright (C) 2012  _authorname_
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


#include "_extensionname_.h"

QScriptValue _extensionname_::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new _extensionname_(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

_extensionname_::_extensionname_(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
}

_extensionname_::~_extensionname_()
{

}

bool _extensionname_::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

void _extensionname_::setExampleProperty( short sExampleProperty )
{
	m_ExampleProperty = sExampleProperty;
	emit ExampleSignalTriggered(m_ExampleProperty);//Signal that the value has been changed
}

short _extensionname_::getExampleProperty() const
{
	return m_ExampleProperty;
}

//bool _extensionname_::ExtendScriptContext(QScriptEngine &engine)
//{
//	qScriptRegisterMetaType<SomeCustom_Type_Enum_Struct>(&engine, qScriptValueFromQObject, qScriptValueToQObject);//Also make sure to add Q_DECLARE_METATYPE(SomeCustom_Type_Enum_Struct); to the header global scope
//	QScriptValue ctor = engine.newFunction(qScriptConstructor<SomeCustom_Type_Enum_Struct>);
//	engine.globalObject().setProperty(SOMECUSTOM_TYPE_ENUM_STRUCT_SHORT_NAME, ctor);
//	return true;
//}