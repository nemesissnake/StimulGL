//TBVExchanger
//Copyright (C) 2012  Sven Gijsen
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

#ifndef TBVExchanger_H
#define TBVExchanger_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include "./../../StimulGL/mainappinfo.h"
#include "tbvnetworkinterface.h"

template <typename T,unsigned S>
inline unsigned arraysize(const T (&v)[S]) { return S; }

class TBVExchanger : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "TBVExchanger");//Can't use defines here!, moc doesn't handle defines, not needed here

signals:
	void executePreStep();
	void executePostStep();
	void executePostRun();
	void disconnected();
	void connected();
	void connectionError(QString error);

public:
	TBVExchanger(QObject *parent = 0);
	~TBVExchanger();
	TBVExchanger(const TBVExchanger& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	//Basic Project Functions
	int tGetCurrentTimePoint() {return tbvNetwIntFace->tGetCurrentTimePoint();};
	int tGetExpectedNrOfTimePoints() {return tbvNetwIntFace->tGetExpectedNrOfTimePoints();};
	QList<int> tGetDimsOfFunctionalData() {return tbvNetwIntFace->tGetDimsOfFunctionalData();};
	QString tGetProjectName() {return tbvNetwIntFace->tGetProjectName();};
	QString tGetWatchFolder() {return tbvNetwIntFace->tGetWatchFolder();};
	QString tGetTargetFolder() {return tbvNetwIntFace->tGetTargetFolder();};
	QString tGetFeedbackFolder() {return tbvNetwIntFace->tGetFeedbackFolder();};
	//Protocol, DM, GLM Functions:
	int tGetCurrentProtocolCondition() {return tbvNetwIntFace->tGetCurrentProtocolCondition();};
	int tGetFullNrOfPredictors() {return tbvNetwIntFace->tGetFullNrOfPredictors();};
	int tGetCurrentNrOfPredictors() {return tbvNetwIntFace->tGetCurrentNrOfPredictors();};
	int tGetNrOfConfoundPredictors() {return tbvNetwIntFace->tGetNrOfConfoundPredictors();};
	float tGetValueOfDesignMatrix(int pred, int timepoint) {return tbvNetwIntFace->tGetValueOfDesignMatrix(pred, timepoint);};
	int tGetNrOfContrasts() {return tbvNetwIntFace->tGetNrOfContrasts();};
	//ROI Functions
	int tGetNrOfROIs() {return tbvNetwIntFace->tGetNrOfROIs();};
	float tGetMeanOfROI(int roi) {return tbvNetwIntFace->tGetMeanOfROI(roi);};
	int tGetNrOfVoxelsOfROI(int roi) {return tbvNetwIntFace->tGetNrOfVoxelsOfROI(roi);};
	float tGetBetaOfROI(int roi,int beta) {return tbvNetwIntFace->tGetBetaOfROI(roi,beta);};
	QList<int> tGetCoordsOfVoxelOfROI(int roi, int voxel) {return tbvNetwIntFace->tGetCoordsOfVoxelOfROI(roi, voxel);};
	QList<int> tGetAllCoordsOfVoxelsOfROI(int roi) {return tbvNetwIntFace->tGetAllCoordsOfVoxelsOfROI(roi);};
	//Volume Data Access Functions
	float tGetValueOfVoxelAtTime(int x, int y, int z, int timepoint) {return tbvNetwIntFace->tGetValueOfVoxelAtTime(x, y, z, timepoint);};
	
	QList<short> tGetTimeCourseData(int timepoint) {return tbvNetwIntFace->tGetTimeCourseData(timepoint);};
	QList<short> tGetRawTimeCourseData(int timepoint) {return tbvNetwIntFace->tGetRawTimeCourseData(timepoint);};
	double tGetBetaOfVoxel(int beta, int x, int y, int z) {return tbvNetwIntFace->tGetBetaOfVoxel(beta, x, y, z);};
	QList<double> tGetBetaMaps() {return tbvNetwIntFace->tGetBetaMaps();};
	float tGetMapValueOfVoxel(int map, int x, int y, int z) {return tbvNetwIntFace->tGetMapValueOfVoxel(map, x, y, z);};
	QList<float> tGetContrastMaps() {return tbvNetwIntFace->tGetContrastMaps();};
	bool connectToServer(QString sIPAddress,int port) {return tbvNetwIntFace->connectToServer(sIPAddress.toLatin1().data(),port);};
	bool disconnectFromServer() {return tbvNetwIntFace->disconnectFromServer();};

private:
	QScriptEngine* currentScriptEngine;
	short m_ExampleProperty;
	TBVNetworkInterface *tbvNetwIntFace;
};

#endif // TBVExchanger_H
