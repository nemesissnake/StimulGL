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
#include "ExperimentStructures.h"
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

enum ExperimentRunState
{
	ES_IDLE							=  0,	//!<  0: IDLE STATE
	ES_WAITING_FOR_TRIGGER			=  1,	//!<  1: WAITING FOR TRIGGER
	ES_RUNNING						=  2,	//!<  2: RUNNING
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

public:
	cLoopStructure(QObject *parent = NULL);
	cLoopStructure(const int &LoopID,const int &LoopNumber,const QString &LoopName = "",const int &NumberOfLoops = RepeatAmount::RA_ZERO);
	cLoopStructure(const cLoopStructure& other){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~cLoopStructure();

	static QScriptValue ctor__cLoopStructure(QScriptContext* context, QScriptEngine* engine);

	int getCurrentLoopCounter()
	{
		return nCurrentLoop;
	};
	
	int incrementCurrentLoopCounter() 
	{
		if((nCurrentLoop+1) < nNrOfLoops)
			nCurrentLoop++;
		else
			nCurrentLoop = RA_FINISHED;
		return nCurrentLoop;
	};

	void resetCurrentLoopCounter() 
	{
		nCurrentLoop = 0;
	};

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

private:
	bool Initialize();

	QScriptEngine* currentScriptEngine;
	int nLoopID;
	int nLoopNumber;
	QString sLoopName;
	int nNrOfLoops;
	int nTargetBlockID;
	int nCurrentLoop;
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
	cBlockStructure(QObject *parent = NULL);
	cBlockStructure(const int &BlockID,const int &BlockNumber,const QString &BlockName = "",const int &NumberOfTrials = RepeatAmount::RA_ZERO);
	cBlockStructure(const cBlockStructure& other){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~cBlockStructure();

	static QScriptValue ctor__cBlockStructure(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	void setBlockID(const int &nValue) {nBlockID = nValue;};
	int getBlockID() const {return nBlockID;};
	void setBlockNumber(const int &nValue) {nBlockNumber = nValue;};
	int getBlockNumber() const {return nBlockNumber;};
	void setBlockName(const QString &sValue) {sBlockName = sValue;};
	QString getBlockName() const {return sBlockName;};
	void setNumberOfTrials(const int &nValue) {nNrOfTrials = nValue;};
	int getNumberOfTrials() const {return nNrOfTrials;};
	void setNumberOfInternalTriggers(const int &nValue) {nNrOfInternalTriggers = nValue;};
	int getNumberOfInternalTriggers() const {return nNrOfInternalTriggers;};
	void setNumberOfExternalTriggers(const int &nValue) {nNrOfExternalTriggers = nValue;};
	int getNumberOfExternalTriggers() const {return nNrOfExternalTriggers;};

	cLoopStructure *incrementToNextLoopPointer(cLoopStructure *pCurrentLoop = NULL);
	cLoopStructure *resetToFirstLoopPointer();
	bool insertLoop(cLoopStructure *cLoop);

private:
	bool Initialize();
	bool isUnusedLoopID(const int &nLoopID);

	QScriptEngine* currentScriptEngine;
	int nBlockID;
	int nBlockNumber;
	QString sBlockName;
	int nNrOfTrials;
	int nNrOfInternalTriggers;
	int nNrOfExternalTriggers;
	QList<cLoopStructure*> lLoops;
	int nInternalLoopCounter;
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
	////! The timeout Signal.
	///*!
	//	This signal is emitted automatically at each period time after the trigger is started.
	//*/
	//void timeout();
	void experimentStarted();
	void externalTriggerRecieved();
	void experimentStopped();

public:
	cExperimentStructure(QObject *parent = NULL);
	cExperimentStructure(const cExperimentStructure& other){};//TODO fill in copy constructor, should be declared for the Q_DECLARE_METATYPE macro
	~cExperimentStructure();

	static QScriptValue ctor__cExperimentStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue CreateExperimentStructureStateFromScript(QScriptContext *, QScriptEngine *engine);
	static QScriptValue ExperimentStructureStateToScriptValue(QScriptEngine *engine, const strcExperimentStructureState &s);
	static void resetExperimentStateStruct(strcExperimentStructureState *strcExpState);
	static void ExperimentStructureStateFromScriptValue(const QScriptValue &obj, strcExperimentStructureState &s);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	void resetExperiment() {resetExperimentStateStruct(&currentExperimentState);};
	strcExperimentStructureState getCurrentExperimentState() {return currentExperimentState;};

	void setExperimentName(const QString &sValue) {sExperimentName = sValue;};
	QString getExperimentName() const {return sExperimentName;};
	void setExperimentID(const int &nValue) {nExperimentID = nValue;};
	int getExperimentID() const {return nExperimentID;};
	void setExperimentDebugMode(const bool &bValue) {bDebugMode = bValue;};
	bool getExperimentDebugMode() const {return bDebugMode;};

	void incrementExternalTrigger();
	bool prepareExperiment();
	bool insertBlock(cBlockStructure *cBlock);
	cBlockStructure* getBlockPointerByID(const int &nBlockID);

private:
	void ExperimentAbort();
	void ExperimentStop();
	void ExperimentStart();
	bool Initialize();
	bool isUnusedBlockID(const int &nBlockID);
	bool isValidBlockPointer(cBlockStructure *cBlock);
	bool prepareStartBlock();
	cBlockStructure* getNextClosestBlockNumberByFromNumber(const int &startBlockNumber);

	ExperimentRunState CurrentExperiment_State;		//Only for internal usage
	strcExperimentStructureState currentExperimentState;
	QScriptEngine* currentScriptEngine;
	//QHash<QString, int> timerTypeHash;
	int nExperimentID;
	//int nExperimentNumber;
	QString sExperimentName;
	bool bDebugMode;
	QList<cBlockStructure*> lBlocks;
	cBlockStructure *currentBlockPointer;			//Only for internal usage
	cLoopStructure *currentLoopPointer;				//Only for internal usage
	cBlockStructure *firstBlockPointer;				//Only for internal usage
	//ExperimentManager *pExperimentManager;	
};

Q_DECLARE_METATYPE(strcExperimentStructureState)
Q_DECLARE_METATYPE(cExperimentStructure*)
Q_DECLARE_METATYPE(cBlockStructure*)
Q_DECLARE_METATYPE(cLoopStructure*)

//Q_DECLARE_FLAGS(TriggerTimerTypes, TriggerTimerType)
//Q_DECLARE_OPERATORS_FOR_FLAGS(TriggerTimerTypes)
#endif // EXPERIMENTSTRUCTURES_H
