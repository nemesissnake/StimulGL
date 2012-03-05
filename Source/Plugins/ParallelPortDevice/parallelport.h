//parallelportplugin
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


#ifndef PARALLELPORT_H
#define PARALLELPORT_H
#include <QObject>
#include <QString>
#include <QScriptable>
#include "InpOut32.h"
#include "ppCaptureThread.h"
#include "ppGenerateThread.h"
#include "./../../StimulGL/mainappinfo.h"

class ParallelPort : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_PROPERTY( short BaseAddress WRITE setBaseAddress READ getBaseAddress )

signals:
	void GenerateThreadTriggered(short);
	void GenerateThreadStarted(QString);
	void GenerateThreadStopped(QString);
	void CaptureThreadTriggered(short);
	void CaptureThreadStarted(QString);
	void CaptureThreadStopped(QString);

public:
	ParallelPort(short BaseAddress = 888, QObject *parent = 0);
	ParallelPort(const ParallelPort& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~ParallelPort();

public slots:
	void setBaseAddress( short BaseAddress );
	short getBaseAddress() const;

	short PortRead();
	void PortWrite(const short &Value);
	bool IsPortEpp();
	bool IsPortEcp();
	//ECP_Mode GetEcpMode();
	//PortType GetPortType();
	bool IsPortSpp();
	bool IsPortPS2();
	bool IsPortInput();
	bool IsPortOutput();
	bool ConfigurePortForInput();
	void ConfigurePortForOutput();
	QString GetPortDescription();

	bool BitRead(const short nBitNumber);
	void BitWrite(const short nBitNumber, bool Value);
	short BitReset(const short nBitNumber);
	short BitSet(const short nBitNumber);
	short BitToggle(const short nBitNumber);
	short StatusPortRead();
	short DataPortRead();
	void DataPortWrite(const short Value);
	short ControlPortRead();
	void ControlPortWrite(const short Value);
	int GetEppTimeoutBit();

	bool StartGenerateThread(const short baseAddress,const short method, const short outputMask, const short activeValue, const short inActiveValue, const int activePulseTime, const int repetitionTime);
	void StopGenerateThread();

	bool StartCaptureThread(const short baseAddress, const short mask, const short method, const int postLHDelay = 0, const int postHLDelay = 0);
	void StopCaptureThread();

	//bool SetEcpMode(ECP_Mode nEcpModeValue);

private:
	InpOut32 InpOut32Object;
	short nBaseAddress;

	ppCaptureThread *captureThread;
	ppGenerateThread *generateThread;
};

#endif // PARALLELPORT_H
