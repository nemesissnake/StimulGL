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
	_scene = NULL;
	nWidgetMargin = 45;
	dGraphViewScale = 0.9;//fill till 90%

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
	if(_scene)
		delete _scene;
	if(graphViewLayout)
		delete graphViewLayout;
	resetExpSceneItemsCollection(expSceneItems);
	emit destroyed(this);
}

void ExperimentStructureVisualizer::resetExpSceneItemsCollection(expSceneItemStrc &tmpExpSceneItems)
{
	tmpExpSceneItems.sExperimentName = "<undefined>";
	tmpExpSceneItems.gvStartExperimentNode = NULL;
	tmpExpSceneItems.gvEndExperimentNode = NULL;	
	if(tmpExpSceneItems.lLoops.isEmpty() == false)
	{
		//for (i=0;i<tmpExpSceneItems.lLoops.count();i++)
		//{
		//	delete tmpExpSceneItems.lLoops[i].gvEdge;
		//}
		tmpExpSceneItems.lLoops.clear();
	}
	if(tmpExpSceneItems.lBlocks.isEmpty() == false)
	{
		//for (i=0;i<tmpExpSceneItems.lBlocks.count();i++)
		//{
		//	delete tmpExpSceneItems.lBlocks[i].gvNode;
		//}
		tmpExpSceneItems.lBlocks.clear();
	}
	if(tmpExpSceneItems.lAutoConns.isEmpty() == false)
	{
		tmpExpSceneItems.lAutoConns.clear();
	}
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
	resizeStructureView(ui->graphicsView->size().width(),ui->graphicsView->size().height());
	int test = ui->graphicsView->maximumWidth();
	//drawGraph();	
}

void ExperimentStructureVisualizer::createScene()
{
	_scene = new QGVScene("GraphicScene", this);
	connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
	connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
	ui->graphicsView->setScene(_scene);
}

void ExperimentStructureVisualizer::setupLayout()
{
	ui->mainLayout->setMenuBar(toolBar);
	if(graphViewLayout == NULL)
		graphViewLayout = new QVBoxLayout(ui->graphicsView);
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
	}
}

bool ExperimentStructureVisualizer::drawGraph(const QString &sDotContent)
{    
	if(_scene == NULL)
		createScene();
	else
		_scene->clear();

	if(sDotContent.isEmpty()==false)
	{
		//QString sDotContent2 = "digraph test{node [style=filled,fillcolor=white];N1 -> N2;N2 -> N3;N3 -> N4;N4 -> N1;}";
		//sDotContent = "digraph {"
		//"	subgraph cluster_0 "
		//"	{"
		//"		label=\"Subgraph A\";"
		//"		a -> b;"
		//"		b -> c;"
		//"		c -> d;"
		//"	}"
		//"	subgraph cluster_1 {"
		//"		label=\"Subgraph B\";"
		//"		a -> f;"
		//"		f -> c;"
		//"	}"
		//"}";
		//QString sDotContent2 = "digraph test123{"
		//"	a -> b[label=\"0.2\",weight=\"0.2\"];"
		//"	a -> c[label=\"0.4\",weight=\"0.4\"];"
		//"	c -> b[label=\"0.6\",weight=\"0.6\"];"
		//"	c -> e[label=\"0.6\",weight=\"0.6\"];"
		//"	e -> e[label=\"0.1\",weight=\"0.1\"];"
		//"	e -> b[label=\"0.7\",weight=\"0.7\"];"
		//"}";

		_scene->loadLayout(sDotContent);//gives error!
		//connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
		//connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
		//_view->setScene(_scene);
		return true;
	}
	else
	{
		//Configure scene attributes
		_scene->setGraphAttribute("label", "Experiment Structure");
		_scene->setGraphAttribute("splines", "spline");//"polyline");//"spline");//"line");//"ortho");
		_scene->setGraphAttribute("rankdir", "LR");
		//_scene->setGraphAttribute("concentrate", "true"); //Error !
		_scene->setGraphAttribute("nodesep", "0.2");

		_scene->setNodeAttribute("shape", "box");
		//_scene->setNodeAttribute("orientation", "0");
		_scene->setNodeAttribute("style", "filled");
		_scene->setNodeAttribute("fillcolor", "lightgrey");
		_scene->setNodeAttribute("height", "1.2");

		_scene->setNodeAttribute("labelfontname", "Times-Bold");
		_scene->setNodeAttribute("labelfontsize", "6");
		_scene->setNodeAttribute("labelfontcolor", "red");

		_scene->setEdgeAttribute("minlen", "3");
		//_scene->setEdgeAttribute("dir", "both");


		//START AND END EXPERIMENT//
		expSceneItems.gvStartExperimentNode = _scene->addNode("Start of\n experiment");
		expSceneItems.gvStartExperimentNode->setAttribute("shape", "hexagon");
		expSceneItems.gvEndExperimentNode = _scene->addNode("End of\n experiment");
		expSceneItems.gvEndExperimentNode->setAttribute("shape", "hexagon");

		//BLOCKS//
		if(expSceneItems.lBlocks.isEmpty() == false)
		{
			for (int i=0;i<expSceneItems.lBlocks.count();i++)
			{
				expSceneItems.lBlocks[i].gvNode = _scene->addNode(QString("%1 %2").arg(expSceneItems.lBlocks[i].sName).arg(expSceneItems.lBlocks[i].nNumber));
				if(i==0)
					_scene->setRootNode(expSceneItems.lBlocks[i].gvNode);
			}
		}
		else
		{
			return false;//no blocks defined...
		}

		//AUTO-CONNECTIONS//
		QGVNode *tmpSourceBlock = NULL;
		QGVNode *tmpTargetBlock = NULL;
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
							expSceneItems.lAutoConns[i].gvEdge = _scene->addEdge(tmpSourceBlock, tmpTargetBlock, "Middle()");
							expSceneItems.lAutoConns[i].gvEdge->setAttribute("color", "brown");
							expSceneItems.lAutoConns[i].gvEdge->setAttribute("weight", "9999");
							continue;
						}
					}
				}
				else //End of experiment, no target
				{
					tmpSourceBlock = getGVNodePointer(expSceneItems.lAutoConns[i].nSourceBlockId);
					if(tmpSourceBlock)
					{
						expSceneItems.lAutoConns[i].gvEdge = _scene->addEdge(tmpSourceBlock, expSceneItems.gvEndExperimentNode, "End()");
						expSceneItems.lAutoConns[i].gvEdge->setAttribute("color", "red");
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
						expSceneItems.lAutoConns[i].gvEdge = _scene->addEdge(expSceneItems.gvStartExperimentNode, tmpTargetBlock, "Start()");
						expSceneItems.lAutoConns[i].gvEdge->setAttribute("color", "green");
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
			QGVNode *tmpSourceGVNode = NULL;
			QGVNode *tmpTargetGVNode = NULL;
			for (int i=0;i<expSceneItems.lLoops.count();i++)
			{
				tmpSourceGVNode = getGVNodePointer(expSceneItems.lLoops[i].nSourceBlockId);
				tmpTargetGVNode = getGVNodePointer(expSceneItems.lLoops[i].nTargetBlockId);
				if((tmpSourceGVNode) && (tmpTargetGVNode))
					expSceneItems.lLoops[i].gvEdge = _scene->addEdge(tmpSourceGVNode,tmpTargetGVNode,QString("%1 %2").arg(expSceneItems.lLoops[i].sName).arg(expSceneItems.lLoops[i].nNumber));
			}
		}
		//Layout scene
		_scene->applyLayout();
				
		QRectF currentSceneRect = _scene->itemsBoundingRect();
		qreal dScaleFactor = ((ui->graphicsView->size().width()-(nWidgetMargin))*dGraphViewScale)/currentSceneRect.width();
		ui->graphicsView->scale(dScaleFactor, dScaleFactor);
		ui->graphicsView->centerOn(currentSceneRect.center());
		ui->graphicsView->centerOn(_scene->itemsBoundingRect().center());
	}
	return true;

	//node1->setIcon(QImage(":/icons/Gnome-System-Run-64.png"));
	//QList<QGraphicsItem*> items = _scene->items();//QRectF(pos - QPointF(1,1), QSize(3,3)));
	//foreach(QGraphicsItem *item, items)
	//	if (item->type() > QGraphicsItem::UserType)
			
	//ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
	//rectNeededSize
	//gView->ensureVisible(rectNeededSize, dLeftCanvasMargin, dTopCanvasMargin);
	//gView->centerOn(rectNeededSize.x()/2,rectNeededSize.y()/2);
}

void ExperimentStructureVisualizer::nodeContextMenu(QGVNode *node)
{
    //Context menu example
    QMenu menu(node->label());

    menu.addSeparator();
    menu.addAction(tr("Informations"));
    menu.addAction(tr("Options"));

    QAction *action = menu.exec(QCursor::pos());
    if(action == 0)
        return;
}

void ExperimentStructureVisualizer::nodeDoubleClick(QGVNode *node)
{
	//node->moveBy(10,10);
	//_scene->setNodeAttribute("pos","200,200");
	//QGVNode *tmpNode = _scene->getNode("SERVER1");
	//if(tmpNode)
	//{
	//	tmpNode->setAttribute("fillcolor","purple");
	//	tmpNode->updateLayout();
	//}

	//_scene->setNodeAttribute("fillcolor","black");

    QMessageBox::information(this, tr("Node double clicked"), tr("Node %1").arg(node->label()));
	//_scene->applyLayout();
	//_scene->saveLayout("E:\\Projects\\qgv\\debug\\debug.dot",0);
	//_scene->saveLayout("E:\\Projects\\qgv\\debug\\debug.png",1);
}

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
		//if(i==0)
		//	tmpAutoConnItem.tConnType = AutoConnType_StartToBlock;
		//else
		//	tmpAutoConnItem.tConnType = AutoConnType_BlockToBlock;

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
			//tmpBlockItem.gvNode = _scene->addNode(QString("%1 %2").arg(tmpBlockItem.sName).arg(tmpBlockItem.nNumber));
			tmpBlockItem.gvNode = NULL;
			expSceneItems.lBlocks.append(tmpBlockItem);
		}
		expSceneItems.lAutoConns.append(tmpAutoConnItem);
		if(i==(nExpBlockCount-1))
		{
			//tmpAutoConnItem.tConnType = AutoConnType_BlockToEnd;
			tmpAutoConnItem.nSourceBlockId = tmpBlock->getBlockID();
			tmpAutoConnItem.nTargetBlockId = -1;
			//tmpAutoConnItem.gvEdge = NULL;
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
	return drawGraph();
}

QGVNode *ExperimentStructureVisualizer::getGVNodePointer(const int &nBlockID)
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