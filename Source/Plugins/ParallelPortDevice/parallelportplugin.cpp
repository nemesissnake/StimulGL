/****************************************************************************
**
**
****************************************************************************/

#include "parallelportplugin.h"

Q_DECLARE_METATYPE(ParallelPort*)

ParallelPortPlugin::ParallelPortPlugin(short BaseAddress, QObject *parent)
{
	nBaseAddress = BaseAddress;
	PPortObject = new ParallelPort(nBaseAddress,parent);
	PPortDiagObject = new PPort_Dialog();
	MainPluginName = "Parallel port plugin";
	MainPluginOrganizationName = "MBIC";
	MainPluginVersion = "1.0.0";
	MainPluginTitle = MainPluginName + "(v" + MainPluginVersion + ")";	
}

ParallelPortPlugin::~ParallelPortPlugin()
{
	delete PPortDiagObject;
	delete PPortObject;
}

QString ParallelPortPlugin::GetPluginInformation()
{
	return MainPluginTitle;
}

QScriptValue ctor_ParallelPort(QScriptContext* context, QScriptEngine* engine)
{
	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new ParallelPort(), QScriptEngine::ScriptOwnership);
} 

//QScriptValue getSetBaseAddress(QScriptContext *ctx, QScriptEngine *eng)
//{
//	QScriptValue obj = ctx->thisObject();
//	QScriptValue data = obj.data();
//	if (!data.isValid()) {
//		data = eng->newObject();
//		obj.setData(data);
//	}
//	QScriptValue result;
//	if (ctx->argumentCount() == 1)//setter
//	{
//		QString str = ctx->argument(0).toString();
//		short val = ctx->argument(0).toInteger();
//		//str.replace("Roberta", "Ken");
//		result = QScriptValue(eng, str);
//		//data.setProperty("BaseAddress", result);
//				
//		eng->globalObject.
//		ParallelPortPlugin::nBaseAddress = val;
//	} 
//	else//getter
//	{
//		result = data.property("BaseAddress");
//	}
//	return result;
//}

bool ParallelPortPlugin::ConfigureScriptEngine(QScriptEngine &engine)
{
	// example for letting the script do the work...
	//QScriptEngine eng;
	//testScriptObject *to = new testScriptObject(&eng);
	//QString scriptFile = "e:/Active Projects/ParallelPortDevicePlugin/Resources/testScriptObject.js";
	//to->runScript(scriptFile);

	// create and register the ParallelPort prototype and constructor in the engine
	//ParallelPort *ParallelPortObject = new ParallelPort();
	QScriptValue ParallelPortProto = engine.newQObject(PPortObject);
	engine.setDefaultPrototype(qMetaTypeId<ParallelPort*>(), ParallelPortProto);
	QScriptValue ParallelPortCtor = engine.newFunction(ctor_ParallelPort, ParallelPortProto);
	engine.globalObject().setProperty("ParallelPort", ParallelPortCtor);

	//QScriptEngine eng;
	//QScriptValue obj = engine.newObject();
	//ParallelPortProto.setProperty("BaseAddress", engine.newFunction(getSetBaseAddress), 
	//	QScriptValue::PropertyGetter|QScriptValue::PropertySetter);

	return true;
}

QString ParallelPortPlugin::Test(const QString &message)
{
    return "I'm oke(" + message + ")";
}

bool ParallelPortPlugin::ShowGUI()
{
	int returnVal;
	PPortDiagObject->setWindowTitle(MainPluginTitle);
	returnVal = PPortDiagObject->exec();

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

Q_EXPORT_PLUGIN2(parallelportplugin, ParallelPortPlugin);
