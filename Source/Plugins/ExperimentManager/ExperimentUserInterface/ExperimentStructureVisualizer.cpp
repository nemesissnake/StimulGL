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
	gAttr = NULL;
	parsedExpStruct = NULL;
	nWidgetMargin = 45;
	dGraphViewScale = 0.9;//fill till 90%
	bDrawVertical = true;

	ui = new Ui::ExperimentStructureVisualizer();
	ui->setupUi(this);

	resetExpSceneItemsCollection(expSceneItems);
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
	if(gAttr)
		delete gAttr;
	resetExpSceneItemsCollection(expSceneItems);
	emit destroyed(this);
}

void ExperimentStructureVisualizer::resetExpSceneItemsCollection(expSceneItemStrc &tmpExpSceneItems)
{
	parsedExpStruct = NULL;
	tmpExpSceneItems.sExperimentName = "<undefined>";
	//tmpExpSceneItems.gvStartExperimentNode = NULL;
	//tmpExpSceneItems.gvEndExperimentNode = NULL;	
	if(tmpExpSceneItems.lLoops.isEmpty() == false)
		tmpExpSceneItems.lLoops.clear();
	if(tmpExpSceneItems.lBlocks.isEmpty() == false)
		tmpExpSceneItems.lBlocks.clear();
	if(tmpExpSceneItems.lAutoConns.isEmpty() == false)
		tmpExpSceneItems.lAutoConns.clear();
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
	gScene = new QGraphicsScene(this);
	//connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
	//connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
	ui->graphicsView->setScene(gScene);
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
	//if(ui->graphicsView)
	//{
		//ui->graphicsView->setFixedSize(nWidth-nWidgetMargin,nHeight-nWidgetMargin);
		//ui->graphicsView->setFixedSize(500,500);
	//}
}

bool ExperimentStructureVisualizer::drawGraph(const QString &sDotContent)
{
	if(gScene == NULL)
		createScene();
	else
		gScene->clear();	
	
	//Configure graph attributes
	gAttr = new GraphAttributes(graph, GraphAttributes::nodeGraphics | GraphAttributes::edgeGraphics );	

	/*
	gScene->setGraphAttribute("label", "Experiment Structure");
	gScene->setGraphAttribute("splines", "spline");//"polyline");//"spline");//"line");//"ortho");
	if(bDrawVertical)
		gScene->setGraphAttribute("rankdir", "TB");
	else
		gScene->setGraphAttribute("rankdir", "LR");
	//gScene->setGraphAttribute("concentrate", "true"); //Error !
	gScene->setGraphAttribute("nodesep", "0.2");

	gScene->setNodeAttribute("shape", "box");
	//gScene->setNodeAttribute("orientation", "0");
	gScene->setNodeAttribute("style", "filled");
	gScene->setNodeAttribute("fillcolor", "lightgrey");
	gScene->setNodeAttribute("height", "1.2");
	gScene->setNodeAttribute("width", "5.0");
	gScene->setNodeAttribute("fixedsize", "true");

	gScene->setNodeAttribute("labelfontname", "Courier");//fixed, do not change here!
	gScene->setNodeAttribute("labelfontsize", "15");
	gScene->setNodeAttribute("labelfontcolor", "blue");

	gScene->setEdgeAttribute("minlen", "3");
	//gScene->setEdgeAttribute("dir", "both");
	gScene->setEdgeAttribute("penwidth", "5");
	//gScene->setEdgeAttribute("lwidth", "5");
	gScene->setEdgeAttribute("labelfontsize", "15");
	*/

	//START AND END EXPERIMENT//
	expSceneItems.gvStartExperimentNode = graph.newNode();//indexing??
	//expSceneItems.gvStartExperimentNode = gScene->addNode("Start");
	//expSceneItems.gvStartExperimentNode->setAttribute("shape", "hexagon");
	//expSceneItems.gvStartExperimentNode->setAttribute("labelfontcolor", "green");
	//expSceneItems.gvStartExperimentNode->setAttribute("labelfontsize", "25");
	//expSceneItems.gvStartExperimentNode->setAttribute("tooltip", "Start of the experiment");
	expSceneItems.gvEndExperimentNode = graph.newNode();//indexing??
	//expSceneItems.gvEndExperimentNode = gScene->addNode("\nEnd");
	//expSceneItems.gvEndExperimentNode->setAttribute("shape", "hexagon");
	//expSceneItems.gvEndExperimentNode->setAttribute("labelfontcolor", "red");
	//expSceneItems.gvEndExperimentNode->setAttribute("labelfontsize", "25");
	//expSceneItems.gvEndExperimentNode->setAttribute("tooltip", "End of the experiment");

	//BLOCKS//
	if(expSceneItems.lBlocks.isEmpty() == false)
	{
		QString sTmpTooltip;
		for (int i=0;i<expSceneItems.lBlocks.count();i++)
		{
			//expSceneItems.lBlocks[i].gvNode = gScene->addNode(QString("%1 %2").arg(expSceneItems.lBlocks[i].sName).arg(expSceneItems.lBlocks[i].nNumber));
			expSceneItems.lBlocks[i].gvNode = new ogdf::node(graph.newNode());
			cBlockStructure *tmpBlockStruct = parsedExpStruct->getBlockPointerByID(expSceneItems.lBlocks[i].nId);
			if (tmpBlockStruct)
			{
				sTmpTooltip = "Trials: " + QString::number(tmpBlockStruct->getNumberOfTrials());
				sTmpTooltip = sTmpTooltip + "\nInt. Triggers: " + QString::number(tmpBlockStruct->getNumberOfInternalTriggers());
				sTmpTooltip = sTmpTooltip + "\nExt. Triggers: " + QString::number(tmpBlockStruct->getNumberOfExternalTriggers());
			}

			gAttr->x(*expSceneItems.lBlocks[i].gvNode) = -5*(i+1);
			gAttr->y(*expSceneItems.lBlocks[i].gvNode) = -20*i;		
			gAttr->width(*expSceneItems.lBlocks[i].gvNode) = 10*(i+1);
			gAttr->height(*expSceneItems.lBlocks[i].gvNode) = 15;

			//expSceneItems.lBlocks[i].gvNode->setAttribute("tooltip", sTmpTooltip);
			//if(i==0)
			//	gScene->setRootNode(expSceneItems.lBlocks[i].gvNode);
		}
	}
	else
	{
		return false;//no blocks defined...
	}

	//AUTO-CONNECTIONS//
	ogdf::node *tmpSourceBlock = NULL;
	ogdf::node *tmpTargetBlock = NULL;
	for (int i=0;i<expSceneItems.lAutoConns.count();i++)
	{	
		tmpSourceBlock = NULL;
		tmpTargetBlock = NULL;
		if(expSceneItems.lAutoConns[i].nSourceBlockId >= 0)
		{
			if(expSceneItems.lAutoConns[i].nTargetBlockId >= 0)//Block to Block connection
			{
				tmpSourceBlock = getGVNodePointer(expSceneItems.lAutoConns[i].nSourceBlockId);
				if(tmpSourceBlock)
				{
					tmpTargetBlock = getGVNodePointer(expSceneItems.lAutoConns[i].nTargetBlockId);
					if(tmpTargetBlock)
					{
						expSceneItems.lAutoConns[i].gvEdge = graph.newEdge(*tmpSourceBlock, *tmpTargetBlock);
							//gScene->addEdge(tmpSourceBlock, tmpTargetBlock, "Middle()");
						//expSceneItems.lAutoConns[i].gvEdge->setAttribute("color", "brown");
						//expSceneItems.lAutoConns[i].gvEdge->setAttribute("weight", "9999");
						continue;
					}
				}
			}
			else //End of experiment, no target
			{
				tmpSourceBlock = getGVNodePointer(expSceneItems.lAutoConns[i].nSourceBlockId);
				if(tmpSourceBlock)
				{
					expSceneItems.lAutoConns[i].gvEdge = graph.newEdge(*tmpSourceBlock, expSceneItems.gvEndExperimentNode);
						//..gScene->addEdge(tmpSourceBlock, expSceneItems.gvEndExperimentNode, "End()");
					//expSceneItems.lAutoConns[i].gvEdge->setAttribute("color", "red");
					//expSceneItems.lAutoConns[i].gvEdge->setAttribute("weight", "9999");
					continue;
				}
			}
		}
		else
		{
			if(expSceneItems.lAutoConns[i].nTargetBlockId >= 0)//Start of Experiment, no source
			{
				tmpTargetBlock = getGVNodePointer(expSceneItems.lAutoConns[i].nTargetBlockId);
				if(tmpTargetBlock)
				{
					expSceneItems.lAutoConns[i].gvEdge = graph.newEdge(expSceneItems.gvStartExperimentNode, *tmpTargetBlock);
						//gScene->addEdge(expSceneItems.gvStartExperimentNode, tmpTargetBlock, "Start()");
					//expSceneItems.lAutoConns[i].gvEdge->setAttribute("color", "green");
					//expSceneItems.lAutoConns[i].gvEdge->setAttribute("weight", "9999");
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
		ogdf::node *tmpSourceGVNode = NULL;
		ogdf::node *tmpTargetGVNode = NULL;
		for (int i=0;i<expSceneItems.lLoops.count();i++)
		{
			tmpSourceGVNode = getGVNodePointer(expSceneItems.lLoops[i].nSourceBlockId);
			tmpTargetGVNode = getGVNodePointer(expSceneItems.lLoops[i].nTargetBlockId);
			if((tmpSourceGVNode) && (tmpTargetGVNode))
				expSceneItems.lLoops[i].gvEdge = new ogdf::edge(graph.newEdge(*tmpSourceGVNode, *tmpTargetGVNode));
				//gScene->addEdge(tmpSourceGVNode,tmpTargetGVNode,QString("%1 %2").arg(expSceneItems.lLoops[i].sName).arg(expSceneItems.lLoops[i].nNumber));
		}
	}
	//Layout scene
	//gScene->applyLayout();			
	

	////////////////////////////////////////////////////////////////////////

	
	//const int LEN = 11;
	//for(int i = 1; i<LEN; ++i) 
	//{
	//	ogdf::node left = graph.newNode();
	//	gAttr->x(left) = -5*(i+1);
	//	gAttr->y(left) = -20*i;		
	//	gAttr->width(left) = 10*(i+1);
	//	gAttr->height(left) = 15;

	//	node bottom = graph.newNode();
	//	gAttr->x(bottom) = 20*(LEN-i);		
	//	gAttr->y(bottom) = 5*(LEN+1-i);
	//	gAttr->width(bottom) = 15;
	//	gAttr->height(bottom) = 10*(LEN+1-i);

	//	edge e = graph.newEdge(left,bottom);
	//	DPolyline &p = gAttr->bends(e);
	//	p.pushBack(DPoint(10,-20*i));
	//	p.pushBack(DPoint(20*(LEN-i),-10));
	//}

	
	int nodeWidth = 30;
	int nodeHeight = 30;
	int siblingDistance = nodeWidth + nodeHeight;
	ogdf::TreeLayout treeLayout;
	treeLayout.siblingDistance(siblingDistance);
	treeLayout.call(*gAttr);

	int width = gAttr->boundingBox().width(), height = gAttr->boundingBox().height();

	//ui->graphicsView->scene()->setSceneRect(QRect(0, 0, width+nodeWidth, height+nodeHeight));
	//cout << "Scene dimensions: " << gAttr->boundingBox().width() << " x " << gAttr->boundingBox().height() << endl;
	

	QRectF currentSceneRect = gScene->itemsBoundingRect();
	ui->graphicsView->setSceneRect(currentSceneRect);
	//qreal dScaleFactor;
	//if(bDrawVertical)
	//	dScaleFactor = ((ui->graphicsView->size().height()-(nWidgetMargin))*dGraphViewScale)/currentSceneRect.height();
	//else
	//	dScaleFactor = ((ui->graphicsView->size().width()-(nWidgetMargin))*dGraphViewScale)/currentSceneRect.width();
	//ui->graphicsView->resetTransform();
	//ui->graphicsView->scale(dScaleFactor, dScaleFactor);
	ui->graphicsView->centerOn(currentSceneRect.center());
	//ui->graphicsView->centerOn(gScene->itemsBoundingRect().center());


	gAttr->setAllWidth(nodeWidth);
	gAttr->setAllHeight(nodeHeight);

	ogdf::edge e;
	forall_edges(e,graph)
	{
		ogdf::node source = e->source(), target = e->target();
		int x1 = gAttr->x(source), y1 = gAttr->y(source);
		int x2 = gAttr->x(target), y2 = gAttr->y(target);
		QPainterPath p;
		p.moveTo(x1 + nodeWidth/2, y1 + nodeHeight/2);
		p.lineTo(x2 + nodeWidth/2, y2 + nodeHeight/2);
		(void) ui->graphicsView->scene()->addPath(p, QPen(Qt::darkGray), QBrush(Qt::white));
	}

	ogdf::node n;
	forall_nodes(n, graph) 
	{
		double x = gAttr->x(n);
		double y = gAttr->y(n);
		double w = gAttr->width(n);
		double h = gAttr->height(n);
		QRectF boundingRect(x, y, w, h);
		cout << x << " : " << y << " : " << endl;
		QRadialGradient radialGradient(boundingRect.center(), boundingRect.width());
		radialGradient.setColorAt(1.0, Qt::lightGray);
		radialGradient.setColorAt(0.7, QColor(230,230,240));
		radialGradient.setColorAt(0.0, Qt::white);
		(void) ui->graphicsView->scene()->addEllipse(boundingRect, QPen(Qt::black), QBrush(QRadialGradient(radialGradient)));
		//QGraphicsTextItem *text = ui->graphicsView->scene()->addText(QString(gAttr->labelNode(n).cstr()));
		//text->setPos(x, y);
	}
	// clear the graph after it has been displayed
	graph.clear();

	return true;
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
	resetExpSceneItemsCollection(expSceneItems);

	int nExpBlockCount = ExpStruct->getBlockCount();
	cBlockStructure *tmpBlock = NULL;
	cBlockStructure *lastBlock = NULL;
	int nNextSearchBlockNumber = 0;
	int nNextSearchLoopID = 0;
	expBlockItemStrc tmpBlockItem;
	expLoopItemStrc tmpLoopItem;

	expAutoConnItemStrc tmpAutoConnItem;

	expSceneItems.sExperimentName = ExpStruct->getExperimentName();
	for (int i=0;i<nExpBlockCount;i++)
	{
		tmpAutoConnItem.gvEdge = NULL;
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
			tmpBlockItem.gvNode = NULL;
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
						tmpLoopItem.gvEdge = NULL;						
						expSceneItems.lLoops.append(tmpLoopItem);
					}
				}
			}
		}
	}
	parsedExpStruct = ExpStruct;
	return drawGraph();
}

ogdf::node *ExperimentStructureVisualizer::getGVNodePointer(const int &nBlockID)
{
	for(int i=0;i<expSceneItems.lBlocks.count();i++)
	{
		if(expSceneItems.lBlocks[i].nId == nBlockID)
		{
			return expSceneItems.lBlocks[i].gvNode;
		}
	}
	return NULL;
}