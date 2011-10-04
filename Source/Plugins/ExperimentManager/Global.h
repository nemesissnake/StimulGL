#ifndef EXPMANAGER_GLOBAL_H
#define EXPMANAGER_GLOBAL_H

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

typedef struct{
	int nExperimentID;
	//int nExperimentNumber;
	QString nExperimentName;
	bool bDebugMode;
} ExperimentConfiguration;

typedef struct{
	int nTrialID;
	int nTrialNumber;
	//QString nBlockName;
	int nNrOfTriggers;
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

#define SIGNAL_USERCLOSE_FULL					"UserWantsToClose(void)"
#define SIGNAL_OBJECTSTOP_FULL					"ObjectShouldStop(void)"
#define FUNC_OBJECTSTATECHANGED_FULL			"ObjectStateHasChanged(ExperimentSubObjectState)"
#define FUNC_SETBLOCKTRIALDOMNODELIST			"setBlockTrialDomNodeList"
#define FUNC_SETBLOCKTRIALDOMNODELIST_FULL		"setBlockTrialDomNodeList(QDomNodeList*)"
#define FUNC_SETOBJECTID						"setObjectID"
#define FUNC_SETOBJECTID_FULL					"setObjectID(int)"
#define FUNC_SETEXPERIMENTCONFIGURATION			"setExperimentConfiguration"
#define FUNC_SETEXPERIMENTCONFIGURATION_FULL	"setExperimentConfiguration(ExperimentConfiguration*)"
#define FUNC_START								"start"
#define FUNC_STOP								"stop"
#define FUNC_ABORT								"abort"
#define SIGNAL_LOGTOMANAGER						"LogToExperimentManager(QString)"//Don't change see also connect function!
#define DATETIMESTAMPFORMAT						"yyyyMMddHHmmsszzz"
#define RETINOMAP_WIDGET_NAME					"RetinoMap_glwidget"
#define MODULE_NAME								"Experiment Parser"

#define ROOT_TAG					"EXML"
#define VERSION_TAG					"version"
#define BOOKMARK_TAG				"bookmark"
#define ID_TAG						"ID"
#define OBJECT_TAG					"object"
#define DEFINES_TAG					"defines"
#define ACTIONS_TAG					"actions"
#define BLOCKTRIALS_TAG				"blocks"
#define BLOCK_TAG					"block"
#define PARAMETER_TAG				"parameter"
#define PARAMETERS_TAG				"parameters"
#define BLOCKNUMBER_TAG				"block_number"
#define TRIALAMOUNT_TAG				"nr_of_trials"
#define TRIGGERAMOUNT_TAG			"nr_of_triggers"
#define DECLARATIONS_TAG			"declarations"
#define CONNECTIONS_TAG				"connections"
#define CONNECTIONS_TYPE_TAG		"type"
#define CONNECTIONS_TYPE_SIGNAL_TAG	"signal"
#define CONNECTIONS_TYPE_SLOT_TAG	"slot"
#define CONNECTIONS_SIGNATURE_TAG	"signature"
#define CONNECTIONS_TARGET_TAG		"target"
#define DEBUGMODE_TAG				"debugmode"
#define EXPERIMENT_TAG				"experiment"
#define CLASS_TAG					"class"
#define NAME_TAG					"name"
#define VALUE_TAG					"value"
#define HREF_TAG					"href"
#define FOLDER_TAG					"folder"
#define TITLE_TAG					"title"
#define BOOL_FALSE_TAG				"false"
#define BOOL_TRUE_TAG				"true"
#define FOLDED_TAG					"folded"
#define EVENT_TAG					"event"

#endif //EXPMANAGER_GLOBAL_H