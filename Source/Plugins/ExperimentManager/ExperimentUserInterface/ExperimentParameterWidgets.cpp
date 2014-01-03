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

#include "ExperimentParameterWidgets.h"
#include "ExperimentParameterVisualizer.h"
#include "global.h"

ExperimentParameterWidgets* ExperimentParameterWidgets::m_Instance = 0;

ExperimentParameterWidgets::ExperimentParameterWidgets()
{
	lExperimentParameterDefinitions = NULL;
	fetchExperimentParameterDefinitions();
	createExperimentParameterWidgets();
}

ExperimentParameterWidgets::~ExperimentParameterWidgets()
{
	if(lExperimentParameterDefinitions)
	{
		if(lExperimentParameterDefinitions->isEmpty() == false)
		{
			for (int i=0;i<lExperimentParameterDefinitions->count();i++)
			{
				if((*lExperimentParameterDefinitions)[i].cExperimentParameterDefinition)
					delete (*lExperimentParameterDefinitions)[i].cExperimentParameterDefinition;
				if((*lExperimentParameterDefinitions)[i].wWidget)
					delete (*lExperimentParameterDefinitions)[i].wWidget;
				(*lExperimentParameterDefinitions)[i].sCollectionName = "";
			}
			lExperimentParameterDefinitions->clear();
		}
	}
}

void ExperimentParameterWidgets::fetchExperimentParameterDefinitions()
{
	if(lExperimentParameterDefinitions)
		return;
	else
		lExperimentParameterDefinitions = new QList<ExperimentParameterDefinitionCollection>;
	QString fileString = RETINOTOPYMAPPER_PARAMDEF_PATH;
	ExperimentParameterDefinitionCollection tmpParDefCollection;

	tmpParDefCollection.sCollectionName = RETINOTOPYMAPPER_NAME;
	tmpParDefCollection.cExperimentParameterDefinition = new ExperimentParameterDefinitionContainer();
	tmpParDefCollection.cExperimentParameterDefinition->loadFromFile(fileString);
	lExperimentParameterDefinitions->append(tmpParDefCollection);
}

void ExperimentParameterWidgets::createExperimentParameterWidgets()
{
	if(lExperimentParameterDefinitions == NULL)
		return;

	for (QList<ExperimentParameterDefinitionCollection>::iterator itParamDefs=lExperimentParameterDefinitions->begin();itParamDefs!=lExperimentParameterDefinitions->end();++itParamDefs)
	{
		if(itParamDefs->wWidget == NULL)
		{
			itParamDefs->wWidget = new ExperimentParameterVisualizer();
			itParamDefs->wWidget->addGroupProperties(itParamDefs->cExperimentParameterDefinition->getGroupDefinitions());
			//connect(itParamDefs->wWidget, SIGNAL(destroyed(QWidget*)), this, SLOT(childWidgetDestroyed(QWidget*)));
			//connect(this, SIGNAL(onTableViewRedrawned(int, int)), itParamDefs->wWidget, SLOT(resizeParameterView(int, int)));
			
			QList<ExperimentParameterDefinitionStrc> *tmpExpParamDefList = itParamDefs->cExperimentParameterDefinition->getParameterDefinitions();
			if(tmpExpParamDefList == NULL)
				continue;
			for (QList<ExperimentParameterDefinitionStrc>::const_iterator itParamDef=tmpExpParamDefList->cbegin();itParamDef!=tmpExpParamDefList->cend();++itParamDef)
			{
				if(itParamDef->bEnabled)
				{
					if(itParamDefs->wWidget->addParameterProperty(&(*itParamDef),itParamDef->sDefaultValue) == false)
					{
						QMessageLogger::qDebug() << __FUNCTION__ << "Could not add parameter property: " << itParamDef->sName;
					}
				}
			}
		}
	}
}

ExperimentParameterVisualizer *ExperimentParameterWidgets::getExperimentParameterWidget(const QString &sCollectionName)
{
	if((lExperimentParameterDefinitions == NULL) || (sCollectionName.isEmpty()))
		return NULL;

	for (QList<ExperimentParameterDefinitionCollection>::iterator itParamDefs=lExperimentParameterDefinitions->begin();itParamDefs!=lExperimentParameterDefinitions->end();++itParamDefs)
	{
		if(itParamDefs->sCollectionName.compare(sCollectionName,Qt::CaseInsensitive) == 0)
		{
			if(itParamDefs->wWidget)
			{
				return itParamDefs->wWidget;
			}
			return NULL;
		}		
	}
	return NULL;
}

ExperimentParameterDefinitionContainer *ExperimentParameterWidgets::getExperimentParameterDefinition(const QString &sCollectionName)
{
	if(lExperimentParameterDefinitions->isEmpty())
		return NULL;
	for(int i=0;i<lExperimentParameterDefinitions->count();i++)
	{
		if(lExperimentParameterDefinitions->at(i).sCollectionName == sCollectionName)
			return lExperimentParameterDefinitions->at(i).cExperimentParameterDefinition;
	}	
	return NULL;
}
