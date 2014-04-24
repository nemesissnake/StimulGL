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

#include "ExperimentStructureVisualizer.h"
#include "ui_ExperimentStructureVisualizer.h"
#include "QGraphicsViewEc.h"
#include "../ExperimentStructures.h"
#include <QMessageBox>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QVBoxLayout>
#include <QResizeEvent>
#include <QGraphicsTextItem>

ExperimentStructureVisualizer::ExperimentStructureVisualizer(QWidget *parent) : QWidget(parent)
{
	action_Quit = NULL;
	action_Test = NULL;
	toolBar = NULL;
	menuFile = NULL;
	menuEdit = NULL;
	buttonFile = NULL;
	buttonEdit = NULL;
	graphViewLayout = NULL;
	gScene = NULL;
	parsedExpStruct = NULL;
	nWidgetMargin = 45;
	dGraphViewScale = 0.9;//fill till 90%
	bDrawVertical = true;

	ui = new Ui::ExperimentStructureVisualizer();
	ui->setupUi(this);

	resetExpScene();
	configureActions(true);
	setupMenuAndActions();
	createScene();
	setupLayout();
}

ExperimentStructureVisualizer::~ExperimentStructureVisualizer()
{
	delete ui;
    configureActions(false);
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
	if(gScene)
		delete gScene;
	if(graphViewLayout)
		delete graphViewLayout;
	resetExpScene();
	emit destroyed(this);
}

void ExperimentStructureVisualizer::resetExpScene()
{
	parsedExpStruct = NULL;
	expSceneItems.sExperimentName = "<undefined>";
	expSceneItems.lLoops.clear();
	expSceneItems.lBlocks.clear();
	expSceneItems.lAutoConns.clear();

	expLoopDrawing.hBlockIDMasterSideCount.clear();
	expLoopDrawing.hBlockIDSlaveSideCount.clear();
	expLoopDrawing.hBlockIDTargetLoopBlockIDs.clear();
	expLoopDrawing.lMasterSideDrawOrderedLoops.clear();
	expLoopDrawing.lSlaveSideDrawOrderedLoops.clear();
}

void ExperimentStructureVisualizer::configureActions(bool bCreate)
{
	if(bCreate)
	{
		action_Quit = new QAction("Quit", this);
		action_Test = new QAction("Test", this);
	}
	else
	{		
		if(action_Quit)
		{
			delete action_Quit;
			action_Quit = NULL;
		}	
		if(action_Test)
		{
			delete action_Test;
			action_Test = NULL;
		}
	}
}

void ExperimentStructureVisualizer::setupMenuAndActions()
{
	toolBar = new QToolBar(this);

	//File menu///
	buttonFile=new QToolButton(this);
	buttonFile->setText("File ");
	buttonFile->setPopupMode(QToolButton::InstantPopup);
	menuFile = new QMenu();//FileButton
	menuFile->addAction(action_Quit);
	buttonFile->setMenu(menuFile);
	toolBar->addWidget(buttonFile);  
	////Edit menu///
	buttonEdit=new QToolButton(this);
	buttonEdit->setText("Edit ");
	buttonEdit->setPopupMode(QToolButton::InstantPopup);
	menuEdit = new QMenu();//EditButton
	menuEdit->addAction(action_Test);
	buttonEdit->setMenu(menuEdit);
	toolBar->addWidget(buttonEdit);

	connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
	connect(action_Test, SIGNAL(triggered()), this, SLOT(Test()));
	
}

void ExperimentStructureVisualizer::Test()
{
	//resizeStructureView(ui->graphicsView->size().width(),ui->graphicsView->size().height());
	//int test = ui->graphicsView->maximumWidth();
	//drawGraph();	
}

void ExperimentStructureVisualizer::createScene()
{
	gScene = new ExperimentStructureScene(this);
	//connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
	//connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
	ui->graphicsView->setScene(gScene);
	//ui->graphicsView->viewport()->setCursor(Qt::ArrowCursor);
}

void ExperimentStructureVisualizer::setupLayout()
{
	ui->mainLayout->setMenuBar(toolBar);
	if(graphViewLayout == NULL)
	{
		graphViewLayout = new QVBoxLayout();
		graphViewLayout->addWidget(ui->graphicsView);
	}
	ui->mainLayout->addLayout(graphViewLayout);
	ui->mainLayout->setContentsMargins(0,0,0,0);
	ui->mainLayout->setSpacing(0);
	setLayout(ui->mainLayout);
}

void ExperimentStructureVisualizer::resizeStructureView(const int &nWidth, const int &nHeight)
{
	if(ui->graphicsView)
	{
		ui->graphicsView->setFixedSize(nWidth-nWidgetMargin,nHeight-nWidgetMargin);
		//ui->graphicsView->setFixedSize(500,500);
	}
}

bool ExperimentStructureVisualizer::drawGraph(const QString &sDotContent)
{
	if(gScene == NULL)
		createScene();
	else
		gScene->clear();	

	int nCurrentBlockNumber = 0;
	QRectF nSingleBlockBoundingBox;
	int nAutoConnDistance = (int)EXPGRAPH_AUTOCONN_DISTANCE;
	int nBlockDistance;
	
	//START EXPERIMENT//
	expSceneItems.gStartGraphBlockItem = new ExperimentGraphBlockItem();
	nSingleBlockBoundingBox = expSceneItems.gStartGraphBlockItem->boundingRect();
	nBlockDistance = nSingleBlockBoundingBox.height() + nAutoConnDistance;
	expSceneItems.gStartGraphBlockItem->setType(GRAPHBLOCK_TYPE_START);
	expSceneItems.gStartGraphBlockItem->setPos(0,0);
	gScene->addItem(expSceneItems.gStartGraphBlockItem);	
	nCurrentBlockNumber++;

	//BLOCKS//
	if(expSceneItems.lBlocks.isEmpty() == false)
	{
		QString sTmpTooltip;
		for (int i=0;i<expSceneItems.lBlocks.count();i++)
		{
			sTmpTooltip = "";			
			//QRectF rectTemp(0,nCurrentBlockNumber*nBlockDistance,nBlockWidth,nBlockHeight);
			expSceneItems.lBlocks[i].gGraphBlockItem = new ExperimentGraphBlockItem();
			expSceneItems.lBlocks[i].gGraphBlockItem->setType(GRAPHBLOCK_TYPE_BLOCK);
			expSceneItems.lBlocks[i].gGraphBlockItem->setCaption(expSceneItems.lBlocks[i].sName);
			expSceneItems.lBlocks[i].gGraphBlockItem->setPos(0,nCurrentBlockNumber*nBlockDistance);
			gScene->addItem(expSceneItems.lBlocks[i].gGraphBlockItem);
			//expSceneItems.lBlocks[i].gTextItem = gScene->addText(expSceneItems.lBlocks[i].sName);
			cBlockStructure *tmpBlockStruct = parsedExpStruct->getBlockPointerByID(expSceneItems.lBlocks[i].nId);
			if (tmpBlockStruct)
			{
				sTmpTooltip = "Trials: " + QString::number(tmpBlockStruct->getNumberOfTrials());
				sTmpTooltip = sTmpTooltip + "\nInt. Triggers: " + QString::number(tmpBlockStruct->getNumberOfInternalTriggers());
				sTmpTooltip = sTmpTooltip + "\nExt. Triggers: " + QString::number(tmpBlockStruct->getNumberOfExternalTriggers());				
			}
			expSceneItems.lBlocks[i].gGraphBlockItem->setToolTip(sTmpTooltip);
			nCurrentBlockNumber++;			
		}
	}
	else
	{
		return false;//no blocks defined...
	}

	//END EXPERIMENT//
	expSceneItems.gEndGraphBlockItem = new ExperimentGraphBlockItem();
	expSceneItems.gEndGraphBlockItem->setType(GRAPHBLOCK_TYPE_END);
	expSceneItems.gEndGraphBlockItem->setPos(0,(nCurrentBlockNumber*nBlockDistance));
	gScene->addItem(expSceneItems.gEndGraphBlockItem);
	nCurrentBlockNumber++;

	//AUTO-CONNECTIONS//
	ExperimentGraphBlockItem *tmpSourceBlock = NULL;
	ExperimentGraphBlockItem *tmpTargetBlock = NULL;
	for (int i=0;i<expSceneItems.lAutoConns.count();i++)
	{	
		tmpSourceBlock = NULL;
		tmpTargetBlock = NULL;
		if(expSceneItems.lAutoConns[i].nSourceBlockId >= 0)
		{
			if(expSceneItems.lAutoConns[i].nTargetBlockId >= 0)//Block to Block connection
			{
				tmpSourceBlock = getGraphBlockItemPointer(expSceneItems.lAutoConns[i].nSourceBlockId);
				if(tmpSourceBlock)
				{
					tmpTargetBlock = getGraphBlockItemPointer(expSceneItems.lAutoConns[i].nTargetBlockId);
					if(tmpTargetBlock)
					{
						expSceneItems.lAutoConns[i].gGraphConnectionItem = new ExperimentGraphConnectionItem();
						expSceneItems.lAutoConns[i].gGraphConnectionItem->setEndPoint(QPoint(0,nAutoConnDistance));
						int tmpY = tmpSourceBlock->pos().y() + tmpSourceBlock->boundingRect().bottom();
						expSceneItems.lAutoConns[i].gGraphConnectionItem->setPos(0, tmpY);
						gScene->addItem(expSceneItems.lAutoConns[i].gGraphConnectionItem);	
						continue;
					}
				}
			}
			else //End of experiment, no target
			{
				tmpSourceBlock = getGraphBlockItemPointer(expSceneItems.lAutoConns[i].nSourceBlockId);
				if(tmpSourceBlock && expSceneItems.gEndGraphBlockItem)
				{
					expSceneItems.lAutoConns[i].gGraphConnectionItem = new ExperimentGraphConnectionItem();
					expSceneItems.lAutoConns[i].gGraphConnectionItem->setEndPoint(QPoint(0,nAutoConnDistance));
					int tmpY = tmpSourceBlock->pos().y() + tmpSourceBlock->boundingRect().bottom();
					//int tmpY = expSceneItems.gEndGraphBlockItem->pos().y() + expSceneItems.gEndGraphBlockItem->boundingRect().top();
					expSceneItems.lAutoConns[i].gGraphConnectionItem->setPos(0, tmpY);
					gScene->addItem(expSceneItems.lAutoConns[i].gGraphConnectionItem);
					continue;
				}
			}
		}
		else
		{
			if(expSceneItems.lAutoConns[i].nTargetBlockId >= 0)//Start of Experiment, no source
			{
				tmpTargetBlock = getGraphBlockItemPointer(expSceneItems.lAutoConns[i].nTargetBlockId);
				if(tmpTargetBlock && expSceneItems.gStartGraphBlockItem)
				{
					expSceneItems.lAutoConns[i].gGraphConnectionItem = new ExperimentGraphConnectionItem();
					expSceneItems.lAutoConns[i].gGraphConnectionItem->setEndPoint(QPoint(0,nAutoConnDistance));
					int tmpY = expSceneItems.gStartGraphBlockItem->pos().y() + expSceneItems.gStartGraphBlockItem->boundingRect().bottom();
					expSceneItems.lAutoConns[i].gGraphConnectionItem->setPos(0, tmpY);
					gScene->addItem(expSceneItems.lAutoConns[i].gGraphConnectionItem);
					continue;
				}
			}
		}
		//Could not handle the connection, so delete it
		expSceneItems.lAutoConns.removeAt(i);
		i--;
	}

	//LOOPS//
	if(expSceneItems.lLoops.isEmpty() == false)
	{
		for (int i=0;i<expSceneItems.lLoops.count();i++)
		{
			tmpSourceBlock = getGraphBlockItemPointer(expSceneItems.lLoops[i].nSourceBlockId);
			tmpTargetBlock = getGraphBlockItemPointer(expSceneItems.lLoops[i].nTargetBlockId);
			if((tmpSourceBlock) && (tmpTargetBlock))
			{
				insertLoopInGraphDrawingStruct(expSceneItems.lLoops[i].nSourceBlockId,expSceneItems.lLoops[i].nTargetBlockId);
			}
		}

		expConnItemStrc *tmpexpConnItemStrc;
		QList<expConnItemStrc> *pLexpConnItemStrc;
		QPen tmpPen;
		tmpPen.setColor("#00ff00");
		tmpPen.setWidth(5);
		int nLocation = 1;
		int tmpYSource;
		int tmpYTarget;
		int tmpXOffset;
		for (int j=0;j<2;j++)
		{
			if(j==0)
			{
				pLexpConnItemStrc = &expLoopDrawing.lMasterSideDrawOrderedLoops;
				nLocation = 1;
			}
			else if(j==1)
			{
				pLexpConnItemStrc = &expLoopDrawing.lSlaveSideDrawOrderedLoops;
				nLocation = -1;
			}

			for(int i=0;i<pLexpConnItemStrc->count();i++)
			{
				tmpexpConnItemStrc = &(*pLexpConnItemStrc)[i];
				tmpSourceBlock = getGraphBlockItemPointer(tmpexpConnItemStrc->nSourceBlockId);
				tmpTargetBlock = getGraphBlockItemPointer(tmpexpConnItemStrc->nTargetBlockId);
				expSceneItems.lAutoConns[i].gGraphConnectionItem = new ExperimentGraphConnectionItem();
				tmpYSource = tmpSourceBlock->pos().y() - tmpSourceBlock->boundingRect().top() + EXPGRAPHCONNITEM_LOOP_START_HEIGHT_DISTANCE;
				tmpXOffset = ((nLocation * EXPGRAPHCONNITEM_LOOP_START_WIDTH_DISTANCE) + (nLocation * i*EXPGRAPHCONNITEM_LOOP_DISTANCE));
				expSceneItems.lAutoConns[i].gGraphConnectionItem->setPos(tmpXOffset, tmpYSource);
				tmpYTarget = tmpYSource - (tmpTargetBlock->pos().y() + tmpTargetBlock->boundingRect().top() - EXPGRAPHCONNITEM_LOOP_START_HEIGHT_DISTANCE);
				expSceneItems.lAutoConns[i].gGraphConnectionItem->setEndPoint(QPoint(0,-tmpYTarget),tmpXOffset);
				gScene->addItem(expSceneItems.lAutoConns[i].gGraphConnectionItem);	
				
			}
		}
	}	
	
	QRectF currentSceneRect = gScene->itemsBoundingRect();
	currentSceneRect.setWidth(currentSceneRect.width()+nSingleBlockBoundingBox.width());
	currentSceneRect.setHeight(currentSceneRect.height()+nSingleBlockBoundingBox.height());
	ui->graphicsView->setSceneRect(currentSceneRect);//QRect(0, 0, nWidth+nodeWidth, nHeight+nodeHeight));//currentSceneRect);
	qreal dScaleFactor;
	if(bDrawVertical)
		dScaleFactor = ((ui->graphicsView->size().height()-(nWidgetMargin))*dGraphViewScale)/currentSceneRect.height();
	else
		dScaleFactor = ((ui->graphicsView->size().width()-(nWidgetMargin))*dGraphViewScale)/currentSceneRect.width();
	ui->graphicsView->resetTransform();
	ui->graphicsView->scale(dScaleFactor, dScaleFactor);
	ui->graphicsView->centerOn(currentSceneRect.center());//QRect(0, 0, nWidth+nodeWidth, nHeight+nodeHeight).center());//currentSceneRect.center());
	//ui->graphicsView->centerOn(gScene->itemsBoundingRect().center());

	return true;
}

bool ExperimentStructureVisualizer::insertLoopInGraphDrawingStruct(const int &nSourceBlockID, const int &nTargetBlockID)
{
	expConnItemStrc *tmpExpConnItem=NULL;
	QList<expConnItemStrc> *lDrawOrderedLoops=NULL;
	expConnItemStrc appendExpConnItem;
	bool bFitsInsideResult = false;
	bool bFitsOverResult = false;
	bool bFitIndexDetermined = false;
	bool bFitsAsideResult = false;
	bool bIsCrossed = false;
	struct strcLastFit
	{
		int nLastFitIndex;
		int nMin;
		int nMax;
		strcLastFit()
		{
			nLastFitIndex = -1;
			nMin = -1;
			nMax = -1;
		}
	};
	
	int nCurrentIndex;
	int nTestMin;
	int nTestMax;
	int nParsedMin;
	int nParsedMax;
	strcLastFit sLastFit;

	for (int j=0;j<2;j++)
	{
		if(bFitIndexDetermined)
			break;
		
		if(j==0)
			lDrawOrderedLoops = &expLoopDrawing.lMasterSideDrawOrderedLoops;
		else if(j==1)
			lDrawOrderedLoops = &expLoopDrawing.lSlaveSideDrawOrderedLoops;

		if(lDrawOrderedLoops->isEmpty())
		{
			appendExpConnItem.nSourceBlockId = nSourceBlockID;
			appendExpConnItem.nTargetBlockId = nTargetBlockID;
			lDrawOrderedLoops->append(appendExpConnItem);
			return true;
		}
		else
		{
			sLastFit.nLastFitIndex = -1;
			sLastFit.nMin = -1;
			sLastFit.nMax = -1;
			bFitsInsideResult = false;
			bFitsOverResult = false;
			bFitsAsideResult = false;
			bIsCrossed = false;
			nTestMin = qMin(nSourceBlockID,nTargetBlockID);
			nTestMax = qMax(nSourceBlockID,nTargetBlockID);
			for(int i=0;i<lDrawOrderedLoops->count();i++)
			{
				nCurrentIndex = lDrawOrderedLoops->count()-(i+1);
				tmpExpConnItem = &((*lDrawOrderedLoops)[nCurrentIndex]);
				nParsedMin = qMin(tmpExpConnItem->nSourceBlockId, tmpExpConnItem->nTargetBlockId);
				nParsedMax = qMax(tmpExpConnItem->nSourceBlockId, tmpExpConnItem->nTargetBlockId);

				//Next: Does the Loop fit inside?
				if((nTestMin>=nParsedMin) && (nTestMin<=nParsedMax) && (nTestMax>=nParsedMin) && (nTestMax<=nParsedMax))
				{
					bFitsInsideResult = true;
					sLastFit.nLastFitIndex = nCurrentIndex;
					sLastFit.nMin = nParsedMin;
					sLastFit.nMax = nParsedMax;
					continue;//Let's see how deep we can go...
				}
				else
				{
					if(bFitsInsideResult)//Was it already previously fitted inside?
					{
						//Next: Does the Loop not cross?
						if(!(((nTestMin<=nParsedMin) && (nTestMax<nParsedMax)) || ((nTestMin>nParsedMin) && (nTestMax>=nParsedMax))))
							bFitIndexDetermined = true;
						else
							bIsCrossed = true;
						break;
					}
					else
					{
						//Next: Does the Loop fit completely over?
						if((nTestMin<=nParsedMin) && (nTestMax>=nParsedMax))
						{
							//Did it not yet fit completely over?
							if(bFitsOverResult == false)
							{
								sLastFit.nLastFitIndex = nCurrentIndex+1;
								sLastFit.nMin = nParsedMin;
								sLastFit.nMax = nParsedMax;
								bFitsOverResult = true;
							}
							continue;//Let's check the remaining...
						}
						//Next: Does the Loop occur aside parallel?	
						if(((nTestMin<=nParsedMin) && (nTestMax<=nParsedMin)) || ((nTestMin>=nParsedMax) && (nTestMax>=nParsedMax)))
						{
							sLastFit.nLastFitIndex = nCurrentIndex+1;
							sLastFit.nMin = nParsedMin;
							sLastFit.nMax = nParsedMax;
							bFitsAsideResult = true;
							continue;//Let's check the remaining...
						}
						break;
					}
				}				
			}
		}
		if((bIsCrossed == false) && (bFitsInsideResult || bFitsOverResult || bFitsAsideResult))
			bFitIndexDetermined = true;
	}
	if(bFitIndexDetermined)
	{
		appendExpConnItem.nSourceBlockId = nSourceBlockID;
		appendExpConnItem.nTargetBlockId = nTargetBlockID;
		lDrawOrderedLoops->insert(sLastFit.nLastFitIndex,appendExpConnItem);
		return true;
	}
	appendExpConnItem.nSourceBlockId = nSourceBlockID;
	appendExpConnItem.nTargetBlockId = nTargetBlockID;
	lDrawOrderedLoops->insert(0,appendExpConnItem);
	return false;
}

//void ExperimentStructureVisualizer::nodeContextMenu(QGVNode *node)
//{
//    //Context menu example
//    QMenu menu(node->label());
//
//    menu.addSeparator();
//    menu.addAction(tr("Informations"));
//    menu.addAction(tr("Options"));
//
//    QAction *action = menu.exec(QCursor::pos());
//    if(action == 0)
//        return;
//}

//void ExperimentStructureVisualizer::nodeDoubleClick(QGVNode *node)
//{
//	//node->moveBy(10,10);
//	//_scene->setNodeAttribute("pos","200,200");
//	//QGVNode *tmpNode = _scene->getNode("SERVER1");
//	//if(tmpNode)
//	//{
//	//	tmpNode->setAttribute("fillcolor","purple");
//	//	tmpNode->updateLayout();
//	//}
//
//	//_scene->setNodeAttribute("fillcolor","black");
//
//    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
//	//_scene->applyLayout();
//	//_scene->saveLayout("E:\\Projects\\qgv\\debug\\debug.dot",0);
//	//_scene->saveLayout("E:\\Projects\\qgv\\debug\\debug.png",1);
//}

bool ExperimentStructureVisualizer::parseExperimentStructure(cExperimentStructure *ExpStruct)
{//Make sure to first call the above install()!
	resetExpScene();

	int nExpBlockCount = ExpStruct->getBlockCount();
	cBlockStructure *tmpBlock = NULL;
	cBlockStructure *lastBlock = NULL;
	int nNextSearchBlockNumber = 0;
	int nNextSearchLoopID = 0;
	expBlockItemStrc tmpBlockItem;
	expLoopItemStrc tmpLoopItem;

	expConnItemStrc tmpAutoConnItem;

	expSceneItems.sExperimentName = ExpStruct->getExperimentName();
	for (int i=0;i<nExpBlockCount;i++)
	{
		//tmpAutoConnItem.gvEdge = NULL;
		tmpAutoConnItem.nSourceBlockId = -1;
		tmpAutoConnItem.nTargetBlockId = -1;

		lastBlock = tmpBlock;
		tmpBlock = NULL;
		tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
		if(tmpBlock) 
		{
			if(lastBlock)
				tmpAutoConnItem.nSourceBlockId = lastBlock->getBlockID();
			tmpAutoConnItem.nTargetBlockId = tmpBlock->getBlockID();

			nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
			tmpBlockItem.nId = tmpBlock->getBlockID();
			tmpBlockItem.nNumber = tmpBlock->getBlockNumber();
			tmpBlockItem.sName = tmpBlock->getBlockName();
			//tmpBlockItem.gvNode = NULL;
			expSceneItems.lBlocks.append(tmpBlockItem);
		}
		expSceneItems.lAutoConns.append(tmpAutoConnItem);
		if(i==(nExpBlockCount-1))
		{
			tmpAutoConnItem.nSourceBlockId = tmpBlock->getBlockID();
			tmpAutoConnItem.nTargetBlockId = -1;
			expSceneItems.lAutoConns.append(tmpAutoConnItem);
		}
	}
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
				nNextSearchLoopID = 0;
				for (int j=0;j<nExpBlockLoopCount;j++)
				{
					tmpLoop = NULL;
					tmpLoop = tmpBlock->getNextClosestLoopIDByFromID(nNextSearchLoopID);
					if (tmpLoop)
					{
						nNextSearchLoopID = tmpLoop->getLoopID() + 1;
						nTargetBlockID = tmpLoop->getTargetBlockID();
						tmpLoopItem.nId = tmpLoop->getLoopID();
						tmpLoopItem.nNumber = tmpLoop->getLoopNumber();
						tmpLoopItem.sName = tmpLoop->getLoopName();
						tmpLoopItem.nNumberOfLoops = tmpLoop->getNumberOfLoops();
						tmpLoopItem.nSourceBlockId = tmpBlock->getBlockID();
						tmpLoopItem.nTargetBlockId = nTargetBlockID;
						//tmpLoopItem.gvEdge = NULL;						
						expSceneItems.lLoops.append(tmpLoopItem);
					}
				}
			}
		}
	}
	parsedExpStruct = ExpStruct;
	return drawGraph();
}

//ogdf::node *ExperimentStructureVisualizer::getGraphNodePointer(const int &nBlockID)
//{
//	for(int i=0;i<expSceneItems.lBlocks.count();i++)
//	{
//		if(expSceneItems.lBlocks[i].nId == nBlockID)
//		{
//			return expSceneItems.lBlocks[i].gvNode;
//		}
//	}
//	return NULL;
//}

ExperimentGraphBlockItem *ExperimentStructureVisualizer::getGraphBlockItemPointer(const int &nBlockID)
{
	for(int i=0;i<expSceneItems.lBlocks.count();i++)
	{
		if(expSceneItems.lBlocks[i].nId == nBlockID)
		{
			return expSceneItems.lBlocks[i].gGraphBlockItem;
		}
	}
	return NULL;
}