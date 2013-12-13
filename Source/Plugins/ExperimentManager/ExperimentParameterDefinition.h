//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

#ifndef EXPERIMENTPARAMETERDEFINITION_H
#define EXPERIMENTPARAMETERDEFINITION_H

#include <QObject>
#include <QVariant>
#include <QStringList>
#include <QRegularExpression>
#include <QXmlStreamReader>

#define EXPERIMENT_PARAMETER_TAG				"parameter"
#define EXPERIMENT_PARAMETERS_TAG				"parameters"
#define EXPERIMENT_PARAMETER_ID_TAG				"id"
#define EXPERIMENT_PARAMETER_ENABLED_TAG		"enabled"
#define EXPERIMENT_PARAMETER_NAME_TAG			"name"
#define EXPERIMENT_PARAMETER_DISPLAYNAME_TAG	"displayname"
#define EXPERIMENT_PARAMETER_GROUPNAME_TAG		"groupname"
#define EXPERIMENT_PARAMETER_INFORMATION_TAG	"information"
#define EXPERIMENT_PARAMETER_TYPE_TAG			"type"
#define EXPERIMENT_PARAMETER_DEFAULTVALUE_TAG	"defaultvalue"
#define EXPERIMENT_PARAMETER_RESTRICTION_TAG	"restriction"
#define EXPERIMENT_PARAMETER_MINIMAL_TAG		"minimal"
#define EXPERIMENT_PARAMETER_MAXIMAL_TAG		"maximal"
#define EXPERIMENT_PARAMETER_VALUE_TAG			"value"
#define EXPERIMENT_PARAMETER_ALLOWED_TAG		"allowed"
#define EXPERIMENT_PARAMETER_DEPENDENCIES_TAG	"dependencies"
#define EXPERIMENT_PARAMETER_DEPENDENCY_TAG		"dependency"
#define EXPERIMENT_PARAMETER_RELATION_TAG		"relation"
#define EXPERIMENT_PARAMETER_REGEXP_TAG			"regularexpression"
#define EXPERIMENT_PARAMETER_PATTERN_TAG		"pattern"
#define EXPERIMENT_PARAMETER_CASESENSITIVE_TAG	"casesensitive"
#define EXPERIMENT_PARAMETER_LISTSEP_CHAR		";"

enum ExperimentParameterSection
{
	Experiment_ParameterSection_Root					= 0,
	Experiment_ParameterSection_Parameter				= 1,
	Experiment_ParameterSection_Restriction				= 2,
	Experiment_ParameterSection_Restriction_Minimal		= 3,
	Experiment_ParameterSection_Restriction_Maximal		= 4,
	Experiment_ParameterSection_Restriction_RegExp		= 5,
	Experiment_ParameterSection_Dependency				= 6,
	Experiment_ParameterSection_Dependency_RegExp		= 7
};

enum ExperimentParameterTypeName
{
	Experiment_ParameterType_Unknown			= 0,
	Experiment_ParameterType_String				= 1,
	Experiment_ParameterType_StringArray		= 2,
	Experiment_ParameterType_Color				= 3,
	Experiment_ParameterType_Integer			= 4,
	Experiment_ParameterType_Float				= 5,
	Experiment_ParameterType_Double				= 6,
	Experiment_ParameterType_Boolean			= 7,
	Experiment_ParameterType_RotationDirection	= 8
	//See also below here!!!	
};

struct mapParamDefinitionsTypesStrc : QMap<QString, ExperimentParameterTypeName>
{
	mapParamDefinitionsTypesStrc()
	{
		this->operator[]( "string" )			= Experiment_ParameterType_String;
		this->operator[]( "stringarray" )		= Experiment_ParameterType_StringArray;
		this->operator[]( "color" )				= Experiment_ParameterType_Color;
		this->operator[]( "integer" )			= Experiment_ParameterType_Integer;
		this->operator[]( "float" )				= Experiment_ParameterType_Float;
		this->operator[]( "double" )			= Experiment_ParameterType_Double;
		this->operator[]( "boolean" )			= Experiment_ParameterType_Boolean;
		this->operator[]( "rotationdirection" )	= Experiment_ParameterType_RotationDirection;
	};
	~mapParamDefinitionsTypesStrc(){};
};

struct ExperimentParameterMinMaxRestrictionStrc
{
	bool bEnabled;
	QVariant vValue;
	ExperimentParameterMinMaxRestrictionStrc()
	{
		this->bEnabled = false;
		this->vValue = NULL;
	};
	~ExperimentParameterMinMaxRestrictionStrc() {};
};

struct ExperimentParameterDefinitionRestrictionStrc
{
	QStringList lAllowedValues;
	ExperimentParameterMinMaxRestrictionStrc MinimalValue;
	ExperimentParameterMinMaxRestrictionStrc MaximalValue;
	QRegularExpression rRegularExpression;
};

struct ExperimentParameterDefinitionDependencyStrc
{
	int nId;
	QString sDependencyName;
	QRegularExpression rRegularExpression;
	ExperimentParameterDefinitionDependencyStrc()
	{
		this->nId = -1;
		this->sDependencyName = "";
	};
	~ExperimentParameterDefinitionDependencyStrc() {};
};

struct ExperimentParameterDefinitionStrc
{
	int nId;
	bool bEnabled;
	QString sName;
	QString sDisplayName;
	QString sGroupName;//Can also be a directory like "Root;Item 1;SubItem A"
	QString sInformation;
	ExperimentParameterTypeName eType;
	QString sDefaultValue;
	//QList<ExperimentParameterDefinitionRestrictionStrc> Restrictions;
	ExperimentParameterDefinitionRestrictionStrc Restriction;
	QList<ExperimentParameterDefinitionDependencyStrc> Dependencies;

	ExperimentParameterDefinitionStrc()
	{
		this->nId = -1;
		this->bEnabled = false;
		this->sName = "";
		this->sDisplayName = "";
		this->sGroupName = "";
		this->sInformation = "";
		this->eType = Experiment_ParameterType_Unknown;
		this->sDefaultValue = "";
	};
	~ExperimentParameterDefinitionStrc() {};
};


class ExperimentParameterDefinitionContainer
{
public:

	ExperimentParameterDefinitionContainer();
	~ExperimentParameterDefinitionContainer();
	
	bool LoadFromFile(const QString &sFilePath);//const QUrl &SFileUrl);
	int getID(const QString &sName);
	QString getName(const int &nId);
	ExperimentParameterDefinitionStrc *item(const int &nId);

private:

	ExperimentParameterDefinitionStrc *parseParameterDefinition(QXmlStreamReader& xml);
	bool addElementDataToStructure(QXmlStreamReader& xml, ExperimentParameterDefinitionStrc &expParamDefStrc, ExperimentParameterSection &expParamSection) const;

	QList<ExperimentParameterDefinitionStrc> expParamDefinitions;
	mapParamDefinitionsTypesStrc mParamDefinitionsTypesMap;
};

#endif // EXPERIMENTPARAMETERDEFINITION_H
