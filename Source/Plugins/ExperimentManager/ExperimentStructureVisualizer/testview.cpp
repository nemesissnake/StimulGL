#include "testview.h"
#include "ui_testview.h"
#include "QGraphicsViewEc.h"
#include "../ExperimentStructures.h"
#include <QMessageBox>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QVBoxLayout>

testView::testView(QWidget *parent) : QWidget(parent)
{
	ui = new Ui::testView();
	ui->setupUi(this);

	action_Quit = NULL;
	action_Test = NULL;
	toolBar = NULL;
	menuFile = NULL;
	menuEdit = NULL;
	buttonFile = NULL;
	buttonEdit = NULL;
	//_view = NULL;
	_scene = NULL;

	resetExpSceneItemsCollection(expSceneItems);
	configureActions(true);
	setupMenuAndActions();

	//_scene = new QGVScene("DEMO", this);
	//_view = new QGraphicsViewEc(this);

	setupLayout();
	
    //connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    //connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
}

testView::~testView()
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
	resetExpSceneItemsCollection(expSceneItems);
	emit destroyed(this);
}

void testView::resetExpSceneItemsCollection(expSceneItemStrc &tmpExpSceneItems)
{
	tmpExpSceneItems.sExperimentName = "<undefined>";
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
}

void testView::configureActions(bool bCreate)
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

void testView::setupMenuAndActions()
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
	menuEdit = new QMenu();//FileButton
	menuEdit->addAction(action_Test);
	//menuEdit->addAction(actionAdd_Node);
	//menuEdit->addAction(actionAdd_Subnode);
	//menuEdit->addAction(action_Remove_Node);
	//menuEdit->addAction(actionAdd_Attribute);
	buttonEdit->setMenu(menuEdit);
	toolBar->addWidget(buttonEdit);

	connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
	connect(action_Test, SIGNAL(triggered()), this, SLOT(Test()));
	
}

void testView::Test()
{
	drawGraph();
}

void testView::setupLayout()
{
	_scene = new QGVScene("DEMO", this);
	//ui->graphicsView = new QGraphicsViewEc(this);
	ui->graphicsView->setScene(_scene);

	//ui->graphicsView->fitInView(QRectF(0.0,0.0,200.0,200.0), Qt::KeepAspectRatio);
	//ui->graphicsView->setMinimumSize(200,200);
	//_scene->saveLayout("D:\\Projects\\Sven.Gijsen\\StimulGL\\Install\\debug.png",1);


	//rectNeededSize
	//gView->ensureVisible(rectNeededSize, dLeftCanvasMargin, dTopCanvasMargin);
	//gView->centerOn(rectNeededSize.x()/2,rectNeededSize.y()/2);
		
	toolBar->setMinimumSize(300,0);
	ui->mainLayout->setMenuBar(toolBar);
	ui->mainLayout->addWidget(ui->graphicsView);
	ui->mainLayout->setContentsMargins(0,0,0,0);
	ui->mainLayout->setSpacing(0);

	QSize tmp = ui->mainLayout->minimumSize();//61,61...
	//tmp = this->minimumSize();
	setLayout(ui->mainLayout);	
}

void testView::showEvent ( QShowEvent * event )
{
	//QTimer::singleShot(2000,this,SLOT("drawGraph()"));//drawGraph();
}

bool testView::drawGraph()
{    
	if(_scene == NULL)
		_scene = new QGVScene("DEMO", this);
	else
		_scene->clear();
	///QString sDotContent;
	//sDotContent = "digraph test{node [style=filled,fillcolor=white];N1 -> N2;N2 -> N3;N3 -> N4;N4 -> N1;}";
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
	//sDotContent = "digraph test123{"
	//"	a -> b[label=\"0.2\",weight=\"0.2\"];"
	//"	a -> c[label=\"0.4\",weight=\"0.4\"];"
	//"	c -> b[label=\"0.6\",weight=\"0.6\"];"
	//"	c -> e[label=\"0.6\",weight=\"0.6\"];"
	//"	e -> e[label=\"0.1\",weight=\"0.1\"];"
	//"	e -> b[label=\"0.7\",weight=\"0.7\"];"
	//"}";

    //_scene->loadLayout(sDotContent);
    //connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    //connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
    //_view->setScene(_scene);
    //return;

	/*		
			QString sBlockName;
			QList<QGVSubGraph *> tmpGraph;
			QList<QGVNode *> tmpNodeInt;
			QList<QGVNode *> tmpNodeLoops;
			QList<QGVEdge *> tmpConnectionLoops;
			QList<QGVEdge *> tmpBackLoops;
			int nBlockCount = 5;
			//Add some nodes
			for(int i=0;i<nBlockCount;i++)
			{
				sBlockName = "Block " + QString::number(i);
				tmpGraph.append(_scene->addSubGraph(sBlockName));
				tmpGraph.last()->setAttribute("label", sBlockName + " Label");
				//tmpGraph.last()
				tmpNodeInt.append(tmpGraph.last()->addNode(sBlockName + "(internal)\nTrials: 6\nTriggers(int): 2\nTriggers(ext): 1"));
				tmpNodeLoops.append(tmpGraph.last()->addNode("Loops"));
			}
			//QGVNode *node1 = _scene->addNode("Block 1");
			//node1->setIcon(QImage(":/icons/Gnome-System-Run-64.png"));

			//Add some edges(Connections)
			for(int i=0;i<nBlockCount-1;i++)
			{	
				tmpConnectionLoops.append(_scene->addEdge(tmpNodeInt[i], tmpNodeInt[i+1], "Connection()"));
				tmpConnectionLoops.last()->setAttribute("color", "blue");
				tmpConnectionLoops.last()->setAttribute("weight", "9999");
			}

			//Add some edges(Loops)
			tmpBackLoops.append(_scene->addEdge(tmpNodeLoops[3], tmpNodeInt[2], "Loop 1"));
			tmpBackLoops.last()->setAttribute("color", "green");
			//tmpBackLoops.append(_scene->addEdge(tmpNodeLoops[3], tmpNodeInt[0], "Loop 3"));
			//tmpBackLoops.last()->setAttribute("color", "green");
			//tmpBackLoops.append(_scene->addEdge(tmpNodeLoops[3], tmpNodeInt[1], "Loop 2b"));//a is drawn above b!
			//tmpBackLoops.last()->setAttribute("color", "green");
			//tmpBackLoops.last()->setAttribute("weight", "1");
			//tmpBackLoops.append(_scene->addEdge(tmpNodeLoops[3], tmpNodeInt[1], "Loop 2a"));//a is drawn above b!
			//tmpBackLoops.last()->setAttribute("color", "green");
			//tmpBackLoops.last()->setAttribute("weight", "2");

			tmpBackLoops.append(_scene->addEdge(tmpNodeInt[2], tmpNodeLoops[3], "Loop 4"));
			tmpBackLoops.last()->setAttribute("color", "green");

			tmpBackLoops.append(_scene->addEdge(tmpNodeInt[2], tmpNodeLoops[1], "Loop 5"));
			tmpBackLoops.last()->setAttribute("color", "green");
			tmpBackLoops.append(_scene->addEdge( tmpNodeLoops[1], tmpNodeInt[2],"Loop 6"));
			tmpBackLoops.last()->setAttribute("color", "green");
			tmpBackLoops.append(_scene->addEdge( tmpNodeInt[0], tmpNodeInt[4],"Loop 7"));
			tmpBackLoops.last()->setAttribute("color", "green");

			//Add Start and End
			tmpNodeInt.append(_scene->addNode("End of\n experiment"));
			tmpNodeInt.last()->setAttribute("shape", "hexagon");
			_scene->addEdge(tmpNodeInt[nBlockCount-1], tmpNodeInt.last(), "XXX")->setAttribute("color", "red");
			tmpNodeInt.append(_scene->addNode("Start of\n experiment"));
			tmpNodeInt.last()->setAttribute("shape", "hexagon");
			_scene->addEdge(tmpNodeInt.last(), tmpNodeInt[0], "XXX")->setAttribute("color", "red");

	*/
	
		//_view = new QGraphicsViewEc(this);
			//ui->graphicsView->setScene(_scene);

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

			//QList<QGraphicsItem*> items = _scene->items();//QRectF(pos - QPointF(1,1), QSize(3,3)));
			//foreach(QGraphicsItem *item, items)
			//	if (item->type() > QGraphicsItem::UserType)
			QGVNode *tmpNode = _scene->addNode("label1");
			_scene->applyLayout();
			//_scene->setRootNode(tmpNode);
			//_scene->setSceneRect(QRectF(0, 0, 400, 400));
			QRectF tmpRect = _scene->sceneRect();
			//_view->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
			//_view->setScene(_scene);

			
			ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
			//_scene->saveLayout("D:\\Projects\\Sven.Gijsen\\StimulGL\\Install\\debug.png",1);


			//rectNeededSize
			//gView->ensureVisible(rectNeededSize, dLeftCanvasMargin, dTopCanvasMargin);
			//gView->centerOn(rectNeededSize.x()/2,rectNeededSize.y()/2);

	return true;

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
	
	if(expSceneItems.lBlocks.isEmpty() == false)
	{
		for (int i=0;i<expSceneItems.lBlocks.count();i++)
		{
			expSceneItems.lBlocks[i].gvNode = _scene->addNode(QString("%1 %2").arg(expSceneItems.lBlocks[i].sName).arg(expSceneItems.lBlocks[i].nNumber));
			if(i==0)
				_scene->setRootNode(expSceneItems.lBlocks[i].gvNode);
		}
	}

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
	//gvEdge = 

	


    //Layout scene
    _scene->applyLayout();
	//QRectF tmpRect = _scene->sceneRect();
	ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
	ui->graphicsView->setScene(_scene);
}

void testView::nodeContextMenu(QGVNode *node)
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

void testView::nodeDoubleClick(QGVNode *node)
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

bool testView::parseExperimentStructure(cExperimentStructure *ExpStruct)
{//Make sure to first call the above install()!
	resetExpSceneItemsCollection(expSceneItems);

	int nExpBlockCount = ExpStruct->getBlockCount();
	cBlockStructure *tmpBlock = NULL;
	int nNextSearchBlockNumber = 0;
	int nNextSearchLoopID = 0;
	expBlockItemStrc tmpBlockItem;
	expLoopItemStrc tmpLoopItem;

	expSceneItems.sExperimentName = ExpStruct->getExperimentName();
	for (int i=0;i<nExpBlockCount;i++)
	{
		tmpBlock = NULL;
		tmpBlock = ExpStruct->getNextClosestBlockNumberByFromNumber(nNextSearchBlockNumber);
		if(tmpBlock) 
		{
			nNextSearchBlockNumber = tmpBlock->getBlockNumber() + 1;
			tmpBlockItem.nId = tmpBlock->getBlockID();
			tmpBlockItem.nNumber = tmpBlock->getBlockNumber();
			tmpBlockItem.sName = tmpBlock->getBlockName();
			//tmpBlockItem.gvNode = _scene->addNode(QString("%1 %2").arg(tmpBlockItem.sName).arg(tmpBlockItem.nNumber));
			tmpBlockItem.gvNode = NULL;
			expSceneItems.lBlocks.append(tmpBlockItem);


			//ExperimentGraphBlock *gBlock = new ExperimentGraphBlock(NULL);
			//gScene->addItem(gBlock);
			//gBlock->setName(tmpBlock->getBlockName());
			//gBlock->setID(tmpBlock->getBlockID());
			//gBlock->addPort(tmpBlock->getBlockName(), false, ExperimentGraphPort::NamePort);
			//gBlock->addPort(QString::number(tmpBlock->getBlockID()) + ": Blocknumber " + QString::number(tmpBlock->getBlockNumber()), false, ExperimentGraphPort::TypePort);
			//gBlock->addInputPort("In");
			//gBlock->addOutputPort("Out");
			//tmpRect = gBlock->boundingRect();
			//gBlock->setPos(dLeftCanvasMargin + (i*dBlockDistance) + tmpRect.width()/2, dTopCanvasMargin + tmpRect.height()/2);
			//mBlockPositions.insert(gBlock->getID(),gBlock->pos());
			//if((tmpRect.x() + tmpRect.width() + (dLeftCanvasMargin*2)) > rectNeededSize.width())
			//	rectNeededSize.setWidth(tmpRect.x() + tmpRect.width() + (dLeftCanvasMargin*2));
			//if((tmpRect.y() + tmpRect.height() + (dTopCanvasMargin*2)) > rectNeededSize.height())
			//	rectNeededSize.setHeight(tmpRect.y() + tmpRect.height()  + (dTopCanvasMargin*2));
			////gBlock = b->clone();
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



						//if((tmpSourceGVNode)&&(tmpTargetGVNode))
						//	tmpLoopItem.gvEdge = _scene->addEdge(tmpSourceGVNode,tmpTargetGVNode,QString("%1 %2").arg(tmpLoopItem.sName).arg(tmpLoopItem.nNumber));
						//else
							tmpLoopItem.gvEdge = NULL;
						
						expSceneItems.lLoops.append(tmpLoopItem);

						//if ((mBlockPositions.contains(nTargetBlockID)) && (mBlockPositions.contains(tmpBlock->getBlockID())))
						//{
							//tmpFromGraphItem = itemAt(mBlockPositions.value(tmpBlock->getBlockID()));
							//tmpToGraphItem = itemAt(mBlockPositions.value(nTargetBlockID));
							//tmpFromBlockGraphItem = (ExperimentGraphBlock*)tmpFromGraphItem;
							//tmpToBlockGraphItem = (ExperimentGraphBlock*)tmpToGraphItem;		
							//QVector<ExperimentGraphPort*> tmpFromPorts = tmpFromBlockGraphItem->ports();
							//QVector<ExperimentGraphPort*> tmpToPorts = tmpToBlockGraphItem->ports();
							//createConnection(tmpFromPorts[3],tmpToPorts[2]);
						//}
					}
				}
			}
		}
	}
	return true;//drawGraph();
}

QGVNode *testView::getGVNodePointer(const int &nTargetBlockID)
{
	for(int i=0;i<expSceneItems.lBlocks.count();i++)
	{
		if(expSceneItems.lBlocks[i].nId == nTargetBlockID)
		{
			return expSceneItems.lBlocks[i].gvNode;
		}
	}
	return NULL;
}