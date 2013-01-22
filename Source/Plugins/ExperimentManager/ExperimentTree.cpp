//ExperimentManagerplugin
//Copyright (C) 2012  Sven Gijsen
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


#include <QtWidgets>
#include "experimenttree.h"
#include "defines.h"

ExperimentTree::ExperimentTree(QWidget *parent) : QTreeWidget(parent)
{
	nXMLDocumentVersion.major = 1;
	nXMLDocumentVersion.minor = 0;
	nXMLDocumentVersion.version = 0;
	nXMLDocumentVersion.build = 0;
	nXMLCurrentClassVersion.major = PLUGIN_EXMLDOC_MINIMAL_VERSION_MAJOR;
	nXMLCurrentClassVersion.minor = PLUGIN_EXMLDOC_MINIMAL_VERSION_MINOR;
	nXMLCurrentClassVersion.version = PLUGIN_EXMLDOC_MINIMAL_VERSION_REVISION;
	nXMLCurrentClassVersion.build = PLUGIN_EXMLDOC_MINIMAL_VERSION_BUILD;

	QStringList labels;
	labels << tr("Elements") << tr("Properties");

	header()->setSectionResizeMode(QHeaderView::Stretch);
	setHeaderLabels(labels);

	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
	propertyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
}

ExperimentTree::~ExperimentTree()
{

}

bool ExperimentTree::read(QByteArray &byteArrayContent)
{
	bool bRetval = false;
	QBuffer buffer(&byteArrayContent);
	if(buffer.open(QIODevice::ReadOnly))
	{
		bRetval = read(&buffer);
		buffer.close();
	}
	return bRetval;
}

bool ExperimentTree::read(QIODevice *device)
{
	QString errorStr;
	int errorLine;
	int errorColumn;
	//Perform a global XML standard parse test
	if (!domDocument.setContent(device, true, &errorStr, &errorLine, &errorColumn))
	{
			QMessageBox::information(window(), tr(MODULE_NAME),
				tr("Parse error at line %1, column %2:\n%3")
				.arg(errorLine)
				.arg(errorColumn)
				.arg(errorStr));
			return false;
	}
	//First we search for the Root Tag
	QDomElement root = domDocument.documentElement();
	if (root.tagName() != ROOT_TAG) 
	{
		QMessageBox::information(window(), tr(MODULE_NAME), tr("The file is not an EXML file."));
		return false;
	} 
	else//Found!
	{
		if (root.hasAttribute(VERSION_TAG))//Check whether we have a version tag defined
		{	
			QString strCurrentVersion = root.attribute(VERSION_TAG);
			QString strMinimalVersion = QString::number(nXMLCurrentClassVersion.major) + "." + QString::number(nXMLCurrentClassVersion.minor) + "." + QString::number(nXMLCurrentClassVersion.version) + "." + QString::number(nXMLCurrentClassVersion.build);
			QStringList lVersion = strCurrentVersion.split(".");
			nXMLDocumentVersion.major = lVersion[0].toInt();
			nXMLDocumentVersion.minor = lVersion[1].toInt();
			nXMLDocumentVersion.version = lVersion[2].toInt();
			nXMLDocumentVersion.build = lVersion[3].toInt();
			//Is the documents version compatible with this class?
			if(!MainAppInfo::isCompatibleVersion(strMinimalVersion,strCurrentVersion))
			{
				QMessageBox::information(window(), tr(MODULE_NAME), tr("The defined EXML file is not compatible from EXML version %1.%2.%3.%4")
				.arg(nXMLCurrentClassVersion.major)
				.arg(nXMLCurrentClassVersion.minor)
				.arg(nXMLCurrentClassVersion.version)
				.arg(nXMLCurrentClassVersion.build));
				return false;
			}
		}
		else
		{
			QMessageBox::information(window(), tr(MODULE_NAME), tr("The EXML file version could not be determined."));
			return false;
		}
	}
	//Version available and compatible!
	clear();//Clears the tree widget by removing all of its items and selections.
	disconnect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(updateDomElement(QTreeWidgetItem*,int)));


	//Search and parse the "declarations" Elements
	QDomElement child;
	child = root.firstChildElement(DECLARATIONS_TAG);
	if (!child.isNull())
	{
		QTreeWidgetItem *item = createItem(child, NULL);
		item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));//(item->flags() | Qt::ItemIsEditable);
		item->setIcon(0, folderIcon);
		item->setText(0, DECLARATIONS_TAG);
		//bool folded = (child.attribute(FOLDED_TAG) != BOOL_NO_TAG);
		setItemExpanded(item, true);//!folded);
		while (!child.isNull()) 
		{
			parseRootElement(child,EXML_Declarations,item);
			child = child.nextSiblingElement(DECLARATIONS_TAG);
		}
	}

	//Search and parse the "actions" Elements
	child = root.firstChildElement(ACTIONS_TAG);
	if (!child.isNull())
	{
		QTreeWidgetItem *item = createItem(child, NULL);
		item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));//(item->flags() | Qt::ItemIsEditable);
		item->setIcon(0, folderIcon);
		item->setText(0, ACTIONS_TAG);
		//bool folded = (child.attribute(FOLDED_TAG) != BOOL_NO_TAG);
		setItemExpanded(item, true);//!folded);
		while (!child.isNull()) 
		{
			parseRootElement(child,EXML_Actions,item);
			child = child.nextSiblingElement(ACTIONS_TAG);
		}
	}
	connect(this, SIGNAL(itemChanged(QTreeWidgetItem*,int)), this, SLOT(updateDomElement(QTreeWidgetItem*,int)));
	return true;
}

bool ExperimentTree::write(QIODevice *device)
{
	const int IndentSize = 4;

	//QTextStream out(device);
	//QString a;
	//QTextStream tempstr(&a);
	//domDocument.save(tempstr, IndentSize);
	//tempstr.flush();
	
	QTextStream out(device);
	domDocument.save(out, IndentSize);
	//out.flush();

	//QString test = tempstr.readAll();
	
	return true;
}

void ExperimentTree::updateDomElement(QTreeWidgetItem *item, int column)
{
	QDomElement element = domElementForItem.value(item);
	if (!element.isNull()) 
	{
		if (column == 0) 
		{
			QDomElement oldTitleElement = element.firstChildElement(TITLE_TAG);
			QDomElement newTitleElement = domDocument.createElement(TITLE_TAG);

			QDomText newTitleText = domDocument.createTextNode(item->text(0));
			newTitleElement.appendChild(newTitleText);

			element.replaceChild(newTitleElement, oldTitleElement);
		} 
		else 
		{
			if (element.tagName() == BOOKMARK_TAG)
				element.setAttribute(HREF_TAG, item->text(1));
		}
	}
}

void ExperimentTree::parseRootElement(const QDomElement &element, EXML_TAG_SECTIONS tSection, QTreeWidgetItem *parentItem)
{
	//Parse declarations and actions blocks
	switch (tSection)
	{
	case EXML_Declarations:
		//Get the object tag
		tmpString1 = element.firstChildElement(OBJECT_TAG).text();
		if (tmpString1.isEmpty())//Is there an Object item?
		{
			return;
		}
		break;
	case EXML_Actions:
		tmpString1 = element.firstChildElement(EVENT_TAG).text();
		if (tmpString1.isEmpty())//Is there an Event item?
		{
			return;
		}
		break;
	default:
		return;
	}

	//QTreeWidgetItem *item = createItem(element, parentItem);//Create the TreeWidgetItem
	//if (tmpString1.isEmpty())
	//	tmpString1 = QObject::tr(FOLDER_TAG);
	//item->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
	//item->setIcon(0, folderIcon);//folderIcon);//Give it a Icon
	//item->setText(0, tmpString1);

	//bool folded = (element.attribute(FOLDED_TAG) != BOOL_NO_TAG);
	//setItemExpanded(item, !folded);

	QDomElement child = element.firstChildElement();
	while (!child.isNull()) 
	{
		if (child.tagName() == OBJECT_TAG) 
		{
			parseElement(child, EXML_Object,parentItem);
			//What to do here!!!!
		} 
		else if (child.tagName() == EVENT_TAG) 
		{
			parseElement(child,EXML_Event, parentItem);
			//QTreeWidgetItem *childItem = createItem(child, item);
			//if (tmpString1.isEmpty())
			//	//title = QObject::tr(FOLDER_TAG);

			//childItem->setFlags(item->flags() | Qt::ItemIsEditable);
			//childItem->setIcon(0, propertyIcon);
			//childItem->setText(0, title);
			//childItem->setText(1, child.attribute(HREF_TAG));
		} 
	//	else if (child.tagName() == "separator") 
	//	{
	//		QTreeWidgetItem *childItem = createItem(child, item);
	//		childItem->setFlags(item->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
	//		childItem->setText(0, QString(30, 0xB7));
	//	}
		child = child.nextSiblingElement();
	}
}

void ExperimentTree::parseElement(const QDomElement &element, EXML_TAG_SECTIONS tSection, QTreeWidgetItem *parentItem)
{
	if(!element.hasAttribute(ID_TAG))
		return;
	tmpString1 =element.attribute(ID_TAG,"");//Correct ObjectID?
	if (tmpString1.isEmpty())
	{
		return;//No ID Tag found!
	}
	tmpString2 = element.firstChildElement(NAME_TAG).text();
	if (!tmpString2.isEmpty())
	{
		tmpString1 = tmpString1 + ": " + tmpString2;
	}	
	QTreeWidgetItem *childItem = createItem(element, parentItem);
	childItem->setFlags(childItem->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));//childItem->setFlags(childItem->flags() | Qt::ItemIsEditable);
	childItem->setIcon(0, folderIcon);
	childItem->setText(0, tmpString1);
	//childItem->setText(1, tmpString2);
	parseProperties(element,tSection,childItem);

}

void ExperimentTree::parseProperties(const QDomElement &element, EXML_TAG_SECTIONS tSection, QTreeWidgetItem *parentItem)
{
	if(!element.hasAttribute(ID_TAG))
		return;
	tmpString1 =element.attribute(ID_TAG,"");//Correct ObjectID?
	if (!tmpString1.isEmpty())
	{
		QTreeWidgetItem *childItem = createItem(element, parentItem);
		childItem->setFlags(childItem->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));
		childItem->setIcon(0, propertyIcon);
		childItem->setText(0, ID_TAG);
		childItem->setText(1, tmpString1);
	}
	tmpString2 = element.firstChildElement(NAME_TAG).text();
	if (!tmpString2.isEmpty())
	{
		QTreeWidgetItem *childItem = createItem(element, parentItem);
		childItem->setFlags(childItem->flags() | Qt::ItemIsEditable);
		childItem->setIcon(0, propertyIcon);
		childItem->setText(0, NAME_TAG);
		childItem->setText(1, tmpString2);
	}	

	//QTreeWidgetItem *childItem = createItem(element, parentItem);
	//childItem->setFlags(childItem->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));//childItem->setFlags(childItem->flags() | Qt::ItemIsEditable);
	//childItem->setIcon(0, propertyIcon);
	//childItem->setText(0, tmpString1);
	//childItem->setText(1, tmpString2);

	//switch (tSection)
	//{
	//case EXML_TAG_SECTIONS::EXML_Object:
	//	tmpString1 = element.firstChildElement(OBJECT_TAG).text();
	//	if (tmpString1.isEmpty())//Is there an Object item?
	//	{
	//		return;
	//	}
	//	break;
	//case EXML_TAG_SECTIONS::EXML_Event:
	//	tmpString1 = element.firstChildElement(EVENT_TAG).text();
	//	if (tmpString1.isEmpty())//Is there an Event item?
	//	{
	//		return;
	//	}
	//	break;
	//default:
	//	return;
	//}

	//if (tmpString1.isEmpty())
	//{
	//	return;//No ID Tag found!
	//}
	//tmpString2 = element.firstChildElement(NAME_TAG).text();
	//if (!tmpString2.isEmpty())
	//{
	//	tmpString1 = tmpString1 + ": " + tmpString2;
	//}	
	//QTreeWidgetItem *childItem = createItem(element, parentItem);
	//childItem->setFlags(childItem->flags() & ~(Qt::ItemIsSelectable | Qt::ItemIsEditable));//childItem->setFlags(childItem->flags() | Qt::ItemIsEditable);
	//childItem->setIcon(0, propertyIcon);
	//childItem->setText(0, tmpString1);
	//childItem->setText(1, tmpString2);
}

QTreeWidgetItem *ExperimentTree::createItem(const QDomElement &element, QTreeWidgetItem *parentItem)
{
	QTreeWidgetItem *item;
	if (parentItem) 
	{
		item = new QTreeWidgetItem(parentItem);
	} 
	else 
	{
		item = new QTreeWidgetItem(this);
	}
	domElementForItem.insert(item, element);
	return item;
}

int ExperimentTree::getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList)
{
	int nDepth = sElementTagName.count();
	if(nDepth<=0)
		return -1;

	QDomElement tmpElem;
	for (int i=0;i<nDepth;i++)
	{
		if (i==(nDepth-1))//last one?
		{
			ResultDomNodeList = tmpElem.elementsByTagName(sElementTagName[i]);//Return all the elements from this level
			//ElemRoot = domDocument.elementsByTagName(sElementTagName[i]);
		}
		else if(i==0)//first one
		{
			//QDomNodeList ElemList = domDocument.elementsByTagName(sElementTagName[i]);
			//if(ElemList.count()>0)
			//{							
			//	tmpElem = ElemList.at(0).toElement();//Only first one for now!
			//}
			//else
			//	return false;
			tmpElem = domDocument.firstChildElement(sElementTagName[i]);//Only first one for now!
		}
		else
		{
			tmpElem = tmpElem.firstChildElement(sElementTagName[i]);//Only first one for now!
		}
		//ResultDomNodeList = domDocument.elementsByTagName(sElementTagName);
		//ResultDomNodeList = ResultDomNodeList.e
	}
	//ResultDomNodeList = ElemRoot.elementsByTagName(nDepth)
	//QDomDocument doc = // ...
	//	QDomElement root = doc.firstChildElement("database");
	//QDomElement elt = root.firstChildElement("entry");
	//for (; !elt.isNull(); elt = elt.nextSiblingElement("entry")) {
	//	// ...
	//}
	return ResultDomNodeList.count();		
}
