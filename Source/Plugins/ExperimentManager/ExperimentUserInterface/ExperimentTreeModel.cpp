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
#include "ExperimentTreeItem.h"
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

void ExperimentTreeModel::saveNewData(const QString &sName, const QString &sValue, const QModelIndex &parentIndex, ExperimentTreeItem *pParametersSection)
{
	ExperimentTreeItem *m_parent;
	if(pParametersSection == NULL)
		m_parent = itemFromIndex(parentIndex);
	else
		m_parent = pParametersSection;

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
										test();
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
			}
		}
	}
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
								if (lineEdit)
									m_parent->child(j)->setValue(lineEdit->text());
								else if (comboBox)
								{
									QString valAux = "";
									if (comboBox->currentText() == "TRUE")
										valAux = "1";
									else if (comboBox->currentText() == "FALSE")
										valAux = "0";

									m_parent->child(j)->setValue(valAux);
								}

								ExperimentTreeItem *item = new ExperimentTreeItem(m_parent->child(j));
								m_parent->removeRow(j);
								m_parent->insertRow(j, item);
							}
						}
					}
				}
			}
		}
		emit modelModified();
	}
}

void ExperimentTreeModel::test()
{

	return;

	QString fileName = "D:\\Sven.Gijsen\\Projects\\StimulGL\\Install\\examples\\Tutorials\\11_ExperimentStructure_Editing\\tempDoc.exml";
	if(write(fileName))
	{
		if(reset())
		{
			QFile expFile(fileName);
			if (expFile.open(QFile::ReadOnly | QFile::Text)) 
			{
				QString tmpString = expFile.readAll();			
				expFile.close();
				bool bResult = read(tmpString.toLatin1());
				bResult = bResult;
			}
			else
			{
				qDebug() << __FUNCTION__ << "Could not open the file " << fileName;
			}
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

/*int ExperimentTreeModel::getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList)
{
	if(doc == NULL)
		return -1;
	int nDepth = sElementTagName.count();
	if(nDepth<=0)
		return -1;

	QDomElement tmpElem;
	for (int i=0;i<nDepth;i++)
	{
		if (i==(nDepth-1))//last one?
		{
			ResultDomNodeList = tmpElem.elementsByTagName(sElementTagName[i]);//Return all the elements from this level
		}
		else if(i==0)//first one
		{
			tmpElem = doc->firstChildElement(sElementTagName[i]);//Only first one for now!
		}
		else
		{
			tmpElem = tmpElem.firstChildElement(sElementTagName[i]);//Only first one for now!
		}
	}
	return ResultDomNodeList.count();		
}*/

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
