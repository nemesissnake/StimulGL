#ifndef CUSTOMSCRIPTOBJECTS_H
#define CUSTOMSCRIPTOBJECTS_H

//Q_DECLARE_METATYPE(SerialPortEnums::BaudRateType);
//Q_DECLARE_METATYPE(SerialPortEnums::DataBitsType);
//Q_DECLARE_METATYPE(SerialPortEnums::ParityType);
//Q_DECLARE_METATYPE(SerialPortEnums::StopBitsType);
//Q_DECLARE_METATYPE(SerialPortEnums::FlowType);
//Q_DECLARE_METATYPE(QextSerialPort::QueryMode);
//Q_DECLARE_METATYPE(QIODevice::OpenMode);

#define BAUDRATE_ENUM_TYPE_NAME			typeid(SerialPortEnums::BaudRateType).name()
#define BAUDRATE_ENUM_SHORT_TYPE_NAME	"BaudRateType"
#define DATABITS_ENUM_TYPE_NAME			typeid(SerialPortEnums::DataBitsType).name()
#define DATABITS_ENUM_SHORT_TYPE_NAME	"DataBitsType"
#define PARITY_ENUM_TYPE_NAME			typeid(SerialPortEnums::ParityType).name()
#define PARITY_ENUM_SHORT_TYPE_NAME		"ParityType"
#define STOPBITS_ENUM_TYPE_NAME			typeid(SerialPortEnums::StopBitsType).name()
#define STOPBITS_ENUM_SHORT_TYPE_NAME	"StopBitsType"
#define FLOW_ENUM_TYPE_NAME				typeid(SerialPortEnums::FlowType).name()
#define FLOW_ENUM_SHORT_TYPE_NAME		"FlowType"
#define QUERYMODE_ENUM_TYPE_NAME		typeid(QextSerialPort::QueryMode).name()
#define QUERYMODE_ENUM_SHORT_TYPE_NAME	"QueryMode"
#define OPENMODE_ENUM_TYPE_NAME			typeid(QIODevice::OpenMode).name()
#define OPENMODE_ENUM_SHORT_TYPE_NAME	"OpenMode"
#define OPENMODE_ENUM_META_TYPE_NAME	"QIODevice::OpenMode"

static bool CreateHashTableFromEnum(const QString &sTypeName, QHash<QString, int> &hTable)
{
	if((sTypeName == BAUDRATE_ENUM_SHORT_TYPE_NAME) || 
	   (sTypeName == DATABITS_ENUM_SHORT_TYPE_NAME) ||
	   (sTypeName == PARITY_ENUM_SHORT_TYPE_NAME) ||
	   (sTypeName == STOPBITS_ENUM_SHORT_TYPE_NAME) ||
	   (sTypeName == FLOW_ENUM_SHORT_TYPE_NAME) ||
	   (sTypeName == QUERYMODE_ENUM_SHORT_TYPE_NAME)
	   )
	{
		QMetaObject metaObject;
		if(sTypeName == QUERYMODE_ENUM_SHORT_TYPE_NAME)
		{
			metaObject = QextSerialPort::staticMetaObject;
		}
		else
		{
			metaObject = SerialPortEnums::staticMetaObject;
		}		
		int enumeratorIndex = metaObject.indexOfEnumerator(sTypeName.toLatin1());
		if (enumeratorIndex >= 0)
		{
			QMetaEnum metaEnum = metaObject.enumerator(enumeratorIndex);
			int nKeyCount = metaEnum.keyCount();
			for (int i=0;i<nKeyCount;i++)
			{
				hTable.insert(metaEnum.key(i),metaEnum.value(i));
			}
			if (hTable.count() > 0)
				return true;
			return false;
		}
		else
		{
			qDebug() << __FUNCTION__ << "::Unknown Enumeration Type (" << sTypeName << ")!";	
			return false;
		}
		return true;
	}
	else if(sTypeName == OPENMODE_ENUM_SHORT_TYPE_NAME)
	{
		hTable.insert("NotOpen", (int)QIODevice::NotOpen);
		hTable.insert("ReadOnly", (int)QIODevice::ReadOnly);
		hTable.insert("WriteOnly", (int)QIODevice::WriteOnly);
		hTable.insert("ReadWrite", (int)QIODevice::ReadWrite);
		hTable.insert("Append", (int)QIODevice::Append);
		hTable.insert("Truncate", (int)QIODevice::Truncate);
		hTable.insert("Text", (int)QIODevice::Text);
		hTable.insert("Unbuffered", (int)QIODevice::Unbuffered);
		return true;
	}
	return false;
}

//static int EnumIndexFromStringValue(QString &sTypeName, QString &sValue)
//{
	//QMetaObject metaObject = SerialPortEnums::staticMetaObject;
	//QString sEnumShortTypeName;

	//if(sTypeName == BAUDRATE_ENUM_TYPE_NAME)
		//return baudRateHash.value(sValue,UNKNOWNENUMSTRING);
	//	sEnumShortTypeName = BAUDRATE_ENUM_SHORT_TYPE_NAME;
	//else if(sTypeName == DATABITS_ENUM_TYPE_NAME)
	//	sEnumShortTypeName = DATABITS_ENUM_SHORT_TYPE_NAME;
	//else if(sTypeName == PARITY_ENUM_TYPE_NAME)
	//	sEnumShortTypeName =PARITY_ENUM_SHORT_TYPE_NAME;
	//else if(sTypeName == STOPBITS_ENUM_TYPE_NAME)
	//	sEnumShortTypeName = STOPBITS_ENUM_SHORT_TYPE_NAME;
	//else if(sTypeName == FLOW_ENUM_TYPE_NAME)
	//	sEnumShortTypeName = FLOW_ENUM_SHORT_TYPE_NAME;
	//else if(sTypeName == QUERYMODE_ENUM_TYPE_NAME)
	//{
	//	metaObject = QextSerialPort::staticMetaObject;
	//	sEnumShortTypeName = QUERYMODE_ENUM_SHORT_TYPE_NAME;
	//}
	//else if(sTypeName == OPENMODE_ENUM_TYPE_NAME)
	//{
	//	QStringList sFlagList = sValue.split("|",QString::SplitBehavior::SkipEmptyParts);
	//	if (sFlagList.isEmpty() == false)
	//	{
	//		int nFlags = 0;
	//		bool bValueFound = false;
	//		for (int i=0;i<sFlagList.count();i++)
	//		{
	//			if (sFlagList.at(i).toLower().trimmed() == "notopen")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::NotOpen;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "readonly")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::ReadOnly;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "writeonly")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::WriteOnly;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "readwrite")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::ReadWrite;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "append")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::Append;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "truncate")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::Truncate;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "text")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::Text;
	//				bValueFound = true;
	//				continue;
	//			}
	//			else if (sFlagList.at(i).toLower().trimmed() == "unbuffered")
	//			{
	//				nFlags = nFlags +  (int)QIODevice::Unbuffered;
	//				bValueFound = true;
	//				continue;
	//			}
	//		}
	//		if (bValueFound == false)
	//			return -1;//No valid flag defined
	//		return nFlags;
	//	}
	//	return -1;//No flag defined
	//}

	//int index = metaObject.indexOfEnumerator(sEnumShortTypeName.toLatin1());
	//if (index >= 0)
	//{
	//	//int nCount = metaObject.enumeratorCount();
	//	QMetaEnum metaEnum = metaObject.enumerator(index);
	//	int nEnumIndex=metaEnum.keyToValue(sValue.toLatin1());
	//	if (nEnumIndex>=0)
	//	{
	//		return nEnumIndex;
	//	}
	//	else
	//	{
	//		qDebug() << __FUNCTION__ << "::False Enumeration (" << sEnumShortTypeName << ")!";	
	//	}
	//}
//};

//template <typename Tp>
//static QScriptValue qScriptValueFromQObject(QScriptEngine *engine, Tp const &qobject)
//{
//	QString sTmpTypeName = typeid(Tp).name();//SerialPortEnums::BaudRateType).name();
//	QScriptValue obj = engine->newObject();
//
//	if((sTmpTypeName == BAUDRATE_ENUM_TYPE_NAME) || 
//		(sTmpTypeName == DATABITS_ENUM_TYPE_NAME) || 
//		(sTmpTypeName == PARITY_ENUM_TYPE_NAME) || 
//		(sTmpTypeName == STOPBITS_ENUM_TYPE_NAME) ||
//		(sTmpTypeName == QUERYMODE_ENUM_TYPE_NAME) ||
//		(sTmpTypeName == FLOW_ENUM_TYPE_NAME))
//	{
//		obj = (Tp)qobject;
//	}
//	else
//	{
//		qDebug() << __FUNCTION__ << "::Undefined type (" << sTmpTypeName << ")!";
//		obj = qobject;
//	}
//	return obj;
//	//return engine->newVariant((int)s);
//	////return engine->newQObject((int)qobject);
//	//QScriptValue obj = engine->newObject();
//	////obj.setProperty("x", qobject.x);
//	////obj.setProperty("y", qobject.y);
//}

//template <typename Tp>
//static void qScriptValueToQObject(const QScriptValue &value, Tp &qobject)
//{   
//	QString sTmpTypeName = typeid(Tp).name();	
//	QVariant tmpVariant = value.toVariant();
//	QString sVariantTypeName = tmpVariant.typeName();
//	if(sVariantTypeName == "int")
//	{
//		qobject = (Tp)value.toInt32();
//	}
//	else if(sVariantTypeName == "QString")
//	{
//		int nEnumIndex = EnumIndexFromStringValue(sTmpTypeName,tmpVariant.toString()) >=0;
//		if(nEnumIndex>=0)
//		{
//			qobject = (Tp)nEnumIndex;
//		}
//	}
//	//qobject = qobject_cast<Tp>(value.toQObject());
//	//qobject.x = value.property("x").toInt32();
//	//qobject.y = value.property("y").toInt32();
//}

//template <typename Tp>
//static QScriptValue qScriptValueFromQFlags(QScriptEngine *engine, Tp const &qobject)
//{
//	QString sTmpTypeName = typeid(Tp).name();
//	QScriptValue obj = engine->newObject();
//
//	if((sTmpTypeName == OPENMODE_ENUM_TYPE_NAME))
//	{
//		obj = (int)qobject;
//	}
//	else
//	{
//		qDebug() << __FUNCTION__ << "::Undefined type (" << sTmpTypeName << ")!";
//	}
//	return obj;
//}

//template <typename Tp>
//static void qScriptValueToQFlags(const QScriptValue &value, Tp &qobject)
//{   
//	QString sTmpTypeName = typeid(Tp).name();
//
//	if((sTmpTypeName == OPENMODE_ENUM_TYPE_NAME))
//	{
//		QVariant tmpVariant = value.toVariant();
//		QString sVariantTypeName = tmpVariant.typeName();
//		if(sVariantTypeName == "int")
//		{
//			qobject = (Tp)value.toInt32();
//		}
//		else if(sVariantTypeName == "QString")
//		{
//			int nFlags;
//			nFlags = EnumIndexFromStringValue(sTmpTypeName,tmpVariant.toString());
//			if (nFlags>=0)
//			{
//				qobject = (Tp)nFlags;
//			} 
//
//			//QStringList sFlagList = tmpVariant.toString().split("|",QString::SplitBehavior::SkipEmptyParts);
//			//if (sFlagList.isEmpty() == false)
//			//{
//			//	int nFlags = 0;
//			//	for (int i=0;i<sFlagList.count();i++)
//			//	{
//			//		if (sFlagList.at(i).toLower() == "notopen")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::NotOpen;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "readonly")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::ReadOnly;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "writeonly")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::WriteOnly;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "readwrite")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::ReadWrite;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "append")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::Append;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "truncate")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::Truncate;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "text")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::Text;
//			//			continue;
//			//		}
//			//		else if (sFlagList.at(i).toLower() == "unbuffered")
//			//		{
//			//			nFlags = nFlags +  (int)QIODevice::Unbuffered;
//			//			continue;
//			//		}
//			//	}
//			//	qobject = (Tp)nFlags;
//			//}
//		}
//		else
//		{
//			qDebug() << __FUNCTION__ << "::False Enumeration base type (" << sVariantTypeName << ")!";
//		}
//	}
//}

//template <typename Tp>
//QScriptValue qScriptConstructor(QScriptContext *, QScriptEngine *engine)
//{
//	QString sTmpTypeName = typeid(Tp).name();
//
//	if(sTmpTypeName == BAUDRATE_ENUM_TYPE_NAME)
//	{
//		BaudRateType tmpBaudRateType;
//		tmpBaudRateType = BAUD9600;//Default Type Value
//		return engine->toScriptValue(tmpBaudRateType);
//	}
//	else
//	{
//		return NULL;
//	}
//}

//static bool RegisterMetaTypes(QScriptEngine &engine)
//{
//	//QScriptValue ctor = engine.newFunction(qScriptConstructor<BaudRateType>);
//	//engine.globalObject().setProperty(BAUDRATE_ENUM_SHORT_TYPE_NAME, ctor);
//	qScriptRegisterMetaType<SerialPortEnums::BaudRateType>(&engine, qScriptValueFromQObject, qScriptValueToQObject);
//	qScriptRegisterMetaType<SerialPortEnums::DataBitsType>(&engine, qScriptValueFromQObject, qScriptValueToQObject);
//	qScriptRegisterMetaType<SerialPortEnums::ParityType>(&engine, qScriptValueFromQObject, qScriptValueToQObject);
//	qScriptRegisterMetaType<SerialPortEnums::StopBitsType>(&engine, qScriptValueFromQObject, qScriptValueToQObject);
//	qScriptRegisterMetaType<SerialPortEnums::FlowType>(&engine, qScriptValueFromQObject, qScriptValueToQObject);
//	qScriptRegisterMetaType<QextSerialPort::QueryMode>(&engine, qScriptValueFromQObject, qScriptValueToQObject);
//	qScriptRegisterMetaType<QIODevice::OpenMode>(&engine, qScriptValueFromQFlags, qScriptValueToQFlags);
//	qRegisterMetaType<QIODevice::OpenMode>(QString(OPENMODE_ENUM_META_TYPE_NAME).toLatin1());
//	return true;
//}
#endif // CUSTOMSCRIPTOBJECTS_H