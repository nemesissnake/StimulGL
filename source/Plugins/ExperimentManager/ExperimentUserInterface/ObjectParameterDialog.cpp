//ExperimentManagerplugin
//Copyright (C) 2014  Sven Gijsen
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

#include "ObjectParameterDialog.h"
#include "ui_ObjectParameterDialog.h"
#include "ExperimentManager.h"
#include "ExperimentParameterWidgets.h"
#include "ParameterPropertyExtensions.h"
#include "ExperimentParameterVisualizer.h"

ObjectParameterDialog::ObjectParameterDialog(QWidget *parent) : QDialog(parent), pCurrentExpTree(NULL), pCurrentExpStructure(NULL), pParameterEditWidget(NULL), bIsParsing(false)
{
	setAttribute(Qt::WA_DeleteOnClose);

	ui = new Ui::ObjectParameterDialog();
	ui->setupUi(this);
	setWindowTitle("Configure object/block parameter(s)");

	connect(ui->pbUpdate, SIGNAL(clicked()), this, SLOT(updateParameter()));
	connect(ui->pbReset, SIGNAL(clicked()), this, SLOT(resetParameter()));
	connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui->cmbBlock, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedBlockChanged(int)));
	connect(ui->cmbObject, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedObjectChanged(int)));
	connect(ui->cmbParameter, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedParameterChanged(int)));
	connect(ui->rdbParamType_1, SIGNAL(clicked()), this, SLOT(parameterTypeChanged()));
	connect(ui->rdbParamType_2, SIGNAL(clicked()), this, SLOT(parameterTypeChanged()));
	clearAllParsedStructures();
}

ObjectParameterDialog::~ObjectParameterDialog()
{
	clearAllParsedStructures();
	delete ui;
}

void ObjectParameterDialog::parameterTypeChanged()
{
	ui->leName->clear();
	if(ui->rdbParamType_1->isChecked())//pre-defined
	{
		ui->leName->setReadOnly(true);
		parseParameters(hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()],hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()]);
	}
	else//custom
	{
		ui->leName->setReadOnly(false);	
		parseParameters(hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()],hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()], true);
	}
} 

void ObjectParameterDialog::resetParameter()
{

}

void ObjectParameterDialog::updateParameter()
{

}

void ObjectParameterDialog::selectedObjectChanged(int nIndex)
{
	Q_UNUSED(nIndex)
	if(bIsParsing == false)
	{
		if(pCurrentExpTree)
		{
			parseParameters(hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()],hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()]);
			selectedParameterChanged(ui->cmbParameter->currentIndex());
		}
	}
}

void ObjectParameterDialog::selectedBlockChanged(int nIndex)
{
	Q_UNUSED(nIndex)
	if(bIsParsing == false)
	{
		if(pCurrentExpTree)
		{
			parseParameters(hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()],hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()]);
			selectedParameterChanged(ui->cmbParameter->currentIndex());
		}
	}
}

void ObjectParameterDialog::derivedItemEditFinished(const QString& sParamName, const QString& sNewValue)
{
	Q_UNUSED(sParamName);
	//if(bEditHandlingEnabled)
	//{
	//	int nRow = currentRow();
	//	int nColumn = currentColumn();
	//	int nTempRow;
	//	int nTempColumn;
	//	int nIndex1;
	//	int nIndex2;
	//	if(bVerticalViewEnabled)
	//	{
	//		nIndex1 = nColumn;
	//		nIndex2 = nRow;
	//	}
	//	else
	//	{
	//		nIndex1 = nRow;
	//		nIndex2 = nColumn;
	//	}
	//	int nObjectID = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[nIndex2]].nObjectID;
	//	int nBlockID = hashRowOrColumnIndexBlockId[nIndex1];
	if(pCurrentExpTree)
	{
		int nObjectID = hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()];
		int nBlockID = hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()];
		if((nObjectID>=0)&&(nBlockID>=0))
		{
//		bDoReparseModel = false;
			pCurrentExpTree->saveNewData(nBlockID,nObjectID,sParamName,sNewValue);
//		bDoReparseModel = true;
		}
	}
}

void ObjectParameterDialog::selectedParameterChanged(int nIndex)
{
	Q_UNUSED(nIndex)
	if(bIsParsing == false)
	{
		ui->leValue->setText("<undefined>");
		if(pParameterEditWidget)
		{
			pParameterEditWidget->setParent(NULL);
			pParameterEditWidget = NULL;
		}
		if(nIndex >= 0)
		{
			if(hashParamControlIndexToUniqueHexParamID.contains(nIndex))
			{
				if(mapUniqueHexParamIDToParamDef.contains(hashParamControlIndexToUniqueHexParamID[nIndex]))
				{
					QString sResolvedValue;
					if(mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].bIsChanged)
					{
						//QString sVal1 = hashParamControlIndexToUniqueHexParamID[nIndex];
						//QString sVal2 = mapUniqueHexParamIDToParamDef[sVal1].sFullDisplayName;
						//QString sVal3 = mapUniqueHexParamIDToParamDef[sVal1].sValue;
						//ExperimentParameterTypeName eType1 = mapUniqueHexParamIDToParamDef[sVal1].eType;
						sResolvedValue = VariantExtensionPropertyManager::resolveParameterValueType(mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].sValue,mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].ParamDef.eType,true).toString();
					}
					else
					{
						sResolvedValue = VariantExtensionPropertyManager::resolveParameterValueType(mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].ParamDef.sDefaultValue,mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].ParamDef.eType,true).toString();
					}
					ui->leValue->setText(sResolvedValue);

					if(expParamVizualizer)
					{
						bool bTempPropertyEditSignaling = expParamVizualizer->hasPropertyEditSignaling();
						if(bTempPropertyEditSignaling)
							expParamVizualizer->configurePropertyEditSignaling(false);
							
						QString sUniqueGeneratedPropertyIdentifier = "";
						int nObjectID = hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()];
						int nBlockID = hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()];
						QString sDerivedPrefixName = tr("%1:%2").arg(nObjectID).arg(nBlockID);
						QString sParamName = mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].ParamDef.sName;
						pParameterEditWidget = expParamVizualizer->getParameterEditWidget(sParamName, sDerivedPrefixName, sUniqueGeneratedPropertyIdentifier, sResolvedValue, true);
						if(pParameterEditWidget)
						{					
							pParameterEditWidget->setAutoFillBackground(true);
						//	ExperimentParameterDefinitionStrc *tmpDef = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[nIndex2]].strcParamDef;
						//	if(tmpDef)
						//	{
						//		expParamWidgets->setWidgetParameter(sUniqueGeneratedPropertyIdentifier, hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass, VariantExtensionPropertyManager::resolveParameterValueType(tmpItem->text(),tmpDef->eType,false).toString(), true);
						//	}						
			
							//ExperimentParameterVisualizer* tmpVisualizer = expParamWidgets->getExperimentParameterWidget(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass);
							connect((QObject*)expParamVizualizer, SIGNAL(derivedItemEditFinished(const QString&, const QString&)), this, SLOT(derivedItemEditFinished(const QString&, const QString&)),Qt::ConnectionType(Qt::UniqueConnection | Qt::DirectConnection));					
							ui->lEditParameter->insertWidget(0,pParameterEditWidget);
						
							if(mapUniqueHexParamIDToParamDef[hashParamControlIndexToUniqueHexParamID[nIndex]].bIsChanged)
							{
								ui->pbReset->setEnabled(true);
								ui->pbUpdate->setEnabled(true);
								ui->pbUpdate->setText("Update value");
							}
							else
							{
								ui->pbReset->setEnabled(false);
								ui->pbUpdate->setEnabled(true);
								ui->pbUpdate->setText("Set value");
							}
						}
						if(bTempPropertyEditSignaling)
							expParamVizualizer->configurePropertyEditSignaling(true);
					}
					if(ui->rdbParamType_1->isChecked())//pre-defined
						ui->leName->setText(ui->cmbParameter->currentText());
				}				
			}
		}
	}
}

bool ObjectParameterDialog::parseExperimentTreeModel(ExperimentTreeModel *pExperimentTreeModel)
{
	bIsParsing = true;
	clearAllParsedStructures();
	pCurrentExpTree = pExperimentTreeModel;
	pCurrentExpStructure = new cExperimentStructure();	
	bool bResult;
	
	bResult = ExperimentManager::createExperimentStructure(lCurrentExpTreeItems,pCurrentExpTree,pCurrentExpStructure);
	if(bResult)
	{
		int nExpBlockCount = pCurrentExpStructure->getBlockCount();
		cBlockStructure *tmpBlock = NULL;
		int nNextSearchBlockNumber = 0;
		QString sExpName = pCurrentExpStructure->getExperimentName();
		QString tmpString;
		int i;
		for (i=0;i<nExpBlockCount;i++)
		{
			tmpBlock = NULL;
			tmpBlock = pCurrentExpStructure->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
			if(tmpBlock) 
			{
				tmpString = QString::number(tmpBlock->getBlockNumber()) + ": " + tmpBlock->getBlockName();
				ui->cmbBlock->addItem(tmpString);				
				hBlockControlIndexToBlockID.insert(i,tmpBlock->getBlockID());
				nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
			}
		}
				
		lCurrentExpObjects = pCurrentExpTree->getDefinedExperimentObjectInfoList(NULL);
		if(lCurrentExpObjects.isEmpty() == false)
		{
			for(i=0;i<lCurrentExpObjects.count();i++)
			{
				tmpString = lCurrentExpObjects[i].sName;
				ui->cmbObject->addItem(tmpString);
				hObjectControlIndexToObjectID.insert(i,lCurrentExpObjects[i].nID);
			}
		}
	}
	if(bResult)
	{
		parseParameters(hObjectControlIndexToObjectID[ui->cmbObject->currentIndex()],hBlockControlIndexToBlockID[ui->cmbBlock->currentIndex()]);
		bIsParsing = false;	
		selectedParameterChanged(ui->cmbParameter->currentIndex());
	}
	else
	{
		clearAllParsedStructures();
		bIsParsing = false;	
	}			
	return bResult;
}

void ObjectParameterDialog::clearAllParsedStructures()
{
	pCurrentExpTree = NULL;
	expParamVizualizer = NULL;
	if(pCurrentExpStructure)
	{
		delete pCurrentExpStructure;
		pCurrentExpStructure = NULL;
	}	
	if(pParameterEditWidget)
	{
		pParameterEditWidget->setParent(NULL);
		pParameterEditWidget = NULL;
	}
	lCurrentExpObjects.clear();
	lCurrentExpTreeItems.clear();
	hObjectControlIndexToObjectID.clear();
	hBlockControlIndexToBlockID.clear();
	mapUniqueHexParamIDToParamDef.clear();
	hashParamControlIndexToUniqueHexParamID.clear();
	hashParamNameToParamID.clear();
	ui->pbReset->setEnabled(false);
	ui->pbUpdate->setEnabled(false);
	ui->pbUpdate->setText("Set/Update value");
}
				
bool ObjectParameterDialog::parseParameters(const int &nObjectID, const int &nBlockID, const bool &bIsCustom)
{
	bool bWasParsing = bIsParsing;
	bIsParsing = true;	
	ui->cmbParameter->clear();
	hashParamControlIndexToUniqueHexParamID.clear();
	mapUniqueHexParamIDToParamDef.clear();
	hashParamNameToParamID.clear();
	bool bResult = false;
	if((nObjectID<0) || (nBlockID<0))
	{
		bIsParsing = bWasParsing;
		return bResult;
	}
	int nBlockCount = pCurrentExpStructure->getBlockCount();
	int nObjectCount = lCurrentExpObjects.count();

	if(bIsCustom)
	{
		bIsParsing = bWasParsing;
		return bResult;
	}


	if((nObjectCount<1) || (nBlockCount<1) || (lCurrentExpTreeItems.isEmpty()))
	{
		bIsParsing = bWasParsing;
		return bResult;	
	}
	ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
	ExperimentParameterDefinitionContainer *expParamDefContainer = NULL;
	QList<ExperimentParameterDefinitionStrc> *lTmpExpParamDefStrc = NULL;
	ExperimentParameterDefinitionStrc tmpExpParamDefStrc;
	QString tmpString;

	for(int i=0;i<lCurrentExpObjects.count();i++)
	{
		if(lCurrentExpObjects[i].nID == nObjectID)
		{
			QString sClassName = lCurrentExpObjects[i].sClass;
			expParamVizualizer = expParamWidgets->getExperimentParameterWidget(sClassName);
			expParamDefContainer = expParamWidgets->getExperimentParameterDefinition(sClassName);
			if(expParamDefContainer)
			{
				lTmpExpParamDefStrc = expParamDefContainer->getParameterDefinitions();
				break;
			}
			break;
		}
	}
	if(lTmpExpParamDefStrc == NULL)
	{
		bIsParsing = bWasParsing;
		return bResult;
	}
	foreach(tmpExpParamDefStrc,*lTmpExpParamDefStrc)
	{
		if(tmpExpParamDefStrc.bEnabled)
		{
			tmpString = tmpExpParamDefStrc.sGroupPath + ";" + tmpExpParamDefStrc.sDisplayName;
			tmpString = tmpString.replace(";"," / ");
			strcParameterDefinition tmpParamDef;
			tmpParamDef.ParamDef = tmpExpParamDefStrc;
			tmpParamDef.sFullDisplayName = tmpString;
			mapUniqueHexParamIDToParamDef["0" + QString(tmpExpParamDefStrc.nId).toLatin1().toHex()] = tmpParamDef;
			hashParamNameToParamID.insert(tmpExpParamDefStrc.sName,tmpExpParamDefStrc.nId);
		}
	}		
		
	ExperimentTreeItem* pTmpExpTreeItem;
	QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
	int nTempBlockID = -1;
	int nTempObjectID = -1;
	int nTempParameterID = -1;
	int nNrOfParameters = -1;
	int nTempCurrentBlockObjectCount = -1;
	QString sTempParamName;
	QString sTempParamValue;
	QString uniqueObjParamIdentifier;
	QStringList lObjectSearchPath;
	QStringList lObjectParameterSearchPath;
	QStringList lObjectCustomParameterSearchPath;
	lObjectSearchPath << OBJECT_TAG;
	lObjectParameterSearchPath << PARAMETERS_TAG << PARAMETER_TAG;
	lObjectCustomParameterSearchPath << CUSTOM_PARAMETERS_TAG << PARAMETER_TAG;
	for (int i=0;i<nBlockCount;i++) //for each block
	{
		nTempBlockID = -1;
		nTempObjectID = -1;
		nTempParameterID = -1;
		nNrOfParameters = -1;
		nTempCurrentBlockObjectCount = -1;

		//BlockID
		pTmpExpTreeItem = lCurrentExpTreeItems.at(i);
		tTmpTreeItemDefs = pTmpExpTreeItem->getDefinitions();
		if(tTmpTreeItemDefs.contains(ID_TAG))
		{
			nTempBlockID = tTmpTreeItemDefs[ID_TAG].value.toInt();
			if(nTempBlockID == nBlockID)
			{
				//Objects
				QList<ExperimentTreeItem*> lExpTreeObjectItems;
				nTempCurrentBlockObjectCount = ExperimentTreeModel::getStaticTreeElements(lObjectSearchPath, lExpTreeObjectItems, lCurrentExpTreeItems.at(i));
				if(nTempCurrentBlockObjectCount == 0)
					continue;
				for (int j=0;j<nTempCurrentBlockObjectCount;j++)//For each Object
				{
					//ObjectID
					tTmpTreeItemDefs = lExpTreeObjectItems.at(j)->getDefinitions();
					if(tTmpTreeItemDefs.contains(ID_TAG))
					{
						nTempObjectID = tTmpTreeItemDefs[ID_TAG].value.toInt();
						if(nTempObjectID != nObjectID)
							continue;

						//Pre-defined Parameters
						QList<ExperimentTreeItem*> lExpTreeObjectParameterItems;
						nNrOfParameters = ExperimentTreeModel::getStaticTreeElements(lObjectParameterSearchPath, lExpTreeObjectParameterItems, lExpTreeObjectItems.at(j));
						if(nNrOfParameters > 0)
						{
							for (int k=0;k<nNrOfParameters;k++)//For each Parameter
							{
								//ParameterID
								tTmpTreeItemDefs = lExpTreeObjectParameterItems.at(k)->getDefinitions();
								if(tTmpTreeItemDefs.contains(ID_TAG))
									nTempParameterID = tTmpTreeItemDefs[ID_TAG].value.toInt();
								else
									continue;
									
								//ParameterName
								sTempParamName = "";
								pTmpExpTreeItem = lExpTreeObjectParameterItems.at(k)->firstChild(NAME_TAG);
								if(pTmpExpTreeItem)
									sTempParamName = pTmpExpTreeItem->getValue();
								else
									continue;

								//ParameterValue
								pTmpExpTreeItem = lExpTreeObjectParameterItems.at(k)->firstChild(VALUE_TAG);
								if((pTmpExpTreeItem) && (sTempParamName.isEmpty() == false))
								{
									sTempParamValue = pTmpExpTreeItem->getValue();									
									tmpString = "0" + QString(hashParamNameToParamID[sTempParamName.toLower()]).toLatin1().toHex();
									if(mapUniqueHexParamIDToParamDef.contains(tmpString))
									{
										ui->cmbParameter->addItem(mapUniqueHexParamIDToParamDef[tmpString].sFullDisplayName);
										mapUniqueHexParamIDToParamDef[tmpString].bIsChanged = true;
										mapUniqueHexParamIDToParamDef[tmpString].sValue = sTempParamValue;
										hashParamControlIndexToUniqueHexParamID.insert(ui->cmbParameter->count()-1,tmpString);										
										ui->cmbParameter->setItemData(ui->cmbParameter->count()-1, QColor(Qt::red), Qt::DecorationRole);
									}
									bResult = true;
								}
							}
						}

						//Custom Parameters
						QList<ExperimentTreeItem*> lExpTreeObjectCustomParameterItems;
						nNrOfParameters = ExperimentTreeModel::getStaticTreeElements(lObjectCustomParameterSearchPath, lExpTreeObjectCustomParameterItems, lExpTreeObjectItems.at(j));
						if(nNrOfParameters > 0)
						{
							for (int k=0;k<nNrOfParameters;k++)//For each Parameter
							{
								//ParameterID
								tTmpTreeItemDefs = lExpTreeObjectCustomParameterItems.at(k)->getDefinitions();
								if(tTmpTreeItemDefs.contains(ID_TAG))
									nTempParameterID = tTmpTreeItemDefs[ID_TAG].value.toInt();
								else
									continue;

								//ParameterName
								sTempParamName = "";
								pTmpExpTreeItem = lExpTreeObjectCustomParameterItems.at(k)->firstChild(NAME_TAG);
								if(pTmpExpTreeItem)
									sTempParamName = pTmpExpTreeItem->getValue();
								else
									continue;

								//ParameterValue
								pTmpExpTreeItem = lExpTreeObjectCustomParameterItems.at(k)->firstChild(VALUE_TAG);
								if((pTmpExpTreeItem) && (sTempParamName.isEmpty() == false))
								{
									sTempParamValue = pTmpExpTreeItem->getValue();									
									tmpString = "1" + QString(hashParamNameToParamID[sTempParamName.toLower()]).toLatin1().toHex();
									if(mapUniqueHexParamIDToParamDef.contains(tmpString))
									{
										ui->cmbParameter->addItem(mapUniqueHexParamIDToParamDef[tmpString].sFullDisplayName);
										mapUniqueHexParamIDToParamDef[tmpString].bIsChanged = true;
										mapUniqueHexParamIDToParamDef[tmpString].sValue = sTempParamValue;
										hashParamControlIndexToUniqueHexParamID.insert(ui->cmbParameter->count()-1,tmpString);										
										ui->cmbParameter->setItemData(ui->cmbParameter->count()-1, QColor(Qt::red), Qt::DecorationRole);
									}
									bResult = true;
								}
							}
						}
					}						
				}
			}
		}
	}

	if(bResult)//Fill remaining undefined block parameters
	{
		for (int i=0;i<mapUniqueHexParamIDToParamDef.count();i++)
		{
			if(mapUniqueHexParamIDToParamDef.values().at(i).bIsChanged == false)
			{
				ui->cmbParameter->addItem(mapUniqueHexParamIDToParamDef.values().at(i).sFullDisplayName);
				tmpString = mapUniqueHexParamIDToParamDef.keys().at(i);
				hashParamControlIndexToUniqueHexParamID.insert(ui->cmbParameter->count()-1,tmpString);
			}			
		}
	}
	bIsParsing = bWasParsing;
	return bResult;
}
