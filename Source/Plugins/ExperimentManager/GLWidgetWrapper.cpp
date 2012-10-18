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
#include "ExperimentManager.h"

GLWidgetWrapper::GLWidgetWrapper(QWidget *parent) : QGLWidget(parent)
{
	currentSubObjectState = Experiment_SubObject_Constructing;
	init();
	lockedPainter = NULL;
	thisMetaObject = NULL;
	stimContainerDlg = NULL;
	pExpBlockTrialDomNodeList = NULL;
	//bForceToStop = false;
	bExperimentShouldStop = false;
	//bIsPainting = false;
	bCheckForDoubleBuffering = false;
	#ifdef Q_WS_MACX
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
	#elseabot
		nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
	#endif
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
	stimContainerDlg = new ContainerDlg();//MainAppInfo::getMainWindow()); //Can be used like this...
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	stimContainerDlg->setAttribute(Qt::WA_PaintOnScreen);
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
	mainLayout = NULL;
	nObjectID = -1;
	pExpConf = NULL;
	tEventObjectStopped = (QEvent::Type)(QEvent::User + 1);	
	ExpBlockParams = NULL;
	nFrameTimerIndex = -1;
	nTrialTimerIndex = -1;
	bCurrentSubObjectIsLocked = true;
	bCurrentSubObjectReadyToUnlock = false;
	dLastPreSwapTime = 0.0f;
	setVerticalSyncSwap();
}

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
	cleanupExperimentBlockTrialStructure();	
}

void GLWidgetWrapper::setVerticalSyncSwap()
{
	QGLFormat StimulGLQGLFormat;
	StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
	StimulGLQGLFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(StimulGLQGLFormat);
}

bool GLWidgetWrapper::insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue,bool bIsInitializing)
{
	if (ExpBlockParams == NULL)
	{
		ExpBlockParams = new tParsedParameterList();//new QHash<QString, QString>();
		pExpConf->pExperimentManager->setExperimentObjectBlockParameterStructure(nObjectID,ExpBlockParams);
	}
	return (pExpConf->pExperimentManager->insertExperimentObjectBlockParameter(nObjectID,sName,sValue,bIsInitializing));
}

ParsedParameterDefinition GLWidgetWrapper::getExperimentObjectBlockParameter(const int nObjectID,const QString sName, QString sDefValue)
{
	ParsedParameterDefinition PPDResult;
	PPDResult.bHasChanged = false;
	PPDResult.sValue = sDefValue;
	PPDResult.bIsInitialized = true;
	if (pExpConf->pExperimentManager->getExperimentObjectBlockParameter(nObjectID,sName,PPDResult))
	{
		return PPDResult;
	}
	else
	{
		return PPDResult;
	}
}

void GLWidgetWrapper::init()
{
	cleanupExperimentBlockTrialStructure();
	mutExpSnapshot.lock();
	nCurrentExperimentReceivedExternalTriggers = 0;
	nCurrentExperimentProcessedExternalTriggers = 0;
	nCurrentExperimentLastProcExternalTriggers = -1;
	nCurrentExperimentReceivedInternalTriggers = 0;
	expFullStruct.parentStruct.currExpTrial = -1;
	expFullStruct.parentStruct.currExpBlock = 0;
	expFullStruct.parentStruct.currExpInternalTrigger = 0;
	expFullStruct.parentStruct.currExpExternalTrigger = 0;
	expFullStruct.parentStruct.currExpBlockTrialTrigger = 0;
	expFullStruct.parentStruct.currExpBlockTrialFrame = 0;
	expFullStruct.nTotalProcessedExperimentTrials = 0;
	expFullStruct.nNextThresholdTriggerCount = 0;
	mutExpSnapshot.unlock();


	//nCurrentExperimentTrial = -1;
	//nCurrentExperimentBlock = 0;
	//nTotalProcessedExperimentTrials = 0;
	//nCurrentExperimentTrigger = 0;
	//nCurrentExperimentBlockTrialReceivedTriggers = 0;
	//nBlockTrialFrameCounter = 0;
	//nNextThresholdTriggerCount = 0;
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
		bool bRetVal;
		if (thisMetaObject)
		{
			if (!(thisMetaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETOBJECTCONFIGURATION_FULL)) == -1))//Is the slot present?
			{
				//Invoke the slot
				if(!(thisMetaObject->invokeMethod(this, FUNC_SETOBJECTCONFIGURATION, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QObject*,(QObject*)pExpConfStruct))))
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETOBJECTCONFIGURATION << ")!";		
				}		
			}
		}
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

						tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(SUBTRIGGERAMOUNT_TAG);
						int nSubTriggerAmount = 1;//Default
						if(!tmpElement.isNull())//Is there a SubTriggerAmount defined?
						{
							if (!tmpElement.text().isEmpty())
							{
								if (tmpElement.text().toInt() > 0)
								{
									nSubTriggerAmount = tmpElement.text().toInt();
								}
							}
						}

						for (int j=0;j<tmpBlockTrialStruct.nNrOfTrials;j++)
						{
							tmpTrialStruct.nTrialID = j;
							tmpTrialStruct.nTrialNumber = j;
							tmpTrialStruct.nNrOfInternalTriggers = nTriggerAmount;//Default if not defined
							tmpTrialStruct.nNrOfExternalSubTriggers = nSubTriggerAmount;//Default if not defined
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

//bool GLWidgetWrapper::getExperimentBlockTrialStructureCopy(ExperimentBlockTrialStructure &expBlockTrialStructure)
//{
//	expBlockTrialStructure = strcExperimentBlockTrials;
//	return true;
//}

bool GLWidgetWrapper::getCurrentExperimentProgressSnapshot(ExperimentSnapshotStructure *expSnapshotstrc)
{
	if (expSnapshotstrc)
	{
		mutExpSnapshot.lock();
		expSnapshotstrc->currExpBlockTrialFrame = expFullStruct.parentStruct.currExpBlockTrialFrame;
		expSnapshotstrc->currExpInternalTrigger = expFullStruct.parentStruct.currExpInternalTrigger;
		expSnapshotstrc->currExpExternalTrigger = expFullStruct.parentStruct.currExpExternalTrigger;		
		expSnapshotstrc->currExpBlockTrialTrigger = expFullStruct.parentStruct.currExpBlockTrialTrigger;
		expSnapshotstrc->currExpTrial = expFullStruct.parentStruct.currExpTrial;
		expSnapshotstrc->currExpBlock = expFullStruct.parentStruct.currExpBlock;
				
		if(strcExperimentBlockTrials.nNrOfBlocks > expSnapshotstrc->currExpBlock)
		{
			if(expSnapshotstrc->currExpTrial == -1)//When the experiment has not jet been fully initialized
			{
				if (strcExperimentBlockTrials.lBlockTrialStructure.at(expSnapshotstrc->currExpBlock).nNrOfTrials > 0)//let's try/assume here that we need the first ExpTrial!
				{
					expSnapshotstrc->currExpBlockTrialInternalTriggerAmount = strcExperimentBlockTrials.lBlockTrialStructure.at(expSnapshotstrc->currExpBlock).lTrialStructure.at(0).nNrOfInternalTriggers;
				}
			}
			else if (strcExperimentBlockTrials.lBlockTrialStructure.at(expSnapshotstrc->currExpBlock).nNrOfTrials > expSnapshotstrc->currExpTrial) 
			{
				expSnapshotstrc->currExpBlockTrialInternalTriggerAmount = strcExperimentBlockTrials.lBlockTrialStructure.at(expSnapshotstrc->currExpBlock).lTrialStructure.at(expSnapshotstrc->currExpTrial).nNrOfInternalTriggers;
			}
		}
		else
		{
			expSnapshotstrc->currExpBlockTrialInternalTriggerAmount = -1;
		}
		expSnapshotstrc->elapsedTrialTime = expTrialTimer.getElapsedTimeInMilliSec();
		mutExpSnapshot.unlock();
		return true;
	}
	return false;
}

bool GLWidgetWrapper::getCurrentExpBlockTrialInternalTriggerAmount(int &nInternalTriggerAmount)
{
	nInternalTriggerAmount = -1;
	if(strcExperimentBlockTrials.nNrOfBlocks > expFullStruct.parentStruct.currExpBlock)
	{
		if(expFullStruct.parentStruct.currExpTrial == -1)//When the experiment has not jet been fully initialized
		{
			if (strcExperimentBlockTrials.lBlockTrialStructure.at(expFullStruct.parentStruct.currExpBlock).nNrOfTrials > 0)//let's try/assume here that we need the first ExpTrial!
			{
				nInternalTriggerAmount = strcExperimentBlockTrials.lBlockTrialStructure.at(expFullStruct.parentStruct.currExpBlock).lTrialStructure.at(0).nNrOfInternalTriggers;
				return true;
			}
		}
		else if (strcExperimentBlockTrials.lBlockTrialStructure.at(expFullStruct.parentStruct.currExpBlock).nNrOfTrials > expFullStruct.parentStruct.currExpTrial) 
		{
			nInternalTriggerAmount = strcExperimentBlockTrials.lBlockTrialStructure.at(expFullStruct.parentStruct.currExpBlock).lTrialStructure.at(expFullStruct.parentStruct.currExpTrial).nNrOfInternalTriggers;
			return true;
		}
	}
	return false;
}

//int GLWidgetWrapper::getExperimentBlockTrialTriggerAmount(int nBlock, int nTrial)
//{
//	if(strcExperimentBlockTrials.nNrOfBlocks > nBlock)
//	{
//		if (strcExperimentBlockTrials.lBlockTrialStructure.at(nBlock).nNrOfTrials > nTrial)
//		{
//			return strcExperimentBlockTrials.lBlockTrialStructure.at(nBlock).lTrialStructure.at(nTrial).nNrOfTriggers;
//		}
//	}
//	return -1;
//}

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
	strcExperimentBlockTrials.lBlockTrialStructure.clear();
	strcExperimentBlockTrials.nNrOfBlocks = 0;
	return true;
}

bool GLWidgetWrapper::getExperimentBlockParamsFromDomNodeList(int nBlockNumber, int nObjectID, tParsedParameterList *hParams) //QHash<QString, QString> *hParams)
{
	if (hParams == NULL)
		return false;
	if(hParams->count() == 0)
		return false;

	//Set all the parameter bHasChanged attributes too false again
	QList<ParsedParameterDefinition> tmpStrValueList = hParams->values();//The order is guaranteed to be the same as that used by keys()!
	QList<QString> tmpStrKeyList = hParams->keys();//The order is guaranteed to be the same as that used by values()!
	for(int i=0;i<tmpStrKeyList.count();i++)
	{
		tmpStrValueList[i].bHasChanged = false;
		hParams->insert(tmpStrKeyList[i], tmpStrValueList[i]);
	}

	//The below code seems not to work due to the iterator...
	//
	//
	////Set all the parameter bHasChanged attributes too false again
	//tParsedParameterList::const_iterator iterPPL = hParams->constBegin();
	//while (iterPPL != hParams->constEnd()) 
	//{
	//	tmpParDef = iterPPL.value();
	//	tmpString = iterPPL.key();
	//	tmpParDef.bHasChanged = false;
	//	//cout << iterPPL.key() << ": " << iterPPL.value() << endl;
	//	 hParams->insert(tmpString, tmpParDef);
	//	++iterPPL;
	//}

	if(pExpBlockTrialDomNodeList == NULL)
		return false;
	if (pExpBlockTrialDomNodeList->count() == 0)
		return false;

	int nBlockCount = pExpBlockTrialDomNodeList->count();
	if (!(nBlockCount > 0))
		return false;

	QDomElement tmpElement;
	QDomNode tmpNode;
	ParsedParameterDefinition tmpParDef;
	QString tmpString;
	QString tmpValue;

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
													if (hParams->contains(tmpString))//Is the Parameter available in the predefined plugin list?
													{
														tmpElement = tmpParameterNodeList.item(k).firstChildElement(VALUE_TAG);
														if(!tmpElement.isNull())
														{
															tmpValue = tmpElement.text();
															expandExperimentBlockParameterValue(tmpValue);
															tmpParDef.sValue = tmpValue;
															tmpParDef.bHasChanged = true;
															hParams->insert(tmpString,tmpParDef);
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

bool GLWidgetWrapper::expandExperimentBlockParameterValue(QString &sValue)
{
	if (!sValue.isEmpty())
	{		
		if(pExpConf)
		{
			if (pExpConf->pExperimentManager)
			{
				return pExpConf->pExperimentManager->expandExperimentBlockParameterValue(sValue);				
			}
		}
	}
	return false;
}

//bool GLWidgetWrapper::getExperimentBlockParameter(int nBlockNumber, int nObjectID, QString strParamName, ParsedParameterDefinition &pParDef)
//{
//	if(pExpBlockTrialDomNodeList == NULL)
//		return false;
//	if (pExpBlockTrialDomNodeList->count() == 0)
//		return false;
//
//	int nBlockCount = pExpBlockTrialDomNodeList->count();
//	if (!(nBlockCount > 0))
//		return false;
//
//	QDomElement tmpElement;
//	QDomNode tmpNode;
//	QString tmpValue;
//
//	for(int i=0;i<nBlockCount;i++)//Loop through the blocks
//	{
//		if(pExpBlockTrialDomNodeList->at(i).hasChildNodes())
//		{
//			tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(BLOCKNUMBER_TAG);
//			if(!tmpElement.isNull())//Is there a block_number defined?
//			{
//				if (nBlockNumber == tmpElement.text().toInt())//Correct block number?
//				{
//					QDomNodeList tmpObjectNodeList = pExpBlockTrialDomNodeList->at(i).toElement().elementsByTagName(OBJECT_TAG);//Retrieve all the objects
//					int nObjectListCount = tmpObjectNodeList.count();
//					if (nObjectListCount>0)
//					{
//						for (int j=0;j<nObjectListCount;j++)//For each object
//						{
//							if(tmpObjectNodeList.item(j).toElement().hasAttribute(ID_TAG))
//							{
//								if(nObjectID == tmpObjectNodeList.item(j).toElement().attribute(ID_TAG,"").toInt())//Correct ObjectID?
//								{
//									if(tmpObjectNodeList.item(j).firstChildElement(PARAMETERS_TAG).isElement())
//									{
//										tmpElement = tmpObjectNodeList.item(j).firstChildElement(PARAMETERS_TAG);
//										QDomNodeList tmpParameterNodeList = tmpElement.elementsByTagName(PARAMETER_TAG);//Retrieve all the parameters
//										int nParameterListCount = tmpParameterNodeList.count();
//										if (nParameterListCount>0)
//										{
//											for (int k=0;k<nParameterListCount;k++)//For each parameter
//											{
//												tmpElement = tmpParameterNodeList.item(k).firstChildElement(NAME_TAG);
//												if(!tmpElement.isNull())
//												{
//													if(strParamName.toLower() == tmpElement.text().toLower())//Correct Parameter Name?
//													{
//														tmpElement = tmpParameterNodeList.item(k).firstChildElement(VALUE_TAG);
//														if(!tmpElement.isNull())
//														{
//															tmpValue = tmpElement.text().toLower();
//															expandExperimentBlockParameterValue(tmpValue);
//															pParDef.sValue = tmpValue;
//															pParDef.bHasChanged = true;//If the item is present than it has changed
//															return true;
//														}
//														else
//															return false;
//													}
//												}
//												else
//													continue;//Next parameter maybe?
//											}
//										}
//										else
//											return false;
//									}
//									else
//										return false;
//								}
//								else
//									return false;
//							}
//							else
//								return false;
//						}
//					}
//					else
//						return false;//Nothing defined		
//				}
//				else
//					continue;//Search next block
//			}
//			else
//				continue;//Search next block
//		}
//	}
//	return false;
//}

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
			//qDebug() << __FUNCTION__ << "RetinoMap_glwidget::No Double Buffering available!";
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
	pExpConf->pExperimentManager->startExperimentTimer(nFrameTimerIndex);//Starts the Frame timer
	pExpConf->pExperimentManager->startExperimentTimer(nTrialTimerIndex);//Starts the Trial timer
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
		if(isDebugMode())
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Ready to Unlock the Experiment Object");
	}
	return true;
}

bool GLWidgetWrapper::unlockExperimentObject()
{
	if (bCurrentSubObjectIsLocked)
	{
		bCurrentSubObjectIsLocked = false;
		if(isDebugMode())
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Unlocked the Experiment Object");
	}
	return true;
}

bool GLWidgetWrapper::initExperimentObject()
{
	nFrameTimerIndex = pExpConf->pExperimentManager->createExperimentTimer();
	nTrialTimerIndex = pExpConf->pExperimentManager->createExperimentTimer();
	bCurrentSubObjectIsLocked = true;
	bCurrentSubObjectReadyToUnlock = false;
	nRefreshRate = 0;
	dAdditionalRefreshDelayTime = 0.0;
	insertExperimentObjectBlockParameter(nObjectID,GLWWRAP_WIDGET_STIMULI_REFRESHRATE,QString::number(nRefreshRate));
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
	// Get the number of processors in this system
	int iCPU = omp_get_num_procs();
    qDebug() << __FUNCTION__ << "::" << iCPU << "processors available.";//The omp_get_num_threads() call returns 1 in the serial section of the code!!
	// Now set the number of threads
	omp_set_num_threads(iCPU);
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
	thisMetaObject->invokeMethod( this, "finalizePaintEvent",Qt::QueuedConnection);//a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
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
	//if(!mutProcEvents.tryLock())
	//	return;
	//mutProcEvents.lock();
	//waitProcEvents.wait(&mutProcEvents);
	int nPaintFlags;
	if(isDebugMode())
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Starting to paint the widget");
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
		if(isDebugMode()) 
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Starting to paint the initial widget");
		//if(isDebugMode())
		//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","pre Step 1");
		if (lockedPainter == NULL)
		{
			//makeCurrent();
			lockedPainter = new QPainter(this);//Constructor automatically calls begin()
		}
		else
		{
			lockedPainter->begin(this);
		}
		//if(isDebugMode())
		//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 1");
		QFont textFont("arial", 22, QFont::Bold, false);
		QString strText;
		QString strTextStart = "Experiment ready, ";
		if (bCurrentSubObjectReadyToUnlock)
			strText = strTextStart + "waiting for a trigger to start...";
		else
			strText = strTextStart + "press 'Alt' to proceed or CTRL + 'a' to abort the experiment";
		//lockedPainter->begin(this);
		const QRectF windowRect = lockedPainter->window();
		//if(isDebugMode())
		//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 2");
		const int nBorder = 25;
		QPainterPath textPath;
		textPath.addText(0,0,textFont,strText);
		const QRectF textPathRect = textPath.boundingRect();
		lockedPainter->setRenderHint(QPainter::Antialiasing);
		lockedPainter->fillRect(windowRect,QColor(87,87,87));
		//if(isDebugMode())
		//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 3");
		lockedPainter->setPen(Qt::NoPen);
		lockedPainter->setBrush(Qt::white);
		lockedPainter->setWindow ( textPathRect.x() - nBorder , textPathRect.y() - (windowRect.height()/2) , textPathRect.width() + (nBorder*2) , windowRect.height());//translate text rect to rect window button
		lockedPainter->drawPath(textPath);
		//if(isDebugMode())
		//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Painting the initial widget","Step 4");
		//tmpMatrix = lockedPainter->deviceMatrix();
		lockedPainter->end();
		//if(isDebugMode())
		//	pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Finished painting the initial widget");
		
	}
	if(isDebugMode())
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","painter.end()");
	finalizePaintEvent();
}

void GLWidgetWrapper::incrementExternalTrigger()
{
	if (bCurrentSubObjectIsLocked)
	{
		if (bCurrentSubObjectReadyToUnlock == true)
		{
			bCurrentSubObjectReadyToUnlock = false;
			unlockExperimentObject();
		}
	}
	else
	{
		nCurrentExperimentReceivedExternalTriggers++;//Externally Triggered
		emit ExternalTriggerIncremented(nCurrentExperimentReceivedExternalTriggers);
		if(isDebugMode())
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Externally Triggered!, Received="),QString::number(nCurrentExperimentReceivedExternalTriggers));
	}
}

void GLWidgetWrapper::initBlockTrial()
{
	//stimContainerDlg->activateWindow();
	emit NewInitBlockTrial();
	//bForceToStop = false;
	bExperimentShouldStop = false;
//#ifdef Q_WS_MACX
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // make param in interface and recommend per platform
//#else
//	nMinScreenUpdateTime = MIN_SCREEN_UPDATE_TIME; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
//#endif

	bool bDoUnlock = false;
	if (mutExpSnapshot.tryLock())
	{
		bDoUnlock = true;
	}
	//expTrialTimer.restart();
	expFullStruct.parentStruct.currExpBlockTrialFrame = 0;
	int tmpExpBlock = expFullStruct.parentStruct.currExpBlock;
	if(bDoUnlock)
		mutExpSnapshot.unlock();
	getExperimentBlockParamsFromDomNodeList(tmpExpBlock,nObjectID,ExpBlockParams);//Should be moved to the manager?!
	ParsedParameterDefinition pParDef;
	pParDef = getExperimentObjectBlockParameter(nObjectID,GLWWRAP_WIDGET_STIMULI_REFRESHRATE,QString::number(nRefreshRate));
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

bool GLWidgetWrapper::checkForNextBlockTrial()
{
	//stimContainerDlg->activateWindow();
	if (bExperimentShouldStop)//||(bForceToStop)
		return false;	
	int nIntExtDiv;//The ratio between external and internal triggers
	bool goToNextBlockTrial = false;
	bool bFirstCheckAfterExperimentStarted = false;	
	bool bRetval = false;
	bool bExperimentStructureChanged = false;

	mutExpSnapshot.lock();
	if(expFullStruct.parentStruct.currExpTrial == -1)//First Experiment Trial? (Start/Init), occurs before the start of the Trigger(timer)
	{
		expFullStruct.nTotalProcessedExperimentTrials = 0;
		goToNextBlockTrial = true;
		expFullStruct.parentStruct.currExpBlock = 0;//currExpBlockTrialTrigger
		bFirstCheckAfterExperimentStarted = true;		
		bExperimentStructureChanged = true;
	}
	if (expFullStruct.parentStruct.currExpExternalTrigger != nCurrentExperimentReceivedExternalTriggers)
	{
		expFullStruct.parentStruct.currExpExternalTrigger = nCurrentExperimentReceivedExternalTriggers;//Take a snapshot!
		bExperimentStructureChanged = true;
	}	
	if (bFirstCheckAfterExperimentStarted)
	{
		if(strcExperimentBlockTrials.nNrOfBlocks > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
			nIntExtDiv = strcExperimentBlockTrials.lBlockTrialStructure[0].lTrialStructure[0].nNrOfExternalSubTriggers;
	} 
	else
	{
		if(strcExperimentBlockTrials.nNrOfBlocks > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
			nIntExtDiv = strcExperimentBlockTrials.lBlockTrialStructure[expFullStruct.parentStruct.currExpBlock].lTrialStructure[expFullStruct.parentStruct.currExpTrial].nNrOfExternalSubTriggers;
	}
	if ((nCurrentExperimentReceivedExternalTriggers!=0) && ((nCurrentExperimentReceivedExternalTriggers-nCurrentExperimentProcessedExternalTriggers)%nIntExtDiv == 0) && (nCurrentExperimentLastProcExternalTriggers != nCurrentExperimentReceivedExternalTriggers))
	{
		nCurrentExperimentProcessedExternalTriggers = nCurrentExperimentReceivedExternalTriggers;
		nCurrentExperimentLastProcExternalTriggers = nCurrentExperimentReceivedExternalTriggers;
		nCurrentExperimentReceivedInternalTriggers++;
	}
	if (expFullStruct.parentStruct.currExpInternalTrigger != nCurrentExperimentReceivedInternalTriggers)
	{
		expFullStruct.parentStruct.currExpInternalTrigger = nCurrentExperimentReceivedInternalTriggers;//Take a snapshot!
		bExperimentStructureChanged = true;
	}
	if(bFirstCheckAfterExperimentStarted == false)
	{
		if(strcExperimentBlockTrials.nNrOfBlocks > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
			goToNextBlockTrial = expFullStruct.parentStruct.currExpInternalTrigger >= expFullStruct.nNextThresholdTriggerCount;//Go to next Block Trial?
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		bExperimentStructureChanged = true;
		if(strcExperimentBlockTrials.nNrOfBlocks > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
		{
			if((expFullStruct.parentStruct.currExpTrial+1) >= strcExperimentBlockTrials.lBlockTrialStructure[expFullStruct.parentStruct.currExpBlock].nNrOfTrials)//End of this Block(No more Trials)?
			{			
				if ((expFullStruct.parentStruct.currExpBlock+1) >= strcExperimentBlockTrials.nNrOfBlocks)//No more blocks?
				{
						QCoreApplication::postEvent(this,new QEvent(tEventObjectStopped));//,Qt::HighEventPriority);
						bExperimentShouldStop = true;
						pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("No More BlockTrials to process"),QString("Block=") + QString::number(expFullStruct.parentStruct.currExpBlock) + ", Trial=" + QString::number(expFullStruct.parentStruct.currExpTrial) +", Trigger=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialTrigger) + ", Frame=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialFrame) + ")");
						//if (bExperimentStructureChanged)
						//{
						//	if(getSubObjectState() == Experiment_SubObject_Started)
						//		emit ExperimentStructureChanged(expFullStruct.parentStruct.currExpBlock,expFullStruct.parentStruct.currExpTrial,expFullStruct.parentStruct.currExpBlockTrialTrigger);//,expFullStruct.parentStruct.currExpBlockTrialInternalTriggerAmount);
						//}
						mutExpSnapshot.unlock();
						return false;
				} 
				else
				{
					expFullStruct.parentStruct.currExpTrial = 0;	//Reset the Experiment Trial Counter
					expFullStruct.parentStruct.currExpBlock++;		//Increment the Experiment Block Counter
					expFullStruct.parentStruct.currExpBlockTrialTrigger = 0;
				}
			}
			else
			{
				expFullStruct.parentStruct.currExpTrial++;//Increment the Experiment Trial Counter
				expFullStruct.parentStruct.currExpBlockTrialTrigger = 0;
			}
			expFullStruct.nNextThresholdTriggerCount += strcExperimentBlockTrials.lBlockTrialStructure[expFullStruct.parentStruct.currExpBlock].lTrialStructure[expFullStruct.parentStruct.currExpTrial].nNrOfInternalTriggers;//increment the nextTimeThresholdTRs
			expFullStruct.parentStruct.currExpBlockTrialFrame = 0;

			//if(isDebugMode())
			pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"",QString("Starting to Init new BlockTrial"),QString("Block=") + QString::number(expFullStruct.parentStruct.currExpBlock) + ", Trial=" + QString::number(expFullStruct.parentStruct.currExpTrial) +", Trigger=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialTrigger) + ", Frame=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialFrame) + ")");
			initBlockTrial();
		}
		if (bFirstCheckAfterExperimentStarted)
		{
			if (expFullStruct.parentStruct.currExpTrial == -1)
				expFullStruct.parentStruct.currExpTrial = 0;
			changeSubObjectState(Experiment_SubObject_Started);
		}
		if (bCurrentSubObjectIsLocked == false)
		{
			if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","New BlockTrial Prepared, going to qeueu animate()");
			QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
			if(!bFirstCheckAfterExperimentStarted)
				expFullStruct.nTotalProcessedExperimentTrials++;
		}
		else
		{
			//QMetaObject::invokeMethod( this, "aniiiimmmmaaatttte",Qt::QueuedConnection);// a QEvent will be sent and the member is invoked as soon as the application enters the main event loop.
			//if(strcExperimentBlockTrials.nNrOfBlocks > 0)//Are there blocks defined? qmlWidgetViewer trough UI (without ExperimentManager) doesn't have any defined blocks here!
			QMetaObject::invokeMethod( this, "animate",Qt::QueuedConnection,Q_ARG(bool, false));
		}
		expTrialTimer.restart();
		bRetval = true;
	}
	else
	{
		int nRelBlockTrigger = getRelativeBlockTrialTrigger(expFullStruct.parentStruct.currExpInternalTrigger);
		if (expFullStruct.parentStruct.currExpBlockTrialTrigger != nRelBlockTrigger)
		{
			expFullStruct.parentStruct.currExpBlockTrialTrigger = nRelBlockTrigger;
			bExperimentStructureChanged = true;
		}		
		bRetval = false;
	}
	if ((bCurrentSubObjectIsLocked == false) && (bFirstTriggerAfterUnlock))//has the experiment just been unlocked for the first time?
	{
		bFirstTriggerAfterUnlock = false;
		emit ExperimentStructureChanged(expFullStruct.parentStruct.currExpBlock,expFullStruct.parentStruct.currExpTrial,expFullStruct.parentStruct.currExpBlockTrialTrigger);
	}
	if ((bExperimentStructureChanged) && (bFirstCheckAfterExperimentStarted == false))
	{
		if(getSubObjectState() == Experiment_SubObject_Started)
			emit ExperimentStructureChanged(expFullStruct.parentStruct.currExpBlock,expFullStruct.parentStruct.currExpTrial,expFullStruct.parentStruct.currExpBlockTrialTrigger);//,expFullStruct.parentStruct.currExpBlockTrialInternalTriggerAmount);
	}
	mutExpSnapshot.unlock();
	return bRetval;
}

int GLWidgetWrapper::getRelativeBlockTrialTrigger(int nAbsoluteTrigger)
{
	bool bDoUnlock = false;
	//Important, before using this function the Mutex mutExpSnapshot should be locked!
	if(mutExpSnapshot.tryLock())
	{
		bDoUnlock = true;//Make sure to unlock it at the end of the function again
	}
	int nTriggersOutsideCurrentBlockTrial = 0;
	for (int i=0;i<strcExperimentBlockTrials.nNrOfBlocks;i++)
	{
		for (int j=0;j<strcExperimentBlockTrials.lBlockTrialStructure.at(i).nNrOfTrials;j++)
		{
			if ((nTriggersOutsideCurrentBlockTrial + (strcExperimentBlockTrials.lBlockTrialStructure.at(i).lTrialStructure.at(j).nNrOfInternalTriggers))>nAbsoluteTrigger)
			{
				if (bDoUnlock)
					mutExpSnapshot.unlock();
				return (nAbsoluteTrigger - nTriggersOutsideCurrentBlockTrial);
			}
			else
			{
				nTriggersOutsideCurrentBlockTrial = nTriggersOutsideCurrentBlockTrial + strcExperimentBlockTrials.lBlockTrialStructure.at(i).lTrialStructure.at(j).nNrOfInternalTriggers;
			}
		}
	}
	if (bDoUnlock)
		mutExpSnapshot.unlock();
	return -1;
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

			if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Going to call update()");
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
			if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","update() called");
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

	if (nRefreshRate > 0)
	{
		dFramePeriodTime = 1000.0f/nRefreshRate; //DisplayRefreshRate
		dAdditionalRefreshDelayTime = dFramePeriodTime/10.0f;
		dWaitTime = 0.0f;
		//dCurrentTime = pExpConf->pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);	
		//while((dCurrentTime)<dFramePeriodTime)//Don't go too fast...//((dCurrentTime+dAdditionalRefreshDelayTime)<dFramePeriodTime)//Don't go too fast...
		//{
		//	dWaitTime = dFramePeriodTime - dCurrentTime;// + dAdditionalRefreshDelayTime;
		//	if(isDebugMode())// && (bObjectIsLocked==false))
		//		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Too fast --> Sleeping(" + QString::number(dWaitTime) + ")...") + QString::number(bObjectIsLocked);			
		//	ExperimentTimer::SleepMSecAccurate(dWaitTime);
		//	//Sleep(nSTime);
		//	dCurrentTime = pExpConf->pExperimentManager->elapsedExperimentTimerTime(nFrameTimerIndex);
		//}
		if (bObjectIsLocked == false)
		{
			double currentPreSwapTime = pExpConf->pExperimentManager->elapsedExperimentTimerTime(0);
			mutExpSnapshot.lock();
			if ((expFullStruct.parentStruct.currExpBlockTrialFrame == 0) && (expFullStruct.parentStruct.currExpBlock == 0))
			{
				dLastPreSwapTime = currentPreSwapTime;//Just set this initial value and proceed
			}
			else
			{
				dWaitTime = (dLastPreSwapTime + dFramePeriodTime) - currentPreSwapTime;
				if(dWaitTime > dAdditionalRefreshDelayTime)//Do we need to wait?
				{
					if(isDebugMode())
						pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Too fast --> Sleeping(" + QString::number(dWaitTime-dAdditionalRefreshDelayTime) + ")... Locked="),QString::number(bObjectIsLocked);
					ExperimentTimer::SleepMSecAccurate(dWaitTime-dAdditionalRefreshDelayTime);
				}
				else if(dWaitTime < 0.0f)//This means that a frame is going to be skipped!
				{					
					while(dWaitTime < 0.0f)//Search the first next available frame threshold time
					{
						pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,-1,__FUNCTION__,"","Frame skipped!, missed=", QString::number(dWaitTime));
						dLastPreSwapTime = dLastPreSwapTime + dFramePeriodTime;
						dWaitTime = (dLastPreSwapTime + dFramePeriodTime) - currentPreSwapTime;
					}
					//if (dAdditionalRefreshDelayTime < (dFramePeriodTime/2))
					//	dAdditionalRefreshDelayTime = dAdditionalRefreshDelayTime + 1.0f;
					if(isDebugMode())
						pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Sleeping for next frame(" + QString::number(dWaitTime-dAdditionalRefreshDelayTime) + ")...") + QString::number(bObjectIsLocked);
					ExperimentTimer::SleepMSecAccurate(dWaitTime-dAdditionalRefreshDelayTime);
				}
				dLastPreSwapTime = dLastPreSwapTime + dFramePeriodTime;
			}
			mutExpSnapshot.unlock();
		}
		dCurrentTime = pExpConf->pExperimentManager->restartExperimentTimer(nFrameTimerIndex);
	}

	if(isDebugMode())// && (bObjectIsLocked==false))
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","Going to Swap, locked=", QString::number(bObjectIsLocked));
	swapBuffers();
	if(isDebugMode())// && (bObjectIsLocked==false))
		pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,0,__FUNCTION__,"","BlockTrial Buffer Swapped, locked=",QString::number(bObjectIsLocked));

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

	if(mutExpSnapshot.tryLock())
	{
		if (bExperimentShouldStop)
		{
			changeSubObjectState(Experiment_SubObject_Stop);
			mutExpSnapshot.unlock(); 
			return;
		}
		if (bObjectIsLocked==false)
		{
			expFullStruct.parentStruct.currExpBlockTrialFrame++;
		}
		if((nRefreshRate > 0) && (bObjectIsLocked==false)) //&& isDebugMode())
		{
			if (dCurrentTime > (dFramePeriodTime*1.5f))
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,-1,__FUNCTION__,"","Paint routine took too long(" + QString::number(dCurrentTime) + " mSecs),(Block=" + QString::number(expFullStruct.parentStruct.currExpBlock) + ", Trial=" + QString::number(expFullStruct.parentStruct.currExpTrial) +", Trigger=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialTrigger) + ", Frame=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialFrame) + ")",QString::number(dCurrentTime));
			else if(isDebugMode())
				pExpConf->pExperimentManager->logExperimentObjectData(nObjectID,-1,__FUNCTION__,"","Paint routine took(" + QString::number(dCurrentTime) + " mSecs),(Block=" + QString::number(expFullStruct.parentStruct.currExpBlock) + ", Trial=" + QString::number(expFullStruct.parentStruct.currExpTrial) +", Trigger=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialTrigger) + ", Frame=" + QString::number(expFullStruct.parentStruct.currExpBlockTrialFrame) + ")",QString::number(dCurrentTime));
		}
		mutExpSnapshot.unlock(); 
		//QMetaObject::invokeMethod( this, "proceedPaintEventLoop",Qt::QueuedConnection);
		//tStimTimer.singleShot(1, this, SLOT(proceedPaintEventLoop()));
		if( !checkForNextBlockTrial() ) //Check whether we need to prepare for an new block Trial
		{
			//if(bForceToStop)
			//{
			//	changeSubObjectState(Experiment_SubObject_Abort);
			//	//QCoreApplication::postEvent(this,new QEvent(tEventObjectStopped),Qt::HighEventPriority);
			//	return;
			//}
			//else 
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
	}
}
