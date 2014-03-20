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

#include <QFile>
#include <QXmlStreamWriter>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "ExperimentTreeModel.h"
#include "ExperimentParameterDefinition.h"

ExperimentTreeModel::ExperimentTreeModel(QObject *parent) : QStandardItemModel(parent)
{
	doc = NULL;
	root = NULL;
    rootItem = NULL;

	nXMLDocumentVersion.major = 1;
	nXMLDocumentVersion.minor = 0;
	nXMLDocumentVersion.version = 0;
	nXMLDocumentVersion.build = 0;
	nXMLCurrentClassVersion.major = PLUGIN_EXMLDOC_MINIMAL_VERSION_MAJOR;
	nXMLCurrentClassVersion.minor = PLUGIN_EXMLDOC_MINIMAL_VERSION_MINOR;
	nXMLCurrentClassVersion.version = PLUGIN_EXMLDOC_MINIMAL_VERSION_REVISION;
	nXMLCurrentClassVersion.build = PLUGIN_EXMLDOC_MINIMAL_VERSION_BUILD;
}

ExperimentTreeModel::~ExperimentTreeModel()
{
	reset();
}

//QVariant ExperimentTreeModel::data(const QModelIndex &index, int role) const
//{
//	if (role == Qt::TextAlignmentRole) 
//		return Qt::AlignRight;
//	return QStandardItemModel::data(index, role);
//}

bool ExperimentTreeModel::fillModel()
{
	if(doc->isNull() || root->isNull() || rootItem == NULL)
		return false;

	for (int i = 0; i < root->attributes().size(); i++)//version --> <EXML version="2.1.0.0">
	{
		//tmpString = root.attributes().item(i).nodeName();
		//tmpString = root.attributes().item(i).nodeValue();
		rootItem->addDefinition(root->attributes().item(i).nodeName(), root->attributes().item(i).nodeValue(), TreeItemType_Attribute);
	}

	QDomNode dNode = root->firstChild();//<defines>
    recursiveRead(dNode, rootItem);
	setItem(0, rootItem);
	return true;
}

void ExperimentTreeModel::recursiveRead(QDomNode dNode, ExperimentTreeItem *item)
{
    do
    {
        int totalOfChilds = dNode.childNodes().size();
        if (dNode.nodeType() != QDomNode::CommentNode)
        {
            if (totalOfChilds == 0)
            {
                if (dNode.nodeType() == QDomNode::TextNode)
				{
					//tmpString = dNode.nodeValue();
                    item->setValue(dNode.nodeValue());
				}
                else
                {
					//tmpString = dNode.nodeName();
                    ExperimentTreeItem *subItem = new ExperimentTreeItem(dNode.nodeName());
					for (int i = 0; i < dNode.attributes().size(); i++)
					{
						//tmpString = dNode.attributes().item(i).nodeName();
                        subItem->addDefinition(dNode.attributes().item(i).nodeName(), dNode.attributes().item(i).nodeValue(), TreeItemType_Attribute);
					}
                    item->appendRow(subItem);
                }
            }
            else
            {
				//tmpString = dNode.nodeName();
                ExperimentTreeItem *item2 = new ExperimentTreeItem(dNode.nodeName());
                for (int i = 0; i < dNode.attributes().size(); i++)
				{
					//tmpString = dNode.attributes().item(i).nodeName();
					//tmpString = dNode.attributes().item(i).nodeValue();
					item2->addDefinition(dNode.attributes().item(i).nodeName(), dNode.attributes().item(i).nodeValue(), TreeItemType_Attribute);
				}
                for (int i = 0; i < totalOfChilds; i++)
				{
                    if (dNode.childNodes().size() > 0 && i == 0)
					{
                        recursiveRead(dNode.childNodes().at(i), item2);
					}
				}
                item->appendRow(item2);
            }
        }
		else//This is an comment node...
		{
			//tmpString = dNode.nodeValue();
			//tmpString = dNode.nodeName();
			ExperimentTreeItem *item3 = new ExperimentTreeItem(DEFINITION_TAG_COMMENT);
			item3->addDefinition(DEFINITION_NAME_VALUE, dNode.nodeValue(), TreeItemType_Comment);
			item->appendRow(item3);			
		}
        dNode = dNode.nextSibling();
    }
    while (!dNode.isNull());
}

bool ExperimentTreeModel::saveNewData(const int &nBlockID, const int &nObjectID, const QString &sParamName, const QString &sParamValue)
{
	QStringList sFilterList;
	TreeItemDefinition tmpTreeItemDef;
	int nTempObjectID;
	sFilterList << EXPERIMENTTREEMODEL_FILTER_TAGS;

	if(nObjectID >= 0)
	{
		ExperimentTreeItem* tmpItem3 = getExperimentBlockTreeItem(nBlockID);
		if(tmpItem3)
		{
			QList<ExperimentTreeItem*> list4 = getFilteredItemList(OBJECT_TAG, sFilterList, tmpItem3);
			foreach (ExperimentTreeItem* tmpItem4 ,list4)
			{
				if(tmpItem4->getDefinitions().contains(ID_TAG))
				{
					nTempObjectID = tmpItem4->getDefinition(ID_TAG).value.toInt();
					if(nTempObjectID == nObjectID)//Correct Object ID?
					{
						QList<ExperimentTreeItem*> list5 = getFilteredItemList(PARAMETERS_TAG, sFilterList, tmpItem4);
						foreach (ExperimentTreeItem* tmpItem5 ,list5)
						{
							this->saveNewData(sParamName,sParamValue,QModelIndex(),tmpItem5);
							return true;
						}
						return false;
					}
				}
			}
			return false;
		}
	}
	else if(nObjectID == EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID)
	{
		ExperimentTreeItem* tmpItem1 = getExperimentBlockTreeItem(nBlockID);
		if(tmpItem1)
		{
			if (tmpItem1->hasChildren())
			{
				for (int i=0;i<tmpItem1->rowCount(); i++)
				{
					if(tmpItem1->child(i)->getName().toLower() == sParamName)
					{
						tmpItem1->child(i)->setValue(sParamValue);
						emit modelModified();
						return true;
					}
				}
			}
		}
	}	
	return false;
}

bool ExperimentTreeModel::addExperimentBlocks(const int &nAmount)
{
	ExperimentTreeItem* tmpExpTreeItem;
	QModelIndex tmpModelIndex;
	bool bResult = false;
	tmpExpTreeItem = addExperimentBlockTreeItems(nAmount);
	if(tmpExpTreeItem)
	{
		bResult = true;
	}
	return bResult;
}

bool ExperimentTreeModel::removeExperimentParameters(const QList<ExperimentTreeModel::strcParameterSpecifier> lstParameterSpecifiers)
{
	ExperimentTreeItem* tmpExpTreeItem;
	QModelIndex tmpModelIndex;
	bool bResult = false;
	foreach(ExperimentTreeModel::strcParameterSpecifier sTempParamSpec, lstParameterSpecifiers)
	{
		if(sTempParamSpec.nBlockID >= 0)
		{
			tmpExpTreeItem = getExperimentParameterTreeItem(sTempParamSpec.nBlockID,sTempParamSpec.nObjectID,sTempParamSpec.sParamName);
			if(tmpExpTreeItem)
			{
				tmpModelIndex = indexFromItem(tmpExpTreeItem);
				if(removeRow(tmpModelIndex.row(),tmpModelIndex.parent()))
					bResult = true;
				else
					return false;
			}
		}
	}
	if(bResult)
		emit modelModified();
	return bResult;
}

bool ExperimentTreeModel::removeExperimentBlocks(const QList<int> &lBlockIDs)
{
	ExperimentTreeItem* tmpExpTreeItem;
	QModelIndex tmpModelIndex;
	bool bResult = false;
	foreach(int nBlockId, lBlockIDs)
	{
		if(nBlockId >= 0)
		{
			tmpExpTreeItem = getExperimentBlockTreeItem(nBlockId);
			if(tmpExpTreeItem)
			{
				tmpModelIndex = indexFromItem(tmpExpTreeItem);
				if(removeRow(tmpModelIndex.row(),tmpModelIndex.parent()))
				{
					bResult = true;
				}
				else
				{
					bResult = false;
					break;
				}
			}
		}		
	}
	if(bResult)
		emit modelModified();
	return bResult;
}

ExperimentTreeItem* ExperimentTreeModel::getExperimentParameterTreeItem(const int &nBlockID, const int &nObjectID, const QString &sParamName)
{
	QStringList sFilterList;
	ExperimentTreeItem *tempExpTreeItem;
	int nTempBlockID;
	int nTempObjectID;

	sFilterList << EXPERIMENTTREEMODEL_FILTER_TAGS;
	QList<ExperimentTreeItem*> list1 = getFilteredItemList(ACTIONS_TAG, sFilterList);
	foreach (ExperimentTreeItem* tmpItem1 ,list1)
	{
		QList<ExperimentTreeItem*> list2 = getFilteredItemList(BLOCKTRIALS_TAG, sFilterList, tmpItem1);
		foreach (ExperimentTreeItem* tmpItem2 ,list2)
		{
			QList<ExperimentTreeItem*> list3 = getFilteredItemList(BLOCK_TAG, sFilterList, tmpItem2);
			foreach (ExperimentTreeItem* tmpItem3 ,list3)
			{
				if(tmpItem3->getDefinitions().contains(ID_TAG))
				{
					nTempBlockID = tmpItem3->getDefinition(ID_TAG).value.toInt();
					if(nTempBlockID == nBlockID)//Correct Block ID?
					{	
						QList<ExperimentTreeItem*> list4 = getFilteredItemList(OBJECT_TAG, sFilterList, tmpItem3);
						foreach (ExperimentTreeItem* tmpItem4 ,list4)
						{
							if(tmpItem4->getDefinitions().contains(ID_TAG))
							{
								nTempObjectID = tmpItem4->getDefinition(ID_TAG).value.toInt();
								if(nTempObjectID == nObjectID)//Correct Object ID?
								{
									QList<ExperimentTreeItem*> list5 = getFilteredItemList(PARAMETER_TAG, sFilterList, tmpItem4);
									foreach (ExperimentTreeItem* tmpItem5 ,list5)
									{
										if(tmpItem5->hasChildren())
										{
											tempExpTreeItem = tmpItem5->firstChild(NAME_TAG);
											if(tempExpTreeItem)
											{
												if(tempExpTreeItem->getValue().toLower() == sParamName)
												{
													return tempExpTreeItem->parent();
												}
											}
										}
									}
									break;
								}
							}
						}
						break;
					}
				}
			}
		}
	}
	return NULL;
}

ExperimentTreeItem* ExperimentTreeModel::getExperimentBlockTreeItem(const int &nBlockID)
{
	QStringList sFilterList;
	int nTempBlockID;

	sFilterList << EXPERIMENTTREEMODEL_FILTER_TAGS;
	QList<ExperimentTreeItem*> list1 = getFilteredItemList(ACTIONS_TAG, sFilterList);
	foreach (ExperimentTreeItem* tmpItem1 ,list1)
	{
		QList<ExperimentTreeItem*> list2 = getFilteredItemList(BLOCKTRIALS_TAG, sFilterList, tmpItem1);
		foreach (ExperimentTreeItem* tmpItem2 ,list2)
		{
			QList<ExperimentTreeItem*> list3 = getFilteredItemList(BLOCK_TAG, sFilterList, tmpItem2);
			foreach (ExperimentTreeItem* tmpItem3 ,list3)
			{
				if(tmpItem3->getDefinitions().contains(ID_TAG))
				{
					nTempBlockID = tmpItem3->getDefinition(ID_TAG).value.toInt();
					if(nTempBlockID == nBlockID)//Correct Block ID?
					{						
						return tmpItem3;
					}
				}
			}
		}
	}
	return NULL;
}

bool ExperimentTreeModel::moveExperimentBlocks(const QList<int> &lBlockIDsToMove, const int &nBlockIDToSwitch, const int &nBlockNumberChangeDirection)
{
	QStringList sFilterList;
	int nTempBlockID;
	int nBlockNumberToSwitch = -1;
	QList<ExperimentTreeItem*> lstActions;
	QList<ExperimentTreeItem*> lstBlockTrials;
	QList<ExperimentTreeItem*> lstBlocks;
	QMap<int,int> mapNewBlockNumbers;//BlockNumber, BlockID	//QMap items are ordered by key -> BlockNumber!
	QList<int> lstNewOrderedBlockIDList;

	sFilterList << EXPERIMENTTREEMODEL_FILTER_TAGS;
	lstActions = getFilteredItemList(ACTIONS_TAG, sFilterList);
	foreach (ExperimentTreeItem* tmpItem1 ,lstActions)
	{
		lstBlockTrials = getFilteredItemList(BLOCKTRIALS_TAG, sFilterList, tmpItem1);
		foreach (ExperimentTreeItem* tmpItem2 ,lstBlockTrials)
		{
			lstBlocks = getFilteredItemList(BLOCK_TAG, sFilterList, tmpItem2);
			foreach (ExperimentTreeItem* tmpItem3 ,lstBlocks)
			{
				if(tmpItem3->getDefinitions().contains(ID_TAG))
				{
					nTempBlockID = tmpItem3->getDefinition(ID_TAG).value.toInt();
					if(nTempBlockID >= 0)
					{
						if (tmpItem3->hasChildren())
						{
							for (int i=0;i<tmpItem3->rowCount();i++)
							{
								if(tmpItem3->child(i)->getName().toLower() == BLOCKNUMBER_TAG)
								{
									int nTempBlockNumber = tmpItem3->child(i)->getValue().toInt();
									mapNewBlockNumbers.insert(tmpItem3->child(i)->getValue().toInt(),nTempBlockID);
									if(nTempBlockID == nBlockIDToSwitch)
										nBlockNumberToSwitch = nTempBlockNumber;
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	if(mapNewBlockNumbers.isEmpty())
		return false;
	bool bBlockIDsInjected = false;
	foreach(nTempBlockID, mapNewBlockNumbers)
	{
		if(nTempBlockID == nBlockIDToSwitch)
		{
			if(bBlockIDsInjected == false)
			{
				if(nBlockNumberChangeDirection == -1)
				{
					lstNewOrderedBlockIDList.append(lBlockIDsToMove);
					lstNewOrderedBlockIDList.append(nBlockIDToSwitch);
				}
				else if(nBlockNumberChangeDirection == 1)
				{
					lstNewOrderedBlockIDList.append(nBlockIDToSwitch);
					lstNewOrderedBlockIDList.append(lBlockIDsToMove);
				}
				bBlockIDsInjected = true;
			}
		}
		else if(lstNewOrderedBlockIDList.contains(nTempBlockID) == false)
		{
			lstNewOrderedBlockIDList.append(nTempBlockID);
		}
	}
	if(bBlockIDsInjected == false)
		return false;

	bool bRetVal = false;
	int nNewBlockNumberCounter = 0;
	ExperimentTreeItem* tmpExpItem;
	foreach(nTempBlockID, lstNewOrderedBlockIDList)
	{
		tmpExpItem = getExperimentBlockTreeItem(nTempBlockID);
		if (tmpExpItem->hasChildren())
		{
			for (int i=0;i<tmpExpItem->rowCount();i++)
			{
				if(tmpExpItem->child(i)->getName().toLower() == BLOCKNUMBER_TAG)
				{
					tmpExpItem->child(i)->setValue(QString::number(nNewBlockNumberCounter));
					nNewBlockNumberCounter++;
					bRetVal = true;
					break;
				}
			}
		}
	}
	if(bRetVal)
		emit modelModified();
	return bRetVal;
}

ExperimentTreeItem* ExperimentTreeModel::addExperimentBlockTreeItems(const int &nAmount)
{
	if(nAmount < 1)
		return NULL;

	QStringList sFilterList;
	int nTempBlockID;
	int nLatestFoundBlockID = -1;
	int nHighestFoundBlockNumber = -1;
	int nFoundBlockNumber = -1;
	QList<ExperimentTreeItem*> lstActions;
	QList<ExperimentTreeItem*> lstBlockTrials;
	QList<ExperimentTreeItem*> lstBlocks;

	sFilterList << EXPERIMENTTREEMODEL_FILTER_TAGS;
	lstActions = getFilteredItemList(ACTIONS_TAG, sFilterList);
	foreach (ExperimentTreeItem* tmpItem1 ,lstActions)
	{
		lstBlockTrials = getFilteredItemList(BLOCKTRIALS_TAG, sFilterList, tmpItem1);
		foreach (ExperimentTreeItem* tmpItem2 ,lstBlockTrials)
		{
			lstBlocks = getFilteredItemList(BLOCK_TAG, sFilterList, tmpItem2);
			foreach (ExperimentTreeItem* tmpItem3 ,lstBlocks)
			{
				if(tmpItem3->getDefinitions().contains(ID_TAG))
				{
					nTempBlockID = tmpItem3->getDefinition(ID_TAG).value.toInt();
					if(nTempBlockID >= 0)
					{
						if (tmpItem3->hasChildren())
						{
							for (int i=0;i<tmpItem3->rowCount(); i++)
							{
								if(tmpItem3->child(i)->getName().toLower() == BLOCKNUMBER_TAG)
								{
									nFoundBlockNumber = tmpItem3->child(i)->getValue().toInt();
									if(nFoundBlockNumber > nHighestFoundBlockNumber)//Highest Block Number?
									{
										nHighestFoundBlockNumber = nFoundBlockNumber;
										nLatestFoundBlockID = nTempBlockID;
									}
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	if(lstBlockTrials.count() != 1)
		return NULL;
	ExperimentTreeItem* tmpItemBlockTrials = lstBlockTrials.at(0);
	ExperimentTreeItem* tmpNewBlockItem = NULL;

	for (int nNewBlockCounter=0;nNewBlockCounter<nAmount;nNewBlockCounter++)
	{
		ExperimentTreeItem* tmpNewBlockItem = new ExperimentTreeItem(BLOCK_TAG);
		tmpNewBlockItem->addDefinition(ID_TAG,QString::number(nLatestFoundBlockID+1+nNewBlockCounter),TreeItemType_Attribute);
		tmpNewBlockItem->appendRow(new ExperimentTreeItem(NAME_TAG,"_noname_"));
		tmpNewBlockItem->appendRow(new ExperimentTreeItem(BLOCKNUMBER_TAG,QString::number(nHighestFoundBlockNumber+1+nNewBlockCounter)));
		tmpNewBlockItem->appendRow(new ExperimentTreeItem(TRIALAMOUNT_TAG,"1"));
		tmpNewBlockItem->appendRow(new ExperimentTreeItem(INTERNALTRIGGERAMOUNT_TAG,"1"));
		tmpNewBlockItem->appendRow(new ExperimentTreeItem(EXTERNALTRIGGERAMOUNT_TAG,"1"));
		tmpItemBlockTrials->insertRow(tmpItemBlockTrials->rowCount(),tmpNewBlockItem);
	}
	emit modelModified();
	return tmpNewBlockItem;
}

void ExperimentTreeModel::saveNewData(const QString &sName, const QString &sValue, const QModelIndex &parentIndex, ExperimentTreeItem *pParametersSection)
{
	ExperimentTreeItem *m_parent;
	if(pParametersSection == NULL)
		m_parent = itemFromIndex(parentIndex);
	else
		m_parent = pParametersSection;

	bool bModelChanged = false;

	if ((m_parent != NULL) && (sName.isEmpty() == false))
	{
		int totalChilds;
		QString sTempString1;
		QString sTempString2;
		int nValueIndex;
		int nHighestIDNumber;
		bool bParamFound;
		bool bDoBreak = false;
		TreeItemDefinition tmpTreeItemDef;
		QMap<QString,TreeItemDefinition> m_definitions;
		QStringList sRelativeNames = sName.split("/");

		while(bDoBreak == false)
		{
			totalChilds = m_parent->childCount();
			nHighestIDNumber = -1;
			bParamFound = false;
			bDoBreak = false;
			if(totalChilds == 0)
				bDoBreak = true;
			for (int j=0;j<totalChilds;j++)
			{
				if(m_parent->getName() == EXPERIMENT_PARAMETERS_TAG)
				{
					if(m_parent->child(j)->getName() == EXPERIMENT_PARAMETER_TAG)
					{
						if(m_parent->child(j)->hasChildren())
						{
							m_definitions = m_parent->child(j)->getDefinitions();
							QMapIterator<QString, TreeItemDefinition> it(m_definitions);
							while (it.hasNext()) 
							{
								it.next();
								if(it.key().toLower() == EXPERIMENT_ID_TAG)
								{
									if(it.value().value.toInt() > nHighestIDNumber)
									{
										nHighestIDNumber = it.value().value.toInt();
									}
									break;
								}
							}
							sTempString1 = ""; 
							sTempString2 = "";
							nValueIndex = -1;
							for(int i=0;i<m_parent->child(j)->childCount();i++)
							{
								if(m_parent->child(j)->child(i)->getName() == EXPERIMENT_NAME_TAG)
								{
									if (sRelativeNames[0].compare(m_parent->child(j)->child(i)->getValue(), Qt::CaseInsensitive) == 0)
									{
										sTempString1 = m_parent->child(j)->child(i)->getValue();
									}
									else
									{
										continue;	
									}							
								}
								else if(m_parent->child(j)->child(i)->getName() == EXPERIMENT_VALUE_TAG)
								{
									sTempString2 = m_parent->child(j)->child(i)->getValue();
									nValueIndex = i;
								}
								if((nValueIndex != -1) && (sTempString1.isEmpty() == false))
								{
									if (sRelativeNames[0].compare(sTempString1, Qt::CaseInsensitive) == 0)
									{
										m_parent->child(j)->child(nValueIndex)->setValue(sValue);
										bModelChanged = true;
										//ExperimentTreeItem *item = new ExperimentTreeItem(m_parent->child(j)->child(nValueIndex));
										//m_parent->child(j)->removeRow(nValueIndex);
										//m_parent->child(j)->insertRow(nValueIndex, item);								
										sTempString1 = ""; 
										sTempString2 = "";
										nValueIndex = -1;
										bParamFound = true;
										//We break because then only the first parameter would be changed. 
										//In a non-unique situation this would not be compatible, so you need to comment the below 2 lines.
										bDoBreak = true;
										break;
									}
								}
							}					
						}
					}
					else
					{
						if(j == (totalChilds-1))//last one?
						{
							bDoBreak = true;
						}
						continue;
					}
					if(bDoBreak)
						break;
				}
				else
				{
					if (sRelativeNames[0].compare(m_parent->child(j)->getName(), Qt::CaseInsensitive) == 0)
					{
						if(sRelativeNames.count() > 1)
						{
							m_parent = m_parent->child(j);
							sRelativeNames.removeFirst();
							bDoBreak = false;
							break;							
						}
						else
						{
							m_parent->child(j)->setValue(sValue);
							bModelChanged = true;
							bParamFound = true;
							bDoBreak = true;
							break;
						}
					}
					else
					{
						if(j == (totalChilds-1))//last one?
						{
							bDoBreak = true;
						}
						continue;	
					}
					if(bDoBreak)
						break;
				}
				if(j == (totalChilds-1))//last one?
				{
					bDoBreak = true;
				}
			}
		}		
		if(bParamFound == false)
		{
			if(m_parent->getName() == EXPERIMENT_PARAMETERS_TAG)
			{
				//we need to add this new data to the tree model
				ExperimentTreeItem *item = new ExperimentTreeItem(EXPERIMENT_PARAMETER_TAG, "");
				m_parent->appendRow(item);			
				TreeItemDefinition tmpTreeItemDefinition;
				tmpTreeItemDefinition.type = TreeItemType_Attribute;
				tmpTreeItemDefinition.value = QString::number(nHighestIDNumber + 1);
				QMap<QString,TreeItemDefinition> m_definitions = item->getDefinitions();
				m_definitions.insert(QString(EXPERIMENT_ID_TAG).toUpper(), tmpTreeItemDefinition);
				item->setDefinitions(m_definitions);
				//Append the name
				ExperimentTreeItem *subNameItem = new ExperimentTreeItem(EXPERIMENT_NAME_TAG, sRelativeNames[0]);
				item->appendRow(subNameItem);			
				//Append the value
				subNameItem = new ExperimentTreeItem(EXPERIMENT_VALUE_TAG, sValue);
				item->appendRow(subNameItem);
				bModelChanged = true;
			}
		}
	}
	if(bModelChanged)
		emit modelModified();
}

void ExperimentTreeModel::saveNewData(QWidget *widgetContainer, const QModelIndex &parentIndex)
{
    ExperimentTreeItem *m_parent = itemFromIndex(parentIndex);
	//int nRow = parentIndex.row();
	//int nCol = parentIndex.column();
	if (m_parent != NULL)
	{
		int totalChilds = m_parent->childCount();
		QGridLayout *gridLayout = dynamic_cast<QGridLayout*>(widgetContainer->layout());
		bool bModelChanged = false;

		if(gridLayout == NULL)
		{
			//ExperimentParameterVisualizer *paramVisualizer = dynamic_cast<ExperimentParameterVisualizer*>(widgetContainer->layout());
		}
		else
		{
			for (int i = 0; i < gridLayout->rowCount(); i++)
			{
				QWidgetItem *widgetItem = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,3));
				if (widgetItem)
				{
					QLabel *label = dynamic_cast<QLabel*>(widgetItem->widget());
					QWidgetItem *widgetItem2 = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,1));

					if (label && widgetItem2)
					{
						for (int j = 0; j < totalChilds; j++)
						{
							if (label->text() == m_parent->child(j)->getUID())
							{
								QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widgetItem2->widget());
								QComboBox *comboBox = qobject_cast<QComboBox*>(widgetItem2->widget());
								if(lineEdit)
								{
									m_parent->child(j)->setValue(lineEdit->text());
								}
								else if(comboBox)
								{
									QString valAux = "";
									if (comboBox->currentText() == "TRUE")
										valAux = "1";
									else if (comboBox->currentText() == "FALSE")
										valAux = "0";
									m_parent->child(j)->setValue(valAux);
								}
								else
								{
									ExperimentTreeItem *item = new ExperimentTreeItem(m_parent->child(j));
									m_parent->removeRow(j);
									m_parent->insertRow(j, item);
								}
								bModelChanged = true;
							}
						}
					}
				}
			}
		}
		if (bModelChanged)
		{
			emit modelModified();
		}
	}
}

bool ExperimentTreeModel::write(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
		xmlWriter.writeDTD(DEFAULT_DOCUMENTTYPEDEFINITION);
		xmlWriter.writeComment("\nThis file was automatically generated by StimulGL.\n");

        recursiveWrite(xmlWriter, rootItem);

        xmlWriter.writeEndDocument();
        file.close();
		return true;
    }
	return false;
}

void ExperimentTreeModel::recursiveWrite(QXmlStreamWriter &xml, ExperimentTreeItem *item)
{
    if (item->hasChildren())
    {		
        xml.writeStartElement(item->getName());
		//tmpString = item->getName();
        QList<QString> keyDefinitions = item->getDefinitions().keys();
        for (int i = 0; i < keyDefinitions.count(); i++)
		{
			//tmpString = keyDefinitions.at(i);
			if(item->getDefinition(keyDefinitions.at(i)).type == TreeItemType_Attribute)
				xml.writeAttribute(keyDefinitions.at(i), item->getDefinition(keyDefinitions.at(i)).value.toString());
			else if(item->getDefinition(keyDefinitions.at(i)).type == TreeItemType_Comment)
				xml.writeComment(item->getDefinition(keyDefinitions.at(i)).value.toString());
		}
        for (int i = 0; i < item->childCount(); i++)
		{
            recursiveWrite(xml, item->child(i));
		}
		xml.writeEndElement();
    }
    else
    {
        if (item->getValue() == "")
        {
			//tmpString = item->getName();
			if(item->getName() != DEFINITION_TAG_COMMENT)
				xml.writeEmptyElement(item->getName());
            QList<QString> keyDefinitions = item->getDefinitions().keys();
            for (int i = 0; i < keyDefinitions.count(); i++)
			{
				//tmpString = keyDefinitions.at(i);
				if(item->getDefinition(keyDefinitions.at(i)).type == TreeItemType_Attribute)
					xml.writeAttribute(keyDefinitions.at(i), item->getDefinition(keyDefinitions.at(i)).value.toString());
				else if(item->getDefinition(keyDefinitions.at(i)).type == TreeItemType_Comment)
					xml.writeComment(item->getDefinition(keyDefinitions.at(i)).value.toString());
			}
        }
        else
		{
			//tmpString = item->getName();
            xml.writeTextElement(item->getName(), item->getValue());
		}
    }
}

QList<ExperimentTreeItem*> ExperimentTreeModel::getFilteredItemList(const QString &textToFind, const QStringList &filters, ExperimentTreeItem *expTreeItem)
{
    QList<ExperimentTreeItem*> list;
	if(expTreeItem == NULL)
	{
		expTreeItem = rootItem;
	}
	recursiveSearch(textToFind, filters, expTreeItem, list);
    return list;
}

void ExperimentTreeModel::recursiveSearch(const QString &textToFind, const QStringList &filters, ExperimentTreeItem *item, QList<ExperimentTreeItem*> &list)
{
    QString itName = item->getName();
    QString itValue = item->getValue();
    QString text = textToFind;

    if (!filters.contains(EXPERIMENTTREEMODEL_FILTER_CASE_SENSITIVE))
    {
        text = textToFind.toLower();
        itName = itName.toLower();
        itValue = itValue.toLower();
    }

    if ((itName == text && filters.contains(EXPERIMENTTREEMODEL_FILTER_TAGS)) ||
        (itValue == text && filters.contains(EXPERIMENTTREEMODEL_FILTER_VALUES)) ||
        (item->getDefinition(textToFind).value.toString().size() > 0 && filters.contains(EXPERIMENTTREEMODEL_FILTER_ATTRIBUTES)))
    {
        list.append(item);
    }
    if (item->hasChildren())
        for (int i = 0; i < item->rowCount(); i++)
            recursiveSearch(textToFind, filters, item->child(i), list);
}

QModelIndex ExperimentTreeModel::getIndexByUID(const QString &uuid)
{
    QModelIndex index;
    bool found = false;
    recursiveUidSearch(uuid, rootItem, found, index);
    return index;
}

void ExperimentTreeModel::recursiveUidSearch(const QString &uuid, ExperimentTreeItem *item, bool found, QModelIndex &index)
{
    if (item->getUID() == uuid)
    {
        index = indexFromItem(item->parent());
        found = true;
    }
    else if (item->hasChildren())
        for (int i = 0; i < item->rowCount() && !found; i++)
            recursiveUidSearch(uuid, item->child(i), found, index);
}

ExperimentTreeItem *ExperimentTreeModel::itemFromIndex(const QModelIndex &index) const
{
    return dynamic_cast<ExperimentTreeItem*>(QStandardItemModel::itemFromIndex(index));
}

int ExperimentTreeModel::getTreeElements(const QStringList &sElementTagName, QList<ExperimentTreeItem *> &lFoundTreeItems, ExperimentTreeItem *pSearchRootItem)
{
	if(doc == NULL)
		return -1;

	if(pSearchRootItem)
		return getStaticTreeElements(sElementTagName,lFoundTreeItems,pSearchRootItem);
	else
		return getStaticTreeElements(sElementTagName,lFoundTreeItems,rootItem);
}

int ExperimentTreeModel::getStaticTreeElements(const QStringList &sElementTagName, QList<ExperimentTreeItem *> &lFoundTreeItems, ExperimentTreeItem *pSearchRootItem)
{
	if(pSearchRootItem == NULL)
		return -1;
	int nDepth = sElementTagName.count();
	if(nDepth<=0)
		return 0;

	QStringList sTreeFilter;
	sTreeFilter << EXPERIMENTTREEMODEL_FILTER_TAGS;
	QList<ExperimentTreeItem *> currentStartSearchItems;
	currentStartSearchItems << pSearchRootItem;

	for (int i=0;i<nDepth;i++)
	{
		recursiveMultiSearch(sElementTagName.at(i), sTreeFilter, currentStartSearchItems, lFoundTreeItems);
		if(i==(nDepth-1))
			return lFoundTreeItems.count();

		if(lFoundTreeItems.isEmpty() == false)
		{
			currentStartSearchItems = lFoundTreeItems;
			lFoundTreeItems.clear();
		}
		else
		{
			return 0;
		}
	}	
	return 0;
}

QList<ExperimentStructuresNameSpace::strcExperimentObject> ExperimentTreeModel::getDefinedExperimentObjectInfoList(ExperimentTreeItem *objItem)
{
	//If objItem is NULL then all the objects are returned!
	QList<ExperimentStructuresNameSpace::strcExperimentObject> tmpExperimentObjectList;
	int nSearchObjectID = -1;
	if(objItem)
	{
		QMap<QString, TreeItemDefinition> mapTreeItemDefinition = objItem->getDefinitions();
		if(mapTreeItemDefinition.contains(ID_TAG))
		{
			nSearchObjectID = mapTreeItemDefinition.value(ID_TAG).value.toInt();
		}
		else
		{
			return tmpExperimentObjectList;
		}
	}
	ExperimentStructuresNameSpace::strcExperimentObject tmpExperimentObject;
	QList<ExperimentTreeItem*> tmpExpTreeItemList;
	QStringList strList;
	strList.clear();
	strList.append(ROOT_TAG);
	strList.append(DECLARATIONS_TAG); 
	strList.append(OBJECT_TAG);
	if (this->getTreeElements(strList,tmpExpTreeItemList) >= 0)
	{
		int nNrOfObjects = tmpExpTreeItemList.count();
		if (nNrOfObjects>0)
		{
			ExperimentTreeItem *pExpTreeItem;
			QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
			QString tmpString;
			int nObjectID = -1;
			for(int i=0;i<nNrOfObjects;i++)
			{
				pExpTreeItem = tmpExpTreeItemList.at(i);
				if (pExpTreeItem) 
				{
					tTmpTreeItemDefs = pExpTreeItem->getDefinitions();
					if(!tTmpTreeItemDefs.contains(ID_TAG))
						break;
					tmpString = tTmpTreeItemDefs[ID_TAG].value.toString();//Correct ObjectID?
					if (tmpString.isEmpty() == false)
					{
						nObjectID = tmpString.toInt();
						if((nObjectID == nSearchObjectID) || (objItem == NULL))
						{
							tmpExperimentObject.nID = nObjectID;
							pExpTreeItem = tmpExpTreeItemList.at(i)->firstChild(NAME_TAG);
							//if(tmpElement.tagName() == NAME_TAG)
							if(pExpTreeItem)
								tmpExperimentObject.sName = pExpTreeItem->getValue();
							pExpTreeItem = tmpExpTreeItemList.at(i)->firstChild(CLASS_TAG);
							//if(tmpElement.tagName() == CLASS_TAG)
							if(pExpTreeItem)
								tmpExperimentObject.sClass =pExpTreeItem->getValue();
							tmpExperimentObjectList.append(tmpExperimentObject);
						}
					}
				}
			}
		}
	}
	return tmpExperimentObjectList;
}

void ExperimentTreeModel::recursiveMultiSearch(const QString &textToFind, const QStringList &filters, QList<ExperimentTreeItem *> items, QList<ExperimentTreeItem*> &list)
{
	if(items.isEmpty())
		return;
	QList<ExperimentTreeItem *> resultingItemList;
	for (int i = 0;i<items.count(); i++)
	{
		recursiveSearch(textToFind,filters,items.at(i),list);
	}
}

bool ExperimentTreeModel::reset()
{
	if(rootItem)
	{
		delete rootItem;
		rootItem = NULL;
	}
	if(root)
	{
		delete root;
		root = NULL;
	}
	if(doc)
	{
		delete doc;
		doc = NULL;
	}
	return true;
}

bool ExperimentTreeModel::read(QByteArray &byteArrayContent)
{
	QString errorStr;
	int errorLine;
	int errorColumn;

	if(root)
	{
		delete root;
		root = NULL;
	}
	if(doc)
	{
		delete doc;
		doc = NULL;
	}
	doc = new QDomDocument();
	//Perform a global XML standard parse test
	if (!doc->setContent(byteArrayContent, true, &errorStr, &errorLine, &errorColumn))
	{
		//QMessageBox::information(window(), tr(MODULE_NAME),
		//	tr("Parse error at line %1, column %2:\n%3")
		//	.arg(errorLine)
		//	.arg(errorColumn)
		//	.arg(errorStr));
		qDebug() << tr("Parse error at line %1, column %2:\n%3").arg(errorLine).arg(errorColumn).arg(errorStr);
		return false;
	}
	//First we search for the Root Tag
	root = new QDomElement(doc->documentElement());
	rootItem = new ExperimentTreeItem(root->tagName());//root.tagName() = EXML
	if (root->tagName() != ROOT_TAG) 
	{
		//QMessageBox::information(window(), tr(MODULE_NAME), tr("The file is not an EXML file."));
		qDebug() << tr("The file is not an EXML file.");
		return false;
	} 
	else//Found!
	{
		if (root->hasAttribute(VERSION_TAG))//Check whether we have a version tag defined
		{	
			QString strCurrentVersion = root->attribute(VERSION_TAG);
			QString strMinimalVersion = QString::number(nXMLCurrentClassVersion.major) + "." + QString::number(nXMLCurrentClassVersion.minor) + "." + QString::number(nXMLCurrentClassVersion.version) + "." + QString::number(nXMLCurrentClassVersion.build);
			QStringList lVersion = strCurrentVersion.split(".");
			nXMLDocumentVersion.major = lVersion[0].toInt();
			nXMLDocumentVersion.minor = lVersion[1].toInt();
			nXMLDocumentVersion.version = lVersion[2].toInt();
			nXMLDocumentVersion.build = lVersion[3].toInt();
						
			//Is the documents version compatible with this class?
			if(!MainAppInfo::isCompatibleVersion(strMinimalVersion,strCurrentVersion))
			{
				//QMessageBox::information(window(), tr(MODULE_NAME), tr("The defined EXML file is not compatible from EXML version %1.%2.%3.%4")
				//	.arg(nXMLCurrentClassVersion.major)
				//	.arg(nXMLCurrentClassVersion.minor)
				//	.arg(nXMLCurrentClassVersion.version)
				//	.arg(nXMLCurrentClassVersion.build));
				qDebug() << tr("The defined EXML file is not compatible from EXML version %1.%2.%3.%4").arg(nXMLCurrentClassVersion.major).arg(nXMLCurrentClassVersion.minor).arg(nXMLCurrentClassVersion.version).arg(nXMLCurrentClassVersion.build);
				return false;
			}
		}
		else
		{
			//QMessageBox::information(window(), tr(MODULE_NAME), tr("The EXML file version could not be determined."));
			qDebug() << tr("The EXML file version could not be determined.");
			return false;
		}
	}
	return fillModel();
}
