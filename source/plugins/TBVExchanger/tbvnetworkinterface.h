//Copyright (C) 2013  Michael Luers
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
#include <QDataStream>
class TBVNetworkInterface : public QThread
{
	Q_OBJECT

public:
	TBVNetworkInterface();
	~TBVNetworkInterface();

	//Basic Project Functions
	int tGetCurrentTimePoint();
	int tGetExpectedNrOfTimePoints();
    QList<int> tGetDimsOfFunctionalData();
    QString tGetProjectName();
    QString tGetWatchFolder();
    QString tGetTargetFolder();
    QString tGetFeedbackFolder();

	//Protocol, DM, GLM Functions:
	int tGetCurrentProtocolCondition();
    int tGetFullNrOfPredictors();
	int tGetCurrentNrOfPredictors();
	int tGetNrOfConfoundPredictors();
	float tGetValueOfDesignMatrix(int pred, int timepoint);
	int tGetNrOfContrasts();

	//ROI Functions
	int tGetNrOfROIs();
	float tGetMeanOfROI(int roi);
	int tGetNrOfVoxelsOfROI(int roi);
	float tGetBetaOfROI(int roi,int beta);
    QList<int> tGetCoordsOfVoxelOfROI(int roi, int voxel);
    QList<int> tGetAllCoordsOfVoxelsOfROI(int roi);

	//Volume Data Access Functions
	float tGetValueOfVoxelAtTime(int x, int y, int z, int timepoint);
    QList<short int> tGetTimeCourseData(int timepoint);
    QList<short int> tGetRawTimeCourseData(int timepoint);
    double tGetBetaOfVoxel(int beta, int x, int y, int z);
    QList<double> tGetBetaMaps();
	float tGetMapValueOfVoxel(int map, int x, int y, int z);
    QList<float> tGetContrastMaps();

    bool connectToServer(char *ipAddress,quint16 port);
	bool disconnectFromServer();

private:
	QUdpSocket *udpSocket;
	QTcpSocket *tcpSocket;
	QTcpSocket *rTcpSocket;
	qint64 blockSize;
	QQueue<QString> *querryQueue;
	QVector<short int> TimeCourseData;
	QVector<short int> RawTimeCourseData;
	QVector<double> BetaMaps;
	QVector<float> ContrastMaps;
	QVector<int> CoordsOfVoxelsOfROI;
	bool sendStreamDefinition(char *definition,QTcpSocket *dSocket);

signals:
	void executePreStep();
	void executePostStep();
	void executePostRun();
	void disconnected();
	void connected();
	void connectionError(QString error);

private slots:
	void readExecuteStep();
	void connectionEstablished();
	void connectionLost();
	void writeError(QAbstractSocket::SocketError Error);
	void processPendingDatagrams();
};


