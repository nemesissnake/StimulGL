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

#include "DiagramArrow.h"
#include "DiagramItem.h"
#include "DiagramScene.h"
#include "DiagramTextItem.h"
#include "VisualExperimentEditor.h"
#include "ExperimentGraphPort.h"
#include "ExperimentGraphConnection.h"
#include "ExperimentGraphBlock.h"

#include <QtWidgets>

const int InsertTextButton = 10;

VisualExperimentEditor::VisualExperimentEditor()
{
	conn = NULL;
	bAllowSelfRecurrentConnection = true;

    createActions();
    createToolBox();
    createMenus();

    gScene = new DiagramScene(itemMenu, this);
	gScene->installEventFilter(this);
    gScene->setSceneRect(QRectF(0, 0, 5000, 5000));
    connect(gScene, SIGNAL(itemInserted(DiagramItem*)), this, SLOT(itemInserted(DiagramItem*)));
    connect(gScene, SIGNAL(textInserted(QGraphicsTextItem*)), this, SLOT(textInserted(QGraphicsTextItem*)));
    connect(gScene, SIGNAL(itemSelected(QGraphicsItem*)), this, SLOT(itemSelected(QGraphicsItem*)));
    createToolbars();

    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(toolBox);
    gView = new QGraphicsView(gScene);
	gView->setRenderHint(QPainter::Antialiasing);
	//    gView.backgroundBrush = new QBrush(new QPixmap("images/cheese.png"));
	//    gView.cacheMode = new QGraphicsView.CacheMode(QGraphicsView.CacheBackground);
	//	  gView->dragMode = QGraphicsView.ScrollHandDrag;
	//    gView.viewportUpdateMode = QGraphicsView.FullViewportUpdate;
	hLayout->addWidget(gView);

	QVBoxLayout *vLayout = new QVBoxLayout;
	vLayout->setMenuBar(mainMenuBar);
	vLayout->addWidget(mainToolBar);
	vLayout->addLayout(hLayout);

	this->setLayout(vLayout);
}

VisualExperimentEditor::~VisualExperimentEditor()
{

}

bool VisualExperimentEditor::eventFilter(QObject *o, QEvent *e)
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


void VisualExperimentEditor::backgroundButtonGroupClicked(QAbstractButton *button)
{
    QList<QAbstractButton *> buttons = backgroundButtonGroup->buttons();
    foreach (QAbstractButton *myButton, buttons) 
	{
        if (myButton != button)
            button->setChecked(false);
    }
    QString text = button->text();
    if (text == tr("Blue Grid"))
        gScene->setBackgroundBrush(QPixmap(":/visualexperimenteditor/images/background1.png"));
    else if (text == tr("White Grid"))
        gScene->setBackgroundBrush(QPixmap(":/visualexperimenteditor/images/background2.png"));
    else if (text == tr("Gray Grid"))
        gScene->setBackgroundBrush(QPixmap(":/visualexperimenteditor/images/background3.png"));
    else
        gScene->setBackgroundBrush(QPixmap(":/visualexperimenteditor/images/background4.png"));

    gScene->update();
    gView->update();
}

void VisualExperimentEditor::buttonGroupClicked(int id)
{
    QList<QAbstractButton *> buttons = buttonGroup->buttons();
    foreach (QAbstractButton *button, buttons) {
        if (buttonGroup->button(id) != button)
            button->setChecked(false);
    }
    if (id == InsertTextButton) {
        gScene->setMode(DiagramScene::InsertText);
    } else {
        gScene->setItemType(DiagramItem::DiagramType(id));
        gScene->setMode(DiagramScene::InsertItem);
    }
}

void VisualExperimentEditor::deleteItem()
{
    foreach (QGraphicsItem *item, gScene->selectedItems()) {
        if (item->type() == DiagramArrow::Type) {
            gScene->removeItem(item);
            DiagramArrow *arrow = qgraphicsitem_cast<DiagramArrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, gScene->selectedItems()) {
         if (item->type() == DiagramItem::Type)
             qgraphicsitem_cast<DiagramItem *>(item)->removeArrows();
         gScene->removeItem(item);
         delete item;
     }
}

void VisualExperimentEditor::pointerGroupClicked(int)
{
    gScene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void VisualExperimentEditor::bringToFront()
{
    if (gScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = gScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() + 0.1;
    }
    selectedItem->setZValue(zValue);
}

void VisualExperimentEditor::sendToBack()
{
    if (gScene->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = gScene->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->type() == DiagramItem::Type)
            zValue = item->zValue() - 0.1;
    }
    selectedItem->setZValue(zValue);
}

void VisualExperimentEditor::itemInserted(DiagramItem *item)
{
    pointerTypeGroup->button(int(DiagramScene::MoveItem))->setChecked(true);
    gScene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
    buttonGroup->button(int(item->diagramType()))->setChecked(false);
}

void VisualExperimentEditor::textInserted(QGraphicsTextItem *)
{
    buttonGroup->button(InsertTextButton)->setChecked(false);
    gScene->setMode(DiagramScene::Mode(pointerTypeGroup->checkedId()));
}

void VisualExperimentEditor::currentFontChanged(const QFont &)
{
    handleFontChange();
}

void VisualExperimentEditor::fontSizeChanged(const QString &)
{
    handleFontChange();
}

void VisualExperimentEditor::sceneScaleChanged(const QString &scale)
{
    double newScale = scale.left(scale.indexOf(tr("%"))).toDouble() / 100.0;
    QMatrix oldMatrix = gView->matrix();
    gView->resetMatrix();
    gView->translate(oldMatrix.dx(), oldMatrix.dy());
    gView->scale(newScale, newScale);
}

void VisualExperimentEditor::textColorChanged()
{
    textAction = qobject_cast<QAction *>(sender());
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/visualexperimenteditor/images/textpointer.png", qvariant_cast<QColor>(textAction->data())));
    textButtonTriggered();
}

void VisualExperimentEditor::itemColorChanged()
{
    fillAction = qobject_cast<QAction *>(sender());
    fillColorToolButton->setIcon(createColorToolButtonIcon(":/visualexperimenteditor/images/floodfill.png", qvariant_cast<QColor>(fillAction->data())));
    fillButtonTriggered();
}

void VisualExperimentEditor::lineColorChanged()
{
    lineAction = qobject_cast<QAction *>(sender());
    lineColorToolButton->setIcon(createColorToolButtonIcon(":/visualexperimenteditor/images/linecolor.png", qvariant_cast<QColor>(lineAction->data())));
    lineButtonTriggered();
}

void VisualExperimentEditor::textButtonTriggered()
{
    gScene->setTextColor(qvariant_cast<QColor>(textAction->data()));
}

void VisualExperimentEditor::fillButtonTriggered()
{
    gScene->setItemColor(qvariant_cast<QColor>(fillAction->data()));
}

void VisualExperimentEditor::lineButtonTriggered()
{
    gScene->setLineColor(qvariant_cast<QColor>(lineAction->data()));
}

void VisualExperimentEditor::handleFontChange()
{
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    gScene->setFont(font);
}

void VisualExperimentEditor::itemSelected(QGraphicsItem *item)
{
    DiagramTextItem *textItem =
    qgraphicsitem_cast<DiagramTextItem *>(item);

    QFont font = textItem->font();
    fontCombo->setCurrentFont(font);
    fontSizeCombo->setEditText(QString().setNum(font.pointSize()));
    boldAction->setChecked(font.weight() == QFont::Bold);
    italicAction->setChecked(font.italic());
    underlineAction->setChecked(font.underline());
}

void VisualExperimentEditor::about()
{
    QMessageBox::about(NULL, tr("About Visual Experiment Manager"), tr("The <b>Visual Experiment Manager</b> allows you to a experiment."));
}

void VisualExperimentEditor::createToolBox()
{
    buttonGroup = new QButtonGroup(this);
    buttonGroup->setExclusive(false);
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(buttonGroupClicked(int)));
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(createCellWidget(tr("Conditional"), DiagramItem::Conditional), 0, 0);
    layout->addWidget(createCellWidget(tr("Process"), DiagramItem::Step),0, 1);
    layout->addWidget(createCellWidget(tr("Input/Output"), DiagramItem::Io), 1, 0);

    QToolButton *textButton = new QToolButton;
    textButton->setCheckable(true);
    buttonGroup->addButton(textButton, InsertTextButton);
    textButton->setIcon(QIcon(QPixmap(":/visualexperimenteditor/images/textpointer.png")));
    textButton->setIconSize(QSize(50, 50));
    QGridLayout *textLayout = new QGridLayout;
    textLayout->addWidget(textButton, 0, 0, Qt::AlignHCenter);
    textLayout->addWidget(new QLabel(tr("Text")), 1, 0, Qt::AlignCenter);
    QWidget *textWidget = new QWidget;
    textWidget->setLayout(textLayout);
    layout->addWidget(textWidget, 1, 1);

    layout->setRowStretch(3, 10);
    layout->setColumnStretch(2, 10);

    QWidget *itemWidget = new QWidget;
    itemWidget->setLayout(layout);

    backgroundButtonGroup = new QButtonGroup(this);
    connect(backgroundButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(backgroundButtonGroupClicked(QAbstractButton*)));

    QGridLayout *backgroundLayout = new QGridLayout;
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Blue Grid"), ":/visualexperimenteditor/images/background1.png"), 0, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("White Grid"), ":/visualexperimenteditor/images/background2.png"), 0, 1);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("Gray Grid"), ":/visualexperimenteditor/images/background3.png"), 1, 0);
    backgroundLayout->addWidget(createBackgroundCellWidget(tr("No Grid"), ":/visualexperimenteditor/images/background4.png"), 1, 1);
	backgroundLayout->setRowStretch(2, 10);
    backgroundLayout->setColumnStretch(2, 10);

    QWidget *backgroundWidget = new QWidget;
    backgroundWidget->setLayout(backgroundLayout);

    toolBox = new QToolBox;
    toolBox->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Ignored));
    toolBox->setMinimumWidth(itemWidget->sizeHint().width());
    toolBox->addItem(itemWidget, tr("Basic Flowchart Shapes"));
    toolBox->addItem(backgroundWidget, tr("Backgrounds"));
}

void VisualExperimentEditor::createActions()
{
    toFrontAction = new QAction(QIcon(":/visualexperimenteditor/images/bringtofront.png"), tr("Bring to &Front"), this);
    toFrontAction->setShortcut(tr("Ctrl+F"));
    toFrontAction->setStatusTip(tr("Bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()), this, SLOT(bringToFront()));

    sendBackAction = new QAction(QIcon(":/visualexperimenteditor/images/sendtoback.png"), tr("Send to &Back"), this);
    sendBackAction->setShortcut(tr("Ctrl+B"));
    sendBackAction->setStatusTip(tr("Send item to back"));
    connect(sendBackAction, SIGNAL(triggered()), this, SLOT(sendToBack()));

    deleteAction = new QAction(QIcon(":/visualexperimenteditor/images/delete.png"), tr("&Delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("Delete item from diagram"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setCheckable(true);
    QPixmap pixmap(":/visualexperimenteditor/images/bold.png");
    boldAction->setIcon(QIcon(pixmap));
    boldAction->setShortcut(tr("Ctrl+B"));
    connect(boldAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    italicAction = new QAction(QIcon(":/visualexperimenteditor/images/italic.png"), tr("Italic"), this);
    italicAction->setCheckable(true);
    italicAction->setShortcut(tr("Ctrl+I"));
    connect(italicAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    underlineAction = new QAction(QIcon(":/visualexperimenteditor/images/underline.png"), tr("Underline"), this);
    underlineAction->setCheckable(true);
    underlineAction->setShortcut(tr("Ctrl+U"));
    connect(underlineAction, SIGNAL(triggered()), this, SLOT(handleFontChange()));

    aboutAction = new QAction(tr("A&bout"), this);
    aboutAction->setShortcut(tr("Ctrl+B"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

void VisualExperimentEditor::createMenus()
{
	mainMenuBar = new QMenuBar(this);
	fileMenu = mainMenuBar->addMenu(tr("&File"));
	fileMenu->addAction(exitAction);
    
	itemMenu = mainMenuBar->addMenu(tr("&Item"));
    itemMenu->addAction(deleteAction);
    itemMenu->addSeparator();
    itemMenu->addAction(toFrontAction);
    itemMenu->addAction(sendBackAction);

    aboutMenu = mainMenuBar->addMenu(tr("&Help"));
    aboutMenu->addAction(aboutAction);
}

void VisualExperimentEditor::createToolbars()
{
	mainToolBar = new QToolBar();

	editToolBar = new QToolBar(tr("Edit"));
    editToolBar->addAction(deleteAction);
    editToolBar->addAction(toFrontAction);
    editToolBar->addAction(sendBackAction);
	mainToolBar->addWidget(editToolBar);
	mainToolBar->addSeparator();

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontSizeChanged(QString)));

    fontColorToolButton = new QToolButton;
    fontColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);

    fontColorToolButton->setMenu(createColorMenu(SLOT(textColorChanged()), Qt::black));
    textAction = fontColorToolButton->menu()->defaultAction();
    fontColorToolButton->setIcon(createColorToolButtonIcon(":/visualexperimenteditor/images/textpointer.png", Qt::black));
    fontColorToolButton->setAutoFillBackground(true);
    connect(fontColorToolButton, SIGNAL(clicked()), this, SLOT(textButtonTriggered()));

    fillColorToolButton = new QToolButton;
    fillColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    fillColorToolButton->setMenu(createColorMenu(SLOT(itemColorChanged()), Qt::white));
    fillAction = fillColorToolButton->menu()->defaultAction();
    fillColorToolButton->setIcon(createColorToolButtonIcon(":/visualexperimenteditor/images/floodfill.png", Qt::white));
    connect(fillColorToolButton, SIGNAL(clicked()), this, SLOT(fillButtonTriggered()));

    lineColorToolButton = new QToolButton;
    lineColorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    lineColorToolButton->setMenu(createColorMenu(SLOT(lineColorChanged()), Qt::black));
    lineAction = lineColorToolButton->menu()->defaultAction();
    lineColorToolButton->setIcon(createColorToolButtonIcon(":/visualexperimenteditor/images/linecolor.png", Qt::black));
    connect(lineColorToolButton, SIGNAL(clicked()), this, SLOT(lineButtonTriggered()));

	textToolBar = new QToolBar(tr("Font"));
	textToolBar = new QToolBar(tr("Font"));
	textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
	mainToolBar->addWidget(textToolBar);
	mainToolBar->addSeparator();

	colorToolBar = new QToolBar(tr("Color"));
    colorToolBar->addWidget(fontColorToolButton);
    colorToolBar->addWidget(fillColorToolButton);
    colorToolBar->addWidget(lineColorToolButton);
	mainToolBar->addWidget(colorToolBar);
	mainToolBar->addSeparator();

    QToolButton *pointerButton = new QToolButton;
    pointerButton->setCheckable(true);
    pointerButton->setChecked(true);
    pointerButton->setIcon(QIcon(":/visualexperimenteditor/images/pointer.png"));
    QToolButton *linePointerButton = new QToolButton;
    linePointerButton->setCheckable(true);
    linePointerButton->setIcon(QIcon(":/visualexperimenteditor/images/linepointer.png"));

    pointerTypeGroup = new QButtonGroup(this);
    pointerTypeGroup->addButton(pointerButton, int(DiagramScene::MoveItem));
    pointerTypeGroup->addButton(linePointerButton, int(DiagramScene::InsertLine));
    connect(pointerTypeGroup, SIGNAL(buttonClicked(int)), this, SLOT(pointerGroupClicked(int)));

    sceneScaleCombo = new QComboBox;
    QStringList scales;
    scales << tr("50%") << tr("75%") << tr("100%") << tr("125%") << tr("150%");
    sceneScaleCombo->addItems(scales);
    sceneScaleCombo->setCurrentIndex(2);
    connect(sceneScaleCombo, SIGNAL(currentIndexChanged(QString)), this, SLOT(sceneScaleChanged(QString)));

	pointerToolbar = new QToolBar(tr("Pointer type"));
    pointerToolbar->addWidget(pointerButton);
    pointerToolbar->addWidget(linePointerButton);
    pointerToolbar->addWidget(sceneScaleCombo);
	mainToolBar->addWidget(pointerToolbar);
	//mainToolBar->addSeparator();
}

QWidget *VisualExperimentEditor::createBackgroundCellWidget(const QString &text, const QString &image)
{
    QToolButton *button = new QToolButton;
    button->setText(text);
    button->setIcon(QIcon(image));
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    backgroundButtonGroup->addButton(button);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

QWidget *VisualExperimentEditor::createCellWidget(const QString &text, DiagramItem::DiagramType type)
{
    DiagramItem item(type, itemMenu);
    QIcon icon(item.image());

    QToolButton *button = new QToolButton;
    button->setIcon(icon);
    button->setIconSize(QSize(50, 50));
    button->setCheckable(true);
    buttonGroup->addButton(button, int(type));

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(button, 0, 0, Qt::AlignHCenter);
    layout->addWidget(new QLabel(text), 1, 0, Qt::AlignCenter);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);
    return widget;
}

QMenu *VisualExperimentEditor::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black << Qt::white << Qt::red << Qt::blue << Qt::yellow;
    QStringList names;
    names << tr("black") << tr("white") << tr("red") << tr("blue") << tr("yellow");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()), this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor)
            colorMenu->setDefaultAction(action);
    }
    return colorMenu;
}

QIcon VisualExperimentEditor::createColorToolButtonIcon(const QString &imageFile, QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPixmap image(imageFile);
    // Draw icon centered horizontally on button.
    QRect target(4, 0, 42, 43);
    QRect source(0, 0, 42, 43);
    painter.fillRect(QRect(0, 60, 50, 80), color);
    painter.drawPixmap(target, image, source);
    return QIcon(pixmap);
}

QIcon VisualExperimentEditor::createColorIcon(QColor color)
{
    QPixmap pixmap(20, 20);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 20, 20), color);
    return QIcon(pixmap);
}

bool VisualExperimentEditor::parseExperimentStructure(cExperimentStructure *ExpStruct)
{//Make sure to first call the above install()!
	if(gScene == NULL)
		return false;
	gScene->clear();
	
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
					ExperimentGraphBlock *gBlock = new ExperimentGraphBlock(NULL);
					gScene->addItem(gBlock);
					gBlock->setName(tmpBlock->getBlockName());
					gBlock->setID(tmpBlock->getBlockID());
					gBlock->addPort(tmpBlock->getBlockName(), false, ExperimentGraphPort::NamePort);
					gBlock->addPort(QString::number(tmpBlock->getBlockID()) + ": Blocknumber " + QString::number(tmpBlock->getBlockNumber()), false, ExperimentGraphPort::TypePort);
					gBlock->addInputPort("In");
					gBlock->addOutputPort("Out");
					tmpRect = gBlock->boundingRect();
					gBlock->setPos(dLeftCanvasMargin + (i*dBlockDistance) + tmpRect.width()/2, dTopCanvasMargin + tmpRect.height()/2);
					mBlockPositions.insert(gBlock->getID(),gBlock->pos());
					if((tmpRect.x() + tmpRect.width() + (dLeftCanvasMargin*2)) > rectNeededSize.width())
						rectNeededSize.setWidth(tmpRect.x() + tmpRect.width() + (dLeftCanvasMargin*2));
					if((tmpRect.y() + tmpRect.height() + (dTopCanvasMargin*2)) > rectNeededSize.height())
						rectNeededSize.setHeight(tmpRect.y() + tmpRect.height()  + (dTopCanvasMargin*2));
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
	//QList<QGraphicsItem*> items = gScene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));
	//foreach(QGraphicsItem *item, items)
	//	if (item->type() > QGraphicsItem::UserType)
	//gScene->setSceneRect(QRectF(0, 0, 50, 50));
	//rectNeededSize
	gView->ensureVisible(rectNeededSize, dLeftCanvasMargin, dTopCanvasMargin);
	gView->centerOn(rectNeededSize.x()/2,rectNeededSize.y()/2);
	return true;
}

bool VisualExperimentEditor::createConnection(QGraphicsItem *from, QGraphicsItem *to, int nRepeats)
{
	Q_UNUSED(nRepeats);
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

QGraphicsItem* VisualExperimentEditor::itemAt(const QPointF &pos)
{
	QList<QGraphicsItem*> items = gScene->items(QRectF(pos - QPointF(1,1), QSize(3,3)));

	foreach(QGraphicsItem *item, items)
		if (item->type() > QGraphicsItem::UserType)
			return item;

	return 0;
}

void VisualExperimentEditor::save(QDataStream &ds)
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

void VisualExperimentEditor::load(QDataStream &ds)
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