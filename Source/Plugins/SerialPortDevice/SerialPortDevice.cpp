//SerialPortDeviceplugin
//Copyright (C) 2013  Sven Gijsen
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


#include "SerialPortDevice.h"
//#include <windows.h>

QScriptValue SerialPortDevice::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		QString aa = context->thisObject().toString();//setProperty("bar", ...);
	//		context->thisObject().call()
	////		// ...
	////		// return a non-object value to indicate that the
	////		// thisObject() should be the result of the "new Foo()" expression
	////		//return engine->undefinedValue();
	//	}

	//QScriptValue sRetVal;
	return engine->newQObject(new SerialPortDevice(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor

	//if (context->isCalledAsConstructor()) {
	//	// initialize the new object
	//	QString aa = context->thisObject().toString();//setProperty("bar", ...);
	//	context->thisObject().call())
	//		//		// ...
	//		//		// return a non-object value to indicate that the
	//		//		// thisObject() should be the result of the "new Foo()" expression
	//		//		//return engine->undefinedValue();
	//}
	//return sRetVal;
} 

/*! \brief The SerialPortDevice constructor.
*
*   You do not need to specify the parent object, 
*	the StimulGL script engine automatically retrieves the parent role
*/
SerialPortDevice::SerialPortDevice(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;

	//Windows settings
	serialPort = NULL;
	serialPort = new QextSerialPort();
	serialPort->setBaudRate(BaudRateType::BAUD19200);
	serialPort->setDataBits(DataBitsType::DATA_8);
	serialPort->setFlowControl(FlowType::FLOW_OFF);
	serialPort->setParity(ParityType::PAR_NONE);
	serialPort->setStopBits(StopBitsType::STOP_1);
	serialPort->setTimeout(10);
	serialPort->setPortName("COM1");
	serialPort->setQueryMode(QextSerialPort::EventDriven);

	CreateHashTableFromEnum(QString(BAUDRATE_ENUM_SHORT_TYPE_NAME),baudRateHash);
	CreateHashTableFromEnum(QString(DATABITS_ENUM_SHORT_TYPE_NAME),dataBitsHash);
	CreateHashTableFromEnum(QString(PARITY_ENUM_SHORT_TYPE_NAME),parityHash);
	CreateHashTableFromEnum(QString(FLOW_ENUM_SHORT_TYPE_NAME),flowControlHash);
	CreateHashTableFromEnum(QString(STOPBITS_ENUM_SHORT_TYPE_NAME),stopBitsHash);
	CreateHashTableFromEnum(QString(QUERYMODE_ENUM_SHORT_TYPE_NAME),queryModeHash);
	CreateHashTableFromEnum(QString(OPENMODE_ENUM_SHORT_TYPE_NAME),openModeHash);

	connect(serialPort, SIGNAL(readyRead()), this, SIGNAL(CaptureThreadReadyRead()));
	connect(serialPort, SIGNAL(readyRead()), this, SLOT(ProcessSerialData()));

}

/*! \brief The SerialPortDevice destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
SerialPortDevice::~SerialPortDevice()
{
	disconnect(serialPort, SIGNAL(readyRead()), this, SIGNAL(CaptureThreadReadyRead()));
	disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(ProcessSerialData()));


	if (serialPort)
	{
		serialPort->close();
		delete serialPort;
		serialPort = NULL;
	}
}

bool SerialPortDevice::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
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

void SerialPortDevice::ProcessSerialData()
{
	return;
}

QString SerialPortDevice::portName() const
{
/*! Returns the Serial Port Name. */
	return serialPort->portName();
} 

void SerialPortDevice::setPortName(const QString & name)
{
/*! Changes the Serial Port Name to the string parameter(name). */
	return serialPort->setPortName(name);
}

BaudRateType SerialPortDevice::baudRate() const 
{
	return serialPort->baudRate();
} 

QString SerialPortDevice::baudRateToString() const 
{
/*! Returns the Serial Port BaudRate as a string representation, see SerialPortDevice::BaudRateType_. */
	return baudRateHash.key((int)serialPort->baudRate(),UNKNOWNENUMSTRING);
} 

void SerialPortDevice::setBaudRate(BaudRateType baudRate)
{
	return serialPort->setBaudRate(baudRate);
} 

bool SerialPortDevice::setBaudRate(QString baudRate)
{
/*! Changes the Serial Port BaudRate configuration to the string parameter(baudrate), see SerialPortDevice::BaudRateType_. */
	nTmpEnumIndex =  baudRateHash.value(baudRate,-1);
	if (nTmpEnumIndex>=0)
	{
		serialPort->setBaudRate((BaudRateType)nTmpEnumIndex);
		return true;
	}
	return false;
}

DataBitsType SerialPortDevice::dataBits () const
{
	return serialPort->dataBits();
}

QString SerialPortDevice::dataBitsToString() const
{
/*! Returns the Serial Port DataBits as a string representation, see SerialPortDevice::DataBitsType_. */
	return dataBitsHash.key((int)serialPort->dataBits(),UNKNOWNENUMSTRING);
}

void SerialPortDevice::setDataBits(DataBitsType dataBits)
{
	return serialPort->setDataBits(dataBits);
}

bool SerialPortDevice::setDataBits(QString dataBits)
{
/*! Changes the Serial Port DataBits configuration to the string parameter(dataBits), see SerialPortDevice::DataBitsType_. */
	nTmpEnumIndex =  dataBitsHash.value(dataBits,-1);
	if (nTmpEnumIndex>=0)
	{
		serialPort->setDataBits((DataBitsType)nTmpEnumIndex);
		return true;
	}
	return false;
}

ParityType SerialPortDevice::parity() const
{
	return serialPort->parity();
}

QString SerialPortDevice::parityToString() const
{
/*! Returns the Serial Port Parity as a string representation, see SerialPortDevice::ParityType_. */
	return parityHash.key((int)serialPort->parity(),UNKNOWNENUMSTRING);
}

void SerialPortDevice::setParity(ParityType parity)
{
	return serialPort->setParity(parity);
}

bool SerialPortDevice::setParity(QString parity)
{
/*! Changes the Serial Port Parity configuration to the string parameter(parity), see SerialPortDevice::ParityType_. */
	nTmpEnumIndex =  parityHash.value(parity,-1);
	if (nTmpEnumIndex>=0)
	{
		serialPort->setParity((ParityType)nTmpEnumIndex);
		return true;
	}
	return false;
}

FlowType SerialPortDevice::flowControl() const
{
	return serialPort->flowControl();
}

QString SerialPortDevice::flowControlToString() const
{
/*! Returns the Serial Port Flow Control as a string representation, see SerialPortDevice::FlowType_. */
	return flowControlHash.key((int)serialPort->flowControl(),UNKNOWNENUMSTRING);
}

void SerialPortDevice::setFlowControl(FlowType flow)
{
	return serialPort->setFlowControl(flow);
}

bool SerialPortDevice::setFlowControl(QString flow)
{
/*! Changes the Serial Port Flow Control configuration to the string parameter(flow), see SerialPortDevice::FlowType_. */
	nTmpEnumIndex =  flowControlHash.value(flow,-1);
	if (nTmpEnumIndex>=0)
	{
		serialPort->setFlowControl((FlowType)nTmpEnumIndex);
		return true;
	}
	return false;
}

StopBitsType SerialPortDevice::stopBits() const
{
	return serialPort->stopBits();
}

QString SerialPortDevice::stopBitsToString() const
{
/*! Returns the Serial Port Stop Bits as a string representation, see SerialPortDevice::StopBitsType_. */
	return stopBitsHash.key((int)serialPort->stopBits(),UNKNOWNENUMSTRING);
}

void SerialPortDevice::setStopBits(StopBitsType stopBits)
{
	return serialPort->setStopBits(stopBits);
}

bool SerialPortDevice::setStopBits(QString stopBits)
{
/*! Changes the Serial Port Stop Bits configuration to the string parameter(stopBits), see SerialPortDevice::StopBitsType_. */
	nTmpEnumIndex =  stopBitsHash.value(stopBits,-1);
	if (nTmpEnumIndex>=0)
	{
		serialPort->setStopBits((StopBitsType)nTmpEnumIndex);
		return true;
	}
	return false;
}

void SerialPortDevice::setTimeout(long millisec)
{
/*! Changes the Serial Port Timeout configuration to the long parameter(millisec) in milliseconds. */
	serialPort->setTimeout(millisec);
}

void SerialPortDevice::setTimeout(qlonglong millisec)
{
/*! Changes the Serial Port Timeout configuration to the qlonglong parameter(millisec) in milliseconds. */
	serialPort->setTimeout(millisec);
}

QextSerialPort::QueryMode SerialPortDevice::queryMode() const
{
	return serialPort->queryMode();
}

QString SerialPortDevice::queryModeToString() const
{
/*! Returns the Serial Port Query Mode as a string representation, see SerialPortDevice::QueryMode_. */
	return queryModeHash.key((int)serialPort->queryMode(),UNKNOWNENUMSTRING);
}

void SerialPortDevice::setQueryMode(QextSerialPort::QueryMode mode)
{
	return serialPort->setQueryMode(mode);
}

bool SerialPortDevice::setQueryMode(QString mode)
{
/*! Changes the Serial Port Query Mode configuration to the string parameter(mode), see SerialPortDevice::QueryMode_. */
	nTmpEnumIndex = queryModeHash.value(mode,-1);
	if (nTmpEnumIndex>=0)
	{
		serialPort->setQueryMode((QextSerialPort::QueryMode)nTmpEnumIndex);
		return true;
	}
	return false;
}

bool SerialPortDevice::open(QIODevice::OpenMode mode)
{
	return serialPort->open(mode);
}

bool SerialPortDevice::open(QString mode)
{
/*! Opens the Serial Port for further usage in the mode string parameter(mode), see SerialPortDevice::OpenMode_. */
	QStringList tmpStrLst = mode.split("|",QString::SplitBehavior::SkipEmptyParts);
	int nFlags = 0;
	bool bValueFound = false;
	if (tmpStrLst.isEmpty() == false)
	{
		int nTmpEnumIndex = 0;
		for (int i=0;i<tmpStrLst.count();i++)
		{
			nTmpEnumIndex = openModeHash.value(tmpStrLst[i].trimmed(),-1);
			if (nTmpEnumIndex>=0)
			{
				nFlags = nFlags + nTmpEnumIndex;
				bValueFound = true;
			}
			else
			{
				qDebug() << __FUNCTION__ << "::False Enumeration type found (" << tmpStrLst[i] << ")!";
				return false;
			}
		}
	}
	else
	{
		qDebug() << __FUNCTION__ << "::No Enumeration type found in (" << mode << ")!";
		return false;
	}
	if (bValueFound)
	{
		return serialPort->open((QIODevice::OpenMode)nFlags);
	}
	qDebug() << __FUNCTION__ << "::No Enumeration type found in (" << mode << ")!";
	return false;
}

bool SerialPortDevice::isOpen() const
{
/*! Returns whether the Serial Port is currently open. */
	return serialPort->isOpen();
}

QIODevice::OpenMode SerialPortDevice::openMode() const
{
	return serialPort->openMode();
}

QString SerialPortDevice::openModeToString() const
{
/*! Returns the Serial Port Open Mode as a string representation, see SerialPortDevice::OpenMode_. */
	return openModeHash.key((int)serialPort->openMode(),UNKNOWNENUMSTRING);
}

void SerialPortDevice::close()
{
/*! Closes the Serial Port. */
	return serialPort->close();
}

bool SerialPortDevice::CreateHashTableFromEnum(const QString &sTypeName, QHash<QString, int> &hTable)
{
	if((sTypeName == BAUDRATE_ENUM_SHORT_TYPE_NAME) ||
		(sTypeName == DATABITS_ENUM_SHORT_TYPE_NAME) ||
		(sTypeName == PARITY_ENUM_SHORT_TYPE_NAME) ||
		(sTypeName == STOPBITS_ENUM_SHORT_TYPE_NAME) ||
		(sTypeName == FLOW_ENUM_SHORT_TYPE_NAME) ||
		(sTypeName == QUERYMODE_ENUM_SHORT_TYPE_NAME)
		)
	{
		const QMetaObject metaObject = this->staticMetaObject;//SerialPortEnums::staticMetaObject;//this->staticMetaObject;// 
		//int test = metaObject.enumeratorCount();
		//QMetaEnum testmetaEnum = metaObject.enumerator(test-1);
		//QString teststr = testmetaEnum.name();

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
