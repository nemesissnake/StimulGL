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
	void LogToOutputWindow(const QString &strText2Write);

public:
	GLWidgetWrapper(QWidget *parent = NULL);
	~GLWidgetWrapper();

	void setBlockTrials();

public slots:
	//Can be overridden
	virtual bool initExperimentObject();
	virtual bool startExperimentObject();
	virtual bool stopExperimentObject();
	virtual bool abortExperimentObject();
	virtual bool setBlockTrialDomNodeList(QDomNodeList *pExpBlockTrialDomNodeList = NULL);
	virtual bool setExperimentObjectID(int nObjID);
	virtual bool setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct = NULL);
	
	QRectF getScreenResolution();
	int getObjectID();
	bool insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue);
	QString getExperimentObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue);

protected:
	bool checkForNextBlockTrial();
	bool getExperimentBlockParameter(int nBlockNumber, int nObjectID, QString strParamName, QString &Result);
	bool getExperimentBlockParameters(int nBlockNumber, int nObjectID, QHash<QString, QString> *hParams = NULL);
	void finalizePaintEvent();
	void setupLayout(QWidget* layoutWidget);	
	bool isDebugMode();
	int getCurrentExperimentBlockTrialFrame() {return nBlockTrialFrameCounter;}
	int getCurrentExperimentTrial() {return nCurrentExperimentTrial;}
	int getCurrentExperimentBlock() {return nCurrentExperimentBlock;}
	int getCurrentExperimentTrigger() {return nCurrentExperimentTrigger;}
	int getCurrentExperimentBlockTrialTrigger() {return nCurrentExperimentBlockTrialReceivedTriggers;}
	int getExperimentBlockTrialTriggerAmount(int nBlock, int nTrial);
	//int getElapsedFrameTime();
	QString getLastLoggedObjectStateTime(ExperimentSubObjectState state);

	void closeEvent(QCloseEvent *evt);
	void customEvent(QEvent *event);

	virtual void initBlockTrial();
	//virtual bool loadBlockTrial();
	virtual void paintEvent(QPaintEvent *event);

protected slots:
	void incrementTrigger();
	void animate();

private:
	//void startTriggerTimer(int msTime);
	//void stopTriggerTimer();
	void init();
	bool eventFilter(QObject *target, QEvent *event);
	int getRelativeBlockTrialTrigger(int nAbsoluteTrigger);		//Returns the relative (within a BlockTrial) trigger from a absolute (within the whole Experiment) trigger
	bool updateExperimentBlockTrialStructure();
	bool cleanupExperimentBlockTrialStructure();
	void changeSubObjectState(ExperimentSubObjectState newSubObjectState);
	ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;}

private:
	int nBlockTrialFrameCounter;
	int nCurrentExperimentReceivedTriggers;				//The current experiment number of trigger received since it started, internal use!
	int nCurrentExperimentTrigger;						//The current experiment trigger
	int nCurrentExperimentTrial;						//The current experiment trial within the block 
	int nCurrentExperimentBlockTrialReceivedTriggers;	//The current experiment number of trigger received within the current block trial
	int nCurrentExperimentBlock;						//The current experiment block
	int nTotalProcessedExperimentTrials;				//The total number of trials processed within experiment, might be that this Trial is not fully processed.
	ContainerDlg *stimContainerDlg;
	bool bForceToStop;
	bool bExperimentShouldStop;
	QRectF rScreenResolution;
	int nMinScreenUpdateTime;
	QVBoxLayout *mainLayout;
	int nObjectID;
	QTimer tStimTimer;
	QDomNodeList *pExpBlockTrialDomNodeList;
	int nNextThresholdTriggerCount;//When we should switch to the next block
	ExperimentBlockTrialStructure strcExperimentBlockTrials;
	QEvent::Type tEventObjectStopped;
	ExperimentSubObjectState currentSubObjectState;
	ExperimentSubObjectStateHistory subObjectStateHistory;
	int nFrameTimerIndex;
	int nTrialTimerIndex;
	QHash<QString, QString> *ExpBlockParams;
	ExperimentConfiguration *pExpConf;
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
