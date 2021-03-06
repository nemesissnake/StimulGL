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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QDomDocument>
#include "ExperimentGraphicEditor_Global.h"
#include "ExperimentStructures.h"
#include "ExperimentTreeItem.h"

#define EXPERIMENTTREEMODEL_BLOCKOBJECT_INDEXID		-1
#define EXPERIMENTTREEMODEL_FILTER_TAGS				"TAGS"
#define EXPERIMENTTREEMODEL_FILTER_VALUES			"VALUES"
#define EXPERIMENTTREEMODEL_FILTER_ATTRIBUTES		"ATTRIBUTES"
#define EXPERIMENTTREEMODEL_FILTER_CASE_SENSITIVE	"CASE_SENSITIVE"

class QXmlStreamWriter;
class ExperimentTreeItem;

class ExperimentTreeModel : public QStandardItemModel
{
    Q_OBJECT

    signals:
        void modelModified();

    public:
        explicit ExperimentTreeModel(QObject *parent = 0);
		~ExperimentTreeModel();

		struct  strcParameterSpecifier
		{
			int nBlockID;
			int nObjectID;
			QString sParamHexID;
			QString sParamName;
			strcParameterSpecifier()
			{
				nBlockID = -1;
				nObjectID = -1;
				sParamName = "";
				sParamHexID = "";
			}
		};

		//QVariant data(const QModelIndex & index, int role = Qt::DisplayRole ) const;
        ExperimentTreeItem *getRootItem() const { return rootItem; }
        bool write(const QString &fileName);
        QList<ExperimentTreeItem*> getFilteredItemList(const QString &textToFind, const QStringList &filters, ExperimentTreeItem *expTreeItem = NULL);
        QModelIndex getIndexByUID(const QString &uuid);
        ExperimentTreeItem *itemFromIndex(const QModelIndex &index) const;

		bool read(QByteArray &byteArrayContent);
		bool reset();
		//int getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList);
		int getTreeElements(const QStringList &sElementTagName, QList<ExperimentTreeItem *> &lFoundTreeItems, ExperimentTreeItem *pSearchRootItem = NULL);
		static int getStaticTreeElements(const QStringList &sElementTagName, QList<ExperimentTreeItem *> &lFoundTreeItems, ExperimentTreeItem *pSearchRootItem);
		QList<ExperimentStructuresNameSpace::strcExperimentObject> getDefinedExperimentObjectInfoList(ExperimentTreeItem *objItem);

		bool removeExperimentBlocks(const QList<int> &lBlockIDs);
		bool moveExperimentBlocks(const QList<int> &lBlockIDsToMove, const int &nBlockIDToSwitch, const int &nBlockNumberChangeDirection);
		bool addExperimentBlocks(const int &nAmount = 1);
		bool removeExperimentParameters(const QList<ExperimentTreeModel::strcParameterSpecifier> lstParameterSpecifiers);

    public slots:
        void saveNewData(QWidget *widgetContainer, const QModelIndex &parentIndex);
		void saveNewData(const QString &sName, const QString &sValue, const QModelIndex &parentIndex, ExperimentTreeItem *pParametersSection = NULL);
		bool saveNewData(const int &nBlockID, const int &nObjectID, const QString &sParamName, const QString &sParamValue);

    private:
		void recursiveRead(QDomNode dNode, ExperimentTreeItem *item);
        void recursiveWrite(QXmlStreamWriter &xml, ExperimentTreeItem *item);
        static void recursiveSearch(const QString &textToFind, const QStringList &filters, ExperimentTreeItem *item, QList<ExperimentTreeItem *> &list);
        static void recursiveMultiSearch(const QString &textToFind, const QStringList &filters, QList<ExperimentTreeItem *> items, QList<ExperimentTreeItem*> &list);
		void recursiveUidSearch(const QString &uuid, ExperimentTreeItem *item, bool found, QModelIndex &index);
		bool fillModel();

		ExperimentTreeItem* getExperimentBlockTreeItem(const int &nBlockID);
		ExperimentTreeItem* getExperimentParameterTreeItem(const int &nBlockID, const int &nObjectID, const QString &sParamName);
		ExperimentTreeItem* addExperimentBlockTreeItems(const int &nAmount = 1);

		QDomDocument *doc;
		QDomElement *root;
        ExperimentTreeItem *rootItem;
		ExperimentManagerNameSpace::EXML_DocVersion nXMLCurrentClassVersion;
		ExperimentManagerNameSpace::EXML_DocVersion nXMLDocumentVersion;
};

#endif // TREEMODEL_H
