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
#include "../ExperimentStructures.h"
#include "../Global.h"
#include <QDomNode>
#include <QComboBox>

ExperimentBlockParameterView::ExperimentBlockParameterView(QWidget *parent) : QTableWidget(parent)
{
	parsedExpStruct = NULL;
	bEditHandlingEnabled = false;
	//initTableSetup();
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
	//if((nExpBlockCount>0) && (this->columnCount() == 0))
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

bool ExperimentBlockParameterView::parseExperimentBlockParameters(const QDomNodeList &tmpDomNodeList)
{
	int nBlockCount = tmpDomNodeList.count();
	configureEditHandling(false);
	if(nBlockCount>0)
	{		
		QDomNode tmpNode;
		QDomNamedNodeMap tmpNodeMap;
		for (int i=0;i<nBlockCount;i++) //for each block
		{
			int nBlockID = -1;
			tmpNode = tmpDomNodeList.at(i);
			tmpNodeMap = tmpNode.attributes();
			for (int p=0;p<tmpNodeMap.count();p++)
			{
				if(tmpNodeMap.item(p).nodeName().compare(ID_TAG,Qt::CaseInsensitive)==0)
				{
					nBlockID = tmpNodeMap.item(p).nodeValue().toInt();
					break;
				}
			}
			if(nBlockID == -1)
				continue;

			if(tmpNode.hasChildNodes())
			{				
				int nChildCount = tmpNode.childNodes().count();
				for (int j=0;j<nChildCount;j++) //for each block-child
				{
					if(tmpNode.childNodes().at(j).nodeName().toLower() == OBJECT_TAG) //object section
					{
						int nObjectID = -1;
						if(tmpNode.childNodes().at(j).hasAttributes())
						{
							tmpNodeMap = tmpNode.childNodes().at(j).attributes();
							for (int k=0;k<tmpNodeMap.count();k++)
							{
								if(tmpNodeMap.item(k).nodeName().compare(ID_TAG,Qt::CaseInsensitive)==0)
								{
									nObjectID = tmpNodeMap.item(k).nodeValue().toInt();
									for (int l=0;l<tmpNode.childNodes().at(j).childNodes().count();l++)
									{
										QDomNode tmpObjectNode = tmpNode.childNodes().at(j).childNodes().at(l);
										if(tmpObjectNode.nodeName().toLower() == PARAMETERS_TAG)
										{
											for (int m=0;m<tmpObjectNode.childNodes().count();m++)
											{
												QDomNode tmpParametersNode = tmpObjectNode.childNodes().at(m);
												if(tmpParametersNode.nodeName().toLower() == PARAMETER_TAG)
												{													
													QDomElement tmpElement = tmpParametersNode.firstChildElement(NAME_TAG);
													if(!tmpElement.isNull())
													{
														QString sParamName = tmpElement.text().toLower();
														tmpElement = tmpParametersNode.firstChildElement(VALUE_TAG);
														if(!tmpElement.isNull())
														{
															strcParameterBlockChanges paramBlockChanges;
															paramBlockChanges.sValue = tmpElement.text();
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
											break;
										}
									}
								}
								else
								{
									continue;
								}
							}
						}
						else
						{
							continue;//No ID present, continue.
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
		tmpObjectInfo.ObjectGlobalInfo.nID = -1;
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


		strcColumnInfo tmpColumnInfo;
		QString sObjectParamIdentifier;
		for (int i=0; i<BLOCKPARAMVIEW_DEFAULTBLOCKHEADER_COUNT; i++)
		{
			if(i==BLOCKPARAMVIEW_BLOCKNAME_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(-1).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(NAME_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKTRIALS_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(-1).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(TRIALAMOUNT_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKINTTRGS_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(-1).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(INTERNALTRIGGERAMOUNT_TAG);
			else if(i==BLOCKPARAMVIEW_BLOCKEXTTRGS_COLUMNINDEX)
				sObjectParamIdentifier = tr("%1%2%3").arg(-1).arg(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER).arg(EXTERNALTRIGGERAMOUNT_TAG);
			tmpColumnInfo.sHeader = lColumnHeaders.at(i);
			tmpColumnInfo.nColumnIndex = i;
			hashObjectParameterColumnIndex[sObjectParamIdentifier] = tmpColumnInfo;
			hashColumnIndexObjectIDParamName[tmpColumnInfo.nColumnIndex] = sObjectParamIdentifier;
		}
	
		foreach(QList<strcParameterBlockChanges> tmpParamBlockChangesList,hashExpParamBlockChanges)
		{
			foreach(strcParameterBlockChanges tmpParamBlockChanges,tmpParamBlockChangesList)
			{
				sObjectIDParamName = hashExpParamBlockChanges.keys().at(nParamCounter);
				lObjectIDParamName = sObjectIDParamName.split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER);
				if(lObjectIDParamName.count() > 1)
				{
					nObjectID = lObjectIDParamName.at(0).toInt();
					pTempObjectParamDefContainer = hashObjectIdExperimentObjectInfo[nObjectID].pObjectParamDefContainer;
					if(pTempObjectParamDefContainer)
					{
						int nParamID = pTempObjectParamDefContainer->getFirstParameterID(lObjectIDParamName.at(1));
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
						hashObjectParameterColumnIndex[sObjectIDParamName] = tmpColumnInfo;
						hashColumnIndexObjectIDParamName[tmpColumnInfo.nColumnIndex] = sObjectIDParamName;
						nColumnIndex = tmpColumnInfo.nColumnIndex;
					}
					if(hashBlockIdRowIndex.contains(tmpParamBlockChanges.nBlockID))
					{						
						tmpItem = new QTableWidgetItem(tr("%1").arg(tmpParamBlockChanges.sValue));
						this->setItem(hashBlockIdRowIndex[tmpParamBlockChanges.nBlockID], nColumnIndex, tmpItem);
						
						/*
						int nRowIndex = hashBlockIdRowIndex[tmpParamBlockChanges.nBlockID];
						//removeCellWidget(previous.row(), previous.column());
						QStringList tmpObjectIDParamNameList = hashColumnIndexObjectIDParamName[nColumnIndex].split(BLOCKPARAMVIEW_BLOCKIDPARAM_SPLITTER);
						if(tmpObjectIDParamNameList.count() > 1)
						{
							expParamWidgets = ExperimentParameterWidgets::instance();
							//QString tmpParamName = tmpObjectIDParamNameList.at(1);
							int tmpInt = tmpObjectIDParamNameList.at(0).toInt();
							//QString tmpStr = hashObjectIdExperimentObjectInfo[tmpInt].sClass;//like RETINOTOPYMAPPER_NAME
							if(tmpInt<hashObjectIdExperimentObjectInfo.count())
							{
								QWidget *tmpWidget = expParamWidgets->getExperimentParameterWidgetSubControl(hashObjectIdExperimentObjectInfo[tmpInt].sClass, tmpObjectIDParamNameList.at(1), QString::number(hashRowIndexBlockId[nRowIndex]));		
								setCellWidget(nRowIndex, nColumnIndex, tmpWidget);	
							}
						}
						*/
					}
				}
			}
			nParamCounter++;
		}
		this->setHorizontalHeaderLabels(lColumnHeaders);
	}
	return true;
}

void ExperimentBlockParameterView::configureEditHandling(const bool &bEnable) 
{
	mutexEditHandlingEnabled.lock();
	if(bEnable && (bEditHandlingEnabled==false))
	{
		bEditHandlingEnabled = connect(this, SIGNAL(cellChanged(int, int)), this, SLOT(viewEdited(int, int)));		
	}
	else if((bEnable==false) && bEditHandlingEnabled)
	{
		bEditHandlingEnabled = !(disconnect(this, SIGNAL(cellChanged(int, int)), this, SLOT(viewEdited(int, int))));		
	}
	mutexEditHandlingEnabled.unlock();
}

void ExperimentBlockParameterView::viewEdited(const int &nRow, const int &nColumn)
{
	Q_UNUSED(nRow);
	Q_UNUSED(nColumn);
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
			if((tmpInt<hashObjectIdExperimentObjectInfo.count()) || (tmpInt==-1))
			{
				QString sDerivedPrefixName = QString::number(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.nID) + ":" + QString::number(hashRowIndexBlockId[tmpModelIndexList.at(0).row()]);
				QString sUniqueGeneratedPropertyIdentifier = "";
				QWidget *tmpWidget = expParamWidgets->getExperimentParameterWidgetSubControl(hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass, tmpObjectIDParamNameList.at(1), sDerivedPrefixName, sUniqueGeneratedPropertyIdentifier);		
				if(tmpWidget)
				{	
					configureEditHandling(false);
					setCellWidget(tmpModelIndexList.at(0).row(), tmpModelIndexList.at(0).column(), tmpWidget);
					QTableWidgetItem *tmpItem = item(tmpModelIndexList.at(0).row(), tmpModelIndexList.at(0).column());
					if(tmpItem)
						expParamWidgets->setWidgetParameter(sUniqueGeneratedPropertyIdentifier, hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass, tmpItem->text(), false);
					else
						expParamWidgets->setWidgetParameter(sUniqueGeneratedPropertyIdentifier, hashObjectIdExperimentObjectInfo[tmpInt].ObjectGlobalInfo.sClass, "", false);
					configureEditHandling(true);
				}
			}
		}
	}
	return QAbstractItemView::selectionChanged(selected,deselected);
}
