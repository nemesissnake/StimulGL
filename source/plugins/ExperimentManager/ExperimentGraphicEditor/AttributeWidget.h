#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QMap>
#include "Global.h"

class QModelIndex;
class ExperimentTreeItem;

namespace Ui
{
    class AttributeWidget;
}

class AttributeWidget : public QWidget
{
    Q_OBJECT
        
    signals:
        void newAttributes(QMap<QString,QVariant> attributes);

    public:
        explicit AttributeWidget(ExperimentTreeItem *item);
        ~AttributeWidget();
        
    private:
        Ui::AttributeWidget *ui;
        QMap<QString,TreeItemDefinition> m_definitions;
        ExperimentTreeItem *itemEditing;

        void fillAttTable();

    private slots:
        void addAttributes();
        void removeAttributes();
        void saveData();
        void modifyTable(const QModelIndex &index);
};

#endif // ATTRIBUTEWIDGET_H
