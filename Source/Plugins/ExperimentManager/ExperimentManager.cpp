#include "ExperimentManager.h"
#include <QFileDialog>
#include <QWaitCondition>
//#include "..\ParallelPortDevice\parallelport.h"
//#include "glwidget.h"

QScriptValue ExperimentManager::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script
	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();
	return engine->newQObject(new ExperimentManager(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

ExperimentManager::ExperimentManager(QObject *parent) : QObject(parent)
{
	//qDebug("ExperimentManager Main Constructor.");
	//qWarning("");
	//qCritical("");
	//qFatal("");
	m_RunFullScreen = true;
	m_ExpFileName = "";
	m_DebugMode = false;
	currentExperimentTree = NULL;
	RegisterMetaTypes();
	changeCurrentExperimentState(Experiment_Constructed);
	changeCurrentExperimentState(Experiment_Initialized);
}

ExperimentManager::~ExperimentManager()
{
	cleanupExperiment();
}

void ExperimentManager::RegisterMetaTypes()
{
	qRegisterMetaType<RetinoMap_glwidget>(RETINOMAP_WIDGET_NAME);
}

QString ExperimentManager::getCurrentDateTimeStamp()
{
	return (QDateTime::currentDateTime().toString(DATETIMESTAMPFORMAT));
}

QString ExperimentManager::getExperimentFileName()
{
	if(QFile::exists(m_ExpFileName))
	{
		return m_ExpFileName;
	}
	else
	{
		m_ExpFolder = "";
		return m_ExpFileName;
	}
}

bool ExperimentManager::setExperimentFileName(const QString qstrExpFileName)
{
	m_ExpFileName = qstrExpFileName;
	QFileInfo fInfo(m_ExpFileName);
	m_ExpFolder = fInfo.absolutePath();
	return true;
}

bool ExperimentManager::saveExperiment(QString strFile)
{
	if (!currentExperimentTree)
		return false;
	
	QString fileName;
	if (strFile.isEmpty())
	{
		fileName = QFileDialog::getSaveFileName(NULL, tr("Save Experiment File"), QDir::currentPath(), tr("Experiment Files (*.exml *.xml);;Any file (*)"));
	}
	else
	{
		fileName = strFile;
	}

	if (fileName.isEmpty())
		return false;
	//fileName = "D:\\save.exml";//"C:\\Users\\John\\Desktop\\experiment_example1_save.exml";

	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) 
	{
		//QMessageBox::warning(this, tr("SAX Bookmarks"),	tr("Cannot write file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}

	if (currentExperimentTree->write(&file))
	{
		return true;
	}
	return false;
}

bool ExperimentManager::openExperiment(QString strFile, bool bViewEditTree)
{
	if (currentExperimentTree)
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	currentExperimentTree = new ExperimentTree;

	QString fileName;
	if (strFile.isEmpty())
	{
		fileName = QFileDialog::getOpenFileName(NULL, tr("Open Experiment File"), QDir::currentPath(), tr("Experiment Files (*.exml *.xml);;Any file (*)"));
	}
	else
	{
		fileName = strFile;
	}
	
	if (fileName.isEmpty())
		return false;
	//fileName = "C:/Users/John/Desktop/experiment_example1.exml";

	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		//QMessageBox::warning(this, tr("Open Experiment"),
		//	tr("Cannot read file %1:\n%2.")
		//	.arg(fileName)
		//	.arg(file.errorString()));
		emit WriteToLogOutput("Could not open experiment file!");
		return false;
	}

	if (currentExperimentTree->read(&file))
	{
		if (bViewEditTree)
			currentExperimentTree->showMaximized();
		return true;
	}
	else
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	return false;
}

void ExperimentManager::changeCurrentExperimentState(ExperimentState expCurrState)
{
	if (expCurrState != getCurrentExperimentState())
	{
		experimentCurrentState = expCurrState;
		emit ExperimentStateHasChanged(expCurrState,getCurrentDateTimeStamp());
	}
}

bool ExperimentManager::runExperiment()
{
	if (!currentExperimentTree)
	{
		qDebug() << "runExperiment::No Experiment loaded!";
		return false;
	}
	if(getCurrentExperimentState() != Experiment_Initialized)
	{
		qDebug() << "runExperiment::Wrong state, could not start experiment!";
		return false;
	}
	changeCurrentExperimentState(Experiment_IsStarting);
	//m_ForceExperimentToStop = false;
#ifdef Q_OS_WIN
	bool ret = SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#else
	QThread::currentThread()->setPriority(QThread::HighPriority);  
	// QThread::TimeCriticalPriority);
#endif
	if (!configureExperiment())
	{
		changeCurrentExperimentState(Experiment_Initialized);
		return false;
	}	
	if (!createExperimentObjects())
	{
		changeCurrentExperimentState(Experiment_Initialized);
		return false;
	}

	if(!connectExperimentObjects())
	{
		changeCurrentExperimentState(Experiment_Initialized);
		return false;
	}

	//ContainerDlg *stimContainerDlg;
	//stimContainerDlg = new ContainerDlg();//parent parameter?
	//stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	//stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	//QVBoxLayout *mainLayout = new QVBoxLayout;
	//mainLayout->setAlignment(Qt::AlignCenter);
	//mainLayout->setMargin(0);
	//mainLayout->addWidget(widget);
	//stimContainerDlg->setLayout(mainLayout);
	////stimContainerDlg->show();
	//stimContainerDlg->showFullScreen();

	//ContainerDlg *stimContainerDlg;
	//stimContainerDlg = new ContainerDlg();//parent parameter?
	//stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	//ThreadedGLWidgetWrapper *widget = new ThreadedGLWidgetWrapper(stimContainerDlg);
	//QVBoxLayout *mainLayout = new QVBoxLayout;
	//mainLayout->setAlignment(Qt::AlignCenter);
	//mainLayout->setMargin(0);
	//mainLayout->addWidget(widget);
	//stimContainerDlg->setLayout(mainLayout);
	////stimContainerDlg->show();
	//stimContainerDlg->showFullScreen();
	//widget->startRendering();
	//SleeperThread::msleep(10000);
	//if (widget) {
	//	widget->stopRendering();
	//	delete widget;
	//}
	//stimContainerDlg->close();
	//delete stimContainerDlg;
	//return true;

	//retinoMapWdg->setFixedSize(1000,1000);
	//retinoMapWdg->setDebugMode(true);
	//retinoMapWdg->startAnimationTimer(50);
	//if(m_RunFullScreen)
	//stimContainerDlg->showFullScreen();
	//else		
	//QDesktopWidget *dt = QApplication::desktop();
	//QCursor::setPos(dt->width(), dt->height()/2); // not at bottom because then mouse movement on Mac would show dock		
	//return retinoMapWdg->start();
	//}
	//void AppWindow::closeEvent(QCloseEvent *evt)
	//{
	//	QWidgetList windows = ws->windowList();
	//	for (int i = 0; i < int(windows.count()); ++i) {
	//		GLWidget *window = (GLWidget *)windows.at(i);
	//		window->stopRendering();
	//	}
	//	QMainWindow::closeEvent(evt);
	//}
	
	if(!startExperimentObjects(m_RunFullScreen))
	{
		changeCurrentExperimentState(Experiment_Initialized);
		return false;
	}
	changeCurrentExperimentState(Experiment_Started);
	return true;
}

void ExperimentManager::abortExperiment()
{
	if(getCurrentExperimentState()==Experiment_Started)
	{
		changeCurrentExperimentState(Experiment_IsStopping);
		if(!abortExperimentObjects())
			qDebug() << "ExperimentManager::abortExperiment Could not abort the Experiment Objects";
		QThread::currentThread()->setPriority(QThread::NormalPriority);
	}
}

void ExperimentManager::stopExperiment()
{
	if(getCurrentExperimentState()==Experiment_Started)
	{
		changeCurrentExperimentState(Experiment_IsStopping);
		if(!stopExperimentObjects())
			qDebug() << "ExperimentManager::stopExperiment Could not stop the Experiment Objects";
		QThread::currentThread()->setPriority(QThread::NormalPriority);
	}
}

bool ExperimentManager::cleanupExperiment()
{
	cleanupExperimentObjects();
	if (currentExperimentTree)
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	return true;
}

void ExperimentManager::cleanupExperimentObjects()
{
	int nCount = lExperimentObjectList.count();
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			if ((lExperimentObjectList[i].pObject))
			{
				//if (!(lExperimentObjectList[i].pObject->metaObject()->indexOfSignal(QMetaObject::normalizedSignature(SIGNAL_LOGTOMANAGER)) == -1))//Is the signal present?
				//{
				//	//Disconnect the signal
				//	disconnect(lExperimentObjectList[i].pObject, SIGNAL(LogToExperimentManager(QString)), this, SIGNAL(WriteToLogOutput(QString)));//Qt::QueuedConnection --> makes it asynchronyous
				//}
				QMetaType::destroy(lExperimentObjectList[i].nMetaID, lExperimentObjectList[i].pObject);
				lExperimentObjectList[i].pObject = NULL;
			}
			lExperimentObjectList[i].nMetaID = -1;
			lExperimentObjectList[i].nObjectID = -1;
			lExperimentObjectList[i].sObjectName = "";
		}
		lExperimentObjectList.clear();
	}
}

bool ExperimentManager::changeExperimentObjectsSignalSlots(bool bDisconnect, int nSpecificIndex)
{
	if (!currentExperimentTree)
	{
		qDebug() << "connectExperimentObjectsSignalSlots::No Experiment loaded!";
		return false;
	}
	int nCount = lExperimentObjectList.count();
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			if ((nSpecificIndex != -1) && (nSpecificIndex < nCount))
				i = nSpecificIndex;//set i to the index
			
			const QMetaObject* metaObject = lExperimentObjectList.at(i).pObject->metaObject();

			//QString sSlotFullName = "ObjectStateHasChanged";//FUNC_WIDGETSTATECHANGED_FULL;//FUNC_USERCLOSE_FULL;
			if (metaObject->indexOfSignal(metaObject->normalizedSignature(SIGNAL_USERCLOSE_FULL)) != -1)//Is the signal present?
			{
				if (bDisconnect)
				{
					disconnect(lExperimentObjectList[i].pObject, SIGNAL(UserWantsToClose(void)), this, SLOT(abortExperiment(void)));
				}
				else
				{//For now the complete experiment including all object is aborted whenever the UserWantsToClose Signal gets activated
					connect(lExperimentObjectList[i].pObject, SIGNAL(UserWantsToClose(void)), this, SLOT(abortExperiment(void)));
				}
			}

			if (metaObject->indexOfSignal(metaObject->normalizedSignature(SIGNAL_OBJECTSTOP_FULL)) != -1)//Is the signal present?
			{
				if (bDisconnect)
				{
					disconnect(lExperimentObjectList[i].pObject, SIGNAL(ObjectShouldStop(void)), this, SLOT(stopExperiment(void)));
				}
				else
				{//For now the complete experiment including all object is aborted whenever the ObjectShouldStop Signal gets activated
					connect(lExperimentObjectList[i].pObject, SIGNAL(ObjectShouldStop(void)), this, SLOT(stopExperiment(void)));
				}
			}

			if (metaObject->indexOfSignal(metaObject->normalizedSignature(FUNC_OBJECTSTATECHANGED_FULL)) != -1)//Is the signal present?
			{
				if (bDisconnect)
				{
					disconnect(lExperimentObjectList[i].pObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
				} 
				else
				{
					connect(lExperimentObjectList[i].pObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
				}
					
			}


			if ((nSpecificIndex != -1) && (nSpecificIndex < nCount))
				break;//Only one adjustment needed!
		}
	}
	return true;
}

void ExperimentManager::changeExperimentSubObjectState(ExperimentSubObjectState nState)
{
	if (!currentExperimentTree)
	{
		qDebug() << "invokeExperimentObjectSlot::No Experiment loaded!";
	}
	int nCount = lExperimentObjectList.count();
	int nActiveExperimentObjects = 0;
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			if (QObject::sender() == lExperimentObjectList.at(i).pObject)
			{
				//We automatically close and delete the object after a "Abort" command...
				if(nState == Experiment_SubObject_Abort)
				{
					if (lExperimentObjectList[i].nState != Experiment_SubObject_IsAborting)//Multiple Abort events could occur, catch only first one
					{
						connectExperimentObjects(true,lExperimentObjectList[i].nObjectID);//disconnect!
						lExperimentObjectList[i].pObject->deleteLater();
						lExperimentObjectList[i].nState = Experiment_SubObject_IsAborting;
					}
				}
				else if(nState == Experiment_SubObject_Stop)
				{
					if (lExperimentObjectList[i].nState != Experiment_SubObject_IsStopping)//Multiple Stop events could occur, catch only first one
					{
						connectExperimentObjects(true,lExperimentObjectList[i].nObjectID);//disconnect!
						lExperimentObjectList[i].pObject->deleteLater();
						lExperimentObjectList[i].nState = Experiment_SubObject_IsStopping;
					}
				}
				else if(nState == Experiment_SubObject_Stopped)
				{
					changeExperimentObjectsSignalSlots(true,i);
					lExperimentObjectList[i].pObject = NULL;
					lExperimentObjectList[i].nState = nState;
				}
				else//All other cases
				{
					lExperimentObjectList[i].nState = nState;//Set the new object state
				}
			}
			if (!((lExperimentObjectList[i].nState == Experiment_SubObject_Initialized)||(lExperimentObjectList[i].nState == Experiment_SubObject_Stopped)))
			{
				nActiveExperimentObjects++;
			}			
		}
		if(nActiveExperimentObjects == 0)//No more active object? Then the experiment ended
			changeCurrentExperimentState(Experiment_Stopped);
	}
}

bool ExperimentManager::invokeExperimentObjectsSlots(const QString &sSlotName)
{
	if (!currentExperimentTree)
	{
		qDebug() << "invokeExperimentObjectSlot::No Experiment loaded!";
		return false;
	}
	int nCount = lExperimentObjectList.count();
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			//Get the meta object
			bool bRetVal;
			QString sSlotFullName = sSlotName + "()";
			const QMetaObject* metaObject = lExperimentObjectList[i].pObject->metaObject();
			////Query the properties(only Q_PROPERTY)
			//QStringList properties;
			//QString nolist;
			//for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
			//{
			//	properties << QString::fromLatin1(metaObject->property(i).name());
			//	nolist = QString::fromLatin1(metaObject->property(i).name());
			//}
			////Query the methods(only public slots!)
			//QStringList methods;
			//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
			//	methods << QString::fromLatin1(metaObject->method(i).signature());

			if (!(metaObject->indexOfMethod(sSlotFullName.toLatin1()) == -1))//Is the slot present?
			{
				//Invoke the start() slot
				if(!metaObject->invokeMethod(lExperimentObjectList.at(i).pObject, sSlotName.toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))//, Q_ARG(QString, "sqrt"), Q_ARG(int, 42), Q_ARG(double, 9.7));
					//if(!QMetaObject::invokeMethod(lExperimentObjectList[i].pObject, sMethod.toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))//, Q_ARG(QString, "sqrt"), Q_ARG(int, 42), Q_ARG(double, 9.7));
				{
					qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << sSlotFullName << ")!";		
					return false;
				}
			}

		}
		return true;
	}
	else
	{
		return true;
	}
}

bool ExperimentManager::abortExperimentObjects()
{
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_ABORT));
	return bRetVal;
}

bool ExperimentManager::stopExperimentObjects()
{
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_STOP));
	return bRetVal;
}

bool ExperimentManager::startExperimentObjects(bool bRunFullScreen)
{
	changeExperimentObjectsSignalSlots(false);
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_START));
	return bRetVal;
}

bool ExperimentManager::configureExperiment()
{
	if (!currentExperimentTree)
	{
		qDebug() << "configureExperiment::No Experiment loaded!";
		return false;
	}
	//Default Settings
	strcExperimentConfiguration.bDebugMode = false;
	strcExperimentConfiguration.nExperimentID = 0;
	strcExperimentConfiguration.nExperimentName = "";

	QStringList strList;
	strList.append(ROOT_TAG);
	strList.append(DEFINES_TAG);
	strList.append(EXPERIMENT_TAG);
	QDomNodeList tmpList;
	if (currentExperimentTree->getDocumentElements(strList,tmpList))
	{
		int nNrOfObjects = tmpList.count();
		QDomNode tmpNode;
		QDomElement tmpElement;
		QString tmpString = "";

		if (nNrOfObjects>0)
		{
			tmpNode = tmpList.at(0);//We'll only use the first one by now and ignore the rest
			if (tmpNode.isElement()) 
			{
				tmpElement = tmpNode.toElement();
				if(tmpElement.hasAttribute(ID_TAG))
				{
					tmpString = tmpElement.attribute(ID_TAG,"");//Correct ObjectID?
					if (!tmpString.isEmpty())
						strcExperimentConfiguration.nExperimentID =  tmpString.toInt();				
				}

				tmpElement = tmpNode.firstChildElement(NAME_TAG);
				tmpString = tmpElement.text();
				if (!tmpString.isEmpty())
					strcExperimentConfiguration.nExperimentName =  tmpString;

				tmpElement = tmpNode.firstChildElement(DEBUGMODE_TAG);
				tmpString = tmpElement.text();
				if (!tmpString.isEmpty())
				{
					if (tmpString == BOOL_TRUE_TAG)
						strcExperimentConfiguration.bDebugMode = true;
					else
						strcExperimentConfiguration.bDebugMode = false;
				}
				else
				{
					strcExperimentConfiguration.bDebugMode = false;
				}
			}
		}
	}
	return true;
}

bool ExperimentManager::connectExperimentObjects(bool bDisconnect, int nObjectID)
{//nObjectID only implemented when bDisconnect = true!
	if (!currentExperimentTree)
	{
		qDebug() << "connectExperimentObjects::No Experiment loaded!";
		return false;
	}
	QStringList strList;
	strList.clear();
	strList.append(ROOT_TAG);
	strList.append(CONNECTIONS_TAG);
	strList.append(OBJECT_TAG);
	if (currentExperimentTree->getDocumentElements(strList,ExperimentObjectDomNodeList))
	{
		int nNrOfObjects = ExperimentObjectDomNodeList.count();
		QDomNode tmpNode;
		QDomElement tmpElement;
		QDomNodeList tmpObjectNodeList;
		QString tmpString;

		int nSourceObjectID = -1;
		int nTargetObjectID = -1;
		QString sSourceType = "";
		QString sSourceSignature = "";
		QString sTargetType = "";
		QString sTargetSignature = "";
		QObject *pSourceObject = NULL;
		QObject *pTargetObject = NULL;

		for(int i=0;i<nNrOfObjects;i++)
		{
			tmpNode = ExperimentObjectDomNodeList.at(i);
			if (tmpNode.isElement()) 
			{
				tmpString = "";
				tmpElement = tmpNode.toElement();
				if(!tmpElement.hasAttribute(ID_TAG))
					break;
				tmpString =tmpElement.attribute(ID_TAG,"");//Correct ObjectID?
				if (tmpString.isEmpty())
					break;
				nSourceObjectID = tmpString.toInt();
				if (!(nSourceObjectID >= 0))
					break;
				pSourceObject = getObjectElementById(nSourceObjectID);
				if (pSourceObject == NULL)
					continue;

				tmpElement = tmpNode.firstChildElement(CONNECTIONS_TYPE_TAG);
				tmpString = tmpElement.text();
				if (tmpString.isEmpty())
					break;
				if (!(tmpString == CONNECTIONS_TYPE_SIGNAL_TAG))
					break;
				sSourceType = tmpString;

				tmpElement = tmpNode.firstChildElement(CONNECTIONS_SIGNATURE_TAG);
				tmpString = tmpElement.text();
				if (tmpString.isEmpty())
					break;
				sSourceSignature = tmpString;



				tmpObjectNodeList = tmpNode.toElement().elementsByTagName(CONNECTIONS_TARGET_TAG);//Retrieve all the targets
				int nTargetCount = tmpObjectNodeList.count();
				if (nTargetCount>0)
				{
					for (int j=0;j<nTargetCount;j++)//For each object
					{
						tmpString = "";
						tmpNode = tmpObjectNodeList.at(j);
						tmpElement = tmpNode.toElement();
						if(!tmpElement.hasAttribute(ID_TAG))
							continue;
						tmpString =tmpElement.attribute(ID_TAG,"");//Correct ObjectID?
						if (tmpString.isEmpty())
							continue;
						nTargetObjectID = tmpString.toInt();
						if (!(nTargetObjectID >= 0))
							continue;
						pTargetObject = getObjectElementById(nTargetObjectID);
						if (pTargetObject == NULL)
							continue;

						tmpElement = tmpNode.firstChildElement(CONNECTIONS_TYPE_TAG);
						tmpString = tmpElement.text();
						if (tmpString.isEmpty())
							continue;
						if (!((tmpString == CONNECTIONS_TYPE_SIGNAL_TAG) || (tmpString == CONNECTIONS_TYPE_SLOT_TAG)))
							continue;
						sTargetType = tmpString;

						tmpElement = tmpNode.firstChildElement(CONNECTIONS_SIGNATURE_TAG);
						tmpString = tmpElement.text();
						if (tmpString.isEmpty())
							continue;
						sTargetSignature = tmpString;

						const QMetaObject* targetMetaObject = pTargetObject->metaObject();
						const QMetaObject* sourceMetaObject = pSourceObject->metaObject();
						int nSourceID = -1;
						int nTargetID = -1;

						if (!bDisconnect)
						{
							if (sTargetType == CONNECTIONS_TYPE_SIGNAL_TAG)
							{
								if (sSourceType == CONNECTIONS_TYPE_SIGNAL_TAG)
								{
									nSourceID = sourceMetaObject->indexOfSignal(QMetaObject::normalizedSignature(sSourceSignature.toLatin1()));
									if (!(nSourceID == -1))//Is the signal present?
									{
										nTargetID = targetMetaObject->indexOfSignal(QMetaObject::normalizedSignature(sTargetSignature.toLatin1()));
										if (!(nTargetID == -1))//Is the signal present?
										{
											QMetaObject::connect(pSourceObject, nSourceID, pTargetObject, nTargetID);
										}
									}
								} 
							} 
							else if(sTargetType == CONNECTIONS_TYPE_SLOT_TAG)
							{
								if (sSourceType == CONNECTIONS_TYPE_SIGNAL_TAG)
								{
									nSourceID = sourceMetaObject->indexOfSignal(QMetaObject::normalizedSignature(sSourceSignature.toLatin1()));
									if (!(nSourceID == -1))//Is the signal present?
									{
										nTargetID = targetMetaObject->indexOfMethod(QMetaObject::normalizedSignature(sTargetSignature.toLatin1()));
										if (!(nTargetID == -1))//Is the method present?
										{
											QMetaObject::connect(pSourceObject, nSourceID, pTargetObject, nTargetID);
										}
									}
								} 
							}							
						} 
						else
						{
							if ((nObjectID == -1) ||((nSourceObjectID == nObjectID) || (nTargetObjectID == nObjectID)))
							{
								if (sTargetType == CONNECTIONS_TYPE_SIGNAL_TAG)
								{
									if (sSourceType == CONNECTIONS_TYPE_SIGNAL_TAG)
									{
										nSourceID = sourceMetaObject->indexOfSignal(QMetaObject::normalizedSignature(sSourceSignature.toLatin1()));
										if (!(nSourceID == -1))//Is the signal present?
										{
											nTargetID = targetMetaObject->indexOfSignal(QMetaObject::normalizedSignature(sTargetSignature.toLatin1()));
											if (!(nTargetID == -1))//Is the signal present?
											{
												QMetaObject::disconnect(pSourceObject, nSourceID, pTargetObject, nTargetID);
											}
										}
									} 
								} 
								else if(sTargetType == CONNECTIONS_TYPE_SLOT_TAG)
								{
									if (sSourceType == CONNECTIONS_TYPE_SIGNAL_TAG)
									{
										nSourceID = sourceMetaObject->indexOfSignal(QMetaObject::normalizedSignature(sSourceSignature.toLatin1()));
										if (!(nSourceID == -1))//Is the signal present?
										{
											nTargetID = targetMetaObject->indexOfMethod(QMetaObject::normalizedSignature(sTargetSignature.toLatin1()));
											if (!(nTargetID == -1))//Is the method present?
											{
												QMetaObject::disconnect(pSourceObject, nSourceID, pTargetObject, nTargetID);
											}
										}
									} 
								}
							}
						}
					}
				}
			}
		}
		return true;
	}
	return false;
}

QObject *ExperimentManager::getObjectElementById(int nID)
{
	if (nID >= 0)
	{
		int nObjectCount = lExperimentObjectList.count();
		if (nObjectCount>0)
		{
			for (int i=0;i<nObjectCount;i++)
			{
				if (lExperimentObjectList.at(i).nObjectID == nID)
				{
					return(lExperimentObjectList.at(i).pObject);
				}
			}
		}
	}
	return NULL;
}
				//int metaID = QMetaType::type(sClass.toLatin1());
				//if (metaID > 0)//(id != -1) 
				//{
				//	bool bRetVal = true;
				//	objectElement tmpElement;
				//	tmpElement.nObjectID = nID;
				//	tmpElement.nMetaID = metaID;
				//	tmpElement.sObjectName = sName;
				//	tmpElement.pObject = static_cast< QObject* > ( QMetaType::construct(metaID) );

				//	const QMetaObject* metaObject = tmpElement.pObject->metaObject();

				//	//QStringList properties;
				//	//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
				//	//	properties << QString::fromLatin1(metaObject->method(i).signature());

				//	if (!(metaObject->indexOfSignal(QMetaObject::normalizedSignature(SIGNAL_LOGTOMANAGER)) == -1))//Is the signal present?
				//	{
				//		//Connect the signal
				//		connect(tmpElement.pObject, SIGNAL(LogToExperimentManager(QString)), this, SIGNAL(WriteToLogOutput(QString)));//Qt::QueuedConnection --> makes it asynchronyous
				//	}

				//	if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETOBJECTID_FULL)) == -1))//Is the slot present?
				//	{
				//		//Invoke the slot
				//		bRetVal = true;
				//		if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETOBJECTID, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int, tmpElement.nObjectID))))
				//		{
				//			qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETOBJECTID << "()" << ")!";		
				//			return false;
				//		}		
				//	}

				//	if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTCONFIGURATION_FULL)) == -1))//Is the slot present?
				//	{
				//		//Invoke the slot
				//		bRetVal = true;
				//		if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETEXPERIMENTCONFIGURATION, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(ExperimentConfiguration*, &strcExperimentConfiguration))))
				//		{
				//			qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETEXPERIMENTCONFIGURATION << "()" << ")!";		
				//			return false;
				//		}		
				//	}	

				//	if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETBLOCKTRIALDOMNODELIST_FULL)) == -1))//Is the slot present?
				//	{
				//		//Invoke the slot
				//		bRetVal = true;
				//		if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETBLOCKTRIALDOMNODELIST, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QDomNodeList*, &ExperimentBlockTrialsDomNodeList))))
				//		{
				//			qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETBLOCKTRIALDOMNODELIST << "()" << ")!";		
				//			return false;
				//		}		
				//	}

				//	tmpElement.nState = Experiment_SubObject_Initialized;//This is still an inactive state!
				//	lExperimentObjectList.append(tmpElement);					
				//}
//			}
//		}
//		return true;
//	}
//	return false;
//}


bool ExperimentManager::createExperimentObjects()
{
	if (!currentExperimentTree)
	{
		qDebug() << "createExperimentObjects::No Experiment loaded!";
		return false;
	}
	QStringList strList;
	cleanupExperimentObjects();
	strList.clear();
	strList.append(ROOT_TAG);
	strList.append(DECLARATIONS_TAG);
	strList.append(OBJECT_TAG);
	if (currentExperimentTree->getDocumentElements(strList,ExperimentObjectDomNodeList))
	{
		int nNrOfObjects = ExperimentObjectDomNodeList.count();
		int nNrOfBlockTrials = 0;
		QDomNode tmpNode;
		QDomElement tmpElement;
		QString tmpString;

		if (nNrOfObjects>0)
		{
			strList.clear();
			strList.append(ROOT_TAG);
			strList.append(ACTIONS_TAG);
			strList.append(BLOCKTRIALS_TAG);
			strList.append(BLOCK_TAG);
			if (currentExperimentTree->getDocumentElements(strList,ExperimentBlockTrialsDomNodeList))
			{
				nNrOfBlockTrials = ExperimentBlockTrialsDomNodeList.count();
			}
		}
		for(int i=0;i<nNrOfObjects;i++)
		{
			tmpNode = ExperimentObjectDomNodeList.at(i);
			if (tmpNode.isElement()) 
			{
				QString tmpString1 = "";
				tmpElement = tmpNode.toElement();
				if(!tmpElement.hasAttribute(ID_TAG))
					break;
				tmpString1 =tmpElement.attribute(ID_TAG,"");//Correct ObjectID?
				if (tmpString1.isEmpty())
					break;
				//tmpElement = tmpNode.firstChildElement(tmpString);
				int nID = tmpString1.toInt();//tmpElement.text().toInt();
				if (!(nID >= 0))
					break;
				tmpString = CLASS_TAG;
				tmpElement = tmpNode.firstChildElement(tmpString);
				QString sClass = tmpElement.text();
				if (sClass.isEmpty())
					break;
				tmpString = NAME_TAG;
				tmpElement = tmpNode.firstChildElement(tmpString);
				QString sName = tmpElement.text();
				//if (sName.isEmpty())
				//	break;
	
				int metaID = QMetaType::type(sClass.toLatin1());
				if (metaID > 0)//(id != -1) 
				{
					bool bRetVal = true;
					objectElement tmpElement;
					tmpElement.nObjectID = nID;
					tmpElement.nMetaID = metaID;
					tmpElement.sObjectName = sName;
					tmpElement.pObject = static_cast< QObject* > ( QMetaType::construct(metaID) );
					
					const QMetaObject* metaObject = tmpElement.pObject->metaObject();

					//QStringList properties;
					//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
					//	properties << QString::fromLatin1(metaObject->method(i).signature());

					if (!(metaObject->indexOfSignal(QMetaObject::normalizedSignature(SIGNAL_LOGTOMANAGER)) == -1))//Is the signal present?
					{
						//Connect the signal
						connect(tmpElement.pObject, SIGNAL(LogToExperimentManager(QString)), this, SIGNAL(WriteToLogOutput(QString)));//Qt::QueuedConnection --> makes it asynchronyous
					}

					if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETOBJECTID_FULL)) == -1))//Is the slot present?
					{
						//Invoke the slot
						bRetVal = true;
						if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETOBJECTID, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int, tmpElement.nObjectID))))
						{
							qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETOBJECTID << "()" << ")!";		
							return false;
						}		
					}

					if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTCONFIGURATION_FULL)) == -1))//Is the slot present?
					{
						//Invoke the slot
						bRetVal = true;
						if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETEXPERIMENTCONFIGURATION, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(ExperimentConfiguration*, &strcExperimentConfiguration))))
						{
							qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETEXPERIMENTCONFIGURATION << "()" << ")!";		
							return false;
						}		
					}	

					if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETBLOCKTRIALDOMNODELIST_FULL)) == -1))//Is the slot present?
					{
						//Invoke the slot
						bRetVal = true;
						if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETBLOCKTRIALDOMNODELIST, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QDomNodeList*, &ExperimentBlockTrialsDomNodeList))))
						{
							qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETBLOCKTRIALDOMNODELIST << "()" << ")!";		
							return false;
						}		
					}

					tmpElement.nState = Experiment_SubObject_Initialized;//This is still an inactive state!
					lExperimentObjectList.append(tmpElement);					
				}
			}
		}
		return true;
	}
	return false;
}

bool ExperimentManager::setFullScreenMode(const bool bFullScreen)
{
	m_RunFullScreen = bFullScreen;
	return true;
}

bool ExperimentManager::setDebugMode(const bool bDebugMode)
{
	m_DebugMode = bDebugMode;
	return true;
}

//void ExperimentManager::setBackgroundColor(const QColor &col)
//{
	//this->setBackgroundBrush(QBrush(col));
//}

void ExperimentManager::changeToOpenGLView(QGraphicsView *GraphView)
{
	GraphView->setViewport(new QGLWidget(QGLFormat(QGL::DoubleBuffer)));//QGL::DoubleBuffer, SampleBuffers

	//GraphView->setViewportUpdateMode(QGraphicsView::NoViewportUpdate);
	//QGraphicsView::FullViewportUpdate	0	When any visible part of the scene changes or is reexposed, QGraphicsView will update the entire viewport. This approach is fastest when QGraphicsView spends more time figuring out what to draw than it would spend drawing (e.g., when very many small items are repeatedly updated). This is the preferred update mode for viewports that do not support partial updates, such as QGLWidget, and for viewports that need to disable scroll optimization.
	//QGraphicsView::MinimalViewportUpdate	1	QGraphicsView will determine the minimal viewport region that requires a redraw, minimizing the time spent drawing by avoiding a redraw of areas that have not changed. This is QGraphicsView's default mode. Although this approach provides the best performance in general, if there are many small visible changes on the scene, QGraphicsView might end up spending more time finding the minimal approach than it will spend drawing.
	//QGraphicsView::SmartViewportUpdate	2	QGraphicsView will attempt to find an optimal update mode by analyzing the areas that require a redraw.
	//QGraphicsView::BoundingRectViewportUpdate	4	The bounding rectangle of all changes in the viewport will be redrawn. This mode has the advantage that QGraphicsView searches only one region for changes, minimizing time spent determining what needs redrawing. The disadvantage is that areas that have not changed also need to be redrawn.
	//QGraphicsView::NoViewportUpdate	3	QGraphicsView will never update its viewport when the scene changes; the user is expected to control all updates. This mode disables all (potentially slow) item visibility testing in QGraphicsView, and is suitable for scenes that either require a fixed frame rate, or where the viewport is otherwise updated externally.
	//view.setScene(new OpenGLScene);

	//GraphView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
	////GraphView.setCacheMode(QGraphicsView.CacheBackground);// to speed up difficult background refresh speed
	//GraphView->setOptimizationFlags(QGraphicsView::DontSavePainterState | QGraphicsView::DontAdjustForAntialiasing);
		
	QDesktopWidget* desktopWidget = QApplication::desktop();
	const QRectF rScreenResolution = desktopWidget->screenGeometry();//availableGeometry();
	//int w = rScreenResolution.width();//GraphView->width();
	//int h = rScreenResolution.height();//GraphView->height();
	//this->setSceneRect(rScreenResolution);
	//GraphView->fitInView(rScreenResolution, Qt::KeepAspectRatio);
	GraphView->setSceneRect(rScreenResolution);

	//GraphView->setSceneRect(-0.5*w, -0.5*h, w, h);
	//GraphView->setHorizontalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	//GraphView->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff );
	//GraphView->updateSceneRect(QRect(0, 0, w, h));
	//boundingRect()
	//this->setItemIndexMethod(QGraphicsScene::NoIndex);
	//GraphView->setDragMode(QGraphicsView::NoDrag);
}

ContainerDlg::ContainerDlg(QWidget *parent) : QDialog(parent)
{
	//Setting a black background
	//QPalette pal = palette();
	//pal.setColor(QPalette::Window, QColor(16, 16, 16));
	//setPalette(pal);
}

ContainerDlg::~ContainerDlg()
{

}

void ContainerDlg::closeEvent(QCloseEvent *e)
{
	showNormal();
	e->accept();
}

void ContainerDlg::reject() // called when "Esc" pressed
{
	showNormal(); // to get back dock on Mac, we need to call this here (restore from fullscreen mode)
	QDialog::reject();
}