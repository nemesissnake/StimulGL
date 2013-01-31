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
	void tGetDimsOfFunctionalData(int &dim_x, int &dim_y, int &dim_z);
	void tGetProjectName(char *cProjectName);
	void tGetWatchFolder(char *cWatchFolder);
	void tGetTargetFolder(char *cTargetFolder);
	void tGetFeedbackFolder(char *cFeedbackFolder);

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
	bool tGetCoordsOfVoxelOfROI(int roi, int voxel, int &x, int &y, int &z);
	int *tGetAllCoordsOfVoxelsOfROI(int roi);

	//Volume Data Access Functions
	float tGetValueOfVoxelAtTime(int x, int y, int z, int timepoint);
	short int *tGetTimeCourseData(int timepoint);
	short int *tGetRawTimeCourseData(int timepoint);
	double tGetBetaOfVoxel(int beta, int x, int y, int z);
	double *tGetBetaMaps();
	float tGetMapValueOfVoxel(int map, int x, int y, int z);
	float *tGetContrastMaps();

	bool connectToServer(char *ipAddress,qint64 port);
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


