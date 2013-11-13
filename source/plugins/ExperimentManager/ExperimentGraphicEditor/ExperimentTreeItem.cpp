#include "ExperimentTreeItem.h"

ExperimentTreeItem::ExperimentTreeItem(const QString &name, const QString &value) : QStandardItem(name)
{
    m_value = value;
    uid = QUuid::createUuid();
}

ExperimentTreeItem::ExperimentTreeItem(const ExperimentTreeItem &item) : QStandardItem(item.getName())
{
    m_value = item.m_value;
    m_storeValue = item.m_storeValue;
    m_definitions = item.m_definitions;
    uid = QUuid::createUuid();
}

ExperimentTreeItem::ExperimentTreeItem(ExperimentTreeItem *item) : QStandardItem(item->getName())
{
    m_value = item->m_value;
    m_storeValue = item->m_storeValue;
    m_definitions = item->m_definitions;
    uid = QUuid::createUuid();
}

QVariant ExperimentTreeItem::getType() const
{
	if (m_definitions.contains("TYPE"))
		return m_definitions.value("TYPE").value;
	else if (m_definitions.contains("Type"))
		return m_definitions.value("Type").value;
	else if (m_definitions.contains("type"))
		return m_definitions.value("type").value;
	else
        return QVariant();
}

QVariant ExperimentTreeItem::getDescription() const
{
    if (m_definitions.contains("Description"))
        return m_definitions.value("Description").value;
    else if (m_definitions.contains("DESCRIPTION"))
        return m_definitions.value("DESCRIPTION").value;
    else if (m_definitions.contains("description"))
        return m_definitions.value("description").value;
    else
        return QVariant();
}

QVariant ExperimentTreeItem::getDefaultValue() const
{
    if (m_definitions.contains("Default"))
        return m_definitions.value("Default").value;
    else if (m_definitions.contains("DEFAULT"))
        return m_definitions.value("DEFAULT").value;
    else if (m_definitions.contains("default"))
        return m_definitions.value("default").value;
    else
        return QVariant();
}

QStringList ExperimentTreeItem::getAvailableValuesList() const
{
    if (m_definitions.contains("LIST"))
        return m_definitions.value("LIST").value.toString().split(";");
    else if (m_definitions.contains("List"))
        return m_definitions.value("List").value.toString().split(";");
    else if (m_definitions.contains("list"))
        return m_definitions.value("list").value.toString().split(";");
    else
        return QStringList();
}

void ExperimentTreeItem::addDefinition(const QString &defName, const QVariant &defValue, TreeItemType defType) 
{
	TreeItemDefinition tmpTreeItemDef;
	tmpTreeItemDef.type = defType;
	tmpTreeItemDef.value = defValue;
	m_definitions.insert(defName,tmpTreeItemDef); 
}

void ExperimentTreeItem::setDefinitions(const QMap<QString,TreeItemDefinition> &definitions) 
{ 
	m_definitions = definitions; 
}

TreeItemDefinition ExperimentTreeItem::getDefinition(const QString &defName) const 
{ 
	return m_definitions.value(defName); 
}

QMap<QString,TreeItemDefinition> ExperimentTreeItem::getDefinitions() const 
{ 
	return m_definitions; 
}

int ExperimentTreeItem::getDefinitionsCount() 
{ 
	return m_definitions.size(); 
}

//bool TreeItem::existDefinition(const QString &defName) const
//{
//	if (m_definitions.key(defName) != "")
//		return true;
//   return false;
//}
