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

	return engine->newQObject(new TCPNetworkServer(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

TCPNetworkServer::TCPNetworkServer(QObject *parent) : QTcpServer(parent)//QObject(parent)
{
	currentScriptEngine = NULL;
	tcpSocket = NULL;
	blockSize = 0;
}

TCPNetworkServer::~TCPNetworkServer()
{
	if(tcpSocket)
	{
		tcpSocket->close();
		delete tcpSocket;
		tcpSocket = NULL;
	}
}

bool TCPNetworkServer::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
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

QString TCPNetworkServer::startListening(QString a, QString b)
{
	//tcpServer = new QTcpServer(this);
	if (!this->listen()) 
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

	connect(this, SIGNAL(newConnection()), this, SLOT(newIncomingServerConnection()));
	return QString("%1:%2").arg(ipAddress).arg(this->serverPort());
	//return QString("The server is running on\n\tIP: %1\n\tport: %2\nRun the Client now.").arg(ipAddress).arg(this->serverPort());
}

void TCPNetworkServer::newIncomingServerConnection()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_0);
	out << (quint16)0;
	out << QString("You've been leading a dog's life. Stay off the furniture.");//fortunes.at(qrand() % fortunes.size());
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));

	QTcpSocket *clientConnection = this->nextPendingConnection();
	connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));

	clientConnection->write(block);
	clientConnection->disconnectFromHost();
}

bool TCPNetworkServer::connectToServer(QString a, QString b)
{
	tcpSocket = new QTcpSocket(this);
	tcpSocket->abort();
	tcpSocket->connectToHost(a,b.toInt());

	//connect(hostCombo, SIGNAL(editTextChanged(QString)), this, SLOT(enableGetFortuneButton()));
	//connect(portLineEdit, SIGNAL(textChanged(QString)), this, SLOT(enableGetFortuneButton()));
	//connect(getFortuneButton, SIGNAL(clicked()),this, SLOT(requestNewFortune()));
	//connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
	return connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(serverDataAvailable()));

	//connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void TCPNetworkServer::serverDataAvailable()
{
	QDataStream in(tcpSocket);
	in.setVersion(QDataStream::Qt_5_0);

	

	if (blockSize == 0) 
	{
		if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
			return;
		in >> blockSize;
	}

	if (tcpSocket->bytesAvailable() < blockSize)
		return;

	QString nextFortune;
	in >> nextFortune;

	blockSize = 0;
	//if (nextFortune == currentFortune) 
	//{
	//	QTimer::singleShot(0, this, SLOT(requestNewFortune()));
	//	return;
	//}

	//currentFortune = nextFortune;
	//statusLabel->setText(currentFortune);
	//getFortuneButton->setEnabled(true);
}

//void TCPNetworkServer::setExampleProperty( short sExampleProperty )
//{
//	m_ExampleProperty = sExampleProperty;
//	emit ExampleSignalTriggered(m_ExampleProperty);//Signal that the value has been changed
//}

//short TCPNetworkServer::getExampleProperty() const
//{
//	return m_ExampleProperty;
//}
