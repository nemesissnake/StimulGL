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


#include "parallelport.h"

ParallelPort::ParallelPort(short BaseAddress, QObject *parent) : QObject(parent)
{
	nBaseAddress = BaseAddress;
	generateThread = NULL;
	captureThread = NULL;
}

ParallelPort::~ParallelPort()
{
	if (captureThread)
	{
		delete captureThread;
		captureThread = NULL;
	}
	if (generateThread)
	{
		delete generateThread;
		generateThread = NULL;
	}
}

void ParallelPort::setBaseAddress( short BaseAddress )
{
	nBaseAddress = BaseAddress;
}

short ParallelPort::getBaseAddress() const
{
	return nBaseAddress;
}

short ParallelPort::PortRead()
{
	return InpOut32Object.ReadPort(nBaseAddress);
}

void ParallelPort::PortWrite(const short &Value)
{
	InpOut32Object.WritePort(nBaseAddress, Value);	
}

bool ParallelPort::IsPortEpp()
{
	return InpOut32Object.IsPortEpp(nBaseAddress);
}

bool ParallelPort::IsPortEcp()
{
	return InpOut32Object.IsPortEcp(nBaseAddress);
}
//ECP_Mode ParallelPort::GetEcpMode()
//PortType ParallelPort::GetPortType()
bool ParallelPort::IsPortSpp()
{
	return InpOut32Object.IsPortSpp(nBaseAddress);
}
bool ParallelPort::IsPortPS2()
{
	return InpOut32Object.IsPortPS2(nBaseAddress);
}
bool ParallelPort::IsPortInput()
{
	return InpOut32Object.IsPortInput(nBaseAddress);
}
bool ParallelPort::IsPortOutput()
{
	return InpOut32Object.IsPortOutput(nBaseAddress);
}
bool ParallelPort::ConfigurePortForInput()
{
	return InpOut32Object.ConfigurePortForInput(nBaseAddress);
}
void ParallelPort::ConfigurePortForOutput()
{
	InpOut32Object.ConfigurePortForOutput(nBaseAddress);
}
QString ParallelPort::GetPortDescription()
{
	return InpOut32Object.GetPortDescription(nBaseAddress);
}
bool ParallelPort::BitRead(const short nBitNumber)
{
	return InpOut32Object.BitRead(nBaseAddress,nBitNumber);
}
void ParallelPort::BitWrite(const short nBitNumber, bool Value)
{
	InpOut32Object.WritePortBit(nBaseAddress,nBitNumber,Value);
}
short ParallelPort::BitReset(const short nBitNumber)
{
	return InpOut32Object.BitReset(nBaseAddress,nBitNumber);
}
short ParallelPort::BitSet(const short nBitNumber)
{
	return InpOut32Object.BitSet(nBaseAddress,nBitNumber);
}
short ParallelPort::BitToggle(const short nBitNumber)
{
	return InpOut32Object.BitToggle(nBaseAddress,nBitNumber);
}
short ParallelPort::StatusPortRead()
{
	return InpOut32Object.StatusPortRead(nBaseAddress);
}
short ParallelPort::DataPortRead()
{
	return InpOut32Object.DataPortRead(nBaseAddress);	
}
void ParallelPort::DataPortWrite(const short Value)
{
	InpOut32Object.DataPortWrite(nBaseAddress, Value);
}
short ParallelPort::ControlPortRead()
{
	return InpOut32Object.ControlPortRead(nBaseAddress);
}
void ParallelPort::ControlPortWrite(const short Value)
{
	InpOut32Object.ControlPortWrite(nBaseAddress, Value);
}
int ParallelPort::GetEppTimeoutBit()
{
	return InpOut32Object.GetEppTimeoutBit(nBaseAddress);
}
//bool ParallelPort::SetEcpMode(ECP_Mode nEcpModeValue)


bool ParallelPort::StartGenerateThread(const short baseAddress,const short method, const short outputMask, const short activeValue, const short inActiveValue, const int activePulseTime, const int repetitionTime)
{
	ppGenerateThread::GenerationMethod GenMethod;
	if (method == 0) 
        {GenMethod = ppGenerateThread::Value;}
	else if (method == 1) 
    {GenMethod = ppGenerateThread::Pulse;}
	else 
    {GenMethod = ppGenerateThread::Periodical;}

	generateThread = new ppGenerateThread(baseAddress,GenMethod,outputMask,activeValue,inActiveValue,activePulseTime,repetitionTime,this);
		
	connect(generateThread, SIGNAL(generateThreadTriggered(short)), this, SIGNAL(GenerateThreadTriggered(short)));
	connect(generateThread, SIGNAL(generateThreadStarted(QString)), this, SIGNAL(GenerateThreadStarted(QString)));
	connect(generateThread, SIGNAL(generateThreadStopped(QString)), this, SIGNAL(GenerateThreadStopped(QString)));
	generateThread->start();
	return true;
}

void ParallelPort::StopGenerateThread()
{
	generateThread->stop();
}

bool ParallelPort::StartCaptureThread(const short baseAddress, const short mask, const short method,const int postLHDelay, const int postHLDelay)
{
	ppCaptureThread::DetectionMethod CapDecMethod;
	if (method == 0) 
    {CapDecMethod = ppCaptureThread::MaskedValueChanged;}
	else if (method == 1) 
    {CapDecMethod = ppCaptureThread::MaskedValueChangedHigh;}
	else 
    {CapDecMethod = ppCaptureThread::MaskedValueChangedLow;}

	captureThread = new ppCaptureThread(baseAddress,mask,CapDecMethod,postLHDelay,postHLDelay,this);

	connect(captureThread, SIGNAL(recieveThreadTriggered(short)), this, SIGNAL(CaptureThreadTriggered(short)));
	connect(captureThread, SIGNAL(recieveThreadStarted(QString)), this, SIGNAL(CaptureThreadStarted(QString)));
	connect(captureThread, SIGNAL(recieveThreadStopped(QString)), this, SIGNAL(CaptureThreadStopped(QString)));
	captureThread->start();
	return true;
}

void ParallelPort::StopCaptureThread()
{
	captureThread->stop();
}
