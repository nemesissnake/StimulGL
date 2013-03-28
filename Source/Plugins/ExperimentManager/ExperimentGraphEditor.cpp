//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QMenu>
#include "ExperimentGraphEditor.h"
#include "ExperimentGraphPort.h"
#include "ExperimentGraphConnection.h"
#include "ExperimentGraphBlock.h"

ExperimentGraphEditor::ExperimentGraphEditor(QObject *parent) : QObject(parent)
{
	conn = NULL;
	gScene = NULL;
	gView = NULL;
	bAllowSelfRecurrentConnection = true;
}

void ExperimentGraphEditor::install(QGraphicsScene *s, QGraphicsView *v)
{
	s->installEventFilter(this);
	gScene = s;
	gView = v;	
}

bool ExperimentGraphEditor::parseExperimentStructure(cExperimentStructure *ExpStruct)
{//Make sure to first call the above install()!
	if(gScene == NULL)
		return false;
	int nExpBlockCount = ExpStruct->getBlockCount();
	double dLeftCanvasMargin = 50.0;
	double dTopCanvasMargin = 50.0;
	double dBlockDistance = 200.0;
	cBlockStructure *tmpBlock;
	int nNextSearchBlockNumber = 0;
	int nNextSearchLoopID = 0;
	QMap<int,QPointF> mBlockPositions;
	for(int j=0;j<2;j++)
	{
		if(j==0)//first draw the blocks
		{
			for (int i=0;i<nExpBlockCount;i++)
			{
				tmpBlock = NULL;
				tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
				if(tmpBlock) 
				{
					nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
					ExperimentGraphBlock *gBlock = new ExperimentGraphBlock(NULL);
					gScene->addItem(gBlock);
					gBlock->setName(tmpBlock->getBlockName());
					gBlock->setID(tmpBlock->getBlockID());
					gBlock->addPort(tmpBlock->getBlockName(), false, ExperimentGraphPort::NamePort);
					gBlock->addPort(QString::number(tmpBlock->getBlockID()) + ": Blocknumber " + QString::number(tmpBlock->getBlockNumber()), false, ExperimentGraphPort::TypePort);
					gBlock->addInputPort("In");
					gBlock->addOutputPort("Out");
					gBlock->setPos(dLeftCanvasMargin + (i*dBlockDistance), dTopCanvasMargin);
					mBlockPositions.insert(gBlock->getID(),gBlock->pos());
					//gBlock = b->clone();
				}
			}
		}
		else if(j==1)//draw the connections
		{
			nNextSearchBlockNumber = 0;
			int nExpBlockLoopCount;
			for (int i=0;i<nExpBlockCount;i++)
			{
				tmpBlock = NULL;
				tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
				if(tmpBlock)
				{
					nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
					nExpBlockLoopCount = tmpBlock->getLoopCount();
					if(nExpBlockLoopCount > 0)
					{
						cLoopStructure *tmpLoop;
						int nTargetBlockID = -1;
						QGraphicsItem *tmpFromGraphItem = NULL;
						QGraphicsItem *tmpToGraphItem = NULL;
						ExperimentGraphBlock *tmpFromBlockGraphItem = NULL;
						ExperimentGraphBlock *tmpToBlockGraphItem = NULL;
						nNextSearchLoopID = 0;
						for (int j=0;j<nExpBlockLoopCount;j++)
						{
							tmpLoop = NULL;
							tmpLoop = tmpBlock->getNextClosestLoopIDByFromID(nNextSearchLoopID);
							if (tmpLoop)
							{
								nNextSearchLoopID = tmpLoop->getLoopID() + 1;
								nTargetBlockID = tmpLoop->getTargetBlockID();
								if ((mBlockPositions.contains(nTargetBlockID)) && (mBlockPositions.contains(tmpBlock->getBlockID())))
								{
									tmpFromGraphItem = itemAt(mBlockPositions.value(tmpBlock->getBlockID()));
									tmpToGraphItem = itemAt(mBlockPositions.value(nTargetBlockID));
									tmpFromBlockGraphItem = (ExperimentGraphBlock*)tmpFromGraphItem;
									tmpToBlockGraphItem = (ExperimentGraphBlock*)tmpToGraphItem;		
									QVector<ExperimentGraphPort*> tmpFromPorts = tmpFromBlockGraphItem->ports();
									QVector<ExperimentGraphPort*> tmpToPorts = tmpToBlockGraphItem->ports();
									createConnection(tmpFromPorts[3],tmpToPorts[2]);
								}
							}
						}
					}
				}
			}
		}
	}
	//QList<QGraphicsItem*> items = scene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));
	//foreach(QGraphicsItem *item, items)
	//	if (item->type() > QGraphicsItem::UserType)
	return true;
}

bool ExperimentGraphEditor::createConnection(QGraphicsItem *from, QGraphicsItem *to)
{
	if (from && from->type() == ExperimentGraphPort::Type)
	{
		if (to && to->type() == ExperimentGraphPort::Type)
		{
			conn = new ExperimentGraphConnection(NULL);
			gScene->addItem(conn);
			conn->setPort1((ExperimentGraphPort*) from);
			conn->setPos1(from->scenePos());
			conn->setPos2(to->scenePos());
			conn->updatePath();

			ExperimentGraphPort *port1 = conn->port1();
			ExperimentGraphPort *port2 = (ExperimentGraphPort*) to;

			if ((port1->block() != port2->block()) || bAllowSelfRecurrentConnection)
			{
				if(port1->isOutput() != port2->isOutput() && !port1->isConnected(port2))
				{
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = NULL;
					return true;
				}
			}
			delete conn;
			conn = 0;
			return false;
		}
		return false;
	}
	return false;					
}

QGraphicsItem* ExperimentGraphEditor::itemAt(const QPointF &pos)
{
	QList<QGraphicsItem*> items = gScene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

bool ExperimentGraphEditor::eventFilter(QObject *o, QEvent *e)
{
	QGraphicsSceneMouseEvent *me = (QGraphicsSceneMouseEvent*) e;

	switch ((int) e->type())
	{
	case QEvent::GraphicsSceneMousePress:
	{
		switch ((int) me->button())
		{
		case Qt::LeftButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == ExperimentGraphPort::Type)
			{
				conn = new ExperimentGraphConnection(0);
				gScene->addItem(conn);
				conn->setPort1((ExperimentGraphPort*) item);
				conn->setPos1(item->scenePos());
				conn->setPos2(me->scenePos());
				conn->updatePath();

				return true;
			} 
			else if (item && item->type() == ExperimentGraphBlock::Type)
			{
				/* if (selBlock)
					selBlock->setSelected(); */
				// selBlock = (ExperimentGraphBlock*) item;
			}
			break;
		}
		case Qt::RightButton:
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && (item->type() == ExperimentGraphConnection::Type || item->type() == ExperimentGraphBlock::Type))
			{
				QMenu myMenu;
				if(item->type() == ExperimentGraphConnection::Type)
					myMenu.addAction("Delete Connection");
				else if(item->type() == ExperimentGraphBlock::Type)
					myMenu.addAction("Delete Block");
				QAction* selectedItem = myMenu.exec(me->screenPos());
				if (selectedItem)
				{
					delete item;
					// something was chosen, do stuff
				}
				else
				{
					//bool b = true;
					// nothing was chosen
				}
			}								
			//if (item && (item->type() == ExperimentGraphConnection::Type || item->type() == ExperimentGraphBlock::Type))
			// if (selBlock == (ExperimentGraphBlock*) item)
			// selBlock = 0;
			break;
		}
		}
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		if (conn)
		{
			conn->setPos2(me->scenePos());
			conn->updatePath();
			return true;
		}
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		if (conn && me->button() == Qt::LeftButton)
		{
			QGraphicsItem *item = itemAt(me->scenePos());
			if (item && item->type() == ExperimentGraphPort::Type)
			{
				ExperimentGraphPort *port1 = conn->port1();
				ExperimentGraphPort *port2 = (ExperimentGraphPort*) item;

				if (((port1->block() != port2->block()) || bAllowSelfRecurrentConnection) && port1->isOutput() != port2->isOutput() && !port1->isConnected(port2))
				{
					conn->setPos2(port2->scenePos());
					conn->setPort2(port2);
					conn->updatePath();
					conn = 0;
					return true;
				}
			}

			delete conn;
			conn = 0;
			return true;
		}
		break;
	}
	}
	return QObject::eventFilter(o, e);
}

void ExperimentGraphEditor::save(QDataStream &ds)
{
	foreach(QGraphicsItem *item, gScene->items())
		if (item->type() == ExperimentGraphBlock::Type)
		{
			ds << item->type();
			((ExperimentGraphBlock*) item)->save(ds);
		}

	foreach(QGraphicsItem *item, gScene->items())
		if (item->type() == ExperimentGraphConnection::Type)
		{
			ds << item->type();
			((ExperimentGraphConnection*) item)->save(ds);
		}
}

void ExperimentGraphEditor::load(QDataStream &ds)
{
	gScene->clear();
	QMap<quint64, ExperimentGraphPort*> portMap;
	while (!ds.atEnd())
	{
		int type;
		ds >> type;
		if (type == ExperimentGraphBlock::Type)
		{
			ExperimentGraphBlock *block = new ExperimentGraphBlock(0);
			gScene->addItem(block);
			block->load(ds, portMap);
		} else if (type == ExperimentGraphConnection::Type)
		{
			ExperimentGraphConnection *conn = new ExperimentGraphConnection(0);
			gScene->addItem(conn);
			conn->load(ds, portMap);
		}
	}
}
