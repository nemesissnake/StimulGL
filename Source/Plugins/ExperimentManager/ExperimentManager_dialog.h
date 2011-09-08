//This file implements the UI functionality

#ifndef ExperimentManager_DIALOG_H
#define ExperimentManager_DIALOG_H

#include <QDialog>
#include "ui_ExperimentManager_dialog.h"
#include <QtGui>
#include <Windows.h>
#include "stimulglgraphscene.h"

class StimulGLGraphScene;
class ExperimentManager_Dialog : public QDialog, private Ui::ExperimentManager_DialogClass
{
	Q_OBJECT

public:
	ExperimentManager_Dialog(QWidget *parent = 0);
	~ExperimentManager_Dialog();

private:
	Ui::ExperimentManager_DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void on_okButton_clicked();
	//void on_GraphScene_Closed();

private:
	StimulGLGraphScene *StimGLScene;
	QGraphicsView *GraphView;
};

#endif // ExperimentManager_DIALOG_H
