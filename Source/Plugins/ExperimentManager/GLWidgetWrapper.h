//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QDialog>
#include <QGLWidget>
#include <QTimer>
#include <QDesktopWidget>
#include <QPaintEvent>
//123 #include <QKeyEvent>
#include <QBoxLayout>
#include <QEvent>
#include <QMutex>
#include <QScriptValue>
#include <omp.h>
//123 #include "ExperimentManager.h"
#include "Global.h"
#include "ContainerDlg.h"
#include "ExperimentEngine.h"
//123 #include "ExperimentTimer.h"

//123 using namespace ExperimentManagerNameSpace;

#define GLWWRAP_WIDGET_STIMULI_REFRESHRATE	"stimulirefreshrate"

enum GLWidgetPaintFlags //An optional paint flag that can be forwarded to the FUNC_PAINTOBJECT_FULL routine 
{
	GLWidgetPaintFlags_NoFlag		= 0,
	GLWidgetPaintFlags_LockedState	= 1,
	GLWidgetPaintFlags_Reserved1	= 2, //This one is reserved for future implementation
	GLWidgetPaintFlags_Reserved2	= 4, //This one is reserved for future implementation
	GLWidgetPaintFlags_Reserved3	= 8, //This one is reserved for future implementation
	GLWidgetPaintFlags_Reserved4	= 16 //This one is reserved for future implementation
};

class QPaintEvent;
class QWidget;
class ContainerDlg;
//123 class QDomNodeList;

//!  The GLWidgetWrapper class. 
/*!
  The GLWidgetWrapper class can be derived from to create custom classes that can again be used in combination with the ExperimentManager for presenting visual or other kind of stimuli.
  This class is optimized for various painting operations supported by OpenGL.
*/
class GLWidgetWrapper : public ExperimentEngine//123 , public QGLWidget
{
	Q_OBJECT

signals:
	//! The UserWantsToClose Signal.
	/*!
		You can use this Signal to detect whenever the user tries to abort the experiment.
		No Parameter.
	*/
//123 	void UserWantsToClose(void);
	//! The ObjectShouldStop Signal.
	/*!
		You can use this Signal to detect whenever this object tries to abort/stop the experiment.
		No Parameter.
	*/
//123 	void ObjectShouldStop(void);
	//! The ObjectStateHasChanged Signal.
	/*!
		You can use this Signal to detect whenever the Experiment State for this object changes.
		Parameter expSubObjectState is type of the enum ExperimentManagerNameSpace::ExperimentSubObjectState.
	*/
//123 	void ObjectStateHasChanged(ExperimentSubObjectState expSubObjectState);
	//! The NewInitBlockTrial Signal.
	/*!
		You can use this Signal to keep track of whenever the Experiment Manager tries to Initialize a new BlockTrial for this object.
		No Parameter.
	*/
//123 	void NewInitBlockTrial(void);
	//! The ExternalTriggerIncremented Signal.
	/*!
		You can use this Signal to keep track of whenever the External Trigger gets incremented, see GLWidgetWrapper::incrementExternalTrigger
		Please bear in mind that this is already emitted before a new BlockTrial is initialized.
		Parameter nTrigger holds the number of received external triggers after the start of the experiment.
	*/
//123 	void ExternalTriggerIncremented(int nTrigger);
	//! The ExperimentStructureChanged Signal.
	/*!
		You can use this Signal to keep track Experiment structure changes.
		Parameter nBlock represents the current Block number.
		Parameter nTrial represents the current Trial number.
		Parameter nInternalTrigger represents the current Internal Trigger number.
	*/
//123 	void ExperimentStructureChanged(int nBlock,int nTrial,int nInternalTrigger);

public:
	GLWidgetWrapper(QObject *parent = NULL);
	~GLWidgetWrapper();
	
	void setBlockTrials();
	
	//123 template< typename T1 > bool insertExpObjectParameter(const int &nObjectID,const QString &sKeyName,const T1 &tVariabele,bool bIsInitializing = true)
	//123 {
	//123 	bool bRetVal = false;
	//123 	QString tVarName = typeid(tVariabele).name();
	//123 	QString sValue = templateVariabeleToString(tVariabele);
	//123 	if(insertExpObjectBlockParameter(nObjectID,sKeyName,sValue,bIsInitializing))
	//123 		bRetVal = pExperimentManager->insertExperimentObjectVariabelePointer(nObjectID,sKeyName,tVariabele);
	//123 	return bRetVal;
	//123 }
	//123 QString templateVariabeleToString(const QStringList &Var) {return Var.join(",");}
	//123 QString templateVariabeleToString(const QString &Var) {return QString(Var);}
	//123 QString templateVariabeleToString(const QColor &Var) {return Var.name();}
	//123 QString templateVariabeleToString(const int &Var) {return QString::number(Var);}
	//123 QString templateVariabeleToString(const float &Var) {return QString::number(Var);}
	//123 QString templateVariabeleToString(const double &Var) {return QString::number(Var);}
	//123 QString templateVariabeleToString(const bool &Var) 
	//123 {
	//123 	if (Var)
	//123 		return TYPE_BOOL_TRUE; 
	//123 	else
	//123 		return TYPE_BOOL_FALSE;
	//123 }
	//123 template< typename T2 > T2* getExpObjectVariabelePointer(const int &nObjectID,const QString &sKeyName)
	//123 {
	//123 	return pExpConf->pExperimentManager->getExperimentObjectVariabelePointer<T2>(nObjectID,sKeyName);
	//123 }
	//123 bool setScriptEngine(QScriptEngine *currScriptEngine) {currentScriptEngine = currScriptEngine; return true;};
	//123 QScriptEngine *getScriptEngine() {return currentScriptEngine;};

public slots:
	//Can be overridden, virtual 
	bool initExperimentObject();
	//123 bool startExperimentObject();
	bool stopExperimentObject();
	//123 bool abortExperimentObject();
	//123 void ExperimentShouldFinish();					//Slot for a signal to connect to
	//123 bool setExperimentObjectID(int nObjID);			//Necessary to set the ID!
	//123 bool setExperimentMetaObject();					//Necessary to set the MetaObject!
	bool setExperimentManager(ExperimentManager *expManager);
	void setStimuliResolution(int w, int h);
	QRectF getScreenResolution();
	
	//123 int getObjectID();
	//strcScriptExperimentStructure getExperimentStructure();
	//123 bool insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing = true);
	//123 ParsedParameterDefinition getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue);
	//123 QScriptValue getExperimentObjectParameter(const int &nObjectID, const QString &strName);
	//123 bool setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal);
	
protected:
	//123 int checkForNextBlockTrial();
	void setupLayout(QWidget* layoutWidget);
	//void setupLayout2(QQuickView* quickView);
	//123 bool isDebugMode();
	//123 double getElapsedTrialTime() {return dElapsedTrialTime;};
	//123 int getCurrentBlockTrialFrame() {return nCurrExpBlockTrialFrame;};
	int getCurrentStimuliRefreshRate() {return nRefreshRate;};
	//123 QString getLastLoggedObjectStateTime(ExperimentSubObjectState state);
	//123 void setDoubleBufferCheck(bool bShouldCheck) {bCheckForDoubleBuffering = bShouldCheck;};
	void paintEvent(QPaintEvent *event);
	void closeEvent(QCloseEvent *evt);
	//123 void customEvent(QEvent *event);
	void initBlockTrial();
	void setAlternativeContainerDialog(QDialog *ContainerDlg = NULL);
	//1234 bool eventFilter(QObject *target, QEvent *event);

protected slots:
	//123 void incrementExternalTrigger();
	void animate(bool bOnlyCheckBlockTrials = false);
	void finalizePaintEvent();

private:
	//123 bool expandExperimentBlockParameterValue(QString &sValue);
	//123 bool unlockExperimentObject();
	//123 bool setExperimentObjectReadyToUnlock();
	void setVerticalSyncSwap();
	void init();
	//1234 bool changeSubObjectState(ExperimentSubObjectState newSubObjectState);
	//123 ExperimentSubObjectState getSubObjectState() {return currentSubObjectState;};
	void fetchCurrentExperimentStructures();

	//123 bool bCurrentSubObjectReadyToUnlock;				//The user first has to press the 'Alt' key before the experiment can be unlocked by the next trigger.
	//123 bool bFirstTriggerAfterUnlock;						//To detect the exact start of the experiment detected by the checkForNextBlockTrial() function.
	//123 bool bCurrentSubObjectIsLocked;						//After the above key is pressed this variable is set to false at the first trigger and the experiment starts.
	bool bCheckForDoubleBuffering;
	double dWaitTime;
	QMutex mutRecursivePaint;
	//123 QMutex mutProcEvents;								//Another implementation, due to qApp->processEvents() RecursiveRepaint can occur...
	//123 int nLastProcessedExternalTriggers;
	//123 int nCurrExpBlockTrialFrame;
	//123 double dElapsedTrialTime;
	int nRefreshRate;									//The refresh rate of the screen
	double dLastPreSwapTime;
	double dAdditionalRefreshDelayTime;
	ContainerDlg *stimContainerDlg;
	//Quick2ContainerWidget *Quick2CntnrWdgt;
	QObject *alternativeContainerDlg;
	//123 bool bExperimentShouldStop;
	QRectF rScreenResolution;
	QVBoxLayout *mainLayout;
	//123 int nObjectID;
	//123 QTimer tStimTimer;
	//123 QEvent::Type tEventObjectStopped;
	//123 ExperimentSubObjectState currentSubObjectState;
	//123 ExperimentSubObjectStateHistory subObjectStateHistory;
	//123 int nFrameTimerIndex;
	//123 int nTrialTimerIndex;
	//123 tParsedParameterList *ExpBlockParams;
	ExperimentManager *pExperimentManager;
	//123 ExperimentTimer expTrialTimer;
	//123 const QMetaObject* thisMetaObject;
	QPainter *lockedPainter;
	//123 QScriptEngine* currentScriptEngine;
};

#endif // GLWIDGET_H
