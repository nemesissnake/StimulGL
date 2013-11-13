#ifndef TREEITEM_H
#define TREEITEM_H

#include <QUuid>
#include <QTime>
#include <QCryptographicHash>
#include <QStandardItem>
#include "Global.h"

class ExperimentTreeItem : public QStandardItem
{
	public:
        ExperimentTreeItem(const QString &name, const QString &value = "");
		ExperimentTreeItem(const ExperimentTreeItem &item);
		ExperimentTreeItem(ExperimentTreeItem *item);

        QString getUID() const { return uid.toString(); }
        QString getName() const { return QStandardItem::text(); }
        void setValue(const QString &value) { m_value = value; }
        QString getValue() const { return m_value; }
        void setStoreValue(bool store) { m_storeValue = store; }
        bool canStoreValue() const { return m_storeValue; }

        void addDefinition(const QString &defName, const QVariant &defValue, TreeItemType defType);
        void setDefinitions(const QMap<QString,TreeItemDefinition> &definitions);
        TreeItemDefinition getDefinition(const QString &defName) const;
        QMap<QString,TreeItemDefinition> getDefinitions() const;
        int getDefinitionsCount();
        //bool existDefinition(const QString &defName) const;

		QStringList getAvailableValuesList() const;
        QVariant getType() const;
        QVariant getDescription() const;
        QVariant getDefaultValue() const;
		ExperimentTreeItem * child(int row) const { return dynamic_cast<ExperimentTreeItem*>(QStandardItem::child(row,0)); }
        int childCount() const { return rowCount(); }
        ExperimentTreeItem * parent() const { return dynamic_cast<ExperimentTreeItem*>(QStandardItem::parent()); }
        int row() const { return QStandardItem::row(); }

    private:
        QUuid uid;
        QString m_value;
        bool m_storeValue;
        QMap<QString,TreeItemDefinition> m_definitions;
};

#endif // TREEITEM_H
