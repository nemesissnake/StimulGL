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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QDomDocument>
//#include "ExperimentTreeFilterProxyModel.h"
#include "ExperimentGraphicEditor_Global.h"

class QDomNode;
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

        ExperimentTreeItem *getRootItem() const { return rootItem; }
        bool write(const QString &fileName);
        QList<ExperimentTreeItem*> getFilteredItemList(const QString &textToFind, const QStringList &filters);
        QModelIndex getIndexByUID(const QString &uuid);
        ExperimentTreeItem * itemFromIndex(const QModelIndex &index) const;

		bool read(QByteArray &byteArrayContent);
		bool reset();
		int getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList);

    public slots:
        void saveNewData(QWidget *widgetContainer, const QModelIndex &parentIndex);
		void saveNewData(const QString &sName, const QString &sValue, const QModelIndex &parentIndex);

    private:
		void recursiveRead(QDomNode dNode, ExperimentTreeItem *item);
        void recursiveWrite(QXmlStreamWriter &xml, ExperimentTreeItem *item);
        void recursiveSearch(const QString &textToFind, const QStringList &filters, ExperimentTreeItem *item, QList<ExperimentTreeItem *> &list);
        void recursiveUidSearch(const QString &uuid, ExperimentTreeItem *item, bool found, QModelIndex &index);
		bool fillModel();

		QDomDocument *doc;
		QDomElement *root;
        ExperimentTreeItem *rootItem;
		ExperimentManagerNameSpace::EXML_DocVersion nXMLCurrentClassVersion;
		ExperimentManagerNameSpace::EXML_DocVersion nXMLDocumentVersion;
};

#endif // TREEMODEL_H
