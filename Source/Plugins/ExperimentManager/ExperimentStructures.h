//ExperimentManagerplugin
//Copyright (C) 2014  Sven Gijsen
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

#include <QtScript/QtScript>
#include <QList>
#include <QSharedData>
#include "maindefines.h"

/*! The namespace for the Experiment Structures */
namespace ExperimentStructuresNameSpace
{
	//The enum RepeatAmount can store the remaining amount of trials or actions to take
	enum RepeatAmount
	{
		RA_ZERO							=  0,	//  0: Zero Repeats
		RA_FINISHED						= -1,	// -1: Repeats have all been processed/finished
		RA_REINITIALIZE					= -2,	// -2: Repeats should be re-initialized first
		RA_INFINITE						= -3,	// -3: Infinite Repeats
		RA_UNDEFINED					= -4	// -4: None defined (i.e. no loops defined)
	};

	enum LoopCounterException
	{
		LCE_FIRSTLOOP					=  0,	//   0: This is the first loop
		LCE_UNUSED						= -1,	//  -1: This loop is not yet used
		LCE_FINISHED					= -2	//  -2: This loop is fully processed
	};

	enum ExperimentRunState
	{
		ES_IDLE							=  0,	//  0: IDLE STATE
		ES_WAITING_FOR_TRIGGER			=  1,	//  1: WAITING FOR TRIGGER
		ES_RUNNING						=  2	//  2: RUNNING
	};

	enum ObjectIndex
	{
		OI_UNDEFINED					= -1
	};

	enum CounterFlag
	{
		CF_UNINITIALIZED				= -1
	};

	/*! \struct strcExperimentStructureState
	 *  \brief The strcExperimentStructureState structure.
	 *
	 * This can store a state of a Experiment Structure, that holds a number of public accessible variables.
	 */
	typedef struct strcExperimentStructureState
	{
		int Experiment_ExternalTrigger;			//!<  The received ExternalTriggers since the Experiment started.
		int CurrentBlock_BlockID;				//!<  The current Block ID.
		int CurrentBlock_LoopID;				//!<  The current Loop ID from the current Block (with ID=strcExperimentStructureState::CurrentBlock_BlockID).
		int CurrentBlock_TrialNumber;			//!<  The current Trial Number from the current Block (with ID=strcExperimentStructureState::CurrentBlock_BlockID).
		int CurrentBlock_InternalTrigger;		//!<  The current Internal Trigger Number from the current Block (with ID=strcExperimentStructureState::CurrentBlock_BlockID).
		int CurrentBlock_ExternalTrigger;		//!<  The current External Trigger Number from the current Block (with ID=strcExperimentStructureState::CurrentBlock_BlockID).
	} ExperimentStructureState;

	struct strcExperimentObject
	{
		int nID;
		QString sName;
		QString sClass;
		strcExperimentObject()
		{
			nID = -1;
			sName = "";
			sClass = "";
		}
	};
}

//!  The cLoopStructure class. 
/*!
  The cLoopStructure class is a container to store a Loop structure from a block structure, see cBlockStructure.
*/
class cLoopStructure : public QObject
{
	Q_OBJECT

	//! \brief LoopID property.
	/*!  Use this property to set or retrieve the LoopID.
    */
	Q_PROPERTY(int LoopID WRITE setLoopID READ getLoopID)
	//! \brief LoopNumber property.
	/*!  Use this property to set or retrieve the LoopNumber.
    */
	Q_PROPERTY(int LoopNumber WRITE setLoopNumber READ getLoopNumber)
	//! \brief LoopName property.
	/*!  Use this property to set or retrieve the LoopName.
    */
	Q_PROPERTY(QString LoopName WRITE setLoopName READ getLoopName)
	//! \brief NumberOfLoops property.
	/*!  Use this property to set or retrieve the NumberOfLoops.
    */
	Q_PROPERTY(int NumberOfLoops WRITE setNumberOfLoops READ getNumberOfLoops)
	//! \brief TargetBlockID property.
	/*!  Use this property to set or retrieve the TargetBlockID.
    */
	Q_PROPERTY(int TargetBlockID WRITE setTargetBlockID READ getTargetBlockID)
	//! \brief LoopCounter property.
	/*!  Use this property to set or retrieve the LoopCounter.
    */
	Q_PROPERTY(int LoopCounter READ getTargetBlockID)

public:
	cLoopStructure();
	cLoopStructure(const int &LoopID,const int &LoopNumber,const int &LoopTargetBlockID,const QString &LoopName = "",const int &NumberOfLoops = ExperimentStructuresNameSpace::RA_ZERO);
	cLoopStructure(const cLoopStructure& other);
	~cLoopStructure();

	static QScriptValue ctor__cLoopStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue loopStructureToScriptValue(QScriptEngine *engine, cLoopStructure* const &s);
	static void loopStructureFromScriptValue(const QScriptValue &obj, cLoopStructure* &s);

	int initializeCurrentLoopCounter();
	int incrementCurrentLoopCounter();
	void resetCurrentLoopCounter();
	void finalizeCurrentLoopCounter();

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	//! \brief setLoopID slot.
	/*!  This function sets the loop ID to the new provided value.
	 * @param nValue a integer value (>0) holding the new loop ID.
	 */	
	void setLoopID(const int &nValue) {nLoopID = nValue;};
	//! \brief getLoopID slot.
	/*!  This function returns the loop ID.
	 * @return a integer value containing the requested loop ID.
	 */	
	int getLoopID() const {return nLoopID;};
	//! \brief setLoopNumber slot.
	/*!  This function sets the loop number to the new provided value.
	 * @param nValue a integer value (>0) holding the new loop number.
	 */	
	void setLoopNumber(const int &nValue) {nLoopNumber = nValue;};
	//! \brief getLoopNumber slot.
	/*!  This function returns the loop number.
	 * @return a integer value containing the requested loop number.
	 */	
	int getLoopNumber() const {return nLoopNumber;};
	//! \brief setLoopName slot.
	/*!  This function sets the loop name to the new provided value.
	 * @param sValue a string value holding the new loop name.
	 */	
	void setLoopName(const QString &sValue) {sLoopName = sValue;};
	//! \brief getLoopName slot.
	/*!  This function returns the loop name.
	 * @return a string value containing the requested loop name.
	 */	
	QString getLoopName() const {return sLoopName;};
	//! \brief setNumberOfLoops slot.
	/*!  This function set the number of loops to the new provided value.
	 * @param nValue a integer value holding the number of loops.
	 */	
	void setNumberOfLoops(const int &nValue) {nNrOfLoops = nValue;};
	//! \brief getNumberOfLoops slot.
	/*!  This function returns the number of defined loops.
	 * @return a integer value containing number of defined loops.
	 */	
	int getNumberOfLoops() const {return nNrOfLoops;};
	//! \brief setTargetBlockID slot.
	/*!  This function sets the target block ID for the current loop to the new provided value.
	 * @param nValue a integer value (>0) holding the new target block ID.
	 */	
	void setTargetBlockID(const int &nValue) {nTargetBlockID = nValue;};
	//! \brief getTargetBlockID slot.
	/*!  This function returns the current loop target block ID.
	 * @return a integer value containing the current loop target block ID.
	 */	
	int getTargetBlockID() const {return nTargetBlockID;};
	//! \brief getLoopCounter slot.
	/*!  This function returns the loop counter value for the current loop.
	 * @return a integer value containing the loop counter value for the current loop.
	 */	
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
	QList<cLoopStructure*> lLoops;
};

//!  The cBlockStructure class. 
/*!
  The cBlockStructure class is a container to store a Block structure from a experiment structure, see cExperimentStructure.
*/
class cBlockStructure : public QObject
{
	Q_OBJECT

	//! \brief BlockID property.
	/*!  Use this property to set or retrieve the BlockID.
    */
	Q_PROPERTY(int BlockID WRITE setBlockID READ getBlockID)
	//! \brief BlockNumber property.
	/*!  Use this property to set or retrieve the BlockNumber.
    */
	Q_PROPERTY(int BlockNumber WRITE setBlockNumber READ getBlockNumber)
	//! \brief BlockName property.
	/*!  Use this property to set or retrieve the BlockName.
    */
	Q_PROPERTY(QString BlockName WRITE setBlockName READ getBlockName)
	//! \brief NumberOfTrials property.
	/*!  Use this property to set or retrieve the NumberOfTrials.
    */
	Q_PROPERTY(int NumberOfTrials WRITE setNumberOfTrials READ getNumberOfTrials)
	//! \brief NumberOfInternalTriggers property.
	/*!  Use this property to set or retrieve the NumberOfInternalTriggers.
    */
	Q_PROPERTY(int NumberOfInternalTriggers WRITE setNumberOfInternalTriggers READ getNumberOfInternalTriggers)
	//! \brief NumberOfExternalTriggers property.
	/*!  Use this property to set or retrieve the NumberOfExternalTriggers.
    */
	Q_PROPERTY(int NumberOfExternalTriggers WRITE setNumberOfExternalTriggers READ getNumberOfExternalTriggers)

public:
	cBlockStructure();
	cBlockStructure(const int &BlockID,const int &BlockNumber,const QString &BlockName = "",const int &NumberOfTrials = ExperimentStructuresNameSpace::RA_ZERO);
	cBlockStructure(const cBlockStructure& other);
	~cBlockStructure();

	static QScriptValue ctor__cBlockStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue blockStructureToScriptValue(QScriptEngine *engine, cBlockStructure* const &s);
	static void blockStructureFromScriptValue(const QScriptValue &obj, cBlockStructure* &s);
	
public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	
	//! \brief setBlockID slot.
	/*!  This function sets the block ID to the new provided value.
	 * @param nValue a integer value (>0) containing the new block ID.
	 */		
	void setBlockID(const int &nValue) {pSharedData->nBlockID = nValue;};
	//! \brief getBlockID slot.
	/*!  This function returns the block ID.
	 * @return a integer containing the block ID.
	 */		
	int getBlockID() const {return pSharedData->nBlockID;};
	//! \brief setBlockNumber slot.
	/*!  This function sets the block number to the new provided value.
	 * @param nValue a integer value (>0) containing the new block number.
	 */		
	void setBlockNumber(const int &nValue) {pSharedData->nBlockNumber = nValue;};
	//! \brief getBlockNumber slot.
	/*!  This function returns the block number.
	 * @return a integer containing the block number.
	 */		
	int getBlockNumber() const {return pSharedData->nBlockNumber;};
	//! \brief setBlockName slot.
	/*!  This function sets the block name to the new provided value.
	 * @param sValue a string value containing the new block name.
	 */		
	void setBlockName(const QString &sValue) {pSharedData->sBlockName = sValue;};
	//! \brief getBlockName slot.
	/*!  This function returns the block name.
	 * @return a string containing the block name.
	 */		
	QString getBlockName() const {return pSharedData->sBlockName;};
	//! \brief setNumberOfTrials slot.
	/*!  This function sets the number of trials for the block to the new provided value.
	 * @param nValue a integer value containing the new number of trials.
	 */		
	void setNumberOfTrials(const int &nValue) {pSharedData->nNrOfTrials = nValue;};
	//! \brief getNumberOfTrials slot.
	/*!  This function returns the number of trials for the block.
	 * @return a integer value containing the number of trials from the block.
	 */		
	int getNumberOfTrials() const {return pSharedData->nNrOfTrials;};
	//! \brief setNumberOfInternalTriggers slot.
	/*!  This function sets the number of internal triggers for the block to the new provided value.
	 * @param nValue a integer value containing the new number of internal triggers.
	 */		
	void setNumberOfInternalTriggers(const int &nValue) {pSharedData->nNrOfInternalTriggers = nValue;};
	//! \brief getNumberOfInternalTriggers slot.
	/*!  This function returns the number of internal triggers for the block.
	 * @return a integer value containing the number of internal triggers from the block.
	 */		
	int getNumberOfInternalTriggers() const {return pSharedData->nNrOfInternalTriggers;};
	//! \brief setNumberOfExternalTriggers slot.
	/*!  This function sets the number of external triggers for the block to the new provided value.
	 * @param nValue a integer value containing the new number of external triggers.
	 */		
	void setNumberOfExternalTriggers(const int &nValue) {pSharedData->nNrOfExternalTriggers = nValue;};
	//! \brief getNumberOfExternalTriggers slot.
	/*!  This function returns the number of external triggers for the block.
	 * @return a integer value containing the number of external triggers from the block.
	 */		
	int getNumberOfExternalTriggers() const {return pSharedData->nNrOfExternalTriggers;};
	//! \brief incrementToNextLoopPointer slot.
	/*!  This function increments and returns the provided cLoopStructure pointer to the next available cLoopStructure pointer.
	 *   If the provided cLoopStructure is NULL, then the internal cLoopStructure is reset and returns the first available cLoopStructure for this block.
	 *   If there's no next available cLoopStructure the this function returns NULL.
	 * @param pCurrentLoop a pointer to a cLoopStructure that should be incremented.
	 * @return a pointer to a cLoopStructure or a NULL value.
	 */		
	cLoopStructure *incrementToNextLoopPointer(cLoopStructure *pCurrentLoop = NULL);
	//! \brief resetToFirstFreeLoopPointer slot.
	/*!  This function resets the provided cLoopStructure to the first free (unprocessed) cLoopStructure and return a pointer to this.
	 * @return a pointer to a cLoopStructure, if there's no available free cLoopStructure the NULL is returned.
	 */		
	cLoopStructure *resetToFirstFreeLoopPointer();
	//! \brief resetAllLoopCounters slot.
	/*!  This function resets all cLoopStructure structures from the block.
	 */		
	void resetAllLoopCounters();
	//! \brief resetAllInnerLoopCounters slot.
	/*!  This function resets all cLoopStructure structures from the block which have loop number(s) smaller than the provided loop number.
	 * @param nCurrentLoopCounter a integer value that holds the first outer loop number that should not be reset by this function.
	 */		
	void resetAllInnerLoopCounters(const int &nCurrentLoopCounter);
	//! \brief insertLoop slot.
	/*!  This function inserts a new cLoopStructure in the block.
	 * @param cLoop a pointer to a cLoopStructure that should be inserted.
	 * @return a boolean value determining whether the new cLoopStructure structure could be inserted.
	 */	
	bool insertLoop(cLoopStructure *cLoop);
	//! \brief getNextClosestLoopIDByFromID slot.
	/*!  This function returns a pointer to the next first cLoopStructureloop where the loop ID value is bigger then the provided loop ID.
	 * @param startLoopID a integer value determining the value for the requested cLoopStructureloop where the loop ID value is bigger then this value.
	 * @return a pointer to the next first cLoopStructure structure, if no cLoopStructure is available then this function returns NULL.
	 */		
	cLoopStructure* getNextClosestLoopIDByFromID(const int &startLoopID);
	//! \brief *  slot.
	/*!  This function returns a pointer to a cLoopStructure specified by the provided loop ID.
	 * @param nLoopID a integer value holding the value for the specified loop ID.
	 * @return a pointer to the cLoopStructure structure specified by the provided loop ID, if it's unavailable then NULL is returned.
	 */		
	cLoopStructure* getLoopPointerByID(const int &nLoopID);
	//! \brief getLoopCount slot.
	/*!  This function return the amount of defined cLoopStructure structures in the block.
	 * @return a integer value holding the amount of defined cLoopStructure structures.
	 */		
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

	ExperimentStructuresNameSpace::ExperimentRunState CurrentExperiment_RunState;			//Only for internal usage
	ExperimentStructuresNameSpace::strcExperimentStructureState currentExperimentState;
	QScriptEngine* currentScriptEngine;
	int nExperimentID;
	QString sExperimentName;
	bool bDebugMode;
	QList<cBlockStructure*> lBlocks;
	cBlockStructure *currentBlockPointer;					//Only for internal usage
	cLoopStructure *currentLoopPointer;						//Only for internal usage
	cBlockStructure *firstBlockPointer;						//Only for internal usage
};


//!  The cExperimentStructure class. 
/*!
  The cExperimentStructure class is a container to store the experiment structure, it can be created/changed dynamically or loaded from a EXML file and is internally used by the ExperimentManager to store the Experiment Structure in memory.
*/
class cExperimentStructure : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "cExperimentStructure");//Can't use defines here!, moc doesn't handle defines, not needed here
	
	//! \brief ExperimentName property.
	/*!  Use this property to set or retrieve the Experiment Name.
    */	
	Q_PROPERTY(QString ExperimentName WRITE setExperimentName READ getExperimentName)
	//! \brief ExperimentID property.
	/*!  Use this property to set or retrieve the Experiment ID.
    */	
	Q_PROPERTY(int ExperimentID WRITE setExperimentID READ getExperimentID)
	//! \brief ExperimentDebugMode property.
	/*!  Use this property to set or retrieve the Experiment Debug Mode.
    */	
	Q_PROPERTY(bool ExperimentDebugMode WRITE setExperimentDebugMode READ getExperimentDebugMode)

signals:
	//! \brief experimentStarted() signal.
	/*!  This signal is emitted when the Experiment is started.
	 */
	void experimentStarted();
	//! \brief externalTriggerRecieved() signal.
	/*!  This signal is emitted when the Experiment Manager/Engine receives an external trigger.
	 */
	void externalTriggerRecieved();
	//! \brief experimentStopped() signal.
	/*!  This signal is emitted when the Experiment is stopped.
	 */
	void experimentStopped();

public:
	cExperimentStructure();
	cExperimentStructure(const cExperimentStructure& other);
	~cExperimentStructure();

	static QScriptValue ctor__cExperimentStructure(QScriptContext* context, QScriptEngine* engine);
	static QScriptValue experimentStructureToScriptValue(QScriptEngine *engine, cExperimentStructure* const &s);
	static void experimentStructureFromScriptValue(const QScriptValue &obj, cExperimentStructure* &s);
	static QScriptValue createExperimentStructureStateFromScript(QScriptContext *, QScriptEngine *engine);
	static QScriptValue experimentStructureStateToScriptValue(QScriptEngine *engine, const ExperimentStructuresNameSpace::strcExperimentStructureState &s);
	static void resetExperimentStateStruct(ExperimentStructuresNameSpace::strcExperimentStructureState *strcExpState);
	static void experimentStructureStateFromScriptValue(const QScriptValue &obj, ExperimentStructuresNameSpace::strcExperimentStructureState &s);
	cBlockStructure* getNextClosestBlockNumberByFromNumber(const int &startBlockNumber); 
	cBlockStructure getCurrentBlock(bool &bHasCurrBlock) const;//This doesn't work within the script, see reference...

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	//! \brief resetExperiment slot.
	/*!  This function resets the current Experiment Structure state, see ExperimentStructuresNameSpace::strcExperimentStructureState.
	 */
	void resetExperiment() {resetExperimentStateStruct(&pSharedData->currentExperimentState);};
	//! \brief getCurrentExperimentState slot.
	/*!  This function returns a copy of the current Experiment Structure state, see ExperimentStructuresNameSpace::strcExperimentStructureState.
	 * @return a strcExperimentStructureState structure .
	 */	
	ExperimentStructuresNameSpace::strcExperimentStructureState getCurrentExperimentState() const {return pSharedData->currentExperimentState;};
	//! \brief setExperimentName slot.
	/*!  This function sets the Experiment Name.
	 * @param sValue a string value containing the new Experiment File Name.
	 */	
	void setExperimentName(const QString &sValue) {pSharedData->sExperimentName = sValue;};
	//! \brief getExperimentName slot.
	/*!  This function returns the current Experiment Name.
	 * @return a string value containing the Experiment Name.
	 */	
	QString getExperimentName() const {return pSharedData->sExperimentName;};
	//! \brief setExperimentID slot.
	/*!  This function sets the Experiment ID.
	 * @param nValue a integer value containing the new Experiment ID.
	 */	
	void setExperimentID(const int &nValue) {pSharedData->nExperimentID = nValue;};
	//! \brief getExperimentID slot.
	/*!  This function returns the current Experiment ID.
	 * @return a integer value containing the Experiment ID.
	 */	
	int getExperimentID() const {return pSharedData->nExperimentID;};
	//! \brief setExperimentDebugMode slot.
	/*!  This function configures the Experiment Debug mode.
	 * @param bValue a boolean value determining whether the debug mode should be enabled.
	 */	
	void setExperimentDebugMode(const bool &bValue) {pSharedData->bDebugMode = bValue;};
	//! \brief getExperimentDebugMode slot.
	/*!  This function returns the current Experiment Debug mode.
	 * @return a boolean value determining whether the current Experiment Debug mode is enabled.
	 */	
	bool getExperimentDebugMode() const {return pSharedData->bDebugMode;};
	//! \brief incrementExternalTrigger slot.
	/*!  This function increments the current Experiment External trigger with 1. 
	 *   It is used to control the Experiment Structure.
	 */	
	void incrementExternalTrigger();
	//! \brief prepareExperiment slot.
	/*!  This function prepares the current Experiment so it can be started.
	 * @return a boolean value determining whether the function executed successfully.
	 */	
	bool prepareExperiment();
	//! \brief startExperiment slot.
	/*!  This function starts the current Experiment and will then wait for an external trigger, see cExperimentStructure::incrementExternalTrigger().
	 */	
	void startExperiment() {ExperimentStart();};
	//! \brief insertBlock slot.
	/*!  This function inserts a new cBlockStructure structure in the current Experiment Structure.
	 * @param cBlock a pointer to a cBlockStructure that needs to be inserted.
	 * @return a boolean value determining whether the function executed successfully.
	 */	
	bool insertBlock(cBlockStructure *cBlock);
	//! \brief getBlockPointerByID slot.
	/*!  This function returns a pointer to a cBlockStructure identified by a block ID.
	 * @param nBlockID a integer value defining the block ID from the requested Block pointer.
	 * @return a pointer to a cBlockStructure structure .
	 */	
	cBlockStructure* getBlockPointerByID(const int &nBlockID);
	//! \brief getBlockCount slot.
	/*!  This function return the number of defined blocks in the current experiment.
	 * @return a integer value holding the experiment block count.
	 */	
	int getBlockCount() const;
	//! \brief getExternalTriggerCount slot.
	/*!  This function returns the total number of received external triggers from the experiment since it was started.
	 * @return a integer value containing the received experiment external triggers.
	 */	
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

Q_DECLARE_METATYPE(ExperimentStructuresNameSpace::strcExperimentStructureState)
Q_DECLARE_METATYPE(cExperimentStructure*)
Q_DECLARE_METATYPE(cBlockStructure*)
Q_DECLARE_METATYPE(cLoopStructure*)

#endif // EXPERIMENTSTRUCTURES_H
