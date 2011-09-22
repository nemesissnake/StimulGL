#include <QtGui>
#include <stdlib.h>
#include <math.h>
#include <QDomDocument>
#include "GLWidgetWrapper.h"

GLWidgetWrapper::GLWidgetWrapper(QWidget *parent) : QGLWidget(parent)
{
	init();
	stimContainerDlg = NULL;
	pExpBlockTrialDomNodeList = NULL;
	bForceToStop = false;
	bDebugMode = false;
	#ifdef Q_WS_MACX
		nMinScreenUpdateTime = 1; // make param in interface and recommend per platform
	#else
		nMinScreenUpdateTime = 1; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
	#endif
	setAutoFillBackground(false);
	setAutoBufferSwap(false); // in order to have control over time point for buffer swap
	//dDesktopWidget = QApplication::desktop();
	rScreenResolution = QApplication::desktop()->screenGeometry();//dDesktopWidget->screenGeometry();//availableGeometry();
	setFixedSize(rScreenResolution.width(), rScreenResolution.height());
	//setMinimumSize(200, 200);
	setWindowTitle(tr("Painting a Scene"));

	stimContainerDlg = new ContainerDlg();//parent parameter?
	stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	//retinoMapWdg = new RetinoMap_glwidget(stimContainerDlg);
	//retinoMapWdg->setObjectName("RetinoMap_RenderWidgetGL");
	stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	mainLayout = NULL;
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
	emit WidgetStateHasChanged(ExperimentObject_Aborted);
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
					//abortExperiment();
				}
				break;
			}

			//if(keyEvent->text() == ui.USBCharEdit->text()) // for fMRI triggering via USB port (simulated keyboard)
			//{
			//	updateTriggerCount();
			//	return true;
			//}
		}
	}
	//return QDialog::eventFilter(target, event);
	return true;
}

bool GLWidgetWrapper::setBlockTrialDomNodeList(QDomNodeList *pDomNodeList)
{
	if(pDomNodeList)
	{
		pExpBlockTrialDomNodeList = pDomNodeList;
		return true;
	}
	return false;
}

bool GLWidgetWrapper::getBlockTrialParameter(int nBlockNumber, int nObjectID, QString strParamName, QString &Result)
{
	if(pExpBlockTrialDomNodeList == NULL)
		return false;
	if (pExpBlockTrialDomNodeList->count() == 0)
		return false;

	int nBlockTrialCount = pExpBlockTrialDomNodeList->count();
	if (!(nBlockTrialCount > 0))
		return false;

	QDomElement tmpElement;
	QDomNode tmpNode;

	for(int i=0;i<nBlockTrialCount;i++)//Loop through the blocks
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
									QDomNodeList tmpParameterNodeList = tmpObjectNodeList.item(j).toElement().elementsByTagName(PARAMETER_TAG);//Retrieve all the parameters
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
					}
					else
						return false;//Nothing defined


					//tmpNode = d.firstChild();
					//while (!n.isNull()) {
					//	if (n.isElement()) {
					//		QDomElement e = n.toElement();
					//		cout << "Element name: " << e.tagName() << endl;
					//		break;
					//	}
					//	n = n.nextSibling();
					//}				
					//tmpElement = pExpBlockTrialDomNodeList->at(i).firstChildElement(OBJECT_TAG);
					//if(!tmpElement.isNull())
					//{
					//	//aa = tmpElement.tagName();//object
					//	//aa = tmpElement.nodeName();//object
					//	QDomNamedNodeMap NamedNodeMap = tmpElement.attributes();//pExpBlockTrialDomNodeList->at(i).toElement().attributes();
					//	int nCount = NamedNodeMap.count();//
					//	//for (int j=0;j<nCount;j++)
					//	//{
					//	//	aa = NamedNodeMap.item(i).nodeValue();
					//	//}
					//	if (NamedNodeMap.contains(ID_TAG))//
					//	{
					//		//aa = NamedNodeMap.namedItem(ID_TAG).nodeValue();
					//	}
					//	//aa=aa;					
				}
				else
					continue;//Search next block
			}
			else
				continue;//Search next block
		}
	}
	return false;


	//	QDomNode DomNode = pExpBlockTrialDomNodeList->at(i);
	//	while (!DomNode.isNull()) 
	//	{
	//		if (DomNode.isElement()) 
	//		{
	//			QDomElement e = DomNode.toElement();
	//			if (e.tagName() == a)
	//			{
	//				QString aa("ID");
	//				hh = e.firstChildElement(aa).text();
	//				QDomNode ee = e.namedItem(aa);
	//				if(!ee.isNull())
	//				{
	//					QDomElement de = ee.toElement();
	//					hh = ee.nodeName() + ":" + de.tagName() + ":" + de.attribute("ID","defvalue");
	//					//qDebug()qPrintable
	//				}
	//			}
	//			//qDebug() << "Element name: " << e.tagName() << endl;
	//			break;
	//		}
	//		DomNode = DomNode.nextSibling();
	//	}
	////	//bool c = list.at(i).hasChildNodes();
	//}


	//			int metaID = QMetaType::type(sClass.toLatin1());
	//			if (metaID > 0)//(id != -1) 
	//			{
	//				objectElement tmpElement;
	//				tmpElement.nObjectID = nID;
	//				tmpElement.nMetaID = metaID;
	//				tmpElement.sObjectName = sName;
	//				tmpElement.pObject = static_cast< QObject* > ( QMetaType::construct(metaID) );

	//				//tmpElement.pObject
	//				const QMetaObject* metaObject = tmpElement.pObject->metaObject();
	//				////Query the properties(only Q_PROPERTY)
	//				//QStringList properties;
	//				//QString nolist;
	//				//for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
	//				//{
	//				//	properties << QString::fromLatin1(metaObject->property(i).name());
	//				//	nolist = QString::fromLatin1(metaObject->property(i).name());
	//				//}
	//				////Query the methods(only public slots!)
	//				//QStringList methods;
	//				//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
	//				//	methods << QString::fromLatin1(metaObject->method(i).signature());

	//				QStringList properties;
	//				for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
	//					properties << QString::fromLatin1(metaObject->method(i).signature());
	//				if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETDOMNODELIST_FULL)) == -1))//Is the slot present?//
	//				{
	//					//Invoke the slot
	//					bool bRetVal = true;
	//					if(!(metaObject->invokeMethod(tmpElement.pObject, "setDomNodeList", Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QDomNodeList*, &ExperimentObjectDomNodeList))))
	//					{
	//						qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << "setDomNodeList()" << ")!";		
	//						return false;
	//					}		
	//				}
	//				tmpElement.nState = ExperimentObject_Initialized;//This is still an inactive state!
	//				lExperimentObjectList.append(tmpElement);					
	//			}
	//		}
	//	}
	//	return true;
}

void GLWidgetWrapper::SetupLayout(QWidget* layoutWidget)
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

void GLWidgetWrapper::setDebugMode(bool bMode)
{
	bDebugMode = bMode;
}

void GLWidgetWrapper::init()
{
	currentBlock = 0;
	currentBlockTrial = -1;
	completedTR = 0;
	m_TriggerCount = 0;
	nFrameCounter = 0;
	nElapsedFrameTime = 0;
	nPaintUpdateTime = 0;
	nextTimeThresholdTRs = 0;
}

bool GLWidgetWrapper::start()
{
	init();
	totalRunningTime.start();
	//renderWdg->trialTime.start();
	//renderWdg->frameTime.start();	
	checkForNextBlockTrial();
	startTriggerTimer(2000);
	return true;
}

void GLWidgetWrapper::closeEvent(QCloseEvent *evt)
{
	QGLWidget::closeEvent(evt);
}

bool GLWidgetWrapper::stop()
{
	stopTriggerTimer();
	bForceToStop = true;
	return true;
}

void GLWidgetWrapper::startTriggerTimer(int msTime)
{
	tTriggerTimer.setSingleShot(false);
	connect(&tTriggerTimer, SIGNAL(timeout()), this, SLOT(incrementTriggerCount()));
	tTriggerTimer.start(msTime);
}

void GLWidgetWrapper::stopTriggerTimer()
{
	tTriggerTimer.stop();
	disconnect(&tTriggerTimer, SIGNAL(timeout()), this, SLOT(incrementTriggerCount()));
}

void GLWidgetWrapper::paintEvent(QPaintEvent *event)
{
	nFrameCounter += 1;	
}

void GLWidgetWrapper::incrementTriggerCount()
{
	m_TriggerCount++;
}

void GLWidgetWrapper::initBlockTrial()
{
	bForceToStop = false;
	nFrameCounter = 0;
#ifdef Q_WS_MACX
	nMinScreenUpdateTime = 1; // make param in interface and recommend per platform
#else
	nMinScreenUpdateTime = 1; // on Win (with recent OGL drivers), "swapBuffers" will wait for n retraces as indicated by "setSwapInterval" command (works like DX "flip")
#endif
}

bool GLWidgetWrapper::loadBlockTrial()
{
	return true;
}

bool GLWidgetWrapper::checkForNextBlockTrial()
{
	if (bForceToStop)
		return false;
	int total_elapsed_time = totalRunningTime.elapsed();//The Total time past since the experiment started
	//if(m_TriggerMode == 0)
	//{
	//	completedTR = total_elapsed_time / m_TrTime;//The time past in number of Tr's
	//}
	//else
	//{
		completedTR = m_TriggerCount;
	//}
	bool goToNextBlockTrial = false;
	bool bFirstBlock = false;
	if(currentBlockTrial == -1) //First Block Trial? (Start/Init)
	{
		goToNextBlockTrial = true;
		bFirstBlock = true;
	}
	else
	{
		goToNextBlockTrial = completedTR > nextTimeThresholdTRs;//Go to next Block Trial when the total completed Tr's is larger than the next Threshold Tr.
	}
	if(goToNextBlockTrial)//When we init/start or switch from a Block Trial 
	{
		//qDebug() << "GLWidgetWrapper::goToNextBlockTrial(current=" << currentBlockTrial << ", total_elapsed_time=" << total_elapsed_time << ")";
		currentBlockTrial++;//Increment the Block Trial Counter
		if(currentBlockTrial >= 100)//---->>//BlockTrialFiles.size())//No more Block Trials left?
		{
			return false;
		}
		nextTimeThresholdTRs += 2;//----->>> BlockTrialsDurationInTRs[currentBlockTrial];//increment the nextTimeThresholdTRs
		loadBlockTrial();
		//loadBlockTrialFile(BlockTrialFiles[currentBlockTrial], true);
		nFrameCounter = 0;
		initBlockTrial();
		tFrameTime.restart();
		if (bFirstBlock)
		{
			emit WidgetStateHasChanged(ExperimentObject_Started);
		}

		tStimTimer.singleShot(10, this, SLOT(animate()));
		return true;
	}
	return false;
}

void GLWidgetWrapper::animate()
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
			//this->setUpdatesEnabled(false);
			emit WidgetStateHasChanged(ExperimentObject_Abort);
			return;
		}
		// if next trial/block reached, the function will prepare for it and calls "onAnimate()" when ready, otherwise we compute next frame
		QTimer::singleShot(10, this, SLOT(animate()));
	}
}