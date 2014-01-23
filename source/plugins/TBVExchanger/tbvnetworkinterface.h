//Copyright (C) 2014  Michael Luehrs
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
	//!  tGetCurrentTimePoint slot. 
	/*!
	  Provides the number of the currently processed step during real-time processing as an integer. Note that this function is 1-based, i.e. when the first step is processed the function returns "1" not "0"; this is important when the return value is used to access time-related information; in this case subtract "1" from the returned value.
	*/
	int tGetCurrentTimePoint();
	//!  tGetExpectedNrOfTimePoints slot. 
	/*!
	  Provides the number of time points as an integer. The name contains the term "expected" since a real-time run might be interrupted by the user, i.e. this is the intended number of volumes as specified in the TBV settings file.
	*/
	int tGetExpectedNrOfTimePoints();
	//!  tGetDimsOfFunctionalData slot. 
	/*!
	  Provides the dimensions of the functional volumes; "dim_x" and "dim_y" are the dimensions of the slices constituting the volume and "dim_z" corresponds to the number of slices.
	*/
	QList<int> tGetDimsOfFunctionalData();
	//!  tGetProjectName slot. 
	/*!
	  Provides the name of the project as specified in the TBV file as a C string; note that the received data must point to a pre-allocated array that is large enough (a buffer of 100 bytes should be sufficient). The returned name can, for example, be used as part of names identifying exported data (see example "Export Volume Data" client).
	*/
	QString tGetProjectName();
	//!  tGetWatchFolder slot. 
	/*!
	  Provides the path of the "watch folder" as specified in the TBV file as a C string; Note that the received data must point to a pre-allocated array that is large enough for the returned path (a buffer of 513 bytes is recommended).
	*/
	QString tGetWatchFolder();
	//!  tGetTargetFolder slot. 
	/*!
	  Provides the path of the "target folder" as specified in the TBV file as a C string; note that the received data must point to a pre-allocated array that is large enough for the returned path (a buffer of 513 bytes is recommended). The target folder can be used to export data for custom processing (see example "Export Volume Data" client).
	*/
	QString tGetTargetFolder();
	//!  tGetFeedbackFolder slot. 
	/*!
	  Provides the path of the "feedback folder" as specified in the TBV file as a C string; note that the provided data must point to a pre-allocated array that is large enough for the received path (a buffer of 513 bytes is recommended). The feedback folder can be used to store the result of custom calculations, e.g. providing custom input for the "Presenter" software tool.
	*/
	QString tGetFeedbackFolder();


	//Protocol, DM, GLM Functions:
	//!  tGetCurrentProtocolCondition slot. 
	/*!
	  Provides the index of the currently "active" condition of the protocol (0-based), i.e. the condition that has a defined interval enclosing the current time point.
	*/
	int tGetCurrentProtocolCondition();
	//!  tGetFullNrOfPredictors slot. 
	/*!
	  Provides the number of predictors of the design matrix. Note that this query returns the "full" number of intended predictors while the "tGetCurrentNrOfPredictors" returns the number of predictors currently in use.
	*/
	int tGetFullNrOfPredictors();
	//!  tGetCurrentNrOfPredictors slot. 
	/*!
	  Provides the currently effective number of predictors. Note that this query may return a smaller number than the "tGetFullNrOfPredictors" query since the internal GLM calculations use a restricted set of predictors in case that for one or more predictors not enough non-zero data points are available. Roughly speaking, the number of current predictors increases each time when a new condition is encountered during real-time processing.
	*/
	int tGetCurrentNrOfPredictors();
	//!  tGetNrOfConfoundPredictors slot. 
	/*!
	  Provides the full number of confound predictors. To get the full/effective number of predictors-of-interest, subtract the returned value from the "tGetFullNrOfPredictors" or "tGetCurrentNrOfPredictors" function, respectively.
	*/
	int tGetNrOfConfoundPredictors();
	//!  tGetValueOfDesignMatrix slot. 
	/*!
	  Provides the value of a predictor at a given time point of the current design matrix. Note that the design matrix always contains the "full" set of predictors, a reduced set of predictors is only used internally (predictors that are not used internally are those containing only "0.0" entries up to the current time point). Note that the "timepoint" parameter must be smaller than the value returned by the "tGetCurrentTimePoint" query. For details, see the provided example clients.
	*/
	float tGetValueOfDesignMatrix(int pred, int timepoint);
	//!  tGetNrOfContrasts slot. 
	/*!
	  Provides the number of (automatically or manually) specified contrasts in the TBV settings file. This value is important for accessing t maps, see the "tGetMapValueOfVoxel" and "tGetContrastMaps" queries.
	*/
	int tGetNrOfContrasts();


	//ROI Functions
	//!  tGetNrOfROIs slot. 
	/*!
	  Provides the number of currently available ROIs. Note that the number of ROIs may change during real-time processing since the user may open additional ROI windows or close ROI windows at any time. It is thus important to use this function prior to other functions accessing ROI information.
	*/
	int tGetNrOfROIs();
	//!  tGetMeanOfROI slot. 
	/*!
	  Returns the mean signal value of the ROI referenced with the "roi" parameter (0-based index). A valid number must be smaller than the value returned by the "tGetNrOfROIs" query. Note that the voxels defining a ROI might change in case that the user selects another region for the same ROI index (replaces the content of the same plot in the GUI). The query should be used in situations when ROIs are not changed, i.e. when a set of ROIs is pre-loaded for a neurofeedback study. For details, see the "ROI Processing" example client.
	*/
	float tGetMeanOfROI(int roi);
	//!  tGetNrOfVoxelsOfROI slot. 
	/*!
	  Provides the number of voxels of the specified ROI. Note that the returned number might change during real-time processing in case that the user replaces a ROI with another set of voxels. The value of this query is important for accessing information of individual ROI voxels.
	*/
	int tGetNrOfVoxelsOfROI(int roi);
	//!  tGetBetaOfROI slot. 
	/*!
	  Retrieves the value of a specified beta (0-based index) of the specified ROI (0-based index). For each ROI a GLM is calculated incrementally using the mean signal time course; the betas of the calculated GLM are accessible with this query; note that the beta indices range from 0 to the full number of predictors; to retrieve only the betas of the predictors of interest, the beta index must be smaller than "tGetFullNrOfPredictors" minus "tGetNrOfConfoundPredictors". For details, see the "ROI Processing" example client.
	*/
	float tGetBetaOfROI(int roi,int beta);
	//!  tGetCoordsOfVoxelOfROI slot. 
	/*!
	  Provides the coordinates of a voxel (0-based enumeration index) of the ROI specified with the "roi" parameter (0-based index); the value for the "voxel" index ranges from "0" to one less than the value returned by the "tGetNrOfVoxelsOfROI" query; since ROIs content may change, use the latter function for a specific ROI index always before using the current function. For details, see the "ROI Processing" example client.
	*/
	QList<int> tGetCoordsOfVoxelOfROI(int roi, int voxel);
	//!  tGetAllCoordsOfVoxelsOfROI slot. 
	/*!
	  Provides the coordinates of all voxels of the ROI specified with the "roi" parameter (0-based index); since ROIs content may change, use the latter function for a specific ROI index always before using the current function. For details, see the "ROI Processing" example plugin. If a coordinate of a specific voxel of a roi needs to be accessed, use the term "x_coord = voxel_roi+0; y_coord = voxel_roi+1; z_coord = voxel_roi+2". For details, see the "ROI Processing" example client.
	*/
	QList<int> tGetAllCoordsOfVoxelsOfROI(int roi);


	//Volume Data Access Functions
	//!  tGetValueOfVoxelAtTime slot. 
	/*!
	  Provides the signal value as a 4-byte float value of the voxel specified by the coordinate parameters "x", "y" and "z" for the given time point (0-based indices). The given "timepoint" parameter must be smaller than the value obtained by the "tGetCurrentTimePoint" query.
	*/
	float tGetValueOfVoxelAtTime(int x, int y, int z, int timepoint);
	//!  tGetTimeCourseData slot. 
	/*!
	  Provides the full time course data to a given time point that is also used internally in TBV. Individual values are 2-byte short integers. Note that the "timepoint" parameter must be smaller than the value returned by the "tGetCurrentTimePoint()" function. If a voxel with specific coordinates needs to be accessed, use the term "z_coord*dim_x*dim_y + y_coord*dim_x + x_coord". For details, see the provided example clients.
	*/
	QList<short> tGetTimeCourseData(int timepoint);
	//!  tGetRawTimeCourseData slot. 
	/*!
	  Provides the full raw (not processed) time course data to a given time point. Individual values are 2-byte short integers. Note that the "timepoint" parameter must be smaller than the value returned by the "tGetCurrentTimePoint()" function. If a voxel with specific coordinates needs to be accessed, use the term "z_coord*dim_x*dim_y + y_coord*dim_x + x_coord". For details, see the provided example clients.
	*/
	QList<short> tGetRawTimeCourseData(int timepoint);
	//!  tGetBetaOfVoxel slot. 
	/*!
	  Provides the value of a beta indexed by the "beta" parameter as an 8-byte double value for the voxel specified by the coordinate parameters "x", "y" and "z" (0-based indices). This function allows to access estimated beta values resulting from the incremental GLM performed by TBV. Note that the beta index ranges from 0 to the current number of predictors; to retrieve only the betas of the predictors of interest, the beta index must be smaller than "tGetCurrentNrOfPredictors" minus "tGetNrOfConfoundPredictors". For details, see the "Export Volume Data" example client.
	*/
	double tGetBetaOfVoxel(int beta, int x, int y, int z);
	//!  tGetBetaMaps slot. 
	/*!
	  Provides the full stack of beta maps that is also used internally in TBV. Individual entries are high-precision 8-byte double values. The data is organized as a flat array; in order to obtain the beta value of a specific predictor index for a voxel with specific coordinates, use the term "beta_i*dim_xyz + z_coord*dim_xy + y_coord*dim_x + x_coord". Note that the beta_i index must be in the ranges from 0 to the current number of predictors; to retrieve only the betas of the predictors of interest, the beta index must be smaller than tGetCurrentNrOfPredictors" minus "tGetNrOfConfoundPredictors". For details, see the provided "Export Volume Data" client.
	*/
	QList<double> tGetBetaMaps();
	//!  tGetMapValueOfVoxel slot. 
	/*!
	  Provides the value of a t map indexed by the "map" parameter as a 4-byte float value for the voxel specified by the coordinate parameters "x", "y" and "z" (0-based indices). This function allows to access calculated contrast values that are calculated based on the beta values from the incremental GLM performed by TBV. The "map" index ranges from 0 to one less than the number of contrasts specified in the TBV settings file (implicitly or via a specified contrast ".ctr" file); the number of contrasts can be retrieved using the "tGetNrOfContrasts" query. For details, see the "Export Volume Data" example client.
	*/
	float tGetMapValueOfVoxel(int map, int x, int y, int z);
	//!  tGetContrastMaps slot. 
	/*!
	  Provides the full stack of contrast maps that is also used internally in TBV. Individual entries are 4-byte float values. The data is organized as a flat array; in order to obtain the t value of a specific contrast map index for a voxel with specific coordinates, use the term "map_i*dim_xyz + z_coord*dim_xy + y_coord*dim_x + x_coord". The "map_i" index ranges from 0 to one less than the number of contrasts specified in the TBV settings file (implicitly or via a specified contrast ".ctr" file); the number of contrasts can be retrieved using the "tGetNrOfContrasts" query. For details, see the provided "Export Volume Data" client.
	*/
	QList<float> tGetContrastMaps();

	//Connection functions
	//!  connectToServer slot. 
	/*!
	  Connect to the server using ip and port.
	*/
	bool connectToServer(char *ipAddress,quint16 port);
	//!  disconnectFromServer slot. 
	/*!
	  Disconnect from the server.
	*/
	bool disconnectFromServer();
	//!  setAutoConnection slot. 
	/*!
	  Sets the automatic connect functionality to connect to a TBV server if available and accessible.
	*/
	bool setAutoConnection(bool bActivate);

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
	char *iIpAddress;
	quint16 iPort;

signals:
	//!  executePreStep signal. 
	/*!
	  This signal will be called whenever new data is available in the TBV.
	*/
	void executePreStep();
	//!  executePostStep signal. 
	/*!
	  This signal will be called whenever new data was processed in the TBV.
	*/
	void executePostStep();
	//!  executePostRun signal. 
	/*!
	  This signal will be called when the TBV finished processing the experiment run.
	*/
	void executePostRun();
	//!  disconnected signal.
	/*!
	  This signal will be called when the connection is interrupted.
	*/
	void disconnected();
	//!  connected signal.
	/*!
	  This signal will be called when a connection to the TBV was established.
	*/
	void connected();
	//!  connectionError signal. 
	/*!
	  This signal will be called when a connection error occurs. The error is described in the received QString.
	*/
	void connectionError(QString error);

private slots:
	void readExecuteStep();
	void connectionEstablished();
	void connectionLost();
	void writeError(QAbstractSocket::SocketError Error);
	void processPendingDatagrams();
};


