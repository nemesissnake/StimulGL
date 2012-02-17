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

enum ExperimentState //The state of the main experiment object
{
	Experiment_Constructed	= 0,
	Experiment_Initialized	= 1,
	Experiment_IsStarting	= 2,
	Experiment_Started		= 3,
	Experiment_IsStopping	= 4,
	Experiment_Stopped		= 5
};

enum ExperimentSubObjectState //The state of an experiment (sub)object
{	
	Experiment_SubObject_Initialized	= 0,
	Experiment_SubObject_Started		= 1,
	Experiment_SubObject_Abort			= 2,
	Experiment_SubObject_IsAborting		= 3,
	Experiment_SubObject_Stop			= 4,
	Experiment_SubObject_IsStopping		= 5,
	Experiment_SubObject_Stopped		= 6	
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

//typedef struct{
//	QString sSignature;
//	QList<QGenericArgument> lInputParameters;
//} SlotDefinition;
//
//typedef struct{
//	QString sName;
//	int nMetaType;
//
//} ParameterDefinition;

typedef struct{
	int nExperimentID;
	//int nExperimentNumber;
	QString nExperimentName;
	bool bDebugMode;
	ExperimentManager *pExperimentManager;
} ExperimentConfiguration;

typedef struct{
	QList<ExperimentSubObjectState> nState;
	QList<QString> sDateTimeStamp;
} ExperimentSubObjectStateHistory;

typedef struct{
	int nTrialID;
	int nTrialNumber;
	//QString nBlockName;
	int nNrOfInternalTriggers;
	int nNrOfExternalSubTriggers;//Normally this is 1 which means that each ExternalSubTrigger is valid for 1 InternalTrigger, eg 3 means that only after 3 ExternalSubTriggers 1 InternalTriggers is processed!
	//RandomGenerator *randEmptyStimGenerator;//This can hold some trial specific Information that can also be randomized
} TrialStructure;

typedef struct{
	int nBlockID;
	int nBlockNumber;
	QString nBlockName;
	int nNrOfTrials;
	QList<TrialStructure> lTrialStructure;
} BlockTrialStructure;

typedef struct{
	//int nExperimentID??
	//int nExperimentName??
	int nNrOfBlocks;
	QList<BlockTrialStructure> lBlockTrialStructure;
} ExperimentBlockTrialStructure;

typedef struct{
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

typedef struct{
	ExperimentSnapshotStructure parentStruct;
	int nTotalProcessedExperimentTrials;		//The total number of trials processed within experiment, might be that this Trial is not fully processed
	int nNextThresholdTriggerCount;				//When we should switch to the next block
} ExperimentSnapshotFullStructure;

//typedef QHash<QString, QString> ExperimentBlockParameterStructure;
#define FUNC_PAINTOBJECT						"paintObject"
#define FUNC_PAINTOBJECT_FULL					"paintObject(int,QObject *)"
#define FUNC_SETOBJECTID						"setObjectID"
#define FUNC_SETOBJECTID_FULL					"setObjectID(int)"
#define FUNC_INITOBJECT							"initObject"
#define FUNC_INITOBJECT_FULL					"initObject()"
#define FUNC_STARTOBJECT						"startObject"
#define FUNC_STARTOBJECT_FULL					"startObject()"
#define FUNC_SETOBJECTCONFIGURATION				"setObjectConfiguration"
#define FUNC_SETOBJECTCONFIGURATION_FULL		"setObjectConfiguration(QObject *)"
#define FUNC_INITOBJECTBLOCKTRIAL				"initObjectBlockTrial"
#define FUNC_INITOBJECTBLOCKTRIAL_FULL			"initObjectBlockTrial()"

#define SIGNAL_USERCLOSE_FULL					"UserWantsToClose(void)"
#define SIGNAL_OBJECTSTOP_FULL					"ObjectShouldStop(void)"
#define FUNC_OBJECTSTATECHANGED_FULL			"ObjectStateHasChanged(ExperimentSubObjectState)"
#define FUNC_SETBLOCKTRIALDOMNODELIST			"setBlockTrialDomNodeList"
#define FUNC_SETBLOCKTRIALDOMNODELIST_FULL		"setBlockTrialDomNodeList(QDomNodeList*)"

#define FUNC_SETMETAOBJECT						"setExperimentMetaObject"
#define FUNC_SETMETAOBJECT_FULL					"setExperimentMetaObject()"
#define FUNC_SETEXPERIMENTOBJECTID				"setExperimentObjectID"
#define FUNC_SETEXPERIMENTOBJECTID_FULL			"setExperimentObjectID(int)"
#define FUNC_SETEXPERIMENTCONFIGURATION			"setExperimentConfiguration"
#define FUNC_SETEXPERIMENTCONFIGURATION_FULL	"setExperimentConfiguration(ExperimentConfiguration*)"
#define FUNC_OBJECT_INIT						"initExperimentObject"
#define FUNC_OBJECT_START						"startExperimentObject"
#define FUNC_OBJECT_STOP						"stopExperimentObject"
#define FUNC_OBJECT_ABORT						"abortExperimentObject"
#define RETINOMAP_WIDGET_NAME					"RetinoMap_glwidget"
#define TRIGGERTIMER_NAME						"TriggerTimer"
#define IMAGEPROCESSOR_NAME						"ImageProcessor"
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
#define CONNECTIONS_TYPE_SIGNAL_TAG		"signal"
#define CONNECTIONS_TYPE_SLOT_TAG		"slot"
#define CONNECTIONS_SIGNATURE_TAG		"signature"
#define CONNECTIONS_TARGET_TAG			"target"
#define INIT_FINIT_TYPE_TAG				"type"
#define INIT_FINIT_TYPE_SLOT_TAG		"slot"
#define INIT_FINIT_SIGNATURE_TAG		"signature"
#define MEMBER_TYPE_TAG					"type"
#define MEMBER_TYPE_SLOT_TAG			"slot"
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
