#include <QtGui>
#include "experimenttree.h"

#define MODULE_NAME			"Experiment Parser"
#define ROOT_TAG			"EXML"
#define VERSION_TAG			"version"
#define BOOKMARK_TAG		"bookmark"
#define HREF_TAG			"href"
#define FOLDER_TAG			"folder"
#define TITLE_TAG			"title"
#define BOOL_NO_TAG			"no"
#define BOOL_YES_TAG		"yes"
#define FOLDED_TAG			"folded"

#define DECLARATIONS_TAG	"declarations"
#define ACTIONS_TAG			"actions"
#define EVENT_TAG			"event"

ExperimentTree::ExperimentTree(QWidget *parent) : QTreeWidget(parent)
{
	nXMLDocumentVersion.major = 1;
	nXMLDocumentVersion.minor = 0;
	nXMLDocumentVersion.version = 0;
	nXMLDocumentVersion.build = 0;
	nXMLCurrentClassVersion.major = 1;
	nXMLCurrentClassVersion.minor = 0;
	nXMLCurrentClassVersion.version = 0;
	nXMLCurrentClassVersion.build = 0;

	QStringList labels;
	labels << tr("Elements") << tr("Properties");

	header()->setResizeMode(QHeaderView::Stretch);
	setHeaderLabels(labels);

	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon), QIcon::Normal, QIcon::Off);
	folderIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon), QIcon::Normal, QIcon::On);
	propertyIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));
}

ExperimentTree::~ExperimentTree()
{

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
			QStringList lVersion = root.attribute(VERSION_TAG).split(".");
			nXMLDocumentVersion.major = lVersion[0].toInt();
			nXMLDocumentVersion.minor = lVersion[1].toInt();
			nXMLDocumentVersion.version = lVersion[2].toInt();
			nXMLDocumentVersion.build = lVersion[3].toInt();
			//Is the documents version compatible with this class?
			if (!(nXMLDocumentVersion.major == nXMLCurrentClassVersion.major && nXMLDocumentVersion.minor == nXMLCurrentClassVersion.minor && nXMLDocumentVersion.version == nXMLCurrentClassVersion.version && nXMLDocumentVersion.build == nXMLCurrentClassVersion.build))
			{
				QMessageBox::information(window(), tr(MODULE_NAME), tr("The file is not an EXML version %1.%2.%3.%4")
				.arg(nXMLCurrentClassVersion.major)
				.arg(nXMLCurrentClassVersion.minor)
				.arg(nXMLCurrentClassVersion.version)
				.arg(nXMLCurrentClassVersion.build));
				return false;
			}
		}
		else
		{
			QMessageBox::information(window(), tr(MODULE_NAME), tr("The file version could not be determined."));
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
			//tmpString1 = child.firstChildElement(ID_TAG).text();
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
	tmpString1 = element.firstChildElement(ID_TAG).text();
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
	tmpString1 = element.firstChildElement(ID_TAG).text();
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

	//tmpString1 = element.firstChildElement(ID_TAG).text();
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

bool ExperimentTree::getDocumentElements(const QString &sElementTagName,QDomNodeList &ResultDomNodeList)
{
	ResultDomNodeList = domDocument.elementsByTagName(sElementTagName);
	return (ResultDomNodeList.count() > 0);		
	//QDomNodeList list = domDocument.elementsByTagName(sElementTagName);
	//int b = list.count();
	//for(int i=0;i<list.count();i++)
	//{
	//	QDomNode n = list.at(i);
	//	while (!n.isNull()) 
	//	{
	//		if (n.isElement()) 
	//		{
	//			QDomElement e = n.toElement();
	//			if (e.tagName() == a)
	//			{
	//				QString aa("ID");
	//				hh = e.firstChildElement(aa).text();
	//				QDomNode ee = e.namedItem(aa);
	//				if(!ee.isNull())
	//				{
	//					QDomElement de = ee.toElement();
	//					hh = ee.nodeName() + ":" + de.tagName() + ":" + de.attribute("ID","defvalue");
	//					//qDebug()qPrintable
	//				}
	//			}
	//			//qDebug() << "Element name: " << e.tagName() << endl;
	//			break;
	//		}
	//		n = n.nextSibling();
	//	}
	//	//bool c = list.at(i).hasChildNodes();
}
