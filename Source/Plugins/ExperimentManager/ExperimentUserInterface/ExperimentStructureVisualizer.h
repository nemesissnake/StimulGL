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
#include "QGVScene.h"

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
	void nodeContextMenu(QGVNode* node);
	void nodeDoubleClick(QGVNode* node);
	void Test();

//protected:
	//void showEvent ( QShowEvent * event );
	//void resizeEvent(QResizeEvent * event);

private:
	Ui::ExperimentStructureVisualizer *ui;

	//enum AutoConnType
	//{
	//	AutoConnType_BlockToBlock	= 0,
	//	AutoConnType_StartToBlock	= 1,
	//	AutoConnType_BlockToEnd		= 2
	//};

	struct expLoopItemStrc 
	{
		QString sName;
		int nId;
		int nNumber;
		int nNumberOfLoops;
		QGVEdge *gvEdge;
		int nSourceBlockId;
		int nTargetBlockId;
	};	

	struct expBlockItemStrc 
	{
		QString sName;
		int nId;
		int nNumber;
		QGVNode *gvNode;
	};

	struct expAutoConnItemStrc
	{
		QGVEdge *gvEdge;
		//AutoConnType tConnType;
		int nSourceBlockId;
		int nTargetBlockId;
	};

	struct expSceneItemStrc 
	{
		QString sExperimentName;
		QList<expBlockItemStrc> lBlocks;
		QList<expLoopItemStrc> lLoops;
		QList<expAutoConnItemStrc> lAutoConns;
		QGVNode *gvStartExperimentNode;
		QGVNode *gvEndExperimentNode;
	};

	bool drawGraph(const QString &sDotContent = "");
	void configureActions(bool bCreate);
	void setupMenuAndActions();
	void createScene();
	void setupLayout();
	void resetExpSceneItemsCollection(expSceneItemStrc &tmpExpSceneItems);
	QGVNode *getGVNodePointer(const int &nBlockID);

	QAction *action_Quit;
	QAction *action_Test;
	QToolBar *toolBar;
	QMenu *menuFile;
	QMenu *menuEdit;
	QToolButton *buttonFile;
	QToolButton *buttonEdit;
	QVBoxLayout *graphViewLayout;

	QGVScene *_scene;
	expSceneItemStrc expSceneItems;
	cExperimentStructure *parsedExpStruct;
	int nWidgetMargin;
	qreal dGraphViewScale;
	bool bDrawVertical; //Otherwise horizontal
};

#endif // EXPERIMENTSTRUCTUREVISUALIZER_H
