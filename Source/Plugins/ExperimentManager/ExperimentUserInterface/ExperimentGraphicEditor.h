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

#ifndef ExperimentGraphicEditor_H
#define ExperimentGraphicEditor_H

#define APP_NAME		"Experiment Graphic Editor"
#define VERSION			"v1.0"
#define TABLEVIEWINDEX	1

#include <QWidget>
#include <QModelIndex>
#include "ExperimentStructures.h"
#include "ExperimentTreeModel.h"
#include "ExperimentGraphicEditor_Global.h"

class QMenu;
class QToolButton;
class QVBoxLayout;
class QTreeView;
class QScrollArea;
class QTableWidget;
class QGridLayout;
class QToolBar;
class QtProperty;
class QSplitter;
class FindDialog;
class AttributeWidget;
class ExperimentTreeModel;
class ExperimentTreeItem;
class TreeFilterProxyModel;
class ExperimentManager;
class ExperimentStructureVisualizer;
class cExperimentStructure;
class ExperimentParameterDefinitionContainer;
class ExperimentParameterVisualizer;
class ExperimentBlockParameterView;

class ExperimentGraphicEditor : public QWidget
{
    Q_OBJECT

signals:
	void onTableViewRedrawned(int nNewWidth, int nNewHeight);
        
public:
    explicit ExperimentGraphicEditor(QWidget *parent = 0);
    ~ExperimentGraphicEditor();
        
public slots:
	void setExperimentManager(ExperimentManager *pExpManager);
	bool setExperimentTreeModel(ExperimentTreeModel *expModel = NULL);
		
//protected slots:
//	void closeEvent(QCloseEvent *event);
//	void resizeEvent(QResizeEvent * event);

private slots:
    void newFile();
	void openFile();
	void closeFile();
	void saveFile();
	void showInfo(const QModelIndex &index);
	void fillTableView(const QString &textToFind, const QStringList &filters);
	void tableViewResized(int pos, int index);
    void selectTreeItem();
    void showFindDialog();
    void setNewModel();
    void insertNode();
    void insertSubnode();
    void removeNode();
    void addDefinition();
    void saveNewData();
	void saveNewData(const QString &sName, const QString &sValue);
	void setViewFilter(const TreeFilterSettings &newViewSettings);
	void updateWindowTitle(const QString sSuffix = QString(""));

	void childWidgetDestroyed(QWidget* pWidget);

private:
	void configureActions(bool bCreate);
	void setupExperimentTreeView();
	void setupMenuAndActions();
	void setupLayout();
	void setupFilterModel();
	QList<ExperimentStructuresNameSpace::strcExperimentObject> getDefinedExperimentObjectInfoList(ExperimentTreeItem *objItem);

	bool bShowTreeView;

	QTreeView *treeView;
	QToolBar *toolBar;
	QSplitter *horSplitter;
	QScrollArea *scrollArea;
	QWidget *dynamicGraphicWidget;
	QGridLayout *gridLayout;

	QTableWidget *tblWidgetView;
	TreeFilterProxyModel *filterModel;
	ExperimentTreeModel *pExpTreeModel;		//used for setExperimentTreeModel() -> Pointer
	ExperimentTreeModel loadedExpTreeModel; //used for openFile();
	FindDialog *findDlg;
	AttributeWidget *attWidget;	

	QVBoxLayout *mainLayout;
	QAction *actionNew_File;
	QAction *action_Open_File;
	QAction *action_Save;
	QAction *action_Close_File;
	QAction *actionFind;
	QAction *action_Quit;
	QAction *actionAdd_Node;
	QAction *actionAdd_Subnode;
	QAction *action_Remove_Node;
	QAction *actionAdd_Attribute;
	QMenu *menuFile;
	QMenu *menuEdit;
	QToolButton *buttonFile;
	QToolButton *buttonEdit;

	//ExperimentTreeItem *rootItem;

	TreeFilterSettings currentViewSettings;
	QModelIndex selectedIndex;

	ExperimentManager *expManager;
	ExperimentStructureVisualizer *expStructVisualizer;
	ExperimentBlockParameterView *expBlockParamView;
	cExperimentStructure *tmpExpStruct; 
	ExperimentParameterDefinitionContainer *tmpExpObjectParamDefs;
	QHash<QString, ExperimentParameterVisualizer *> staticGraphicWidgetsHashTable;
};

#endif // ExperimentGraphicEditor_H
