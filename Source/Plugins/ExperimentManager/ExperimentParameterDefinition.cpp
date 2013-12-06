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

#include "ExperimentParameterDefinition.h"
#include "Global.h"
#include <QUrl>
#include <QFile>
#include <QDebug>

ExperimentParameterDefinitionContainer::ExperimentParameterDefinitionContainer()
{
	LoadFromFile("");
	return;

	ExperimentParameterDefinitionStrc tmpExpParamDef;
	ExperimentParameterDefinitionRestrictionStrc tmpExpParamDefRestriction;
	ExperimentParameterDefinitionDependencyStrc tmpExpParamDefDependency;
	//Initialize with some values

	tmpExpParamDef.nId = 0;
	tmpExpParamDef.bEnabled = true;
	tmpExpParamDef.sName = "showfixpoint";
	tmpExpParamDef.sDisplayName = "ShowFixPoint";
	tmpExpParamDef.sGroupName = "Fixation Point";
	tmpExpParamDef.sInformation = "The Visibility of the Fixation Point";
	tmpExpParamDef.eType = Experiment_ParameterType_Boolean;
	tmpExpParamDef.sDefaultValue = "true";
	tmpExpParamDef.Restriction.MinimalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MinimalValue.vMinMaxValue = 0;
	tmpExpParamDef.Restriction.MaximalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MaximalValue.vMinMaxValue = 0;	
	//tmpExpParamDefRestriction.rRegularExpression.setPattern("");
	//tmpExpParamDefRestriction.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Restrictions.append(tmpExpParamDefRestriction);
	//tmpExpParamDefDependency.nDenpendencyId = 0;
	//tmpExpParamDefDependency.rRegularExpression.setPattern("");
	//tmpExpParamDefDependency.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Dependencies.append(tmpExpParamDefDependency);
	expParamDefinitions.append(tmpExpParamDef);
	//expParamDefinitions.last().Restriction.lAllowedValues << "PolarAngle" << "Eccentricity" << "MovingBar" << "MovingDots" << "Fixation" << "Custom";

	tmpExpParamDef.nId = 1;
	tmpExpParamDef.bEnabled = true;
	tmpExpParamDef.sName = "retinopattern";
	tmpExpParamDef.sDisplayName = "RetinoPattern";
	tmpExpParamDef.sGroupName = "Global Properties";
	tmpExpParamDef.sInformation = "The Retinotopic Pattern to use";
	tmpExpParamDef.eType = Experiment_ParameterType_String;
	tmpExpParamDef.sDefaultValue = "PolarAngle";
	tmpExpParamDef.Restriction.MinimalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MinimalValue.vMinMaxValue = 0;
	tmpExpParamDef.Restriction.MaximalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MaximalValue.vMinMaxValue = 0;	
	//tmpExpParamDefRestriction.rRegularExpression.setPattern("");
	//tmpExpParamDefRestriction.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Restrictions.append(tmpExpParamDefRestriction);
	//tmpExpParamDefDependency.nDenpendencyId = 0;
	//tmpExpParamDefDependency.rRegularExpression.setPattern("");
	//tmpExpParamDefDependency.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Dependencies.append(tmpExpParamDefDependency);
	expParamDefinitions.append(tmpExpParamDef);
	expParamDefinitions.last().Restriction.lAllowedValues << "PolarAngle" << "Eccentricity" << "MovingBar" << "MovingDots" << "Fixation" << "Custom";

	tmpExpParamDef.nId = 2;
	tmpExpParamDef.bEnabled = true;
	tmpExpParamDef.sName = "fixationsize";
	tmpExpParamDef.sDisplayName = "FixationSize";
	tmpExpParamDef.sGroupName = "Fixation Point";
	tmpExpParamDef.sInformation = "The Size of the Fixation Point";
	tmpExpParamDef.eType = Experiment_ParameterType_Integer;
	tmpExpParamDef.sDefaultValue = "8";
	tmpExpParamDef.Restriction.MinimalValue.bEnabled = true;
	tmpExpParamDef.Restriction.MinimalValue.vValue = 0;
	tmpExpParamDef.Restriction.MaximalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MaximalValue.vValue = 0;	
	//tmpExpParamDefRestriction.rRegularExpression.setPattern("");
	//tmpExpParamDefRestriction.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Restrictions.append(tmpExpParamDefRestriction);
	//tmpExpParamDefDependency.nDenpendencyId = 0;
	//tmpExpParamDefDependency.rRegularExpression.setPattern("");
	//tmpExpParamDefDependency.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Dependencies.append(tmpExpParamDefDependency);
	expParamDefinitions.append(tmpExpParamDef);
	//expParamDefinitions.last().Restriction.lAllowedValues << "PolarAngle" << "Eccentricity" << "MovingBar" << "MovingDots" << "Fixation" << "Custom";

	tmpExpParamDef.nId = 3;
	tmpExpParamDef.bEnabled = true;
	tmpExpParamDef.sName = "fixationcolor";
	tmpExpParamDef.sDisplayName = "FixationColor";
	tmpExpParamDef.sGroupName = "Fixation Point";
	tmpExpParamDef.sInformation = "The Color of the Fixation Point";
	tmpExpParamDef.eType = Experiment_ParameterType_Color;
	tmpExpParamDef.sDefaultValue = "#FF0000";
	tmpExpParamDef.Restriction.MinimalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MinimalValue.vValue = 0;
	tmpExpParamDef.Restriction.MaximalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MaximalValue.vValue = 0;	
	//tmpExpParamDefRestriction.rRegularExpression.setPattern("#([0-9a-fA-f][0-9a-fA-f])\\g{1}\\g{1}");
	//tmpExpParamDefRestriction.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Restrictions.append(tmpExpParamDefRestriction);
	tmpExpParamDefDependency.sDependencyName = "RetinoPattern";
	tmpExpParamDefDependency.rRegularExpression.setPattern("polarangle");//see below case in-sensitive option!
	tmpExpParamDefDependency.rRegularExpression.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
	tmpExpParamDef.Dependencies.append(tmpExpParamDefDependency);
	expParamDefinitions.append(tmpExpParamDef);
	//expParamDefinitions.last().Restriction.lAllowedValues << "PolarAngle" << "Eccentricity" << "MovingBar" << "MovingDots" << "Fixation" << "Custom";
}

ExperimentParameterDefinitionContainer::~ExperimentParameterDefinitionContainer()
{
	if(expParamDefinitions.isEmpty() == false)
	{
		//for (int i=0;i<expParamDefinitions.count();i++)
		//{
		//	expParamDefinitions[i].Restrictions.clear();
		//}
		expParamDefinitions.clear();
	}
}

int ExperimentParameterDefinitionContainer::getID(const QString &sName)
{
	if(expParamDefinitions.isEmpty())
		return -1;
	QString sNameToLower = sName.toLower();
	for (int i=0;i<expParamDefinitions.count();i++)
	{
		if(expParamDefinitions.at(i).sName == sNameToLower)
			return expParamDefinitions.at(i).nId;
	}
	return -1;
}

QString ExperimentParameterDefinitionContainer::getName(const int &nId)
{
	if((expParamDefinitions.isEmpty()) || (nId<0))
		return "";
	for (int i=0;i<expParamDefinitions.count();i++)
	{
		if(expParamDefinitions.at(i).nId == nId)
			return expParamDefinitions.at(i).sName;
	}
	return "";
}

ExperimentParameterDefinitionStrc *ExperimentParameterDefinitionContainer::item(const int &nId)
{
	if((expParamDefinitions.isEmpty()) || (nId<0))
		return NULL;
	for (int i=0;i<expParamDefinitions.count();i++)
	{
		if(expParamDefinitions.at(i).nId == nId)
			return &expParamDefinitions[i];
	}
	return NULL;
}

bool ExperimentParameterDefinitionContainer::LoadFromFile(const QString &sFilePath)
{
	QString fileString = "qrc:/resources/RetinotopyMapper.xdef";
	QUrl fileUrl = QUrl(fileString);
	fileString = "C:\\Users\\sven.gijsen\\Desktop\\desktop.xdef";//":/resources/RetinotopyMapper.xdef";

	QFile* file = new QFile(fileString);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) 
	{
        qDebug() << __FUNCTION__ << "Couldn't open the file " << fileString;
        return false;
    }
    QXmlStreamReader xml(file);
	/* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() && !xml.hasError()) 
	{
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) 
            continue;
        else if(token == QXmlStreamReader::StartElement) 
		{
            if(xml.name() == EXPERIMENT_PARAMETERS_TAG)
                continue;
            else if(xml.name() == EXPERIMENT_PARAMETER_TAG)
                expParamDefinitions.append(*(this->parseParameterDefinition(xml)));
        }
    }
    /* Error handling. */
    if(xml.hasError()) 
		qDebug() << __FUNCTION__ << xml.errorString();

    /* Removes any device() or data from the reader and resets its internal state to the initial state. */
    xml.clear();
	return true;
}

ExperimentParameterDefinitionStrc *ExperimentParameterDefinitionContainer::parseParameterDefinition(QXmlStreamReader& xml)
{
	ExperimentParameterDefinitionStrc *tmpParamDef = NULL;
    /* Let's check that we're really getting a parameter definition. */
    if((xml.tokenType() != QXmlStreamReader::StartElement) || (xml.name() != EXPERIMENT_PARAMETER_TAG))
        return NULL;
	ExperimentParameterSection expParamCurrectSection = Experiment_ParameterSection_Parameter;
    /* Let's get the attributes for parameter definition */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that the parameter definition has a id attribute. */
	int nID = -1;
	//QVariant::Type vType;
    if(attributes.hasAttribute(EXPERIMENT_PARAMETER_ID_TAG)) 
	{
        /* We'll add it to the map. */		
		nID = attributes.value(EXPERIMENT_PARAMETER_ID_TAG).toInt();
		if(nID < 0)
			return NULL;
		tmpParamDef = new ExperimentParameterDefinitionStrc;
		tmpParamDef->nId = nID;
    }
    /* Next element... */
    xml.readNext();
    /* We're going to loop because the order might change.
     * We'll continue the loop until we hit an EndElement property definition.  */
	QStringRef tmpStringRef = xml.name();
	QXmlStreamReader::TokenType tmpToken = xml.tokenType();
    while(!((tmpToken == QXmlStreamReader::EndElement) && (tmpStringRef == EXPERIMENT_PARAMETER_TAG))) 
	{		
        if(tmpToken == QXmlStreamReader::StartElement)
		{
			if(tmpStringRef == EXPERIMENT_PARAMETER_RESTRICTION_TAG)
			{
				expParamCurrectSection = Experiment_ParameterSection_Restriction;
			}
			else if(tmpStringRef == EXPERIMENT_PARAMETER_DEPENDENCY_TAG)
			{
				expParamCurrectSection = Experiment_ParameterSection_Dependency;
				QXmlStreamAttributes depAttributes = xml.attributes();
				/* Let's check that the parameter dependency has a id attribute. */
				int nDepID = -1;
				if(depAttributes.hasAttribute(EXPERIMENT_PARAMETER_ID_TAG)) 
				{
					/* We'll add it to the map. */		
					nDepID = depAttributes.value(EXPERIMENT_PARAMETER_ID_TAG).toInt();
					if((nDepID < 0) || (tmpParamDef == NULL))
						return NULL;
					ExperimentParameterDefinitionDependencyStrc tmpStruct;
					tmpStruct.nId = nDepID;
					tmpParamDef->Dependencies.append(tmpStruct);
				}
			}
			else if((expParamCurrectSection == Experiment_ParameterSection_Dependency) && (tmpStringRef == EXPERIMENT_PARAMETER_REGEXP_TAG))
			{
				expParamCurrectSection = Experiment_ParameterSection_Dependency_RegExp;
			}
			else if((expParamCurrectSection == Experiment_ParameterSection_Restriction) && (tmpStringRef == EXPERIMENT_PARAMETER_MINIMAL_TAG))
			{
				expParamCurrectSection = Experiment_ParameterSection_Restriction_Minimal;
			}
			else if((expParamCurrectSection == Experiment_ParameterSection_Restriction) && (tmpStringRef == EXPERIMENT_PARAMETER_MAXIMAL_TAG))
			{
				expParamCurrectSection = Experiment_ParameterSection_Restriction_Maximal;
			}
			else if((expParamCurrectSection == Experiment_ParameterSection_Restriction) && (tmpStringRef == EXPERIMENT_PARAMETER_REGEXP_TAG))
			{
				expParamCurrectSection = Experiment_ParameterSection_Restriction_RegExp;
			}
			else
			{
				this->addElementDataToStructure(xml, *tmpParamDef,expParamCurrectSection);
			}
		}
		else if(tmpToken == QXmlStreamReader::EndElement)
		{
			if((tmpStringRef == EXPERIMENT_PARAMETER_RESTRICTION_TAG) || (tmpStringRef == EXPERIMENT_PARAMETER_DEPENDENCY_TAG))
			{
				expParamCurrectSection = Experiment_ParameterSection_Parameter;
			}			
			else if(tmpStringRef == EXPERIMENT_PARAMETER_REGEXP_TAG) 
			{
				if(expParamCurrectSection == Experiment_ParameterSection_Dependency_RegExp)
					expParamCurrectSection = Experiment_ParameterSection_Dependency;
				else if(expParamCurrectSection == Experiment_ParameterSection_Restriction_RegExp)
					expParamCurrectSection = Experiment_ParameterSection_Restriction;
			}
			else if(tmpStringRef == EXPERIMENT_PARAMETER_MINIMAL_TAG) 
			{
				if(expParamCurrectSection == Experiment_ParameterSection_Restriction_Minimal)
					expParamCurrectSection = Experiment_ParameterSection_Restriction;
			}
			else if(tmpStringRef == EXPERIMENT_PARAMETER_MAXIMAL_TAG) 
			{
				if(expParamCurrectSection == Experiment_ParameterSection_Restriction_Maximal)
					expParamCurrectSection = Experiment_ParameterSection_Restriction;
			}
		}
        xml.readNext();
		tmpStringRef = xml.name();
		tmpToken = xml.tokenType();
    }
    return tmpParamDef;
}

bool ExperimentParameterDefinitionContainer::addElementDataToStructure(QXmlStreamReader& xml, ExperimentParameterDefinitionStrc &expParamDefStrc, ExperimentParameterSection &expParamSection) const
{
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement)
        return false;
    /* Let's read the property definition... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /* This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave. */
    if(xml.tokenType() != QXmlStreamReader::Characters)
        return false;  

	if(expParamSection == Experiment_ParameterSection_Parameter)
	{
		/* Now we can add it to the map.*/
		if(elementName == EXPERIMENT_PARAMETER_ENABLED_TAG) 
		{
			bool bNewValue = false;
			const QString tmpString = xml.text().trimmed().toString();
			if(tmpString == BOOL_TRUE_TAG)
			{
				bNewValue = true;
			}
			else if(tmpString != BOOL_FALSE_TAG)
			{
				qDebug() << __FUNCTION__ << "wrong defined boolean value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			expParamDefStrc.bEnabled = bNewValue;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_NAME_TAG) 
		{
			const QString tmpString = xml.text().trimmed().toString();
			if(tmpString.isEmpty())
			{
				qDebug() << __FUNCTION__ << "non defined string value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			expParamDefStrc.sName = tmpString;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_DISPLAYNAME_TAG) 
		{
			const QString tmpString = xml.text().trimmed().toString();
			if(tmpString.isEmpty())
			{
				qDebug() << __FUNCTION__ << "non defined string value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			expParamDefStrc.sDisplayName = tmpString;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_GROUPNAME_TAG) 
		{
			expParamDefStrc.sGroupName = xml.text().trimmed().toString();//tmpString;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_INFORMATION_TAG) 
		{
			expParamDefStrc.sInformation = xml.text().trimmed().toString();//tmpString;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_TYPE_TAG) 
		{
			const QString tmpString = xml.text().trimmed().toString();
			ExperimentParameterTypeName tmpExpParTypeName = mParamDefinitionsTypesMap[tmpString];
			if(tmpExpParTypeName == Experiment_ParameterType_Unknown)
			{
				qDebug() << __FUNCTION__ << "unknown defined string value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			expParamDefStrc.eType = tmpExpParTypeName;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_DEFAULTVALUE_TAG) 
		{
			expParamDefStrc.sDefaultValue = xml.text().trimmed().toString();//tmpString;
			return true;
		}
	}
	else if((expParamSection == Experiment_ParameterSection_Restriction_Minimal) || (expParamSection == Experiment_ParameterSection_Restriction_Maximal))
	{
		if(elementName == EXPERIMENT_PARAMETER_ENABLED_TAG) 
		{
			bool bNewValue = false;
			const QString tmpString = xml.text().trimmed().toString();
			if(tmpString == BOOL_TRUE_TAG)
			{
				bNewValue = true;
			}
			else if(tmpString != BOOL_FALSE_TAG)
			{
				qDebug() << __FUNCTION__ << "wrong defined boolean value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			if(expParamSection == Experiment_ParameterSection_Restriction_Minimal)
				expParamDefStrc.Restriction.MinimalValue.bEnabled = bNewValue;
			else if(expParamSection == Experiment_ParameterSection_Restriction_Maximal)
				expParamDefStrc.Restriction.MaximalValue.bEnabled = bNewValue;
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_VALUE_TAG) 
		{
			if(expParamSection == Experiment_ParameterSection_Restriction_Minimal)
				expParamDefStrc.Restriction.MinimalValue.vValue = xml.text().trimmed().toString();
			else if(expParamSection == Experiment_ParameterSection_Restriction_Maximal)
				expParamDefStrc.Restriction.MaximalValue.vValue = xml.text().trimmed().toString();
			return true;
		}
	}
	else if(expParamSection == Experiment_ParameterSection_Restriction)
	{
		if(elementName == EXPERIMENT_PARAMETER_ALLOWED_TAG) 
		{
			QString tmpString = xml.text().trimmed().toString();
			if(tmpString.isEmpty())
				return true;
			const QStringList tmpStringList = tmpString.split(EXPERIMENT_PARAMETER_LISTSEP_CHAR,QString::SkipEmptyParts);
			if(tmpStringList.isEmpty())
				return true;
			expParamDefStrc.Restriction.lAllowedValues = tmpStringList;
			return true;
		}
	}
	else if(expParamSection == Experiment_ParameterSection_Dependency)
	{
		if(elementName == EXPERIMENT_PARAMETER_RELATION_TAG) 
		{
			const QString tmpString = xml.text().trimmed().toString();
			if(tmpString.isEmpty())
			{
				qDebug() << __FUNCTION__ << "non defined string value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			if(expParamDefStrc.Dependencies.isEmpty())
			{
				qDebug() << __FUNCTION__ << "could not define Parameter Dependency (Doesn't exist!)" << "(> " << tmpString << ")";
				return false;
			}
			expParamDefStrc.Dependencies.last().sDependencyName = tmpString;
			return true;
		}
	}
	else if((expParamSection == Experiment_ParameterSection_Dependency_RegExp) || (expParamSection == Experiment_ParameterSection_Restriction_RegExp))
	{
		if(elementName == EXPERIMENT_PARAMETER_PATTERN_TAG) 
		{
			if(expParamSection == Experiment_ParameterSection_Dependency_RegExp)
				expParamDefStrc.Dependencies.last().rRegularExpression.setPattern(xml.text().trimmed().toString());
			else if(expParamSection == Experiment_ParameterSection_Restriction_RegExp)
				expParamDefStrc.Restriction.rRegularExpression.setPattern(xml.text().trimmed().toString());
			return true;
		}
		else if(elementName == EXPERIMENT_PARAMETER_CASESENSITIVE_TAG) 
		{
			QRegularExpression::PatternOptions tmpOptions = QRegularExpression::NoPatternOption;
			const QString tmpString = xml.text().trimmed().toString();
			if(tmpString == BOOL_FALSE_TAG)
			{
				tmpOptions = QRegularExpression::CaseInsensitiveOption;
			}
			else if(tmpString != BOOL_TRUE_TAG)
			{
				qDebug() << __FUNCTION__ << "wrong defined boolean value for parameter " << elementName << "(> " << tmpString << ")";
				return false;
			}
			if(expParamSection == Experiment_ParameterSection_Dependency_RegExp)
				expParamDefStrc.Dependencies.last().rRegularExpression.setPatternOptions(tmpOptions);
			else if(expParamSection == Experiment_ParameterSection_Restriction_RegExp)
				expParamDefStrc.Restriction.rRegularExpression.setPatternOptions(tmpOptions);
			return true;
		}
	}
	return false;
}