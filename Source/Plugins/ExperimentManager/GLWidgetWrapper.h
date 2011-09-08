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

class GLWidgetWrapper : public QGLWidget
{
	Q_OBJECT

signals:
	void UserWantsToClose(void);
	void StateHasChanged(ExperimentObjectState);

public:
	GLWidgetWrapper(QWidget *parent = NULL, QGLFormat fmt = GLWidget_Global::getDefaultGLFormat());
	~GLWidgetWrapper();

	void setBlockTrials();

public slots:
	//Can be overridden
	virtual bool start();
	virtual bool stop();
	void setDebugMode(bool bMode);

protected:
	bool checkForNextBlockTrial();
	void finalizePaintEvent();
	void SetupLayout(QWidget* layoutWidget);

	//Can be overridden
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

protected:
	bool bForceToStop;
	bool bDebugMode;
	int nElapsedFrameTime;
	int nPaintUpdateTime;
	QTime tFrameTime;
	QRectF rScreenResolution;
	int nMinScreenUpdateTime;
	int nFrameCounter;
	QTime totalRunningTime;
	ContainerDlg *stimContainerDlg;
	QVBoxLayout *mainLayout;

private:
	QTimer tTriggerTimer;
	QTimer tStimTimer;
	QDesktopWidget* dDesktopWidget;
	int currentBlockTrial;
	int completedTR;
	int m_TriggerCount;
	int nextTimeThresholdTRs;
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
