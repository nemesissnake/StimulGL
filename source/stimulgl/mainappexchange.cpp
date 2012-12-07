#include <QLocalSocket>
#include "mainappexchange.h"

MainAppExchange::MainAppExchange(int &argc, char *argv[], const QString uniqueKey) : QApplication(argc, argv), _uniqueKey(uniqueKey)
{
	sharedMemory.setKey(_uniqueKey);
	if (sharedMemory.attach())
		_isRunning = true;
	else
	{
		_isRunning = false;
		// create shared memory.
		if (!sharedMemory.create(1))
		{
			qDebug() << __FUNCTION__ << "Unable to create MainAppExchange";
			return;
		}
		// create local server and listen to incoming messages from other instances.
		localServer = new QLocalServer(this);
		connect(localServer, SIGNAL(newConnection()), this, SLOT(receiveMessage()));
		localServer->listen(_uniqueKey);
	}
}

bool MainAppExchange::isRunning()
{
	return _isRunning;
}

void MainAppExchange::receiveMessage()
{
	QLocalSocket *localSocket = localServer->nextPendingConnection();
	if (!localSocket->waitForReadyRead(timeout))
	{
		qDebug(localSocket->errorString().toLatin1());
		return;
	}
	QByteArray byteArray = localSocket->readAll();
	QString message = QString::fromUtf8(byteArray.constData());
	emit messageAvailable(message);
	localSocket->disconnectFromServer();
}

bool MainAppExchange::sendMessage(const QString &message)
{
	if (!_isRunning)
		return false;
	QLocalSocket localSocket(this);
	localSocket.connectToServer(_uniqueKey, QIODevice::WriteOnly);
	if (!localSocket.waitForConnected(timeout))
	{
		qDebug() << __FUNCTION__ << localSocket.errorString().toLatin1();
		return false;
	}
	localSocket.write(message.toUtf8());
	if (!localSocket.waitForBytesWritten(timeout))
	{
		qDebug() << __FUNCTION__ << localSocket.errorString().toLatin1();
		return false;
	}
	localSocket.disconnectFromServer();
	return true;
}

