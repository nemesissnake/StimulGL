#ifndef CUSTOMSCRIPTOBJECTS_H
#define CUSTOMSCRIPTOBJECTS_H

/*
typedef int SomeCustom_Type_Enum_Struct;

Q_DECLARE_METATYPE(SomeCustom_Type_Enum_Struct);
#define SOMECUSTOM_TYPE_ENUM_STRUCT_NAME	typeid(SomeCustom_Type_Enum_Struct).name()
#define SOMECUSTOM_TYPE_ENUM_STRUCT_SHORT_NAME	"SomeCustom_Type_Enum_Struct"

template <typename Tp>
static QScriptValue qScriptValueFromQObject(QScriptEngine *engine, Tp const &qobject)
{
	QString sTmpTypeName = typeid(Tp).name();
	QScriptValue obj = engine->newObject();

	if(sTmpTypeName == SOMECUSTOM_TYPE_ENUM_STRUCT_NAME)
	{
		obj = qobject;
	}
	else
	{
		obj = qobject;
	}
	return obj;

	//return engine->newVariant((int)s);
	////return engine->newQObject((int)qobject);
	////return NULL;
	//QScriptValue obj = engine->newObject();
	////obj.setProperty("x", qobject.x);
	////obj.setProperty("y", qobject.y);
	//obj = qobject;//BAUD9600;
	//return obj;
}

template <typename Tp>
static void qScriptValueToQObject(const QScriptValue &value, Tp &qobject)
{   
	QString sTmpTypeName = typeid(Tp).name();

	if(sTmpTypeName == SOMECUSTOM_TYPE_ENUM_STRUCT_NAME)
	{
		qobject = (SomeCustom_Type_Enum_Struct)value.toInt32();//BAUD9600;
	}
	else
	{
		qobject = (Tp)value.toInt32();
	}
	//qobject = qobject_cast<Tp>(value.toQObject());
	//qobject.x = value.property("x").toInt32();
	//qobject.y = value.property("y").toInt32();

}

template <typename Tp>
QScriptValue qScriptConstructor(QScriptContext *, QScriptEngine *engine)
{
	QString sTmpTypeName = typeid(Tp).name();

	if(sTmpTypeName == SOMECUSTOM_TYPE_ENUM_STRUCT_NAME)
	{
		SomeCustom_Type_Enum_Struct tmpBaudRateType;
		tmpBaudRateType = -1;//Default Type Value
		return engine->toScriptValue(tmpBaudRateType);
	}
	else
	{
		return NULL;
	}
}
*/
#endif // CUSTOMSCRIPTOBJECTS_H