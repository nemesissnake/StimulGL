#ifndef EDITTREE_GLOBAL_H
#define EDITTREE_GLOBAL_H

#include "..\Global.h"
#include "..\defines.h"

#define DEFAULT_DOCUMENTTYPEDEFINITION	"<!DOCTYPE EXML>"
#define DEFINITION_NAME_VALUE			"Value"
#define DEFINITION_TAG_COMMENT			"<COMMENT>"

enum TreeItemType
{	
	TreeItemType_Attribute	= 0, 
	TreeItemType_Comment	= 1 
};

struct TreeItemDefinition 
{
	TreeItemType type;
	QVariant value;
};

struct TreeFilterSettings
{
	bool bSkipEmptyAttributes;
	bool bSkipComments;
};

#endif //EDITTREE_GLOBAL_H