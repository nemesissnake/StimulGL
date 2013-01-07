//ExperimentManagerplugin
//Copyright (C) 2012  Sven Gijsen
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

#include <QtGui>
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
GLWidgetWrapper::GLWidgetWrapper(QWidget *parent) : QGLWidget(parent)
{
	currentSubObjectState = Experiment_SubObject_Constructing;
	lockedPainter = NULL;
	thisMetaObject = NULL;
	stimContainerDlg = NULL;
	pExperimentManager = NULL;
	currentScriptEngine = NULL;
	bExperimentShouldStop = false;
	bCheckForDoubleBuffering = false;
	#ifdef Q_WS_MACX
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
	#elseabot
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
	#endif
	init();
	setAutoFillBackground(false);
	setAttribute(Qt::WA_OpaquePaintEvent, true);
	setAttribute(Qt::WA_NoSystemBackground, true);
	setAutoBufferSwap(false); // in order to have control over time point for buffer swap
	rScreenResolution = QApplication::desktop()->screenGeometry();//dDesktopWidget->screenGeometry();//availableGeometry();
	//setFixedSize(rScreenResolution.width(), rScreenResolution.height());
	setStimuliResolution(rScreenResolution.width(), rScreenResolution.height());
	//setFixedSize(768,768);
	//setMinimumSize(200, 200);
	setWindowTitle(tr("Painting a Scene"));
	alternativeContainerDlg = NULL;
	stimContainerDlg = new ContainerDlg();
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	stimContainerDlg->setAttribute(Qt::WA_PaintOnScreen);
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
	mainLayout = NULL;
	nObjectID = -1;
	tEventObjectStopped = (QEvent::Type)(QEvent::User + 1);	
	ExpBlockParams = NULL;
	nFrameTimerIndex = -1;
	nTrialTimerIndex = -1;
	bCurrentSubObjectIsLocked = true;
	bCurrentSubObjectReadyToUnlock = false;
	dLastPreSwapTime = 0.0f;
	dElapsedTrialTime = 0.0f;
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
	changeSubObjectState(Experiment_SubObject_Destructing);
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
	if (ExpBlockParams)
	{
		delete ExpBlockParams;
		ExpBlockParams = NULL;
	}
	if (lockedPainter)
	{
		delete lockedPainter;
		lockedPainter = NULL;
	}
	if (alternativeContainerDlg)
	{
		alternativeContainerDlg = NULL;//not owned by this class
	}
	if (currentScriptEngine)
	{
		currentScriptEngine = NULL;//not owned by this class
	}
}

void GLWidgetWrapper::setVerticalSyncSwap()
{
	QGLFormat StimulGLQGLFormat;
	StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
	StimulGLQGLFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(StimulGLQGLFormat);
}

bool GLWidgetWrapper::insertExpObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing)
{
/*! \brief Update or insert an Experiment Block Parameter that can be parsed.
 *
 *  This function can be used to insert or update an Experiment Block Parameter to be parsed for a specific object.
 *  This Parameter can be parsed at the next initialization of a new BlockTrial if not jet initialized.
 *  Setting the parameter bIsInitializing to true forces the new parameter to be re-parsed at the next initialization of a new BlockTrial.
 */
	if (ExpBlockParams == NULL)
	{
		ExpBlockParams = new tParsedParameterList();
		pExperimentManager->setExperimentObjectBlockParameterStructure(nObjectID,ExpBlockParams);
	}
	bool bRetVal = pExperimentManager->insertExperimentObjectBlockParameter(nObjectID,sName,sValue,bIsInitializing);
	return bRetVal;
}

ParsedParameterDefinition GLWidgetWrapper::getExpObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue)
{
/*! \brief retrieves an buffered Experiment Block Parameter.
 *
 *  This function can be used to retrieve an buffered Experiment Block Parameter for a specific object.
 *	If this parameter is not found then the default String value(sDefValue) is returned.
 */
	ParsedParameterDefinition PPDResult;
	PPDResult.bHasChanged = false;
	PPDResult.sValue = sDefValue;
	PPDResult.bIsInitialized = true;
	if (pExperimentManager->getExperimentObjectBlockParameter(nObjectID,sName,PPDResult))		
		return PPDResult;
	else
		return PPDResult;
}

void GLWidgetWrapper::init()
{
	nLastProcessedExternalTriggers = CF_UNINITIALIZED;
	nCurrExpBlockTrialFrame = CF_UNINITIALIZED;
	changeSubObjectState(Experiment_SubObject_Initialized);
}

QRectF GLWidgetWrapper::getScreenResolution()
{
	//int a = this->format().swapInterval();
	return rScreenResolution;
	//return QRectF(0.0f,0.0f,768.0f,768.0f);
}

void GLWidgetWrapper::setStimuliResolution(int w, int h)
{
	setFixedSize(w,h);
}

int GLWidgetWrapper::getObjectID()
{
/*! \brief Returns this Object ID.
 *
 *  This function returns its unique Object ID as registered in the ExperimentManager.
 */
	return nObjectID;
}

bool GLWidgetWrapper::setExperimentMetaObject()
{
	thisMetaObject = this->metaObject();
	return true;
}

bool GLWidgetWrapper::setExperimentObjectID(int nObjID)
{
	if(nObjID >= 0)
	{
		nObjectID = nObjID;
		bool bRetVal;
		if (thisMetaObject)
		{
			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETOBJECTID_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(thisMetaObject->invokeMethod(this, FUNC_SETOBJECTID, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int,nObjID))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETOBJECTID_FULL << ")!";		
				}		
			}
		}
	}
	return false;
}

void GLWidgetWrapper::customEvent(QEvent *event)
{
	if (event->type() == tEventObjectStopped) 
	{
		emit ObjectShouldStop();
	} 
	else 
	{
		GLWidgetWrapper::customEvent(event);
	}
}

void GLWidgetWrapper::ExperimentShouldFinish()
{
	emit ObjectShouldStop();
	bExperimentShouldStop = true;
	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Finished experiment"));
}

void GLWidgetWrapper::setAlternativeContainerDialog(QDialog *ContainerDlg)
{
	alternativeContainerDlg = ContainerDlg;
}

bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
{
	if ((target == stimContainerDlg) || (target == alternativeContainerDlg))
	{
		if (event->type() == QEvent::KeyPress) 
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

			switch (keyEvent->key())
			{
			case Qt::Key_A://Key_Escape:	//To abort the experiment
				if((keyEvent->modifiers() & Qt::ControlModifier))// && (keyEvent->modifiers() & Qt::AltModifier))
				{
					emit UserWantsToClose();
				}
				break;
			//case Qt::Key_Tab:
			//	setExperimentObjectReadyToUnlock();
			//	break;
			case Qt::Key_Alt:	//To start the experiment
				setExperimentObjectReadyToUnlock();
				if(target == alternativeContainerDlg)//here we don't have a trigger object because of the lack of the ExperimentManager, so we trigger it automatically to start
					tStimTimer.singleShot(100, this, SLOT(incrementExternalTrigger()));//incrementExternalTrigger();
				break;
			}
		}
	}
	return true;
}

bool GLWidgetWrapper::setExperimentManager(ExperimentManager *expManager)
{
	if(expManager)
	{
		pExperimentManager = expManager;
		bool bRetVal;
		if (thisMetaObject)
		{
			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTMANAGER_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(thisMetaObject->invokeMethod(this, FUNC_SETEXPERIMENTMANAGER, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(ExperimentManager*,pExperimentManager))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETEXPERIMENTMANAGER << ")!";		
				}		
			}
		}
		return true;
	}
	return false;
}

bool GLWidgetWrapper::expandExperimentBlockParameterValue(QString &sValue)
{
	if (pExperimentManager)
	{
		return pExperimentManager->expandExperimentBlockParameterValue(sValue);				
	}
	return false;
}

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

bool GLWidgetWrapper::startExperimentObject()
{
	//QString a = thisMetaObject->className();
	if (bCheckForDoubleBuffering)
	{
		if(!this->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
		{
			qDebug() << __FUNCTION__ << "RetinoMap_glwidget::No Double Buffering available!";
			stimContainerDlg->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
			return false;
		}
	}
	//else
	//{
	//	QGLFormat newFormat;
	//	newFormat.setDoubleBuffer(false);
	//	this->setFormat(newFormat);
	//	if(this->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
	//	{
	//		bool a = true;
	//	}
	//	else
	//	{
	//		bfool b = true;
	//	}
	//}

	bFirstTriggerAfterUnlock = true;
	pExperimentManager->startExperimentTimer(nFrameTimerIndex);//Starts the Frame timer
	pExperimentManager->startExperimentTimer(nTrialTimerIndex);//Starts the Trial timer
	checkForNextBlockTrial();
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STARTOBJECT_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_STARTOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_STARTOBJECT_FULL << ")!";		
			}		
		}
	}
	return true;
}

bool GLWidgetWrapper::setExperimentObjectReadyToUnlock()
{
	if (bCurrentSubObjectReadyToUnlock == false)
	{
		bCurrentSubObjectReadyToUnlock = true;
		if(isDebugMode() && pExperimentManager)
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Ready to Unlock the Experiment Object");
	}
	return true;
}

bool GLWidgetWrapper::unlockExperimentObject()
{
	if (bCurrentSubObjectIsLocked)
	{
		bCurrentSubObjectIsLocked = false;
		if(isDebugMode() && pExperimentManager)
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Unlocked the Experiment Object");
	}
	return true;
}

bool GLWidgetWrapper::initExperimentObject()
{
	nFrameTimerIndex = pExperimentManager->createExperimentTimer();
	nTrialTimerIndex = pExperimentManager->createExperimentTimer();
	bCurrentSubObjectIsLocked = true;
	bCurrentSubObjectReadyToUnlock = false;
	nRefreshRate = 0;
	dAdditionalRefreshDelayTime = 0.0;
	insertExpObjectParameter(nObjectID,GLWWRAP_WIDGET_STIMULI_REFRESHRATE,nRefreshRate);
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_INITOBJECT_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_INITOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_INITOBJECT_FULL << ")!";		
			}		
		}
	}	
	int iCPU = omp_get_num_procs(); // Get the number of processors in this system
    qDebug() << __FUNCTION__ << "::" << iCPU << "processors available.";//The omp_get_num_threads() call returns 1 in the serial section of the code!!
	omp_set_num_threads(iCPU); // Set the number of threads
	bool bResult = connect(pExperimentManager->getExperimentStructure(),SIGNAL(experimentStopped()),this,SLOT(ExperimentShouldFinish()));
	return true;
}

bool GLWidgetWrapper::isDebugMode() 
{
	if (pExperimentManager)
	{
		if(pExperimentManager->getExperimentStructure())
			return pExperimentManager->getExperimentStructure()->getExperimentDebugMode();
	}
	return false;
}

QString GLWidgetWrapper::getLastLoggedObjectStateTime(ExperimentSubObjectState state)
{
	if (!subObjectStateHistory.nState.isEmpty())
	{
		int nHistoryCount = subObjectStateHistory.nState.count();
		if (nHistoryCount > 0)
		{
			for (int i=nHistoryCount-1;i>0;i--)
			{
				if (subObjectStateHistory.nState.at(i) == state)
				{
					return subObjectStateHistory.sDateTimeStamp.at(i);
				}
			}
		} 
	}
	return "";
}

void GLWidgetWrapper::closeEvent(QCloseEvent *evt)
{
	QGLWidget::closeEvent(evt);
}

bool GLWidgetWrapper::stopExperimentObject()
{
	bExperimentShouldStop = true;
	thisMetaObject->invokeMethod(this, "finalizePaintEvent",Qt::QueuedConnection);//a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STOPOBJECT_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_STOPOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_STOPOBJECT_FULL << ")!";		
			}		
		}
	}
	return true;
}

bool GLWidgetWrapper::abortExperimentObject()
{
	return stopExperimentObject();
}

void GLWidgetWrapper::paintEvent(QPaintEvent *event)
{
	int nPaintFlags;
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Starting to paint the widget");
	if (bCurrentSubObjectIsLocked == false)
	{
		nPaintFlags = (int)GLWidgetPaintFlags_NoFlag;
	//}
	//else
	//{
	//	nPaintFlags = (int)GLWidgetPaintFlags_LockedState;
	//}

		bool bRetVal = false;		
		if (thisMetaObject)
		{
			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PAINTOBJECT_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(thisMetaObject->invokeMethod(this, FUNC_PAINTOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int,nPaintFlags),Q_ARG(QObject*, (QObject*)event))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_PAINTOBJECT_FULL << ")!";		
				}		
			}
		}
	}
	else
	{
		if(isDebugMode() && pExperimentManager) 
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Starting to paint the initial widget");
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","pre Step 1");
		if (lockedPainter == NULL)
			lockedPainter = new QPainter(this);//Constructor automatically calls begin()
		else
			lockedPainter->begin(this);
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 1");
		QFont textFont("arial", 22, QFont::Bold, false);
		QString strText;
		QString strTextStart = "Experiment ready, ";
		if (bCurrentSubObjectReadyToUnlock)
			strText = strTextStart + "waiting for a trigger to start...";
		else
			strText = strTextStart + "press 'Alt' to proceed or CTRL + 'a' to abort the experiment";
		const QRectF windowRect = lockedPainter->window();
		//if(isDebugMode() && pExperimentManager)
		//	pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 2");
		const int nBorder = 25;
		QPainterPath textPath;
		textPath.addText(0,0,textFont,strText);
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
		pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","painter.end()");
	finalizePaintEvent();
}

void GLWidgetWrapper::incrementExternalTrigger()
{
/*! \brief Increments the External Trigger.
 *
 *  Invoke this slot to increment the External Trigger.
 */
	if (bCurrentSubObjectIsLocked)
	{
		if (bCurrentSubObjectReadyToUnlock == true)
		{
			bCurrentSubObjectReadyToUnlock = false;
			unlockExperimentObject();
			if(pExperimentManager)
			{
				pExperimentManager->getExperimentStructure()->resetExperiment();
				pExperimentManager->getExperimentStructure()->prepareExperiment();
				pExperimentManager->getExperimentStructure()->incrementExternalTrigger();
			}
			else
			{//No Experiment Manager? QML?
				update();
				//QTimer::singleShot(1000, this, SLOT(incrementExternalTrigger()));//Let's start!
				//QMetaObject::invokeMethod()
			}
		}
	}
	else
	{
		if(pExperimentManager)
		{
			pExperimentManager->getExperimentStructure()->incrementExternalTrigger();
			emit ExternalTriggerIncremented(pExperimentManager->getExperimentStructure()->getExternalTriggerCount());//Should be automatically connected!!
			if(isDebugMode() && pExperimentManager)
				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Externally Triggered!, Received="),QString::number(pExperimentManager->getExperimentStructure()->getExternalTriggerCount()));
		}
		else
		{
			emit ExternalTriggerIncremented(-1);
			if(isDebugMode() && pExperimentManager)
				qDebug() << __FUNCTION__ << "Automatically Externally Triggered!, no experiment manager active.";
		}
	}
}

void GLWidgetWrapper::initBlockTrial()
{
	emit NewInitBlockTrial();
	bExperimentShouldStop = false;
//#ifdef Q_WS_MACX
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
//#else
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
//#endif
	expTrialTimer.restart();
	if(pExperimentManager)
	{
		bool bHasCurrentBlock = false;
		cBlockStructure tmpBlock = pExperimentManager->getExperimentStructure()->getCurrentBlock(bHasCurrentBlock);
		if(bHasCurrentBlock)
			pExperimentManager->fetchExperimentBlockParamsFromDomNodeList(tmpBlock.getBlockNumber(),nObjectID);
		else
			qDebug() << __FUNCTION__ << "::Could not create a Block Parameter List, no current block!";
	}
	else
	{
		qDebug() << __FUNCTION__ << "::No ExperimentManager defined!";
	}
	ParsedParameterDefinition pParDef;
	pParDef = getExpObjectBlockParameter(nObjectID,GLWWRAP_WIDGET_STIMULI_REFRESHRATE,QString::number(nRefreshRate));
	nRefreshRate = pParDef.sValue.toInt();
	bool bRetVal;
	if (thisMetaObject)
	{
		if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_INITOBJECTBLOCKTRIAL_FULL)) == -1))//Is the slot present?
		{
			//Invoke the slot
			if(!(thisMetaObject->invokeMethod(this, FUNC_INITOBJECTBLOCKTRIAL, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
			{
				qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_INITOBJECTBLOCKTRIAL_FULL << ")!";		
			}		
		}
	}
}

int GLWidgetWrapper::checkForNextBlockTrial()
{
	if (bExperimentShouldStop)
		return 0;
	if (pExperimentManager == NULL)
	{
		if(bCurrentSubObjectIsLocked)
			return -1;
		//qDebug() << __FUNCTION__ << "::No Experiment Manager set!";
		return -1;//Special case... direct QML execute
	}
	bool goToNextBlockTrial = false;
	bool bFirstCheckAfterExperimentStarted = false;	
	int nRetval = 0;
	bool bExperimentStructureChanged = false;
	bool bHasCurrentBlock = false;
	cExperimentStructure tmpExpStr = cExperimentStructure(*pExperimentManager->getExperimentStructure());
	cBlockStructure tmpExpBlockStr = tmpExpStr.getCurrentBlock(bHasCurrentBlock);
	strcExperimentStructureState tmpExpStrState = tmpExpStr.getCurrentExperimentState();

	if((tmpExpStrState.Experiment_ExternalTrigger == RA_REINITIALIZE) && (nCurrExpBlockTrialFrame == CF_UNINITIALIZED))
	{//First Experiment Trial? (Start/Init), occurs before the start of the Trigger(timer)
		goToNextBlockTrial = true;
		bFirstCheckAfterExperimentStarted = true;		
		bExperimentStructureChanged = true;
	}
	if ((tmpExpStrState.Experiment_ExternalTrigger >= 0) && (tmpExpStrState.Experiment_ExternalTrigger > nLastProcessedExternalTriggers))
	{
		nLastProcessedExternalTriggers = tmpExpStrState.Experiment_ExternalTrigger;
		bExperimentStructureChanged = true;
	}
	if(bFirstCheckAfterExperimentStarted == false)
	{
		if(tmpExpStr.getBlockCount() > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
			if(bHasCurrentBlock)
				goToNextBlockTrial = (tmpExpStrState.CurrentBlock_ExternalTrigger == 0);//Go to next Block Trial?
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		bExperimentStructureChanged = true;
		nCurrExpBlockTrialFrame = 0;
		if(pExperimentManager)
			pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Starting to Init new BlockTrial"),QString("BlockNumber=") + QString::number(tmpExpBlockStr.getBlockNumber()) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +  ", TrialExternalTrigger=" + QString::number(tmpExpStrState.CurrentBlock_ExternalTrigger) + ", TrialInternalTrigger=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(nCurrExpBlockTrialFrame) + ")");
		initBlockTrial();
		if (bFirstCheckAfterExperimentStarted)
		{
			changeSubObjectState(Experiment_SubObject_Started);
		}
		if (bCurrentSubObjectIsLocked == false)
		{
			if((isDebugMode()) && (pExperimentManager))
				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","New BlockTrial Prepared, going to qeueu animate()");
			QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
		}
		else
		{
			if(tmpExpStr.getBlockCount() > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
				if(bHasCurrentBlock)
					QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));
		}
		expTrialTimer.restart();
		nRetval = 1;
	}
	else
	{
		nRetval = 0;
	}
	if ((bCurrentSubObjectIsLocked == false) && (bFirstTriggerAfterUnlock))//has the experiment just been unlocked for the first time?
	{
		bFirstTriggerAfterUnlock = false;
		emit ExperimentStructureChanged(tmpExpBlockStr.getBlockNumber(),tmpExpStrState.CurrentBlock_TrialNumber,tmpExpStrState.CurrentBlock_InternalTrigger);
	}
	if ((bExperimentStructureChanged) && (bFirstCheckAfterExperimentStarted == false))
	{
		if(getSubObjectState() == Experiment_SubObject_Started)
			emit ExperimentStructureChanged(tmpExpBlockStr.getBlockNumber(),tmpExpStrState.CurrentBlock_TrialNumber,tmpExpStrState.CurrentBlock_InternalTrigger);
	}
	return nRetval;
}

void GLWidgetWrapper::animate(bool bOnlyCheckBlockTrials)
{
	//QObject *a = sender();	
	if(getSubObjectState() == Experiment_SubObject_Started)
	{
		if(mutRecursivePaint.tryLock())
		{
			if (bExperimentShouldStop)
			{
				changeSubObjectState(Experiment_SubObject_Stop);
				mutRecursivePaint.unlock();
				return;
			}

			if((isDebugMode()) && (pExperimentManager))
				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Going to call update()");
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
				update();
			}
			if((isDebugMode()) && (pExperimentManager))
				pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","update() called");
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

bool GLWidgetWrapper::changeSubObjectState(ExperimentSubObjectState newSubObjectState)
{
	if(newSubObjectState != currentSubObjectState)
	{
		if((((int)newSubObjectState > (int)ExperimentSubObjectState::Experiment_SubObject_MaxState) || ((int)newSubObjectState < 0)) ||
		  (((int)currentSubObjectState > (int)ExperimentSubObjectState::Experiment_SubObject_MaxState) || ((int)currentSubObjectState < 0)) ||
		  ((int)currentSubObjectState == Experiment_SubObject_Destructing))
		{
			return false;
		}
		currentSubObjectState = newSubObjectState;
		subObjectStateHistory.nState.append(currentSubObjectState);
		subObjectStateHistory.sDateTimeStamp.append(QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
		emit ObjectStateHasChanged(currentSubObjectState);
		if(currentSubObjectState == Experiment_SubObject_Stop)
		{
			qApp->setActiveWindow(MainAppInfo::getMainWindow());
		}
		return true;
	}
	return false;
}

void GLWidgetWrapper::finalizePaintEvent() 
{
	double dCurrentTime;
	double dFramePeriodTime;
	bool bObjectIsLocked = bCurrentSubObjectIsLocked;//buffer it..

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
			if((tmpExpStrState.Experiment_ExternalTrigger == 0) && (nCurrExpBlockTrialFrame == 0))
			{
				dLastPreSwapTime = currentPreSwapTime;//Just set this initial value and proceed
			}
			else
			{
				dWaitTime = (dLastPreSwapTime + dFramePeriodTime) - currentPreSwapTime;
				if(dWaitTime > dAdditionalRefreshDelayTime)//Do we need to wait?
				{
					if(isDebugMode() && pExperimentManager)
						pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Too fast --> Sleeping(" + QString::number(dWaitTime-dAdditionalRefreshDelayTime) + ")... Locked="),QString::number(bObjectIsLocked);
					ExperimentTimer::SleepMSecAccurate(dWaitTime-dAdditionalRefreshDelayTime);
				}
				else if(dWaitTime < 0.0f)//This means that a frame is going to be skipped!
				{					
					while(dWaitTime < 0.0f)//Search the first next available frame threshold time
					{
						pExperimentManager->logExperimentObjectData(nObjectID,-1,__FUNCTION__,"","Frame skipped!, missed=", QString::number(dWaitTime));
						dLastPreSwapTime = dLastPreSwapTime + dFramePeriodTime;
						dWaitTime = (dLastPreSwapTime + dFramePeriodTime) - currentPreSwapTime;
					}
					if(isDebugMode() && pExperimentManager)
						pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Sleeping for next frame(" + QString::number(dWaitTime-dAdditionalRefreshDelayTime) + ")...") + QString::number(bObjectIsLocked);
					ExperimentTimer::SleepMSecAccurate(dWaitTime-dAdditionalRefreshDelayTime);
				}
				dLastPreSwapTime = dLastPreSwapTime + dFramePeriodTime;
			}
		}
		dCurrentTime = pExperimentManager->restartExperimentTimer(nFrameTimerIndex);
	}
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Going to Swap, locked=", QString::number(bObjectIsLocked));
	swapBuffers();
	if(isDebugMode() && pExperimentManager)
		pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","BlockTrial Buffer Swapped, locked=",QString::number(bObjectIsLocked));
	if (bExperimentShouldStop)
	{
		changeSubObjectState(Experiment_SubObject_Stop);
		return;
	}
	//qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,1); //!Important: To receive Trigger Signals and process them before the below checkForNextBlockTrial();
	if (bExperimentShouldStop)
	{
		changeSubObjectState(Experiment_SubObject_Stop);
		return;
	}
	if (bExperimentShouldStop)
	{
		changeSubObjectState(Experiment_SubObject_Stop);
		return;
	}
	if (bObjectIsLocked==false)
	{
		nCurrExpBlockTrialFrame++;// = CF_UNINITIALIZED;
	}
	if((nRefreshRate > 0) && (bObjectIsLocked==false))
	{
		if(pExperimentManager)
		{
			if (dCurrentTime > (dFramePeriodTime*1.5f))
				pExperimentManager->logExperimentObjectData(nObjectID,-1,__FUNCTION__,"","Paint routine took too long(" + QString::number(dCurrentTime) + " mSecs),(BlockID=" + QString::number(tmpExpStrState.CurrentBlock_BlockID) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +", Trigger(Int)=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(nCurrExpBlockTrialFrame) + ")",QString::number(dCurrentTime));
			else if(isDebugMode() && pExperimentManager)
				pExperimentManager->logExperimentObjectData(nObjectID,-1,__FUNCTION__,"","Paint routine took(" + QString::number(dCurrentTime) + " mSecs),(BlockID=" + QString::number(tmpExpStrState.CurrentBlock_BlockID) + ", TrialNumber=" + QString::number(tmpExpStrState.CurrentBlock_TrialNumber) +", Trigger(Int)=" + QString::number(tmpExpStrState.CurrentBlock_InternalTrigger) + ", Frame=" + QString::number(nCurrExpBlockTrialFrame) + ")",QString::number(dCurrentTime));
		}
	}
	int nResult = checkForNextBlockTrial();//Check whether we need to prepare for an new block Trial
	if(nResult == 0) //No Block Trials to check
	{
		if (bExperimentShouldStop)
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

QScriptValue GLWidgetWrapper::getExperimentObjectParameter(const int &nObjectID, const QString &strName)
{
/*! \brief retrieves the current value of an Experiment Parameter variable.
 *
 *  This function can be used to retrieve the current value of an Experiment Parameter variable for a specific object.
 *	This parameter variable value is currently in use.
 */
	if (currentScriptEngine)
	{
		//return currentScriptEngine->toScriptValue(getExpObjectVariabelePointer<QString>(nObjectID,strName));
		QScriptValue sScriptValue;
		if(pExperimentManager->getExperimentObjectScriptValue(nObjectID,strName,sScriptValue))
		{
			//QString test = sScriptValue.toString();
			return sScriptValue;
		}
		return currentScriptEngine->undefinedValue();
	}
	qDebug() << __FUNCTION__ << ":No Script Engine available!";
	return NULL;	
}

bool GLWidgetWrapper::setExperimentObjectParameter(const int &nObjectID, const QString &strName, const QScriptValue &sScriptVal)
{
/*! \brief Sets the current value of an Experiment Parameter variable.
 *
 *  This function can be used to immediately set a value of an Experiment Parameter variable for a specific object.
 *	This parameter variable value is immediately set and used.
 */
	return pExperimentManager->setExperimentObjectFromScriptValue(nObjectID,strName,sScriptVal);
}