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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cBlockStructure_SharedData::cBlockStructure_SharedData()
{
	currentScriptEngine = NULL;
	nBlockID = 0;
	nBlockNumber = 0;
	sBlockName = "Block(ID=" + QString::number(nBlockID) + ",Number=" + QString::number(nBlockNumber) + ")";
	nNrOfTrials = RepeatAmount::RA_ZERO;
	nNrOfInternalTriggers = RepeatAmount::RA_ZERO;
	nNrOfExternalTriggers = RepeatAmount::RA_ZERO;
}

cBlockStructure_SharedData::cBlockStructure_SharedData(const cBlockStructure_SharedData &other) : QSharedData(other)
{
	currentScriptEngine = other.currentScriptEngine;
	nBlockID = other.nBlockID;
	nBlockNumber = other.nBlockNumber;
	sBlockName = other.sBlockName;
	nNrOfTrials = other.nNrOfTrials;
	nNrOfInternalTriggers = other.nNrOfInternalTriggers;
	nNrOfExternalTriggers = other.nNrOfExternalTriggers;

	if(other.lLoops.isEmpty() == false)
	{
		for (int i=0;i<other.lLoops.count();i++)
		{
			lLoops.append(other.lLoops[i]);
		}
	}
}

cBlockStructure_SharedData::~cBlockStructure_SharedData()
{	
	lLoops.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \brief The cBlockStructure constructor.
*
*   No parameter
*/
cBlockStructure::cBlockStructure()
{
	pSharedData = new cBlockStructure_SharedData();
}

cBlockStructure::cBlockStructure(const cBlockStructure& other) : pSharedData(other.pSharedData)
{

}

bool cBlockStructure::Initialize()
{
	pSharedData->nBlockID = 0;
	pSharedData->nBlockNumber = 0;
	pSharedData->sBlockName = "Block(ID=" + QString::number(pSharedData->nBlockID) + ",Number=" + QString::number(pSharedData->nBlockNumber) + ")";
	pSharedData->nNrOfTrials = RepeatAmount::RA_ZERO;
	pSharedData->nNrOfInternalTriggers = RepeatAmount::RA_ZERO;
	pSharedData->nNrOfExternalTriggers = RepeatAmount::RA_ZERO;
	return true;
}

cBlockStructure::cBlockStructure(const int &BlockID,const int &BlockNumber,const QString &BlockName,const int &NumberOfTrials)
{
	pSharedData = new cBlockStructure_SharedData();
	pSharedData->nBlockID = BlockID;
	pSharedData->nBlockNumber = BlockNumber;
	pSharedData->sBlockName = BlockName;
	pSharedData->nNrOfTrials = NumberOfTrials;
	pSharedData->currentScriptEngine = NULL;
	if(pSharedData->sBlockName == "")
		pSharedData->sBlockName = "Block(ID=" + QString::number(pSharedData->nBlockID) + ",Number=" + QString::number(pSharedData->nBlockNumber) + ")";
};

/*! \brief The cBlockStructure destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
cBlockStructure::~cBlockStructure()
{	

}

QScriptValue cBlockStructure::ctor__cBlockStructure(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new cBlockStructure(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

QScriptValue cBlockStructure::blockStructureToScriptValue(QScriptEngine *engine, cBlockStructure* const &s)
{
	QScriptValue obj = engine->newQObject(s);
	return obj;
}

void cBlockStructure::blockStructureFromScriptValue(const QScriptValue &obj, cBlockStructure* &s)
{
	s = qobject_cast<cBlockStructure*>(obj.toQObject());
}

bool cBlockStructure::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		pSharedData->currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = pSharedData->currentScriptEngine->newQObject(this);
		pSharedData->currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool cBlockStructure::isUnusedLoopID(const int &nLoopID) const
{
	if(pSharedData->lLoops.isEmpty())
		return true;
	for (int i = 0; i < pSharedData->lLoops.size(); i++) 
	{
		if (pSharedData->lLoops.at(i).getLoopID() == nLoopID)
			return false;
	}
	return true;
}

bool cBlockStructure::insertLoop(cLoopStructure *cLoop)
{
	if(isUnusedLoopID(cLoop->getLoopID()))
	{
		pSharedData->lLoops.append(cLoopStructure(*cLoop));
		return true;
	}
	return false;
}

cLoopStructure *cBlockStructure::resetToFirstFreeLoopPointer()
{
	if(pSharedData->lLoops.isEmpty() == false)
	{
		int nCount;
		for(int i=0;i<pSharedData->lLoops.count();i++)
		{
			nCount = pSharedData->lLoops.at(i).getLoopCounter();
			if((nCount >= 0) || (nCount==LCE_UNUSED))
				return &pSharedData->lLoops[i];
		}
	}
	return NULL;
}

cLoopStructure *cBlockStructure::incrementToNextLoopPointer(cLoopStructure *pCurrentLoop)
{
	if(pSharedData->lLoops.isEmpty() == false)
	{
		if(pCurrentLoop == NULL)
			return resetToFirstFreeLoopPointer();//just return the first loop
		if(pCurrentLoop->incrementCurrentLoopCounter() >= 0)
		{
			return pCurrentLoop;
		}
		bool bReturnNextPointer = false;
		for(int i=0;i<pSharedData->lLoops.count();i++)
		{
			if(bReturnNextPointer)
			{
				if(pSharedData->lLoops[i].initializeCurrentLoopCounter())//Initialization of the new loop?
					return &pSharedData->lLoops[i];
			}
			if(&pSharedData->lLoops.at(i) == pCurrentLoop)
				bReturnNextPointer = true;
		}
	}
	return NULL;//This means that all loops are processed and we can take the exit
}

void cBlockStructure::resetAllLoopCounters()
{
	if(pSharedData->lLoops.isEmpty() == false)
	{
		for(int i=0;i<pSharedData->lLoops.count();i++)
		{
			pSharedData->lLoops[i].resetCurrentLoopCounter();
		}
	}
}

cLoopStructure* cBlockStructure::getLoopPointerByID(const int &nLoopID)
{
	if(pSharedData->lLoops.isEmpty())//Are there any loops defined?
		return NULL;
	for (int i=0;i<pSharedData->lLoops.size();i++) 
	{
		if(pSharedData->lLoops[i].getLoopID() == nLoopID)
			return &pSharedData->lLoops[i];
	}
	return NULL;
}

cLoopStructure* cBlockStructure::getNextClosestLoopIDByFromID(const int &startLoopID)
{
	//result is an cLoopStructure* with ID=startLoopID or first closest higher value
	if(pSharedData->lLoops.isEmpty())//Are there any loops defined?
		return NULL;
	//First try the expected location
	if(pSharedData->lLoops.size()>startLoopID)
		if (pSharedData->lLoops.at(startLoopID).getLoopID() == startLoopID)
			return &pSharedData->lLoops[startLoopID];
	//Now try the other items
	int closestIndex = -1;
	int closestLoopID = startLoopID;//This shouldn't matter
	int tmpLoopID;
	bool bAcceptAny = true;//Makes sure to accept any valid value from the start of the search
	for (int i=0;i<pSharedData->lLoops.size();i++) 
	{
		tmpLoopID = pSharedData->lLoops.at(i).getLoopID();
		if(tmpLoopID == startLoopID)
		{//We found it although it was not at its expected location
			return &pSharedData->lLoops[i];
		}
		else if(tmpLoopID > startLoopID)
		{//We found a larger block number...
			if(bAcceptAny || (tmpLoopID < closestLoopID))//First one or a more closer(thus smaller) number?
			{
				closestLoopID = tmpLoopID;
				closestIndex = i;
				bAcceptAny = false;
			}
		}
	}
	if(closestIndex>=0)//Do we have an closest result?
		return &pSharedData->lLoops[closestIndex];
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \brief The cLoopStructure constructor.
*
*   No parameter
*/
cLoopStructure::cLoopStructure()
{
	currentScriptEngine = NULL;
	this->Initialize();
	sLoopName = "Loop(ID=" + QString::number(nLoopID) + ",Number=" + QString::number(nLoopNumber) + ")";
}

cLoopStructure::cLoopStructure(const cLoopStructure& other)
{
	currentScriptEngine = other.currentScriptEngine;
	nLoopID = other.nLoopID;
	nLoopNumber = other.nLoopNumber;
	sLoopName = other.sLoopName;
	nNrOfLoops = other.nNrOfLoops;
	nTargetBlockID = other.nTargetBlockID;
	nLoopCounter = other.nLoopCounter;
}

bool cLoopStructure::Initialize()
{
	nLoopID = 0;
	nLoopNumber = 0;
	nLoopCounter = LCE_UNUSED;
	nNrOfLoops = RepeatAmount::RA_ZERO;
	nTargetBlockID = RepeatAmount::RA_UNDEFINED;
	return true;
}

cLoopStructure::cLoopStructure(const int &LoopID,const int &LoopNumber,const int &LoopTargetBlockID,const QString &LoopName,const int &NumberOfLoops) : nLoopID(LoopID), nLoopNumber(LoopNumber), nTargetBlockID(LoopTargetBlockID), sLoopName(LoopName), nNrOfLoops(NumberOfLoops)
{
	currentScriptEngine = NULL;
	nLoopCounter = LCE_UNUSED;
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

QScriptValue cLoopStructure::loopStructureToScriptValue(QScriptEngine *engine, cLoopStructure* const &s)
{
	QScriptValue obj = engine->newQObject(s);
	return obj;
}

void cLoopStructure::loopStructureFromScriptValue(const QScriptValue &obj, cLoopStructure* &s)
{
	s = qobject_cast<cLoopStructure*>(obj.toQObject());
}

bool cLoopStructure::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool cLoopStructure::initializeCurrentLoopCounter()
{
	if(nLoopCounter==LCE_FINISHED)
	{
		return false;
	}
	else if(nLoopCounter==LCE_UNUSED)
	{
		nLoopCounter = LCE_FIRSTLOOP;
		return true;
	}
	return true;
}

int cLoopStructure::incrementCurrentLoopCounter() 
{
	if(nLoopCounter==LCE_FINISHED)
	{
		nLoopCounter = LCE_FINISHED;
	}
	else if(nLoopCounter==LCE_UNUSED)
	{
		nLoopCounter = LCE_FIRSTLOOP;
	}
	else if((nLoopCounter+1) < nNrOfLoops)
	{
		nLoopCounter++;
	}
	else
	{
		nLoopCounter = LCE_FINISHED;
	}
	return nLoopCounter;
}

void cLoopStructure::resetCurrentLoopCounter() 
{
	nLoopCounter = LCE_UNUSED;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

cExperimentStructure_SharedData::cExperimentStructure_SharedData()
{
	currentScriptEngine = NULL;
	currentBlockPointer = NULL;
	currentLoopPointer = NULL;
	firstBlockPointer = NULL;
	nExperimentID = 0;
	sExperimentName = "Experiment " + QString::number(nExperimentID);
	bDebugMode = false;
	CurrentExperiment_RunState = ES_IDLE;
	currentExperimentState.Experiment_ExternalTrigger = RA_REINITIALIZE;
	currentExperimentState.CurrentBlock_BlockID = RA_REINITIALIZE;
	currentExperimentState.CurrentBlock_ExternalTrigger = RA_REINITIALIZE;
	currentExperimentState.CurrentBlock_InternalTrigger = RA_REINITIALIZE;
	currentExperimentState.CurrentBlock_LoopID = RA_REINITIALIZE;
	currentExperimentState.CurrentBlock_TrialNumber = RA_REINITIALIZE;
}

cExperimentStructure_SharedData::cExperimentStructure_SharedData(const cExperimentStructure_SharedData &other) : QSharedData(other)
{
	CurrentExperiment_RunState = other.CurrentExperiment_RunState;
	currentExperimentState = other.currentExperimentState;
	currentScriptEngine = other.currentScriptEngine;
	nExperimentID = other.nExperimentID;
	sExperimentName = other.sExperimentName;
	bDebugMode = other.bDebugMode;
	currentBlockPointer = NULL;
	firstBlockPointer = NULL;
	currentLoopPointer = other.currentLoopPointer;
	if(other.lBlocks.isEmpty() == false)
	{
		for (int i=0;i<other.lBlocks.count();i++)
		{
			lBlocks.append(other.lBlocks[i]);
			if(&other.lBlocks[i] == other.currentBlockPointer)
			{
				currentBlockPointer = &lBlocks[i];
			}
			if(&other.lBlocks[i] == other.firstBlockPointer)
				firstBlockPointer = &lBlocks[i];

		}
	}
}

cExperimentStructure_SharedData::~cExperimentStructure_SharedData()
{	
	lBlocks.clear();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*! \brief The cExperimentStructure constructor.
*
*   No parameter
*/
cExperimentStructure::cExperimentStructure()
{
	pSharedData = new cExperimentStructure_SharedData();
}

cExperimentStructure::cExperimentStructure(const cExperimentStructure& other) : pSharedData(other.pSharedData)
{
	//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
}

/*! \brief The cExperimentStructure destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
cExperimentStructure::~cExperimentStructure()
{	
}

bool cExperimentStructure::Initialize()
{
	pSharedData->nExperimentID = 0;
	pSharedData->sExperimentName = "Experiment " + QString::number(pSharedData->nExperimentID);
	pSharedData->bDebugMode = false;
	pSharedData->CurrentExperiment_RunState = ES_IDLE;
	resetExperiment();
	return true;
}

QScriptValue cExperimentStructure::ctor__cExperimentStructure(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new cExperimentStructure(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
}

QScriptValue cExperimentStructure::experimentStructureToScriptValue(QScriptEngine *engine, cExperimentStructure* const &s)
{
	QScriptValue obj = engine->newQObject(s);
	return obj;
}

void cExperimentStructure::experimentStructureFromScriptValue(const QScriptValue &obj, cExperimentStructure* &s)
{
	s = qobject_cast<cExperimentStructure*>(obj.toQObject());
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

QScriptValue cExperimentStructure::createExperimentStructureStateFromScript(QScriptContext *, QScriptEngine *engine)
{
	strcExperimentStructureState s;
	cExperimentStructure::resetExperimentStateStruct(&s);
	return engine->toScriptValue(s);
}

QScriptValue cExperimentStructure::experimentStructureStateToScriptValue(QScriptEngine *engine, const strcExperimentStructureState &s)
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

void cExperimentStructure::experimentStructureStateFromScriptValue(const QScriptValue &obj, strcExperimentStructureState &s)
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
		pSharedData->currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		QScriptValue objectValue = pSharedData->currentScriptEngine->newQObject(this);
		pSharedData->currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

void cExperimentStructure::ExperimentStop()
{
	if(pSharedData->CurrentExperiment_RunState == ES_RUNNING)
	{
		pSharedData->CurrentExperiment_RunState = ES_IDLE ;
		emit experimentStopped();
	}
}

void cExperimentStructure::ExperimentAbort()
{
	ExperimentStop();
}

void cExperimentStructure::ExperimentStart()
{
	if(pSharedData->CurrentExperiment_RunState == ES_WAITING_FOR_TRIGGER)
	{
		pSharedData->CurrentExperiment_RunState = ES_RUNNING;
		emit experimentStarted();
	}
}

bool cExperimentStructure::isUnusedBlockID(const int &nBlockID) const
{
	if(pSharedData->lBlocks.isEmpty())
		return true;
	for (int i = 0; i < pSharedData->lBlocks.size(); i++) 
	{
		if (pSharedData->lBlocks.at(i).getBlockID() == nBlockID)
			return false;
	}
	return true;
}

cBlockStructure* cExperimentStructure::getNextClosestBlockNumberByFromNumber(const int &startBlockNumber)
{
	//result is an cBlockStructure* with ID=startID or first closest higher value
	if(pSharedData->lBlocks.isEmpty())//Are there any blocks defined?
		return NULL;
	//First try the expected location
	if(pSharedData->lBlocks.size()>startBlockNumber)
		if (pSharedData->lBlocks.at(startBlockNumber).getBlockID() == startBlockNumber)
		{
			return &pSharedData->lBlocks[startBlockNumber];
		}
	//Now try the other items
	int closestIndex = -1;
	int closestBlockNumber = startBlockNumber;//This shouldn't matter
	int tmpBlockNumber;
	bool bAcceptAny = true;//Makes sure to accept any valid value from the start of the search
	for (int i=0;i<pSharedData->lBlocks.size();i++) 
	{
		tmpBlockNumber = pSharedData->lBlocks.at(i).getBlockNumber();
		if(tmpBlockNumber == startBlockNumber)
		{//We found it although it was not at its expected location
			return &pSharedData->lBlocks[i];
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
	{
		return &pSharedData->lBlocks[closestIndex];
	}
	return NULL;
}

int cExperimentStructure::getCurrentBlockIndex() const
{
	if (pSharedData->currentBlockPointer == NULL)
		return OI_UNDEFINED;
	if (pSharedData->lBlocks.isEmpty())
		return OI_UNDEFINED;
	for (int i=0;i<pSharedData->lBlocks.count();i++)
	{
		if(&pSharedData->lBlocks[i] == pSharedData->currentBlockPointer)
		{
			return i;
		}
	}
	return OI_UNDEFINED;
}

cBlockStructure cExperimentStructure::getCurrentBlock(bool &bHasCurrBlock) const
{
	if(pSharedData->lBlocks.isEmpty() == false)
	{
		int nFoundIndex = getCurrentBlockIndex();
		if(nFoundIndex >= 0)
		{
			bHasCurrBlock = true;
			return pSharedData->lBlocks[nFoundIndex];
		}
	}
	bHasCurrBlock = false;
	return cBlockStructure();
}

bool cExperimentStructure::prepareStartBlock()
{
	if(isValidBlockPointer(pSharedData->firstBlockPointer) == false)
	{
		if(pSharedData->lBlocks.isEmpty() == false)
		{
			pSharedData->firstBlockPointer = getNextClosestBlockNumberByFromNumber(0);//Here we just take the block with the lowest block number
			if(pSharedData->firstBlockPointer == NULL)
			{
				return false;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool cExperimentStructure::isValidBlockPointer(cBlockStructure *cBlock) const
{
	if(cBlock == NULL)
		return false;
	if(pSharedData->lBlocks.isEmpty())//Are there any blocks defined?
		return false;
	for (int i=0;i<pSharedData->lBlocks.size();i++) 
	{
		if(&pSharedData->lBlocks[i] == cBlock)
			return true;
	}
	return false;
}

bool cExperimentStructure::insertBlock(cBlockStructure *cBlock)
{
	if(isUnusedBlockID(cBlock->getBlockID()))
	{
		pSharedData->lBlocks.append(*cBlock);
		if(pSharedData->lBlocks.count()==1)//First block to append?
			pSharedData->firstBlockPointer = &pSharedData->lBlocks[0];
		return true;
	}
	return false;
}

int cExperimentStructure::getBlockIndexByID(const int &nBlockID) const
{
	if(pSharedData->lBlocks.isEmpty())//Are there any blocks defined?
		return -1;
	for (int i=0;i<pSharedData->lBlocks.size();i++) 
	{
		if(pSharedData->lBlocks.at(i).getBlockID() == nBlockID)
			return i;
	}
	return -1;
}

cBlockStructure* cExperimentStructure::getBlockPointerByID(const int &nBlockID)
{
	int nIndex = getBlockIndexByID(nBlockID);
	if(nIndex >= 0)
		return &pSharedData->lBlocks[nIndex];
	return NULL;
}

int cExperimentStructure::getBlockCount() const 
{
	return pSharedData->lBlocks.count();
}

bool cExperimentStructure::prepareExperiment()
{
	if(prepareStartBlock())
	{
		pSharedData->currentBlockPointer = pSharedData->firstBlockPointer;
		pSharedData->currentLoopPointer = pSharedData->currentBlockPointer->resetToFirstFreeLoopPointer();
		pSharedData->currentExperimentState.Experiment_ExternalTrigger = RA_REINITIALIZE;
		pSharedData->currentExperimentState.CurrentBlock_BlockID = pSharedData->currentBlockPointer->getBlockID();
		pSharedData->currentExperimentState.CurrentBlock_ExternalTrigger = RA_REINITIALIZE;
		pSharedData->currentExperimentState.CurrentBlock_InternalTrigger = RA_REINITIALIZE;
		if(pSharedData->currentLoopPointer == NULL)
			pSharedData->currentExperimentState.CurrentBlock_LoopID = RA_UNDEFINED;
		else
			pSharedData->currentExperimentState.CurrentBlock_LoopID = pSharedData->currentLoopPointer->getLoopID();
		pSharedData->currentExperimentState.CurrentBlock_TrialNumber = RA_REINITIALIZE;
		pSharedData->CurrentExperiment_RunState = ES_WAITING_FOR_TRIGGER;
		return true;
	}
	return false;
}

int cExperimentStructure::getExternalTriggerCount() const
{
	return pSharedData->currentExperimentState.Experiment_ExternalTrigger;
}

void cExperimentStructure::incrementExternalTrigger()
{
	if(pSharedData->CurrentExperiment_RunState == ES_IDLE)
		return;
	if(pSharedData->CurrentExperiment_RunState == ES_WAITING_FOR_TRIGGER)//currentExperimentState.Experiment_ExternalTrigger == RA_REINITIALIZE)//First external experiment trigger!
	{
		if(pSharedData->currentExperimentState.CurrentBlock_BlockID == RA_REINITIALIZE)
		{
			if(prepareExperiment() == false)
				return;
			pSharedData->currentExperimentState.CurrentBlock_BlockID = pSharedData->currentBlockPointer->getBlockID();
		}
		if(pSharedData->currentExperimentState.CurrentBlock_LoopID == RA_REINITIALIZE)
		{
			pSharedData->currentLoopPointer = pSharedData->currentBlockPointer->resetToFirstFreeLoopPointer();
			if(pSharedData->currentLoopPointer == NULL)
				pSharedData->currentExperimentState.CurrentBlock_LoopID = RA_UNDEFINED;
			else
				pSharedData->currentExperimentState.CurrentBlock_LoopID = pSharedData->currentLoopPointer->getLoopID();
		}
		pSharedData->currentExperimentState.Experiment_ExternalTrigger = 0;
		pSharedData->currentExperimentState.CurrentBlock_ExternalTrigger = 0;
		pSharedData->currentExperimentState.CurrentBlock_InternalTrigger = 0;
		pSharedData->currentExperimentState.CurrentBlock_TrialNumber = 0;
		ExperimentStart();
	}
	else if(pSharedData->CurrentExperiment_RunState == ES_RUNNING)
	{
		int BlockExtTriggerAmount = pSharedData->currentBlockPointer->getNumberOfExternalTriggers();
		int BlockIntTriggerAmount = pSharedData->currentBlockPointer->getNumberOfInternalTriggers();
		int BlockTrialAmount = pSharedData->currentBlockPointer->getNumberOfTrials();
		cLoopStructure *nextLoopBlock = NULL;
		if((pSharedData->currentExperimentState.CurrentBlock_ExternalTrigger + 1) >= BlockExtTriggerAmount)
		{
			pSharedData->currentExperimentState.CurrentBlock_ExternalTrigger = 0;//Reset to zero again!
			if((pSharedData->currentExperimentState.CurrentBlock_InternalTrigger + 1) >= BlockIntTriggerAmount)
			{
				pSharedData->currentExperimentState.CurrentBlock_InternalTrigger = 0;//Reset to zero again!
				if((pSharedData->currentExperimentState.CurrentBlock_TrialNumber + 1) >= BlockTrialAmount)
				{//Next Block?
					pSharedData->currentExperimentState.CurrentBlock_TrialNumber = 0;//Reset to zero again!
					nextLoopBlock = pSharedData->currentBlockPointer->incrementToNextLoopPointer(pSharedData->currentLoopPointer);//Do we have a Block defined by a loop?
					if(nextLoopBlock == NULL)//No Loop Block available
					{
						pSharedData->currentBlockPointer->resetAllLoopCounters();
						int nFoundIndex = -1;
						pSharedData->currentBlockPointer = getNextClosestBlockNumberByFromNumber(pSharedData->currentBlockPointer->getBlockNumber()+1);
						if(pSharedData->currentBlockPointer == NULL)
						{
							ExperimentStop();
							return;					
						}
						pSharedData->currentLoopPointer = pSharedData->currentBlockPointer->incrementToNextLoopPointer(NULL);
					}
					else
					{
						int nTargetBlockID = nextLoopBlock->getTargetBlockID();
						pSharedData->currentBlockPointer = getBlockPointerByID(nTargetBlockID);
						if(pSharedData->currentBlockPointer == NULL)
						{
							ExperimentAbort();
							return;					
						}
						pSharedData->currentLoopPointer = pSharedData->currentBlockPointer->resetToFirstFreeLoopPointer();
					}
					pSharedData->currentExperimentState.CurrentBlock_BlockID = pSharedData->currentBlockPointer->getBlockID();
					if(pSharedData->currentLoopPointer)
						pSharedData->currentExperimentState.CurrentBlock_LoopID = pSharedData->currentLoopPointer->getLoopID();
					else
						pSharedData->currentExperimentState.CurrentBlock_LoopID = RA_UNDEFINED;
				}
				else
				{
					pSharedData->currentExperimentState.CurrentBlock_TrialNumber++;//Increment with 1
				}
			}
			else
			{
				pSharedData->currentExperimentState.CurrentBlock_InternalTrigger++;//Increment with 1
			}
		}
		else
		{
			pSharedData->currentExperimentState.CurrentBlock_ExternalTrigger++;//Increment with 1
		}
		pSharedData->currentExperimentState.Experiment_ExternalTrigger++;
	}
	emit externalTriggerRecieved();
}