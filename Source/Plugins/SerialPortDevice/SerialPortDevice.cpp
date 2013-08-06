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
	Q_UNUSED(context);
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
	serialPort = NULL;

	//connect(serialPort, SIGNAL(readyRead()), this, SIGNAL(CaptureThreadReadyRead()));
	//connect(serialPort, SIGNAL(readyRead()), this, SLOT(ProcessSerialData()));

}

/*! \brief The SerialPortDevice destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
SerialPortDevice::~SerialPortDevice()
{
	//disconnect(serialPort, SIGNAL(readyRead()), this, SIGNAL(CaptureThreadReadyRead()));
	//disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(ProcessSerialData()));


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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool SerialPortDevice::isInitialized(bool bDoInit /*=false*/) 
	{
		if (serialPort == NULL) 
		{
			if(bDoInit)
			{
				serialPort = new QSerialPort();
				return true;
			}
			return false;
		}
		return true;
	}

	qint32 SerialPortDevice::baudRate(QSerialPort::Directions dir /*= AllDirections*/) 
	{
		if(isInitialized())
		{
			qint32 tmp = serialPort->baudRate(dir);
			return tmp;
		}
		return -1;
	}
	
	bool SerialPortDevice::clear(QSerialPort::Directions dir /*= AllDirections*/)
	{
		if(isInitialized())
		{
			return serialPort->clear(dir);
		}
		return false;
	}
	
	void SerialPortDevice::clearError()
	{
		if(isInitialized())
		{
			serialPort->clearError();
		}
	}
	
	QSerialPort::DataBits SerialPortDevice::dataBits() 
	{
		if(isInitialized())
		{
			return serialPort->dataBits();
		}
		return QSerialPort::UnknownDataBits;
	}
	
	QSerialPort::DataErrorPolicy SerialPortDevice::dataErrorPolicy() 
	{
		if(isInitialized())
		{
			return serialPort->dataErrorPolicy();
		}
		return QSerialPort::UnknownPolicy;
	}
	
	QSerialPort::SerialPortError SerialPortDevice::error() 
	{
		if(isInitialized())
		{
			return serialPort->error();
		}
		return QSerialPort::NoError;
	}
	
	QSerialPort::FlowControl SerialPortDevice::flowControl() 
	{
		if(isInitialized())
		{
			return serialPort->flowControl();
		}
		return QSerialPort::UnknownFlowControl;
	}
	
	bool SerialPortDevice::flush()
	{
		if(isInitialized())
		{
			return serialPort->flush();
		}
		return false;
	}
	
	bool SerialPortDevice::isDataTerminalReady()
	{
		if(isInitialized())
		{
			return serialPort->isDataTerminalReady();
		}
		return false;
	}
	
	bool SerialPortDevice::isRequestToSend()
	{
		if(isInitialized())
		{
			return serialPort->isRequestToSend();
		}
		return false;
	}
	
	bool SerialPortDevice::open(QSerialPort::OpenMode mode)
	{
		if(isInitialized())
		{
			return serialPort->open(mode);
		}
		return false;
	}
	
	QSerialPort::Parity SerialPortDevice::parity() 
	{
		if(isInitialized())
		{
			return serialPort->parity();
		}
		return QSerialPort::UnknownParity;
	}
	
	QSerialPort::PinoutSignals SerialPortDevice::pinoutSignals()
	{
		if(isInitialized())
		{
			return serialPort->pinoutSignals();
		}
		return -1;
	}
	
	QString SerialPortDevice::portName() 
	{
		if(isInitialized())
		{
			return serialPort->portName();
		}
		return -1;
	}
	
	qint64 SerialPortDevice::readBufferSize() 
	{
		if(isInitialized())
		{
			return serialPort->readBufferSize();
		}
		return -1;
	}
	
	bool SerialPortDevice::sendBreak(int duration /*= 0*/)
	{
		if(isInitialized())
		{
			return serialPort->sendBreak(duration);
		}
		return false;
	}
	
	bool SerialPortDevice::setBaudRate(qint32 baudRate, QSerialPort::Directions dir /*dir = AllDirections*/)
	{
		if(isInitialized())
		{
			return serialPort->setBaudRate(baudRate, dir);
		}
		return false;
	}
	
	bool SerialPortDevice::setBreakEnabled(bool set /*= true*/)
	{
		if(isInitialized())
		{
			return serialPort->setBreakEnabled(set);
		}
		return false;
	}
	
	bool SerialPortDevice::setDataBits(QSerialPort::DataBits dataBits)
	{
		if(isInitialized())
		{
			return serialPort->setDataBits(dataBits);
		}
		return false;
	}
	
	bool SerialPortDevice::setDataErrorPolicy(QSerialPort::DataErrorPolicy policy/* = IgnorePolicy*/)
	{
		if(isInitialized())
		{
			return serialPort->setDataErrorPolicy(policy);
		}
		return false;
	}
	
	bool SerialPortDevice::setDataTerminalReady(bool set)
	{
		if(isInitialized())
		{
			return serialPort->setDataTerminalReady(set);
		}
		return false;
	}
	
	bool SerialPortDevice::setFlowControl(QSerialPort::FlowControl flow)
	{
		if(isInitialized())
		{
			return serialPort->setFlowControl(flow);
		}
		return false;
	}
	
	bool SerialPortDevice::setParity(QSerialPort::Parity parity)
	{
		if(isInitialized())
		{
			return serialPort->setParity(parity);
		}
		return false;
	}
	
	void SerialPortDevice::setPort(const QSerialPortInfo & serialPortInfo)
	{
		if(isInitialized(true))
		{
			serialPort->setPort(serialPortInfo);
		}
	}
	
	void SerialPortDevice::setPortName(const QString & name)
	{
		if(isInitialized(true))
		{
			serialPort->setPortName(name);
		}
	}
	
	void SerialPortDevice::setReadBufferSize(qint64 size)
	{
		if(isInitialized())
		{
			serialPort->setReadBufferSize(size);
		}
	}
	
	bool SerialPortDevice::setRequestToSend(bool set)
	{
		if(isInitialized())
		{
			return serialPort->setRequestToSend(set);
		}
		return false;
	}
	
	void SerialPortDevice::setSettingsRestoredOnClose(bool restore)
	{
		if(isInitialized())
		{
			serialPort->setSettingsRestoredOnClose(restore);
		}
	}
	
	bool SerialPortDevice::setStopBits(QSerialPort::StopBits stopBits)
	{
		if(isInitialized())
		{
			return serialPort->setStopBits(stopBits);
		}
		return false;
	}
	
	bool SerialPortDevice::settingsRestoredOnClose() 
	{
		if(isInitialized())
		{
			return serialPort->settingsRestoredOnClose();
		}
		return false;
	}
	
	QSerialPort::StopBits SerialPortDevice::stopBits() 
	{
		if(isInitialized())
		{
			return serialPort->stopBits();
		}
		return QSerialPort::UnknownStopBits;
	}




