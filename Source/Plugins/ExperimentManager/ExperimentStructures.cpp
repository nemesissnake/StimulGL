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


#include "ExperimentStructures.h"

/*! \brief The cBlockStructure constructor.
*
*   No parameter
*/
cBlockStructure::cBlockStructure(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	this->Initialize();
}

bool cBlockStructure::Initialize()
{
	nInternalLoopCounter = 0;
	nBlockID = 0;
	nBlockNumber = 0;
	sBlockName = "Block(ID=" + QString::number(nBlockID) + ",Number=" + QString::number(nBlockNumber) + ")";
	nNrOfTrials = RepeatAmount::RA_ZERO;
	nNrOfInternalTriggers = RepeatAmount::RA_ZERO;
	nNrOfExternalTriggers = RepeatAmount::RA_ZERO;
	return true;
}

cBlockStructure::cBlockStructure(const int &BlockID,const int &BlockNumber,const QString &BlockName,const int &NumberOfTrials) : nBlockID(BlockID), nBlockNumber(BlockNumber), sBlockName(BlockName), nNrOfTrials(NumberOfTrials)
{
	currentScriptEngine = NULL;
	if(sBlockName == "")
		sBlockName = "Block(ID=" + QString::number(nBlockID) + ",Number=" + QString::number(nBlockNumber) + ")";
};

/*! \brief The cBlockStructure destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
cBlockStructure::~cBlockStructure()
{	
	if(lLoops.isEmpty() == false)
	{
		for(int i=0;i<lLoops.count();i++)
		{
			if(lLoops[i])
			{
				delete lLoops[i];
				lLoops[i] = NULL;
			}
		}
		lLoops.clear();
	}
}

QScriptValue cBlockStructure::ctor__cBlockStructure(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new cBlockStructure(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

bool cBlockStructure::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool cBlockStructure::isUnusedLoopID(const int &nLoopID)
{
	if(lLoops.isEmpty())
		return true;
	for (int i = 0; i < lLoops.size(); i++) 
	{
		if (lLoops.at(i)->getLoopID() == nLoopID)
			return false;
	}
	return true;
}

bool cBlockStructure::insertLoop(cLoopStructure *cLoop)
{
	if(isUnusedLoopID(cLoop->getLoopID()))
	{
		lLoops.append(cLoop);
		//if(lLoops.count()==1)//First loop to append?
		//	firstBlockPointer = lBlocks[0];
		return true;
	}
	return false;
}

cLoopStructure *cBlockStructure::resetToFirstLoopPointer()
{
	nInternalLoopCounter = 0;
	if(lLoops.isEmpty() == false)
	{
		nInternalLoopCounter = lLoops[0]->getCurrentLoopCounter();
		return lLoops[0];//just return the first loop
	}
	return NULL;
}

cLoopStructure *cBlockStructure::incrementToNextLoopPointer(cLoopStructure *pCurrentLoop)
{
	if(lLoops.isEmpty() == false)
	{
		if(pCurrentLoop == NULL)
			return resetToFirstLoopPointer();//just return the first loop
		nInternalLoopCounter = pCurrentLoop->incrementCurrentLoopCounter();
		if(nInternalLoopCounter >= 0)
		{
			return pCurrentLoop;
		}
		bool bReturnNextPointer = false;
		for(int i=0;i<lLoops.count();i++)
		{
			if(bReturnNextPointer)
				return lLoops[i];
			if(lLoops[i] == pCurrentLoop)
				bReturnNextPointer = true;
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \brief The cLoopStructure constructor.
*
*   No parameter
*/
cLoopStructure::cLoopStructure(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	this->Initialize();
}

bool cLoopStructure::Initialize()
{
	nLoopID = 0;
	nLoopNumber = 0;
	nCurrentLoop = 0;
	sLoopName = "Loop(ID=" + QString::number(nLoopID) + ",Number=" + QString::number(nLoopNumber) + ")";
	nNrOfLoops = RepeatAmount::RA_ZERO;
	nTargetBlockID = RepeatAmount::RA_UNDEFINED;
	return true;
}

cLoopStructure::cLoopStructure(const int &LoopID,const int &LoopNumber,const QString &LoopName,const int &NumberOfLoops) : nLoopID(LoopID), nLoopNumber(LoopNumber), sLoopName(LoopName), nNrOfLoops(NumberOfLoops)
{
	currentScriptEngine = NULL;
	if(sLoopName == "")
		sLoopName = "Loop(ID=" + QString::number(nLoopID) + ",Number=" + QString::number(nLoopNumber) + ")";
};

/*! \brief The cLoopStructure destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
cLoopStructure::~cLoopStructure()
{	

}

QScriptValue cLoopStructure::ctor__cLoopStructure(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new cLoopStructure(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

bool cLoopStructure::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \brief The cExperimentStructure constructor.
*
*   No parameter
*/
cExperimentStructure::cExperimentStructure(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	currentBlockPointer = NULL;
	currentLoopPointer = NULL;
	firstBlockPointer = NULL;
	this->Initialize();
}

/*! \brief The cExperimentStructure destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
cExperimentStructure::~cExperimentStructure()
{	
	if(lBlocks.isEmpty() == false)
	{
		for(int i=0;i<lBlocks.count();i++)
		{
			if(lBlocks[i])
			{
				delete lBlocks[i];
				lBlocks[i] = NULL;
			}
		}
		lBlocks.clear();
	}
}

bool cExperimentStructure::Initialize()
{
	nExperimentID = 0;
	//nExperimentNumber = 0;
	sExperimentName = "Experiment " + QString::number(nExperimentID);
	bDebugMode = false;
	resetExperiment();
	return true;
}

QScriptValue cExperimentStructure::ctor__cExperimentStructure(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new cExperimentStructure(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

void cExperimentStructure::resetExperimentStateStruct(strcExperimentStructureState *strcExpState)
{
	if(strcExpState == NULL)
		return;
	strcExpState->Experiment_ExternalTrigger = RA_REINITIALIZE;
	strcExpState->CurrentBlock_BlockID = RA_REINITIALIZE;
	strcExpState->CurrentBlock_ExternalTrigger = RA_REINITIALIZE;
	strcExpState->CurrentBlock_InternalTrigger = RA_REINITIALIZE;
	strcExpState->CurrentBlock_LoopID = RA_REINITIALIZE;
	strcExpState->CurrentBlock_TrialNumber = RA_REINITIALIZE;
};

QScriptValue cExperimentStructure::CreateExperimentStructureStateFromScript(QScriptContext *, QScriptEngine *engine)
{
	strcExperimentStructureState s;
	cExperimentStructure::resetExperimentStateStruct(&s);
	return engine->toScriptValue(s);
}

QScriptValue cExperimentStructure::ExperimentStructureStateToScriptValue(QScriptEngine *engine, const strcExperimentStructureState &s)
{
	QScriptValue obj = engine->newObject();
	obj.setProperty("Experiment_ExternalTrigger", s.Experiment_ExternalTrigger);
	obj.setProperty("CurrentBlock_BlockID", s.CurrentBlock_BlockID);
	obj.setProperty("CurrentBlock_ExternalTrigger", s.CurrentBlock_ExternalTrigger);
	obj.setProperty("CurrentBlock_InternalTrigger", s.CurrentBlock_InternalTrigger);
	obj.setProperty("CurrentBlock_LoopID", s.CurrentBlock_LoopID);
	obj.setProperty("CurrentBlock_TrialNumber", s.CurrentBlock_TrialNumber);
	return obj;
}

void cExperimentStructure::ExperimentStructureStateFromScriptValue(const QScriptValue &obj, strcExperimentStructureState &s)
{
	s.Experiment_ExternalTrigger = obj.property("Experiment_ExternalTrigger").toInt32();
	s.CurrentBlock_BlockID = obj.property("CurrentBlock_BlockID").toInt32();
	s.CurrentBlock_ExternalTrigger = obj.property("CurrentBlock_ExternalTrigger").toInt32();
	s.CurrentBlock_InternalTrigger = obj.property("CurrentBlock_InternalTrigger").toInt32();
	s.CurrentBlock_LoopID = obj.property("CurrentBlock_LoopID").toInt32();
	s.CurrentBlock_TrialNumber = obj.property("CurrentBlock_TrialNumber").toInt32();
}

bool cExperimentStructure::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

void cExperimentStructure::ExperimentStop()
{
	if(CurrentExperiment_State == ES_RUNNING)
	{
		CurrentExperiment_State = ES_IDLE ;
		emit experimentStopped();
	}
}

void cExperimentStructure::ExperimentAbort()
{
	ExperimentStop();
}

void cExperimentStructure::ExperimentStart()
{
	if(CurrentExperiment_State == ES_WAITING_FOR_TRIGGER)
	{
		CurrentExperiment_State = ES_RUNNING;
		emit experimentStarted();
	}
}

bool cExperimentStructure::isUnusedBlockID(const int &nBlockID)
{
	if(lBlocks.isEmpty())
		return true;
	for (int i = 0; i < lBlocks.size(); i++) 
	{
		if (lBlocks.at(i)->getBlockID() == nBlockID)
			return false;
	}
	return true;
}

cBlockStructure* cExperimentStructure::getNextClosestBlockNumberByFromNumber(const int &startBlockNumber)
{
	//result is an cBlockStructure* with ID=startID or first closest higher value
	if(lBlocks.isEmpty())//Are there any blocks defined?
		return NULL;
	//First try the expected location
	if(lBlocks.size()>startBlockNumber)
		if (lBlocks.at(startBlockNumber)->getBlockID() == startBlockNumber)
			return lBlocks[startBlockNumber];
	//Now try the other items
	int closestIndex = -1;
	int closestBlockNumber = startBlockNumber;//This shouldn't matter
	int tmpBlockNumber;
	bool bAcceptAny = true;//Makes sure to accept any valid value from the start of the search
	for (int i=0;i<lBlocks.size();i++) 
	{
		tmpBlockNumber = lBlocks.at(i)->getBlockNumber();
		if(tmpBlockNumber == startBlockNumber)
		{//We found it although it was not at its expected location
			return lBlocks[i];
		}
		else if(tmpBlockNumber > startBlockNumber)
		{//We found a larger block number...
			if(bAcceptAny || (tmpBlockNumber < closestBlockNumber))//First one or a more closer(thus smaller) number?
			{
				closestBlockNumber = tmpBlockNumber;
				closestIndex = i;
				bAcceptAny = false;
			}
		}
	}
	if(closestIndex>=0)//Do we have an closest result?
		return lBlocks[closestIndex];
	else
		return NULL;
}

bool cExperimentStructure::prepareStartBlock()
{
	if(isValidBlockPointer(firstBlockPointer) == false)
	{
		if(lBlocks.isEmpty() == false)
		{
			firstBlockPointer = getNextClosestBlockNumberByFromNumber(0);//Here we just take the block with the lowest block number
			return !(firstBlockPointer == NULL);
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool cExperimentStructure::isValidBlockPointer(cBlockStructure *cBlock)
{
	if(cBlock == NULL)
		return false;
	if(lBlocks.isEmpty())//Are there any blocks defined?
		return false;
	for (int i=0;i<lBlocks.size();i++) 
	{
		if(lBlocks[i] == cBlock)
			return true;
	}
	return false;
}

bool cExperimentStructure::insertBlock(cBlockStructure *cBlock)
{
	if(isUnusedBlockID(cBlock->getBlockID()))
	{
		lBlocks.append(cBlock);
		if(lBlocks.count()==1)//First block to append?
			firstBlockPointer = lBlocks[0];
		return true;
	}
	return false;
}

cBlockStructure* cExperimentStructure::getBlockPointerByID(const int &nBlockID)
{
	if(lBlocks.isEmpty())//Are there any blocks defined?
		return NULL;
	for (int i=0;i<lBlocks.size();i++) 
	{
		if(lBlocks.at(i)->getBlockID() == nBlockID)
			return lBlocks[i];
	}
	return false;
}

bool cExperimentStructure::prepareExperiment()
{
	if(prepareStartBlock())
	{
		currentBlockPointer = firstBlockPointer;
		currentLoopPointer = currentBlockPointer->resetToFirstLoopPointer();
		currentExperimentState.Experiment_ExternalTrigger = RA_REINITIALIZE;
		currentExperimentState.CurrentBlock_BlockID = currentBlockPointer->getBlockID();
		currentExperimentState.CurrentBlock_ExternalTrigger = RA_REINITIALIZE;
		currentExperimentState.CurrentBlock_InternalTrigger = RA_REINITIALIZE;
		if(currentLoopPointer == NULL)
			currentExperimentState.CurrentBlock_LoopID = RA_UNDEFINED;
		else
			currentExperimentState.CurrentBlock_LoopID = currentLoopPointer->getLoopID();
		currentExperimentState.CurrentBlock_TrialNumber = RA_REINITIALIZE;
		CurrentExperiment_State = ES_WAITING_FOR_TRIGGER;
		return true;
	}
	return false;
}

void cExperimentStructure::incrementExternalTrigger()
{
	if(CurrentExperiment_State == ES_IDLE)
		return;
	if(CurrentExperiment_State == ES_WAITING_FOR_TRIGGER)//currentExperimentState.Experiment_ExternalTrigger == RA_REINITIALIZE)//First external experiment trigger!
	{
		if(currentExperimentState.CurrentBlock_BlockID == RA_REINITIALIZE)
		{
			if(prepareExperiment() == false)
				return;
			currentExperimentState.CurrentBlock_BlockID = currentBlockPointer->getBlockID();
		}
		if(currentExperimentState.CurrentBlock_LoopID == RA_REINITIALIZE)
		{
			currentLoopPointer = currentBlockPointer->resetToFirstLoopPointer();
			if(currentLoopPointer == NULL)
				currentExperimentState.CurrentBlock_LoopID = RA_UNDEFINED;
			else
				currentExperimentState.CurrentBlock_LoopID = currentLoopPointer->getLoopID();
		}
		currentExperimentState.Experiment_ExternalTrigger = 0;
		currentExperimentState.CurrentBlock_ExternalTrigger = 0;
		currentExperimentState.CurrentBlock_InternalTrigger = 0;
		currentExperimentState.CurrentBlock_TrialNumber = 0;
		ExperimentStart();
	}
	else if(CurrentExperiment_State == ES_RUNNING)
	{
		int BlockExtTriggerAmount = currentBlockPointer->getNumberOfExternalTriggers();
		int BlockIntTriggerAmount = currentBlockPointer->getNumberOfInternalTriggers();
		int BlockTrialAmount = currentBlockPointer->getNumberOfTrials();
		cLoopStructure *nextLoopBlock = NULL;
		if((currentExperimentState.CurrentBlock_ExternalTrigger + 1) >= BlockExtTriggerAmount)
		{
			currentExperimentState.CurrentBlock_ExternalTrigger = 0;//Reset to zero again!
			if((currentExperimentState.CurrentBlock_InternalTrigger + 1) >= BlockIntTriggerAmount)
			{
				currentExperimentState.CurrentBlock_InternalTrigger = 0;//Reset to zero again!
				if((currentExperimentState.CurrentBlock_TrialNumber + 1) >= BlockTrialAmount)
				{//Next Block?
					currentExperimentState.CurrentBlock_TrialNumber = 0;//Reset to zero again!
					nextLoopBlock = currentBlockPointer->incrementToNextLoopPointer(currentLoopPointer);//Do we have a Block defined by a loop?
					if(nextLoopBlock == NULL)//No Loop Block available
					{
						currentBlockPointer = getNextClosestBlockNumberByFromNumber(currentBlockPointer->getBlockNumber()+1);
						if(currentBlockPointer == NULL)
						{
							ExperimentStop();
							return;					
						}
						currentLoopPointer = currentBlockPointer->incrementToNextLoopPointer(nextLoopBlock);
					}
					else
					{
						int nTargetBlockID = nextLoopBlock->getTargetBlockID();
						currentBlockPointer = getBlockPointerByID(nTargetBlockID);
						if(currentBlockPointer == NULL)
						{
							ExperimentAbort();
							return;					
						}
						currentLoopPointer = currentBlockPointer->resetToFirstLoopPointer();
					}
					currentExperimentState.CurrentBlock_BlockID = currentBlockPointer->getBlockID();
					if(currentLoopPointer)
						currentExperimentState.CurrentBlock_LoopID = currentLoopPointer->getLoopID();
					else
						currentExperimentState.CurrentBlock_LoopID = RA_UNDEFINED;
				}
				else
				{
					currentExperimentState.CurrentBlock_TrialNumber++;//Increment with 1
				}
			}
			else
			{
				currentExperimentState.CurrentBlock_InternalTrigger++;//Increment with 1
			}
		}
		else
		{
			currentExperimentState.CurrentBlock_ExternalTrigger++;//Increment with 1
		}
		currentExperimentState.Experiment_ExternalTrigger++;
	}
	emit externalTriggerRecieved();

	////currentExperimentState.InternalTrigger = 0;to be removed because this depends on the block definition 
	//
	//currentExperimentState.CurrentBlock_BlockID = cTmpBlock->getBlockID();
	//currentExperimentState.CurrentBlock_ExternalTrigger = 0;
	//currentExperimentState.CurrentBlock_InternalTrigger = 0;
	//currentExperimentState.CurrentBlock_LoopID = RA_REINITIALIZE;
	//currentExperimentState.CurrentBlock_TrialNumber = RA_REINITIALIZE;		
}