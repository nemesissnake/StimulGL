#ifndef EXPERIMENTTREE_H
#define EXPERIMENTTREE_H

#define ID_TAG				"ID"
#define OBJECT_TAG			"object"
#define CLASS_TAG			"class"
#define NAME_TAG			"name"

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>

enum EXML_TAG_SECTIONS
{
	EXML_Declarations,
	EXML_Actions,
	EXML_Object,
	EXML_Event
};

struct EXML_DocVersion 
{
	int major;
	int minor;
	int version;
	int build;
};

class ExperimentTree : public QTreeWidget
{
	Q_OBJECT

public:
	ExperimentTree(QWidget *parent = 0);
	~ExperimentTree();

	bool read(QIODevice *device);
	bool write(QIODevice *device);

	bool getDocumentElements(const QString &sElementTagName,QDomNodeList &ResultDomNodeList);

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
