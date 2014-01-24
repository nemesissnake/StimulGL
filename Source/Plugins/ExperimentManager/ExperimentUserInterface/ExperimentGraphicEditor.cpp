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

#include <QMenu>
#include <QCloseEvent>
#include <QToolButton>
#include <QAction>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QToolBar>
#include <QTreeView>
#include <QScrollArea>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSplitter>
#include <QMessageBox>
#include "FindDialog.h"
#include "AttributeWidget.h"
#include "ExperimentTreeFilterProxyModel.h"
#include "ExperimentManager.h"
#include "ExperimentGraphicEditor.h"
#include "ExperimentTreeItem.h"
#include "ExperimentStructureVisualizer.h"
#include "ExperimentParameterVisualizer.h"
#include "ExperimentParameterDefinition.h"
#include "ExperimentParameterWidgets.h"

ExperimentGraphicEditor::ExperimentGraphicEditor(QWidget *parent) : QWidget(parent)//, ui(new Ui::ExperimentGraphicEditor)
{
	treeView = NULL;
	attWidget = NULL;
	gridLayout = NULL;
	tblWidgetView = NULL;
	scrollArea = NULL;
	horSplitter = NULL;
	toolBar = NULL;
	findDlg = NULL;
	dynamicGraphicWidget = NULL;
	pExpTreeModel = NULL;
	filterModel = NULL;
	mainLayout = NULL;
	actionNew_File = NULL;
	action_Open_File = NULL;
	action_Save = NULL;
	action_Close_File = NULL;
	actionFind = NULL;
	action_Quit = NULL;
	actionAdd_Node = NULL;
	actionAdd_Subnode = NULL;
	action_Remove_Node = NULL;
	actionAdd_Attribute = NULL;
	menuFile = NULL;
	menuEdit = NULL;
	buttonFile = NULL;
	buttonEdit = NULL;
	expManager = NULL;
	tmpExpStruct = NULL;
	expStructVisualizer = NULL;
	tmpExpObjectParamDefs = NULL;
	//rootItem = NULL;
	
	currentViewSettings.bSkipComments = true;
	currentViewSettings.bSkipEmptyAttributes = false;
	
	//this->setAttribute( Qt::WA_DeleteOnClose );
	//connect( widget, SIGNAL(destroyed(QObject*)), this, SLOT(widgetDestroyed(QObject*)) );
	configureActions(true);
	setupExperimentTreeView();
	setupMenuAndActions();
	setupLayout();
	updateWindowTitle();
}

ExperimentGraphicEditor::~ExperimentGraphicEditor()
{
	configureActions(false);
	staticGraphicWidgetsHashTable.clear();
	if(gridLayout)
		delete gridLayout;
	if(treeView)
		delete treeView;
	if(attWidget)
		delete attWidget;
	if(dynamicGraphicWidget)
		delete dynamicGraphicWidget;	
	if(tblWidgetView)
		delete tblWidgetView;
	if(scrollArea)
	{
		scrollArea->takeWidget();//Important, otherwise the singleton widgets get deleted...
		delete scrollArea;
	}
	if(horSplitter)
		delete horSplitter;
	if(menuFile)
		delete menuFile;
	if(menuEdit)
		delete menuEdit;
	if(buttonFile)
		delete buttonFile;
	if(buttonEdit)
		delete buttonEdit;
	if(toolBar)
		delete toolBar;
	if(pExpTreeModel)
		pExpTreeModel = NULL;
	if(findDlg)
		delete findDlg;
	if(filterModel)
		delete filterModel;
	if(mainLayout)
		delete mainLayout;
	if(expManager)
		expManager = NULL;
	if(tmpExpStruct)
		delete tmpExpStruct;
	if(tmpExpObjectParamDefs)
		tmpExpObjectParamDefs = NULL;
	//if(rootItem)
	//	delete rootItem;
}

void ExperimentGraphicEditor::setExperimentManager(ExperimentManager *pExpManager)
{
	expManager = pExpManager;
}

void ExperimentGraphicEditor::updateWindowTitle(const QString sSuffix)
{
	QString title = APP_NAME " " VERSION;
	setWindowTitle(title);
}

void ExperimentGraphicEditor::configureActions(bool bCreate)
{
	if(bCreate)
	{
		actionNew_File = new QAction("New Experiment File", this);
		action_Open_File = new QAction("Open Experiment File", this);
		//action_Open_File->setShortcut(QKeySequence::Open);
		action_Save = new QAction("Save Experiment File", this);
		action_Close_File = new QAction("Close Experiment File", this);
		actionFind = new QAction("Find", this);
		action_Quit = new QAction("Quit", this);
		actionAdd_Node = new QAction("Add Node", this);
		actionAdd_Subnode = new QAction("Add sub-Node", this);
		action_Remove_Node = new QAction("Remove Node", this);
		actionAdd_Attribute = new QAction("Add Node Attribute", this);
	}
	else
	{
		if(actionNew_File)
		{
			delete actionNew_File;
			actionNew_File = NULL;
		}
		if(action_Open_File)
		{
			delete action_Open_File;
			action_Open_File = NULL;
		}
		if(action_Save)
		{
			delete action_Save;
			action_Save = NULL;
		}
		if(action_Close_File)
		{
			delete action_Close_File;
			action_Close_File = NULL;
		}
		if(actionFind)
		{
			delete actionFind;
			actionFind = NULL;
		}
		if(action_Quit)
		{
			delete action_Quit;
			action_Quit = NULL;
		}
		if(actionAdd_Node)
		{
			delete actionAdd_Node;
			actionAdd_Node = NULL;
		}
		if(actionAdd_Subnode)
		{
			delete actionAdd_Subnode;
			actionAdd_Subnode = NULL;
		}
		if(action_Remove_Node)
		{
			delete action_Remove_Node;
			action_Remove_Node = NULL;
		}
		if(actionAdd_Attribute)
		{
			delete actionAdd_Attribute;
			actionAdd_Attribute = NULL;
		}
	}

}

void ExperimentGraphicEditor::setupExperimentTreeView()
{
	horSplitter = new QSplitter(Qt::Horizontal, this);
	treeView = new QTreeView(horSplitter);
	scrollArea = new QScrollArea(horSplitter);
	tblWidgetView = new QTableWidget(horSplitter);

	horSplitter->addWidget(treeView);
	horSplitter->addWidget(scrollArea);
	horSplitter->addWidget(tblWidgetView);
	horSplitter->setStretchFactor(0,1);
	horSplitter->setStretchFactor(1,4);
	horSplitter->setStretchFactor(2,1);

	treeView->setHeaderHidden(true);
	//treeView->setEditTriggers(QTreeView::NoEditTriggers);

	tblWidgetView->setSelectionMode(QTableWidget::SingleSelection);
	tblWidgetView->setSelectionBehavior(QTableWidget::SelectRows);
	tblWidgetView->setEditTriggers(QTableWidget::NoEditTriggers);
	tblWidgetView->setVisible(false);
	tblWidgetView->setColumnHidden(4, true);
	tblWidgetView->horizontalHeader()->setHidden(true);
	tblWidgetView->verticalHeader()->setHidden(true);
		
	connect(horSplitter, SIGNAL(splitterMoved(int, int)), this, SLOT(tableViewResized(int, int)));
	//connect(tblWidgetView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectTreeItem()));
	connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(showInfo(QModelIndex)));
}

void ExperimentGraphicEditor::tableViewResized(int pos, int index)
{
	Q_UNUSED(pos);
	Q_UNUSED(index);
	if(horSplitter->count() > TABLEVIEWINDEX)
	{
		emit onTableViewRedrawned(horSplitter->sizes().at(TABLEVIEWINDEX),horSplitter->childAt(TABLEVIEWINDEX,0)->size().height());
	}
}

void ExperimentGraphicEditor::setupMenuAndActions()
{
	toolBar = new QToolBar(this);
	
	//File menu///
	buttonFile=new QToolButton(this);
	buttonFile->setText("File ");
	buttonFile->setPopupMode(QToolButton::InstantPopup);
	menuFile = new QMenu();//FileButton
	menuFile->addAction(actionNew_File);
	menuFile->addAction(action_Open_File);
	menuFile->addAction(action_Save);
	menuFile->addAction(action_Close_File);
	menuFile->addAction(action_Quit);
	buttonFile->setMenu(menuFile);
	toolBar->addWidget(buttonFile);
	//Edit menu///
	buttonEdit=new QToolButton(this);
	buttonEdit->setText("Edit ");
	buttonEdit->setPopupMode(QToolButton::InstantPopup);
	menuEdit = new QMenu();//FileButton
	menuEdit->addAction(actionFind);
	menuEdit->addAction(actionAdd_Node);
	menuEdit->addAction(actionAdd_Subnode);
	menuEdit->addAction(action_Remove_Node);
	menuEdit->addAction(actionAdd_Attribute);
	buttonEdit->setMenu(menuEdit);
	toolBar->addWidget(buttonEdit);
	
	connect(actionNew_File, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(action_Open_File, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(action_Save, SIGNAL(triggered()), this, SLOT(saveFile()));
	connect(action_Close_File, SIGNAL(triggered()), this, SLOT(closeFile()));
	connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
	connect(actionFind, SIGNAL(triggered()), this, SLOT(showFindDialog()));
	connect(actionAdd_Node, SIGNAL(triggered()), this, SLOT(insertNode()));
	connect(actionAdd_Subnode, SIGNAL(triggered()), this, SLOT(insertSubnode()));
	connect(action_Remove_Node, SIGNAL(triggered()), this, SLOT(removeNode()));
	connect(actionAdd_Attribute, SIGNAL(triggered()), this, SLOT(addDefinition()));
}

void ExperimentGraphicEditor::setupLayout()
{
	mainLayout = new QVBoxLayout();
	mainLayout->setMenuBar(toolBar);
	mainLayout->addWidget(horSplitter);
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);	
}

void ExperimentGraphicEditor::setupFilterModel()
{
	if(filterModel)
		delete filterModel;
	filterModel = new TreeFilterProxyModel();
	if(pExpTreeModel)
		filterModel->setSourceModel(pExpTreeModel);
	filterModel->setTreeFilterSettings(currentViewSettings);
	treeView->setModel(filterModel);
}

void ExperimentGraphicEditor::newFile()
{
	bool createNew = true;
	if (pExpTreeModel)
	{
		int confirm = QMessageBox::question(this, tr("Close current file"), tr("Are you sure to close the current file?"), QMessageBox::Ok | QMessageBox::Cancel);
		if (confirm == QMessageBox::Cancel)
			createNew = false;
	}

	if (createNew)
	{
		closeFile();

		action_Save->setEnabled(true);
		action_Close_File->setEnabled(true);
		actionAdd_Attribute->setEnabled(true);
		actionAdd_Node->setEnabled(true);
		actionAdd_Subnode->setEnabled(true);
		action_Remove_Node->setEnabled(true);
		actionFind->setEnabled(true);

		pExpTreeModel = &loadedExpTreeModel;
		connect(pExpTreeModel, SIGNAL(modelModified()), this, SLOT(setNewModel()));
		//rootItem = new ExperimentTreeItem("[Root node]");
		//pExpTreeModel->setItem(0, rootItem);
		setupFilterModel();
	}
}

void ExperimentGraphicEditor::openFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Experiment Files (*.exml);;XML Files (*.xml)"));

	if (fileName != "")
	{
		QFile expFile(fileName);
		if (expFile.open(QFile::ReadOnly | QFile::Text)) 
		{
			closeFile();

			action_Save->setEnabled(true);
			action_Close_File->setEnabled(true);
			actionAdd_Attribute->setEnabled(true);
			actionAdd_Node->setEnabled(true);
			actionAdd_Subnode->setEnabled(true);
			action_Remove_Node->setEnabled(true);
			actionFind->setEnabled(true);			
			
			QString tmpString = expFile.readAll();			
			expFile.close();

			loadedExpTreeModel.read(tmpString.toLatin1());
			pExpTreeModel = &loadedExpTreeModel;
			connect(pExpTreeModel, SIGNAL(modelModified()), this, SLOT(setNewModel()));
			setupFilterModel();
		}
		else
		{
			qDebug() << __FUNCTION__ << "Could not open the file " << fileName;
		}
	}
}

void ExperimentGraphicEditor::closeFile()
{
	//Deleting model && cleaning QTreeView
	treeView->setModel(0);
	loadedExpTreeModel.reset();
	if (pExpTreeModel)
	{
		pExpTreeModel = NULL;
	}
	//Deleting scroll widget && cleaning QSrollArea
	if (dynamicGraphicWidget != NULL)
	{
		delete dynamicGraphicWidget;
		dynamicGraphicWidget = NULL;
	}
	//Hiding QTableWidget
	tblWidgetView->setHidden(true);
	action_Save->setDisabled(true);
	action_Close_File->setDisabled(true);
	actionAdd_Attribute->setDisabled(true);
	actionAdd_Node->setDisabled(true);
	actionAdd_Subnode->setDisabled(true);
	action_Remove_Node->setDisabled(true);
	actionFind->setDisabled(true);
	updateWindowTitle();
}

void ExperimentGraphicEditor::saveFile()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Experiment Files (*.exml);;XML Files (*.xml)"));
	if (fileName.isEmpty() == false)
	{
		pExpTreeModel->write(fileName);
		updateWindowTitle(" - " + fileName);
	}
}

void ExperimentGraphicEditor::setNewModel()
{
	treeView->reset();
	treeView->activateWindow();
	treeView->setFocus(Qt::OtherFocusReason);
	if(filterModel == NULL)
	{
		filterModel = new TreeFilterProxyModel();
	}	
	filterModel->setSourceModel(pExpTreeModel);
	filterModel->setTreeFilterSettings(currentViewSettings);
	treeView->setModel(filterModel);
	treeView->selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::ClearAndSelect);
	treeView->expandAll();
}

void ExperimentGraphicEditor::showInfo(const QModelIndex &index)
{
	selectedIndex = index;
	QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(index);
	ExperimentTreeItem *item = pExpTreeModel->itemFromIndex(originalIndex);
	if (dynamicGraphicWidget != NULL)
	{
		delete dynamicGraphicWidget;
		dynamicGraphicWidget = NULL;
	}
	if (item->hasChildren())
	{
		dynamicGraphicWidget = new QWidget();
		gridLayout = new QGridLayout();
		ExperimentTreeItem *child;
		QVariant tmpVarValue;
		QList<int> nIDList;		
		ExperimentParameterVisualizer *tmpParametersWidget = NULL;
		//bool bDoParseDependencies = false;
		if(tmpExpObjectParamDefs)
			tmpExpObjectParamDefs = NULL;
		for (int i = 0; i < item->rowCount(); i++)
		{
			child = item->child(i);
			if(item->parent()->getName().toLower() == DEFINES_TAG)
			{
				if(i==0)
				{
					if(item->getName().toLower() == EXPERIMENT_TAG)
					{
						QString sParamCollName = EXPERIMENT_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == NAME_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(NAME_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == DEBUGMODE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(DEBUGMODE_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(item->parent()->getName().toLower() == DECLARATIONS_TAG)
			{
				if(i==0)
				{
					if (item->getName().toLower() == OBJECT_TAG)
					{
						QString sParamCollName = DECLARATIONS_OBJECT_TAG;//OBJECT_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == NAME_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(NAME_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == CLASS_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(CLASS_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(item->parent()->getName().toLower() == CONNECTIONS_TAG)
			{
				if(i==0)
				{
					if (item->getName().toLower() == OBJECT_TAG)
					{
						QString sParamCollName = CONNECTIONS_OBJECT_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == CONNECTIONS_SIGNATURE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(CONNECTIONS_SIGNATURE_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == CONNECTIONS_TYPE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(CONNECTIONS_TYPE_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(item->parent()->getName().toLower() == INITIALIZATIONS_TAG)
			{
				if(i==0)
				{
					if (item->getName().toLower() == OBJECT_TAG)
					{
						QString sParamCollName = INITIALIZATIONS_OBJECT_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == INIT_FINIT_SIGNATURE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(INIT_FINIT_SIGNATURE_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == INIT_FINIT_TYPE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(INIT_FINIT_TYPE_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(item->parent()->getName().toLower() == BLOCKTRIALS_TAG)
			{
				if(i==0)
				{
					if (item->getName().toLower() == BLOCK_TAG)
					{
						QString sParamCollName = BLOCK_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == NAME_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(NAME_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == BLOCKNUMBER_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(BLOCKNUMBER_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == TRIALAMOUNT_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(TRIALAMOUNT_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == INTERNALTRIGGERAMOUNT_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(INTERNALTRIGGERAMOUNT_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == EXTERNALTRIGGERAMOUNT_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(EXTERNALTRIGGERAMOUNT_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(item->parent()->getName().toLower() == LOOPS_TAG)
			{
				if(i==0)
				{
					if (item->getName().toLower() == LOOP_TAG)
					{
						QString sParamCollName = LOOP_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == NAME_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(NAME_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == LOOP_NUMBER_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(LOOP_NUMBER_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == LOOP_AMOUNT_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(LOOP_AMOUNT_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == LOOP_TARGETBLOCKID_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(LOOP_TARGETBLOCKID_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(item->parent()->getName().toLower() == FINALIZATIONS_TAG)
			{
				if(i==0)
				{
					if (item->getName().toLower() == OBJECT_TAG)
					{
						QString sParamCollName = FINALIZATIONS_OBJECT_TAG;
						if(staticGraphicWidgetsHashTable.contains(sParamCollName))
						{
							tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
						}
						else
						{
							ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
							tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
							if(tmpParametersWidget == NULL)
							{
								qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
							}
							else
							{
								staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
							}								
						}
						//Set the values
						int nChildCount = item->childCount();
						QString sTmpString = "";
						QString sValue = "";
						for (int j=0;j<nChildCount;j++)
						{							
							if(item->child(j)->getName().toLower() == INIT_FINIT_SIGNATURE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(INIT_FINIT_SIGNATURE_TAG,sTmpString);
							}
							else if(item->child(j)->getName().toLower() == INIT_FINIT_TYPE_TAG)
							{
								sTmpString = item->child(j)->getValue();
								if(sTmpString.isEmpty() == false)
									tmpParametersWidget->setParameter(INIT_FINIT_TYPE_TAG,sTmpString);
							}
						}
					}
				}
			}
			else if(child->hasChildren())
			{
				if((item->getName().toLower() == BLOCKTRIALS_TAG) && (i==0))
				{						
					if(pExpTreeModel)
					{
						QDomNodeList tmpList;
						if(tmpExpStruct)
							delete tmpExpStruct;
						tmpExpStruct = new cExperimentStructure();

						if(expStructVisualizer == NULL)
							expStructVisualizer = new ExperimentStructureVisualizer();//dynamicGraphicWidget);
						if(expStructVisualizer)
						{
							bool bResult = connect(expStructVisualizer, SIGNAL(destroyed(QWidget*)), this, SLOT(childWidgetDestroyed(QWidget*)));
							bResult = connect(this, SIGNAL(onTableViewRedrawned(int, int)), expStructVisualizer, SLOT(resizeStructureView(int, int)));
							bResult = expManager->createExperimentStructure(tmpList, pExpTreeModel,tmpExpStruct);
							if(bResult)
							{
								//int nRows = gridLayout->rowCount();
								//int nColumns = gridLayout->columnCount();
								//gridLayout->addWidget(expStructVisualizer,0,1);

								if(horSplitter->count() > TABLEVIEWINDEX)
									expStructVisualizer->resizeStructureView(horSplitter->sizes().at(TABLEVIEWINDEX),horSplitter->childAt(TABLEVIEWINDEX,0)->size().height());
								bool bResult = expStructVisualizer->parseExperimentStructure(tmpExpStruct);								
								if(bResult)
								{
									//expStructVisualizer->setLayout(gridLayout);
									scrollArea->takeWidget();
									scrollArea->setWidget(expStructVisualizer);
									expStructVisualizer->showMaximized();
									return;
								}
							}
						}
					}
					break;
				}
				else if((item->getName().toLower() == PARAMETERS_TAG))
				{	
					if((child->getName().toLower() == PARAMETER_TAG))
					{
						if(child->hasChildren())
						{
							if((item->parent() != NULL) && (item->parent()->parent() != NULL))// && (item->parent()->parent()->getName().toLower() == BLOCK_TAG))
							{
								if(i==0)
								{
									if(item->parent()->getName().toLower() == OBJECT_TAG)
									{
										ExperimentStructuresNameSpace::strcExperimentObject tmpExperimentObject;
										tmpExperimentObject = getDefinedExperimentObjectInfo(item->parent());
										if(tmpExperimentObject.nID >= 0)
										{
											QString sParamCollName = "";
											if(item->parent()->parent()->getName().toLower() == BLOCK_TAG)
											{
												sParamCollName = tmpExperimentObject.sClass;//Case sensitive!!;
											}
											else if(item->parent()->parent()->getName().toLower() == INITIALIZATIONS_TAG)
											{
												sParamCollName = ""; 
											}
											if(sParamCollName.isEmpty() == false)
											{
												if(staticGraphicWidgetsHashTable.contains(sParamCollName))
												{
													tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
												}
												else
												{
													ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
													tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
													if(tmpParametersWidget == NULL)
													{
														qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
													}
													else
													{
														staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
													}								
												}
											}
										}									
									}
								}
								if(tmpParametersWidget)
								{
									//Set the values
									int nChildCount = child->childCount();
									QString sName = "";
									QString sValue = "";
									for (int j=0;j<nChildCount;j++)
									{							
										if(item->child(i)->child(j)->getName().toLower() == NAME_TAG)
										{
											sName = item->child(i)->child(j)->getValue();
										}
										else if(item->child(i)->child(j)->getName().toLower() == VALUE_TAG)
										{
											sValue = item->child(i)->child(j)->getValue();
										}
										if((sName.isEmpty() || sValue.isEmpty()) == false)
										{
											//bool bResult = 
											tmpParametersWidget->setParameter(sName,sValue);
										}
									}
								}								
							}
						}
					}
				}
			}
			else//unknown and no children items...
			{
				if(item->getName().toLower() == PARAMETER_TAG)
				{
					if((item->parent()) && (item->parent()->parent()) && (item->parent()->parent()->parent())) 
					{
						if(i==0)
						{
							QString sParamCollName = "";
							if (item->parent()->parent()->parent()->getName() == INITIALIZATIONS_TAG)
							{
								sParamCollName = INITIALIZATIONS_PARAMETER_TAG;
							}
							else if(item->parent()->parent()->parent()->getName() == BLOCK_TAG)
							{
								sParamCollName = BLOCK_PARAMETER_TAG;
							}
							if(sParamCollName.isEmpty() == false)
							{
								if(staticGraphicWidgetsHashTable.contains(sParamCollName))
								{
									tmpParametersWidget = staticGraphicWidgetsHashTable.value(sParamCollName);
								}
								else
								{
									ExperimentParameterWidgets *expParamWidgets = ExperimentParameterWidgets::instance();
									tmpParametersWidget = expParamWidgets->getExperimentParameterWidget(sParamCollName);
									if(tmpParametersWidget == NULL)
									{
										qDebug() << __FUNCTION__ << "Could not fetch parameter collection widget named " << sParamCollName;
									}
									else
									{
										staticGraphicWidgetsHashTable.insert(sParamCollName,tmpParametersWidget);
									}								
								}
							}
						}
						if(tmpParametersWidget)
						{
							tmpParametersWidget->setParameter(child->getName().toLower(),child->getValue());
						}
					}
				}
				else
				{
					gridLayout->addWidget(new QLabel(child->getName(), dynamicGraphicWidget),i,0);
					QString defValue;
					if (child->getType().toString().toLower() == "enum" || child->getType().toString().toLower() == "enum_text" || child->getType().toString().toLower() == "list")
					{
						QComboBox *cbList = new QComboBox(dynamicGraphicWidget);
						cbList->addItem("<Default>");
						cbList->addItems(child->getAvailableValuesList());
						QString value = "<Default>";
						if (child->getValue() != "")
							value = child->getValue();
						defValue = child->getDefaultValue().toString();
						cbList->setCurrentIndex(cbList->findText(value));
						cbList->setToolTip(child->getDescription().toString());
						gridLayout->addWidget(cbList,i,1);
						connect(cbList, SIGNAL(currentIndexChanged(int)), this, SLOT(saveNewData()));
					}
					else if (child->getType().toString().toLower() == "bool" || child->getType().toString().toLower() == "boolean")
					{
						QComboBox *cbBool = new QComboBox(dynamicGraphicWidget);
						QStringList items;
						items.append("<Default>");
						items.append("TRUE");
						items.append("FALSE");
						cbBool->addItems(items);
						QString strValue = "<Default>";
						if (child->getValue() != "")
						{
							bool value = false;
							value = QVariant(child->getValue()).toBool();
							if (value)
								strValue = "TRUE";
							else
								strValue = "FALSE";
						}
						defValue = child->getDefaultValue().toString();
						if (defValue == "1")
							defValue = "TRUE";
						else
							defValue = "FALSE";
						cbBool->setCurrentIndex(cbBool->findText(strValue));
						cbBool->setToolTip(child->getDescription().toString());
						gridLayout->addWidget(cbBool,i,1);
						connect(cbBool, SIGNAL(currentIndexChanged(int)), this, SLOT(saveNewData()));
					}
					else
					{
						QLineEdit *lineEdit = new QLineEdit("<Default>",dynamicGraphicWidget);
						lineEdit->setToolTip(child->getDescription().toString());
						QString value = child->getValue();
						if (value != "")
							lineEdit->setText(value);
						defValue = child->getDefaultValue().toString();
						connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(saveNewData()));
						gridLayout->addWidget(lineEdit,i,1);
					}
					if (defValue.size() > 0)
					{
						defValue = "(Default=" + defValue + ")";
						QLabel *labelDefault = new QLabel(defValue);
						gridLayout->addWidget(labelDefault,i,2);
					}
					QLabel *labelID = new QLabel(child->getUID());
					labelID->setVisible(false);
					gridLayout->addWidget(labelID,i,3);
				}
			}
		}
		if(tmpParametersWidget)
		{
			connect(tmpParametersWidget, SIGNAL(destroyed(QWidget*)), this, SLOT(childWidgetDestroyed(QWidget*)));
			connect(this, SIGNAL(onTableViewRedrawned(int, int)), tmpParametersWidget, SLOT(resizeParameterView(int, int)));
			gridLayout->addWidget(tmpParametersWidget,0,0);
			if(horSplitter->count() > TABLEVIEWINDEX)
				tmpParametersWidget->resizeParameterView(horSplitter->sizes().at(TABLEVIEWINDEX),horSplitter->childAt(TABLEVIEWINDEX,0)->size().height());
			//if(bDoParseDependencies)
				tmpParametersWidget->parseDependencies();
			tmpParametersWidget->setAutoDepencyParsing(true);
			scrollArea->takeWidget();
			scrollArea->setWidget(tmpParametersWidget);
			connect(tmpParametersWidget, SIGNAL(editFinished(const QString&, const QString&)), this, SLOT(saveNewData(const QString&, const QString&)),Qt::UniqueConnection);
			return;
		}
		dynamicGraphicWidget->setLayout(gridLayout);
		scrollArea->takeWidget();
		scrollArea->setWidget(dynamicGraphicWidget);		
	}
}

ExperimentStructuresNameSpace::strcExperimentObject ExperimentGraphicEditor::getDefinedExperimentObjectInfo(ExperimentTreeItem *objItem)
{
	ExperimentStructuresNameSpace::strcExperimentObject tmpExperimentObject;
	if(objItem)
	{
		QMap<QString, TreeItemDefinition> mapTreeItemDefinition = objItem->getDefinitions();
		if(mapTreeItemDefinition.contains(ID_TAG))
		{
			int nSearchObjectID = mapTreeItemDefinition.value(ID_TAG).value.toInt();
			QDomNodeList tmpExperimentObjectDomNodeList;
			QStringList strList;
			strList.clear();
			strList.append(ROOT_TAG);
			strList.append(DECLARATIONS_TAG); 
			strList.append(OBJECT_TAG);
			if (pExpTreeModel->getDocumentElements(strList,tmpExperimentObjectDomNodeList) >= 0)
			{
				int nNrOfObjects = tmpExperimentObjectDomNodeList.count();
				if (nNrOfObjects>0)
				{
					QDomNode tmpNode;
					QDomElement tmpElement;
					QString tmpString;
					int nObjectID = -1;
					for(int i=0;i<nNrOfObjects;i++)
					{
						tmpNode = tmpExperimentObjectDomNodeList.at(i);
						if (tmpNode.isElement()) 
						{
							tmpElement = tmpNode.toElement();
							if(!tmpElement.hasAttribute(ID_TAG))
								break;
							tmpString = tmpElement.attribute(ID_TAG,"");//Correct ObjectID?
							if (tmpString.isEmpty() == false)
							{
								nObjectID = tmpString.toInt();
								if(nObjectID == nSearchObjectID)
								{
									tmpExperimentObject.nID = nObjectID;
									tmpElement = tmpNode.firstChildElement(NAME_TAG);
									if(tmpElement.tagName() == NAME_TAG)
										tmpExperimentObject.sName = tmpElement.text();
									tmpElement = tmpNode.firstChildElement(CLASS_TAG);
									if(tmpElement.tagName() == CLASS_TAG)
										tmpExperimentObject.sClass = tmpElement.text();
									break;
								}
							}
						}
					}
				}
			}
		}
	}	
	return tmpExperimentObject;
}

void ExperimentGraphicEditor::fillTableView(const QString &textToFind, const QStringList &filters)
{
	if (pExpTreeModel)
	{
		tblWidgetView->clear();
		tblWidgetView->setRowCount(0);
		tblWidgetView->setColumnCount(4);
		tblWidgetView->setColumnHidden(3, true);
		QList<ExperimentTreeItem*> list = pExpTreeModel->getFilteredItemList(textToFind, filters);
		for (int i = 0; i < list.size(); i++)
		{
			tblWidgetView->insertRow(i);
			tblWidgetView->setItem(i, 0, new QTableWidgetItem(list.at(i)->getName()));
			tblWidgetView->setItem(i, 1, new QTableWidgetItem(list.at(i)->getValue()));
			tblWidgetView->setItem(i, 2, new QTableWidgetItem(list.at(i)->parent()->getName()));
			tblWidgetView->setItem(i, 3, new QTableWidgetItem(list.at(i)->getUID()));
		}
		tblWidgetView->setVisible(true);
	}
}

void ExperimentGraphicEditor::selectTreeItem()
{
	QString selectedUID = tblWidgetView->item(tblWidgetView->currentRow(), 3)->text();
	QModelIndex index = pExpTreeModel->getIndexByUID(selectedUID);
	QModelIndex modelIndex = filterModel->mapFromSource(index);
	if (modelIndex.isValid())
	{
		treeView->selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::ClearAndSelect);
		showInfo(modelIndex);
		//Set the focus over the value editable widget
		for (int i = 0; i < gridLayout->rowCount(); i++)
		{
			QWidgetItem *widgetItem = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,1));
			QWidgetItem *widgetUid = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,3));
			QLabel *label = dynamic_cast<QLabel*>(widgetItem->widget());
			if (widgetUid && label && widgetItem)
			{
				if (label->text() == selectedUID)
				{
					dynamicGraphicWidget->activateWindow();
					widgetItem->widget()->setFocus(Qt::OtherFocusReason);
				}
			}
		}
	}
}

void ExperimentGraphicEditor::showFindDialog()
{
	if (pExpTreeModel)
	{
		findDlg = new FindDialog();
		connect(findDlg, SIGNAL(findSignal(QString,QStringList)), this, SLOT(fillTableView(QString,QStringList)));
		findDlg->show();
	}
}

void ExperimentGraphicEditor::insertNode()
{
	if (pExpTreeModel)
	{
		QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
		QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);
		if (originalIndex.isValid())
		{
			ExperimentTreeItem *item = new ExperimentTreeItem("Node", "");
			ExperimentTreeItem *brother = pExpTreeModel->itemFromIndex(originalIndex);
			if (brother->parent() != NULL)
				brother->parent()->appendRow(item);
		}
	}
}

void ExperimentGraphicEditor::insertSubnode()
{
	if (pExpTreeModel)
	{
		QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
		QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);
		if (originalIndex.isValid())
		{
			ExperimentTreeItem *item = new ExperimentTreeItem("Subnode", "");
			ExperimentTreeItem *parent = pExpTreeModel->itemFromIndex(originalIndex);
			parent->appendRow(item);
		}
	}
}

void ExperimentGraphicEditor::removeNode()
{
	if (pExpTreeModel)
	{
		QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
		QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);
		if (originalIndex.isValid())
			pExpTreeModel->removeRow(originalIndex.row(), originalIndex.parent());
	}
}

void ExperimentGraphicEditor::addDefinition()
{
	if (pExpTreeModel)
	{
		QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
		QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);
		if (originalIndex.isValid())
		{
			ExperimentTreeItem *item = pExpTreeModel->itemFromIndex(originalIndex);
			attWidget = new AttributeWidget(item);
			attWidget->show();
		}
	}
}

void ExperimentGraphicEditor::saveNewData()
{
	QModelIndex sourceIndex = filterModel->mapToSource(treeView->selectionModel()->currentIndex());
	pExpTreeModel->saveNewData(dynamicGraphicWidget, sourceIndex);
}

void ExperimentGraphicEditor::saveNewData(const QString &sName, const QString &sValue)
{
	QModelIndex sourceIndex = filterModel->mapToSource(treeView->selectionModel()->currentIndex());
	pExpTreeModel->saveNewData(sName, sValue, sourceIndex);	
}

void ExperimentGraphicEditor::setViewFilter(const TreeFilterSettings &newViewSettings)
{
	currentViewSettings = newViewSettings;
	if(filterModel)
		filterModel->setTreeFilterSettings(currentViewSettings);
}

bool ExperimentGraphicEditor::setExperimentTreeModel(ExperimentTreeModel *expModel)
{
	loadedExpTreeModel.reset();
	pExpTreeModel = expModel;
	setNewModel();

	//connect(pExpTreeModel, SIGNAL(modelModified()), this, SLOT(setNewModel()));
	//ExperimentTreeItem *rootItem = new ExperimentTreeItem("[Root node]");
	//pExpTreeModel->setItem(0, rootItem);
	//setupFilterModel();

	return true;
}

void ExperimentGraphicEditor::childWidgetDestroyed(QWidget* pWidget)
{
	if(pWidget == expStructVisualizer)
	{
		expStructVisualizer = NULL;
	}	
}