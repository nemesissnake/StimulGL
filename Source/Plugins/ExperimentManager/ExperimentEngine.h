//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

#ifndef EXPERIMENTENGINE_H
#define EXPERIMENTENGINE_H

#include <QObject>
//#include <QTimer>
#include <QKeyEvent>
//#include <QEvent>
//#include <QMutex>
//#include <QScriptValue>

#include "ExperimentManager.h"
//#include "Global.h"
#include "ExperimentTimer.h"

using namespace ExperimentManagerNameSpace;

//#define GLWWRAP_WIDGET_STIMULI_REFRESHRATE	"stimulirefreshrate"

//enum GLWidgetPaintFlags //An optional paint flag that can be forwarded to the FUNC_PAINTOBJECT_FULL routine 
//{
//	GLWidgetPaintFlags_NoFlag		= 0,
//	GLWidgetPaintFlags_LockedState	= 1,
//	GLWidgetPaintFlags_Reserved1	= 2, //This one is reserved for future implementation
//	GLWidgetPaintFlags_Reserved2	= 4, //This one is reserved for future implementation
//	GLWidgetPaintFlags_Reserved3	= 8, //This one is reserved for future implementation
//	GLWidgetPaintFlags_Reserved4	= 16 //This one is reserved for future implementation
//};

//class QPaintEvent;
//class QWidget;
//class ContainerDlg;
class QDomNodeList;

//!  The ExperimentEngine class. 
/*!
  The ExperimentEngine class can be derived from to create custom classes that can again be used in combination with the ExperimentManager for presenting visual or other kind of stimuli.
  This class is optimized for various painting operations supported by OpenGL.
*/
class ExperimentEngine : public QObject
{
	friend class QML2Viewer;

	Q_OBJECT

signals:
	//! The UserWantsToClose Signal.
	/*!
		You can use this Signal to detect whenever the user tries to abort the experiment.
		No Parameter.
	*/
	void UserWantsToClose(void);
	//! The ObjectShouldStop Signal.
	/*!
		You can use this Signal to detect whenever this object tries to abort/stop the experiment.
		No Parameter.
	*/
	void ObjectShouldStop(void);
	//! The ObjectStateHasChanged Signal.
	/*!
		You can use this Signal to detect whenever the Experiment State for this object changes.
		Parameter expSubObjectState is type of the enum ExperimentManagerNameSpace::ExperimentSubObjectState.
	*/
	void ObjectStateHasChanged(ExperimentSubObjectState expSubObjectState);
	//! The NewInitBlockTrial Signal.
	/*!
		You can use this Signal to keep track of whenever the Experiment Manager tries to Initialize a new BlockTrial for this object.
		No Parameter.
	*/
	void NewInitBlockTrial(void);
	//! The ExternalTriggerIncremented Signal.
	/*!
		You can use this Signal to keep track of whenever the External Trigger gets incremented, see GLWidgetWrapper::incrementExternalTrigger
		Please bear in mind that this is already emitted before a new BlockTrial is initialized.
		Parameter nTrigger holds the number of received external triggers after the start of the experiment.
	*/
	void ExternalTriggerIncremented(int nTrigger);
	//! The ExperimentStructureChanged Signal.
	/*!
		You can use this Signal to keep track Experiment structure changes.
		Parameter nBlock represents the current Block number.
		Parameter nTrial represents the current Trial number.
		Parameter nInternalTrigger represents the current Internal Trigger number.
	*/
	void ExperimentStructureChanged(int nBlock,int nTrial,int nInternalTrigger);

public:
	ExperimentEngine(QObject *parent = NULL);
	~ExperimentEngine();
	
	//void setBlockTrials();
	
	template< typename T1 > bool insertExpObjectParameter(const int &nObjectID,const QString &sKeyName,const T1 &tVariabele,bool bIsInitializing = true)
	{
		bool bRetVal = false;
		QString tVarName = typeid(tVariabele).name();
		QString sValue = templateVariabeleToString(tVariabele);
		if(insertExpObjectBlockParameter(nObjectID,sKeyName,sValue,bIsInitializing))
			bRetVal = pExperimentManager->insertExperimentObjectVariabelePointer(nObjectID,sKeyName,tVariabele);
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
	void ExperimentShouldFinish();					//Slot for a signal to connect to
	bool setExperimentObjectID(int nObjID);			//Necessary to set the ID!
	bool setExperimentMetaObject();					//Necessary to set the MetaObject!
	virtual bool setExperimentManager(ExperimentManager *expManager);
	//void setStimuliResolution(int w, int h);
	//QRectF getScreenResolution();
	
	int getObjectID();
	//strcScriptExperimentStructure getExperimentStructure();
	bool insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing = true);
	ParsedParameterDefinition getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue);
	QScriptValue getExperimentObjectParameter(const int &nObjectID, const QString &strName);
	bool setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal);
	
protected:
	int checkForNextBlockTrial();
	//void setupLayout(QWidget* layoutWidget);
	//void setupLayout2(QQuickView* quickView);
	bool isDebugMode();
	double getElapsedTrialTime() {return dElapsedTrialTime;};
	int getCurrentBlockTrialFrame() {return nCurrExpBlockTrialFrame;};
	//int getCurrentStimuliRefreshRate() {return nRefreshRate;};
	QString getLastLoggedObjectStateTime(ExperimentSubObjectState state);
	//void setDoubleBufferCheck(bool bShouldCheck) {bCheckForDoubleBuffering = bShouldCheck;};
	//void paintEvent(QPaintEvent *event);
	//void closeEvent(QCloseEvent *evt);
	void customEvent(QEvent *event);
	void initBlockTrial();
	//void setAlternativeContainerDialog(QDialog *ContainerDlg = NULL);
	//bool eventFilter(QObject *target, QEvent *event);

protected slots:
	void incrementExternalTrigger();
	void animate(bool bOnlyCheckBlockTrials = false);
	//void finalizePaintEvent();

private:
	bool expandExperimentBlockParameterValue(QString &sValue);
	bool unlockExperimentObject();
	bool setExperimentObjectReadyToUnlock();
	//void setVerticalSyncSwap();
	void init();
	bool changeSubObjectState(ExperimentSubObjectState newSubObjectState);
	ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;};
	void fetchCurrentExperimentStructures();

	bool bCurrentSubObjectReadyToUnlock;				//The user first has to press the 'Alt' key before the experiment can be unlocked by the next trigger.
	bool bFirstTriggerAfterUnlock;						//To detect the exact start of the experiment detected by the checkForNextBlockTrial() function.
	bool bCurrentSubObjectIsLocked;						//After the above key is pressed this variable is set to false at the first trigger and the experiment starts.
	//bool bCheckForDoubleBuffering;
	double dWaitTime;
	//QMutex mutRecursivePaint;
	QMutex mutProcEvents;								//Another implementation, due to qApp->processEvents() RecursiveRepaint can occur...
	int nLastProcessedExternalTriggers;
	int nCurrExpBlockTrialFrame;
	double dElapsedTrialTime;
	//int nRefreshRate;									//The refresh rate of the screen
	//double dLastPreSwapTime;
	//double dAdditionalRefreshDelayTime;
	//ContainerDlg *stimContainerDlg;
	//Quick2ContainerWidget *Quick2CntnrWdgt;
	//QObject *alternativeContainerDlg;
	bool bExperimentShouldStop;
	//QRectF rScreenResolution;
	//QVBoxLayout *mainLayout;
	int nObjectID;
	QTimer tStimTimer;
	QEvent::Type tEventObjectStopped;
	ExperimentSubObjectState currentSubObjectState;
	ExperimentSubObjectStateHistory subObjectStateHistory;
	int nFrameTimerIndex;
	int nTrialTimerIndex;
	tParsedParameterList *ExpBlockParams;
	ExperimentManager *pExperimentManager; 
	ExperimentTimer expTrialTimer;
	const QMetaObject* thisMetaObject;
	//QPainter *lockedPainter;
	QScriptEngine* currentScriptEngine;
};

#endif // EXPERIMENTENGINE_H
