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


#ifndef EXPERIMENTSTRUCTURES_H
#define EXPERIMENTSTRUCTURES_H

#include <QtScript>
#include <QList>
#include <QSharedData>
#include "maindefines.h"

//*! The enum (RepeatAmount) can store the remaining amount of trials or action to take */
enum RepeatAmount
{
	RA_ZERO							=  0,	//!<  0: Zero Repeats
	RA_FINISHED						= -1,	//!< -1: Repeats have all been processed/finished
	RA_REINITIALIZE					= -2,	//!< -2: Repeats should be re-initialized first
	RA_INFINITE						= -3,	//!< -3: Infinite Repeats
	RA_UNDEFINED					= -4	//!< -4: None defined (ie. no loops defined)
};

enum LoopCounterException
{
	LCE_FIRSTLOOP					=  0,	//!<   0: This is the first loop
	LCE_UNUSED						= -1,	//!<  -1: This loop is fully processed
	LCE_FINISHED					= -2	//!<  -2: This loop is not yet used
};

enum ExperimentRunState
{
	ES_IDLE							=  0,	//!<  0: IDLE STATE
	ES_WAITING_FOR_TRIGGER			=  1,	//!<  1: WAITING FOR TRIGGER
	ES_RUNNING						=  2	//!<  2: RUNNING
};

enum ObjectIndex
{
	OI_UNDEFINED					= -1
};

enum CounterFlag
{
	CF_UNINITIALIZED				= -1
};

typedef struct strcExperimentStructureState
{
	int Experiment_ExternalTrigger;
	int CurrentBlock_BlockID;
	int CurrentBlock_LoopID;
	int CurrentBlock_TrialNumber;
	int CurrentBlock_InternalTrigger;
	int CurrentBlock_ExternalTrigger;
} ExperimentStructureState;

class cLoopStructure : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int LoopID WRITE setLoopID READ getLoopID)
	Q_PROPERTY(int LoopNumber WRITE setLoopNumber READ getLoopNumber)
	Q_PROPERTY(QString LoopName WRITE setLoopName READ getLoopName)
	Q_PROPERTY(int NumberOfLoops WRITE setNumberOfLoops READ getNumberOfLoops)
	Q_PROPERTY(int TargetBlockID WRITE setTargetBlockID READ getTargetBlockID)
	Q_PROPERTY(int LoopCounter READ getTargetBlockID)

public:
	cLoopStructure();
	cLoopStructure(const int &LoopID,const int &LoopNumber,const QString &LoopName = "",const int &NumberOfLoops = RepeatAmount::RA_ZERO);
	cLoopStructure(const cLoopStructure& other);
	~cLoopStructure();

	static QScriptValue ctor__cLoopStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue loopStructureToScriptValue(QScriptEngine *engine, cLoopStructure* const &s);
	static void loopStructureFromScriptValue(const QScriptValue &obj, cLoopStructure* &s);

	bool initializeCurrentLoopCounter();
	int incrementCurrentLoopCounter();
	void resetCurrentLoopCounter();

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	void setLoopID(const int &nValue) {nLoopID = nValue;};
	int getLoopID() const {return nLoopID;};
	void setLoopNumber(const int &nValue) {nLoopNumber = nValue;};
	int getLoopNumber() const {return nLoopNumber;};
	void setLoopName(const QString &sValue) {sLoopName = sValue;};
	QString getLoopName() const {return sLoopName;};
	void setNumberOfLoops(const int &nValue) {nNrOfLoops = nValue;};
	int getNumberOfLoops() const {return nNrOfLoops;};
	void setTargetBlockID(const int &nValue) {nTargetBlockID = nValue;};
	int getTargetBlockID() const {return nTargetBlockID;};
	int getLoopCounter() const {return nLoopCounter;};

private:
	bool Initialize();

	QScriptEngine* currentScriptEngine;
	int nLoopID;
	int nLoopNumber;
	QString sLoopName;
	int nNrOfLoops;
	int nTargetBlockID;
	int nLoopCounter;
};

class cBlockStructure_SharedData : public QSharedData
{
public:
	cBlockStructure_SharedData();
	cBlockStructure_SharedData(const cBlockStructure_SharedData &other);
	~cBlockStructure_SharedData();

	QScriptEngine* currentScriptEngine;
	int nBlockID;
	int nBlockNumber;
	QString sBlockName;
	int nNrOfTrials;
	int nNrOfInternalTriggers;
	int nNrOfExternalTriggers;
	QList<cLoopStructure> lLoops;
};

class cBlockStructure : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int BlockID WRITE setBlockID READ getBlockID)
	Q_PROPERTY(int BlockNumber WRITE setBlockNumber READ getBlockNumber)
	Q_PROPERTY(QString BlockName WRITE setBlockName READ getBlockName)
	Q_PROPERTY(int NumberOfTrials WRITE setNumberOfTrials READ getNumberOfTrials)
	Q_PROPERTY(int NumberOfInternalTriggers WRITE setNumberOfInternalTriggers READ getNumberOfInternalTriggers)
	Q_PROPERTY(int NumberOfExternalTriggers WRITE setNumberOfExternalTriggers READ getNumberOfExternalTriggers)

public:
	cBlockStructure();
	cBlockStructure(const int &BlockID,const int &BlockNumber,const QString &BlockName = "",const int &NumberOfTrials = RepeatAmount::RA_ZERO);
	cBlockStructure(const cBlockStructure& other);
	~cBlockStructure();

	static QScriptValue ctor__cBlockStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue blockStructureToScriptValue(QScriptEngine *engine, cBlockStructure* const &s);
	static void blockStructureFromScriptValue(const QScriptValue &obj, cBlockStructure* &s);
	
public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	void setBlockID(const int &nValue) {pSharedData->nBlockID = nValue;};
	int getBlockID() const {return pSharedData->nBlockID;};
	void setBlockNumber(const int &nValue) {pSharedData->nBlockNumber = nValue;};
	int getBlockNumber() const {return pSharedData->nBlockNumber;};
	void setBlockName(const QString &sValue) {pSharedData->sBlockName = sValue;};
	QString getBlockName() const {return pSharedData->sBlockName;};
	void setNumberOfTrials(const int &nValue) {pSharedData->nNrOfTrials = nValue;};
	int getNumberOfTrials() const {return pSharedData->nNrOfTrials;};
	void setNumberOfInternalTriggers(const int &nValue) {pSharedData->nNrOfInternalTriggers = nValue;};
	int getNumberOfInternalTriggers() const {return pSharedData->nNrOfInternalTriggers;};
	void setNumberOfExternalTriggers(const int &nValue) {pSharedData->nNrOfExternalTriggers = nValue;};
	int getNumberOfExternalTriggers() const {return pSharedData->nNrOfExternalTriggers;};

	cLoopStructure *incrementToNextLoopPointer(cLoopStructure *pCurrentLoop = NULL);
	cLoopStructure *resetToFirstFreeLoopPointer();
	void resetAllLoopCounters();
	bool insertLoop(cLoopStructure *cLoop);
	cLoopStructure* getNextClosestLoopIDByFromID(const int &startLoopID);
	cLoopStructure* getLoopPointerByID(const int &nLoopID);
	int getLoopCount() const {return pSharedData->lLoops.count();};

private:
	bool Initialize();
	bool isUnusedLoopID(const int &nLoopID) const;

	QSharedDataPointer<cBlockStructure_SharedData> pSharedData;
};


class cExperimentStructure_SharedData : public QSharedData
{
public:
	cExperimentStructure_SharedData();
	cExperimentStructure_SharedData(const cExperimentStructure_SharedData &other);
	~cExperimentStructure_SharedData();

	ExperimentRunState CurrentExperiment_RunState;			//Only for internal usage
	strcExperimentStructureState currentExperimentState;
	QScriptEngine* currentScriptEngine;
	int nExperimentID;
	QString sExperimentName;
	bool bDebugMode;
	QList<cBlockStructure> lBlocks;
	cBlockStructure *currentBlockPointer;					//Only for internal usage
	cLoopStructure *currentLoopPointer;						//Only for internal usage
	cBlockStructure *firstBlockPointer;						//Only for internal usage
};


//!  The cExperimentStructure class. 
/*!
  TODO
*/
class cExperimentStructure : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "cExperimentStructure");//Can't use defines here!, moc doesn't handle defines, not needed here
	Q_PROPERTY(QString ExperimentName WRITE setExperimentName READ getExperimentName)
	Q_PROPERTY(int ExperimentID WRITE setExperimentID READ getExperimentID)
	Q_PROPERTY(bool ExperimentDebugMode WRITE setExperimentDebugMode READ getExperimentDebugMode)

signals:
	void experimentStarted();
	void externalTriggerRecieved();
	void experimentStopped();

public:
	cExperimentStructure();
	cExperimentStructure(const cExperimentStructure& other);
	~cExperimentStructure();

	static QScriptValue ctor__cExperimentStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue experimentStructureToScriptValue(QScriptEngine *engine, cExperimentStructure* const &s);
	static void experimentStructureFromScriptValue(const QScriptValue &obj, cExperimentStructure* &s);
	static QScriptValue createExperimentStructureStateFromScript(QScriptContext *, QScriptEngine *engine);
	static QScriptValue experimentStructureStateToScriptValue(QScriptEngine *engine, const strcExperimentStructureState &s);
	static void resetExperimentStateStruct(strcExperimentStructureState *strcExpState);
	static void experimentStructureStateFromScriptValue(const QScriptValue &obj, strcExperimentStructureState &s);
	cBlockStructure* getNextClosestBlockNumberByFromNumber(const int &startBlockNumber); 
	cBlockStructure getCurrentBlock(bool &bHasCurrBlock) const;//This doesn't work within the script, see reference...

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	void resetExperiment() {resetExperimentStateStruct(&pSharedData->currentExperimentState);};
	strcExperimentStructureState getCurrentExperimentState() const {return pSharedData->currentExperimentState;};

	void setExperimentName(const QString &sValue) {pSharedData->sExperimentName = sValue;};
	QString getExperimentName() const {return pSharedData->sExperimentName;};
	void setExperimentID(const int &nValue) {pSharedData->nExperimentID = nValue;};
	int getExperimentID() const {return pSharedData->nExperimentID;};
	void setExperimentDebugMode(const bool &bValue) {pSharedData->bDebugMode = bValue;};
	bool getExperimentDebugMode() const {return pSharedData->bDebugMode;};

	void incrementExternalTrigger();
	bool prepareExperiment();
	void startExperiment() {ExperimentStart();};
	bool insertBlock(cBlockStructure *cBlock);
	cBlockStructure* getBlockPointerByID(const int &nBlockID);
	int getBlockCount() const;
	int getExternalTriggerCount() const;

private:
	void ExperimentAbort();
	void ExperimentStop();
	void ExperimentStart();
	bool Initialize();
	bool isUnusedBlockID(const int &nBlockID) const;
	bool isValidBlockPointer(cBlockStructure *cBlock) const;
	bool prepareStartBlock();
	int getBlockIndexByID(const int &nBlockID) const;
	int getCurrentBlockIndex() const;

	QSharedDataPointer<cExperimentStructure_SharedData> pSharedData;
};

Q_DECLARE_METATYPE(strcExperimentStructureState)
Q_DECLARE_METATYPE(cExperimentStructure*)
//Q_DECLARE_METATYPE(cExperimentStructure)
Q_DECLARE_METATYPE(cBlockStructure*)
Q_DECLARE_METATYPE(cLoopStructure*)

#endif // EXPERIMENTSTRUCTURES_H
