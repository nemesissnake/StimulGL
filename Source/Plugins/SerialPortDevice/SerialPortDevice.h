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
	Q_ENUMS(BaudRateType_)
	Q_ENUMS(DataBitsType_)
	Q_ENUMS(ParityType_)
	Q_ENUMS(StopBitsType_)
	Q_ENUMS(FlowType_)
	Q_ENUMS(QueryMode_)

	enum BaudRateType_
	{
	#if defined(Q_OS_UNIX) || defined(qdoc)
		BAUD50 = 50,                //POSIX ONLY
		BAUD75 = 75,                //POSIX ONLY
		BAUD134 = 134,              //POSIX ONLY
		BAUD150 = 150,              //POSIX ONLY
		BAUD200 = 200,              //POSIX ONLY
		BAUD1800 = 1800,            //POSIX ONLY
	#  if defined(B76800) || defined(qdoc)
		BAUD76800 = 76800,          //POSIX ONLY
	#  endif
	#  if (defined(B230400) && defined(B4000000)) || defined(qdoc)
		BAUD230400 = 230400,        //POSIX ONLY
		BAUD460800 = 460800,        //POSIX ONLY
		BAUD500000 = 500000,        //POSIX ONLY
		BAUD576000 = 576000,        //POSIX ONLY
		BAUD921600 = 921600,        //POSIX ONLY
		BAUD1000000 = 1000000,      //POSIX ONLY
		BAUD1152000 = 1152000,      //POSIX ONLY
		BAUD1500000 = 1500000,      //POSIX ONLY
		BAUD2000000 = 2000000,      //POSIX ONLY
		BAUD2500000 = 2500000,      //POSIX ONLY
		BAUD3000000 = 3000000,      //POSIX ONLY
		BAUD3500000 = 3500000,      //POSIX ONLY
		BAUD4000000 = 4000000,      //POSIX ONLY
	#  endif
	#endif //Q_OS_UNIX
	#if defined(Q_OS_WIN) || defined(qdoc)
		BAUD14400 = 14400,          //WINDOWS ONLY
		BAUD56000 = 56000,          //WINDOWS ONLY
		BAUD128000 = 128000,        //WINDOWS ONLY
		BAUD256000 = 256000,        //WINDOWS ONLY
	#endif  //Q_OS_WIN
		BAUD110 = 110,
		BAUD300 = 300,
		BAUD600 = 600,
		BAUD1200 = 1200,
		BAUD2400 = 2400,
		BAUD4800 = 4800,
		BAUD9600 = 9600,
		BAUD19200 = 19200,
		BAUD38400 = 38400,
		BAUD57600 = 57600,
		BAUD115200 = 115200
	};
		
	enum DataBitsType_
	{
		DATA_5 = 5,
		DATA_6 = 6,
		DATA_7 = 7,
		DATA_8 = 8
	};
		
	enum ParityType_
	{
		PAR_NONE,
		PAR_ODD,
		PAR_EVEN,
	#if defined(Q_OS_WIN) || defined(qdoc)
		PAR_MARK,               //WINDOWS ONLY
	#endif
		PAR_SPACE
	};
		
	enum StopBitsType_
	{
		STOP_1,
	#if defined(Q_OS_WIN) || defined(qdoc)
		STOP_1_5,               //WINDOWS ONLY
	#endif
		STOP_2
	};
		
	enum FlowType_
	{
		FLOW_OFF,
		FLOW_HARDWARE,
		FLOW_XONXOFF
	};

	enum QueryMode_
	{
		Polling,
		EventDriven
	};

public:
	SerialPortDevice(QObject *parent = 0);//unsigned short vendor_id = 0x181b, unsigned short product_id = 0x4002, QObject *parent = 0);
	SerialPortDevice(const SerialPortDevice& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~SerialPortDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);
	bool CreateHashTableFromEnum(const QString &sTypeName, QHash<QString, int> &hTable);

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

public:
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
