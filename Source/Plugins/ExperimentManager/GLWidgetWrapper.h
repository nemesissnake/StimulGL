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
	void setDebugMode(bool bMode);
	virtual bool setBlockTrialDomNodeList(QDomNodeList *pExpBlockTrialDomNodeList);

protected:
	bool checkForNextBlockTrial();
	bool getBlockTrialParameter(int nBlockNumber, int nObjectID, QString strParamName, QString &Result);

	void finalizePaintEvent();
	void SetupLayout(QWidget* layoutWidget);
	
	void closeEvent(QCloseEvent *evt);

	//Can be overridden
	virtual void init();
	virtual void initBlockTrial();
	virtual bool loadBlockTrial();
	virtual void paintEvent(QPaintEvent *event);

	//Example using standard OpenGL rendering commands...
	//void initializeGL()
	//{
	//	// Set up the rendering context, define display lists etc.:
	//	...
	//		glClearColor(0.0, 0.0, 0.0, 0.0);
	//	glEnable(GL_DEPTH_TEST);
	//	...
	//}

	//void resizeGL(int w, int h)
	//{
	//	// setup viewport, projection etc.:
	//	glViewport(0, 0, (GLint)w, (GLint)h);
	//	...
	//		glFrustum(...);
	//	...
	//}

	//void paintGL()
	//{
	//	// draw the scene:
	//	...
	//		glRotatef(...);
	//	glMaterialfv(...);
	//	glBegin(GL_QUADS);
	//	glVertex3f(...);
	//	glVertex3f(...);
	//	...
	//		glEnd();
	//	...
	//}
	//-->updateGL() function
	//see http://doc.qt.nokia.com/stable/qglwidget.html#details

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
	QDomNodeList *pExpBlockTrialDomNodeList;
	int currentBlockTrial;
	int currentBlock;
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
