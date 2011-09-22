//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef ExperimentManager_H
#define ExperimentManager_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QGraphicsView>
#include <QGLWidget>
#include <QDesktopWidget>
#include <QApplication>
#include "experimenttree.h"
#include "Global.h"
#include "retinomap_glwidget.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif

//#include "ThreadedGLWidgetWrapper.h"
//#include "glwidget.h"

//class GLWidgetWrapper;
class RetinoMap_glwidget;
class ExperimentTree;

class ExperimentManager : public QObject, protected QScriptable
{
	Q_OBJECT

signals:
	void ExperimentStateHasChanged(int nExperimentMainState, QString timeTextStamp);//Should be type of ExperimentMainState

public:
	ExperimentManager(QObject *parent = 0);
	~ExperimentManager();

	typedef struct{
		int nObjectID;
		int nMetaID;
		QObject *pObject;
		QString sObjectName;
		ExperimentObjectState nState;
	} objectElement;

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);
	bool cleanupExperiment();

public slots:
	void changeToOpenGLView(QGraphicsView *GraphView);
	bool setExperimentFileName(const QString qstrExpFileName);
	QString getExperimentFileName();
	bool openExperiment(QString strFile = "", bool bViewEditTree = true);
	bool saveExperiment(QString strFile = "");
	bool runExperiment();
	void abortExperiment();
	void changeExperimentObjectState(ExperimentObjectState nState);
	bool setFullScreenMode(const bool bFullScreen);
	bool setDebugMode(const bool bDebugMode);
	QString getCurrentDateTimeStamp();

private:
	void RegisterMetaTypes();
	bool invokeExperimentObjectsSlots(const QString &sSlotName);
	bool createExperimentObjects();
	bool startExperimentObjects(bool bRunFullScreen = true);
	bool stopExperimentObjects();
	bool changeExperimentObjectsSignalSlots(bool bDisconnect = false, int nSpecificIndex = -1);
	void cleanupExperimentObjects();

	QDomNodeList ExperimentObjectDomNodeList;
	QDomNodeList ExperimentBlockTrialsDomNodeList;
	QList<objectElement> lExperimentObjectList;

//	short m_ExampleProperty;
	bool m_RunFullScreen;
	bool m_DebugMode;
	QString m_ExpFileName;
	QString m_ExpFolder;
	ExperimentTree *currentExperimentTree;
};

class SleeperThread : public QThread
{
public:
	static void msleep(unsigned long msecs)
	{
		QThread::msleep(msecs);
	}
};

class ThreadedContainerDlg : public QDialog
{
	Q_OBJECT

public:
	ThreadedContainerDlg(QWidget *parent = NULL);
	ThreadedContainerDlg::~ThreadedContainerDlg();

	private slots:
		void reject();

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // ExperimentManager_H
