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

}

_extensionname_::~_extensionname_()
{

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
