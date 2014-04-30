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
	//QGraphicsItem *item = itemAt(contextMenuEvent->scenePos());
	QList<QGraphicsItem *> gItemList = selectedItems();
	QGraphicsItem *gItem = itemAt(contextMenuEvent->scenePos(),QTransform());//const QTransform & deviceTransform)

	if(gItem == NULL)
		return;

	//ExperimentGraphBlockItem* gBlock = qobject_cast<ExperimentGraphBlockItem*>(gItem);
	int nType = gItem->type();
	if(nType > QGraphicsItem::UserType) //Custom Type
	{		
		if(nType == ExperimentStructureItemType::TypeBlockItem)//Block
		{
			ExperimentGraphBlockItem* gBlock = qgraphicsitem_cast<ExperimentGraphBlockItem*>(gItem);
			if (gBlock)
			{

			}
		}
		else if(nType == ExperimentStructureItemType::TypeAutoConnectionItem)//Auto-Connection
		{
			ExperimentGraphConnectionItem* gAutoConnection = qgraphicsitem_cast<ExperimentGraphConnectionItem*>(gItem);
			if (gAutoConnection)
			{

			}
		}
		else if(nType == ExperimentStructureItemType::TypeLoopConnectionItem)//Loop-Connection
		{
			ExperimentGraphConnectionItem* gLoopConnection = qgraphicsitem_cast<ExperimentGraphConnectionItem*>(gItem);
			if (gLoopConnection)
			{

			}
		}


		QMenu menu;
		QAction *removeAction = menu.addAction("Remove");
		QAction *markAction = menu.addAction("Mark");
		QAction *selectedAction = menu.exec(contextMenuEvent->screenPos());//contextMenuEvent->globalPos()// contextMenuEvent->scenePos() is available
	}
	return;

	//Event::ignore()
}