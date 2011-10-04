#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QDialog>
#include <QGLWidget>
#include <QTimer>
#include <QTime>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QBoxLayout>
#include <QCustomEvent>
#include "Global.h"

class QPaintEvent;
class QWidget;
class ContainerDlg;
class QDomNodeList;

class GLWidgetWrapper : public QGLWidget
{
	Q_OBJECT

signals:
	void UserWantsToClose(void);
	void ObjectShouldStop(void);
	void ObjectStateHasChanged(ExperimentSubObjectState);

public:
	GLWidgetWrapper(QWidget *parent = NULL);
	~GLWidgetWrapper();

	void setBlockTrials();

public slots:
	//Can be overridden
	virtual bool start();
	virtual bool stop();
	virtual bool abort();
	virtual bool setBlockTrialDomNodeList(QDomNodeList *pExpBlockTrialDomNodeList = NULL);
	virtual bool setObjectID(int nObjID);
	virtual bool setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct = NULL);
	
	QRectF getScreenResolution();
	int getObjectID();	

protected:
	bool checkForNextBlockTrial();
	bool getExperimentBlockParameter(int nBlockNumber, int nObjectID, QString strParamName, QString &Result);
	void finalizePaintEvent();
	void setupLayout(QWidget* layoutWidget);	
	bool isDebugMode();
	int getFrameNumber() {return nFrameCounter;}
	int getCurrentExperimentTrial() {return nCurrentExperimentTrial;}
	int getCurrentExperimentBlock() {return nCurrentExperimentBlock;}
	int getCurrentExperimentTriggers() {return nCurrentExperimentReceivedTriggers;}
	int getElapsedFrameTime() {return nElapsedFrameTime;}
	//bool getExperimentTriggerCount(int &nExperimentTriggerCount);

	void closeEvent(QCloseEvent *evt);
	void customEvent(QEvent *event);

	virtual void init();
	virtual void initBlockTrial();
	//virtual bool loadBlockTrial();
	virtual void paintEvent(QPaintEvent *event);

protected slots:
	void incrementTrigger();
	void animate();

private:
	void startTriggerTimer(int msTime);
	void stopTriggerTimer();
	bool eventFilter(QObject *target, QEvent *event);

	bool updateExperimentBlockTrialStructure();
	bool cleanupExperimentBlockTrialStructure();
	void changeSubObjectState(ExperimentSubObjectState newSubObjectState);
	ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;}

private:
	int nFrameCounter;
	int nCurrentExperimentReceivedTriggers;				//The current experiment number of trigger received since it started
	int nCurrentExperimentTrial;						//The current experiment trial within the block 
	int nCurrentExperimentBlock;						//The current experiment block
	int nTotalProcessedExperimentTrials;				//The total number of trials processed within experiment, might be that this Trial is not fully processed.
	int nElapsedFrameTime;
	ContainerDlg *stimContainerDlg;
	bool bForceToStop;
	bool bExperimentShouldStop;
	int nPaintUpdateTime;
	QTime tFrameTime;
	QRectF rScreenResolution;
	int nMinScreenUpdateTime;
	QTime tTotalRunningTime;
	QVBoxLayout *mainLayout;
	int nObjectID;
	QTimer tTriggerTimer;
	QTimer tStimTimer;
	QDomNodeList *pExpBlockTrialDomNodeList;
	//int nTriggerCount;
	int nNextThresholdTriggerCount;//When we should switch to the next block
	ExperimentBlockTrialStructure strcExperimentBlockTrials;
	ExperimentConfiguration *pExpConf;
	QEvent::Type tEventObjectStopped;
	ExperimentSubObjectState currentSubObjectState;
};

class ContainerDlg : public QDialog
{
	Q_OBJECT

public:
	ContainerDlg(QWidget *parent = NULL);
	ContainerDlg::~ContainerDlg();

	private slots:
		void reject();

protected:
	void closeEvent(QCloseEvent *e);
};



#endif // GLWIDGET_H
