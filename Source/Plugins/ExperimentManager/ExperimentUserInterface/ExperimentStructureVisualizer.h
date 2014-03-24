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
#include <QGraphicsScene>
#include <OGDF/basic/Graph.h>
#include <OGDF/basic/Graph_d.h>
#include <OGDF/basic/graph_generators.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/layered/DfsAcyclicSubgraph.h>
#include <ogdf/tree/TreeLayout.h>

using namespace ogdf;

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
		ogdf::edge *gvEdge;
		int nSourceBlockId;
		int nTargetBlockId;
	};	

	struct expBlockItemStrc 
	{
		QString sName;
		int nId;
		int nNumber;
		ogdf::node *gvNode;
	};

	struct expAutoConnItemStrc
	{
		ogdf::edge gvEdge;
		int nSourceBlockId;
		int nTargetBlockId;
	};

	struct expSceneItemStrc 
	{
		QString sExperimentName;
		QList<expBlockItemStrc> lBlocks;
		QList<expLoopItemStrc> lLoops;
		QList<expAutoConnItemStrc> lAutoConns;
		ogdf::node gvStartExperimentNode;
		ogdf::node gvEndExperimentNode;
	};

	bool drawGraph(const QString &sDotContent = "");
	void configureActions(bool bCreate);
	void setupMenuAndActions();
	void createScene();
	void setupLayout();
	void resetExpSceneItemsCollection(expSceneItemStrc &tmpExpSceneItems);
	ogdf::node *getGVNodePointer(const int &nBlockID);

	QAction *action_Quit;
	QAction *action_Test;
	QToolBar *toolBar;
	QMenu *menuFile;
	QMenu *menuEdit;
	QToolButton *buttonFile;
	QToolButton *buttonEdit;
	QVBoxLayout *graphViewLayout;
	ogdf::Graph graph;
	expSceneItemStrc expSceneItems;
	QGraphicsScene *gScene;
	GraphAttributes *gAttr;
	cExperimentStructure *parsedExpStruct;
	int nWidgetMargin;
	qreal dGraphViewScale;
	bool bDrawVertical; //Otherwise horizontal
};

#endif // EXPERIMENTSTRUCTUREVISUALIZER_H
