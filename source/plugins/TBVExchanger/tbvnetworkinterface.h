//Copyright (C) 2014  Michael Luehrs, Brain Innovation B.V.

#include <QDataStream>
#include <QtNetwork>
#include "tbv_server_request.h"

class TBVNetworkInterface : public QThread
{
	Q_OBJECT

public:
	TBVNetworkInterface(bool autoConnect = false, bool autoReconnect = true);
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
	float *tGetExistingMeansOfROI(int roi, int toTimePoint);
	float tGetMeanOfROIAtTimePoint(int roi, int timepoint);
	int tGetNrOfVoxelsOfROI(int roi);
	float tGetBetaOfROI(int roi,int beta);
	QList<int> tGetCoordsOfVoxelOfROI(int roi, int voxel);
	QList<int> tGetAllCoordsOfVoxelsOfROI(int roi);

	//Volume Data Access Functions
	float tGetValueOfVoxelAtTime(int x, int y, int z, int timepoint);
	QList<short> tGetTimeCourseData(int timepoint);
	QList<short> tGetRawTimeCourseData(int timepoint);
	double tGetBetaOfVoxel(int beta, int x, int y, int z);
	QList<double> tGetBetaMaps();
	float tGetMapValueOfVoxel(int map, int x, int y, int z);
	QList<float> tGetContrastMaps();

	//SVM Access Functions
	int tGetNumberOfClasses();
	QList<float> tGetCurrentClassifierOutput();

	bool connectToServer(char *ipAddress,qint64 port);
	bool disconnectFromServer();

	bool isConnected();

	bool setAutoConnection(bool setting);

private:
	quint16 nDefaultPort;
	QUdpSocket *udpSocket;
	QTcpSocket *tcpSocket;
	QTcpSocket *rTcpSocket;
	qint64 blockSize;
	QQueue<QString> *querryQueue;
	QVector<short int> TimeCourseData;
	QVector<short int> RawTimeCourseData;
	QVector<float> AllMeansOfROI;
	QVector<double> BetaMaps;
	QVector<float> ContrastMaps;
	QVector<int> CoordsOfVoxelsOfROI;
	QVector<float> ClassifierOutput;
	bool sendStreamDefinition(char *definition,QTcpSocket *dSocket);
	bool udpAutoConnect, reConnect, terminateReconnect, terminateReciever, manualDisconnect;
	QString ipAddressString;
	qint64 iPort;
	void tryToReconnect();

signals:
	void executePreStep(int timepoint);
	void executePostStep(int timepoint);
	void executePostRun(int timepoint);
	void disconnected();
	void connected(QString iPAddress, qint64 port);
	void connectionError(QString error);
	void connecting();

private slots:
	void readExecuteStep();
	void connectionEstablished();
	void connectionLost();
	void writeError(QAbstractSocket::SocketError Error);
	void processPendingDatagrams();
};


