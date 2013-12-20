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
	groupManager = NULL;
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
	deleteSubGroupProperties(&lGroupPropertyCollection.propItem);
	if(groupManager)
		delete groupManager;
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

void ExperimentParameterVisualizer::deleteSubGroupProperties(QList<propertyContainerItem> *pRootGroupPropertyItem)
{
	if(pRootGroupPropertyItem == NULL) 
		return;
	if(pRootGroupPropertyItem->isEmpty() == false)
	{
		for (QList<propertyContainerItem>::iterator it=pRootGroupPropertyItem->begin();it!=pRootGroupPropertyItem->end();++it)
		{
			if(it->pSubItems)
			{
				deleteSubGroupProperties(it->pSubItems);
				it->pSubItems->clear();
			}
			delete it->lGroupProperty;
			delete it->pSubItems;
		}
		pRootGroupPropertyItem->clear();
		return;
	}
}

bool ExperimentParameterVisualizer::addPropertyToSubGroup(const QString &sPropertyGroupNames, QtVariantProperty *item1, QList<propertyContainerItem> *pRootGroupPropertyItemList, QString &sSandPath)
{	
	if(groupManager == NULL)
		groupManager = new QtGroupPropertyManager(this);
	if(sPropertyGroupNames.isEmpty())
		return false;
	if(pRootGroupPropertyItemList == NULL)
		return false;
	
	QStringList tmpPropGroups = sPropertyGroupNames.split(EXPERIMENT_LISTSEP_CHAR, QString::SkipEmptyParts);
	propertyContainerItem tmpPropertyContainerItem;
	
	if(pRootGroupPropertyItemList->isEmpty() == false)
	{
		for (int j=0;j<pRootGroupPropertyItemList->count();j++)
		{
			if(pRootGroupPropertyItemList->at(j).lGroupProperty->propertyId() == sSandPath + tmpPropGroups.at(0))//Case sensitive!
			{
				if(tmpPropGroups.count() == 1)
				{
					pRootGroupPropertyItemList->at(j).lGroupProperty->addSubProperty(item1);
					return true;
				}
				else
				{
					sSandPath = sSandPath + tmpPropGroups.first() + ";";
					tmpPropGroups.removeFirst();
					if(pRootGroupPropertyItemList->at(j).pSubItems == NULL)
						(*pRootGroupPropertyItemList)[j].pSubItems = new QList<propertyContainerItem>;
					
					bool bGroupIsPresent = false;
					for (QSet<QtProperty*>::const_iterator it=groupManager->properties().cbegin();it!=groupManager->properties().cend();++it)
					{
						if((*it)->propertyId() == sSandPath + tmpPropGroups.at(0))//Case sensitive!
						{
							bGroupIsPresent = true;
							break;
						}
					}
					if(bGroupIsPresent == false)
					{
						QtProperty *tmpProperty = groupManager->addProperty(tmpPropGroups.at(0));
						tmpProperty->setPropertyId(sSandPath + tmpPropGroups.at(0));
						pRootGroupPropertyItemList->at(j).lGroupProperty->addSubProperty(tmpProperty);
						tmpPropertyContainerItem.pSubItems = NULL;
						tmpPropertyContainerItem.lGroupProperty = tmpProperty;
						pRootGroupPropertyItemList->at(j).pSubItems->append(tmpPropertyContainerItem);
					}
					return addPropertyToSubGroup(tmpPropGroups.join(EXPERIMENT_LISTSEP_CHAR), item1, pRootGroupPropertyItemList->at(j).pSubItems,sSandPath);
				}
			}
		}
	}	
	tmpPropertyContainerItem.pSubItems = NULL;
	tmpPropertyContainerItem.lGroupProperty = groupManager->addProperty(sPropertyGroupNames);
	tmpPropertyContainerItem.lGroupProperty->setPropertyId(sSandPath + sPropertyGroupNames);
	pRootGroupPropertyItemList->append(tmpPropertyContainerItem);
	int nPropertyGroupIndex = pRootGroupPropertyItemList->count() - 1;	
	pRootGroupPropertyItemList->at(nPropertyGroupIndex).lGroupProperty->addSubProperty(item1);
	propertyEditor->addProperty(pRootGroupPropertyItemList->at(nPropertyGroupIndex).lGroupProperty);
	return true;
}

bool ExperimentParameterVisualizer::addGroupProperties(const QList<ExperimentGroupDefinitionStrc> *expParamDef)
{
	if(expParamDef == NULL)
		return false;
	if(expParamDef->isEmpty())
		return false;
	if(lVariantPropertyManager == NULL)
	{
		lVariantPropertyManager = new VariantExtensionPropertyManager(this);
		connect(lVariantPropertyManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));
	}
	if(groupManager == NULL)
		groupManager = new QtGroupPropertyManager(this);	

	int nPropertyGroupIndex;
	QList<propertyContainerItem> *pCurrentRootGroupPropertyItemList = NULL;
	QString sSandPath;
	//QString tmpGroupName;
	QStringList sCurrentRelativePathItems;	
	QtProperty *parentProperty;
	bool bFirstLoop;
	bool bItemAlreadyExists;

	for (QList<ExperimentGroupDefinitionStrc>::const_iterator it=expParamDef->cbegin();it!=expParamDef->cend();++it)
	{
		pCurrentRootGroupPropertyItemList = &lGroupPropertyCollection.propItem;
		bFirstLoop = true;
		bItemAlreadyExists;
		parentProperty = NULL;
		if((it->bEnabled) && (it->sGroupPath.isEmpty() == false))
		{
			sSandPath = "";
			//tmpGroupName = it->sGroupName;
			sCurrentRelativePathItems = it->sGroupPath.split(EXPERIMENT_LISTSEP_CHAR,QString::SkipEmptyParts);
			if(sCurrentRelativePathItems.isEmpty())
				continue;
			while(sCurrentRelativePathItems.isEmpty() == false)
			{
				//Does it already exist in the current level?
				bItemAlreadyExists = false;
				for (QList<propertyContainerItem>::iterator itProp=pCurrentRootGroupPropertyItemList->begin();itProp!=pCurrentRootGroupPropertyItemList->end();++itProp)
				{
					QString sPropID = itProp->lGroupProperty->propertyId();
					if(sPropID.startsWith(sSandPath + sCurrentRelativePathItems.at(0)))
					{
						bItemAlreadyExists = true;
						//Make sure that a sub-group list is available
						if(itProp->pSubItems == NULL)
							itProp->pSubItems = new QList<propertyContainerItem>;
						//Set Parent and Increment the current property group pointer
						parentProperty = itProp->lGroupProperty;
						pCurrentRootGroupPropertyItemList = itProp->pSubItems; 
						break;
					}
				}
				if(bItemAlreadyExists == false)
				{
					propertyContainerItem tmpPropertyContainerItem;
					tmpPropertyContainerItem.pSubItems = NULL;
					tmpPropertyContainerItem.lGroupProperty = groupManager->addProperty(sCurrentRelativePathItems.at(0));
					tmpPropertyContainerItem.lGroupProperty->setPropertyId(sSandPath + sCurrentRelativePathItems.at(0));

					//Append this newly created group property
					pCurrentRootGroupPropertyItemList->append(tmpPropertyContainerItem);		
					nPropertyGroupIndex = pCurrentRootGroupPropertyItemList->count() - 1;
					QtProperty *tmpProperty = pCurrentRootGroupPropertyItemList->at(nPropertyGroupIndex).lGroupProperty;

					if(bFirstLoop)
						propertyEditor->addProperty(tmpProperty);
					else
						parentProperty->addSubProperty(tmpProperty);

					if(sCurrentRelativePathItems.count() > 1)//Are there more sub-group's to check/add?
					{
						//Make sure that a sub-group list is available
						if(pCurrentRootGroupPropertyItemList->at(nPropertyGroupIndex).pSubItems == NULL)
							(*pCurrentRootGroupPropertyItemList)[nPropertyGroupIndex].pSubItems = new QList<propertyContainerItem>;
						//Set Parent and Increment the current property group pointer
						parentProperty = pCurrentRootGroupPropertyItemList->at(nPropertyGroupIndex).lGroupProperty;
						pCurrentRootGroupPropertyItemList = pCurrentRootGroupPropertyItemList->at(nPropertyGroupIndex).pSubItems;
					}
					else
					{
						//Configure Dependencies?
						if(it->Dependencies.isEmpty() == false)
						{
							for (QList<ExperimentParameterDefinitionDependencyStrc>::const_iterator itDep=it->Dependencies.cbegin();itDep!=it->Dependencies.cend();++itDep)
							{
								addDependency((QtVariantProperty *)tmpProperty,(*itDep));
							}
						}
					}
				}
				sSandPath = sSandPath + sCurrentRelativePathItems.takeFirst() + EXPERIMENT_LISTSEP_CHAR;
				bFirstLoop = false;
			}
		}		
	}
	return true;
}

bool ExperimentParameterVisualizer::addParameterProperty(const ExperimentParameterDefinitionStrc *expParamDef, const QVariant &vValue)
{
	if(expParamDef == NULL)
		return false;

	bool bDoMinMax = false;
	bool bDoEnumeratedList = false;

	if(lVariantPropertyManager == NULL)
	{
		lVariantPropertyManager = new VariantExtensionPropertyManager(this);
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
	else if(expParamDef->eType == Experiment_ParameterType_RotationDirection)
	{
		varType = (QVariant::Type) VariantExtensionPropertyManager::rotationDirectionTypeId();
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
	else if ((expParamDef->eType == Experiment_ParameterType_Float) || (expParamDef->eType == Experiment_ParameterType_Double))
	{
		varType = QVariant::Double;
		bDoMinMax = true;
	}
	else if(expParamDef->eType == Experiment_ParameterType_StringArray)
	{
		varType = (QVariant::Type) VariantExtensionPropertyManager::stringArrayTypeId();
	}
	else
	{
		varType = QVariant::String;
	}
	
	QtVariantProperty *item1 = lVariantPropertyManager->addProperty(varType,expParamDef->sDisplayName);
	item1->setPropertyId(QString::number(expParamDef->nId));

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
	VariantExtensionPropertyFactory *tmpFactory = new VariantExtensionPropertyFactory(this);
	propertyEditor->setFactoryForManager((QtVariantPropertyManager*)lVariantPropertyManager, (QtVariantEditorFactory*)tmpFactory);
		
	if (expParamDef->sGroupPath.isEmpty())
	{
		propertyEditor->addProperty(item1);//Here we do not use a subgroup, use root location
	}
	else
	{
		addPropertyToSubGroup(expParamDef->sGroupPath, item1, &lGroupPropertyCollection.propItem);
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
			//if(vValue.type() == )
			QString tmpStr = vValue.toString();
			lVariantPropertyManager->setValue(item1,vValue.toString());
		}		
	}
	else if(varType == (QVariant::Type) VariantExtensionPropertyManager::rotationDirectionTypeId())
	{
		lVariantPropertyManager->setValue(item1,vValue.toInt());//VariantExtensionPropertyManager::RotationDirectionEnum
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
	if(bAutoDepencyParsing)
		parseDependencies((QtVariantProperty*) property);
}

bool ExperimentParameterVisualizer::addDependency(QtVariantProperty *variantProperty, const ExperimentParameterDefinitionDependencyStrc &dependencyParamDef)
{
	for (QList<propertyDependencyStruct>::iterator it=lPropertyDependencies.begin();it!=lPropertyDependencies.end();++it)
	{
		if(it->vProperty == variantProperty)
		{
			it->definitions.append(dependencyParamDef);
			return true;
		}
	}
	propertyDependencyStruct tmpPropertyDependency;
	tmpPropertyDependency.vProperty = variantProperty;
	lPropertyDependencies.append(tmpPropertyDependency);
	lPropertyDependencies.last().definitions.append(dependencyParamDef);
	return true;
}

bool ExperimentParameterVisualizer::parseDependencies(QtVariantProperty *variantProperty)
{
	if(lPropertyDependencies.isEmpty())
		return true;
	if(lVariantPropertyManager == NULL)
		return true;

	QList<propertyDependencyStruct>::iterator itDependency;
	//First make sure everything is enabled
	for (itDependency = lPropertyDependencies.begin(); itDependency != lPropertyDependencies.end(); ++itDependency)
	{
		if(variantProperty != NULL)
		{
			for (QList<ExperimentParameterDefinitionDependencyStrc>::const_iterator itDef=itDependency->definitions.cbegin();itDef!=itDependency->definitions.cend();++itDef)
			{
				if(QString::number(itDef->nDependencyParameterID) == variantProperty->propertyId())
				{
					itDependency->vProperty->setEnabled(true);
					break;
				}
			}
		}
		else
		{
			if(itDependency->vProperty)
				itDependency->vProperty->setEnabled(true);
		}
	}
	//Now we need to check what to disable
	for (itDependency = lPropertyDependencies.begin(); itDependency != lPropertyDependencies.end(); ++itDependency)
	{
		if(variantProperty != NULL)
		{
			bool bPropertyFound = false;
			for (QList<ExperimentParameterDefinitionDependencyStrc>::const_iterator itDef=itDependency->definitions.cbegin();itDef!=itDependency->definitions.cend();++itDef)
			{
				if(QString::number(itDef->nDependencyParameterID) == variantProperty->propertyId())
				{
					//QString sName = variantProperty->propertyName();
					//int n1 = itDef->nDependencyParameterID;
					//QString s2 = variantProperty->propertyId().toLower();
					bPropertyFound = true;
					break;  
				}
			}
			if(bPropertyFound == false)
				continue;
		}
		for (QSet<QtProperty*>::const_iterator it=lVariantPropertyManager->properties().cbegin();it!=lVariantPropertyManager->properties().cend();++it)
		{
			int nID = -1;
			QString tmpS = (*it)->propertyId();
			if(tmpS.isEmpty())
				continue;
			if(tmpS == "0")
			{
				nID = 0;
			}
			else
			{
				nID = tmpS.toInt();
				if(nID == 0)
					continue;
			}
			for (QList<ExperimentParameterDefinitionDependencyStrc>::const_iterator itDef=itDependency->definitions.cbegin();itDef!=itDependency->definitions.cend();++itDef)
			{
				if(itDef->nDependencyParameterID == nID)
				{
					if(itDef->rRegularExpression.match((*it)->valueText()).hasMatch() == false)
					{
						itDependency->vProperty->setEnabled(false);
						break;
					}
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

void ExperimentParameterVisualizer::resizeParameterView(const int &nWidth, const int &nHeight)
{	
	Q_UNUSED(nWidth);
	this->setFixedWidth(300);
	this->setFixedHeight(nHeight - 50);
}


