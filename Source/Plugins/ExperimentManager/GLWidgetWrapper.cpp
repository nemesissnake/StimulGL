#include <QtGui>
#include <stdlib.h>
#include <math.h>
#include <QDomDocument>
#include "GLWidgetWrapper.h"

GLWidgetWrapper::GLWidgetWrapper(QWidget *parent) : QGLWidget(parent)
{
	initExperimentObject();
	stimContainerDlg = NULL;
	pExpBlockTrialDomNodeList = NULL;
	bForceToStop = false;
	bExperimentShouldStop = false;
	#ifdef Q_WS_MACX
		nMinScreenUpdateTime = 1; // make param in interface and recommend per platform
	#elseabot
		nMinScreenUpdateTime = 1; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
	#endif
	setAutoFillBackground(false);
	setAutoBufferSwap(false); // in order to have control over time point for buffer swap
	rScreenResolution = QApplication::desktop()->screenGeometry();//dDesktopWidget->screenGeometry();//availableGeometry();
	setFixedSize(rScreenResolution.width(), rScreenResolution.height());
	//setMinimumSize(200, 200);
	setWindowTitle(tr("Painting a Scene"));

	stimContainerDlg = new ContainerDlg();
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	mainLayout = NULL;
	nObjectID = -1;
	pExpConf = NULL;
	tEventObjectStopped = (QEvent::Type)(QEvent::User + 1);	
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
	cleanupExperimentBlockTrialStructure();
	changeSubObjectState(Experiment_SubObject_Stopped);
}

void GLWidgetWrapper::initExperimentObject()
{
	cleanupExperimentBlockTrialStructure();
	nCurrentExperimentTrial = -1;
	nCurrentExperimentBlock = 0;
	nTotalProcessedExperimentTrials = 0;
	nCurrentExperimentReceivedTriggers = 0;
	//nTriggerCount = 0;
	nFrameCounter = 0;
	nElapsedFrameTime = 0;
	nPaintUpdateTime = 0;
	nNextThresholdTriggerCount = 0;
	changeSubObjectState(Experiment_SubObject_Initialized);
}

QRectF GLWidgetWrapper::getScreenResolution()
{
	return rScreenResolution;
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
													if(strParamName == tmpElement.text())//Correct Parameter Name?
													{
														tmpElement = tmpParameterNodeList.item(k).firstChildElement(VALUE_TAG);
														if(!tmpElement.isNull())
														{
															Result = tmpElement.text();
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
	tTotalRunningTime.start();
	//renderWdg->trialTime.start();
	//renderWdg->frameTime.start();	
	checkForNextBlockTrial();
	//startTriggerTimer(2000);
	return true;
}

bool GLWidgetWrapper::isDebugMode() 
{
	if (pExpConf)
	{
		return pExpConf->bDebugMode;
	}
	return false;
}

void GLWidgetWrapper::closeEvent(QCloseEvent *evt)
{
	QGLWidget::closeEvent(evt);
}

bool GLWidgetWrapper::stopExperimentObject()
{
	bExperimentShouldStop = true;
	stopTriggerTimer();
	return true;
}

bool GLWidgetWrapper::abortExperimentObject()
{
	bForceToStop = true;
	stopTriggerTimer();
	return true;
}

void GLWidgetWrapper::startTriggerTimer(int msTime)
{
	tTriggerTimer.setSingleShot(false);
	connect(&tTriggerTimer, SIGNAL(timeout()), this, SLOT(incrementTrigger()));
	tTriggerTimer.start(msTime);
}

void GLWidgetWrapper::stopTriggerTimer()
{
	tTriggerTimer.stop();
	disconnect(&tTriggerTimer, SIGNAL(timeout()), this, SLOT(incrementTrigger()));
}

void GLWidgetWrapper::paintEvent(QPaintEvent *event)
{
	nFrameCounter += 1;	
}

void GLWidgetWrapper::incrementTrigger()
{
	nCurrentExperimentReceivedTriggers++;
}

void GLWidgetWrapper::initBlockTrial()
{
	bForceToStop = false;
	bExperimentShouldStop = false;
	nFrameCounter = 0;
#ifdef Q_WS_MACX
	nMinScreenUpdateTime = 1; // make param in interface and recommend per platform
#else
	nMinScreenUpdateTime = 1; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
#endif
}

//bool GLWidgetWrapper::loadBlockTrial()
//{
//	return true;
//}

bool GLWidgetWrapper::checkForNextBlockTrial()
{
	if ((bForceToStop)||(bExperimentShouldStop))
		return false;
	//int tTotal_elapsed_time = tTotalRunningTime.elapsed();//The Total time past since the experiment started
	//if(m_TriggerMode == 0)
	//{
	//	completedTR = total_elapsed_time / m_TrTime;//The time past in number of Tr's
	//}
	//else
	//{
		//nCompletedTriggers = nTriggerCount;
	//}
	//nCompletedTriggers = nTriggerCount;
	bool goToNextBlockTrial = false;
	bool bInitBeforeFirstExperimentTrial = false;
	if(nCurrentExperimentTrial == -1) //First Experiment Trial? (Start/Init), occurs before the start of the Trigger(timer).
	{
		nTotalProcessedExperimentTrials = 0;
		goToNextBlockTrial = true;
		nCurrentExperimentBlock = 0;
		bInitBeforeFirstExperimentTrial = true;
	}
	else
	{
		goToNextBlockTrial = nCurrentExperimentReceivedTriggers >= nNextThresholdTriggerCount;//Go to next Block Trial?
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		//qDebug() << "GLWidgetWrapper::goToNextBlockTrial(current=" << currentBlockTrial << ", total_elapsed_time=" << total_elapsed_time << ")";
		//int nTotalExperimentTrialAmount = 0;
		//if (getExperimentTriggerCount(nTotalExperimentTrialAmount))
		//{
		//	if(nTotalProcessedExperimentTrials > nTotalExperimentTrialAmount)//No more Experiment Trials left?
		//	{
		//		QCoreApplication::postEvent(this,new QEvent(tEventObjectStopped));//,Qt::HighEventPriority);
		//		//bExperimentShouldStop = true;
		//		return false;
		//	}
		//}

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
			}
		}
		else
		{
			nCurrentExperimentTrial++;//Increment the Experiment Trial Counter
		}
				

		//if (!bFirstBlock)
		//{

		//else
		//{
		//	qDebug() << "GLWidgetWrapper::checkForNextBlockTrial() Could not getExperimentTriggerAmount()";				
		//	bForceToStop = true;
		//	return false;
		//}
		//}
		nNextThresholdTriggerCount += strcExperimentBlockTrials.lBlockTrialStructure[nCurrentExperimentBlock].lTrialStructure[nCurrentExperimentTrial].nNrOfTriggers;//increment the nextTimeThresholdTRs
		//loadBlockTrial();
		//loadBlockTrialFile(BlockTrialFiles[currentBlockTrial], true);
		nFrameCounter = 0;
		initBlockTrial();
		tFrameTime.restart();
		if (bInitBeforeFirstExperimentTrial)
		{
			changeSubObjectState(Experiment_SubObject_Started);
		}

		tStimTimer.singleShot(10, this, SLOT(animate()));
		if(!bInitBeforeFirstExperimentTrial)
			nTotalProcessedExperimentTrials++;
		return true;
	}
	return false;
}

void GLWidgetWrapper::animate()
{
	//if (((bForceToStop)||(bExperimentShouldStop)) == false)
	//{
		if(getSubObjectState() == Experiment_SubObject_Started)
		{
			nElapsedFrameTime = tFrameTime.elapsed();//Calculate the elapsed time since started
			tFrameTime.restart();
			repaint();
			//update();
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
		emit ObjectStateHasChanged(currentSubObjectState);
	}
}

void GLWidgetWrapper::finalizePaintEvent()
{
	while(1)//Don't go too fast...
	{
		nElapsedFrameTime = tFrameTime.elapsed();
		if(nElapsedFrameTime > nMinScreenUpdateTime)
			break;
	}
	swapBuffers();
	if( !checkForNextBlockTrial() ) //Check whether we need to prepare for an new block Trial, otherwise directly call onAnimate()
	{
		nPaintUpdateTime = tFrameTime.elapsed();
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
			QTimer::singleShot(10, this, SLOT(animate()));
		}
	}
}