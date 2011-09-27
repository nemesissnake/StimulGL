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
	void WidgetStateHasChanged(ExperimentObjectState);

public:
	GLWidgetWrapper(QWidget *parent = NULL);
	~GLWidgetWrapper();

	void setBlockTrials();

public slots:
	//Can be overridden
	virtual bool start();
	virtual bool stop();
	virtual bool setBlockTrialDomNodeList(QDomNodeList *pExpBlockTrialDomNodeList);
	virtual bool setObjectID(int nObjID);
	
	void setDebugMode(bool bMode);
	QRectF getScreenResolution();
	int getObjectID();	

protected:
	bool checkForNextBlockTrial();
	bool getBlockTrialParameter(int nBlockNumber, int nObjectID, QString strParamName, QString &Result);
	void finalizePaintEvent();
	void SetupLayout(QWidget* layoutWidget);	
	void closeEvent(QCloseEvent *evt);
	bool isDebugMode() {return bDebugMode;}
	int getFrameNumber() {return nFrameCounter;}
	int getCurrentBlockTrial() {return nCurrentBlockTrial;}
	int getElapsedFrameTime() {return nElapsedFrameTime;}
	int getCompletedTriggers() {return nCompletedTriggers;}

	virtual void init();
	virtual void initBlockTrial();
	virtual bool loadBlockTrial();
	virtual void paintEvent(QPaintEvent *event);

protected slots:
	void incrementTriggerCount();
	void animate();

private:
	void startTriggerTimer(int msTime);
	void stopTriggerTimer();
	bool eventFilter(QObject *target, QEvent *event);
	bool updateExperimentBlockTrialStructure();
	bool cleanupExperimentBlockTrialStructure();

private:
	bool bDebugMode;
	int nFrameCounter;
	int nCurrentBlockTrial;
	int nElapsedFrameTime;
	int nCompletedTriggers;
	ContainerDlg *stimContainerDlg;
	bool bForceToStop;
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
	int nTriggerCount;
	int nNextTimeThresholdTRs;
	ExperimentBlockTrialStructure strcExperimentBlockTrials;
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
