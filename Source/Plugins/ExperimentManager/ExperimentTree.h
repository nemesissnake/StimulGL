#ifndef EXPERIMENTTREE_H
#define EXPERIMENTTREE_H

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>
#include "Global.h"

class ExperimentTree : public QTreeWidget
{
	Q_OBJECT

public:
	ExperimentTree(QWidget *parent = 0);
	~ExperimentTree();

	bool read(QIODevice *device);
	bool write(QIODevice *device);

	bool getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList);

private slots:
	void updateDomElement(QTreeWidgetItem *item, int column);

private:
	void parseRootElement(const QDomElement &element, EXML_TAG_SECTIONS tSection, QTreeWidgetItem *parentItem = 0);
	void parseElement(const QDomElement &element, EXML_TAG_SECTIONS tSection, QTreeWidgetItem *parentItem = 0);
	void parseProperties(const QDomElement &element, EXML_TAG_SECTIONS tSection, QTreeWidgetItem *parentItem = 0);
	QTreeWidgetItem *createItem(const QDomElement &element, QTreeWidgetItem *parentItem = 0);
	QDomDocument domDocument;
	QHash<QTreeWidgetItem *, QDomElement> domElementForItem;
	QIcon folderIcon;
	QIcon propertyIcon;
	QString tmpString1;
	QString tmpString2;

	EXML_DocVersion nXMLCurrentClassVersion;
	EXML_DocVersion nXMLDocumentVersion;
};

#endif // EXPERIMENTTREE_H
