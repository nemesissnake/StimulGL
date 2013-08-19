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


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef SerialPortDevice_H
#define SerialPortDevice_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QDesktopWidget>
//#include <QHash>
#include <QSerialPort>
#include <QSerialPortInfo>
#include "maindefines.h"

//!  The SerialPortDevice class. 
/*!
  The SerialPortDevice can be used to communicate with a Device connected to the Serial Port.
*/
class SerialPortDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "SerialPort");//Can't use defines here!, moc doesn't handle defines

signals:
	//! The SerialDataReceived Signal.
	/*!
	This Signal is emitted each time new Serial Data is received.
	The sData parameter contains the last received data stream.
	*/
	void SerialDataReceived(QString sData);

public:
	SerialPortDevice(QObject *parent = 0);//unsigned short vendor_id = 0x181b, unsigned short product_id = 0x4002, QObject *parent = 0);
	SerialPortDevice(const SerialPortDevice& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~SerialPortDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	bool isInitialized(bool bDoInit = false);
	void handleError(QSerialPort::SerialPortError error);
	
	//QSerialPort(QObject * parent = 0)
	//QSerialPort(const QString & name, QObject * parent = 0)
	//QSerialPort(const QSerialPortInfo & serialPortInfo, QObject * parent = 0)
	//virtual	~QSerialPort()

	int baudRate(int dir = (int)QSerialPort::AllDirections);// {return baudRate_((QSerialPort::Directions)dir);};
	bool clear(int dir = (int)QSerialPort::AllDirections);// {return clear_((QSerialPort::Directions)dir);};
	bool setFlowControl(int flow);// {return setFlowControl_((QSerialPort::FlowControl)flow);};
	void clearError();
	int dataBits();
	int dataErrorPolicy();
	int error();
	int flowControl();
	bool flush();
	bool isDataTerminalReady();
	bool isRequestToSend();
	bool open(int mode);
	void close();
	int parity();
	int pinoutSignals();
	QString	portName();
	qint64 readBufferSize();
	bool sendBreak(int duration = 0);
	bool setBaudRate(int baudRate, int dir = (int)QSerialPort::AllDirections);
	bool setBreakEnabled(bool set = true);
	bool setDataBits(int dataBits);
	bool setDataErrorPolicy(int policy = (int)QSerialPort::IgnorePolicy);
	bool setDataTerminalReady(bool set);
	bool setParity(int parity);
	bool setPortName(const QString & name);
	void setReadBufferSize(qint64 size);
	bool setRequestToSend(bool set);
	void setSettingsRestoredOnClose(bool restore);
	bool setStopBits(int stopBits);
	bool settingsRestoredOnClose();
	int stopBits();

	qint64 writeData(const QString &data);
	void readData();
	void setPort(const QSerialPortInfo & serialPortInfo);
	QByteArray readAll();
	QString errorString();

private:
	QScriptEngine* currentScriptEngine;
	QSerialPort* serialPort;
};

#endif // SerialPortDevice_H
