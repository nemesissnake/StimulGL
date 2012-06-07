//ExperimentManagerplugin
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

#ifndef ExperimentManager_H
#define ExperimentManager_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QGraphicsView>
#include <QGLWidget>
#include <QDesktopWidget>
#include <QApplication>
#include "experimenttree.h"
#include "Global.h"
#include "retinomap_glwidget.h"
#include "qmlWidget.h"
#include "./../../StimulGL/mainappinfo.h"
#include "experimentlogger.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class RetinoMap_glwidget;
class qmlWidget;
class ExperimentTree;

//!  The Experiment Manager class. 
/*!
  The Experiment Manager can be used configure, open and execute experiment (*.exml) files.
*/
class ExperimentManager : public QObject, protected QScriptable
{
	Q_OBJECT

signals:
	//! The ExperimentStateHasChanged Signal.
	/*!
		You can use this Signal to keep track of the internal Experiment state changes.
		Parameter nExperimentMainState is type of the enum #ExperimentState.
	*/
	void ExperimentStateHasChanged(int nExperimentMainState, QString timeTextStamp);
	//! The WriteToLogOutput Signal.
	/*!
		You can use this signal to let the ExperimentManager write something to the StimulGL log window.
	*/
	void WriteToLogOutput(const QString &strText2Write);

public:
	//ExperimentManager(QObject *parent = NULL);
	ExperimentManager(QObject *parent = NULL, QScriptEngine* engine = NULL);
	~ExperimentManager();

	/*! The enum ExperimentState represents the main state of the ExperimentManager object */
	enum ExperimentState 
	{
		ExperimentManager_NoState		= 0, /*!< enum value 0 */
		ExperimentManager_Constructed	= 1, /*!< enum value 1 */
		ExperimentManager_Loaded		= 2, /*!< enum value 2 */
		ExperimentManager_Configured	= 3, /*!< enum value 3 */
		ExperimentManager_Initialized	= 4, /*!< enum value 4 */
		ExperimentManager_IsStarting	= 5, /*!< enum value 5 */
		ExperimentManager_Started		= 6, /*!< enum value 6 */
		ExperimentManager_IsStopping	= 7, /*!< enum value 7 */
		ExperimentManager_Stopped		= 8  /*!< enum value 8 */
	};

	typedef struct{
		int nObjectID;
		int nMetaID;
		QObject *pObject;
		QString sObjectName;
		ExperimentSubObjectState nCurrentState;
		tParsedParameterList *ExpBlockParams;
	} objectElement;
	
	static QScriptValue ctor__experimentManager(QScriptContext* context, QScriptEngine* engine);
	bool cleanupExperiment();
	tParsedParameterList *getObjectBlockParamListById(int nID);
	bool setExperimentObjectBlockParameterStructure(const int nObjectID, tParsedParameterList *expBlockTrialStruct);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool setExperimentFileName(const QString qstrExpFileName);
	QString getExperimentFileName();
	bool loadExperiment(QString strFile = "", bool bViewEditTree = true);
	bool saveExperiment(QString strFile = "");
	bool runExperiment();
	void abortExperiment();
	void stopExperiment();
	void changeExperimentSubObjectState(ExperimentSubObjectState nState);
	QString getCurrentDateTimeStamp();
	bool insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue, bool bIsInitializing = true);
	bool getExperimentObjectBlockParameter(const int nObjectID,const QString sName, ParsedParameterDefinition &strcParDef);
	bool logExperimentObjectData(const int &nObjectIndex, const int &nTimerIndex, const QString &strFunction = "", const QString &strTag = "", const QString &strMessage = "", const QString &strValue = "");
	int createExperimentTimer();
	bool startExperimentTimer(int nIndex);
	double restartExperimentTimer(int nIndex);
	double elapsedExperimentTimerTime(int nIndex);

private:
	void DefaultConstruct();
	bool WriteAndCloseExperimentOutputData();
	void initializeDataLogger();
	void RegisterMetaTypes();
	bool invokeExperimentObjectsSlots(const QString &sSlotName);
	bool configureExperiment();
	bool createExperimentObjects();
	bool connectExperimentObjects(bool bDisconnect = false, int nObjectID = -1);
	bool initializeExperiment(bool bFinalize = false);
	bool finalizeExperimentObjects();
	bool startExperimentObjects(bool bRunFullScreen = true);
	bool initExperimentObjects();
	bool stopExperimentObjects();
	bool abortExperimentObjects();
	bool changeExperimentObjectsSignalSlots(bool bDisconnect = false, int nSpecificIndex = -1);
	void cleanupExperimentObjects();
	void changeCurrentExperimentState(ExperimentState expCurrState);
	QObject *getObjectElementById(int nID);
	ExperimentState getCurrentExperimentState() {return experimentCurrentState;}
	QDomNodeList ExperimentObjectDomNodeList;
	QDomNodeList ExperimentBlockTrialsDomNodeList;
	QList<objectElement> lExperimentObjectList;

	QScriptEngine* currentScriptEngine;
	ExperimentState experimentCurrentState;
	bool m_RunFullScreen;
	bool m_DebugMode;
	QString m_ExpFileName;
	QString m_ExpFolder;
	ExperimentTree *currentExperimentTree;	
	ExperimentConfiguration strcExperimentConfiguration;
	ExperimentLogger *expDataLogger;
	int nExperimentTimerIndex;
};

#endif // ExperimentManager_H
