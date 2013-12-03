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

#include "ExperimentParameter.h"

ExperimentParameterDefinitionContainer::ExperimentParameterDefinitionContainer()
{
	ExperimentParameterDefinition tmpExpParamDef;
	ExperimentParameterDefinitionRestriction tmpExpParamDefRestriction;
	ExperimentParameterDefinitionDependency tmpExpParamDefDependency;
	//Initialize with some values

	tmpExpParamDef.nId = 0;
	tmpExpParamDef.bEnabled = true;
	tmpExpParamDef.sName = "showfixpoint";
	tmpExpParamDef.sDisplayName = "ShowFixPoint";
	tmpExpParamDef.sGroupName = "Fixation Point";
	tmpExpParamDef.sInformation = "The Visibility of the Fixation Point";
	tmpExpParamDef.eType = Experiment_ParameterType_Boolean;
	tmpExpParamDef.vDefaultValue = true;
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
	tmpExpParamDef.vDefaultValue = "PolarAngle";
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
	tmpExpParamDef.vDefaultValue = 8;
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
	tmpExpParamDef.vDefaultValue = "#FF0000";
	tmpExpParamDef.Restriction.MinimalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MinimalValue.vValue = 0;
	tmpExpParamDef.Restriction.MaximalValue.bEnabled = false;
	//tmpExpParamDef.Restriction.MaximalValue.vValue = 0;	
	tmpExpParamDefRestriction.rRegularExpression.setPattern("#([0-9a-fA-f][0-9a-fA-f])\\g{1}\\g{1}");
	//tmpExpParamDefRestriction.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Restrictions.append(tmpExpParamDefRestriction);
	//tmpExpParamDefDependency.nDenpendencyId = 0;
	//tmpExpParamDefDependency.rRegularExpression.setPattern("");
	//tmpExpParamDefDependency.rRegularExpression.setPatternOptions();
	//tmpExpParamDef.Dependencies.append(tmpExpParamDefDependency);
	expParamDefinitions.append(tmpExpParamDef);
	//expParamDefinitions.last().Restriction.lAllowedValues << "PolarAngle" << "Eccentricity" << "MovingBar" << "MovingDots" << "Fixation" << "Custom";
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

ExperimentParameterDefinition *ExperimentParameterDefinitionContainer::item(const int &nId)
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