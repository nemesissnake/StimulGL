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


#ifndef EXPMANAGER_GLOBAL_H
#define EXPMANAGER_GLOBAL_H

#include "../../StimulGL/mainappinfo.h"
#include "RandomGenerator.h"
class ExperimentManager;

enum ExperimentTimerType //The type of a experiment timer, used for logging(flags, combinations possible)
{
	Experiment_Timer_None		= 0,
	Experiment_Timer_Experiment	= 1,
	Experiment_Timer_Frame		= 2
	//Experiment_Timer_Experiment	= 4,
	//Experiment_Timer_Experiment	= 8,
	//Experiment_Timer_Experiment	= 16,
	//Experiment_Timer_Experiment	= 32
};

enum ExperimentSubObjectState //The state of an experiment (sub)object
{	
	Experiment_SubObject_Constructing	= 0,
	Experiment_SubObject_Initialized	= 1,
	Experiment_SubObject_Started		= 2,
	Experiment_SubObject_Abort			= 3,
	Experiment_SubObject_IsAborting		= 4,
	Experiment_SubObject_Stop			= 5,
	Experiment_SubObject_IsStopping		= 6,
	Experiment_SubObject_Stopped		= 7,
	Experiment_SubObject_Destructing	= 8,
	Experiment_SubObject_MaxState		= 9
};

enum EXML_TAG_SECTIONS //These are the main section Tag's
{
	EXML_Declarations,
	EXML_Actions,
	EXML_Object,
	EXML_Event
};

struct EXML_DocVersion //Structure to hold the version information
{
	int major;
	int minor;
	int version;
	int build;
};

typedef struct strcExperimentConfiguration
{
	int nExperimentID;
	//int nExperimentNumber;
	QString nExperimentName;
	bool bDebugMode;
	ExperimentManager *pExperimentManager;
} ExperimentConfiguration;

typedef struct strcExperimentSubObjectStateHistory
{
	QList<ExperimentSubObjectState> nState;
	QList<QString> sDateTimeStamp;
} ExperimentSubObjectStateHistory;

typedef struct strcTrialStructure
{
	int nTrialID;
	int nTrialNumber;
	//QString nBlockName;
	int nNrOfInternalTriggers;
	int nNrOfExternalSubTriggers;//Normally this is 1 which means that each ExternalSubTrigger is valid for 1 InternalTrigger, eg 3 means that only after 3 ExternalSubTriggers 1 InternalTriggers is processed!
	//RandomGenerator *randEmptyStimGenerator;//This can hold some trial specific Information that can also be randomized
} TrialStructure;

typedef struct strcBlockTrialStructure
{
	int nBlockID;
	int nBlockNumber;
	QString nBlockName;
	int nNrOfTrials;
	QList<TrialStructure> lTrialStructure;
} BlockTrialStructure;

typedef struct strcExperimentBlockTrialStructure
{
	//int nExperimentID??
	//int nExperimentName??
	int nNrOfBlocks;
	QList<BlockTrialStructure> lBlockTrialStructure;
} ExperimentBlockTrialStructure;

typedef struct strcExperimentSnapshotStructure
{
	int currExpBlockTrialFrame;
	int currExpExternalTrigger;			//The current experiment external trigger
	int currExpInternalTrigger;			//The current experiment internal trigger
	int currExpBlockTrialTrigger;		//This is different!! It is now the current trigger within a Block Trial
	int currExpTrial;					//The current experiment trial within the block 	
	int currExpBlock;					//The current experiment block
	int currExpBlockTrialInternalTriggerAmount;
	double elapsedTrialTime;
	//int currExpBlockTrialCycle;--> inside widget
} ExperimentSnapshotStructure;

typedef struct strcExperimentSnapshotFullStructure
{
	ExperimentSnapshotStructure parentStruct;
	int nTotalProcessedExperimentTrials;		//The total number of trials processed within experiment, might be that this Trial is not fully processed
	int nNextThresholdTriggerCount;				//When we should switch to the next block
} ExperimentSnapshotFullStructure;

//The following structure looks like the ExperimentSnapshotStructure, but this structure should NOT be used internally by the Experiment Manager but only by the script context!
//It's sole purpose is to support an easy way to automatically step trough (and buffer) the Experiment Structure just before a new ExternalTriggerIncremented() signal is emitted,
//It Allows the script then to fetch the correct structure using the getAbsoluteExperimentStructure(int nAbsoluteTrigger) slot!
typedef struct strcScriptExperimentStructure
{
	int currExpAbsExternalTrigger;				//The current experiment external trigger
	int currExpAbsInternalTrigger;				//The current experiment internal trigger
	int currExpBlockTrialRelInternalTrigger;	//This is different!! It is now the current trigger within a Block Trial
	int currExpTrial;							//The current experiment trial within the block 	
	int currExpBlock;							//The current experiment block
} ScriptExperimentStructure;

/*! \struct strcParsedParameterDefinition
 * A structure for the Experiment Manager parameter definitions.
 */
struct strcParsedParameterDefinition
{
	//! The default constructor of the structure
	strcParsedParameterDefinition()
	{
		sValue = "<undefined>";
		bHasChanged = true;
		bIsInitialized = true;
	}	
	QString sValue;		 //!< A String that stores the value of the parameter
	bool bHasChanged;    //!< A Boolean value determing wheter the parameter has been changed since it's initialization
	bool bIsInitialized; //!< A Boolean value determing wheter the parameter already has been initialized
};

/*! Creates a type name for strcParsedParameterDefinition */ 
typedef strcParsedParameterDefinition ParsedParameterDefinition;

typedef QHash<QString, ParsedParameterDefinition> tParsedParameterList;

#define GLWIDGET_BACKGROUNDCOLOR				"backgroundcolor"
#define GLWIDGET_WIDTH_PIXEL_AMOUNT				"stimuluswidthspan"
#define GLWIDGET_HEIGHT_PIXEL_AMOUNT			"stimulusheightspan"

#define FUNC_PAINTOBJECT						"paintObject"
#define FUNC_PAINTOBJECT_FULL					FUNC_PAINTOBJECT "(int,QObject *)"
#define FUNC_SETOBJECTID						"setObjectID"
#define FUNC_SETOBJECTID_FULL					FUNC_SETOBJECTID "(int)"
#define FUNC_INITOBJECT							"initObject"
#define FUNC_INITOBJECT_FULL					FUNC_INITOBJECT "()"
#define FUNC_STARTOBJECT						"startObject"
#define FUNC_STARTOBJECT_FULL					FUNC_STARTOBJECT "()"
#define FUNC_STOPOBJECT							"stopObject"
#define FUNC_STOPOBJECT_FULL					FUNC_STOPOBJECT "()"
#define FUNC_SETOBJECTCONFIGURATION				"setObjectConfiguration"
#define FUNC_SETOBJECTCONFIGURATION_FULL		FUNC_SETOBJECTCONFIGURATION "(QObject *)"
#define FUNC_INITOBJECTBLOCKTRIAL				"initObjectBlockTrial"
#define FUNC_INITOBJECTBLOCKTRIAL_FULL			FUNC_INITOBJECTBLOCKTRIAL "()"
#define FUNC_MAKETHISAVAILABLEINSCRIPT			"makeThisAvailableInScript"
#define FUNC_MAKETHISAVAILABLEINSCRIPT_FULL		FUNC_MAKETHISAVAILABLEINSCRIPT "(QString,QObject *)"

#define SIGNAL_USERCLOSE_FULL					"UserWantsToClose(void)"
#define SIGNAL_OBJECTSTOP_FULL					"ObjectShouldStop(void)"
#define FUNC_OBJECTSTATECHANGED_FULL			"ObjectStateHasChanged(ExperimentSubObjectState)"
#define FUNC_SETBLOCKTRIALDOMNODELIST			"setBlockTrialDomNodeList"
#define FUNC_SETBLOCKTRIALDOMNODELIST_FULL		FUNC_SETBLOCKTRIALDOMNODELIST "(QDomNodeList*)"

#define FUNC_SETMETAOBJECT						"setExperimentMetaObject"
#define FUNC_SETMETAOBJECT_FULL					FUNC_SETMETAOBJECT "()"
#define FUNC_SETEXPERIMENTOBJECTID				"setExperimentObjectID"
#define FUNC_SETEXPERIMENTOBJECTID_FULL			FUNC_SETEXPERIMENTOBJECTID "(int)"
#define FUNC_SETEXPERIMENTCONFIGURATION			"setExperimentConfiguration"
#define FUNC_SETEXPERIMENTCONFIGURATION_FULL	FUNC_SETEXPERIMENTCONFIGURATION "(ExperimentConfiguration*)"
#define FUNC_OBJECT_INIT						"initExperimentObject"
#define FUNC_OBJECT_START						"startExperimentObject"
#define FUNC_OBJECT_STOP						"stopExperimentObject"
#define FUNC_OBJECT_ABORT						"abortExperimentObject"
#define RETINOMAP_WIDGET_NAME					"RetinoMap_glwidget"
#define QML_WIDGET_NAME							"qmlWidget"   
#define TRIGGERTIMER_NAME						"TriggerTimer"
#define IMAGEPROCESSOR_NAME						"ImageProcessor"
#define PRTFORMATMANAGER_NAME					"PRTFormatManager"
#define QMLWIDGET_NAME							"qmlWidget"
#define EXPERIMENTMANAGER_NAME					"ExperimentManager"
#define MODULE_NAME								"Experiment Parser"
#define MAX_INVOKE_ARG_COUNT					10
//#define MIN_SCREEN_UPDATE_TIME					10
//#define MAX_SCREEN_UPDATE_TIME					16.6667f

#define DEFAULT_OUTPUTFILE				"Out.txt"
#define ARGB_FORMAT_HEADER				0xCAFE1234
#define DOUBLE_PI_VALUE					3.141592653589793238462643383279502884197
#define ROOT_TAG						"EXML"
#define VERSION_TAG						"version"
#define BOOKMARK_TAG					"bookmark"
#define ID_TAG							"ID"
#define OBJECT_TAG						"object"
#define DEFINES_TAG						"defines"
#define ACTIONS_TAG						"actions"
#define BLOCKTRIALS_TAG					"blocks"
#define BLOCK_TAG						"block"
#define PARAMETER_TAG					"parameter"
#define PARAMETERS_TAG					"parameters"
#define BLOCKNUMBER_TAG					"block_number"
#define TRIALAMOUNT_TAG					"nr_of_trials"
#define TRIGGERAMOUNT_TAG				"nr_of_internal_triggers"
#define SUBTRIGGERAMOUNT_TAG			"nr_of_external_triggers"
#define DECLARATIONS_TAG				"declarations"
#define CONNECTIONS_TAG					"connections"
#define INITIALIZATIONS_TAG				"initializations"
#define FINALIZATIONS_TAG				"finalizations"
#define CONNECTIONS_TYPE_TAG			"type"
#define CONNECTIONS_TYPE_SIGNAL_TAG		METHOD_TYPE_SIGNAL_TAG
#define CONNECTIONS_TYPE_SLOT_TAG		METHOD_TYPE_SLOT_TAG
#define CONNECTIONS_SIGNATURE_TAG		"signature"
#define CONNECTIONS_TARGET_TAG			"target"
#define INIT_FINIT_TYPE_TAG				"type"
#define INIT_FINIT_TYPE_SLOT_TAG		METHOD_TYPE_SLOT_TAG
#define INIT_FINIT_SIGNATURE_TAG		"signature"
#define MEMBER_TYPE_TAG					"type"
#define MEMBER_TYPE_SLOT_TAG			METHOD_TYPE_SLOT_TAG
#define DEBUGMODE_TAG					"debugmode"
#define EXPERIMENT_TAG					"experiment"
#define CLASS_TAG						"class"
#define NAME_TAG						"name"
#define VALUE_TAG						"value"
#define HREF_TAG						"href"
#define FOLDER_TAG						"folder"
#define TITLE_TAG						"title"
#define BOOL_FALSE_TAG					"false"
#define BOOL_TRUE_TAG					"true"
#define FOLDED_TAG						"folded"
#define EVENT_TAG						"event"

#endif //EXPMANAGER_GLOBAL_H
