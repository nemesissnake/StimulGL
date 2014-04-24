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

#ifndef EXPERIMENTSTRUCTUREVISUALIZER_H
#define EXPERIMENTSTRUCTUREVISUALIZER_H

#include <QWidget>
#include "ExperimentStructureScene.h"
#include "ExperimentGraphBlockItem.h"
#include "ExperimentGraphConnectionItem.h"

#define EXPGRAPH_AUTOCONN_DISTANCE		100.0
#define EXPGRAPH_AUTOCONN_TOOLTIP_TEXT	"Auto-connection"

class QMenu;
class QToolBar;
class QToolButton;
class QVBoxLayout;
class QGraphicsViewEc;
class cExperimentStructure;

namespace Ui {class ExperimentStructureVisualizer;};

class ExperimentStructureVisualizer : public QWidget
{
	Q_OBJECT

signals:
	void destroyed(QWidget*);

public:
	explicit ExperimentStructureVisualizer(QWidget *parent = NULL);
	~ExperimentStructureVisualizer();

	bool parseExperimentStructure(cExperimentStructure *ExpStruct);

public slots:
	void resizeStructureView(const int &nWidth, const int &nHeight);

private slots:
	//void nodeContextMenu(QGVNode* node);
	//void nodeDoubleClick(QGVNode* node);
	void Test();

//protected:
	//void showEvent ( QShowEvent * event );
	//void resizeEvent(QResizeEvent * event);

private:
	Ui::ExperimentStructureVisualizer *ui;

	struct expLoopItemStrc 
	{
		QString sName;
		int nId;
		int nNumber;
		int nNumberOfLoops;
		int nSourceBlockId;
		int nTargetBlockId;
	};	

	struct expBlockItemStrc 
	{
		QString sName;
		int nId;
		int nNumber;
		ExperimentGraphBlockItem *gGraphBlockItem;
		//QGraphicsTextItem *gTextItem;
		expBlockItemStrc()
		{
			sName = "";
			nId = -1;
			nNumber = -1;
			gGraphBlockItem = NULL;
			//gTextItem = NULL;
		}
	};

	struct expConnItemStrc
	{
		int nLoopId;
		ExperimentGraphConnectionItem *gGraphConnectionItem;
		int nSourceBlockId;
		int nTargetBlockId;
		expConnItemStrc()
		{
			nLoopId = -1;
			gGraphConnectionItem = NULL;
			nSourceBlockId = -1;
			nTargetBlockId = -1;
		}
	};

	struct expSceneItemStrc 
	{
		QString sExperimentName;
		QList<expBlockItemStrc> lBlocks;
		QList<expLoopItemStrc> lLoops;
		QList<expConnItemStrc> lAutoConns;
		ExperimentGraphBlockItem *gStartGraphBlockItem;
		ExperimentGraphBlockItem *gEndGraphBlockItem;
		expSceneItemStrc()
		{
			gStartGraphBlockItem = NULL;
			gEndGraphBlockItem = NULL;
		}
	};

	struct expParsedLoopDrawing
	{		
		QHash<int, QList<int>> hBlockIDTargetLoopBlockIDs;
		QList<expConnItemStrc> lMasterSideDrawOrderedLoops;
		QHash<int, int> hBlockIDMasterSideCount;
		QList<expConnItemStrc> lSlaveSideDrawOrderedLoops;
		QHash<int, int> hBlockIDSlaveSideCount;
	};

	bool drawGraph(const QString &sDotContent = "");
	void configureActions(bool bCreate);
	void setupMenuAndActions();
	void createScene();
	void setupLayout();
	void resetExpScene();
	ExperimentGraphBlockItem *getGraphBlockItemPointer(const int &nBlockID);
	bool insertLoopInGraphDrawingStruct(const int &nSourceBlockID, const int &nTargetBlockID);

	int nWidgetMargin;
	qreal dGraphViewScale;
	bool bDrawVertical; //Otherwise horizontal
	QAction *action_Quit;
	QAction *action_Test;
	QToolBar *toolBar;
	QMenu *menuFile;
	QMenu *menuEdit;
	QToolButton *buttonFile;
	QToolButton *buttonEdit;
	QVBoxLayout *graphViewLayout;
	ExperimentStructureScene *gScene;
	cExperimentStructure *parsedExpStruct;
	expSceneItemStrc expSceneItems;
	expParsedLoopDrawing expLoopDrawing;
};

#endif // EXPERIMENTSTRUCTUREVISUALIZER_H
