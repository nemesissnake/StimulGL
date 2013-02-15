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
#include <QKeyEvent>
#include "ExperimentManager.h"
#include "ExperimentTimer.h"

using namespace ExperimentManagerNameSpace;

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
		You can use this Signal to keep track of whenever the External Trigger gets incremented, see ExperimentEngine::incrementExternalTrigger
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

	template< typename T1 > bool insertExpObjectParameter(const int &nObjectID,const QString &sKeyName,const T1 &tVariabele,bool bIsInitializing = true)
	{
		bool bRetVal = false;
		QString tVarName = typeid(tVariabele).name();
		QString sValue = templateVariabeleToString(tVariabele);
		if(insertExpObjectBlockParameter(nObjectID,sKeyName,sValue,bIsInitializing))
			bRetVal = pExperimentManager->insertExperimentObjectVariabelePointer(nObjectID,sKeyName,tVariabele);
		return bRetVal;
	}
	QString templateVariabeleToString(const QString &Var) {return Var;}
	QString templateVariabeleToString(const QStringList &Var) {return Var.join(",");}
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
	bool initExperimentObject();
	bool startExperimentObject();
	bool stopExperimentObject();
	bool abortExperimentObject();
	void ExperimentShouldFinish();					//Slot for a signal to connect to
	bool setExperimentObjectID(int nObjID);			//Necessary to set the ID!
	bool setExperimentMetaObject();					//Necessary to set the MetaObject!
	const QMetaObject *getExperimentMetaObject();
	virtual bool setExperimentManager(ExperimentManager *expManager);
	
	int getObjectID();
	bool insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing = true);
	ParsedParameterDefinition getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue);
	QScriptValue getExperimentObjectParameter(const int &nObjectID, const QString &strName);
	bool setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal);
	
protected:
	int checkForNextBlockTrial();
	bool isDebugMode();
	bool isLocked() {return bCurrentSubObjectIsLocked;};
	bool isReadyToUnlock() {return bCurrentSubObjectReadyToUnlock;};
	bool experimentShouldStop() {return bExperimentShouldStop;};
	double getElapsedTrialTime() {return dElapsedTrialTime;};
	int getCurrentBlockTrialFrame() {return nCurrExpBlockTrialFrame;};
	int incrementCurrentBlockTrialFrame() {return nCurrExpBlockTrialFrame++;};
	int getFrameTimerIndex() {return nFrameTimerIndex;};
	QString getLastLoggedObjectStateTime(ExperimentSubObjectState state);
	void customEvent(QEvent *event);
	void initBlockTrial();
	ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;};
	bool changeSubObjectState(ExperimentSubObjectState newSubObjectState);

protected slots:
	void incrementExternalTrigger();
	void animate(bool bOnlyCheckBlockTrials = false);

private:
	bool expandExperimentBlockParameterValue(QString &sValue);
	bool unlockExperimentObject();
	bool setExperimentObjectReadyToUnlock();
	void init();
	void fetchCurrentExperimentStructures();

	bool bCurrentSubObjectReadyToUnlock;				//The user first has to press the 'Alt' key before the experiment can be unlocked by the next trigger.
	bool bFirstTriggerAfterUnlock;						//To detect the exact start of the experiment detected by the checkForNextBlockTrial() function.
	bool bCurrentSubObjectIsLocked;						//After the above key is pressed this variable is set to false at the first trigger and the experiment starts.
	double dWaitTime;
	QMutex mutProcEvents;								//Another implementation, due to qApp->processEvents() RecursiveRepaint can occur...
	int nLastProcessedExternalTriggers;
	int nCurrExpBlockTrialFrame;
	double dElapsedTrialTime;
	bool bExperimentShouldStop;
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
	QScriptEngine* currentScriptEngine;
};

#endif // EXPERIMENTENGINE_H
