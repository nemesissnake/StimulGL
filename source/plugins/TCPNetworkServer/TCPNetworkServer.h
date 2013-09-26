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


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef TCPNetworkServer_H
#define TCPNetworkServer_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "./../../StimulGL/mainappinfo.h"

//!  The TCPNetworkServer class. 
/*!
  The TCPNetworkServer provides the functionality to either start an server to listen for and handle incoming connections from a client,
  or it can be used to act as an client and connect to a server to communicate with. You should not combine both functionalities within the same instance!
*/
class TCPNetworkServer : public QTcpServer, protected QScriptable//QObject
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "TCPNetworkServer");//Can't use defines here!, moc doesn't handle defines, not needed here

signals:
	//////////////////////////////////////////
	//Available Slots when using as a Server//
	//////////////////////////////////////////	
	//! \brief ClientDataAvailable signal (for Server usage).
	/*!  Use this signal to detect whenever data from a client is received for further handling of the data by the server.
	 * @param sData the data received from a client as a string value.
	 */
	void ClientDataAvailable(QString sData);

	//////////////////////////////////////////
	//Available Slots when using as a Client//
	//////////////////////////////////////////	
	//! \brief ServerDataAvailable signal (for Client usage).
	/*!  Use this signal to detect whenever data from the server is received for further handling of the data by the client.
	 * @param sData the data received from the server as a string value.
	 */
	void ServerDataAvailable(QString sData);

public:
	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);
	TCPNetworkServer(QObject *parent = 0);
	~TCPNetworkServer();
	TCPNetworkServer(const TCPNetworkServer& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	
public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	
	//////////////////////////////////////////
	//Available Slots when using as a Server//
	//////////////////////////////////////////	
	//! \brief startServer slot (for Server usage).
	/*!  This function Starts a TCP/IP network server listening on a defined Address and Port.
	 * @param sAddress the IP address it should use to start the server, if it is "" then it'll automatically try to use the local hosts IPv4 Address.
	 * @param port the port number the server should try to use (listening for new incoming connections).
	 * @return a String value if the server could be successfully started then this function returns the <IP-address>:<Port> (e.g. 187.110.136.170:241) it is listening on for new connections.
	 * If the server could not be started it returns the Error message.
	 */
	QString startServer(const QString &sAddress = "", int port = 0);
	//! \brief sendServerData slot (for Server usage).
	/*!  This function sends a data String to the connected client(s).
	 *   Only make use of this function after a server is started, see TCPNetworkServer::startServer.
	 * @param sData a String of data to send.
	 * @return a integer value holding the resulting number of bytes successfully send.
	 */
	int sendServerData(QString sData);

	//////////////////////////////////////////
	//Available Slots when using as a Server//
	//////////////////////////////////////////
	//! \brief connectToServer slot (for Client usage).
	/*!  This function tries to connect to an TCP/IP network server listening on a defined Address and Port.
	 * @param sAddress the IP address of the server to connect to.
	 * @param port the port number of the server to connect to.
	 * @return a boolean value determing whether the connection could be established.
	 */
	bool connectToServer(const QString &sAddress, int port);
	//! \brief sendClientData slot (for Client usage).
	/*!  This function sends a data String to the connected server.
	 *   Only make use of this function after a connection to a server is established, see TCPNetworkServer::connectToServer.
	 * @param sData a String of data to send.
	 * @return a integer value holding the resulting number of bytes successfully send.
	 */
	int sendClientData(QString sData);
	
private slots:
	void newIncomingConnectionFromClient();
	void clientDisconnected();
	void clientErrorNetworkData(QAbstractSocket::SocketError socketError);
	void serverDisconnected();
	void serverErrorNetworkData(QAbstractSocket::SocketError socketError);
	void dataFromClientAvailable();
	void dataFromServerAvailable();
	
private:
	QScriptEngine* currentScriptEngine;
	QTcpSocket* clientConnectionSocket;
	QList<QTcpSocket*> serverClientConnectionSockets;
	quint16 blockSize;
};

#endif // TCPNetworkServer_H
