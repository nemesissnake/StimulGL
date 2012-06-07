//SerialPortDeviceplugin
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


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef SerialPortDevice_H
#define SerialPortDevice_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QDesktopWidget>
#include <QHash>
#include "qextserialport.h"
#include "CustomScriptObjects.h"
//#include "./../../StimulGL/mainappinfo.h"

#define UNKNOWNENUMSTRING	"Unknown_Enumeration"

//!  The SerialPortDevice class. 
/*!
  The SerialPortDevice can be used to communicate with a USD HID Device.
*/
class SerialPortDevice : public QObject, protected QScriptable
{
	Q_OBJECT

signals:
	//! The CaptureThreadTriggered Signal.
	/*!
		You can use this Signal for receiving a notification when the capture thread gets triggered by the received port value.
		The PortValue parameter(one byte) holds the triggered port value when it gets triggered.
	*/
	//void CaptureThreadTriggered(short PortValue);
	//! The CaptureThreadStarted Signal.
	/*!
		You can use this Signal to keep track of when the capture thread starts.
		Parameter DateTimeStamp holds the date/time of when the capture thread started.
	*/
	//void CaptureThreadStarted(QString);
	//! The CaptureThreadStopped Signal.
	/*!
		You can use this Signal to keep track of when the capture thread stops.
		Parameter DateTimeStamp holds the date/time of when the capture thread stopped.
	*/
	//void CaptureThreadStopped(QString);
	void CaptureThreadReadyRead();

public:
	SerialPortDevice(QObject *parent = 0);//unsigned short vendor_id = 0x181b, unsigned short product_id = 0x4002, QObject *parent = 0);
	SerialPortDevice(const SerialPortDevice& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~SerialPortDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

private slots:
		void ProcessSerialData();
		
public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	QString portName() const; 
	void setPortName(const QString & name);
	QString baudRateToString() const;
	bool setBaudRate(QString baudRate);
	QString dataBitsToString() const;
	bool setDataBits(QString dataBits);
	QString parityToString() const;
	bool setParity(QString parity);
	QString flowControlToString() const;
	bool setFlowControl(QString flow);
	QString stopBitsToString() const;
	bool setStopBits(QString stopBits);
	void setTimeout(long millisec);
	QString queryModeToString() const;
	bool setQueryMode(QString mode);
	bool open(QString mode);
	bool isOpen() const;
	QString openModeToString() const;
	virtual void close();

public:
	void setBaudRate(SerialPortEnums::BaudRateType baudRate);
	void setDataBits(SerialPortEnums::DataBitsType dataBits);
	void setParity(SerialPortEnums::ParityType parity);
	void setFlowControl(SerialPortEnums::FlowType flow);
	void setStopBits(SerialPortEnums::StopBitsType stopBits);
	void setQueryMode(QextSerialPort::QueryMode mode);
	void setTimeout(qlonglong millisec);
	bool open(QIODevice::OpenMode mode);
	
	SerialPortEnums::BaudRateType baudRate() const;
	SerialPortEnums::DataBitsType dataBits () const;
	SerialPortEnums::ParityType parity() const;
	SerialPortEnums::FlowType flowControl() const;
	SerialPortEnums::StopBitsType stopBits() const;
	QextSerialPort::QueryMode queryMode() const;
	QIODevice::OpenMode openMode() const;
	

	//QString errorString()
	//	{return serialPort->errorString();}
	//SerialPortEnums::FlowType flowControl() const
	//	{return serialPort->flowControl();}
	//void flush()
	//	{return serialPort->flush();}
	//ulong lastError() const
	//	{return serialPort->lastError();}
	//ulong lineStatus()
	//	{return serialPort->lineStatus();}
	//bool open(OpenMode mode)
	//	{return serialPort->open(mode);}
	//QueryMode queryMode() const
	//	{return serialPort->queryMode();}
	//QByteArray readAll()
	//	{return serialPort->readAll();}
	//SerialPortEnums::StopBitsType stopBits() const
	//	{return serialPort->stopBits();}

	//void setDataBits( DataBitsType dataBits )
	//void setDtr( bool set = true )
	//void setFlowControl( FlowType flow )
	//void setParity( ParityType parity )
	//void setPortName( const QString & name )
	//void setQueryMode( QueryMode mode )
	//void setRts( bool set = true )
	//void setStopBits( StopBitsType stopBits )
	//void setTimeout( long millisec )

	//virtual qint64 readData( char * data, qint64 maxSize )
	//virtual qint64 writeData( const char * data, qint64 maxSize )

	//virtual qint64 bytesAvailable() const
	//virtual bool canReadLine() const
	//virtual void close()
	//virtual bool isSequential() const

//protected slots:
//	bool ExtendScriptContext(QScriptEngine &engine);

private:
	QScriptEngine* currentScriptEngine;
	QextSerialPort* serialPort;
	QString sTmpTypeName;
	int nTmpEnumIndex;

	QHash<QString, int> baudRateHash;
	QHash<QString, int> dataBitsHash;
	QHash<QString, int> parityHash;
	QHash<QString, int> flowControlHash;
	QHash<QString, int> stopBitsHash;
	QHash<QString, int> queryModeHash;
	QHash<QString, int> openModeHash;
};

#endif // SerialPortDevice_H
