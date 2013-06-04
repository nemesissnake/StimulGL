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
#include <QHash>
#include "maindefines.h"
#include "qextserialport.h"

#define BAUDRATE_ENUM_TYPE_NAME			typeid(BaudRateType).name()
#define BAUDRATE_ENUM_SHORT_TYPE_NAME	"BaudRateType_"
#define DATABITS_ENUM_TYPE_NAME			typeid(DataBitsType).name()
#define DATABITS_ENUM_SHORT_TYPE_NAME	"DataBitsType_"
#define PARITY_ENUM_TYPE_NAME			typeid(ParityType).name()
#define PARITY_ENUM_SHORT_TYPE_NAME		"ParityType_"
#define STOPBITS_ENUM_TYPE_NAME			typeid(StopBitsType).name()
#define STOPBITS_ENUM_SHORT_TYPE_NAME	"StopBitsType_"
#define FLOW_ENUM_TYPE_NAME				typeid(FlowType).name()
#define FLOW_ENUM_SHORT_TYPE_NAME		"FlowType_"
#define QUERYMODE_ENUM_TYPE_NAME		typeid(QextSerialPort::QueryMode).name()
#define QUERYMODE_ENUM_SHORT_TYPE_NAME	"QueryMode_"
#define OPENMODE_ENUM_TYPE_NAME			typeid(QIODevice::OpenMode).name()
#define OPENMODE_ENUM_SHORT_TYPE_NAME	"OpenMode_"
#define OPENMODE_ENUM_META_TYPE_NAME	"QIODevice::OpenMode"

//!  The SerialPortDevice class. 
/*!
  The SerialPortDevice can be used to communicate with a USD HID Device.
*/
class SerialPortDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "SerialPort");//Can't use defines here!, moc doesn't handle defines

signals:
	//! The CaptureThreadReadyRead Signal.
	/*!
	You can use this Signal to keep track of when the capture thread is ready for reading data.
	There's no parameter.
	*/
	void CaptureThreadReadyRead();

public:
	Q_ENUMS(BaudRateType_)
	Q_ENUMS(DataBitsType_)
	Q_ENUMS(ParityType_)
	Q_ENUMS(StopBitsType_)
	Q_ENUMS(FlowType_)
	Q_ENUMS(QueryMode_)

	/*! The enum (BaudRateType_) can store a BAUDRATE setting for a Serial Port */
	enum BaudRateType_
	{
	#if defined(Q_OS_UNIX) || defined(qdoc)
		BAUD50 = 50,				//!< "BAUD50"         
		BAUD75 = 75,				//!< "BAUD75" 
		BAUD134 = 134,              //!< "BAUD134"
		BAUD150 = 150,              //!< "BAUD150"
		BAUD200 = 200,              //!< "BAUD200"
		BAUD1800 = 1800,            //!< "BAUD1800"
	#  if defined(B76800) || defined(qdoc)
		BAUD76800 = 76800,          //!< "BAUD76800"
	#  endif
	#  if (defined(B230400) && defined(B4000000)) || defined(qdoc)
		BAUD230400 = 230400,        //!< "BAUD230400"
		BAUD460800 = 460800,        //!< "BAUD460800"
		BAUD500000 = 500000,        //!< "BAUD500000"
		BAUD576000 = 576000,        //!< "BAUD576000"
		BAUD921600 = 921600,        //!< "BAUD921600"
		BAUD1000000 = 1000000,      //!< "BAUD1000000"
		BAUD1152000 = 1152000,      //!< "BAUD1152000"
		BAUD1500000 = 1500000,      //!< "BAUD1500000"
		BAUD2000000 = 2000000,      //!< "BAUD2000000"
		BAUD2500000 = 2500000,      //!< "BAUD2500000"
		BAUD3000000 = 3000000,      //!< "BAUD3000000"
		BAUD3500000 = 3500000,      //!< "BAUD3500000"
		BAUD4000000 = 4000000,      //!< "BAUD4000000"
	#  endif
	#endif //Q_OS_UNIX
		BAUD110 = 110,				//!< "BAUD110"
		BAUD300 = 300,				//!< "BAUD300"
		BAUD600 = 600,				//!< "BAUD600"
		BAUD1200 = 1200,			//!< "BAUD1200"
		BAUD2400 = 2400,			//!< "BAUD2400"
		BAUD4800 = 4800,			//!< "BAUD4800"
		BAUD9600 = 9600,			//!< "BAUD9600"
#if defined(Q_OS_WIN) || defined(qdoc)
		BAUD14400 = 14400,          //!< "BAUD14400"
#endif  //Q_OS_WIN
		BAUD19200 = 19200,			//!< "BAUD19200"
		BAUD38400 = 38400,			//!< "BAUD38400"
#if defined(Q_OS_WIN) || defined(qdoc)
		BAUD56000 = 56000,          //!< "BAUD56000"
#endif  //Q_OS_WIN
		BAUD57600 = 57600,			//!< "BAUD57600"
#if defined(Q_OS_WIN) || defined(qdoc)
		BAUD115200 = 115200,		//!< "BAUD115200"
		BAUD128000 = 128000,        //!< "BAUD128000"
		BAUD256000 = 256000         //!< "BAUD256000"
#else
		BAUD115200 = 115200			//!< "BAUD115200"
#endif  //Q_OS_WIN
	};
	
	/*! The enum (DataBitsType_) can store a DATABITS setting for a Serial Port */
	enum DataBitsType_
	{
		DATA_5 = 5,					//!< "DATA_5"
		DATA_6 = 6,					//!< "DATA_6"
		DATA_7 = 7,					//!< "DATA_7"
		DATA_8 = 8					//!< "DATA_8"
	};
	
	/*! The enum (ParityType_) can store a PARITYTYPE setting for a Serial Port */
	enum ParityType_
	{
		PAR_NONE,					//!< "PAR_NONE"
		PAR_ODD,					//!< "PAR_ODD"
		PAR_EVEN,					//!< "PAR_EVEN"
	#if defined(Q_OS_WIN) || defined(qdoc)
		PAR_MARK,					//!< "PAR_MARK"              
	#endif
		PAR_SPACE					//!< "PAR_SPACE"
	};
	
	/*! The enum (StopBitsType_) can store a STOPBITS setting for a Serial Port */
	enum StopBitsType_
	{
		STOP_1,						//!< "STOP_1"
	#if defined(Q_OS_WIN) || defined(qdoc)
		STOP_1_5,					//!< "STOP_1_5"
	#endif
		STOP_2						//!< "STOP_2"
	};
	
	/*! The enum (FlowType_) can store a FLOWTYPE setting for a Serial Port */
	enum FlowType_
	{
		FLOW_OFF,					//!< "FLOW_OFF"
		FLOW_HARDWARE,				//!< "FLOW_HARDWARE"
		FLOW_XONXOFF				//!< "FLOW_XONXOFF"
	};

	/*! The enum (QueryMode_) can store a QUERYMODE setting for a Serial Port */
	enum QueryMode_
	{
		Polling,					//!< "Polling"
		EventDriven					//!< "EventDriven"
	};

	/*! The enum (OpenMode_) can store a OPENMODE setting for a Serial Port */
	enum OpenMode_
	{
		NotOpen =	(int)QIODevice::NotOpen,		//!< "NotOpen"
		ReadOnly =	(int)QIODevice::ReadOnly,		//!< "ReadOnly"
		WriteOnly =	(int)QIODevice::WriteOnly,		//!< "WriteOnly"
		ReadWrite =	(int)QIODevice::ReadWrite,		//!< "ReadWrite"
		Append =	(int)QIODevice::Append,			//!< "Append"
		Truncate =	(int)QIODevice::Truncate,		//!< "Truncate"
		Text =		(int)QIODevice::Text,			//!< "Text"
		Unbuffered =(int)QIODevice::Unbuffered		//!< "Unbuffered"
	};

	SerialPortDevice(QObject *parent = 0);//unsigned short vendor_id = 0x181b, unsigned short product_id = 0x4002, QObject *parent = 0);
	SerialPortDevice(const SerialPortDevice& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~SerialPortDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

	bool CreateHashTableFromEnum(const QString &sTypeName, QHash<QString, int> &hTable);
	void setBaudRate(BaudRateType baudRate);
	void setDataBits(DataBitsType dataBits);
	void setParity(ParityType parity);
	void setFlowControl(FlowType flow);
	void setStopBits(StopBitsType stopBits);
	void setQueryMode(QextSerialPort::QueryMode mode);

	bool open(QIODevice::OpenMode mode);

	BaudRateType baudRate() const;
	DataBitsType dataBits () const;
	ParityType parity() const;
	FlowType flowControl() const;
	StopBitsType stopBits() const;
	QextSerialPort::QueryMode queryMode() const;
	QIODevice::OpenMode openMode() const;

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

	void setTimeout(qlonglong millisec);

	QString queryModeToString() const;
	bool setQueryMode(QString mode);
	bool open(QString mode);
	bool isOpen() const;
	QString openModeToString() const;
	virtual void close();

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
