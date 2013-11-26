#ifndef TESTVIEW_H
#define TESTVIEW_H

#include <QWidget>
#include "QGVScene.h"

class QMenu;
class QToolBar;
class QToolButton;
class QVBoxLayout;
class QGraphicsViewEc;
class cExperimentStructure;

namespace Ui {class testView;};

class testView : public QWidget
{
	Q_OBJECT

signals:
	void destroyed(QWidget*);

public:
	explicit testView(QWidget *parent = 0);
	~testView();

	bool parseExperimentStructure(cExperimentStructure *ExpStruct);

private slots:
	void nodeContextMenu(QGVNode* node);
	void nodeDoubleClick(QGVNode* node);
	void Test();

protected:
	void showEvent ( QShowEvent * event );

private:
	Ui::testView *ui;

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

	struct expSceneItemStrc 
	{
		QString sExperimentName;
		QList<expBlockItemStrc> lBlocks;
		QList<expLoopItemStrc> lLoops;
	};

	bool drawGraph();
	void configureActions(bool bCreate);
	void setupMenuAndActions();
	void setupLayout();
	void resetExpSceneItemsCollection(expSceneItemStrc &tmpExpSceneItems);
	QGVNode *getGVNodePointer(const int &nTargetBlockID);

	QAction *action_Quit;
	QAction *action_Test;
	QToolBar *toolBar;
	QMenu *menuFile;
	QMenu *menuEdit;
	QToolButton *buttonFile;
	QToolButton *buttonEdit;

	QGVScene *_scene;
	expSceneItemStrc expSceneItems;
};

#endif // TESTVIEW_H
