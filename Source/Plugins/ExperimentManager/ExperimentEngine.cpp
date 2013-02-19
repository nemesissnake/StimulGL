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

#include "ExperimentEngine.h"
#include <omp.h>

/*! \brief The ExperimentEngine constructor.
*
*   You do not need to specify the parent object. 
*	The StimulGL script engine automatically retrieves the parent role
*/
ExperimentEngine::ExperimentEngine(QObject *parent)	: QObject(parent)
{
	currentSubObjectState = Experiment_SubObject_Constructing;
	thisMetaObject = NULL;
	pExperimentManager = NULL;
	currentScriptEngine = NULL;
	bExperimentShouldStop = false;
	init();
	nObjectID = -1;
	tEventObjectStopped = (QEvent::Type)(QEvent::User + 1);	
	ExpBlockParams = NULL;
	nFrameTimerIndex = -1;
	nTrialTimerIndex = -1;
	bCurrentSubObjectIsLocked = true;
	bCurrentSubObjectReadyToUnlock = false;
}

/*! \brief The ExperimentEngine destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
ExperimentEngine::~ExperimentEngine()
{
	changeSubObjectState(Experiment_SubObject_Destructing);
	if (ExpBlockParams)
	{
		delete ExpBlockParams;
		ExpBlockParams = NULL;
	}
	if (currentScriptEngine)
	{
		currentScriptEngine = NULL;//not owned by this class
	}
}

bool ExperimentEngine::insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing)
{
/*! \brief Update or insert an Experiment Block Parameter that can be parsed.
 *
 *  This function can be used to insert or update an Experiment Block Parameter to be parsed for a specific object.
 *  This Parameter can be parsed at the next initialization of a new BlockTrial if not jet initialized.
 *  Setting the parameter bIsInitializing to true forces the new parameter to be re-parsed at the next initialization of a new BlockTrial.
 */
	if (ExpBlockParams == NULL)
	{
		ExpBlockParams = new tParsedParameterList();
		pExperimentManager->setExperimentObjectBlockParameterStructure(nObjectID,ExpBlockParams);
	}
	bool bRetVal = pExperimentManager->insertExperimentObjectBlockParameter(nObjectID,sName,sValue,bIsInitializing);
	return bRetVal;
}

ParsedParameterDefinition ExperimentEngine::getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue)
{
/*! \brief retrieves an buffered Experiment Block Parameter.
 *
 *  This function can be used to retrieve an buffered Experiment Block Parameter for a specific object.
 *	If this parameter is not found then the default String value(sDefValue) is returned.
 */
	ParsedParameterDefinition PPDResult;
	PPDResult.bHasChanged = false;
	PPDResult.sValue = sDefValue;
	PPDResult.bIsInitialized = true;
	if (pExperimentManager->getExperimentObjectBlockParameter(nObjectID,sName,PPDResult))		
		return PPDResult;
	else
		return PPDResult;
}

void ExperimentEngine::init()
{
	nLastProcessedExternalTriggers = CF_UNINITIALIZED;
	nCurrExpBlockTrialFrame = CF_UNINITIALIZED;
	changeSubObjectState(Experiment_SubObject_Initialized);
}

int ExperimentEngine::getObjectID()
{
/*! \brief Returns this Object ID.
 *
 *  This function returns its unique Object ID as registered in the ExperimentManager.
 */
	return nObjectID;
}

bool ExperimentEngine::setExperimentMetaObject()
{
	thisMetaObject = this->metaObject();
	return true;
}

const QMetaObject *ExperimentEngine::getExperimentMetaObject()
{
	return thisMetaObject;
}

bool ExperimentEngine::setExperimentObjectID(int nObjID)
{
	if(nObjID >= 0)
	{
		nObjectID = nObjID;
		bool bRetVal;
		if (thisMetaObject)
		{
			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETOBJECTID_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(thisMetaObject->invokeMethod(this, FUNC_SETOBJECTID, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int,nObjID))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETOBJECTID_FULL << ")!";		
				}		
			}
		}
	}
	return false;
}

void ExperimentEngine::customEvent(QEvent *event)
{
	if (event->type() == tEventObjectStopped) 
	{
		emit ObjectShouldStop();
	} 
	else 
	{
		ExperimentEngine::customEvent(event);
	}
}

void ExperimentEngine::ExperimentShouldFinish()
{
	emit ObjectShouldStop();
	bExperimentShouldStop = true;
	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Finished experiment"));
}

bool ExperimentEngine::setExperimentManager(ExperimentManager *expManager)
{
	if(expManager)
	{
		pExperimentManager = expManager;
		bool bRetVal;
		if (thisMetaObject)
		{
			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTMANAGER_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(thisMetaObject->invokeMethod(this, FUNC_SETEXPERIMENTMANAGER, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(ExperimentManager*,pExperimentManager))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETEXPERIMENTMANAGER << ")!";		
				}		
			}
		}
		return true;
	}
	return false;
}

bool ExperimentEngine::expandExperimentBlockParameterValue(QString &sValue)
{
	if (pExperimentManager)
	{
		return pExperimentManager->expandExperimentBlockParameterValue(sValue);				
	}
	return false;
}

bool ExperimentEngine::startExperimentObject()
{
	bFirstTriggerAfterUnlock = true;
	pExperimentManager->startExperimentTimer(nFrameTimerIndex);//Starts the Frame timer
	pExperimentManager->startExperimentTimer(nTrialTimerIndex);//Starts the Trial timer
	checkForNextBlockTrial();
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STARTOBJECT_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_STARTOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_STARTOBJECT_FULL << ")!";		
			}		
		}
	}
	return true;
}

bool ExperimentEngine::setExperimentObjectReadyToUnlock()
{
	if (bCurrentSubObjectReadyToUnlock == false)
	{
		bCurrentSubObjectReadyToUnlock = true;
		if(isDebugMode() && pExperimentManager)
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Ready to Unlock the Experiment Object");
	}
	return true;
}

bool ExperimentEngine::unlockExperimentObject()
{
	if (bCurrentSubObjectIsLocked)
	{
		bCurrentSubObjectIsLocked = false;
		if(isDebugMode() && pExperimentManager)
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Unlocked the Experiment Object");
	}
	return true;
}

bool ExperimentEngine::initExperimentObject()
{
	nFrameTimerIndex = pExperimentManager->createExperimentTimer();
	nTrialTimerIndex = pExperimentManager->createExperimentTimer();
	bCurrentSubObjectIsLocked = true;
	bCurrentSubObjectReadyToUnlock = false;
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_INITOBJECT_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_INITOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_INITOBJECT_FULL << ")!";		
			}		
		}
	}	
	int iCPU = omp_get_num_procs(); // Get the number of processors in this system
    qDebug() << __FUNCTION__ << "::" << iCPU << "processors available.";//The omp_get_num_threads() call returns 1 in the serial section of the code!!
	omp_set_num_threads(iCPU); // Set the number of threads
	bool bResult = connect(pExperimentManager->getExperimentStructure(),SIGNAL(experimentStopped()),this,SLOT(ExperimentShouldFinish()));
	return true;
}

bool ExperimentEngine::isDebugMode() 
{
	if (pExperimentManager)
	{
		if(pExperimentManager->getExperimentStructure())
			return pExperimentManager->getExperimentStructure()->getExperimentDebugMode();
	}
	return false;
}

QString ExperimentEngine::getLastLoggedObjectStateTime(ExperimentSubObjectState state)
{
	if (!subObjectStateHistory.nState.isEmpty())
	{
		int nHistoryCount = subObjectStateHistory.nState.count();
		if (nHistoryCount > 0)
		{
			for (int i=nHistoryCount-1;i>0;i--)
			{
				if (subObjectStateHistory.nState.at(i) == state)
				{
					return subObjectStateHistory.sDateTimeStamp.at(i);
				}
			}
		} 
	}
	return "";
}

bool ExperimentEngine::stopExperimentObject()
{
	bExperimentShouldStop = true;
	changeSubObjectState(Experiment_SubObject_Stop);
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STOPOBJECT_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_STOPOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_STOPOBJECT_FULL << ")!";		
			}		
		}
	}
	return true;
}

bool ExperimentEngine::abortExperimentObject()
{
	return stopExperimentObject();
}

void ExperimentEngine::incrementExternalTrigger()
{
/*! \brief Increments the External Trigger.
 *
 *  Invoke this slot to increment the External Trigger.
 */
	if (bCurrentSubObjectIsLocked)
	{
		if (bCurrentSubObjectReadyToUnlock == true)
		{
			bCurrentSubObjectReadyToUnlock = false;
			unlockExperimentObject();
			if(pExperimentManager)
			{
				pExperimentManager->getExperimentStructure()->resetExperiment();
				pExperimentManager->getExperimentStructure()->prepareExperiment();
				pExperimentManager->getExperimentStructure()->incrementExternalTrigger();
			}
			//else
			//{//No Experiment Manager? QML?
				//update();
				//QTimer::singleShot(1000, this, SLOT(incrementExternalTrigger()));//Let's start!
				//QMetaObject::invokeMethod()
			//}
		}
	}
	else
	{
		if(pExperimentManager)
		{
			pExperimentManager->getExperimentStructure()->incrementExternalTrigger();
			emit ExternalTriggerIncremented(pExperimentManager->getExperimentStructure()->getExternalTriggerCount());//Should be automatically connected!!
			if(isDebugMode() && pExperimentManager)
				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Externally Triggered!, Received="),QString::number(pExperimentManager->getExperimentStructure()->getExternalTriggerCount()));
		}
		else
		{
			emit ExternalTriggerIncremented(-1);
			if(isDebugMode() && pExperimentManager)
				qDebug() << __FUNCTION__ << "Automatically Externally Triggered!, no experiment manager active.";
		}
		if(getSubObjectState() == Experiment_SubObject_Started)
		{
			if (bExperimentShouldStop==false)
			{
				int nResult = checkForNextBlockTrial();
			}
		}
	}
}

void ExperimentEngine::initBlockTrial()
{
	emit NewInitBlockTrial();
	bExperimentShouldStop = false;
	expTrialTimer.restart();
	if(pExperimentManager)
	{
		bool bHasCurrentBlock = false;
		cBlockStructure tmpBlock = pExperimentManager->getExperimentStructure()->getCurrentBlock(bHasCurrentBlock);
		if(bHasCurrentBlock)
			pExperimentManager->fetchExperimentBlockParamsFromDomNodeList(tmpBlock.getBlockNumber(),nObjectID);
		else
			qDebug() << __FUNCTION__ << "::Could not create a Block Parameter List, no current block!";
	}
	else
	{
		qDebug() << __FUNCTION__ << "::No ExperimentManager defined!";
	}
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_INITOBJECTBLOCKTRIAL_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_INITOBJECTBLOCKTRIAL, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_INITOBJECTBLOCKTRIAL_FULL << ")!";		
			}		
		}
	}
}

int ExperimentEngine::checkForNextBlockTrial()
{
	if (bExperimentShouldStop)
		return 0;
	if (pExperimentManager == NULL)
	{
		if(bCurrentSubObjectIsLocked)
			return -1;
		//qDebug() << __FUNCTION__ << "::No Experiment Manager set!";
		return -1;//Special case... direct QML execute
	}
	bool goToNextBlockTrial = false;
	bool bFirstCheckAfterExperimentStarted = false;	
	int nRetval = 0;
	bool bExperimentStructureChanged = false;
	bool bHasCurrentBlock = false;
	cExperimentStructure tmpExpStr = cExperimentStructure(*pExperimentManager->getExperimentStructure());
	cBlockStructure tmpExpBlockStr = tmpExpStr.getCurrentBlock(bHasCurrentBlock);
	strcExperimentStructureState tmpExpStrState = tmpExpStr.getCurrentExperimentState();

	if((tmpExpStrState.Experiment_ExternalTrigger == RA_REINITIALIZE) && (nCurrExpBlockTrialFrame == CF_UNINITIALIZED))
	{//First Experiment Trial? (Start/Init), occurs before the start of the Trigger(timer)
		goToNextBlockTrial = true;
		bFirstCheckAfterExperimentStarted = true;		
		bExperimentStructureChanged = true;
	}
	if ((tmpExpStrState.Experiment_ExternalTrigger >= 0) && (tmpExpStrState.Experiment_ExternalTrigger > nLastProcessedExternalTriggers))
	{
		nLastProcessedExternalTriggers = tmpExpStrState.Experiment_ExternalTrigger;
		bExperimentStructureChanged = true;
	}
	if(bFirstCheckAfterExperimentStarted == false)
	{
		if(tmpExpStr.getBlockCount() > 0)//Are there blocks defined? QML Viewers trough UI (without ExperimentManager) don't have any defined blocks here!
			if(bHasCurrentBlock)
			{
				goToNextBlockTrial = (tmpExpStrState.CurrentBlock_ExternalTrigger == 0) && (tmpExpStrState.CurrentBlock_InternalTrigger == 0);//Go to next Block Trial?
			}
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		bExperimentStructureChanged = true;
		nCurrExpBlockTrialFrame = 0;
		if(pExperimentManager)
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Starting to Init new BlockTrial"),QString("BlockNumber=") + QString::number(tmpExpBlockStr.getBlockNumber()) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +  ", TrialExternalTrigger=" + QString::number(tmpExpStrState.CurrentBlock_ExternalTrigger) + ", TrialInternalTrigger=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(nCurrExpBlockTrialFrame) + ")");
		initBlockTrial();
		if (bFirstCheckAfterExperimentStarted)
		{
			changeSubObjectState(Experiment_SubObject_Started);
		}
		if (bCurrentSubObjectIsLocked == false)
		{
			if((isDebugMode()) && (pExperimentManager))
				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","New BlockTrial Prepared, going to qeueu animate()");
			QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
		}
		else
		{
			if(tmpExpStr.getBlockCount() > 0)//Are there blocks defined? QML Viewers trough UI (without ExperimentManager) don't have any defined blocks here!
				if(bHasCurrentBlock)
					QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));
		}
		expTrialTimer.restart();
		nRetval = 1;
	}
	else
	{
		nRetval = 0;
	}
	if ((bCurrentSubObjectIsLocked == false) && (bFirstTriggerAfterUnlock))//has the experiment just been unlocked for the first time?
	{
		bFirstTriggerAfterUnlock = false;
		emit ExperimentStructureChanged(tmpExpBlockStr.getBlockNumber(),tmpExpStrState.CurrentBlock_TrialNumber,tmpExpStrState.CurrentBlock_InternalTrigger);
	}
	else
	{
		if ((bExperimentStructureChanged) && (bFirstCheckAfterExperimentStarted == false))
		{
			if(getSubObjectState() == Experiment_SubObject_Started)
				emit ExperimentStructureChanged(tmpExpBlockStr.getBlockNumber(),tmpExpStrState.CurrentBlock_TrialNumber,tmpExpStrState.CurrentBlock_InternalTrigger);
		}
	}
	return nRetval;
}

void ExperimentEngine::animate(bool bOnlyCheckBlockTrials)
{
//	//QObject *a = sender();	
	if(getSubObjectState() == Experiment_SubObject_Started)
	{
		if (bExperimentShouldStop)
		{
			changeSubObjectState(Experiment_SubObject_Stop);
			return;
		}
	}
}

bool ExperimentEngine::changeSubObjectState(ExperimentSubObjectState newSubObjectState)
{
	if(newSubObjectState != currentSubObjectState)
	{
		if((((int)newSubObjectState > (int)ExperimentSubObjectState::Experiment_SubObject_MaxState) || ((int)newSubObjectState < 0)) ||
		  (((int)currentSubObjectState > (int)ExperimentSubObjectState::Experiment_SubObject_MaxState) || ((int)currentSubObjectState < 0)) ||
		  ((int)currentSubObjectState == Experiment_SubObject_Destructing))
		{
			return false;
		}
		currentSubObjectState = newSubObjectState;
		subObjectStateHistory.nState.append(currentSubObjectState);
		subObjectStateHistory.sDateTimeStamp.append(QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
		emit ObjectStateHasChanged(currentSubObjectState);
		if(currentSubObjectState == Experiment_SubObject_Stop)
		{
			qApp->setActiveWindow(MainAppInfo::getMainWindow());
		}
		return true;
	}
	return false;
}

QScriptValue ExperimentEngine::getExperimentObjectParameter(const int &nObjectID, const QString &strName)
{
//*! \brief retrieves the current value of an Experiment Parameter variable.
// *
// *  This function can be used to retrieve the current value of an Experiment Parameter variable for a specific object.
// *	This parameter variable value is currently in use.
// */
	if (currentScriptEngine)
	{
		//return currentScriptEngine->toScriptValue(getExpObjectVariabelePointer<QString>(nObjectID,strName));
		QScriptValue sScriptValue;
		if(pExperimentManager->getExperimentObjectScriptValue(nObjectID,strName,sScriptValue))
		{
			//QString test = sScriptValue.toString();
			return sScriptValue;
		}
		return currentScriptEngine->undefinedValue();
	}
	qDebug() << __FUNCTION__ << ":No Script Engine available!";
	return NULL;	
}

bool ExperimentEngine::setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal)
{
/*! \brief Sets the current value of an Experiment Parameter variable.
 *
 *  This function can be used to immediately set a value of an Experiment Parameter variable for a specific object.
 *	This parameter variable value is immediately set and used.
 */
	return pExperimentManager->setExperimentObjectFromScriptValue(nObjectID,strName,sScriptVal);
}
