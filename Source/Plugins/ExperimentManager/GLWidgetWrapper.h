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
#include <QScriptValue>
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

public:
	GLWidgetWrapper(QWidget *parent = NULL);
	~GLWidgetWrapper();
	
	void setBlockTrials();
	
	template< typename T1 > bool insertExpObjectParameter(const int &nObjectID,const QString &sKeyName,const T1 &tVariabele,bool bIsInitializing = true)
	{
		bool bRetVal = false;
		QString tVarName = typeid(tVariabele).name();
		QString sValue = templateVariabeleToString(tVariabele);
		if(insertExpObjectBlockParameter(nObjectID,sKeyName,sValue,bIsInitializing))
			bRetVal = pExpConf->pExperimentManager->insertExperimentObjectVariabelePointer(nObjectID,sKeyName,tVariabele);
		return bRetVal;
	}
	QString templateVariabeleToString(const QString &Var) {return QString(Var);}
	QString templateVariabeleToString(const QColor &Var) {return Var.name();}
	QString templateVariabeleToString(const int &Var) {return QString::number(Var);}
	QString templateVariabeleToString(const float &Var) {return QString::number(Var);}
	QString templateVariabeleToString(const double &Var) {return QString::number(Var);}
	QString templateVariabeleToString(const bool &Var) 
	{
		if (Var)
			return TYPE_BOOL_TRUE; 
		else
			return TYPE_BOOL_FALSE;
	}
	template< typename T2 > T2* getExpObjectVariabelePointer(const int &nObjectID,const QString &sKeyName)
	{
		return pExpConf->pExperimentManager->getExperimentObjectVariabelePointer<T2>(nObjectID,sKeyName);
	}
	bool setScriptEngine(QScriptEngine *currScriptEngine) {currentScriptEngine = currScriptEngine; return true;};
	QScriptEngine *getScriptEngine() {return currentScriptEngine;};

public slots:
	//Can be overridden, virtual 
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
	bool insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing = true);
	ParsedParameterDefinition getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue);
	QScriptValue getExperimentObjectParameter(const int &nObjectID, const QString &strName);
	bool setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal);
	
protected:
	bool checkForNextBlockTrial();
	bool getExperimentBlockParamsFromDomNodeList(int nBlockNumber, int nObjectID, tParsedParameterList *hParams = NULL);//QHash<QString, QString> *hParams = NULL);
	void setupLayout(QWidget* layoutWidget);
	bool isDebugMode();
	bool getCurrentExperimentProgressSnapshot(ExperimentSnapshotStructure *expSnapshotstrc);
	bool getCurrentExpBlockTrialInternalTriggerAmount(int &nInternalTriggerAmount);
	int getCurrentStimuliRefreshRate() {return nRefreshRate;};
	QString getLastLoggedObjectStateTime(ExperimentSubObjectState state);
	void setDoubleBufferCheck(bool bShouldCheck) {bCheckForDoubleBuffering = bShouldCheck;};
	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *evt);
	void customEvent(QEvent *event);
	void initBlockTrial();
	void setAlternativeContainerDialog(QDialog *ContainerDlg = NULL);
	bool eventFilter(QObject *target, QEvent *event);

protected slots:
	void incrementExternalTrigger();
	void animate(bool bOnlyCheckBlockTrials = false);
	void finalizePaintEvent();

private:
	bool expandExperimentBlockParameterValue(QString &sValue);
	bool unlockExperimentObject();
	bool setExperimentObjectReadyToUnlock();
	void setVerticalSyncSwap();
	void init();
	int getRelativeBlockTrialTrigger(int nAbsoluteTrigger);		//Returns the relative (within a BlockTrial) trigger from a absolute (within the whole Experiment) trigger
	bool updateExperimentBlockTrialStructure();
	bool cleanupExperimentBlockTrialStructure();
	bool changeSubObjectState(ExperimentSubObjectState newSubObjectState);
	ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;}

private:
	bool bCurrentSubObjectReadyToUnlock;				//The user first has to press the 'Alt' key before the experiment can be unlocked by the next trigger.
	bool bFirstTriggerAfterUnlock;						//To detect the exact start of the experiment detected by the checkForNextBlockTrial() function.
	bool bCurrentSubObjectIsLocked;						//After the above key is pressed this variable is set to false at the first trigger and the experiment starts.
	bool bCheckForDoubleBuffering;
	double dWaitTime;
	QMutex mutExpSnapshot;
	QMutex mutRecursivePaint;
	QMutex mutProcEvents;								//Another implementation, due to qApp->processEvents() RecursiveRepaint can occur...
	ExperimentSnapshotFullStructure expFullStruct;
	int nCurrentExperimentReceivedExternalTriggers;		//The current experiment number of external triggers received since it started, local use!
	int nCurrentExperimentProcessedExternalTriggers;	//The current experiment processed external triggers, local use!
	int nCurrentExperimentLastProcExternalTriggers;		//The current experiment last processed external trigger, local use!
	int nCurrentExperimentReceivedInternalTriggers;		//The current experiment number of internal triggers incremented since it started, local use!
	int nRefreshRate;									//The refresh rate of the screen
	double dLastPreSwapTime;
	double dAdditionalRefreshDelayTime;
	ContainerDlg *stimContainerDlg;
	QObject *alternativeContainerDlg;
	bool bExperimentShouldStop;
	QRectF rScreenResolution;
	QVBoxLayout *mainLayout;
	int nObjectID;
	QTimer tStimTimer;
	QDomNodeList *pExpBlockTrialDomNodeList;
	ExperimentBlockTrialStructure strcExperimentBlockTrials;
	QEvent::Type tEventObjectStopped;
	ExperimentSubObjectState currentSubObjectState;
	ExperimentSubObjectStateHistory subObjectStateHistory;
	int nFrameTimerIndex;
	int nTrialTimerIndex;
	tParsedParameterList *ExpBlockParams;
	ExperimentConfiguration *pExpConf;
	ExperimentTimer expTrialTimer;
	const QMetaObject* thisMetaObject;
	QPainter *lockedPainter;
	QScriptEngine* currentScriptEngine;
};

#endif // GLWIDGET_H
