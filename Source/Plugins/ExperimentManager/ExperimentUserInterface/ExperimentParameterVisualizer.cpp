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

#include "ExperimentParameterVisualizer.h"
#include "ui_ExperimentParameterVisualizer.h"

ExperimentParameterVisualizer::ExperimentParameterVisualizer(QWidget *parent) : QWidget(parent)
{
	propertyEditor = NULL;
	mainLayout = NULL;
	lGroupProperties.manager = NULL;

	ui = new Ui::ExperimentParameterVisualizer();
	ui->setupUi(this);
	
	mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	propertyEditor = new QtTreePropertyBrowser();
	mainLayout->addWidget(propertyEditor);
}

ExperimentParameterVisualizer::~ExperimentParameterVisualizer()
{
	int i,j;
	if(lBoolPropertyManagers.isEmpty() == false)
	{
		for (i=0;i<lBoolPropertyManagers.count();i++)
		{
			delete lBoolPropertyManagers[i];
		}
		lBoolPropertyManagers.clear();
	}
	//if(lStringPropertyManagers.isEmpty() == false)
	//{
	//	for (i=0;i<lStringPropertyManagers.count();i++)
	//	{
	//		delete lStringPropertyManagers[i];
	//	}
	//	lStringPropertyManagers.clear();
	//}
	if(lEnumPropertyManagers.isEmpty() == false)
	{
		for (i=0;i<lEnumPropertyManagers.count();i++)
		{
			delete lEnumPropertyManagers[i];
		}
		lEnumPropertyManagers.clear();
	}
	if(lIntPropertyManagers.isEmpty() == false)
	{
		for (i=0;i<lIntPropertyManagers.count();i++)
		{
			delete lIntPropertyManagers[i];
		}
		lIntPropertyManagers.clear();
	}
	if(lVariantPropertyManagers.isEmpty() == false)
	{
		for (i=0;i<lVariantPropertyManagers.count();i++)
		{
			delete lVariantPropertyManagers[i];
		}
		lVariantPropertyManagers.clear();
	}
	
		
	
	//if(lGroupProperties.isEmpty() == false)
	//{
	//	for (int i=0;i<lGroupProperties.count();i++)
	//	{
	//		delete lGroupProperties[i]->manager;
	//		for (j=0;j<lGroupProperties[i]->lProperties.count();j++)
	//		{
	//			delete lGroupProperties[i]->lProperties[j];
	//		}
	//		lGroupProperties[i]->lProperties.clear();
	//	}
	//	lGroupProperties.clear();
	//}
	for (j=0;j<lGroupProperties.lProperties.count();j++)
	{
		delete lGroupProperties.lProperties[j];
	}
	lGroupProperties.lProperties.clear();
	if(lGroupProperties.manager)
		delete lGroupProperties.manager;

	if(propertyEditor)
	{
		delete propertyEditor;
		propertyEditor = NULL;
	}
	if(mainLayout)
	{
		delete mainLayout;
		mainLayout = NULL;
	}

	delete ui;
	emit destroyed(this);
}

int ExperimentParameterVisualizer::getPropertyGroupIndex(const QString &sPropertyGroupName)
{	
	if(sPropertyGroupName.isEmpty())
		return -2;
	else if(lGroupProperties.manager == NULL)
		return -1;
	//for (int i=0;i<lGroupProperties.count();i++)
	//{
		if(lGroupProperties.lProperties.isEmpty())
			return -1;//continue;
		for (int j=0;j<lGroupProperties.lProperties.count();j++)
		{
			if(lGroupProperties.lProperties[j]->propertyName() == sPropertyGroupName)//Case sensitive!
			{
				return j;
			}
		}
	//}
	return -1;
}

bool ExperimentParameterVisualizer::addProperty(const ExperimentParameterDefinition *expParamDef, const QVariant &vValue)
{
	if(expParamDef == NULL)
		return false;
	bool bNewGroup = false;
	bool bDoMinMax = false;
	bool bDoEnumeratedList = false;
	int nPropertyGroupIndex = getPropertyGroupIndex(expParamDef->sGroupName);
	lVariantPropertyManagers.append(new QtVariantPropertyManager(this));

	QVariant::Type varType;
	if(expParamDef->eType == Experiment_ParameterType_Color)
	{
		varType = QVariant::Color;
	}
	else if(expParamDef->eType == Experiment_ParameterType_Boolean)
	{
		varType = QVariant::Bool;		
	}
	else if(expParamDef->eType == Experiment_ParameterType_Integer)
	{
		varType = QVariant::Int;
		bDoMinMax = true;
	}
	else if(expParamDef->eType == Experiment_ParameterType_String)
	{		
		if(expParamDef->Restriction.lAllowedValues.isEmpty() == false)
		{
			varType = (QVariant::Type) QtVariantPropertyManager::enumTypeId();
			bDoEnumeratedList = true;
		}
		else
		{
			varType = QVariant::String;
		}
	}
	else
	{
		varType = QVariant::String;
	}

	QtVariantProperty *item1 = lVariantPropertyManagers.last()->addProperty(varType,expParamDef->sDisplayName);

	if(bDoEnumeratedList)
	{
		item1->setAttribute(QLatin1String("enumNames"), expParamDef->Restriction.lAllowedValues);
		//QMap<int, QIcon> enumIcons;
		//enumIcons[0] = QIcon(":/demo/images/up.png");
		//enumIcons[1] = QIcon(":/demo/images/right.png");
		//enumIcons[2] = QIcon(":/demo/images/down.png");
		//enumIcons[3] = QIcon(":/demo/images/left.png");
		//item1->setAttribute(QLatin1String("enumIcons"), enumIcons);
		//enumManager->setEnumIcons(item8, enumIcons);
		//set value?
	}

	if(bDoMinMax)
	{
		if(expParamDef->Restriction.MinimalValue.bEnabled)
			item1->setAttribute(QLatin1String("minimum"), expParamDef->Restriction.MinimalValue.vValue);
		if(expParamDef->Restriction.MaximalValue.bEnabled)
			item1->setAttribute(QLatin1String("maximum"), expParamDef->Restriction.MaximalValue.vValue);
	}
	
	QtVariantEditorFactory *tmpFactory = new QtVariantEditorFactory(this);
	propertyEditor->setFactoryForManager(lVariantPropertyManagers.last(), tmpFactory);

	if(nPropertyGroupIndex == -1)
	{
		if(lGroupProperties.manager == NULL)
			lGroupProperties.manager = new QtGroupPropertyManager(this);
		lGroupProperties.lProperties.append(lGroupProperties.manager->addProperty(expParamDef->sGroupName));
		nPropertyGroupIndex = lGroupProperties.lProperties.count() - 1;
		bNewGroup = true;
	}	
	if(bNewGroup)//New Subgroup defined, create a new one
	{
		lGroupProperties.lProperties.at(nPropertyGroupIndex)->addSubProperty(item1);
		propertyEditor->addProperty(lGroupProperties.lProperties.at(nPropertyGroupIndex));
	}
	else if (nPropertyGroupIndex != -2)//Subgroup already defined, use that one
	{
		lGroupProperties.lProperties.at(nPropertyGroupIndex)->addSubProperty(item1);
	}
	else//Here we do not use a subgroup, use root location
	{
		propertyEditor->addProperty(item1);
	}

	if(varType == QVariant::Color)
	{
		lVariantPropertyManagers.last()->setValue(item1,QColor(vValue.toString()));
	}
	else if(varType == QVariant::Bool)
	{
		lVariantPropertyManagers.last()->setValue(item1,vValue.toBool());
	}
	else if(varType == QVariant::Int)
	{
		lVariantPropertyManagers.last()->setValue(item1,vValue.toInt());
	}
	else if(varType == QVariant::String)
	{
		if(bDoEnumeratedList)
		{
			QString sSearchVal = vValue.toString().toLower();
			for (int i=0;i<expParamDef->Restriction.lAllowedValues.count();i++)
			{
				if(expParamDef->Restriction.lAllowedValues.at(i).toLower() == sSearchVal)
				{
					//lEnumPropertyManagers.last()->setValue(item1,i);
					lVariantPropertyManagers.last()->setValue(item1,i);
					break;
				}
			}
		}
		else
		{
			lVariantPropertyManagers.last()->setValue(item1,vValue.toString());
		}		
	}
	else
	{
		lVariantPropertyManagers.last()->setValue(item1,vValue.toString());
	}
	

	item1->setToolTip(expParamDef->sInformation);
	item1->setWhatsThis("");
	item1->setStatusTip("");
	return true;
}

