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


#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QDialog>
#include <QGLWidget>
#include <QTimer>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QCustomEvent>
#include <QMutex>
#include <omp.h>
#include "Global.h"
#include "ContainerDlg.h"
#include "ExperimentTimer.h"

enum GLWidgetPaintFlags //An optional paint flag that can be forwarded to the FUNC_PAINTOBJECT_FULL routine 
{
	GLWidgetPaintFlags_NoFlag		= 0,
	GLWidgetPaintFlags_LockedState	= 1,
	GLWidgetPaintFlags_Reserved1	= 2, //This one is reserved for future implementation
	GLWidgetPaintFlags_Reserved2	= 4, //This one is reserved for future implementation
	GLWidgetPaintFlags_Reserved3	= 8, //This one is reserved for future implementation
	GLWidgetPaintFlags_Reserved4	= 16 //This one is reserved for future implementation
};

class QPaintEvent;
class QWidget;
class ContainerDlg;
class QDomNodeList;

class GLWidgetWrapper : public QGLWidget
{
	Q_OBJECT

signals:
	void UserWantsToClose(void);
	void ObjectShouldStop(void);
	void ObjectStateHasChanged(ExperimentSubObjectState);
	void NewInitBlockTrial();
	void ExternalTriggerIncremented(int);
	void ExperimentStructureChanged(int,int,int);//Block,Trial,InternalTrigger
	//void LogToOutputWindow(const QString &strText2Write);

public:
	GLWidgetWrapper(QWidget *parent = NULL);
	~GLWidgetWrapper();

	void setBlockTrials();

public slots:
	//Can be overridden
	//virtual 
	bool initExperimentObject();
	bool startExperimentObject();
	bool stopExperimentObject();
	bool abortExperimentObject();
	bool setBlockTrialDomNodeList(QDomNodeList *pExpBlockTrialDomNodeList = NULL);
	bool setExperimentObjectID(int nObjID);			//Necessary to set the ID!
	bool setExperimentMetaObject();					//Necessary to set the MetaObject!
	bool setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct = NULL);
	
	void setStimuliResolution(int w, int h);
	QRectF getScreenResolution();
	int getObjectID();
	bool insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing = true);
	ParsedParameterDefinition getExperimentObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue);

protected:
	bool checkForNextBlockTrial();
	bool getExperimentBlockParameter(int nBlockNumber, int nObjectID, QString strParamName, ParsedParameterDefinition &pParDef);//QString &Result);
	bool getExperimentBlockParamsFromDomNodeList(int nBlockNumber, int nObjectID, tParsedParameterList *hParams = NULL);//QHash<QString, QString> *hParams = NULL);
	void setupLayout(QWidget* layoutWidget);	
	bool isDebugMode();
	bool getCurrentExperimentProgressSnapshot(ExperimentSnapshotStructure *expSnapshotstrc);
	//bool getExperimentBlockTrialStructureCopy(ExperimentBlockTrialStructure &expBlockTrialStructure);
	bool getCurrentExpBlockTrialInternalTriggerAmount(int &nInternalTriggerAmount);
	int getCurrentStimuliRefreshRate() {return nRefreshRate;};
	//int getElapsedFrameTime();
	QString getLastLoggedObjectStateTime(ExperimentSubObjectState state);
	void setDoubleBufferCheck(bool bShouldCheck) {bCheckForDoubleBuffering = bShouldCheck;};
	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *evt);
	void customEvent(QEvent *event);
	void initBlockTrial();
	bool eventFilter(QObject *target, QEvent *event);

protected slots:
	void incrementExternalTrigger();
	void animate(bool bOnlyCheckBlockTrials = false);
	void finalizePaintEvent();
	//void proceedPaintEventLoop();;

private:
	//int getCurrentExperimentBlockTrialFrame() {return nBlockTrialFrameCounter;}
	//int getCurrentExperimentTrial() {return nCurrentExperimentTrial;}
	//int getCurrentExperimentBlock() {return nCurrentExperimentBlock;}
	//int getCurrentExperimentTrigger() {return nCurrentExperimentTrigger;}
	//int getCurrentExperimentBlockTrialTrigger() {return nCurrentExperimentBlockTrialReceivedTriggers;}
	//int getExperimentBlockTrialTriggerAmount(int nBlock, int nTrial);
	bool unlockExperimentObject();
	bool setExperimentObjectReadyToUnlock();
	void setVerticalSyncSwap();
	void init();
	int getRelativeBlockTrialTrigger(int nAbsoluteTrigger);		//Returns the relative (within a BlockTrial) trigger from a absolute (within the whole Experiment) trigger
	bool updateExperimentBlockTrialStructure();
	bool cleanupExperimentBlockTrialStructure();
	void changeSubObjectState(ExperimentSubObjectState newSubObjectState);
	ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;}

private:
	bool bCurrentSubObjectReadyToUnlock;				//The user first has to press the 'Alt' key before the experiment can be unlocked by the next trigger.
	bool bFirstTriggerAfterUnlock;						//To detect the exact start of the experiment detected by the checkForNextBlockTrial() function.
	bool bCurrentSubObjectIsLocked;						//After the above key is pressed this variable is set to false at the first trigger and the experiment starts.
	bool bCheckForDoubleBuffering;
	double dWaitTime;
	QMutex mutExpSnapshot;
	QMutex mutRecursivePaint;
	ExperimentSnapshotFullStructure expFullStruct;
	//int nBlockTrialFrameCounter;
	int nCurrentExperimentReceivedExternalTriggers;		//The current experiment number of external triggers received since it started, local use!
	int nCurrentExperimentProcessedExternalTriggers;	//The current experiment processed external triggers, local use!
	int nCurrentExperimentLastProcExternalTriggers;		//The current experiment last processed external trigger, local use!
	int nCurrentExperimentReceivedInternalTriggers;		//The current experiment number of internal triggers incremented since it started, local use!
	//int nCurrentExperimentTrigger;					//The current experiment trigger
	//int nCurrentExperimentTrial;						//The current experiment trial within the block 
	//int nCurrentExperimentBlockTrialReceivedTriggers;	//The current experiment number of trigger received within the current block trial
	//int nCurrentExperimentBlock;						//The current experiment block
	//int nTotalProcessedExperimentTrials;				//The total number of trials processed within experiment, might be that this Trial is not fully processed
	int nRefreshRate;									//The refresh rate of the screen
	double dLastPreSwapTime;
	double dAdditionalRefreshDelayTime;
	ContainerDlg *stimContainerDlg;
	bool bForceToStop;
	bool bExperimentShouldStop;
	QRectF rScreenResolution;
	//int nMinScreenUpdateTime;
	QVBoxLayout *mainLayout;
	int nObjectID;
	QTimer tStimTimer;
	QDomNodeList *pExpBlockTrialDomNodeList;
	//int nNextThresholdTriggerCount;//When we should switch to the next block
	ExperimentBlockTrialStructure strcExperimentBlockTrials;
	QEvent::Type tEventObjectStopped;
	ExperimentSubObjectState currentSubObjectState;
	ExperimentSubObjectStateHistory subObjectStateHistory;
	int nFrameTimerIndex;
	int nTrialTimerIndex;
	//QHash<QString, QString> *ExpBlockParams;
	tParsedParameterList *ExpBlockParams;
	ExperimentConfiguration *pExpConf;
	ExperimentTimer expTrialTimer;
	const QMetaObject* thisMetaObject;
	QPainter *lockedPainter;
};

#endif // GLWIDGET_H
