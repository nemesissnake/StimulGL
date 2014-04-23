//Copyright (C) 2014  Michael Luehrs, Brain Innovation B.V.
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

#include "tbv_server_request.h"


TBV_Server_Request::TBV_Server_Request(QTcpSocket *tcpSocket,QQueue<QString> *querryQueue, QString querry) 
{
	eTcpSocket = tcpSocket;
	nrOfValues = 0;
	blockSize = 0;
	querryQueueinternal = querryQueue;
	querryQueueinternal->enqueue(querry);
	iQuerry = querry;
	
	while(querryQueueinternal->head() != iQuerry);
}

TBV_Server_Request::~TBV_Server_Request()
{
	querryQueueinternal->dequeue();
}


bool TBV_Server_Request::sendData(char *Querry,int value0, int value1, int value2, int value3)
{
	QByteArray requ;
	QDataStream out(&requ, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (qint64)0;
	out << Querry;	
	if(value0 == -1)
	{
		nrOfValues = 0;		
	}
	else
	if(value1 == -1)
	{
		out << value0;
		nrOfValues = 1;
	}
	else
	if(value2 == -1)
	{
		out << value0;
		out << value1;
		nrOfValues = 2;
	}
	else
	if(value3 == -1)
	{
		out << value0;
		out << value1;
		out << value2;
		nrOfValues = 3;
	}
	else
	{
		out << value0;
		out << value1;
		out << value2;
		out << value3;
		nrOfValues = 4;
	}
	out.device()->seek(0);
	out << (qint64)(requ.size() - sizeof(qint64));

	return sendQByteArray(requ);

}

bool TBV_Server_Request::sendQByteArray(QByteArray dataToSend)
{

	if(eTcpSocket->write(dataToSend) == 0)
	{
		if(eTcpSocket->state() != QAbstractSocket::ConnectedState)
			return false;
	}
	
	if(!eTcpSocket->waitForBytesWritten())
	{
		return false;
	}

	return true;

}


QString TBV_Server_Request::getReply()
{

	const int Timeout = 2 * 1000;

	while(true)
	{
		while (eTcpSocket->bytesAvailable() < (int)sizeof(qint64)) 
		{
			if (!eTcpSocket->waitForReadyRead(Timeout)) 
			{
				return eTcpSocket->errorString();
			}
		}

		rcvStream.setDevice(eTcpSocket);
		rcvStream.setVersion(QDataStream::Qt_4_0);
		rcvStream >> blockSize;

		while (eTcpSocket->bytesAvailable() < blockSize) 
		{
			if (!eTcpSocket->waitForReadyRead(Timeout)) 
			{
				return eTcpSocket->errorString();
			}
		}
		
		blockSize = 0;
		char *rcvChar;
		rcvStream >> rcvChar;

		if(!rcvChar)
		{
			while (!eTcpSocket->atEnd()) 
				{
					QByteArray data = eTcpSocket->read(100);
				}
		}
		else
		if(strcmp(rcvChar , "Wrong request!") == 0)
		{
			char *wrongRequest;
			rcvStream >> wrongRequest;
			return QString(wrongRequest);
		}
		else
		{
			int trash;
			for(int i=0;i<nrOfValues;i++)
				rcvStream >> trash;
			return QString("");
		}
		
		

	}

}

void TBV_Server_Request::getDataOfByteArray(char *data,int size)
{
	rcvStream.readRawData(data,size);
}

