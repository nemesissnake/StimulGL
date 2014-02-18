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

#include "ExperimentBlockParameterView.h"
#include "ExperimentParameterWidgets.h"
#include "ParameterPropertyExtensions.h"
#include "ExperimentParameterVisualizer.h"
#include "../ExperimentStructures.h"
#include "../Global.h"
#include "ExperimentTreeItem.h"
#include "ExperimentTreeModel.h"

ExperimentBlockParameterView::ExperimentBlockParameterView(QWidget *parent) : QTableWidget(parent)
{
	parsedExpStruct = NULL;
	bEditHandlingEnabled = false;
	cChangedParameter.setNamedColor("red");
	cInheritedParameter.setNamedColor("grey");
	configureEditHandling(true);
}

ExperimentBlockParameterView::~ExperimentBlockParameterView()
{
	configureEditHandling(false);
	if (parsedExpStruct)
		parsedExpStruct = NULL;
	emit destroyed(this);
}

void ExperimentBlockParameterView::initTableSetup()
{
	this->clear();
	lColumnHeaders.clear();
	lVerticalHeaders.clear();

	ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
	ExperimentParameterDefinitionContainer *tmpDefs = expParamWidgets->getExperimentParameterDefinition(BLOCK_TAG);
	int nParamID;
	for (int i=0;i<BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT;i++)
	{
		//if(i==BLOCKPARAMVIEW_BLOCKNUMBER_COLUMNINDEX)
		//	nParamID = tmpDefs->getFirstParameterID(BLOCKNUMBER_TAG);
		//else 
		if(i==BLOCKPARAMVIEW_BLOCKNAME_COLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(NAME_TAG);
		else if(i==BLOCKPARAMVIEW_BLOCKTRIALS_COLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(TRIALAMOUNT_TAG);
		else if(i==BLOCKPARAMVIEW_BLOCKINTTRGS_COLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(INTERNALTRIGGERAMOUNT_TAG);
		else if(i==BLOCKPARAMVIEW_BLOCKEXTTRGS_COLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(EXTERNALTRIGGERAMOUNT_TAG);
		if(nParamID >= 0)
		{
			lColumnHeaders.append(tmpDefs->getParameterDefinition(nParamID)->sDisplayName);
		}
		else
		{
			lColumnHeaders.append("<unknown header>");
		}
	}
	this->setColumnCount(lColumnHeaders.count());
	this->setHorizontalHeaderLabels(lColumnHeaders);	
}

void ExperimentBlockParameterView::resizeView(const int &nWidth, const int &nHeight)
{
	this->setFixedSize(nWidth-5,nHeight-5);//Fill available area
}

bool ExperimentBlockParameterView::parseExperimentStructure(cExperimentStructure *ExpStruct)
{
	int nExpBlockCount = ExpStruct->getBlockCount();
	this->setRowCount(nExpBlockCount);
	initTableSetup();		
	cBlockStructure *tmpBlock = NULL;
	int nNextSearchBlockNumber = 0;
	QString sExpName = ExpStruct->getExperimentName();
	QTableWidgetItem *tmpItem;
	configureEditHandling(false);
	hashBlockIdRowIndex.clear();
	hashRowIndexBlockId.clear();
	for (int i=0;i<nExpBlockCount;i++)
	{
		tmpBlock = NULL;
		tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
		if(tmpBlock) 
		{
			lVerticalHeaders.append(tr("%1").arg(tmpBlock->getBlockNumber()));
			//tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getBlockNumber()));
			//this->setItem(i, BLOCKPARAMVIEW_BLOCKNUMBER_COLUMNINDEX, tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getBlockName()));
			this->setItem(i, BLOCKPARAMVIEW_BLOCKNAME_COLUMNINDEX, tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfTrials()));
			this->setItem(i, BLOCKPARAMVIEW_BLOCKTRIALS_COLUMNINDEX, tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfInternalTriggers()));
			this->setItem(i, BLOCKPARAMVIEW_BLOCKINTTRGS_COLUMNINDEX, tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfExternalTriggers()));
			this->setItem(i, BLOCKPARAMVIEW_BLOCKEXTTRGS_COLUMNINDEX, tmpItem);
			hashBlockIdRowIndex[tmpBlock->getBlockID()] = i;
			hashRowIndexBlockId[i] = tmpBlock->getBlockID();
			nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
		}
	}
	this->setVerticalHeaderLabels(lVerticalHeaders);
	parsedExpStruct = ExpStruct;
	this->resizeColumnsToContents();
	configureEditHandling(true);	
	return true;
}

bool ExperimentBlockParameterView::parseExperimentBlockParameters(const QList<ExperimentTreeItem*> &tmpExpTreeItemList)
{
	hashExpParamBlockChanges.clear();
	int nBlockCount = tmpExpTreeItemList.count();
	configureEditHandling(false);
	if(nBlockCount>0)
	{		
		ExperimentTreeItem* pTmpExpTreeItem;
		QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
		int nBlockID = -1;
		int nNrOfObjects = -1;
		int nObjectID = -1;
		int nParameterID = -1;
		int nNrOfParameters = -1;
		QString sParamName;
		QStringList lObjectSearchPath;
		QStringList lObjectParameterSearchPath;
		lObjectSearchPath << OBJECT_TAG;
		lObjectParameterSearchPath << PARAMETERS_TAG << PARAMETER_TAG;
		for (int i=0;i<nBlockCount;i++) //for each block
		{
			nBlockID = -1;
			nNrOfObjects = -1;
			nObjectID = -1;
			nParameterID = -1;
			nNrOfParameters = -1;
			
			//BlockID
			pTmpExpTreeItem = tmpExpTreeItemList.at(i);
			tTmpTreeItemDefs = pTmpExpTreeItem->getDefinitions();
			if(tTmpTreeItemDefs.contains(ID_TAG))
			{
				nBlockID = tTmpTreeItemDefs[ID_TAG].value.toInt();
			}
			else
			{
				continue;
			}

			//Objects
			QList<ExperimentTreeItem*> lExpTreeObjectItems;
			nNrOfObjects = ExperimentTreeModel::getStaticTreeElements(lObjectSearchPath, lExpTreeObjectItems, tmpExpTreeItemList.at(i));
			if(nNrOfObjects > 0)
			{
				for (int j=0;j<nNrOfObjects;j++)//For each Object
				{

					//ObjectID
					tTmpTreeItemDefs = lExpTreeObjectItems.at(j)->getDefinitions();
					if(tTmpTreeItemDefs.contains(ID_TAG))
					{
						nObjectID = tTmpTreeItemDefs[ID_TAG].value.toInt();
										
						//Parameters
						QList<ExperimentTreeItem*> lExpTreeObjectParameterItems;
						nNrOfParameters = ExperimentTreeModel::getStaticTreeElements(lObjectParameterSearchPath, lExpTreeObjectParameterItems, lExpTreeObjectItems.at(j));
						if(nNrOfParameters > 0)
						{
							for (int k=0;k<nNrOfParameters;k++)//For each Parameter
							{
								//ParameterID
								tTmpTreeItemDefs = lExpTreeObjectParameterItems.at(k)->getDefinitions();
								if(tTmpTreeItemDefs.contains(ID_TAG))
									nParameterID = tTmpTreeItemDefs[ID_TAG].value.toInt();
								else
									continue;

								//ParameterName
								sParamName = "";
								pTmpExpTreeItem = lExpTreeObjectParameterItems.at(k)->firstChild(NAME_TAG);
								if(pTmpExpTreeItem)
									sParamName = pTmpExpTreeItem->getValue().toLower();
								else
									continue;

								//ParameterValue
								pTmpExpTreeItem = lExpTreeObjectParameterItems.at(k)->firstChild(VALUE_TAG);
								if(pTmpExpTreeItem)
								{
									strcParameterBlockChanges paramBlockChanges;
									paramBlockChanges.sValue = pTmpExpTreeItem->getValue();
									//expandExperimentBlockParameterValue(tmpValue);
									paramBlockChanges.nBlockID = nBlockID;

									QString uniqueObjParamIdentifier = tr("%1%2%3").arg(nObjectID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(sParamName);
									if(hashExpParamBlockChanges.contains(uniqueObjParamIdentifier))
									{
										hashExpParamBlockChanges[uniqueObjParamIdentifier].append(paramBlockChanges);
									}
									else
									{
										QList<strcParameterBlockChanges> tmpParameterBlockChangesList;
										tmpParameterBlockChangesList.append(paramBlockChanges);
										hashExpParamBlockChanges.insert(uniqueObjParamIdentifier, tmpParameterBlockChangesList);
									}
								}
							}
						}
					}				
				}
			}
		}
	}
	bool bResult = appendExperimentBlockParameterChanges();
	configureEditHandling(true);
	return bResult;
}

bool ExperimentBlockParameterView::setExperimentObjects(const QList<ExperimentStructuresNameSpace::strcExperimentObject> &lExperimentObjects) 
{
	hashObjectIdExperimentObjectInfo.clear();
	if(lExperimentObjects.isEmpty() == false)
	{
		ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
		for(int i=0;i<lExperimentObjects.count();i++)
		{
			strcExperimentObjectInfo tmpObjectInfo;
			tmpObjectInfo.ObjectGlobalInfo = lExperimentObjects[i];
			tmpObjectInfo.pObjectParamDefContainer = expParamWidgets->getExperimentParameterDefinition(lExperimentObjects[i].sClass);
			hashObjectIdExperimentObjectInfo[lExperimentObjects[i].nID] = tmpObjectInfo;
		}
		strcExperimentObjectInfo tmpObjectInfo;
		tmpObjectInfo.ObjectGlobalInfo.nID = BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID;
		tmpObjectInfo.ObjectGlobalInfo.sClass = BLOCK_TAG;
		tmpObjectInfo.ObjectGlobalInfo.sName = "<undefined>";
		tmpObjectInfo.pObjectParamDefContainer = expParamWidgets->getExperimentParameterDefinition(tmpObjectInfo.ObjectGlobalInfo.sClass);
		hashObjectIdExperimentObjectInfo[tmpObjectInfo.ObjectGlobalInfo.nID] = tmpObjectInfo;
	}
	return true;
}

bool ExperimentBlockParameterView::appendExperimentBlockParameterChanges()
{
	hashObjectParameterColumnIndex.clear();
	hashColumnIndexObjectIDParamName.clear();
	if(hashExpParamBlockChanges.count() > 0)
	{		
		int nParamCounter=0;
		int nObjectID;
		QString sParamName;
		QString sObjectIDParamName;
		QStringList lObjectIDParamName;
		QTableWidgetItem *tmpItem = NULL;
		int nColumnIndex;
		ExperimentParameterDefinitionContainer *pTempObjectParamDefContainer;

		//////////////DEFAULT BLOCK SECTION START//////////////////////
		strcColumnInfo tmpColumnInfo;
		QString sObjectParamIdentifier;
		int nParamID;
		pTempObjectParamDefContainer = hashObjectIdExperimentObjectInfo[BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID].pObjectParamDefContainer;
		for (int i=0; i<BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT; i++)
		{
			if(i==BLOCKPARAMVIEW_BLOCKNAME_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(NAME_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKTRIALS_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(TRIALAMOUNT_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKINTTRGS_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(INTERNALTRIGGERAMOUNT_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKEXTTRGS_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(EXTERNALTRIGGERAMOUNT_TAG);
			
			tmpColumnInfo.sHeader = lColumnHeaders.at(i);
			tmpColumnInfo.nColumnIndex = i;
			tmpColumnInfo.nObjectID = BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID;
			if(pTempObjectParamDefContainer)
			{
				nParamID = pTempObjectParamDefContainer->getFirstParameterID(NAME_TAG);
				if(nParamID >=0)
					tmpColumnInfo.strcParamDef = pTempObjectParamDefContainer->getParameterDefinition(nParamID);
			}
			hashObjectParameterColumnIndex[sObjectParamIdentifier] = tmpColumnInfo;
			hashColumnIndexObjectIDParamName[tmpColumnInfo.nColumnIndex] = sObjectParamIdentifier;
		}
		//////////////DEFAULT BLOCK SECTION STOP////////////////////////
	
		foreach(QList<strcParameterBlockChanges> tmpParamBlockChangesList,hashExpParamBlockChanges)//For each unique Parameter
		{
			foreach(strcParameterBlockChanges tmpParamBlockChanges,tmpParamBlockChangesList)//For each Parameter change (per block)
			{
				sObjectIDParamName = hashExpParamBlockChanges.keys().at(nParamCounter);
				lObjectIDParamName = sObjectIDParamName.split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER);
				if(lObjectIDParamName.count() > 1)
				{
					nObjectID = lObjectIDParamName.at(0).toInt();
					pTempObjectParamDefContainer = hashObjectIdExperimentObjectInfo[nObjectID].pObjectParamDefContainer;
					if(pTempObjectParamDefContainer)
					{
						nParamID = pTempObjectParamDefContainer->getFirstParameterID(lObjectIDParamName.at(1));
						sParamName = pTempObjectParamDefContainer->getParameterDefinition(nParamID)->sDisplayName;
					}
					else
					{
						sParamName = lObjectIDParamName.at(1);
					}
					if(hashObjectParameterColumnIndex.contains(sObjectIDParamName))
					{
						nColumnIndex = hashObjectParameterColumnIndex[sObjectIDParamName].nColumnIndex;
					}
					else
					{
						this->setColumnCount(lColumnHeaders.count()+1);
						lColumnHeaders.append(sParamName);
						strcColumnInfo tmpColumnInfo;
						tmpColumnInfo.sHeader = sParamName;
						tmpColumnInfo.nColumnIndex = lColumnHeaders.count()-1;
						tmpColumnInfo.nObjectID = nObjectID;
						if(pTempObjectParamDefContainer)
						{
							if(nParamID >=0)
								tmpColumnInfo.strcParamDef = pTempObjectParamDefContainer->getParameterDefinition(nParamID);
						}
						hashObjectParameterColumnIndex[sObjectIDParamName] = tmpColumnInfo;
						hashColumnIndexObjectIDParamName[tmpColumnInfo.nColumnIndex] = sObjectIDParamName;
						nColumnIndex = tmpColumnInfo.nColumnIndex;
					}
					if(hashBlockIdRowIndex.contains(tmpParamBlockChanges.nBlockID))
					{						
						tmpItem = new QTableWidgetItem(tr("%1").arg(resolveParameterValueType(tmpParamBlockChanges.sValue, hashObjectParameterColumnIndex[sObjectIDParamName].strcParamDef->eType, true).toString()));
						tmpItem->setTextColor(cChangedParameter);
						this->setItem(hashBlockIdRowIndex[tmpParamBlockChanges.nBlockID], nColumnIndex, tmpItem);						
					}
				}
			}
			nParamCounter++;
		}
		this->setHorizontalHeaderLabels(lColumnHeaders);

		//Fill-in remaining default and buffered Parameter values. We can expect here that the Block Numbers are ordered!
		int nColumnCount = columnCount();
		if(nColumnCount > BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT)
		{
			int nRowCount = rowCount();
			ExperimentParameterDefinitionStrc *tmpExpParDefStruct = NULL;
			QString sLatestValue;
			for (int c=(BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT);c<nColumnCount;c++)
			{
				sLatestValue = "";
				for (int r=0;r<nRowCount;r++)
				{
					if(item(r,c) == NULL)
					{
						if(r==0)
						{
							tmpExpParDefStruct = hashObjectParameterColumnIndex[hashColumnIndexObjectIDParamName[c]].strcParamDef;
							if(tmpExpParDefStruct)
								sLatestValue = tmpExpParDefStruct->sDefaultValue;
						}
						tmpItem = new QTableWidgetItem(sLatestValue);
						tmpItem->setTextColor(cInheritedParameter);
						this->setItem(r,c,tmpItem);
					}
					else
					{
						sLatestValue = item(r,c)->text();
					}
				}
			}
		}
	}
	return true;
}

QVariant ExperimentBlockParameterView::resolveParameterValueType(const QVariant &vInput, const ExperimentParameterTypeName &sType, const bool &bToView) const
{
	if(bToView)
	{
		if(sType == Experiment_ParameterType_Boolean)
		{
			if(vInput.toString().contains("true",Qt::CaseInsensitive))
				return "True";
			else if(vInput.toString().contains("false",Qt::CaseInsensitive))
				return "False";
		}
		else if(sType == Experiment_ParameterType_RotationDirection)
		{
			return RotationDirectionPropertyWidget::rotationDirectionString((RotationDirectionPropertyWidget::RotationDirectionEnum)vInput.toInt());
		}
		else if(sType == Experiment_ParameterType_EccentricityDirection)
		{
			return EccentricityDirectionPropertyWidget::eccentricityDirectionString((EccentricityDirectionPropertyWidget::EccentricityDirectionEnum)vInput.toInt());
		}
		else if(sType == Experiment_ParameterType_MovementDirection)
		{
			return MovementDirectionPropertyWidget::movementDirectionString((MovementDirectionPropertyWidget::MovementDirectionEnum)vInput.toInt());
		}
		else
		{
			return vInput;
		}
		//Experiment_ParameterType_Unknown
		//Experiment_ParameterType_String
		//Experiment_ParameterType_StringArray
		//Experiment_ParameterType_Color
		//Experiment_ParameterType_Integer
		//Experiment_ParameterType_Float
		//Experiment_ParameterType_Double
	}
	else
	{
		//if(sType == Experiment_ParameterType_Boolean)
		if(sType == Experiment_ParameterType_RotationDirection)
		{
			return (int)RotationDirectionPropertyWidget::rotationDirectionEnum(vInput.toString());
		}
		else if(sType == Experiment_ParameterType_EccentricityDirection)
		{
			return (int)EccentricityDirectionPropertyWidget::eccentricityDirectionEnum(vInput.toString());
		}
		else if(sType == Experiment_ParameterType_MovementDirection)
		{
			return (int)MovementDirectionPropertyWidget::movementDirectionEnum(vInput.toString());
		}
		else
		{
			return vInput;
		}
	}
	return NULL;
}

void ExperimentBlockParameterView::configureEditHandling(const bool &bEnable) 
{
	mutexEditHandlingEnabled.lock();
	if(bEnable && (bEditHandlingEnabled==false))
	{
		bEditHandlingEnabled = connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(cellItemChanged(int, int)),Qt::UniqueConnection);	
		//bEditHandlingEnabled = bEditHandlingEnabled && connect(ExperimentParameterWidgets::instance(), SIGNAL(ParameterWidgetChanged(QWidget*, const QString&)), this, SLOT(widgetItemChanged(QWidget*, const QString&)),Qt::UniqueConnection);
	}
	else if((bEnable==false) && bEditHandlingEnabled)
	{
		bEditHandlingEnabled = !((disconnect(this, SIGNAL(cellChanged(int, int)), this, SLOT(cellItemChanged(int, int)))));// && (disconnect(ExperimentParameterWidgets::instance(), SIGNAL(ParameterWidgetChanged(QWidget*, const QString&)), this, SLOT(widgetItemChanged(QWidget*, const QString&)))));		
	}
	mutexEditHandlingEnabled.unlock();
}

void ExperimentBlockParameterView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(current);
	removeCellWidget(previous.row(), previous.column());
}

void ExperimentBlockParameterView::selectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
	QModelIndexList tmpModelIndexList = this->selectedIndexes();
	if(tmpModelIndexList.count()==1)
	{
		QStringList tmpObjectIDParamNameList = hashColumnIndexObjectIDParamName[tmpModelIndexList.at(0).column()].split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER);
		if(tmpObjectIDParamNameList.count() > 1)
		{
			ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
			//QString tmpParamName = tmpObjectIDParamNameList.at(1);
			int tmpInt = tmpObjectIDParamNameList.at(0).toInt();
			//QString tmpStr = hashObjectIdExperimentObjectInfo[tmpInt].sClass;//like RETINOTOPYMAPPER_NAME
			if((tmpInt<hashObjectIdExperimentObjectInfo.count()) || (tmpInt==BLOCKPARAMVIEW_BLOCKOBJECT_INDEXID))
			{
				QString sDerivedPrefixName = QString::number(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.nID) + ":" + QString::number(hashRowIndexBlockId[tmpModelIndexList.at(0).row()]);
				QString sUniqueGeneratedPropertyIdentifier = "";
				QTableWidgetItem *tmpItem = item(tmpModelIndexList.at(0).row(), tmpModelIndexList.at(0).column());
				QVariant vResolvedParameterValue = "";
				bool bDoValueInit = false;
				if(tmpItem)
				{
					ExperimentParameterDefinitionStrc *tmpDef = hashObjectParameterColumnIndex[hashColumnIndexObjectIDParamName[tmpModelIndexList.at(0).column()]].strcParamDef;
					if(tmpDef)
					{
						vResolvedParameterValue = resolveParameterValueType(tmpItem->text(),tmpDef->eType,false);
					}
					else
					{
						vResolvedParameterValue = tmpItem->text();
					}
					bDoValueInit = true;
				}
				configureEditHandling(false);


				QWidget *tmpWidget = NULL;
				ExperimentParameterVisualizer *tmpParamVis = expParamWidgets->getExperimentParameterWidget(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass);
				if(tmpParamVis)
				{
					bool bTempPropertyEditSignaling = tmpParamVis->hasPropertyEditSignaling();
					if(bTempPropertyEditSignaling)
						tmpParamVis->configurePropertyEditSignaling(false);
					tmpWidget = tmpParamVis->getParameterEditWidget(tmpObjectIDParamNameList.at(1), sDerivedPrefixName, sUniqueGeneratedPropertyIdentifier, vResolvedParameterValue, bDoValueInit);
				
					if(tmpWidget)
					{					
						tmpWidget->setAutoFillBackground(true);
						setCellWidget(tmpModelIndexList.at(0).row(), tmpModelIndexList.at(0).column(), tmpWidget);					
						if(tmpItem)
						{
							ExperimentParameterDefinitionStrc *tmpDef = hashObjectParameterColumnIndex[hashColumnIndexObjectIDParamName[tmpModelIndexList.at(0).column()]].strcParamDef;
							if(tmpDef)
							{
								expParamWidgets->setWidgetParameter(sUniqueGeneratedPropertyIdentifier, hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass, resolveParameterValueType(tmpItem->text(),tmpDef->eType,false).toString(), true);
							}						
						}					
						ExperimentParameterVisualizer* tmpVisualizer = expParamWidgets->getExperimentParameterWidget(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass);
						connect((QObject*)tmpVisualizer, SIGNAL(derivedItemEditFinished(const QString&, const QString&)), this, SLOT(cellItemEditFinished(const QString&, const QString&)),Qt::ConnectionType(Qt::UniqueConnection | Qt::DirectConnection));					
					}
					if(bTempPropertyEditSignaling)
						tmpParamVis->configurePropertyEditSignaling(true);
				}
				configureEditHandling(true);
			}
		}
	}
	QAbstractItemView::selectionChanged(selected,deselected);
}

//void ExperimentBlockParameterView::widgetItemChanged(QWidget *pWidget, const QString &sNewValue)
//{
//	if(cellWidget(currentRow(),currentColumn()) == pWidget)
//	{
//		this->setItem(currentRow(),currentColumn(),new QTableWidgetItem(sNewValue));
//	}
//}

void ExperimentBlockParameterView::cellItemEditFinished(const QString& sParamName, const QString& sNewValue)
{
	Q_UNUSED(sParamName);
	if(bEditHandlingEnabled)
	{
		int nRow = currentRow();
		int nColumn = currentColumn();
		int nObjectID = hashObjectParameterColumnIndex[hashColumnIndexObjectIDParamName[nColumn]].nObjectID;
		int nBlockID = hashRowIndexBlockId[nRow];
		emit onItemEditFinished(nBlockID,nObjectID,sParamName,sNewValue);
		QTableWidgetItem *tmpItem = new QTableWidgetItem(tr("%1").arg(resolveParameterValueType(sNewValue, hashObjectParameterColumnIndex[hashColumnIndexObjectIDParamName[nColumn]].strcParamDef->eType, true).toString()));
		this->setItem(nRow,nColumn, tmpItem);
	}
}

void ExperimentBlockParameterView::cellItemChanged(const int &nRow, const int &nColumn)
{
	Q_UNUSED(nRow);
	Q_UNUSED(nColumn);
}
