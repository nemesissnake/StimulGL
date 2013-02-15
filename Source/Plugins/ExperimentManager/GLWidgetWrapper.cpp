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

#include <QtWidgets>
#include <stdlib.h>
#include <math.h>
#include <QDomDocument>
#include <QEventLoop>
#include "GLWidgetWrapper.h"

/*! \brief The GLWidgetWrapper constructor.
*
*   You do not need to specify the parent object. 
*	The StimulGL script engine automatically retrieves the parent role
*/
GLWidgetWrapper::GLWidgetWrapper(QObject *parent) : ExperimentEngine(parent)//123 QGLWidget(parent),
{
	//123 currentSubObjectState = Experiment_SubObject_Constructing;
	lockedPainter = NULL;
	//123 thisMetaObject = NULL;
	stimContainerDlg = NULL;
	//Quick2CntnrWdgt = NULL;
	pExperimentManager = NULL;
	//123 currentScriptEngine = NULL;
	//123 bExperimentShouldStop = false;
	bCheckForDoubleBuffering = false;
	#ifdef Q_WS_MACX
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
	#elseabot
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
	#endif
	//123 init();
	//1234 setAutoFillBackground(false);
	//1234 setAttribute(Qt::WA_OpaquePaintEvent, true);
	//1234 setAttribute(Qt::WA_NoSystemBackground, true);
	//1234 setAutoBufferSwap(false); // in order to have control over time point for buffer swap
	rScreenResolution = QApplication::desktop()->screenGeometry();//dDesktopWidget->screenGeometry();//availableGeometry();
	//setFixedSize(rScreenResolution.width(), rScreenResolution.height());
	setStimuliResolution(rScreenResolution.width(), rScreenResolution.height());
	//setFixedSize(768,768);
	//setMinimumSize(200, 200);
	//1234 setWindowTitle(tr("Painting a Scene"));
	alternativeContainerDlg = NULL;

	stimContainerDlg = new ContainerDlg();
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	//stimContainerDlg->setAttribute(Qt::WA_PaintOnScreen);
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)

	//Quick2CntnrWdgt = new Quick2ContainerWidget();
	//Quick2CntnrWdgt->setAttribute(Qt::WA_DeleteOnClose);
	//Quick2CntnrWdgt->setAttribute(Qt::WA_PaintOnScreen);
	//Quick2CntnrWdgt->installEventFilter(this);//re-route all stimContainerDlg events to this->bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
	
	mainLayout = NULL;
	//123 nObjectID = -1;
	//123 tEventObjectStopped = (QEvent::Type)(QEvent::User + 1);	
	//123 ExpBlockParams = NULL;
	//123 nFrameTimerIndex = -1;
	//123 nTrialTimerIndex = -1;
	//123 bCurrentSubObjectIsLocked = true;
	//123 bCurrentSubObjectReadyToUnlock = false;
	dLastPreSwapTime = 0.0f;
	//123 dElapsedTrialTime = 0.0f;
	nRefreshRate = 0;
	dAdditionalRefreshDelayTime = 0.0f;
	setVerticalSyncSwap();
}

/*! \brief The GLWidgetWrapper destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
GLWidgetWrapper::~GLWidgetWrapper()
{
	//123 changeSubObjectState(Experiment_SubObject_Destructing);
	if (mainLayout)
	{
		delete mainLayout;
		mainLayout = NULL;
	}
	if (stimContainerDlg)
	{
		stimContainerDlg->close();
		stimContainerDlg = NULL;
	}
	//if(Quick2CntnrWdgt)
	//{
	//	Quick2CntnrWdgt->close();
	//	Quick2CntnrWdgt = NULL;
	//}
	//123 if (ExpBlockParams)
	//123 {
	//123 	delete ExpBlockParams;
	//123 	ExpBlockParams = NULL;
	//123 }
	if (lockedPainter)
	{
		delete lockedPainter;
		lockedPainter = NULL;
	}
	if (alternativeContainerDlg)
	{
		alternativeContainerDlg = NULL;//not owned by this class
	}
	//123 if (currentScriptEngine)
	//123 {
	//123 	currentScriptEngine = NULL;//not owned by this class
	//123 }
}

void GLWidgetWrapper::setVerticalSyncSwap()
{
	QGLFormat StimulGLQGLFormat;
	StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
	StimulGLQGLFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(StimulGLQGLFormat);
}

//123 bool GLWidgetWrapper::insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing)
//123 {
//123 /*! \brief Update or insert an Experiment Block Parameter that can be parsed.
//123  *
//123  *  This function can be used to insert or update an Experiment Block Parameter to be parsed for a specific object.
//123  *  This Parameter can be parsed at the next initialization of a new BlockTrial if not jet initialized.
//123  *  Setting the parameter bIsInitializing to true forces the new parameter to be re-parsed at the next initialization of a new BlockTrial.
//123  */
//123 	if (ExpBlockParams == NULL)
//123 	{
//123 		ExpBlockParams = new tParsedParameterList();
//123 		pExperimentManager->setExperimentObjectBlockParameterStructure(nObjectID,ExpBlockParams);
//123 	}
//123 	bool bRetVal = pExperimentManager->insertExperimentObjectBlockParameter(nObjectID,sName,sValue,bIsInitializing);
//123 	return bRetVal;
//123 }

//123 ParsedParameterDefinition GLWidgetWrapper::getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue)
//123 {
/*! \brief retrieves an buffered Experiment Block Parameter.
 *
 *  This function can be used to retrieve an buffered Experiment Block Parameter for a specific object.
 *	If this parameter is not found then the default String value(sDefValue) is returned.
 */
//123 	ParsedParameterDefinition PPDResult;
//123 	PPDResult.bHasChanged = false;
//123 	PPDResult.sValue = sDefValue;
//123 	PPDResult.bIsInitialized = true;
//123 	if (pExperimentManager->getExperimentObjectBlockParameter(nObjectID,sName,PPDResult))		
//123 		return PPDResult;
//123 	else
//123 		return PPDResult;
//123 }

//123 void GLWidgetWrapper::init()
//123 {
//123 	nLastProcessedExternalTriggers = CF_UNINITIALIZED;
//123 	nCurrExpBlockTrialFrame = CF_UNINITIALIZED;
//123 	changeSubObjectState(Experiment_SubObject_Initialized);
//123 }

QRectF GLWidgetWrapper::getScreenResolution()
{
	//int a = this->format().swapInterval();
	return rScreenResolution;
	//return QRectF(0.0f,0.0f,768.0f,768.0f);
}

void GLWidgetWrapper::setStimuliResolution(int w, int h)
{
	//1234 setFixedSize(w,h);
}

//123 int GLWidgetWrapper::getObjectID()
//123 {
//123 /*! \brief Returns this Object ID.
//123  *
//123  *  This function returns its unique Object ID as registered in the ExperimentManager.
//123  */
//123 	return nObjectID;
//123 }

//123 bool GLWidgetWrapper::setExperimentMetaObject()
//123 {
//123 	thisMetaObject = this->metaObject();
//123 	return true;
//123 }
//123 
//123 bool GLWidgetWrapper::setExperimentObjectID(int nObjID)
//123 {
//123 	if(nObjID >= 0)
//123 	{
//123 		nObjectID = nObjID;
//123 		bool bRetVal;
//123 		if (thisMetaObject)
//123 		{
//123 			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETOBJECTID_FULL)) == -1))//Is the slot present?
//123 			{
//123 				//Invoke the slot
//123 				if(!(thisMetaObject->invokeMethod(this, FUNC_SETOBJECTID, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int,nObjID))))
//123 				{
//123 					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETOBJECTID_FULL << ")!";		
//123 				}		
//123 			}
//123 		}
//123 	}
//123 	return false;
//123 }
//123 
//123 void GLWidgetWrapper::customEvent(QEvent *event)
//123 {
//123 	if (event->type() == tEventObjectStopped) 
//123 	{
//123 		emit ObjectShouldStop();
//123 	} 
//123 	else 
//123 	{
//123 		GLWidgetWrapper::customEvent(event);
//123 	}
//123 }

//123 void GLWidgetWrapper::ExperimentShouldFinish()
//123 {
//123 	emit ObjectShouldStop();
//123 	bExperimentShouldStop = true;
//123 	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Finished experiment"));
//123 }

void GLWidgetWrapper::setAlternativeContainerDialog(QDialog *ContainerDlg)
{
	alternativeContainerDlg = ContainerDlg;
}

//123 bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
//123 {
//123 	if ((target == stimContainerDlg) || (target == alternativeContainerDlg))
//123 	{
//123 		if (event->type() == QEvent::KeyPress) 
//123 		{
//123 			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//123 			switch (keyEvent->key())
//123 			{
//123 			case Qt::Key_A://Key_Escape:	//To abort the experiment
//123 				if((keyEvent->modifiers() & Qt::ControlModifier))// && (keyEvent->modifiers() & Qt::AltModifier))
//123 				{
//123 					emit UserWantsToClose();
//123 				}
//123 				break;
//123 			//case Qt::Key_Tab:
//123 			//	setExperimentObjectReadyToUnlock();
//123 			//	break;
//123 			case Qt::Key_Alt:	//To start the experiment
//123 				ExperimentEngine::setExperimentObjectReadyToUnlock();
//123 				if(target == alternativeContainerDlg)//here we don't have a trigger object because of the lack of the ExperimentManager, so we trigger it automatically to start
//123 					tStimTimer.singleShot(100, this, SLOT(incrementExternalTrigger()));//incrementExternalTrigger();
//123 				break;
//123 			}
//123 		}
//123 	}
//123 return true;
//123 }

bool GLWidgetWrapper::setExperimentManager(ExperimentManager *expManager)
{
	if(pExperimentManager!=expManager)
	{
		pExperimentManager = expManager;
		ExperimentEngine::setExperimentManager(expManager);//Important!
	}
	return true;
}

//123 bool GLWidgetWrapper::expandExperimentBlockParameterValue(QString &sValue)
//123 {
//123 	if (pExperimentManager)
//123 	{
//123 		return pExperimentManager->expandExperimentBlockParameterValue(sValue);				
//123 	}
//123 	return false;
//123 }

void GLWidgetWrapper::setupLayout(QWidget* layoutWidget)
{
	mainLayout = new QVBoxLayout;
	mainLayout->setAlignment(Qt::AlignCenter);
	mainLayout->setMargin(0);
	mainLayout->addWidget(layoutWidget);
	stimContainerDlg->setLayout(mainLayout);
	stimContainerDlg->setModal(true); //Set to true because starting an experiment file from the ExperimentManager plugins User Interface requests this to work properly...
	stimContainerDlg->showFullScreen();
}

//void GLWidgetWrapper::setupLayout2(QQuickView* quickView)//QWidget* layoutWidget)
//{
	//mainLayout = new QVBoxLayout;
	//mainLayout->setAlignment(Qt::AlignCenter);
	//mainLayout->setMargin(0);
	//mainLayout->addWidget(layoutWidget);

	//Quick2CntnrWdgt->setWindow((QQuickWindow*) quickView); //= new Quick2ContainerWidget(,this);
	//Quick2CntnrWdgt->setLayout(mainLayout);
	//layoutWidget->setModal(true); //Set to true because starting an experiment file from the ExperimentManager plugins User Interface requests this to work properly...
	//Quick2CntnrWdgt->showFullScreen();
//}

//123 bool GLWidgetWrapper::startExperimentObject()
//123 {
//123 	//QString a = thisMetaObject->className();
//123 	if (bCheckForDoubleBuffering)
//123 	{
//123 		if(!this->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
//123 		{
//123 			qDebug() << __FUNCTION__ << "RetinoMap_glwidget::No Double Buffering available!";
//123 			stimContainerDlg->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
//123 			//Quick2CntnrWdgt->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
//123 			return false;
//123 		}
//123 	}
//123 	//else
//123 	//{
//123 	//	QGLFormat newFormat;
//123 	//	newFormat.setDoubleBuffer(false);
//123 	//	this->setFormat(newFormat);
//123 	//	if(this->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
//123 	//	{
//123 	//		bool a = true;
//123 	//	}
//123 	//	else
//123 	//	{
//123 	//		bfool b = true;
//123 	//	}
//123 	//}
//123 
//123 	bFirstTriggerAfterUnlock = true;
//123 	pExperimentManager->startExperimentTimer(nFrameTimerIndex);//Starts the Frame timer
//123 	pExperimentManager->startExperimentTimer(nTrialTimerIndex);//Starts the Trial timer
//123 	checkForNextBlockTrial();
//123 	bool bRetVal;
//123 	if (thisMetaObject)
//123 	{
//123 		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STARTOBJECT_FULL)) == -1))//Is the slot present?
//123 		{
//123 			//Invoke the slot
//123 			if(!(thisMetaObject->invokeMethod(this, FUNC_STARTOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
//123 			{
//123 				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_STARTOBJECT_FULL << ")!";		
//123 			}		
//123 		}
//123 	}
//123 	return true;
//123 }

//123 bool GLWidgetWrapper::setExperimentObjectReadyToUnlock()
//123 {
//123 	if (bCurrentSubObjectReadyToUnlock == false)
//123 	{
//123 		bCurrentSubObjectReadyToUnlock = true;
//123 		if(isDebugMode() && pExperimentManager)
//123 			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Ready to Unlock the Experiment Object");
//123 	}
//123 	return true;
//123 }

//123 bool GLWidgetWrapper::unlockExperimentObject()
//123 {
//123 	if (bCurrentSubObjectIsLocked)
//123 	{
//123 		bCurrentSubObjectIsLocked = false;
//123 		if(isDebugMode() && pExperimentManager)
//123 			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Unlocked the Experiment Object");
//123 	}
//123 	return true;
//123 }

bool GLWidgetWrapper::initExperimentObject()
{
	nRefreshRate = 0;
	dAdditionalRefreshDelayTime = 0.0;
	insertExpObjectParameter(getObjectID(),GLWWRAP_WIDGET_STIMULI_REFRESHRATE,nRefreshRate);
	return ExperimentEngine::initExperimentObject();
			
//123 	bool bRetVal;
//123 	if (thisMetaObject)
//123 	{
//123 		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_INITOBJECT_FULL)) == -1))//Is the slot present?
//123 		{
//123 			//Invoke the slot
//123 			if(!(thisMetaObject->invokeMethod(this, FUNC_INITOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
//123 			{
//123 				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_INITOBJECT_FULL << ")!";		
//123 			}		
//123 		}
//123 	}	
//123 	int iCPU = omp_get_num_procs(); // Get the number of processors in this system
//123     qDebug() << __FUNCTION__ << "::" << iCPU << "processors available.";//The omp_get_num_threads() call returns 1 in the serial section of the code!!
//123 	omp_set_num_threads(iCPU); // Set the number of threads
//123 	bool bResult = connect(pExperimentManager->getExperimentStructure(),SIGNAL(experimentStopped()),this,SLOT(ExperimentShouldFinish()));
//123 	return true;
}

//123 bool GLWidgetWrapper::isDebugMode() 
//123 {
//123 	if (pExperimentManager)
//123 	{
//123 		if(pExperimentManager->getExperimentStructure())
//123 			return pExperimentManager->getExperimentStructure()->getExperimentDebugMode();
//123 	}
//123 	return false;
//123 }

//123 QString GLWidgetWrapper::getLastLoggedObjectStateTime(ExperimentSubObjectState state)
//123 {
//123 	if (!subObjectStateHistory.nState.isEmpty())
//123 	{
//123 		int nHistoryCount = subObjectStateHistory.nState.count();
//123 		if (nHistoryCount > 0)
//123 		{
//123 			for (int i=nHistoryCount-1;i>0;i--)
//123 			{
//123 				if (subObjectStateHistory.nState.at(i) == state)
//123 				{
//123 					return subObjectStateHistory.sDateTimeStamp.at(i);
//123 				}
//123 			}
//123 		} 
//123 	}
//123 	return "";
//123 }

void GLWidgetWrapper::closeEvent(QCloseEvent *evt)
{
	//1234 QGLWidget::closeEvent(evt);
}

bool GLWidgetWrapper::stopExperimentObject()
{
//123 	bExperimentShouldStop = true;
	getExperimentMetaObject()->invokeMethod(this, "finalizePaintEvent",Qt::QueuedConnection);//a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
//123 	bool bRetVal;
//123 	if (thisMetaObject)
//123 	{
//123 		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STOPOBJECT_FULL)) == -1))//Is the slot present?
//123 		{
//123 			//Invoke the slot
//123 			if(!(thisMetaObject->invokeMethod(this, FUNC_STOPOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
//123 			{
//123 				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_STOPOBJECT_FULL << ")!";		
//123 			}		
//123 		}
//123 	}
//123 	return true;
	return ExperimentEngine::stopExperimentObject();
}

//123 bool GLWidgetWrapper::abortExperimentObject()
//123 {
//123 	return stopExperimentObject();
//123 }

void GLWidgetWrapper::paintEvent(QPaintEvent *event)
{
	int nPaintFlags;
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","Starting to paint the widget");
	if (isLocked() == false)
	{
		nPaintFlags = (int)GLWidgetPaintFlags_NoFlag;
	//}
	//else
	//{
	//	nPaintFlags = (int)GLWidgetPaintFlags_LockedState;
	//}

		bool bRetVal = false;		
		if (getExperimentMetaObject())
		{
			if (!(getExperimentMetaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PAINTOBJECT_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(getExperimentMetaObject()->invokeMethod(this, FUNC_PAINTOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int,nPaintFlags),Q_ARG(QObject*, (QObject*)event))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_PAINTOBJECT_FULL << ")!";		
				}		
			}
		}
	}
	else
	{
		if(isDebugMode() && pExperimentManager) 
			pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","Starting to paint the initial widget");
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","pre Step 1");
		if (lockedPainter == NULL)
			//1234 lockedPainter = new QPainter(this);//Constructor automatically calls begin()
		//1234 else
			//1234 lockedPainter->begin(this);
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 1");
		QFont textFont("arial", 22, QFont::Bold, false);
		QString strText;
		QString strTextStart = "Experiment ready, ";
		if (isReadyToUnlock())
			strText = strTextStart + "waiting for a trigger to start...";
		else
			strText = strTextStart + "press 'Alt' to proceed or CTRL + 'a' to abort the experiment";
		const QRectF windowRect = lockedPainter->window();
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 2");
		const int nBorder = 25;
		QPainterPath textPath;
		//1234 textPath.addText(0,0,textFont,strText);
		const QRectF textPathRect = textPath.boundingRect();
		lockedPainter->setRenderHint(QPainter::Antialiasing);
		lockedPainter->fillRect(windowRect,QColor(87,87,87));
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 3");
		lockedPainter->setPen(Qt::NoPen);
		lockedPainter->setBrush(Qt::white);
		lockedPainter->setWindow ( textPathRect.x() - nBorder , textPathRect.y() - (windowRect.height()/2) , textPathRect.width() + (nBorder*2) , windowRect.height());//translate text rect to rect window button
		lockedPainter->drawPath(textPath);
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 4");
		lockedPainter->end();
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Finished painting the initial widget");		
	}
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","painter.end()");
	finalizePaintEvent();
}

//1234 void GLWidgetWrapper::incrementExternalTrigger()
//{
///*! \brief Increments the External Trigger.
// *
// *  Invoke this slot to increment the External Trigger.
// */
//	if (bCurrentSubObjectIsLocked)
//	{
//		if (bCurrentSubObjectReadyToUnlock == true)
//		{
//			bCurrentSubObjectReadyToUnlock = false;
//			unlockExperimentObject();
//			if(pExperimentManager)
//			{
//				pExperimentManager->getExperimentStructure()->resetExperiment();
//				pExperimentManager->getExperimentStructure()->prepareExperiment();
//				pExperimentManager->getExperimentStructure()->incrementExternalTrigger();
//			}
//			else
//			{//No Experiment Manager? QML?
//				//1234 update();
//				//QTimer::singleShot(1000, this, SLOT(incrementExternalTrigger()));//Let's start!
//				//QMetaObject::invokeMethod()
//			}
//		}
//	}
//	else
//	{
//		if(pExperimentManager)
//		{
//			pExperimentManager->getExperimentStructure()->incrementExternalTrigger();
//			emit ExternalTriggerIncremented(pExperimentManager->getExperimentStructure()->getExternalTriggerCount());//Should be automatically connected!!
//			if(isDebugMode() && pExperimentManager)
//				pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"",QString("Externally Triggered!, Received="),QString::number(pExperimentManager->getExperimentStructure()->getExternalTriggerCount()));
//		}
//		else
//		{
//			emit ExternalTriggerIncremented(-1);
//			if(isDebugMode() && pExperimentManager)
//				qDebug() << __FUNCTION__ << "Automatically Externally Triggered!, no experiment manager active.";
//		}
//	}
//}

void GLWidgetWrapper::initBlockTrial()
{
	//123 emit NewInitBlockTrial();
	//123 bExperimentShouldStop = false;
//#ifdef Q_WS_MACX
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
//#else
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
//#endif
	//123 expTrialTimer.restart();
	//123 if(pExperimentManager)
	//123 {
	//123 	bool bHasCurrentBlock = false;
	//123 	cBlockStructure tmpBlock = pExperimentManager->getExperimentStructure()->getCurrentBlock(bHasCurrentBlock);
	//123 	if(bHasCurrentBlock)
	//123 		pExperimentManager->fetchExperimentBlockParamsFromDomNodeList(tmpBlock.getBlockNumber(),nObjectID);
	//123 	else
	//123 		qDebug() << __FUNCTION__ << "::Could not create a Block Parameter List, no current block!";
	//123 }
	//123 else
	//123 {
	//123 	qDebug() << __FUNCTION__ << "::No ExperimentManager defined!";
	//123 }
	ParsedParameterDefinition pParDef;
	pParDef = getExpObjectBlockParameter(getObjectID(),GLWWRAP_WIDGET_STIMULI_REFRESHRATE,QString::number(nRefreshRate));
	nRefreshRate = pParDef.sValue.toInt();
	//123 bool bRetVal;
	//123 if (thisMetaObject)
	//123 {
	//123 	if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_INITOBJECTBLOCKTRIAL_FULL)) == -1))//Is the slot present?
	//123 	{
	//123 		//Invoke the slot
	//123 		if(!(thisMetaObject->invokeMethod(this, FUNC_INITOBJECTBLOCKTRIAL, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
	//123 		{
	//123 			qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_INITOBJECTBLOCKTRIAL_FULL << ")!";		
	//123 		}		
	//123 	}
	//123 }
	return ExperimentEngine::initBlockTrial();
}

//123 int GLWidgetWrapper::checkForNextBlockTrial()
//123 {
//123 	if (bExperimentShouldStop)
//123 		return 0;
//123 	if (pExperimentManager == NULL)
//123 	{
//123 		if(bCurrentSubObjectIsLocked)
//123 			return -1;
//123 		//qDebug() << __FUNCTION__ << "::No Experiment Manager set!";
//123 		return -1;//Special case... direct QML execute
//123 	}
//123 	bool goToNextBlockTrial = false;
//123 	bool bFirstCheckAfterExperimentStarted = false;	
//123 	int nRetval = 0;
//123 	bool bExperimentStructureChanged = false;
//123 	bool bHasCurrentBlock = false;
//123 	cExperimentStructure tmpExpStr = cExperimentStructure(*pExperimentManager->getExperimentStructure());
//123 	cBlockStructure tmpExpBlockStr = tmpExpStr.getCurrentBlock(bHasCurrentBlock);
//123 	strcExperimentStructureState tmpExpStrState = tmpExpStr.getCurrentExperimentState();
//123 
//123 	if((tmpExpStrState.Experiment_ExternalTrigger == RA_REINITIALIZE) && (nCurrExpBlockTrialFrame == CF_UNINITIALIZED))
//123 	{//First Experiment Trial? (Start/Init), occurs before the start of the Trigger(timer)
//123 		goToNextBlockTrial = true;
//123 		bFirstCheckAfterExperimentStarted = true;		
//123 		bExperimentStructureChanged = true;
//123 	}
//123 	if ((tmpExpStrState.Experiment_ExternalTrigger >= 0) && (tmpExpStrState.Experiment_ExternalTrigger > nLastProcessedExternalTriggers))
//123 	{
//123 		nLastProcessedExternalTriggers = tmpExpStrState.Experiment_ExternalTrigger;
//123 		bExperimentStructureChanged = true;
//123 	}
//123 	if(bFirstCheckAfterExperimentStarted == false)
//123 	{
//123 		if(tmpExpStr.getBlockCount() > 0)//Are there blocks defined? QML Viewers trough UI (without ExperimentManager) don't have any defined blocks here!
//123 			if(bHasCurrentBlock)
//123 				goToNextBlockTrial = (tmpExpStrState.CurrentBlock_ExternalTrigger == 0);//Go to next Block Trial?
//123 	}
//123 	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
//123 	{
//123 		bExperimentStructureChanged = true;
//123 		nCurrExpBlockTrialFrame = 0;
//123 		if(pExperimentManager)
//123 			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Starting to Init new BlockTrial"),QString("BlockNumber=") + QString::number(tmpExpBlockStr.getBlockNumber()) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +  ", TrialExternalTrigger=" + QString::number(tmpExpStrState.CurrentBlock_ExternalTrigger) + ", TrialInternalTrigger=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(nCurrExpBlockTrialFrame) + ")");
//123 		initBlockTrial();
//123 		if (bFirstCheckAfterExperimentStarted)
//123 		{
//123 			changeSubObjectState(Experiment_SubObject_Started);
//123 		}
//123 		if (bCurrentSubObjectIsLocked == false)
//123 		{
//123 			if((isDebugMode()) && (pExperimentManager))
//123 				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","New BlockTrial Prepared, going to qeueu animate()");
//123 			QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
//123 		}
//123 		else
//123 		{
//123 			if(tmpExpStr.getBlockCount() > 0)//Are there blocks defined? QML Viewers trough UI (without ExperimentManager) don't have any defined blocks here!
//123 				if(bHasCurrentBlock)
//123 					QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));
//123 		}
//123 		expTrialTimer.restart();
//123 		nRetval = 1;
//123 	}
//123 	else
//123 	{
//123 		nRetval = 0;
//123 	}
//123 	if ((bCurrentSubObjectIsLocked == false) && (bFirstTriggerAfterUnlock))//has the experiment just been unlocked for the first time?
//123 	{
//123 		bFirstTriggerAfterUnlock = false;
//123 		emit ExperimentStructureChanged(tmpExpBlockStr.getBlockNumber(),tmpExpStrState.CurrentBlock_TrialNumber,tmpExpStrState.CurrentBlock_InternalTrigger);
//123 	}
//123 	if ((bExperimentStructureChanged) && (bFirstCheckAfterExperimentStarted == false))
//123 	{
//123 		if(getSubObjectState() == Experiment_SubObject_Started)
//123 			emit ExperimentStructureChanged(tmpExpBlockStr.getBlockNumber(),tmpExpStrState.CurrentBlock_TrialNumber,tmpExpStrState.CurrentBlock_InternalTrigger);
//123 	}
//123 	return nRetval;
//123 }

void GLWidgetWrapper::animate(bool bOnlyCheckBlockTrials)
{
	//QObject *a = sender();	
	if(getSubObjectState() == Experiment_SubObject_Started)
	{
		if(mutRecursivePaint.tryLock())
		{
			if (experimentShouldStop())
			{
				changeSubObjectState(Experiment_SubObject_Stop);
				mutRecursivePaint.unlock();
				return;
			}

			if((isDebugMode()) && (pExperimentManager))
				pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","Going to call update()");
			if (bOnlyCheckBlockTrials)
			{
				//if(a)
				//	QString b = a->objectName();
				checkForNextBlockTrial();
			} 
			else
			{
				//if(a)
				//	QString b = a->objectName();
				//1234 update();
			}
			if((isDebugMode()) && (pExperimentManager))
				pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","update() called");
			//	elapsed = frameTime.elapsed();//Calculate the elapsed time since started
			//	frameTime.restart();
			//	repaint();//calls the below void GLWidget::paintEvent(QPaintEvent *event)
			//You should usually use 'update', as this will allow multiple queued paint events to be 'collapsed' into a single event. 
			//The update method will call updateGL for QGLWidgets. The 'repaint' method should be used if you want an immediate repaint.
			//If you have hooked up a timer to periodically call 'update', then failure to repaint regularly usually indicates 
			//that you're putting stress on the CPU.
			mutRecursivePaint.unlock();
		}
	}
}

//123 bool GLWidgetWrapper::changeSubObjectState(ExperimentSubObjectState newSubObjectState)
//123 {
//123 	if(newSubObjectState != currentSubObjectState)
//123 	{
//123 		if((((int)newSubObjectState > (int)ExperimentSubObjectState::Experiment_SubObject_MaxState) || ((int)newSubObjectState < 0)) ||
//123 		  (((int)currentSubObjectState > (int)ExperimentSubObjectState::Experiment_SubObject_MaxState) || ((int)currentSubObjectState < 0)) ||
//123 		  ((int)currentSubObjectState == Experiment_SubObject_Destructing))
//123 		{
//123 			return false;
//123 		}
//123 		currentSubObjectState = newSubObjectState;
//123 		subObjectStateHistory.nState.append(currentSubObjectState);
//123 		subObjectStateHistory.sDateTimeStamp.append(QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
//123 		emit ObjectStateHasChanged(currentSubObjectState);
//123 		if(currentSubObjectState == Experiment_SubObject_Stop)
//123 		{
//123 			qApp->setActiveWindow(MainAppInfo::getMainWindow());
//123 		}
//123 		return true;
//123 	}
//123 	return false;
//123 }

void GLWidgetWrapper::finalizePaintEvent() 
{
	double dCurrentTime;
	double dFramePeriodTime;
	bool bObjectIsLocked = isLocked(); //buffer it..

	bool bExperimentStructureChanged = false;
	bool bHasCurrentBlock = false;
	cExperimentStructure tmpExpStr;
	cBlockStructure tmpExpBlockStr;
	strcExperimentStructureState tmpExpStrState;
	if(pExperimentManager)
	{
		cExperimentStructure tmpExpStr = cExperimentStructure(*pExperimentManager->getExperimentStructure());
		cBlockStructure tmpExpBlockStr = tmpExpStr.getCurrentBlock(bHasCurrentBlock);
		strcExperimentStructureState tmpExpStrState = tmpExpStr.getCurrentExperimentState();
	}

	if (nRefreshRate > 0)
	{
		dFramePeriodTime = 1000.0f/nRefreshRate; //DisplayRefreshRate
		dAdditionalRefreshDelayTime = dFramePeriodTime/10.0f;
		dWaitTime = 0.0f;
		//dCurrentTime = pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);	
		//while((dCurrentTime)<dFramePeriodTime)//Don't go too fast...//((dCurrentTime+dAdditionalRefreshDelayTime)<dFramePeriodTime)//Don't go too fast...
		//{
		//	dWaitTime = dFramePeriodTime - dCurrentTime;// + dAdditionalRefreshDelayTime;
		//	if(isDebugMode() && pExperimentManager)// && (bObjectIsLocked==false))
		//		pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Too fast --> Sleeping(" + QString::number(dWaitTime) + ")...") + QString::number(bObjectIsLocked);			
		//	ExperimentTimer::SleepMSecAccurate(dWaitTime);
		//	//Sleep(nSTime);
		//	dCurrentTime = pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);
		//}
		if (bObjectIsLocked == false)
		{
			double currentPreSwapTime = pExperimentManager->elapsedExperimentTimerTime(0);
			if((tmpExpStrState.Experiment_ExternalTrigger == 0) && (getCurrentBlockTrialFrame() == 0))
			{
				dLastPreSwapTime = currentPreSwapTime;//Just set this initial value and proceed
			}
			else
			{
				dWaitTime = (dLastPreSwapTime + dFramePeriodTime) - currentPreSwapTime;
				if(dWaitTime > dAdditionalRefreshDelayTime)//Do we need to wait?
				{
					if(isDebugMode() && pExperimentManager)
						pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","Too fast --> Sleeping(" + QString::number(dWaitTime-dAdditionalRefreshDelayTime) + ")... Locked="),QString::number(bObjectIsLocked);
					ExperimentTimer::SleepMSecAccurate(dWaitTime-dAdditionalRefreshDelayTime);
				}
				else if(dWaitTime < 0.0f)//This means that a frame is going to be skipped!
				{					
					while(dWaitTime < 0.0f)//Search the first next available frame threshold time
					{
						pExperimentManager->logExperimentObjectData(getObjectID(),-1,__FUNCTION__,"","Frame skipped!, missed=", QString::number(dWaitTime));
						dLastPreSwapTime = dLastPreSwapTime + dFramePeriodTime;
						dWaitTime = (dLastPreSwapTime + dFramePeriodTime) - currentPreSwapTime;
					}
					if(isDebugMode() && pExperimentManager)
						pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","Sleeping for next frame(" + QString::number(dWaitTime-dAdditionalRefreshDelayTime) + ")...") + QString::number(bObjectIsLocked);
					ExperimentTimer::SleepMSecAccurate(dWaitTime-dAdditionalRefreshDelayTime);
				}
				dLastPreSwapTime = dLastPreSwapTime + dFramePeriodTime;
			}
		}
		dCurrentTime = pExperimentManager->restartExperimentTimer(getFrameTimerIndex());
	}
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","Going to Swap, locked=", QString::number(bObjectIsLocked));
	//1234 swapBuffers();
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(getObjectID(),0,__FUNCTION__,"","BlockTrial Buffer Swapped, locked=",QString::number(bObjectIsLocked));
	if (experimentShouldStop())
	{
		changeSubObjectState(Experiment_SubObject_Stop);
		return;
	}
	//qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,1); //!Important: To receive Trigger Signals and process them before the below checkForNextBlockTrial();
	//if (experimentShouldStop())
	//{
	//	changeSubObjectState(Experiment_SubObject_Stop);
	//	return;
	//}
	//if (bExperimentShouldStop)
	//{
	//	changeSubObjectState(Experiment_SubObject_Stop);
	//	return;
	//}
	if (bObjectIsLocked==false)
	{
		incrementCurrentBlockTrialFrame();// = CF_UNINITIALIZED;
	}
	if((nRefreshRate > 0) && (bObjectIsLocked==false))
	{
		if(pExperimentManager)
		{
			if (dCurrentTime > (dFramePeriodTime*1.5f))
				pExperimentManager->logExperimentObjectData(getObjectID(),-1,__FUNCTION__,"","Paint routine took too long(" + QString::number(dCurrentTime) + " mSecs),(BlockID=" + QString::number(tmpExpStrState.CurrentBlock_BlockID) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +", Trigger(Int)=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(getCurrentBlockTrialFrame()) + ")",QString::number(dCurrentTime));
			else if(isDebugMode() && pExperimentManager)
				pExperimentManager->logExperimentObjectData(getObjectID(),-1,__FUNCTION__,"","Paint routine took(" + QString::number(dCurrentTime) + " mSecs),(BlockID=" + QString::number(tmpExpStrState.CurrentBlock_BlockID) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +", Trigger(Int)=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(getCurrentBlockTrialFrame()) + ")",QString::number(dCurrentTime));
		}
	}
	int nResult = checkForNextBlockTrial();//Check whether we need to prepare for an new block Trial
	if(nResult == 0) //No Block Trials to check
	{
		if (experimentShouldStop())
		{
			changeSubObjectState(Experiment_SubObject_Stop);
			//QCoreApplication::postEvent(this,new QEvent(tEventObjectStopped),Qt::HighEventPriority);
			return;
		}
		else
		{
			QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
		}
	}
	else if(nResult == -1)//No Experiment Manager set (QML), just invoke animate()
	{
		if (bObjectIsLocked==false)
		{
			if(getSubObjectState() == Experiment_SubObject_Initialized)
			{
				changeSubObjectState(Experiment_SubObject_Started);
				QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
			}
		}
	}
}

//123 QScriptValue GLWidgetWrapper::getExperimentObjectParameter(const int &nObjectID, const QString &strName)
//123 {
//123 /*! \brief retrieves the current value of an Experiment Parameter variable.
//123  *
//123  *  This function can be used to retrieve the current value of an Experiment Parameter variable for a specific object.
//123  *	This parameter variable value is currently in use.
//123  */
//123 	if (currentScriptEngine)
//123 	{
//123 		//return currentScriptEngine->toScriptValue(getExpObjectVariabelePointer<QString>(nObjectID,strName));
//123 		QScriptValue sScriptValue;
//123 		if(pExperimentManager->getExperimentObjectScriptValue(nObjectID,strName,sScriptValue))
//123 		{
//123 			//QString test = sScriptValue.toString();
//123 			return sScriptValue;
//123 		}
//123 		return currentScriptEngine->undefinedValue();
//123 	}
//123 	qDebug() << __FUNCTION__ << ":No Script Engine available!";
//123 	return NULL;	
//123 }

//123 bool GLWidgetWrapper::setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal)
//123 {
//123 /*! \brief Sets the current value of an Experiment Parameter variable.
//123  *
//123  *  This function can be used to immediately set a value of an Experiment Parameter variable for a specific object.
//123  *	This parameter variable value is immediately set and used.
//123  */
//123 	return pExperimentManager->setExperimentObjectFromScriptValue(nObjectID,strName,sScriptVal);
//123 }