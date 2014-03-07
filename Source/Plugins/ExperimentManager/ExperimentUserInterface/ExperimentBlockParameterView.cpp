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
#include "../ExperimentManager.h"
#include "../Global.h"
#include "ExperimentTreeItem.h"
#include "ExperimentTreeModel.h"
#include <QMenu>
#include <QHeaderView>

ExperimentBlockParameterView::ExperimentBlockParameterView(QWidget *parent, ExperimentTreeModel *pExperimentTreeModel) : QTableWidget(parent), pExpTreeModel(pExperimentTreeModel)
{
	parsedExpStruct = NULL;
	bEditHandlingEnabled = false;
	bDoReparseModel = true;
	bVerticalViewEnabled = true;
	cChangedParameter.setNamedColor("red");
	cInheritedParameter.setNamedColor("grey");
	configureEditHandling(true);
	setContextMenuPolicy(Qt::CustomContextMenu);
	this->horizontalHeader()->setDefaultAlignment(Qt::AlignTop | Qt::AlignLeft);
	this->verticalHeader()->setDefaultAlignment(Qt::AlignTop | Qt::AlignLeft);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenu(const QPoint&)));
	connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(cellOpenedForEdit(int,int)), Qt::DirectConnection);
	if(pExpTreeModel)
	{
		connect(pExpTreeModel, SIGNAL(modelModified()), this, SLOT(checkReparseModel()));
		reparseModel();
	}
}

void ExperimentBlockParameterView::checkReparseModel()
{
	if(bDoReparseModel)
		return reparseModel();
	return;
}

void ExperimentBlockParameterView::reparseModel()
{
	cExperimentStructure *tmpExpStructure = new cExperimentStructure();
	QList<ExperimentTreeItem*> tmpFoundExpTreeItemList;
	bool bResult = ExperimentManager::createExperimentStructure(tmpFoundExpTreeItemList,pExpTreeModel,tmpExpStructure);
	if(bResult)
	{
		this->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		this->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
		bResult = bResult && parseExperimentStructure(tmpExpStructure);
		if(bResult)
		{
			QList<ExperimentStructuresNameSpace::strcExperimentObject> tmpExperimentObjectList;
			tmpExperimentObjectList = pExpTreeModel->getDefinedExperimentObjectInfoList(NULL);
			bResult = bResult && setExperimentObjects(tmpExperimentObjectList);
			if(bResult)
			{
				bResult = bResult && parseExperimentBlockParameters(tmpFoundExpTreeItemList);
			}
		}
		this->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		this->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	}
}

void ExperimentBlockParameterView::showContextMenu(const QPoint& pos)
{
	QPoint globalPos = this->mapToGlobal(pos);
	QModelIndex mIndex = this->indexAt(pos);
	QModelIndexList mSelectedIndexes = selectedIndexes();
	QMenu mContexMenu;
	QAction* selectedItemAction;
	QAction* addBlockAction;
	QAction* removeBlockAction;
	QAction* removeBlocksAction;
	QAction* moveUpLeftBlocksAction;
	QAction* moveDownRightBlocksAction;
	QAction* toggleViewAction;
	QAction* resetParametersAction;
	int nSelectionCount = mSelectedIndexes.count();
	QSet<int> bUsedColumnIndexesInSelectionRanges;
	QSet<int> bUsedRowIndexesInSelectionRanges;
	QSet<int> bUsedBlockIDsInSelectionRanges;
	QList<int> lstUsedBlockIDsInSelectionRanges;
	QModelIndexList lstDefinedParamIndexesInSelectionRanges;
	QRect outerSelectionIndexes; 
	QMenu* mViewMenu = mContexMenu.addMenu("View");
	QMenu* mBlocksMenu = mContexMenu.addMenu("Blocks");
	QMenu* mParametersMenu = mContexMenu.addMenu("Parameters");
	int nTempRow;
	int nTempColumn;	

	outerSelectionIndexes.setLeft(-1);
	outerSelectionIndexes.setRight(-1);
	outerSelectionIndexes.setTop(-1);
	outerSelectionIndexes.setBottom(-1);

	for(int i=0;i<nSelectionCount;i++)
	{
		nTempRow = mSelectedIndexes.at(i).row();
		nTempColumn = mSelectedIndexes.at(i).column();

		if(item(nTempRow,nTempColumn)->textColor() == cChangedParameter)
		{
			if(((bVerticalViewEnabled)&&(nTempRow >= BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT)) || ((bVerticalViewEnabled==false)&&(nTempColumn >= BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT)))
				lstDefinedParamIndexesInSelectionRanges.append(mSelectedIndexes.at(i));
		}

		if(i==0)
		{
			outerSelectionIndexes.setLeft(nTempColumn);
			outerSelectionIndexes.setRight(nTempColumn);
			outerSelectionIndexes.setTop(nTempRow);
			outerSelectionIndexes.setBottom(nTempRow);
		}
		else
		{
			if(nTempColumn > outerSelectionIndexes.right())
				outerSelectionIndexes.setRight(nTempColumn);
			else if(nTempColumn < outerSelectionIndexes.left())
				outerSelectionIndexes.setLeft(nTempColumn);
			if(nTempRow > outerSelectionIndexes.bottom())
				outerSelectionIndexes.setBottom(nTempRow);
			else if(nTempRow < outerSelectionIndexes.top())
				outerSelectionIndexes.setTop(nTempRow);
		}

		bUsedColumnIndexesInSelectionRanges.insert(nTempColumn);
		bUsedRowIndexesInSelectionRanges.insert(nTempRow);
		if(bVerticalViewEnabled)
			bUsedBlockIDsInSelectionRanges.insert(hashRowOrColumnIndexBlockId[nTempColumn]);
		else
			bUsedBlockIDsInSelectionRanges.insert(hashRowOrColumnIndexBlockId[nTempRow]);
	}
	lstUsedBlockIDsInSelectionRanges = bUsedBlockIDsInSelectionRanges.toList();

	toggleViewAction = mViewMenu->addAction("Toggle Orientation");
	if(nSelectionCount>0)
	{
		if(bVerticalViewEnabled)
		{
			if(bUsedColumnIndexesInSelectionRanges.contains(0) == false)
				moveUpLeftBlocksAction = mBlocksMenu->addAction("Move Left (Decrease BlockNumber(s)");
			if(bUsedColumnIndexesInSelectionRanges.contains(columnCount()-1) == false)
				moveDownRightBlocksAction = mBlocksMenu->addAction("Move Right (Increase BlockNumber(s))");
			
		}
		else
		{
			if(bUsedRowIndexesInSelectionRanges.contains(0) == false)
				moveUpLeftBlocksAction = mBlocksMenu->addAction("Move Up (Decrease BlockNumber(s))");
			if(bUsedRowIndexesInSelectionRanges.contains(rowCount()-1) == false)
				moveDownRightBlocksAction = mBlocksMenu->addAction("Move Down (Increase BlockNumber(s))");
		}
	}

	addBlockAction = mBlocksMenu->addAction("Add New");
	if(bVerticalViewEnabled)
	{
		if(bUsedColumnIndexesInSelectionRanges.count() == 1)
			removeBlockAction = mBlocksMenu->addAction("Remove Selected");
		else if(bUsedColumnIndexesInSelectionRanges.count() > 1)
			removeBlocksAction = mBlocksMenu->addAction("Remove Multiple Selected");
	}
	else
	{
		if(bUsedRowIndexesInSelectionRanges.count() == 1)
			removeBlockAction = mBlocksMenu->addAction("Remove Selected");
		else if(bUsedRowIndexesInSelectionRanges.count() > 1)
			removeBlocksAction = mBlocksMenu->addAction("Remove Multiple Selected");
	}

	if(lstDefinedParamIndexesInSelectionRanges.isEmpty() == false)
	{		
		if (lstDefinedParamIndexesInSelectionRanges.count() == 1)
			resetParametersAction = mParametersMenu->addAction("Reset Selected");
		else
			resetParametersAction = mParametersMenu->addAction("Reset Multiple Selected");
	}
	
	selectedItemAction = mContexMenu.exec(globalPos);
	if (selectedItemAction)
	{
		if(selectedItemAction == addBlockAction)
		{
			bool bResult = false;
			if(pExpTreeModel)
				bResult = pExpTreeModel->addExperimentBlock();
		}
		else if((selectedItemAction == removeBlockAction) || (selectedItemAction == removeBlocksAction))
		{
			bool bResult = false;
			if(pExpTreeModel)
				bResult = pExpTreeModel->removeExperimentBlocks(lstUsedBlockIDsInSelectionRanges);
		}
		else if(selectedItemAction == toggleViewAction)
		{
			bVerticalViewEnabled = !bVerticalViewEnabled;
			reparseModel();
		}
		else if(selectedItemAction == moveUpLeftBlocksAction)
		{			
			bool bResult = false;
			if(bVerticalViewEnabled)
				bResult = pExpTreeModel->moveExperimentBlocks(lstUsedBlockIDsInSelectionRanges, hashRowOrColumnIndexBlockId[outerSelectionIndexes.left()-1], -1);
			else
				bResult = pExpTreeModel->moveExperimentBlocks(lstUsedBlockIDsInSelectionRanges, hashRowOrColumnIndexBlockId[outerSelectionIndexes.top()-1], -1);
		}
		else if(selectedItemAction == moveDownRightBlocksAction)
		{
			bool bResult = false;
			if(bVerticalViewEnabled)
				bResult = pExpTreeModel->moveExperimentBlocks(lstUsedBlockIDsInSelectionRanges, hashRowOrColumnIndexBlockId[outerSelectionIndexes.right()+1], 1);
			else
				bResult = pExpTreeModel->moveExperimentBlocks(lstUsedBlockIDsInSelectionRanges, hashRowOrColumnIndexBlockId[outerSelectionIndexes.bottom()+1], 1);
		}
		else if(selectedItemAction == resetParametersAction)
		{
			int nBlockID;
			QString sBlockParamName;
			QStringList sSplittedBlockParamName;
			QList<ExperimentTreeModel::strcParameterSpecifier> lstParameterSpecifiers;
			ExperimentTreeModel::strcParameterSpecifier tmpParameterSpecifier;
			foreach(QModelIndex tmpModelIndex,lstDefinedParamIndexesInSelectionRanges)
			{
				if(bVerticalViewEnabled)
				{
					nBlockID = hashRowOrColumnIndexBlockId[tmpModelIndex.column()];
					sBlockParamName = hashRowOrColumnIndexObjectIDParamName[tmpModelIndex.row()];
				}
				else
				{
					nBlockID = hashRowOrColumnIndexBlockId[tmpModelIndex.row()];
					sBlockParamName = hashRowOrColumnIndexObjectIDParamName[tmpModelIndex.column()];
				}
				sSplittedBlockParamName = sBlockParamName.split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER,QString::SkipEmptyParts);
				if(sSplittedBlockParamName.count() > 1)
				{
					tmpParameterSpecifier.nBlockID = nBlockID;
					tmpParameterSpecifier.nObjectID = sSplittedBlockParamName.at(0).toInt();
					tmpParameterSpecifier.sParamName = sSplittedBlockParamName.at(1);
					lstParameterSpecifiers.append(tmpParameterSpecifier);					
				}
			}
			if(lstParameterSpecifiers.isEmpty() == false)
				pExpTreeModel->removeExperimentParameters(lstParameterSpecifiers);
		}
	}
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
	lRowHeaders.clear();

	ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
	ExperimentParameterDefinitionContainer *tmpDefs = expParamWidgets->getExperimentParameterDefinition(BLOCK_TAG);
	int nParamID;
	for (int i=0;i<BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT;i++)
	{
		//if(i==BLOCKPARAMVIEW_BLOCKNUMBER_COLUMNINDEX)
		//	nParamID = tmpDefs->getFirstParameterID(BLOCKNUMBER_TAG);
		//else 
		if(i==BLOCKPARAMVIEW_BLOCKNAME_ROWORCOLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(NAME_TAG);
		else if(i==BLOCKPARAMVIEW_BLOCKTRIALS_ROWORCOLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(TRIALAMOUNT_TAG);
		else if(i==BLOCKPARAMVIEW_BLOCKINTTRGS_ROWORCOLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(INTERNALTRIGGERAMOUNT_TAG);
		else if(i==BLOCKPARAMVIEW_BLOCKEXTTRGS_ROWORCOLUMNINDEX)
			nParamID = tmpDefs->getFirstParameterID(EXTERNALTRIGGERAMOUNT_TAG);
		if(nParamID >= 0)
		{
			QString sTempHeaderCaption = tmpDefs->getParameterDefinition(nParamID)->sDisplayName;
			if(bVerticalViewEnabled)
				lRowHeaders.append(sTempHeaderCaption);
			else
				lColumnHeaders.append(sTempHeaderCaption);
		}
		else
		{
			if(bVerticalViewEnabled)
				lRowHeaders.append("<unknown header>");
			else
				lColumnHeaders.append("<unknown header>");
		}
	}
	if(bVerticalViewEnabled)
	{
		this->setRowCount(lRowHeaders.count());
		this->setVerticalHeaderLabels(lRowHeaders);
	}
	else
	{
		this->setColumnCount(lColumnHeaders.count());
		this->setHorizontalHeaderLabels(lColumnHeaders);
	}
}

void ExperimentBlockParameterView::resizeView(const int &nWidth, const int &nHeight)
{
	this->setFixedSize(nWidth-5,nHeight-5);//Fill available area
}

bool ExperimentBlockParameterView::parseExperimentStructure(cExperimentStructure *ExpStruct)
{
	int nExpBlockCount = ExpStruct->getBlockCount();
	if(bVerticalViewEnabled)
		this->setColumnCount(nExpBlockCount);
	else
		this->setRowCount(nExpBlockCount);
	initTableSetup();		
	cBlockStructure *tmpBlock = NULL;
	int nNextSearchBlockNumber = 0;
	QString sExpName = ExpStruct->getExperimentName();
	QTableWidgetItem *tmpItem;
	configureEditHandling(false);
	hashBlockIdRowOrColumnIndex.clear();
	hashRowOrColumnIndexBlockId.clear();
	for (int i=0;i<nExpBlockCount;i++)
	{
		tmpBlock = NULL;
		tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
		if(tmpBlock) 
		{
			if(bVerticalViewEnabled)
			{
				lColumnHeaders.append(tr("%1").arg(tmpBlock->getBlockNumber()));	
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getBlockName()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(BLOCKPARAMVIEW_BLOCKNAME_ROWORCOLUMNINDEX, i, tmpItem);
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfTrials()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(BLOCKPARAMVIEW_BLOCKTRIALS_ROWORCOLUMNINDEX, i, tmpItem);
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfInternalTriggers()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(BLOCKPARAMVIEW_BLOCKINTTRGS_ROWORCOLUMNINDEX, i, tmpItem);
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfExternalTriggers()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(BLOCKPARAMVIEW_BLOCKEXTTRGS_ROWORCOLUMNINDEX, i, tmpItem);
			}
			else
			{
				lRowHeaders.append(tr("%1").arg(tmpBlock->getBlockNumber()));
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getBlockName()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(i, BLOCKPARAMVIEW_BLOCKNAME_ROWORCOLUMNINDEX, tmpItem);
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfTrials()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(i, BLOCKPARAMVIEW_BLOCKTRIALS_ROWORCOLUMNINDEX, tmpItem);
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfInternalTriggers()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(i, BLOCKPARAMVIEW_BLOCKINTTRGS_ROWORCOLUMNINDEX, tmpItem);
				tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfExternalTriggers()));
				tmpItem->setTextColor(cChangedParameter);
				this->setItem(i, BLOCKPARAMVIEW_BLOCKEXTTRGS_ROWORCOLUMNINDEX, tmpItem);
			}
			hashBlockIdRowOrColumnIndex[tmpBlock->getBlockID()] = i;
			hashRowOrColumnIndexBlockId[i] = tmpBlock->getBlockID();
			nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
		}
	}
	if(bVerticalViewEnabled)
		this->setHorizontalHeaderLabels(lColumnHeaders);
	else
		this->setVerticalHeaderLabels(lRowHeaders);
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
		tmpObjectInfo.ObjectGlobalInfo.nID = EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID;
		tmpObjectInfo.ObjectGlobalInfo.sClass = BLOCK_TAG;
		tmpObjectInfo.ObjectGlobalInfo.sName = "<undefined>";
		tmpObjectInfo.pObjectParamDefContainer = expParamWidgets->getExperimentParameterDefinition(tmpObjectInfo.ObjectGlobalInfo.sClass);
		hashObjectIdExperimentObjectInfo[tmpObjectInfo.ObjectGlobalInfo.nID] = tmpObjectInfo;
	}
	return true;
}

bool ExperimentBlockParameterView::appendExperimentBlockParameterChanges()
{
	hashObjectParameterRowOrColumnIndex.clear();
	hashRowOrColumnIndexObjectIDParamName.clear();
	if(hashExpParamBlockChanges.count() > 0)
	{		
		int nParamCounter=0;
		int nObjectID;
		QString sParamName;
		QString sObjectIDParamName;
		QStringList lObjectIDParamName;
		QTableWidgetItem *tmpItem = NULL;
		int nRowOrColumnIndex;
		ExperimentParameterDefinitionContainer *pTempObjectParamDefContainer;

		//////////////DEFAULT BLOCK SECTION START//////////////////////
		strcRowOrColumnInfo tmpColumnInfo;
		QString sObjectParamIdentifier;
		int nParamID;
		pTempObjectParamDefContainer = hashObjectIdExperimentObjectInfo[EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID].pObjectParamDefContainer;
		for (int i=0; i<BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT; i++)
		{
			if(i==BLOCKPARAMVIEW_BLOCKNAME_ROWORCOLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(NAME_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKTRIALS_ROWORCOLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(TRIALAMOUNT_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKINTTRGS_ROWORCOLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(INTERNALTRIGGERAMOUNT_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKEXTTRGS_ROWORCOLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(EXTERNALTRIGGERAMOUNT_TAG);
			
			if(bVerticalViewEnabled)
				tmpColumnInfo.sHeader = lRowHeaders.at(i);
			else
				tmpColumnInfo.sHeader = lColumnHeaders.at(i);
			tmpColumnInfo.nRowOrColumnIndex = i;
			tmpColumnInfo.nObjectID = EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID;
			if(pTempObjectParamDefContainer)
			{
				nParamID = pTempObjectParamDefContainer->getFirstParameterID(NAME_TAG);
				if(nParamID >=0)
					tmpColumnInfo.strcParamDef = pTempObjectParamDefContainer->getParameterDefinition(nParamID);
			}
			hashObjectParameterRowOrColumnIndex[sObjectParamIdentifier] = tmpColumnInfo;
			hashRowOrColumnIndexObjectIDParamName[tmpColumnInfo.nRowOrColumnIndex] = sObjectParamIdentifier;
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
						if(pTempObjectParamDefContainer->getParameterDefinition(nParamID)->sGroupPath.isEmpty())
						{
							sParamName = pTempObjectParamDefContainer->getParameterDefinition(nParamID)->sDisplayName;
						}
						else
						{
							sParamName = pTempObjectParamDefContainer->getParameterDefinition(nParamID)->sGroupPath + EXPERIMENT_PARAM_GROUPSEP_CHAR + pTempObjectParamDefContainer->getParameterDefinition(nParamID)->sDisplayName;
							sParamName = sParamName.replace(EXPERIMENT_PARAM_GROUPSEP_CHAR,"\n");
						}
					}
					else
					{
						sParamName = lObjectIDParamName.at(1);
					}
					if(hashObjectParameterRowOrColumnIndex.contains(sObjectIDParamName))
					{
						nRowOrColumnIndex = hashObjectParameterRowOrColumnIndex[sObjectIDParamName].nRowOrColumnIndex;
					}
					else
					{
						if(bVerticalViewEnabled)
						{
							this->setRowCount(lRowHeaders.count()+1);
							lRowHeaders.append(sParamName);
						}
						else
						{
							this->setColumnCount(lColumnHeaders.count()+1);
							lColumnHeaders.append(sParamName);
						}
						strcRowOrColumnInfo tmpColumnInfo;
						tmpColumnInfo.sHeader = sParamName;
						if(bVerticalViewEnabled)
							tmpColumnInfo.nRowOrColumnIndex = lRowHeaders.count()-1;
						else
							tmpColumnInfo.nRowOrColumnIndex = lColumnHeaders.count()-1;
						tmpColumnInfo.nObjectID = nObjectID;
						if(pTempObjectParamDefContainer)
						{
							if(nParamID >=0)
								tmpColumnInfo.strcParamDef = pTempObjectParamDefContainer->getParameterDefinition(nParamID);
						}
						hashObjectParameterRowOrColumnIndex[sObjectIDParamName] = tmpColumnInfo;
						hashRowOrColumnIndexObjectIDParamName[tmpColumnInfo.nRowOrColumnIndex] = sObjectIDParamName;
						nRowOrColumnIndex = tmpColumnInfo.nRowOrColumnIndex;
					}
					if(hashBlockIdRowOrColumnIndex.contains(tmpParamBlockChanges.nBlockID))
					{						
						tmpItem = new QTableWidgetItem(tr("%1").arg(resolveParameterValueType(tmpParamBlockChanges.sValue, hashObjectParameterRowOrColumnIndex[sObjectIDParamName].strcParamDef->eType, true).toString()));
						tmpItem->setTextColor(cChangedParameter);
						if(bVerticalViewEnabled)
							this->setItem(nRowOrColumnIndex, hashBlockIdRowOrColumnIndex[tmpParamBlockChanges.nBlockID], tmpItem);	
						else
							this->setItem(hashBlockIdRowOrColumnIndex[tmpParamBlockChanges.nBlockID], nRowOrColumnIndex, tmpItem);						
					}
				}
			}
			nParamCounter++;
		}
		if(bVerticalViewEnabled)
			this->setVerticalHeaderLabels(lRowHeaders);
		else
			this->setHorizontalHeaderLabels(lColumnHeaders);

		//Fill-in remaining default and buffered Parameter values. We can expect here that the Block Numbers are ordered!
		int nColumnCount = columnCount();
		int nRowCount = rowCount();
		if(((bVerticalViewEnabled == false) && (nColumnCount > BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT)) || ((bVerticalViewEnabled) && (nRowCount > BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT)))
		{
			ExperimentParameterDefinitionStrc *tmpExpParDefStruct = NULL;
			QString sLatestValue;
			int nRowOrColumnCountOuter;
			if(bVerticalViewEnabled)
				nRowOrColumnCountOuter = nRowCount;
			else
				nRowOrColumnCountOuter = nColumnCount;

			int nRowOrColumnCountInner;
			if(bVerticalViewEnabled)
				nRowOrColumnCountInner = nColumnCount;
			else
				nRowOrColumnCountInner = nRowCount;
			QTableWidgetItem *tmpItem;
			int nRow;
			int nColumn;
			for (int nOut=(BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT);nOut<nRowOrColumnCountOuter;nOut++)
			{
				sLatestValue = "";
				for (int nIn=0;nIn<nRowOrColumnCountInner;nIn++)
				{
					if(bVerticalViewEnabled)
					{
						nRow = nOut;
						nColumn = nIn;
					}
					else
					{
						nRow = nIn;
						nColumn = nOut;						
					}	
					tmpItem = item(nRow,nColumn);
					if(tmpItem == NULL)
					{
						if(nIn==0)
						{
							tmpExpParDefStruct = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[nOut]].strcParamDef;
							if(tmpExpParDefStruct)
								sLatestValue = resolveParameterValueType(tmpExpParDefStruct->sDefaultValue,tmpExpParDefStruct->eType,true).toString();
						}
						tmpItem = new QTableWidgetItem(sLatestValue);
						tmpItem->setTextColor(cInheritedParameter);
						this->setItem(nRow,nColumn,tmpItem);
					}
					else
					{
						sLatestValue = item(nRow,nColumn)->text();
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
		bEditHandlingEnabled = true;
	else if((bEnable==false) && bEditHandlingEnabled)
		bEditHandlingEnabled = false;
	mutexEditHandlingEnabled.unlock();
}

void ExperimentBlockParameterView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	Q_UNUSED(current);
	removeCellWidget(previous.row(), previous.column());
}

void ExperimentBlockParameterView::cellOpenedForEdit(const int &nRow, const int &nColumn)
{
	Q_UNUSED(nRow);
	Q_UNUSED(nColumn);
	QModelIndexList tmpModelIndexList = this->selectedIndexes();
	if(tmpModelIndexList.count()==1)
	{
		QStringList tmpObjectIDParamNameList;
		if(bVerticalViewEnabled)
			tmpObjectIDParamNameList = hashRowOrColumnIndexObjectIDParamName[tmpModelIndexList.at(0).row()].split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER);
		else
			tmpObjectIDParamNameList = hashRowOrColumnIndexObjectIDParamName[tmpModelIndexList.at(0).column()].split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER);
		if(tmpObjectIDParamNameList.count() > 1)
		{
			ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
			//QString tmpParamName = tmpObjectIDParamNameList.at(1);
			int tmpInt = tmpObjectIDParamNameList.at(0).toInt();
			//QString tmpStr = hashObjectIdExperimentObjectInfo[tmpInt].sClass;//like RETINOTOPYMAPPER_NAME
			if((tmpInt<hashObjectIdExperimentObjectInfo.count()) || (tmpInt==EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID))
			{
				QString sDerivedPrefixName;
				QString sUniqueGeneratedPropertyIdentifier = "";
				QTableWidgetItem *tmpItem;
				if(bVerticalViewEnabled)
					sDerivedPrefixName = QString::number(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.nID) + ":" + QString::number(hashRowOrColumnIndexBlockId[tmpModelIndexList.at(0).column()]);
				else
					sDerivedPrefixName = QString::number(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.nID) + ":" + QString::number(hashRowOrColumnIndexBlockId[tmpModelIndexList.at(0).row()]);
				tmpItem = item(tmpModelIndexList.at(0).row(), tmpModelIndexList.at(0).column());
				QVariant vResolvedParameterValue = "";
				bool bDoValueInit = false;
				if(tmpItem)
				{
					ExperimentParameterDefinitionStrc *tmpDef;
					if(bVerticalViewEnabled)
						tmpDef = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[tmpModelIndexList.at(0).row()]].strcParamDef;
					else
						tmpDef = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[tmpModelIndexList.at(0).column()]].strcParamDef;
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
						int nIndex1;
						int nIndex2;
						if(bVerticalViewEnabled)
						{
							nIndex1 = tmpModelIndexList.at(0).column();	
							nIndex2 = tmpModelIndexList.at(0).row();												
							setCellWidget(nIndex2, nIndex1, tmpWidget);
						}
						else
						{
							nIndex1 = tmpModelIndexList.at(0).row();
							nIndex2 = tmpModelIndexList.at(0).column();
							setCellWidget(nIndex1, nIndex2, tmpWidget);
						}											
						if(tmpItem)
						{
							ExperimentParameterDefinitionStrc *tmpDef = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[nIndex2]].strcParamDef;
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
}

void ExperimentBlockParameterView::cellItemEditFinished(const QString& sParamName, const QString& sNewValue)
{
	Q_UNUSED(sParamName);
	if(bEditHandlingEnabled)
	{
		int nRow = currentRow();
		int nColumn = currentColumn();
		int nTempRow;
		int nTempColumn;
		int nIndex1;
		int nIndex2;
		if(bVerticalViewEnabled)
		{
			nIndex1 = nColumn;
			nIndex2 = nRow;
		}
		else
		{
			nIndex1 = nRow;
			nIndex2 = nColumn;
		}
		int nObjectID = hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[nIndex2]].nObjectID;
		int nBlockID = hashRowOrColumnIndexBlockId[nIndex1];
		if(pExpTreeModel)
		{
			bDoReparseModel = false;
			pExpTreeModel->saveNewData(nBlockID,nObjectID,sParamName,sNewValue);
			bDoReparseModel = true;
		}
		QString sNewResolvedValue = resolveParameterValueType(sNewValue, hashObjectParameterRowOrColumnIndex[hashRowOrColumnIndexObjectIDParamName[nIndex2]].strcParamDef->eType, true).toString();
		QTableWidgetItem *tmpItem = new QTableWidgetItem(tr("%1").arg(sNewResolvedValue));
		tmpItem->setTextColor(cChangedParameter);
		this->setItem(nRow,nColumn, tmpItem);	

		//Fill-in remaining default and buffered Parameter values. We can expect here that the Block Numbers are ordered!
		int nBlockOrRowCount;
		if(bVerticalViewEnabled)
			nBlockOrRowCount = columnCount();
		else
			nBlockOrRowCount = rowCount();
		if((nIndex2 >= BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT) && (nIndex1 < (nBlockOrRowCount-1)))
		{
			QString sLatestValue = sNewResolvedValue;
			for (int r=nIndex1+1;r<nBlockOrRowCount;r++)
			{
				if(bVerticalViewEnabled)
				{
					nTempRow = nIndex2;
					nTempColumn = r;
				}
				else
				{
					nTempRow = r;
					nTempColumn = nIndex2;
				}

				if(item(nTempRow,nTempColumn)->textColor() == cInheritedParameter)
				{
					tmpItem = new QTableWidgetItem(sLatestValue);
					tmpItem->setTextColor(cInheritedParameter);
					this->setItem(nTempRow,nTempColumn,tmpItem);
				}
				else
				{
					break;
				}
			}
		}
	}
}