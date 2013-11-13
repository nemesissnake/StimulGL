#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QDomDocument>
#include "ExperimentGraphicEditor/ExperimentGraphicEditor_Global.h"

class QDomNode;
class QXmlStreamWriter;
class ExperimentTreeItem;

//using namespace ExperimentManagerNameSpace;

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

		bool read(QIODevice *device);
		bool read(QByteArray &byteArrayContent);
		//bool write(QIODevice *device);
		int getDocumentElements(const QStringList &sElementTagName,QDomNodeList &ResultDomNodeList);

    public slots:
        void saveNewData(QWidget *widgetContainer, const QModelIndex &parentIndex);

    private:
		void fillModel(const QString &xmlContent);
        void recursiveRead(QDomNode dNode, ExperimentTreeItem *item);
        void recursiveWrite(QXmlStreamWriter &xml, ExperimentTreeItem *item);
        void recursiveSearch(const QString &textToFind, const QStringList &filters, ExperimentTreeItem *item, QList<ExperimentTreeItem *> &list);
        void recursiveUidSearch(const QString &uuid, ExperimentTreeItem *item, bool found, QModelIndex &index);

		QDomDocument *doc;
		QDomElement *root;
        ExperimentTreeItem *rootItem;
		QString tmpString;
		ExperimentManagerNameSpace::EXML_DocVersion nXMLCurrentClassVersion;
		ExperimentManagerNameSpace::EXML_DocVersion nXMLDocumentVersion;
};

#endif // TREEMODEL_H
