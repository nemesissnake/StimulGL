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
#include "mainappinfo.h"
#include "experimentlogger.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class RetinoMap_glwidget;
class ExperimentTree;

class ExperimentManager : public QObject, protected QScriptable
{
	Q_OBJECT

signals:
	void ExperimentStateHasChanged(int nExperimentMainState, QString timeTextStamp);//Should be type of ExperimentMainState
	void WriteToLogOutput(const QString &strText2Write);

public:
	ExperimentManager(QObject *parent = NULL);
	~ExperimentManager();

	typedef struct{
		int nObjectID;
		int nMetaID;
		QObject *pObject;
		QString sObjectName;
		ExperimentSubObjectState nCurrentState;
		QHash<QString, QString> *ExpBlockParams;
		//objectStateHistory sStateHistory;
	} objectElement;

	static QScriptValue ctor__experimentManager(QScriptContext* context, QScriptEngine* engine);
	bool cleanupExperiment();
	QHash<QString, QString> *getObjectBlockParamListById(int nID);

public slots:
	void changeToOpenGLView(QGraphicsView *GraphView);
	bool setExperimentFileName(const QString qstrExpFileName);
	QString getExperimentFileName();
	bool openExperiment(QString strFile = "", bool bViewEditTree = true);
	bool saveExperiment(QString strFile = "");
	bool runExperiment();
	void abortExperiment();
	void stopExperiment();
	void changeExperimentSubObjectState(ExperimentSubObjectState nState);
	bool setFullScreenMode(const bool bFullScreen);
	bool setDebugMode(const bool bDebugMode);
	QString getCurrentDateTimeStamp();
	bool insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue);
	bool getExperimentObjectBlockParameter(const int nObjectID,const QString sName, QString &sValue);
	bool setExperimentObjectBlockParameterStructure(const int nObjectID, QHash<QString, QString> *expBlockTrialStruct);
	
	bool logExperimentObjectData(const int &nObjectIndex, const int &nTimerIndex, const QString &strFunction = "", const QString &strTag = "", const QString &strMessage = "", const QString &strValue = "");
	int createExperimentTimer();
	bool startExperimentTimer(int nIndex);
	double restartExperimentTimer(int nIndex);
	double elapsedExperimentTimerTime(int nIndex);

private:
	bool WriteAndCloseExperimentOutputData();
	void initializeDataLogger();
	void RegisterMetaTypes();
	bool invokeExperimentObjectsSlots(const QString &sSlotName);
	bool configureExperiment();
	bool createExperimentObjects();
	bool connectExperimentObjects(bool bDisconnect = false, int nObjectID = -1);
	bool initializeExperiment(bool bFinalize = false);
	bool finalizeExperimentObjects();
	bool startExperimentObjects(bool bRunFullScreen = true);
	bool initExperimentObjects();
	bool stopExperimentObjects();
	bool abortExperimentObjects();
	bool changeExperimentObjectsSignalSlots(bool bDisconnect = false, int nSpecificIndex = -1);
	void cleanupExperimentObjects();
	void changeCurrentExperimentState(ExperimentState expCurrState);
	QObject *getObjectElementById(int nID);
	ExperimentState getCurrentExperimentState() {return experimentCurrentState;}
	//bool getGenericArgument(QString strMetaType,QString strValue, QGenericArgument &genArg);
	//QGenericArgument getGenericArgument(QString *strMetaType,QString *strValue, bool &bSucceeded);

	QDomNodeList ExperimentObjectDomNodeList;
	QDomNodeList ExperimentBlockTrialsDomNodeList;
	QList<objectElement> lExperimentObjectList;

//	short m_ExampleProperty;
	ExperimentState experimentCurrentState;
	bool m_RunFullScreen;
	bool m_DebugMode;
	QString m_ExpFileName;
	QString m_ExpFolder;
	ExperimentTree *currentExperimentTree;	
	ExperimentConfiguration strcExperimentConfiguration;

	ExperimentLogger *expDataLogger;
	int nExperimentTimerIndex;
};

class SleeperThread : public QThread
{
public:
	static void msleep(unsigned long msecs)
	{
		QThread::msleep(msecs);
	}
};

//class ThreadedContainerDlg : public QDialog
//{
//	Q_OBJECT
//
//public:
//	ThreadedContainerDlg(QWidget *parent = NULL);
//	ThreadedContainerDlg::~ThreadedContainerDlg();
//
//	private slots:
//		void reject();
//
//protected:
//	void closeEvent(QCloseEvent *e);
//};

#endif // ExperimentManager_H
