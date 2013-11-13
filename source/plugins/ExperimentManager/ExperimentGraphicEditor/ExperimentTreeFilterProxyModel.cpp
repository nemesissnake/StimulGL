#include "ExperimentTreeFilterProxyModel.h"
#include "ExperimentTreeModel.h"
#include "ExperimentTreeItem.h"

TreeFilterProxyModel::TreeFilterProxyModel(QObject *parent) : QSortFilterProxyModel(parent) 
{
	m_ViewSettings.bSkipComments = false;
	m_ViewSettings.bSkipEmptyAttributes = false;
}

void TreeFilterProxyModel::setTreeFilterSettings(const TreeFilterSettings &settings)
{
	m_ViewSettings = settings;
}

bool TreeFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &index) const
{
    //return true;
	if (index.isValid())
	{
		ExperimentTreeModel *model = dynamic_cast<ExperimentTreeModel*>(sourceModel());
		if (model)
		{
			ExperimentTreeItem *item = model->itemFromIndex(index);
			if(item)
			{
				if((item->child(source_row)->getName() == DEFINITION_TAG_COMMENT) && m_ViewSettings.bSkipComments)
				{
					return false;
				}
				else if ((item->child(source_row)->hasChildren() == false) && m_ViewSettings.bSkipEmptyAttributes)
				{
					return false;
				}
			}
		}
	}
    return true;    
    //Q_UNUSED(source_row);
    //Q_UNUSED(index);
    //return true;
}
