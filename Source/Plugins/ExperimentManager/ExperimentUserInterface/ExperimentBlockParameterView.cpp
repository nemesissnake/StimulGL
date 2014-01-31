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
#include "../ExperimentStructures.h"
#include "../Global.h"
#include <QDomNode>

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
	lColumnHeaders.append(BLOCKPARAMVIEW_BLOCKNUMBER_HEADER);
	lColumnHeaders.append(BLOCKPARAMVIEW_BLOCKNAME_HEADER);
	lColumnHeaders.append(BLOCKPARAMVIEW_BLOCKTRIALS_HEADER);
	lColumnHeaders.append(BLOCKPARAMVIEW_BLOCKINTTRIGGERS_HEADER);
	lColumnHeaders.append(BLOCKPARAMVIEW_BLOCKEXTTRIGGERS_HEADER);
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
	for (int i=0;i<nExpBlockCount;i++)
	{
		tmpBlock = NULL;
		tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
		if(tmpBlock) 
		{
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getBlockNumber()));
			this->setItem(i, lColumnHeaders.indexOf(BLOCKPARAMVIEW_BLOCKNUMBER_HEADER), tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getBlockName()));
			this->setItem(i, lColumnHeaders.indexOf(BLOCKPARAMVIEW_BLOCKNAME_HEADER), tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfTrials()));
			this->setItem(i, lColumnHeaders.indexOf(BLOCKPARAMVIEW_BLOCKTRIALS_HEADER), tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfInternalTriggers()));
			this->setItem(i, lColumnHeaders.indexOf(BLOCKPARAMVIEW_BLOCKINTTRIGGERS_HEADER), tmpItem);
			tmpItem = new QTableWidgetItem(tr("%1").arg(tmpBlock->getNumberOfExternalTriggers()));
			this->setItem(i, lColumnHeaders.indexOf(BLOCKPARAMVIEW_BLOCKEXTTRIGGERS_HEADER), tmpItem);
			hashBlockIdRowIndex[tmpBlock->getBlockID()] = i;
			nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
		}
	}
	parsedExpStruct = ExpStruct;
	this->resizeColumnsToContents();
	configureEditHandling(true);	
	return true;
}

bool ExperimentBlockParameterView::parseExperimentBlockParameters(const QDomNodeList &tmpDomNodeList)
{
	int nBlockCount = tmpDomNodeList.count();
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

															QString uniqueObjParamIdentifier = tr("%1_%2").arg(nObjectID).arg(sParamName);															
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
	return appendExperimentBlockParameterChanges();
}

bool ExperimentBlockParameterView::appendExperimentBlockParameterChanges()
{
	hashObjectParameterColumnIndex.clear();
	if(hashExpParamBlockChanges.count() > 0)
	{		
		int nParamCounter=0;
		int nObjectID;
		QString sParamName;
		QString sObjectIDParamName;
		QStringList lObjectIDParamName;
		QTableWidgetItem *tmpItem = NULL;
		foreach(QList<strcParameterBlockChanges> tmpParamBlockChangesList,hashExpParamBlockChanges)
		{
			foreach(strcParameterBlockChanges tmpParamBlockChanges,tmpParamBlockChangesList)
			{
				sObjectIDParamName = hashExpParamBlockChanges.keys().at(nParamCounter);
				lObjectIDParamName = sObjectIDParamName.split("_");
				if(lObjectIDParamName.count() == 2)
				{
					nObjectID = lObjectIDParamName.at(0).toInt();
					sParamName = lObjectIDParamName.at(1);				
					int nColumnIndex;
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
						nColumnIndex = tmpColumnInfo.nColumnIndex;
					}
					if(hashBlockIdRowIndex.contains(tmpParamBlockChanges.nBlockID))
					{						
						tmpItem = new QTableWidgetItem(tr("%1").arg(tmpParamBlockChanges.sValue));
						this->setItem(hashBlockIdRowIndex[tmpParamBlockChanges.nBlockID], nColumnIndex, tmpItem);
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
	bool bResult;
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

}
