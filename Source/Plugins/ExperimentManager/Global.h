#ifndef EXPMANAGER_GLOBAL_H
#define EXPMANAGER_GLOBAL_H

enum ExperimentMainState //The state of the main experiment object
{
	Experiment_Initialized	= 0,
	Experiment_Started		= 1,
	Experiment_Stopped		= 2	
};

enum ExperimentObjectState //The state of an experiment (sub)object
{
	ExperimentObject_Initialized	= 0,
	ExperimentObject_Started		= 1,
	ExperimentObject_Abort			= 2,
	ExperimentObject_IsAborting		= 3,
	ExperimentObject_Aborted		= 4,
	ExperimentObject_Stopped		= 5	
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
	int nBlockID;
	int nBlockNumber;
	QString nBlockName;
	int nNrOfTrials;
} BlockTrialStructure;

typedef struct{
	//int nExperimentID??
	//int nExperimentName??
	int nNrOfBlocks;
	QList<BlockTrialStructure> lBlockTrialStructure;
} ExperimentBlockTrialStructure;

#define FUNC_USERCLOSE_FULL					"UserWantsToClose(void)"
#define FUNC_WIDGETSTATECHANGED_FULL		"WidgetStateHasChanged(ExperimentObjectState)"
#define FUNC_SETBLOCKTRIALDOMNODELIST		"setBlockTrialDomNodeList"
#define FUNC_SETBLOCKTRIALDOMNODELIST_FULL	"setBlockTrialDomNodeList(QDomNodeList*)"
#define FUNC_SETOBJECTID					"setObjectID"
#define FUNC_SETOBJECTID_FULL				"setObjectID(int)"
#define FUNC_START							"start"
#define FUNC_STOP							"stop"
#define SIGNAL_LOGTOMANAGER					"LogToExperimentManager(QString)"//Don't change see also connect function!
#define DATETIMESTAMPFORMAT					"yyyyMMddHHmmsszzz"
#define RETINOMAP_WIDGET_NAME				"RetinoMap_glwidget"
#define MODULE_NAME							"Experiment Parser"

#define ROOT_TAG			"EXML"
#define VERSION_TAG			"version"
#define BOOKMARK_TAG		"bookmark"
#define ID_TAG				"ID"
#define OBJECT_TAG			"object"
#define ACTIONS_TAG			"actions"
#define BLOCKTRIALS_TAG		"blocktrials"
#define BLOCK_TAG			"block"
#define PARAMETER_TAG		"parameter"
#define BLOCKNUMBER_TAG		"block_number"
#define TRIALAMOUNT_TAG		"nr_of_trials"
#define DECLARATIONS_TAG	"declarations"
#define DEBUGMODE_TAG		"debugmode"
#define EXPERIMENT_TAG		"experiment"
#define CLASS_TAG			"class"
#define NAME_TAG			"name"
#define VALUE_TAG			"value"
#define HREF_TAG			"href"
#define FOLDER_TAG			"folder"
#define TITLE_TAG			"title"
#define BOOL_FALSE_TAG		"false"
#define BOOL_TRUE_TAG		"true"
#define FOLDED_TAG			"folded"
#define EVENT_TAG			"event"

#endif //EXPMANAGER_GLOBAL_H