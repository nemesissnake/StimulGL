#ifndef TREEFILTERPROXYMODEL_H
#define TREEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include "ExperimentGraphicEditor_Global.h"

class TreeFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

    public:
		explicit TreeFilterProxyModel(QObject *parent = NULL);
		void setTreeFilterSettings(const TreeFilterSettings &settings);
        
    protected:
        bool filterAcceptsRow(int source_row, const QModelIndex &index) const;

	private:
		TreeFilterSettings m_ViewSettings;
};

#endif // TREEFILTERPROXYMODEL_H
