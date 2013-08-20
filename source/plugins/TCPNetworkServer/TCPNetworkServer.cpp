//TCPNetworkServer
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


#include "TCPNetworkServer.h"
#include <QNetworkInterface>

QScriptValue TCPNetworkServer::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();
	Q_UNUSED(context);
	return engine->newQObject(new TCPNetworkServer(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

TCPNetworkServer::TCPNetworkServer(QObject *parent) : QTcpServer(parent)//QObject(parent)
{
	currentScriptEngine = NULL;
	clientConnectionSocket = NULL;
	blockSize = 0;
}

TCPNetworkServer::~TCPNetworkServer()
{
	if(clientConnectionSocket)
	{
		clientConnectionSocket->close();
		delete clientConnectionSocket;
		clientConnectionSocket = NULL;
	}
	if(serverClientConnectionSockets.isEmpty() == false)
	{
		int nCount = serverClientConnectionSockets.count();
		for(int i=0;i<nCount-1;i++)
		{
			serverClientConnectionSockets.first()->disconnectFromHost();
			delete serverClientConnectionSockets.first();
			serverClientConnectionSockets.removeFirst();
		}
	}
}

bool TCPNetworkServer::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}


//(const QString &sAddress = "", int port = 0);
//
//const QString &sAddress, quint16 port)
//{
//	if (globAppInfo->shouldEnableNetworkServer())
//	{
//		tcpServer = new QTcpServer(this);
//
//		QHostAddress address = QHostAddress::Any;
//		if(sAddress != "")
//			address.setAddress(sAddress);

QString TCPNetworkServer::startServer(const QString &sAddress, int port)
{
	QHostAddress address = QHostAddress::Any;
	if(sAddress != "")
		address.setAddress(sAddress);
	if (!this->listen(address,port)) 
	{
		return QString("Unable to start the server: %1.").arg(this->errorString());
	}
	QString ipAddress;
	QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
	// use the first non-localhost IPv4 address
	for (int i = 0; i < ipAddressesList.size(); ++i) {
		if (ipAddressesList.at(i) != QHostAddress::LocalHost &&	ipAddressesList.at(i).toIPv4Address()) 
		{
				ipAddress = ipAddressesList.at(i).toString();
				break;
		}
	}
	// if we did not find one, use IPv4 localhost
	if (ipAddress.isEmpty())
		ipAddress = QHostAddress(QHostAddress::LocalHost).toString();
	connect(this, SIGNAL(newConnection()), this, SLOT(newIncomingConnectionFromClient()));
	QString usedAddress = sAddress;
	if(usedAddress == "")
		usedAddress = QString("%1").arg(ipAddress);
	QString sInfo = QString("%1:%2").arg(usedAddress).arg(this->serverPort());
	return sInfo;
}

void TCPNetworkServer::newIncomingConnectionFromClient()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_0);
	out << (quint16)0;
	out << QString("newIncomingConnectionFromClient() reaction data packet");//fortunes.at(qrand() % fortunes.size());
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	serverClientConnectionSockets.append(this->nextPendingConnection());
	serverClientConnectionSockets.last()->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	serverClientConnectionSockets.last()->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
	connect(serverClientConnectionSockets.last(), SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
	connect(serverClientConnectionSockets.last(), SIGNAL(readyRead()), this, SLOT(dataFromClientAvailable()));
	connect(serverClientConnectionSockets.last(), SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientErrorNetworkData(QAbstractSocket::SocketError)));
	serverClientConnectionSockets.last()->write(block);	
}

void TCPNetworkServer::clientDisconnected()
{
	serverClientConnectionSockets.last()->deleteLater();
}

void TCPNetworkServer::clientErrorNetworkData(QAbstractSocket::SocketError socketError)
{
	switch (socketError) 
	{
	case QAbstractSocket::RemoteHostClosedError:
		qDebug() << __FUNCTION__ << "The client disconnected the network connection.";
		break;
	case QAbstractSocket::HostNotFoundError:
		qDebug() << __FUNCTION__ << "The host was not found. Please check the host name and port settings.";
		break;
	case QAbstractSocket::ConnectionRefusedError:
		qDebug() << __FUNCTION__ << "The connection was refused by the peer. "
			"Make sure the network server is running, "
			"and check that the host name and port "
			"settings are correct.";
		break;
	default:
		qDebug() << __FUNCTION__ << QString("The following Network error occurred: %1.").arg(serverClientConnectionSockets.last()->errorString());
	}
}

int TCPNetworkServer::sendServerData(QString sData)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_0);
	out << (quint16)0;
	out << sData;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	qint64 bytesSend = serverClientConnectionSockets.last()->write(block);		
	return (int)bytesSend;
}

int TCPNetworkServer::sendClientData(QString sData)
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_0);
	out << (quint16)0;
	out << sData;
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));
	qint64 bytesSend = clientConnectionSocket->write(block);		
	return (int)bytesSend;
}

bool TCPNetworkServer::connectToServer(const QString &sAddress, int port)
{
	clientConnectionSocket = new QTcpSocket(this);
	clientConnectionSocket->abort();
	clientConnectionSocket->connectToHost(sAddress, (quint16)port);
	clientConnectionSocket->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	clientConnectionSocket->setSocketOption(QAbstractSocket::KeepAliveOption, 1);

	connect(clientConnectionSocket, SIGNAL(disconnected()), this, SLOT(serverDisconnected()));
	connect(clientConnectionSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(serverErrorNetworkData(QAbstractSocket::SocketError)));
	return connect(clientConnectionSocket, SIGNAL(readyRead()), this, SLOT(dataFromServerAvailable()));
}

void TCPNetworkServer::serverDisconnected()
{
	clientConnectionSocket->deleteLater();
}

void TCPNetworkServer::serverErrorNetworkData(QAbstractSocket::SocketError socketError)
{
	switch (socketError) 
	{
	case QAbstractSocket::RemoteHostClosedError:
		qDebug() << __FUNCTION__ << "The client disconnected the network connection.";
		break;
	case QAbstractSocket::HostNotFoundError:
		qDebug() << __FUNCTION__ << "The host was not found. Please check the host name and port settings.";
		break;
	case QAbstractSocket::ConnectionRefusedError:
		qDebug() << __FUNCTION__ << "The connection was refused by the peer. "
			"Make sure the network server is running, "
			"and check that the host name and port "
			"settings are correct.";
		break;
	default:
		qDebug() << __FUNCTION__ << QString("The following Network error occurred: %1.").arg(clientConnectionSocket->errorString());
	}
}

void TCPNetworkServer::dataFromClientAvailable()
{
	QDataStream in(serverClientConnectionSockets.last());
	in.setVersion(QDataStream::Qt_5_0);
	if (blockSize == 0) 
	{
		if (serverClientConnectionSockets.last()->bytesAvailable() < (int)sizeof(quint16))
			return;
		in >> blockSize;
		//in.skipRawData(sizeof(blockSize));
	}
	if (serverClientConnectionSockets.last()->bytesAvailable() < blockSize)
		return;
	QString sAvailableData;
	in >> sAvailableData;
	//in.skipRawData(sizeof(sAvailableData));
	emit ClientDataAvailable(sAvailableData);
	blockSize = 0;
	dataFromClientAvailable();
}

void TCPNetworkServer::dataFromServerAvailable()
{
	QDataStream in(clientConnectionSocket);
	in.setVersion(QDataStream::Qt_5_0);
	if (blockSize == 0) 
	{
		if (clientConnectionSocket->bytesAvailable() < (int)sizeof(quint16))
			return;
		in >> blockSize;
		//in.skipRawData(sizeof(blockSize));
	}
	if (clientConnectionSocket->bytesAvailable() < blockSize)
		return;
	QString sAvailableData;
	in >> sAvailableData;
	//in.skipRawData(sizeof(sAvailableData));
	emit ServerDataAvailable(sAvailableData);
	blockSize = 0;
	dataFromServerAvailable();
}
