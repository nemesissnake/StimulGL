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
	explicit ExperimentStructureVisualizer(QWidget *parent = 0);
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
	int nWidgetMargin;
	qreal dGraphViewScale;
};

#endif // EXPERIMENTSTRUCTUREVISUALIZER_H
