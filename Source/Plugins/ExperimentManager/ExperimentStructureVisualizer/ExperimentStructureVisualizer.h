#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "QGVScene.h"

class QMenu;
class QToolBar;
class QToolButton;
class QVBoxLayout;
class QGraphicsViewEc;
class cExperimentStructure;

class ExperimentStructureVisualizer : public QWidget
{
    Q_OBJECT

signals:
	void destroyed(QWidget*);

public:
    explicit ExperimentStructureVisualizer(QWidget *parent = 0);
    ~ExperimentStructureVisualizer();

	bool parseExperimentStructure(cExperimentStructure *ExpStruct);

private slots:
    void nodeContextMenu(QGVNode* node);
    void nodeDoubleClick(QGVNode* node);

private:
	void drawGraph();
	void configureActions(bool bCreate);
	void setupMenuAndActions();
	void setupLayout();

	QVBoxLayout *mainLayout;
	QAction *action_Quit;
	QToolBar *toolBar;
	QMenu *menuFile;
	QMenu *menuEdit;
	QToolButton *buttonFile;
	//QToolButton *buttonEdit;

	QGraphicsViewEc *_view;
    QGVScene *_scene;
};

#endif // MAINWINDOW_H
