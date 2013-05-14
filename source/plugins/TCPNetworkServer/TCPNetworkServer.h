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
#include "./../../StimulGL/mainappinfo.h"

class TCPNetworkServer : public QTcpServer, protected QScriptable//QObject
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "TCPNetworkServer");//Can't use defines here!, moc doesn't handle defines, not needed here
	//Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	//void ExampleSignalTriggered(short);

public:
	TCPNetworkServer(QObject *parent = 0);
	~TCPNetworkServer();
	TCPNetworkServer(const TCPNetworkServer& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	//void setExampleProperty( short sExampleProperty );
	//short getExampleProperty() const;
	QString startListening(QString a, QString b);
	void newIncomingServerConnection();

	bool connectToServer(QString a, QString b);
	void serverDataAvailable();


private:
	QScriptEngine* currentScriptEngine;
	QTcpSocket* tcpSocket;
	quint16 blockSize;
	//short m_ExampleProperty;

};

#endif // TCPNetworkServer_H
