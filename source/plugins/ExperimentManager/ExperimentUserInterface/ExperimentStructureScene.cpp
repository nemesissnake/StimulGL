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

#include "ExperimentStructureScene.h"
#include <QEvent>
#include <QMenu>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsView>
#include <QInputDialog>
#include "ExperimentGraphBlockItem.h"
#include "ExperimentGraphConnectionItem.h"

ExperimentStructureScene::ExperimentStructureScene(QObject *parent) : QGraphicsScene(parent)
{
}

bool ExperimentStructureScene::event(QEvent *event)
{
	bool bResult = false;
	//if(event->type() == QEvent::GraphicsSceneContextMenu) 
	//{
	//	//QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
	//	//qDebug() << "Ate key press" << keyEvent->key();
	//	return true;
	//}
	//else 
	if((event->type() == QEvent::Enter) || (event->type() == QEvent::GraphicsSceneMousePress) || (event->type() == QEvent::GraphicsSceneMouseRelease))
	{
		bResult = QGraphicsScene::event(event);
		foreach(QGraphicsView *pView,views())
			pView->viewport()->setCursor(Qt::ArrowCursor);
		return bResult;
	}
	
	//QEvent::GraphicsSceneContextMenu
	return QGraphicsScene::event(event);
}

void ExperimentStructureScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent)
{
	//int nType = -1;
	QMenu mContexMenu;
	QMenu* mViewMenu = mContexMenu.addMenu("View");
	QMenu* mBlocksMenu = mContexMenu.addMenu("Blocks");
	QMenu* mLoopsMenu = mContexMenu.addMenu("Loops");
	QAction *selectedItemAction = NULL;
	QAction *toggleViewAction;
	QAction *addBlockAction;
	QAction *addMultipleBlocksAction;
	QAction *removeBlockAction;
	QAction *removeBlocksAction;

	QList<QGraphicsItem *> gItemList = selectedItems();
	//QGraphicsItem *gItem = itemAt(contextMenuEvent->scenePos(),QTransform());//item at mouse
	QList<ExperimentGraphBlockItem*> gSelectedBlockItems;
	QList<ExperimentGraphConnectionItem*> gSelectedAutoConnectionItems;
	QList<ExperimentGraphConnectionItem*> gSelectedLoopConnectionItems;

	//if(gItem)
	//if(gItemList.isEmpty() == false)
	//{
		foreach(QGraphicsItem *graphItem,gItemList)
		{
			if(graphItem->type() == ExperimentStructureItemType::TypeBlockItem)
				gSelectedBlockItems.append(qgraphicsitem_cast<ExperimentGraphBlockItem*>(graphItem));
			else if(graphItem->type() == ExperimentStructureItemType::TypeAutoConnectionItem)
				gSelectedAutoConnectionItems.append(qgraphicsitem_cast<ExperimentGraphConnectionItem*>(graphItem));
			else if(graphItem->type() == ExperimentStructureItemType::TypeLoopConnectionItem)
				gSelectedLoopConnectionItems.append(qgraphicsitem_cast<ExperimentGraphConnectionItem*>(graphItem));
		}

		if(gSelectedBlockItems.isEmpty() == false)//Blocks selected?
		{
			addBlockAction = mBlocksMenu->addAction("Add New");
			addMultipleBlocksAction = mBlocksMenu->addAction("Add New(multiple)");
			//if(bUsedColumnIndexesInSelectionRanges.count() == 1)
				removeBlockAction = mBlocksMenu->addAction("Remove Selected");
			//else if(bUsedColumnIndexesInSelectionRanges.count() > 1)
				removeBlocksAction = mBlocksMenu->addAction("Remove Multiple Selected");
		}
		else
		{
			mBlocksMenu->setEnabled(false);
		}
		
		if(gSelectedAutoConnectionItems.isEmpty() == false)//Auto-Connections selected?
		{

		}
		else
		{
			
		}
		
		if(gSelectedLoopConnectionItems.isEmpty() == false)//Loop-Connections selected?
		{
			
		}		
		else
		{
			mLoopsMenu->setEnabled(false);
		}
	//}
	toggleViewAction = mViewMenu->addAction("Toggle Orientation");

	selectedItemAction = mContexMenu.exec(contextMenuEvent->screenPos());//contextMenuEvent->globalPos()// contextMenuEvent->scenePos() is available
	if (selectedItemAction)
	{
		if((selectedItemAction == addBlockAction)||(selectedItemAction == addMultipleBlocksAction))
		{
			//if(pExpTreeModel)
			//{
				int nBlocksToAdd;
				if(selectedItemAction == addMultipleBlocksAction)
				{
					bool bDialogResult;
					nBlocksToAdd = QInputDialog::getInt(NULL, tr("How many blocks?"),	tr("Blocks to add:"), 0, 0, 1000, 1, &bDialogResult);
					if (bDialogResult == false)
						nBlocksToAdd = 0;
				}
				else if(selectedItemAction == addBlockAction)
				{
					nBlocksToAdd = 1;
				}
				bool bResult = false;
				if(nBlocksToAdd>0)
				{
					//bResult = pExpTreeModel->addExperimentBlocks(nBlocksToAdd);
				}			
			//}
		}
		else if((selectedItemAction == removeBlockAction) || (selectedItemAction == removeBlocksAction))
		{
			bool bResult = false;
			//if(pExpTreeModel)
			//	bResult = pExpTreeModel->removeExperimentBlocks(lstUsedBlockIDsInSelectionRanges);
		}
		else if(selectedItemAction == toggleViewAction)
		{
			//bVerticalViewEnabled = !bVerticalViewEnabled;
			//reparseModel();
		}
	}
}