//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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


#ifndef EXPERIMENTTREE_H
#define EXPERIMENTTREE_H

#include <QDomDocument>
#include <QHash>
#include <QIcon>
#include <QTreeWidget>
#include "Global.h"

using namespace ExperimentManagerNameSpace;

class ExperimentTree : public QTreeWidget
{
	Q_OBJECT

public:
	ExperimentTree(QWidget *parent = NULL);
	~ExperimentTree();

	bool read(QIODevice *device);
	bool read(QByteArray &byteArrayContent);
	bool write(QIODevice *device);

	int getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList);

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
