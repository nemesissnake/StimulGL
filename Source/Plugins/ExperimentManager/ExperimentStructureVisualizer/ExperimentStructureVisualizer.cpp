#include "ExperimentStructureVisualizer.h"
#include "QGVScene.h"
#include "QGraphicsViewEc.h"
#include "../ExperimentStructures.h"
#include <QMessageBox>
#include <QToolBar>
#include <QToolButton>
#include <QMenu>
#include <QVBoxLayout>

ExperimentStructureVisualizer::ExperimentStructureVisualizer(QWidget *parent) : QWidget(parent)//, ui(new Ui::MainWindow)
{
	mainLayout = NULL;
	action_Quit = NULL;
	toolBar = NULL;
	menuFile = NULL;
	menuEdit = NULL;
	buttonFile = NULL;
	//buttonEdit = NULL;
	_view = NULL;
	_scene = NULL;

	configureActions(true);
	setupMenuAndActions();
	setupLayout();
	
    connect(_scene, SIGNAL(nodeContextMenu(QGVNode*)), SLOT(nodeContextMenu(QGVNode*)));
    connect(_scene, SIGNAL(nodeDoubleClick(QGVNode*)), SLOT(nodeDoubleClick(QGVNode*)));
}

ExperimentStructureVisualizer::~ExperimentStructureVisualizer()
{
    configureActions(false);
	if(menuFile)
		delete menuFile;
	//if(menuEdit)
	//	delete menuEdit;
	if(buttonFile)
		delete buttonFile;
	//if(buttonEdit)
	//	delete buttonEdit;
	if(toolBar)
		delete toolBar;
	if(_scene)
		delete _scene;
	if(_view)
		delete _view;
	if(mainLayout)
		delete mainLayout;
	emit destroyed(this);
}

void ExperimentStructureVisualizer::configureActions(bool bCreate)
{
	if(bCreate)
	{
		action_Quit = new QAction("Quit", this);
	}
	else
	{		
		if(action_Quit)
		{
			delete action_Quit;
			action_Quit = NULL;
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
	//buttonEdit=new QToolButton(this);
	//buttonEdit->setText("Edit ");
	//buttonEdit->setPopupMode(QToolButton::InstantPopup);
	//menuEdit = new QMenu();//FileButton
	//menuEdit->addAction(actionFind);
	//menuEdit->addAction(actionAdd_Node);
	//menuEdit->addAction(actionAdd_Subnode);
	//menuEdit->addAction(action_Remove_Node);
	//menuEdit->addAction(actionAdd_Attribute);
	//buttonEdit->setMenu(menuEdit);
	//toolBar->addWidget(buttonEdit);

	connect(action_Quit, SIGNAL(triggered()), this, SLOT(close()));
}

void ExperimentStructureVisualizer::setupLayout()
{
	_scene = new QGVScene("DEMO", this);
	_view = new QGraphicsViewEc(this);
	_view->setScene(_scene);

	mainLayout = new QVBoxLayout();
	mainLayout->setMenuBar(toolBar);
	mainLayout->addWidget(_view);
	mainLayout->setContentsMargins(0,0,0,0);
	mainLayout->setSpacing(0);
	setLayout(mainLayout);	
}

void ExperimentStructureVisualizer::drawGraph()
{    
	//QString sDotContent;
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
    //ui->graphicsView->setScene(_scene);
    //return;


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



	/*
    //Configure scene attributes
    _scene->setGraphAttribute("label", "DEMO");

    _scene->setGraphAttribute("splines", "spline");//"line");//"ortho");
    _scene->setGraphAttribute("rankdir", "LR");
    //_scene->setGraphAttribute("concentrate", "true"); //Error !
    _scene->setGraphAttribute("nodesep", "0.4");

    _scene->setNodeAttribute("shape", "box");
    _scene->setNodeAttribute("style", "filled");
    _scene->setNodeAttribute("fillcolor", "white");
    _scene->setNodeAttribute("height", "1.2");
    _scene->setEdgeAttribute("minlen", "3");
    //_scene->setEdgeAttribute("dir", "both");

    //Add some nodes
    QGVNode *node1 = _scene->addNode("BOX");
    node1->setIcon(QImage(":/icons/Gnome-System-Run-64.png"));
	//node1->setPosition(QPointF(100,200));

    QGVNode *node2 = _scene->addNode("SERVER0");
    node2->setIcon(QImage(":/icons/Gnome-Network-Transmit-64.png"));
    QGVNode *node3 = _scene->addNode("SERVER1");
    node3->setIcon(QImage(":/icons/Gnome-Network-Transmit-64.png"));
	node3->setAttribute("fillcolor","yellow");
	//node3->setAttribute("pos","1,20!");
    QGVNode *node4 = _scene->addNode("USER");
    node4->setIcon(QImage(":/icons/Gnome-Stock-Person-64.png"));
	//node4->setAttribute("pos","5,5!");
    QGVNode *node5 = _scene->addNode("SWITCH");
    node5->setIcon(QImage(":/icons/Gnome-Network-Server-64.png"));

    //Add some edges
    _scene->addEdge(node1, node2, "TTL(centronics)")->setAttribute("color", "red");
    _scene->addEdge(node1, node2, "SERIAL");
    _scene->addEdge(node1, node3, "RAZ")->setAttribute("color", "blue");
    _scene->addEdge(node2, node3, "SECU");

    _scene->addEdge(node2, node4, "STATUS")->setAttribute("color", "red");

    _scene->addEdge(node4, node3, "ACK")->setAttribute("color", "red");

    _scene->addEdge(node4, node2, "TBIT");
    _scene->addEdge(node4, node2, "ETH");
    _scene->addEdge(node4, node2, "RS232");

    _scene->addEdge(node4, node5, "ETH1");
    _scene->addEdge(node2, node5, "ETH2");

    QGVSubGraph *sgraph = _scene->addSubGraph("SUB1");
    sgraph->setAttribute("label", "OFFICE");

    QGVNode *snode1 = sgraph->addNode("PC0152");
    QGVNode *snode2 = sgraph->addNode("PC0153");

    _scene->addEdge(snode1, snode2, "RT7");

    _scene->addEdge(node3, snode1, "GB8");
    _scene->addEdge(node3, snode2, "TS9");


    QGVSubGraph *ssgraph = sgraph->addSubGraph("SUB2");
    ssgraph->setAttribute("label", "DESK");
    _scene->addEdge(snode1, ssgraph->addNode("PC0155"), "S10");
	*/

    //Layout scene
    _scene->applyLayout();
    //Fit in view
    //ui->graphicsView->fitInView(_scene->sceneRect(), Qt::KeepAspectRatio);
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
	_scene->saveLayout("E:\\Projects\\qgv\\debug\\debug.dot",0);
	_scene->saveLayout("E:\\Projects\\qgv\\debug\\debug.png",1);
}

bool ExperimentStructureVisualizer::parseExperimentStructure(cExperimentStructure *ExpStruct)
{//Make sure to first call the above install()!
	if((_scene == NULL) || (_view == NULL))
		return false;
	_scene->clear();

	int nExpBlockCount = ExpStruct->getBlockCount();
	double dLeftCanvasMargin = 50.0;
	double dTopCanvasMargin = 50.0;
	double dBlockDistance = 200.0;
	cBlockStructure *tmpBlock;
	int nNextSearchBlockNumber = 0;
	int nNextSearchLoopID = 0;
	QMap<int,QPointF> mBlockPositions;
	QRectF tmpRect;
	QRectF rectNeededSize;

	rectNeededSize.setWidth(50);
	rectNeededSize.setHeight(50);
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
						//ExperimentGraphBlock *tmpFromBlockGraphItem = NULL;
						//ExperimentGraphBlock *tmpToBlockGraphItem = NULL;
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
									//tmpFromGraphItem = itemAt(mBlockPositions.value(tmpBlock->getBlockID()));
									//tmpToGraphItem = itemAt(mBlockPositions.value(nTargetBlockID));
									//tmpFromBlockGraphItem = (ExperimentGraphBlock*)tmpFromGraphItem;
									//tmpToBlockGraphItem = (ExperimentGraphBlock*)tmpToGraphItem;		
									//QVector<ExperimentGraphPort*> tmpFromPorts = tmpFromBlockGraphItem->ports();
									//QVector<ExperimentGraphPort*> tmpToPorts = tmpToBlockGraphItem->ports();
									//createConnection(tmpFromPorts[3],tmpToPorts[2]);
								}
							}
						}
					}
				}
			}
		}
	}
	//QList<QGraphicsItem*> items = gScene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));
	//foreach(QGraphicsItem *item, items)
	//	if (item->type() > QGraphicsItem::UserType)
	//gScene->setSceneRect(QRectF(0, 0, 50, 50));
	//rectNeededSize
	//gView->ensureVisible(rectNeededSize, dLeftCanvasMargin, dTopCanvasMargin);
	//gView->centerOn(rectNeededSize.x()/2,rectNeededSize.y()/2);
	drawGraph();
	return true;
}