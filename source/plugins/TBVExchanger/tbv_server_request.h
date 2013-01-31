#include <QtNetwork>
#include <iostream>
#include <QQueue>
#include <QThread>
#include <QDataStream>

class TBV_Server_Request
{

public:

	TBV_Server_Request(QTcpSocket *tcpSocket, QQueue<QString> *querryQueue, QString querry);
	~TBV_Server_Request();

	bool sendData(char *Querry,int value0 = -1, int value1 = -1, int value2 = -1, int value3 = -1);
	QString getReply();
	template< typename T> void getDataOfByteArray(T &value);
	void getDataOfByteArray(char *data,int size);

private:
	QByteArray rcvData;
	QDataStream rcvStream;
	int nrOfValues;
	QTcpSocket *eTcpSocket;
	bool sendQByteArray(QByteArray dataToSend);
	qint64 blockSize;
	QQueue<QString> *querryQueueinternal;
	QString iQuerry;

};

template< typename T> void TBV_Server_Request::getDataOfByteArray(T &value)
{
	rcvStream >> value;
}


