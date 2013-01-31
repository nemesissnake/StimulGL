#include "tbvnetworkinterface.h"


TBVNetworkInterface::TBVNetworkInterface() : QThread()
{
	tcpSocket = new QTcpSocket();
	rTcpSocket = new QTcpSocket();
	blockSize = 0;
	connect(rTcpSocket,SIGNAL(readyRead()),this,SLOT(readExecuteStep()));
	connect(rTcpSocket,SIGNAL(connected()),this,SLOT(connectionEstablished()));
	connect(rTcpSocket,SIGNAL(disconnected()),this,SLOT(connectionLost()));
	connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(writeError(QAbstractSocket::SocketError)));

	querryQueue = new QQueue<QString>;

	udpSocket = new QUdpSocket();
    udpSocket->bind(55555, QUdpSocket::ShareAddress);

	connect(udpSocket, SIGNAL(readyRead()),this, SLOT(processPendingDatagrams()));

}

TBVNetworkInterface::~TBVNetworkInterface()
{
	
}

void TBVNetworkInterface::processPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
		QHostAddress *tbvServer = new QHostAddress();
		quint16 *tbvPort = new quint16();
		udpSocket->readDatagram(datagram.data(),datagram.size(),tbvServer,tbvPort);

		if(tr(datagram.data()).contains(tr("Turbo-BrainVoyager Broadcast")))
			connectToServer(tbvServer->toString().toLatin1().data(),55555);
    }
}

void TBVNetworkInterface::connectionEstablished()
{
	udpSocket->close();
	emit connected();
}

void TBVNetworkInterface::connectionLost()
{
	emit disconnected();
    udpSocket->bind(55555, QUdpSocket::ShareAddress);
}

bool TBVNetworkInterface::connectToServer(char *ipAddress,qint64 port)
{
	tcpSocket->connectToHost(tr(ipAddress),port);
	if(!tcpSocket->waitForConnected(3000))
		return false;
	sendStreamDefinition("Request Socket",tcpSocket);

	msleep(500);
	rTcpSocket->connectToHost(tr(ipAddress),port);
	if(!rTcpSocket->waitForConnected(3000))
	{
		tcpSocket->close();
		return false;
	}
	sendStreamDefinition("Execute Socket",rTcpSocket);
	
	return true;
}


bool TBVNetworkInterface::sendStreamDefinition(char *definition,QTcpSocket *dSocket)
{
	QByteArray definit;
	QDataStream out(&definit, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_0);

	out << (qint64)0;
	out << definition;	
	out.device()->seek(0);
	out << (qint64)(definit.size() - sizeof(qint64));

	if(dSocket->write(definit) == 0)
	{
		if(dSocket->state() != QAbstractSocket::ConnectedState)
			return false;
	}
	
	if(!dSocket->waitForBytesWritten())
	{
		return false;
	}

	
	return true;

}

bool TBVNetworkInterface::disconnectFromServer()
{
	tcpSocket->disconnectFromHost();
	return tcpSocket->waitForDisconnected();
}


void TBVNetworkInterface::readExecuteStep()
{
	while(true)
	{
		QDataStream in(rTcpSocket);
		in.setVersion(QDataStream::Qt_4_0);
		//get size of data to receive
		if (blockSize == 0) {
			if (rTcpSocket->bytesAvailable() < (int)sizeof(qint64))
			{
				return;
			}
			in >> blockSize;
		}
		//ensure that enough data is on the stream
		if ((qint64)rTcpSocket->bytesAvailable() < blockSize)
		{
			return;
		}

		blockSize = 0;
		char *rcvChar;
		in >> rcvChar;

		if(!rcvChar)
		{
			while (!rTcpSocket->atEnd()) 
			{
				QByteArray data = rTcpSocket->read(100);
			}
		}
		else
		if(strcmp(rcvChar , "PreStepCalled") == 0)
		{
			emit executePreStep();
		}
		else
		if(strcmp(rcvChar , "PostStepCalled") == 0)
		{
			emit executePostStep();	
		}
		else
		if(strcmp(rcvChar , "PostRunCalled") == 0)
		{
			emit executePostRun();
		}
		else
		while (!rTcpSocket->atEnd()) 
		{
			  QByteArray data = tcpSocket->read(100);
		}
	}
}

//Basic TBV Project Functions
int TBVNetworkInterface::tGetCurrentTimePoint()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetCurrentTimePoint"));
	doRequest.sendData("tGetCurrentTimePoint");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	int CurrentTimePoint;
	doRequest.getDataOfByteArray(CurrentTimePoint);
	return CurrentTimePoint;
}
int	TBVNetworkInterface::tGetExpectedNrOfTimePoints()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetExpectedNrOfTimePoints"));
	doRequest.sendData("tGetExpectedNrOfTimePoints");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return 0;
	}
	int ExpectedNrOfTimePoints;
	doRequest.getDataOfByteArray(ExpectedNrOfTimePoints);
	return ExpectedNrOfTimePoints;
}
void TBVNetworkInterface::tGetDimsOfFunctionalData(int &dim_x, int &dim_y, int &dim_z)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetDimsOfFunctionalData"));
	doRequest.sendData("tGetDimsOfFunctionalData");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return;
	}
	doRequest.getDataOfByteArray(dim_x);
	doRequest.getDataOfByteArray(dim_y);
	doRequest.getDataOfByteArray(dim_z);
}
void TBVNetworkInterface::tGetProjectName(char *cProjectName)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetProjectName"));
	doRequest.sendData("tGetProjectName");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return;
	}
	char *ProjectName;
	doRequest.getDataOfByteArray(ProjectName);
	strcpy(cProjectName,ProjectName);
}
void TBVNetworkInterface::tGetWatchFolder(char *cWatchFolder)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetWatchFolder"));
	doRequest.sendData("tGetWatchFolder");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return;
	}
	char *WatchFolder;
	doRequest.getDataOfByteArray(WatchFolder);
	strcpy(cWatchFolder,WatchFolder);
}
void TBVNetworkInterface::tGetTargetFolder(char *cTargetFolder)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetTargetFolder"));
	doRequest.sendData("tGetTargetFolder");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return;
	}
	char *TargetFolder;
	doRequest.getDataOfByteArray(TargetFolder);
	strcpy(cTargetFolder,TargetFolder);
}
void TBVNetworkInterface::tGetFeedbackFolder(char *cFeedbackFolder)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetFeedbackFolder"));
	doRequest.sendData("tGetFeedbackFolder");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return;
	}
	char *FeedbackFolder;
	doRequest.getDataOfByteArray(FeedbackFolder);
	strcpy(cFeedbackFolder,FeedbackFolder);
}

//TBV Protocol, DM, GLM Functions:
int TBVNetworkInterface::tGetCurrentProtocolCondition()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetCurrentProtocolCondition"));
	doRequest.sendData("tGetCurrentProtocolCondition");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	int CurrentProtocolCondition;
	doRequest.getDataOfByteArray(CurrentProtocolCondition);
	return CurrentProtocolCondition;
}
int	TBVNetworkInterface::tGetFullNrOfPredictors()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetFullNrOfPredictors"));
	doRequest.sendData("tGetFullNrOfPredictors");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return 0;
	}
	int FullNrOfPredictors;
	doRequest.getDataOfByteArray(FullNrOfPredictors);
	return FullNrOfPredictors;
}
int TBVNetworkInterface::tGetCurrentNrOfPredictors()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetCurrentNrOfPredictors"));
	doRequest.sendData("tGetCurrentNrOfPredictors");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	int CurrentNrOfPredictors;
	doRequest.getDataOfByteArray(CurrentNrOfPredictors);
	return CurrentNrOfPredictors;
}
int TBVNetworkInterface::tGetNrOfConfoundPredictors()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetNrOfConfoundPredictors"));
	doRequest.sendData("tGetNrOfConfoundPredictors");
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return 0;
	}
	int NrOfConfoundPredictors;
	doRequest.getDataOfByteArray(NrOfConfoundPredictors);
	return NrOfConfoundPredictors;
}
float TBVNetworkInterface::tGetValueOfDesignMatrix(int pred, int timepoint)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetValueOfDesignMatrix"));
	doRequest.sendData("tGetValueOfDesignMatrix", pred, timepoint);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	float ValueOfDesignMatrix;
	doRequest.getDataOfByteArray(ValueOfDesignMatrix);
	return ValueOfDesignMatrix;
}
int TBVNetworkInterface::tGetNrOfContrasts()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetNrOfContrasts"));
	doRequest.sendData("tGetNrOfContrasts");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	int NrOfContrasts;
	doRequest.getDataOfByteArray(NrOfContrasts);
	return NrOfContrasts;
}


//TBV ROI Functions
int TBVNetworkInterface::tGetNrOfROIs()
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetNrOfROIs"));
	doRequest.sendData("tGetNrOfROIs");	
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	int NrOfROIs;
	doRequest.getDataOfByteArray(NrOfROIs);
	return NrOfROIs;
}
float TBVNetworkInterface::tGetMeanOfROI(int roi)
{	
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetMeanOfROI"));
	doRequest.sendData("tGetMeanOfROI",roi);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	float MeanOfROI;
	doRequest.getDataOfByteArray(MeanOfROI);
	return MeanOfROI;
}
int	TBVNetworkInterface::tGetNrOfVoxelsOfROI(int roi)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetNrOfVoxelsOfROI"));
	doRequest.sendData("tGetNrOfVoxelsOfROI",roi);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	int NrOfVoxelsOfROI;
	doRequest.getDataOfByteArray(NrOfVoxelsOfROI);
	return NrOfVoxelsOfROI;
}
float TBVNetworkInterface::tGetBetaOfROI(int roi,int beta)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetBetaOfROI"));
	doRequest.sendData("tGetBetaOfROI",roi,beta);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	float BetaOfROI;
	doRequest.getDataOfByteArray(BetaOfROI);
	return BetaOfROI;

}
bool TBVNetworkInterface::tGetCoordsOfVoxelOfROI(int roi, int voxel, int &x, int &y, int &z)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetCoordsOfVoxelOfROI"));
	doRequest.sendData("tGetCoordsOfVoxelOfROI",roi,voxel);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return false;
	}
	doRequest.getDataOfByteArray(x);
	doRequest.getDataOfByteArray(y);
	doRequest.getDataOfByteArray(z);
	
	return true;
}
int *TBVNetworkInterface::tGetAllCoordsOfVoxelsOfROI(int roi)
{
	int NrOfVoxelsOfROI = tGetNrOfVoxelsOfROI(roi);

	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetAllCoordsOfVoxelsOfROI"));
	doRequest.sendData("tGetAllCoordsOfVoxelsOfROI",roi);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}

	
	CoordsOfVoxelsOfROI.resize(NrOfVoxelsOfROI*3);

	doRequest.getDataOfByteArray((char *)CoordsOfVoxelsOfROI.data(),NrOfVoxelsOfROI*3*sizeof(int));
	
	return CoordsOfVoxelsOfROI.data();
}


//TBV Volume Data Access Functions
float TBVNetworkInterface::tGetValueOfVoxelAtTime(int x, int y, int z, int timepoint)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetValueOfVoxelAtTime"));
	doRequest.sendData("tGetValueOfVoxelAtTime",x,y,z,timepoint);
	QString status = doRequest.getReply();

	if(!status.isEmpty())
	{
		return false;
	}
	float ValueOfVoxelAtTime;
	doRequest.getDataOfByteArray(ValueOfVoxelAtTime);
	return ValueOfVoxelAtTime;
}
short int *TBVNetworkInterface::tGetTimeCourseData(int timepoint)
{
	int dim_x,dim_y,dim_z,dim_xyz;
	tGetDimsOfFunctionalData(dim_x,dim_y,dim_z);
	dim_xyz = dim_x * dim_y * dim_z;
		
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetTimeCourseData"));
	doRequest.sendData("tGetTimeCourseData", timepoint);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	
	TimeCourseData.resize(dim_xyz);
	doRequest.getDataOfByteArray((char *)TimeCourseData.data(),dim_xyz*sizeof(short int));
	return TimeCourseData.data();
}
short int *TBVNetworkInterface::tGetRawTimeCourseData(int timepoint)
{
	int dim_x,dim_y,dim_z,dim_xyz;
	tGetDimsOfFunctionalData(dim_x,dim_y,dim_z);
	dim_xyz = dim_x * dim_y * dim_z;

	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetRawTimeCourseData"));
	doRequest.sendData("tGetRawTimeCourseData", timepoint);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	RawTimeCourseData.resize(dim_xyz);
	doRequest.getDataOfByteArray((char *)RawTimeCourseData.data(),dim_xyz*sizeof(short int));
	return RawTimeCourseData.data();
}
double TBVNetworkInterface::tGetBetaOfVoxel(int beta, int x, int y, int z)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetBetaOfVoxel"));
	doRequest.sendData("tGetBetaOfVoxel", beta, x, y, z);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	double BetaOfVoxel;
	doRequest.getDataOfByteArray(BetaOfVoxel);
	return BetaOfVoxel;
}
double *TBVNetworkInterface::tGetBetaMaps()
{
	int dim_x,dim_y,dim_z,dim_xyz;
	tGetDimsOfFunctionalData(dim_x,dim_y,dim_z);
	dim_xyz = dim_x * dim_y * dim_z;
	int n_predictors_current = tGetCurrentNrOfPredictors();
	
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetBetaMaps"));
	doRequest.sendData("tGetBetaMaps");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	BetaMaps.resize(n_predictors_current*dim_xyz);
	doRequest.getDataOfByteArray((char *)BetaMaps.data(), n_predictors_current*dim_xyz*8*sizeof(unsigned char));
	return BetaMaps.data();
}
float TBVNetworkInterface::tGetMapValueOfVoxel(int map, int x, int y, int z)
{
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetMapValueOfVoxel"));
	doRequest.sendData("tGetMapValueOfVoxel", map, x,y,z);
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	float MapValueOfVoxel;
	doRequest.getDataOfByteArray(MapValueOfVoxel);
	return MapValueOfVoxel;
}
float *TBVNetworkInterface::tGetContrastMaps()
{
	int dim_x,dim_y,dim_z,dim_xyz;
	tGetDimsOfFunctionalData(dim_x,dim_y,dim_z);
	dim_xyz = dim_x * dim_y * dim_z;
	int n_contrast_maps = tGetNrOfContrasts();
	
	TBV_Server_Request doRequest(tcpSocket,querryQueue,tr("tGetContrastMaps"));
	doRequest.sendData("tGetContrastMaps");
	QString status = doRequest.getReply();
	
	if(!status.isEmpty())
	{
		return 0;
	}
	ContrastMaps.resize(n_contrast_maps*dim_xyz*sizeof(float));
	doRequest.getDataOfByteArray((char *)ContrastMaps.data(), n_contrast_maps*dim_xyz*4*sizeof(unsigned char));
	return ContrastMaps.data();
}

void TBVNetworkInterface::writeError(QAbstractSocket::SocketError Error)
 {
     switch (Error) {
	 case QAbstractSocket::RemoteHostClosedError:
		 emit connectionError(tr("The remote host closed the connection.\n Please restart the remote host."));
		 break;
     case QAbstractSocket::HostNotFoundError:
         emit connectionError(tr("The host was not found. Please check the host name and port settings."));
         break;
     case QAbstractSocket::ConnectionRefusedError:
         emit connectionError(tr("The connection was refused by the peer. Make sure the server is running, and check that the host name and port settings are correct."));
         break;
     default:
         emit connectionError(tr("The following error occurred: %1.").arg(tcpSocket->errorString()));
     }

 }
