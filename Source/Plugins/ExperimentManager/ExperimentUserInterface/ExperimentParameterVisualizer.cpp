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
	lVariantPropertyManager = NULL;
	bAutoDepencyParsing = false;

	ui = new Ui::ExperimentParameterVisualizer();
	ui->setupUi(this);
	
	mainLayout = new QVBoxLayout(this);
	this->setLayout(mainLayout);
	propertyEditor = new QtTreePropertyBrowser();
	mainLayout->addWidget(propertyEditor);
}

ExperimentParameterVisualizer::~ExperimentParameterVisualizer()
{
	if(lVariantPropertyManager)
	{
		delete lVariantPropertyManager;
		lVariantPropertyManager = NULL;
	}
	for (QList<QtProperty*>::iterator it=lGroupProperties.lProperties.begin();it!=lGroupProperties.lProperties.end();++it)
	{
		delete (*it);
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
	if(lGroupProperties.lProperties.isEmpty())
		return -1;
	for (int j=0;j<lGroupProperties.lProperties.count();j++)
	{
		if(lGroupProperties.lProperties[j]->propertyName() == sPropertyGroupName)//Case sensitive!
		{
			return j;
		}
	}
	return -1;
}

bool ExperimentParameterVisualizer::addProperty(const ExperimentParameterDefinitionStrc *expParamDef, const QVariant &vValue)
{
	if(expParamDef == NULL)
		return false;

	bool bNewGroup = false;
	bool bDoMinMax = false;
	bool bDoEnumeratedList = false;
	int nPropertyGroupIndex = getPropertyGroupIndex(expParamDef->sGroupName);

	if(lVariantPropertyManager == NULL)
	{
		lVariantPropertyManager = new QtVariantPropertyManager(this);
		connect(lVariantPropertyManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));
	}

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

	QtVariantProperty *item1 = lVariantPropertyManager->addProperty(varType,expParamDef->sDisplayName);
	item1->setPropertyId(expParamDef->sName);// + "_" + QString::number(expParamDef->nId));

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
	propertyEditor->setFactoryForManager(lVariantPropertyManager, tmpFactory);

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
		lVariantPropertyManager->setValue(item1,QColor(vValue.toString()));
	}
	else if(varType == QVariant::Bool)
	{
		lVariantPropertyManager->setValue(item1,vValue.toBool());
	}
	else if(varType == QVariant::Int)
	{
		lVariantPropertyManager->setValue(item1,vValue.toInt());
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
					lVariantPropertyManager->setValue(item1,i);
					break;
				}
			}
		}
		else
		{
			lVariantPropertyManager->setValue(item1,vValue.toString());
		}		
	}
	else
	{
		lVariantPropertyManager->setValue(item1,vValue.toString());
	}
	
	item1->setToolTip(expParamDef->sInformation);
	item1->setWhatsThis("");
	item1->setStatusTip("");

	if(expParamDef->Dependencies.isEmpty() == false)
	{
		for (QList<ExperimentParameterDefinitionDependencyStrc>::const_iterator it=expParamDef->Dependencies.cbegin();it!=expParamDef->Dependencies.cend();++it)
		{
			addDependency(item1,(*it));
		}
	}

	if(bAutoDepencyParsing == false)
		return true;
	return parseDependencies();
}

void ExperimentParameterVisualizer::propertyValueChanged(QtProperty *property, const QVariant &value)
{
	Q_UNUSED(value);
	//QString sName = value.toString();
	//sName = property->valueText();
	//sName = property->propertyName();
	//sName = sName;
	if(bAutoDepencyParsing)
		parseDependencies((QtVariantProperty*) property);
}

bool ExperimentParameterVisualizer::addDependency(QtVariantProperty *variantProperty, const ExperimentParameterDefinitionDependencyStrc &dependencyParamDef)
{
	propertyDependency tmpPropertyDependency;
	tmpPropertyDependency.vProperty = variantProperty;
	tmpPropertyDependency.definition = dependencyParamDef;
	lPropertyDependencies.append(tmpPropertyDependency);
	return true;
}

bool ExperimentParameterVisualizer::parseDependencies(QtVariantProperty *variantProperty)
{
	if(lPropertyDependencies.isEmpty())
		return true;
	if(lVariantPropertyManager == NULL)
		return true;

	QList<propertyDependency>::const_iterator itDependency;
	for (itDependency = lPropertyDependencies.cbegin(); itDependency != lPropertyDependencies.cend(); ++itDependency)
	{
		if(variantProperty != NULL)
		{
			if(itDependency->definition.sDependencyName.toLower() != variantProperty->propertyName().toLower())
				continue;
		}
		for (QSet<QtProperty*>::const_iterator it=lVariantPropertyManager->properties().cbegin();it!=lVariantPropertyManager->properties().cend();++it)
		{
			QString sID = (*it)->propertyId();
			if(sID.isEmpty())
				continue;
			QString tmpName = (*it)->propertyName();
				
			int nResult = QString::compare(itDependency->definition.sDependencyName,sID,Qt::CaseInsensitive);
			if(nResult==0)
			{
				//QRegularExpression regExp(itDependency->definition.rRegularExpression, QRegularExpression::CaseInsensitiveOption);
				//QString tmpValue = (*it)->valueText();
				//QString tmpPattern = itDependency->.definition.rRegularExpression.pattern();
				//QRegularExpressionMatch match = itDependency->definition.rRegularExpression.match(tmpValue);
				if(itDependency->definition.rRegularExpression.match((*it)->valueText()).hasMatch())
				{
					itDependency->vProperty->setEnabled(true);
				}
				else
				{
					itDependency->vProperty->setEnabled(false);
				}
			}
		}
	}	
	return true;
}

void ExperimentParameterVisualizer::setAutoDepencyParsing(bool bEnable)
{
	bAutoDepencyParsing = bEnable;
}


