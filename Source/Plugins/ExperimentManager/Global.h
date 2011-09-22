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

#define FUNC_USERCLOSE_FULL					"UserWantsToClose(void)"
#define FUNC_WIDGETSTATECHANGED_FULL		"WidgetStateHasChanged(ExperimentObjectState)"
#define FUNC_SETBLOCKTRIALDOMNODELIST_FULL	"setBlockTrialDomNodeList(QDomNodeList*)"
#define FUNC_SETBLOCKTRIALDOMNODELIST		"setBlockTrialDomNodeList"
#define FUNC_START							"start"
#define FUNC_STOP							"stop"
#define DATETIMESTAMPFORMAT					"yyyyMMddHHmmsszzz"
#define RETINOMAP_WIDGET_NAME				"RetinoMap_glwidget"

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
#define DECLARATIONS_TAG	"declarations"
#define CLASS_TAG			"class"
#define NAME_TAG			"name"
#define VALUE_TAG			"value"
#define MODULE_NAME			"Experiment Parser"
#define HREF_TAG			"href"
#define FOLDER_TAG			"folder"
#define TITLE_TAG			"title"
#define BOOL_NO_TAG			"no"
#define BOOL_YES_TAG		"yes"
#define FOLDED_TAG			"folded"
#define EVENT_TAG			"event"

#endif //EXPMANAGER_GLOBAL_H