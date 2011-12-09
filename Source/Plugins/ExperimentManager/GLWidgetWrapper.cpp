#include <QtGui>
#include <stdlib.h>
#include <math.h>
#include <QDomDocument>
#include "GLWidgetWrapper.h"
#include "ExperimentManager.h"

GLWidgetWrapper::GLWidgetWrapper(QWidget *parent) : QGLWidget(parent)
{
	init();
	stimContainerDlg = NULL;
	pExpBlockTrialDomNodeList = NULL;
	bForceToStop = false;
	bExperimentShouldStop = false;
	#ifdef Q_WS_MACX
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
	#elseabot
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
	#endif
	setAutoFillBackground(false);
	setAutoBufferSwap(false); // in order to have control over time point for buffer swap
	rScreenResolution = QApplication::desktop()->screenGeometry();//dDesktopWidget->screenGeometry();//availableGeometry();
	//setFixedSize(rScreenResolution.width(), rScreenResolution.height());
	setStimuliResolution(rScreenResolution.width(), rScreenResolution.height());
	//setFixedSize(768,768);
	//setMinimumSize(200, 200);
	setWindowTitle(tr("Painting a Scene"));

	stimContainerDlg = new ContainerDlg();
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	mainLayout = NULL;
	nObjectID = -1;
	pExpConf = NULL;
	tEventObjectStopped = (QEvent::Type)(QEvent::User + 1);	
	ExpBlockParams = NULL;
	nFrameTimerIndex = -1;
	nTrialTimerIndex = -1;
	setVerticalSyncSwap();
}

GLWidgetWrapper::~GLWidgetWrapper()
{
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
	cleanupExperimentBlockTrialStructure();
	changeSubObjectState(Experiment_SubObject_Stopped);
}

void GLWidgetWrapper::setVerticalSyncSwap()
{
	QGLFormat StimulGLQGLFormat;
	StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
	StimulGLQGLFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(StimulGLQGLFormat);
}


bool GLWidgetWrapper::insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue)
{
	if (ExpBlockParams == NULL)
	{
		ExpBlockParams = new QHash<QString, QString>();
		pExpConf->pExperimentManager->setExperimentObjectBlockParameterStructure(nObjectID,ExpBlockParams);
	}
	return (pExpConf->pExperimentManager->insertExperimentObjectBlockParameter(nObjectID,sName,sValue));
}

QString GLWidgetWrapper::getExperimentObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue)
{
	QString sResult = sDefValue;
	if (pExpConf->pExperimentManager->getExperimentObjectBlockParameter(nObjectID,sName,sResult))
		return sResult;
	else
		return sDefValue;
}

void GLWidgetWrapper::init()
{
	cleanupExperimentBlockTrialStructure();
	nCurrentExperimentTrial = -1;
	nCurrentExperimentBlock = 0;
	nTotalProcessedExperimentTrials = 0;
	nCurrentExperimentReceivedTriggers = 0;
	nCurrentExperimentTrigger = 0;
	nCurrentExperimentBlockTrialReceivedTriggers = 0;
	nBlockTrialFrameCounter = 0;
	nNextThresholdTriggerCount = 0;
	changeSubObjectState(Experiment_SubObject_Initialized);
}

QRectF GLWidgetWrapper::getScreenResolution()
{
	//int a = this->format().swapInterval();
	//a = a;
	return rScreenResolution;
	//return QRectF(0.0f,0.0f,768.0f,768.0f);
}

void GLWidgetWrapper::setStimuliResolution(int w, int h)
{
	setFixedSize(w,h);
}

int GLWidgetWrapper::getObjectID()
{
	return nObjectID;
}

bool GLWidgetWrapper::setExperimentObjectID(int nObjID)
{
	if(nObjID >= 0)
	{
		nObjectID = nObjID;
		return true;
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

bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
{
	if (target == stimContainerDlg) 
	{
		if (event->type() == QEvent::KeyPress) 
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
			switch (keyEvent->key())
			{
			case Qt::Key_Escape:
				if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->modifiers() & Qt::AltModifier))
				{
					emit UserWantsToClose();
				}
				break;
			}
		}
	}
	return true;
}

bool GLWidgetWrapper::setBlockTrialDomNodeList(QDomNodeList *pDomNodeList)
{
	if(pDomNodeList)
	{
		pExpBlockTrialDomNodeList = pDomNodeList;
		if (updateExperimentBlockTrialStructure())
			return true;
	}
	return false;
}

bool GLWidgetWrapper::setExperimentConfiguration(ExperimentConfiguration *pExpConfStruct)
{
	if(pExpConfStruct)
	{
		pExpConf = pExpConfStruct;
		return true;
	}
	return false;
}

bool GLWidgetWrapper::updateExperimentBlockTrialStructure()
{
	if(cleanupExperimentBlockTrialStructure() == false)
		return false;
	if(pExpBlockTrialDomNodeList == NULL)
		return false;
	if (pExpBlockTrialDomNodeList->count() == 0)
		return false;

	strcExperimentBlockTrials.nNrOfBlocks = pExpBlockTrialDomNodeList->count();
	if (!(strcExperimentBlockTrials.nNrOfBlocks > 0))
		return false;

	bool bUpdateSucceeded = false;
	QDomElement tmpElement;

	//Loop through the blocks
	for(int i=0;i<strcExperimentBlockTrials.nNrOfBlocks;i++)
	{
		bUpdateSucceeded = false;
		if(pExpBlockTrialDomNodeList->at(i).hasChildNodes())
		{
			tmpElement = pExpBlockTrialDomNodeList->at(i).toElement();
			if(tmpElement.hasAttribute(ID_TAG))
			{
				BlockTrialStructure tmpBlockTrialStruct;
				TrialStructure tmpTrialStruct;
				tmpBlockTrialStruct.nBlockID = tmpElement.attribute(ID_TAG,"").toInt();//Copy the BlockID

				tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(BLOCKNUMBER_TAG);
				if(!tmpElement.isNull())//Is there a block_number defined?
				{
					tmpBlockTrialStruct.nBlockNumber = tmpElement.text().toInt();//Copy the BlockNumber
					if (tmpBlockTrialStruct.nBlockNumber>=0)
					{
						tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(TRIALAMOUNT_TAG);
						if(!tmpElement.isNull())//Is there a TrialAmount defined?
						{
							tmpBlockTrialStruct.nNrOfTrials = tmpElement.text().toInt();//Copy the TrialAmount
							if (tmpBlockTrialStruct.nNrOfTrials<0)
							{
								tmpBlockTrialStruct.nNrOfTrials = 1;//Error set it to Default
							}
						}
						else
						{
							tmpBlockTrialStruct.nNrOfTrials = 1;//Default if not defined
						}

						tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(TRIGGERAMOUNT_TAG);
						int nTriggerAmount = 1;//Default
						if(!tmpElement.isNull())//Is there a TriggerAmount defined?
						{
							if (!tmpElement.text().isEmpty())
							{
								if (tmpElement.text().toInt() > 0)
								{
									nTriggerAmount = tmpElement.text().toInt();
								}
							}
						}

						for (int j=0;j<tmpBlockTrialStruct.nNrOfTrials;j++)
						{
							tmpTrialStruct.nTrialID = j;
							tmpTrialStruct.nTrialNumber = j;
							tmpTrialStruct.nNrOfTriggers = nTriggerAmount;//Default if not defined
							tmpBlockTrialStruct.lTrialStructure.append(tmpTrialStruct);
						}							

						tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(NAME_TAG);
						if(!tmpElement.isNull())//Is there a Name defined?
						{
							tmpBlockTrialStruct.nBlockName = tmpElement.text();//Copy the Name
						}
						else
						{
							tmpBlockTrialStruct.nBlockName = "";
						}
						strcExperimentBlockTrials.lBlockTrialStructure.append(tmpBlockTrialStruct);
						
						//Loop through the trials
						//??

						bUpdateSucceeded = true;
					}
				}
			}
		}
		if(!bUpdateSucceeded)
		{
			cleanupExperimentBlockTrialStructure();
			return false;
		}
	}
	return true;
}

int GLWidgetWrapper::getExperimentBlockTrialTriggerAmount(int nBlock, int nTrial)
{
	if(strcExperimentBlockTrials.nNrOfBlocks > nBlock)
	{
		if (strcExperimentBlockTrials.lBlockTrialStructure.at(nBlock).nNrOfTrials > nTrial)
		{
			return strcExperimentBlockTrials.lBlockTrialStructure.at(nBlock).lTrialStructure.at(nTrial).nNrOfTriggers;
		}
	}
	return -1;
}

//bool GLWidgetWrapper::getExperimentTriggerCount(int &nTriggerCount)
//{
//	int nBlockAmount = strcExperimentBlockTrials.nNrOfBlocks;
//	if(nBlockAmount > 0)
//	{
//		int nTrialAmount = 0;
//		for (int i=0;i<nBlockAmount;i++)
//		{
//			if (strcExperimentBlockTrials.lBlockTrialStructure.at(i).nNrOfTrials > 0)
//			{
//				nTrialAmount = nTrialAmount + strcExperimentBlockTrials.lBlockTrialStructure.at(i).nNrOfTrials;
//			}
//			else
//				return false;
//		}
//		nTriggerCount = nTrialAmount;
//		return true;
//	}
//	return false;
//}

bool GLWidgetWrapper::cleanupExperimentBlockTrialStructure()
{
	//for (int i;i<strcExperimentBlockTrials.lBlockTrialStructure.count();i++)
	//{
		strcExperimentBlockTrials.lBlockTrialStructure.clear();
	//}
	strcExperimentBlockTrials.nNrOfBlocks = 0;
	return true;
}

bool GLWidgetWrapper::getExperimentBlockParameters(int nBlockNumber, int nObjectID, QHash<QString, QString> *hParams)
{
	if (hParams == NULL)
		return false;
	if(hParams->count() == 0)
		return false;
	if(pExpBlockTrialDomNodeList == NULL)
		return false;
	if (pExpBlockTrialDomNodeList->count() == 0)
		return false;

	int nBlockCount = pExpBlockTrialDomNodeList->count();
	if (!(nBlockCount > 0))
		return false;

	QDomElement tmpElement;
	QDomNode tmpNode;
	QString tmpString;

	for(int i=0;i<nBlockCount;i++)//Loop through the blocks
	{
		if(pExpBlockTrialDomNodeList->at(i).hasChildNodes())
		{
			tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(BLOCKNUMBER_TAG);
			if(!tmpElement.isNull())//Is there a block_number defined?
			{
				if (nBlockNumber == tmpElement.text().toInt())//Correct block number?
				{
					QDomNodeList tmpObjectNodeList = pExpBlockTrialDomNodeList->at(i).toElement().elementsByTagName(OBJECT_TAG);//Retrieve all the objects
					int nObjectListCount = tmpObjectNodeList.count();
					if (nObjectListCount>0)
					{
						for (int j=0;j<nObjectListCount;j++)//For each object
						{
							if(tmpObjectNodeList.item(j).toElement().hasAttribute(ID_TAG))
							{
								if(nObjectID == tmpObjectNodeList.item(j).toElement().attribute(ID_TAG,"").toInt())//Correct ObjectID?
								{
									if(tmpObjectNodeList.item(j).firstChildElement(PARAMETERS_TAG).isElement())
									{
										tmpElement = tmpObjectNodeList.item(j).firstChildElement(PARAMETERS_TAG);
										QDomNodeList tmpParameterNodeList = tmpElement.elementsByTagName(PARAMETER_TAG);//Retrieve all the parameters
										int nParameterListCount = tmpParameterNodeList.count();
										if (nParameterListCount>0)
										{
											bool bResult = false;
											for (int k=0;k<nParameterListCount;k++)//For each parameter
											{
												tmpElement = tmpParameterNodeList.item(k).firstChildElement(NAME_TAG);
												if(!tmpElement.isNull())
												{
													tmpString = tmpElement.text().toLower();
													if (hParams->contains(tmpString))//Is the Parameter available in the plugin?
													{
														tmpElement = tmpParameterNodeList.item(k).firstChildElement(VALUE_TAG);
														if(!tmpElement.isNull())
														{
															hParams->insert(tmpString,tmpElement.text().toLower());
															bResult = true;//To define that at least one parameter was parsed successfully
														}
													}
												}
												if(k==(nParameterListCount-1))
													return bResult;											
											}
										}
										else
											return false;
									}
									else
										return false;
								}
								else
									return false;
							}
							else
								return false;
						}
					}
					else
						return false;//Nothing defined		
				}
				else
					continue;//Search next block
			}
			else
				continue;//Search next block
		}
	}
	return false;
}

bool GLWidgetWrapper::getExperimentBlockParameter(int nBlockNumber, int nObjectID, QString strParamName, QString &Result)
{
	if(pExpBlockTrialDomNodeList == NULL)
		return false;
	if (pExpBlockTrialDomNodeList->count() == 0)
		return false;

	int nBlockCount = pExpBlockTrialDomNodeList->count();
	if (!(nBlockCount > 0))
		return false;

	QDomElement tmpElement;
	QDomNode tmpNode;

	for(int i=0;i<nBlockCount;i++)//Loop through the blocks
	{
		if(pExpBlockTrialDomNodeList->at(i).hasChildNodes())
		{
			tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(BLOCKNUMBER_TAG);
			if(!tmpElement.isNull())//Is there a block_number defined?
			{
				if (nBlockNumber == tmpElement.text().toInt())//Correct block number?
				{
					QDomNodeList tmpObjectNodeList = pExpBlockTrialDomNodeList->at(i).toElement().elementsByTagName(OBJECT_TAG);//Retrieve all the objects
					int nObjectListCount = tmpObjectNodeList.count();
					if (nObjectListCount>0)
					{
						for (int j=0;j<nObjectListCount;j++)//For each object
						{
							if(tmpObjectNodeList.item(j).toElement().hasAttribute(ID_TAG))
							{
								if(nObjectID == tmpObjectNodeList.item(j).toElement().attribute(ID_TAG,"").toInt())//Correct ObjectID?
								{
									if(tmpObjectNodeList.item(j).firstChildElement(PARAMETERS_TAG).isElement())
									{
										tmpElement = tmpObjectNodeList.item(j).firstChildElement(PARAMETERS_TAG);
										QDomNodeList tmpParameterNodeList = tmpElement.elementsByTagName(PARAMETER_TAG);//Retrieve all the parameters
										int nParameterListCount = tmpParameterNodeList.count();
										if (nParameterListCount>0)
										{
											for (int k=0;k<nParameterListCount;k++)//For each parameter
											{
												tmpElement = tmpParameterNodeList.item(k).firstChildElement(NAME_TAG);
												if(!tmpElement.isNull())
												{
													if(strParamName.toLower() == tmpElement.text().toLower())//Correct Parameter Name?
													{
														tmpElement = tmpParameterNodeList.item(k).firstChildElement(VALUE_TAG);
														if(!tmpElement.isNull())
														{
															Result = tmpElement.text().toLower();
															return true;
														}
														else
															return false;
													}
												}
												else
													continue;//Next parameter maybe?
											}
										}
										else
											return false;
									}
									else
										return false;
								}
								else
									return false;
							}
							else
								return false;
						}
					}
					else
						return false;//Nothing defined		
				}
				else
					continue;//Search next block
			}
			else
				continue;//Search next block
		}
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
	//if(m_RunFullScreen)
		stimContainerDlg->showFullScreen();
	//else
	//	stimContainerDlg->show();
}

bool GLWidgetWrapper::startExperimentObject()
{
	if(!this->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
	{
		//qDebug() << "RetinoMap_glwidget::start::No Double Buffering available!";
		stimContainerDlg->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
		return false;
	}
	pExpConf->pExperimentManager->startExperimentTimer(nFrameTimerIndex);//Starts the Frame timer
	pExpConf->pExperimentManager->startExperimentTimer(nTrialTimerIndex);//Starts the Trial timer
	checkForNextBlockTrial();
	return true;
}

bool GLWidgetWrapper::initExperimentObject()
{
	nFrameTimerIndex = pExpConf->pExperimentManager->createExperimentTimer();
	nTrialTimerIndex = pExpConf->pExperimentManager->createExperimentTimer();

	nRefreshRate = 0;
	dAdditionalRefreshDelayTime = 0.0;
	insertExperimentObjectBlockParameter(nObjectID,RETINOMAP_WIDGET_DISPLAY_REFRESHRATE,QString::number(nRefreshRate));

	return true;
}

//int GLWidgetWrapper::getElapsedFrameTime() 
//{
//	return pExpConf->pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);
//}

bool GLWidgetWrapper::isDebugMode() 
{
	if (pExpConf)
	{
		return pExpConf->bDebugMode;
	}
	return false;
}

QString GLWidgetWrapper::getLastLoggedObjectStateTime(ExperimentSubObjectState state)
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
	return "";
}

void GLWidgetWrapper::closeEvent(QCloseEvent *evt)
{
	QGLWidget::closeEvent(evt);
}

bool GLWidgetWrapper::stopExperimentObject()
{
	bExperimentShouldStop = true;
	//int a = tTotalRunningTime.elapsed();
	//stopTriggerTimer();
	return true;
}

bool GLWidgetWrapper::abortExperimentObject()
{
	bForceToStop = true;
	//int a = tTotalRunningTime.elapsed();
	//stopTriggerTimer();
	return true;
}

//void GLWidgetWrapper::startTriggerTimer(int msTime)
//{
//	tTriggerTimer.setSingleShot(false);
//	connect(&tTriggerTimer, SIGNAL(timeout()), this, SLOT(incrementTrigger()));
//	tTriggerTimer.start(msTime);
//}

//void GLWidgetWrapper::stopTriggerTimer()
//{
//	tTriggerTimer.stop();
//	disconnect(&tTriggerTimer, SIGNAL(timeout()), this, SLOT(incrementTrigger()));
//}

void GLWidgetWrapper::paintEvent(QPaintEvent *event)
{
	if(isDebugMode())
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"paintEvent():Starting to paint the widget");
}

void GLWidgetWrapper::incrementTrigger()
{
	nCurrentExperimentReceivedTriggers++;
}

void GLWidgetWrapper::initBlockTrial()
{
	bForceToStop = false;
	bExperimentShouldStop = false;
	nBlockTrialFrameCounter = 0;
//#ifdef Q_WS_MACX
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
//#else
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
//#endif
	getExperimentBlockParameters(getCurrentExperimentBlock(),nObjectID,ExpBlockParams);//Should be moved to the manager?!
	nRefreshRate = getExperimentObjectBlockParameter(nObjectID,RETINOMAP_WIDGET_DISPLAY_REFRESHRATE,QString::number(nRefreshRate)).toInt();
}

//bool GLWidgetWrapper::loadBlockTrial()
//{
//	return true;
//}

bool GLWidgetWrapper::checkForNextBlockTrial()
{
	if ((bForceToStop)||(bExperimentShouldStop))
		return false;	
	bool goToNextBlockTrial = false;
	bool bInitBeforeFirstExperimentTrial = false;
	nCurrentExperimentTrigger = nCurrentExperimentReceivedTriggers;//Take a snapshot!
	if(nCurrentExperimentTrial == -1) //First Experiment Trial? (Start/Init), occurs before the start of the Trigger(timer).
	{
		nTotalProcessedExperimentTrials = 0;
		goToNextBlockTrial = true;
		nCurrentExperimentBlock = 0;
		bInitBeforeFirstExperimentTrial = true;
	}
	else
	{
		goToNextBlockTrial = nCurrentExperimentTrigger >= nNextThresholdTriggerCount;//Go to next Block Trial?
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		if((nCurrentExperimentTrial+1) >= strcExperimentBlockTrials.lBlockTrialStructure[nCurrentExperimentBlock].nNrOfTrials)//End of this Block(No more Trials)?
		{			
			if ((nCurrentExperimentBlock+1) >= strcExperimentBlockTrials.nNrOfBlocks)//No more blocks?
			{
					QCoreApplication::postEvent(this,new QEvent(tEventObjectStopped));//,Qt::HighEventPriority);
					bExperimentShouldStop = true;
					return false;
			} 
			else
			{
				nCurrentExperimentTrial = 0;//Reset the Experiment Trial Counter
				nCurrentExperimentBlock++;//Increment the Experiment Block Counter
				nCurrentExperimentBlockTrialReceivedTriggers = 0;
			}
		}
		else
		{
			nCurrentExperimentTrial++;//Increment the Experiment Trial Counter
			nCurrentExperimentBlockTrialReceivedTriggers = 0;
		}
		nNextThresholdTriggerCount += strcExperimentBlockTrials.lBlockTrialStructure[nCurrentExperimentBlock].lTrialStructure[nCurrentExperimentTrial].nNrOfTriggers;//increment the nextTimeThresholdTRs
		nBlockTrialFrameCounter = 0;
		if(isDebugMode())
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"checkForNextBlockTrial():Starting to Init new BlockTrial");
		initBlockTrial();		
		if (bInitBeforeFirstExperimentTrial)
		{
			changeSubObjectState(Experiment_SubObject_Started);
		}
		if(isDebugMode())
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"checkForNextBlockTrial():New BlockTrial Prepared, going to call animate()");
		tStimTimer.singleShot(1, this, SLOT(animate()));
		if(!bInitBeforeFirstExperimentTrial)
			nTotalProcessedExperimentTrials++;
		return true;
	}
	else
	{
		nCurrentExperimentBlockTrialReceivedTriggers = getRelativeBlockTrialTrigger(nCurrentExperimentTrigger);
		return false;
	}
}

int GLWidgetWrapper::getRelativeBlockTrialTrigger(int nAbsoluteTrigger)
{
	int nTriggersOutsideCurrentBlockTrial = 0;
	for (int i=0;i<strcExperimentBlockTrials.nNrOfBlocks;i++)
	{
		for (int j=0;j<strcExperimentBlockTrials.lBlockTrialStructure.at(i).nNrOfTrials;j++)
		{
			if ((nTriggersOutsideCurrentBlockTrial + (strcExperimentBlockTrials.lBlockTrialStructure.at(i).lTrialStructure.at(j).nNrOfTriggers))>nAbsoluteTrigger)
			{
				return (nAbsoluteTrigger - nTriggersOutsideCurrentBlockTrial);
			}
			else
			{
				nTriggersOutsideCurrentBlockTrial = nTriggersOutsideCurrentBlockTrial + strcExperimentBlockTrials.lBlockTrialStructure.at(i).lTrialStructure.at(j).nNrOfTriggers;
			}
		}
	}
	return -1;
}

void GLWidgetWrapper::animate()
{
	//if (((bForceToStop)||(bExperimentShouldStop)) == false)
	//{
		if(getSubObjectState() == Experiment_SubObject_Started)
		{
			if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"animate():Going to call repaint()");
			//repaint();
			update();
			if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"animate():repaint() called");
			//	elapsed = frameTime.elapsed();//Calculate the elapsed time since started
			//	frameTime.restart();
			//	repaint();//calls the below void GLWidget::paintEvent(QPaintEvent *event)
			//You should usually use 'update', as this will allow multiple queued paint events to be 'collapsed' into a single event. 
			//The update method will call updateGL for QGLWidgets. The 'repaint' method should be used if you want an immediate repaint.
			//If you have hooked up a timer to periodically call 'update', then failure to repaint regularly usually indicates 
			//that you're putting stress on the CPU.
			//}
		}
	//}
}

void GLWidgetWrapper::changeSubObjectState(ExperimentSubObjectState newSubObjectState)
{
	if(newSubObjectState != currentSubObjectState)
	{
		currentSubObjectState = newSubObjectState;
		subObjectStateHistory.nState.append(currentSubObjectState);
		subObjectStateHistory.sDateTimeStamp.append(QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
		emit ObjectStateHasChanged(currentSubObjectState);
	}
}

void GLWidgetWrapper::finalizePaintEvent() 
{
	double dCurrentTime;
	double dFramePeriodTime;
	dAdditionalRefreshDelayTime = dFramePeriodTime/5.0f;
	//if(isDebugMode())
	//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,nFrameTimerIndex,"finalizePaintEvent():ElapsedFrameTime");//Here we log the measured frame time (swapBuffers() + initBlockTrial() + paintEvent())
	if (nRefreshRate > 0)
	{
		dFramePeriodTime = 1000.0f/nRefreshRate; //DisplayRefreshRate
		double dWaitTime = 0;
		dCurrentTime = pExpConf->pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);	
		while((dCurrentTime)<dFramePeriodTime)//Don't go too fast...//((dCurrentTime+dAdditionalRefreshDelayTime)<dFramePeriodTime)//Don't go too fast...
		{
			dWaitTime = dFramePeriodTime - dCurrentTime;// + dAdditionalRefreshDelayTime;
			if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"finalizePaintEvent():Sleeping(" + QString::number(dWaitTime) + ")...");			
			ExperimentTimer::SleepMSecAccurate(dWaitTime);
			//Sleep(nSTime);
			dCurrentTime = pExpConf->pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);
		}
	}
	dCurrentTime = pExpConf->pExperimentManager->restartExperimentTimer(nFrameTimerIndex);
	if(isDebugMode())
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"finalizePaintEvent():Going to Swap");
	swapBuffers();
	if(isDebugMode())
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"finalizePaintEvent():BlockTrial Buffer Swapped");
	nBlockTrialFrameCounter++;
	
	
	
	//dCurrentTime = pExpConf->pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);
	//if (nRefreshRate > 0)
	//{
	//	if(dCurrentTime < dFramePeriodTime)
	//	{
	//		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"Too Fast: Additional Time = " + QString::number(dFramePeriodTime - dCurrentTime));
	//		ExperimentTimer::SleepMSecAccurate(dFramePeriodTime - dCurrentTime);
	//	}
	//	//if(dCurrentTime < (dFramePeriodTime-(dFramePeriodTime/20.0f)))//5% Change, too fast
	//	//{
	//	//	dAdditionalRefreshDelayTime = dAdditionalRefreshDelayTime + (dFramePeriodTime - (dFramePeriodTime/40.0f) - dCurrentTime);
	//	//	if(isDebugMode())
	//	//		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"Too Fast: Additional Time = " + QString::number(dAdditionalRefreshDelayTime));
	//	//}
	//	//else if(dCurrentTime > (dFramePeriodTime+(dFramePeriodTime/20.0f)))//5% Change, too slow
	//	//{
	//	//	if(dCurrentTime < ((2*dFramePeriodTime)-2.0f))
	//	//	{
	//	//		dAdditionalRefreshDelayTime = dAdditionalRefreshDelayTime - (dFramePeriodTime + (dFramePeriodTime/40.0f) - dCurrentTime);
	//	//		if(isDebugMode())
	//	//			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,"Too Slow: Additional Time = " + QString::number(dAdditionalRefreshDelayTime));
	//	//	}
	//	//	if(isDebugMode())
	//	//		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,-1,"finalizePaintEvent()::***Paint routine took to long(" + QString::number(dCurrentTime) + " mSecs),(Block=" + QString::number(nCurrentExperimentBlock) + ", Trial=" + QString::number(nCurrentExperimentTrial) +", Trigger=" + QString::number(nCurrentExperimentBlockTrialReceivedTriggers) + ", Frame=" + QString::number(nBlockTrialFrameCounter) + ")");
	//	//}
	//}
	if(isDebugMode() && (nRefreshRate > 0))
	{
		if (dCurrentTime > (dFramePeriodTime+1.0f))
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,-1,"finalizePaintEvent()::***Paint routine took to long(" + QString::number(dCurrentTime) + " mSecs),(Block=" + QString::number(nCurrentExperimentBlock) + ", Trial=" + QString::number(nCurrentExperimentTrial) +", Trigger=" + QString::number(nCurrentExperimentBlockTrialReceivedTriggers) + ", Frame=" + QString::number(nBlockTrialFrameCounter) + ")");
	}
	if( !checkForNextBlockTrial() ) //Check whether we need to prepare for an new block Trial, otherwise directly call onAnimate()
	{
		if(bForceToStop)
		{
			changeSubObjectState(Experiment_SubObject_Abort);
			return;
		}
		else if (bExperimentShouldStop)
		{
			changeSubObjectState(Experiment_SubObject_Stop);
			//QCoreApplication::postEvent(this,new QEvent(tEventObjectStopped),Qt::HighEventPriority);
			return;
		}
		else
		{
			// if next trial/block reached, the function will prepare for it and calls "onAnimate()" when ready, otherwise we compute next frame
			//QTimer::singleShot(1, this, SLOT(animate()));
			animate();
		}
	}
}