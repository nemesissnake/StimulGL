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


#include "ExperimentManager.h"
#include "ImageProcessor.h"
#include "prtformatmanager.h"
#include "TriggerTimer.h"
#include <QFileDialog>
#include <QWaitCondition>
#include <QXmlSchema>
#include <QXmlSchemaValidator>

QScriptValue ExperimentManager::ctor__experimentManager(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script
	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();
	return engine->newQObject(new ExperimentManager(NULL,engine), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

/*! \brief The ExperimentManager constructor.
*
*   You do not need to specify the parent object. 
*	The StimulGL script engine automatically retrieves the parent role
*/
ExperimentManager::ExperimentManager(QObject *parent, QScriptEngine* engine) : QObject(parent), parentObject(parent)
{
	currentScriptEngine = engine;
	DefaultConstruct();
}

bool ExperimentManager::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

void ExperimentManager::DefaultConstruct()
{
	MainAppInfo::CreateHashTableFromEnumeration(typeid(ExperimentState).name(),experimentStateHash,this->staticMetaObject);
	m_RunFullScreen = true;
	m_ExpFileName = "";
	currentExperimentTree = NULL;
	expDataLogger = NULL;
	typedExpParamCntnr = NULL;
	RegisterMetaTypes();
	changeCurrentExperimentState(ExperimentManager_Constructed);
	//rndGen = NULL;
	//rndCounter = 0;
}


/*! \brief The ExperimentManager destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
ExperimentManager::~ExperimentManager()
{
	cleanupExperiment();
}

//void ExperimentManager::Test()
//{
//	QString a = "";
//	if (rndGen == NULL)
//	{
//		rndGen = new RandomGenerator();
//		rndCounter++;
//	}
//	else
//	{
//		//rndGen->clear();
//	}
//
//	for (int j=0;j<100;j++)//Create random Empty trigger steps within the Cycle
//	{
//		a = a + "," + QString::number(rndGen->randomizeInt(0,5));
//	}
//	emit WriteToLogOutput(a);
//
//	QStringList lst;
//	QStringList preserveIndexlst;
//	for (int k=0;k<5;k++)
//	{	
//		preserveIndexlst.append(QString::number(k));
//	}
//	//lst.clear()
//	for (int i=0;i<25;i++)
//	{
//		rndGen->randomizeList(RandomGenerator_RandomizePreservedIndexes,&preserveIndexlst);
//		lst = *rndGen;
//		emit WriteToLogOutput(lst.join(","));
//	}
//}

void ExperimentManager::RegisterMetaTypes()
{//To register the Objects to the Meta, so they can be accessed trough an *.exml file
	qRegisterMetaType<RetinoMap_glwidget>(RETINOMAP_WIDGET_NAME);
	qRegisterMetaType<qmlWidget>(QML_WIDGET_NAME);
	qRegisterMetaType<TriggerTimer>(TRIGGERTIMER_NAME);
	qRegisterMetaType<ImageProcessor>(IMAGEPROCESSOR_NAME);	
	qRegisterMetaType<PrtFormatManager>(PRTFORMATMANAGER_NAME);
}

void ExperimentManager::SendToMainAppLogOutput(const QString &strText2Write)
{
	emit WriteToLogOutput(strText2Write);
}

bool ExperimentManager::insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue, bool bIsInitializing)
{
/*! \brief Inserts a specified object parameter for the current experiment block.
 *
 *  Inserts a specified object parameter with objectID(nObjectID), parameter name(sName) and parameter value(sValue) for the current experiment block.
 *  If bIsInitializing is true then this new parameter is marked as Initializing, see #ParsedParameterDefinition.bIsInitialized,
 *  this marks the parameter as unused which can be helpful for further setting the default value.
 */
	if (nObjectID >= 0) 
	{
		if (!lExperimentObjectList.isEmpty())
		{
			int nObjectCount = lExperimentObjectList.count();
			if (nObjectCount>0)
			{
				for (int i=0;i<nObjectCount;i++)
				{
					if (lExperimentObjectList[i].nObjectID == nObjectID)
					{
						ParsedParameterDefinition tmpParDef;
						tmpParDef.bHasChanged = true;
						tmpParDef.sValue = sValue;
						tmpParDef.bIsInitialized = bIsInitializing;
						lExperimentObjectList[i].ExpBlockParams->insert(sName,tmpParDef);
						return true;
					}
				}
			}
		}
	}
	return false;
}













//template< typename T > bool insertExperimentParameter(const QString &strKeyName, T *pExpParam = NULL)
//{
//	if(pExpParam == NULL)
//		return false;
//	if (typeid(T) == typeid(int))
//	{
//		hIntContainer.insert(strKeyName,pExpParam);
//		return true;
//	}
//	return false;
//}

//template< typename T > T *getExperimentParameter(const QString &strKeyName)

template< typename T > bool ExperimentManager::insertExperimentObjectBlockParameter2(const int &nObjectID,const QString &sKeyName,const T *sVariabele)
{
	if(typedExpParamCntnr == NULL)
		return NULL;
	if (nObjectID >= 0) 
	{
		if (!lExperimentObjectList.isEmpty())
		{
			int nObjectCount = lExperimentObjectList.count();
			if (nObjectCount>0)
			{
				for (int i=0;i<nObjectCount;i++)
				{
					if (lExperimentObjectList[i].nObjectID == nObjectID)
					{
						//ParsedParameterDefinition tmpParDef;
						//tmpParDef.bHasChanged = true;
						//tmpParDef.sValue = sValue;
						//tmpParDef.bIsInitialized = bIsInitializing;
						//lExperimentObjectList[i].ExpBlockParams->insert(sName,tmpParDef);
						bool bRetVal = typedExpParamCntnr->insertExperimentParameter(sName,sVariabele);
						return bRetVal;
						eqwrwer
					}
				}
			}
		}
	}
	return NULL;
}

//getExperimentParameter(const QString &strKeyName)
template< typename T > T* ExperimentManager::getExperimentObjectBlockParameter2(const int &nObjectID,const QString &sKeyName)//, ParsedParameterDefinition &strcParDef)
{
/*! \brief Returns a boolean value that determines if a specified object parameter from the current experiment block could be retrieved and set.
 *
 *  Returns true if &strcParDef could be set to a specified object parameter with objectID(nObjectID), parameter name(sName) for the current experiment block.
 */
	if (nObjectID >= 0)
	{
		if (!lExperimentObjectList.isEmpty())
		{
			int nObjectCount = lExperimentObjectList.count();
			if (nObjectCount>0)
			{
				for (int i=0;i<nObjectCount;i++)
				{
					if (lExperimentObjectList[i].nObjectID == nObjectID)
					{
						//tParsedParameterList test;
						//test = *lExperimentObjectList[i].ExpBlockParams;
						if (lExperimentObjectList[i].ExpBlockParams->contains(sName))
						{
							sfsdfsdf
							//strcParDef = lExperimentObjectList[i].ExpBlockParams->value(sName,strcParDef);
							//if (strcParDef.bIsInitialized == true)
							//{
							//	//Important, here we need to set the bFirstRequire option to false for future requests, but return the parameter with true
							//	ParsedParameterDefinition tmpPPD(strcParDef);
							//	tmpPPD.bIsInitialized = false;
							//	lExperimentObjectList[i].ExpBlockParams->insert(sName,tmpPPD);
							//}
							//return true;
						}
						return false;
					}
				}
			}
		}
	}
	return false;
}

bool ExperimentManager::getExperimentObjectBlockParameter(const int nObjectID,const QString sName, ParsedParameterDefinition &strcParDef)
{
/*! \brief Returns a boolean value that determines if a specified object parameter from the current experiment block could be retrieved and set.
 *
 *  Returns true if &strcParDef could be set to a specified object parameter with objectID(nObjectID), parameter name(sName) for the current experiment block.
 */
	if (nObjectID >= 0)
	{
		if (!lExperimentObjectList.isEmpty())
		{
			int nObjectCount = lExperimentObjectList.count();
			if (nObjectCount>0)
			{
				for (int i=0;i<nObjectCount;i++)
				{
					if (lExperimentObjectList[i].nObjectID == nObjectID)
					{
						//tParsedParameterList test;
						//test = *lExperimentObjectList[i].ExpBlockParams;
						if (lExperimentObjectList[i].ExpBlockParams->contains(sName))
						{
							strcParDef = lExperimentObjectList[i].ExpBlockParams->value(sName,strcParDef);
							if (strcParDef.bIsInitialized == true)
							{
								//Important, here we need to set the bFirstRequire option to false for future requests, but return the parameter with true
								ParsedParameterDefinition tmpPPD(strcParDef);
								tmpPPD.bIsInitialized = false;
								lExperimentObjectList[i].ExpBlockParams->insert(sName,tmpPPD);
							}
							return true;
						}
						return false;
					}
				}
			}
		}
	}
	return false;
}

bool ExperimentManager::setExperimentObjectBlockParameterStructure(const int nObjectID,tParsedParameterList *expBlockTrialStruct)
{
	if (nObjectID >= 0)
	{
		if (!lExperimentObjectList.isEmpty())
		{
			int nObjectCount = lExperimentObjectList.count();
			if (nObjectCount>0)
			{
				for (int i=0;i<nObjectCount;i++)
				{
					if (lExperimentObjectList[i].nObjectID == nObjectID)
					{
						lExperimentObjectList[i].ExpBlockParams = expBlockTrialStruct;
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool ExperimentManager::logExperimentObjectData(const int &nObjectIndex, const int &nTimerIndex, const QString &strFunction, const QString &strTag, const QString &strMessage, const QString &strValue)
{
/*! \brief Use this function to log Experiment Data to a logfile.
 *
 *  Returns a boolean value that determines if a log entry consisting of a ObjectID(nObjectIndex), TimerID(nTimerIndex), 
 *  Function(strFunction), Tag(strTag), Message(strMessage) and Value(strValue) could be forwarded to the internal Experiment Data Logger.
 */
	if (expDataLogger)
	{
		expDataLogger->setLogVars(nObjectIndex, nTimerIndex, strFunction, strTag, strMessage, strValue);
		return true;
	}
	return false;
}

int ExperimentManager::createExperimentTimer()
{
/*! \brief Use this function to create a Experiment Timer.
 *
 *  Returns the TimerID of a newly constructed Experiment Timer.
 *  If this fails the function returns -1.
 */
	if (expDataLogger)
	{
		return expDataLogger->createTimer();
	}
	return -1;
}

bool ExperimentManager::startExperimentTimer(int nIndex)
{
/*! \brief Use this function to start a Experiment Timer.
 *
 *  Returns a boolean value that determines if the Timer with TimerID(nIndex) could be started,
 *  otherwise it returns false.
 */
	if (expDataLogger)
	{
		return expDataLogger->startTimer(nIndex);
	}
	return false;
}

double ExperimentManager::restartExperimentTimer(int nIndex)
{
/*! \brief Use this function to re-start a Experiment Timer.
 *
 *  Returns a boolean value that determines if the Timer with TimerID(nIndex) could be re-started,
 *  otherwise it returns false.
 */
	if ((expDataLogger) && (nIndex >= 0))
	{
		return expDataLogger->restartTimer(nIndex);
	}
	return -1;
}

double ExperimentManager::elapsedExperimentTimerTime(int nIndex)
{
/*! \brief Use this function to retrieve the current elapsed Experiment Timer Time.
 *
 *  Returns a double value that holds the current Experiment Timer Time from the Timer with TimerID(nIndex), 
 *  if the current Time could not be fetched than the function returns -1.
 */
	if ((expDataLogger) && (nIndex >= 0))
	{
		return expDataLogger->elapsedTimerTime(nIndex);
	}
	return -1;
}

QString ExperimentManager::getCurrentExperimentState()
{
/*! \brief Use this function to retrieve the current Experiment State.
 *
 *  Returns a string containing the current Experiment State.
 */
	return experimentStateHash.key(getCurrExperimentState(),UNKNOWNENUMSTRING);
}

QString ExperimentManager::getCurrentDateTimeStamp()
{
/*! \brief Use this function to retrieve the current Date/Time.
 *
 *  Returns a string containing the current Date/Time.
 */
	return (QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
}

QString ExperimentManager::getExperimentFileName()
{
/*! \brief Returns the current set experiment filename.
 *
 *  The internal set experiment filename is returned, see #setExperimentFileName.
 */
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
/*! \brief Sets the current experiment filename.
 *
 *  The internal experiment filename is set to the parameter qstrExpFileName.
 *  Returns true if the filename exists.
 */
	m_ExpFileName = qstrExpFileName;
	QFileInfo fInfo(m_ExpFileName);
	if (fInfo.exists())
	{
		m_ExpFolder = fInfo.absolutePath();
		return true;
	}
	return false;
}

bool ExperimentManager::saveExperiment(QString strFile)
{
/*! \brief Saves the Experiment from memory to a file.
 *
 *  The current Experiment in memory is saved to a file (strFile), if strFile=="" then the experiment is automatically saved to the last set experiment filename, 
 *  see #setExperimentFileName.
 */
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

bool ExperimentManager::loadExperiment(QString strSource, bool bViewEditTree, bool bIsFile)
{
/*! \brief Loads the Experiment from a file into memory.
 *
 *  The Experiment source (strSource) is load into memory (filepath or content), if strSource=="" then the last set experiment file is automatically loaded in memory, see #setExperimentFileName.
 *  If bIsFile is set to true then the strSource contains a path to the experiment file, if bIsFile is set to false then the strSource contains the content itself.
 *  If bViewEditTree is set to true then the experiment filename is shown in a custom viewer (under construction).
 */
	if (currentExperimentTree)
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	currentExperimentTree = new ExperimentTree(MainAppInfo::getMainWindow());
	QFile file;
	QString fileName = "";

	if (bIsFile)
	{
		fileName = getExperimentFileName();
		if (strSource.isEmpty())
		{
			if (fileName.isEmpty())
				fileName = QFileDialog::getOpenFileName(NULL, tr("Open Experiment File"), QDir::currentPath(), tr("Experiment Files (*.exml *.xml);;Any file (*)"));
		}
		else
		{
			fileName = strSource;
		}
		if (fileName.isEmpty())
			return false;
		file.setFileName(fileName);
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			emit WriteToLogOutput("Could not open experiment file!");
			return false;
		}
		else
		{
			currentExperimentFile.clear();
			currentValidationFile.clear();
			currentExperimentFile = file.readAll();
			//QString aa = MainAppInfo::appXsdFilePath() + QString(PLUGIN_EXMLDOC_VALIDATION_NAME);
			file.close();
		}
	}
	else
	{
		currentExperimentFile.clear();
		currentValidationFile.clear();
		fileName = "";
		currentExperimentFile = strSource.toAscii();
	}

	QFile validationFile(MainAppInfo::appXsdFilePath() + QString(PLUGIN_EXMLDOC_VALIDATION_NAME));
	if (validationFile.open(QFile::ReadOnly | QFile::Text)) 
		currentValidationFile = validationFile.readAll();
	validationFile.close();

	//if (bIsFile)
	//{
	//	if (file.open(QFile::ReadOnly | QFile::Text))
	//	{
	//		if (currentExperimentTree->read(&file))
	//		{
	//			if (bViewEditTree)
	//				currentExperimentTree->showMaximized();
	//			setExperimentFileName(fileName);
	//			changeCurrentExperimentState(ExperimentManager_Loaded);
	//			return true;
	//		}
	//	}
	//}
	//else
	//{
	if (currentExperimentTree->read(currentExperimentFile))
	{
		if (bViewEditTree)
			currentExperimentTree->showMaximized();
		setExperimentFileName(fileName);
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return true;
	}
	currentExperimentFile.clear();
	currentValidationFile.clear();
	delete currentExperimentTree;
	currentExperimentTree = NULL;
	return false;
}

void ExperimentManager::changeCurrentExperimentState(ExperimentState expCurrState)
{
	if (expCurrState != getCurrExperimentState())
	{
		experimentCurrentState = expCurrState;
		emit ExperimentStateHasChanged(expCurrState,getCurrentDateTimeStamp());
	}
	if(expCurrState == ExperimentManager_Stopped)
	{
		WriteAndCloseExperimentOutputData();
		cleanupExperiment();
	}
}

bool ExperimentManager::validateExperiment()
{
/*! \brief Validates the current Experiment from memory.
 *
 *  The current Experiment in memory is validated and return the result.
 */

	if (!currentExperimentTree)
	{
		if((loadExperiment("", false) == false) || (!currentExperimentTree))
		{
			qDebug() << __FUNCTION__ << "::No Experiment loaded!";
			return false;
		}
	}
	if (currentExperimentFile.isEmpty())
	{
		qDebug() << __FUNCTION__ << "::No Experiment in memory to validate!";
		return false;
	}
	if (currentValidationFile.isEmpty())
	{
		qDebug() << __FUNCTION__ << "::No Experiment validation in memory!";
		return false;
	}
	if(getCurrExperimentState() != ExperimentManager_Loaded)
	{
		qDebug() << __FUNCTION__ << "::Wrong state, could not validate the experiment!";
		return false;
	}

	////const QByteArray schemaData = schemaView->toPlainText().toUtf8();
	////const QByteArray instanceData = instanceEdit->toPlainText().toUtf8();
	XmlMessageHandler messageHandler;
	QXmlSchema schema;
	schema.setMessageHandler(&messageHandler);
	schema.load(currentValidationFile);

	bool errorOccurred = false;
	if (!schema.isValid()) {
		errorOccurred = true;
	} 
	else 
	{
		QXmlSchemaValidator validator(schema);
		if (!validator.validate(currentExperimentFile))
			errorOccurred = true;
	}

	if (errorOccurred) 
	{
		errorOccurred = errorOccurred;
		QString strMessage = messageHandler.statusMessage();
		int nColumn = messageHandler.column();
		int nLine = messageHandler.line();

		nColumn = nColumn;

		//!!!!!   emit WriteToLogOutput("Invalid schema, " + strMessage + "(line:" + QString::number(nLine) + ", col:" + QString::number(nColumn) + ")");

		return true;
	} 
	//else 
	//{		
	//	//validationStatus->setText(tr("validation successful"));
	//}

	//const QString styleSheet = QString("QLabel {background: %1; padding: 3px}")
	//	.arg(errorOccurred ? QColor(Qt::red).lighter(160).name() :
	//	QColor(Qt::green).lighter(160).name());
	//validationStatus->setStyleSheet(styleSheet);
	return true;
}

bool ExperimentManager::runExperiment()
{
/*! \brief Runs the current Experiment from memory.
 *
 *  The current Experiment in memory is started if/after it has successfully loaded.
 */
	if (!validateExperiment())
		return false;
	
#ifdef Q_OS_WIN
	//bool ret = 
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#else
	QThread::currentThread()->setPriority(QThread::HighPriority);  
	// QThread::TimeCriticalPriority);
#endif

	if (!configureExperiment())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}	

	if (!createExperimentObjects())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}

	if(!connectExperimentObjects())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}
	changeCurrentExperimentState(ExperimentManager_Configured);

	if(!initializeExperiment())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}

	initializeDataLogger();//below should be after createExperimentObjects()

	if(!initExperimentObjects())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}

	changeCurrentExperimentState(ExperimentManager_Initialized);
	QString sExpFileName = 	getExperimentFileName();
	changeCurrentExperimentState(ExperimentManager_IsStarting);

	if (expDataLogger)
		expDataLogger->startTimer(nExperimentTimerIndex);

	logExperimentObjectData(0,nExperimentTimerIndex,__FUNCTION__,"","FileName = ", sExpFileName);
	
	if(!startExperimentObjects(m_RunFullScreen))
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}

	//move the mouse cursor outside the most right screen
	QDesktopWidget *dt = QApplication::desktop();
	QCursor::setPos(dt->width(), dt->height()/2); // not at bottom because then mouse movement on Mac would show dock

	changeCurrentExperimentState(ExperimentManager_Started);
	return true;
}

void ExperimentManager::initializeDataLogger()
{
	if(expDataLogger == NULL)
	{
		expDataLogger = new ExperimentLogger(this);
		nExperimentTimerIndex = createExperimentTimer();
	}
}

void ExperimentManager::abortExperiment()
{
/*! \brief Aborts the current Experiment that is running.
 *
 *  Tries to abort the current Experiment that is running, see #runExperiment.
 */
	if(getCurrExperimentState()==ExperimentManager_Started)
	{
		changeCurrentExperimentState(ExperimentManager_IsStopping);
		if(!abortExperimentObjects())
			qDebug() << __FUNCTION__ << ": Could not abort the Experiment Objects";
		QThread::currentThread()->setPriority(QThread::NormalPriority);
	}
}

void ExperimentManager::stopExperiment()
{
/*! \brief Stops the current Experiment that is running.
 *
 *  Tries to stop the current Experiment that is running, see #runExperiment.
 */
	if(getCurrExperimentState()==ExperimentManager_Started)
	{
		changeCurrentExperimentState(ExperimentManager_IsStopping);
		if(!stopExperimentObjects())
			qDebug() << "ExperimentManager::stopExperiment Could not stop the Experiment Objects";
		QThread::currentThread()->setPriority(QThread::NormalPriority);
	}
}

bool ExperimentManager::WriteAndCloseExperimentOutputData()
{
	if (expDataLogger)
	{
		QString strTemp = MainAppInfo::outputsDirPath() + "/" + QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()) + QString("_") + DEFAULT_OUTPUTFILE;
		expDataLogger->WriteToOutput(strTemp);
		delete expDataLogger;
		expDataLogger = NULL;
		return true;
	}
	return false;
}

bool ExperimentManager::cleanupExperiment()
{
	cleanupExperimentObjects();
	currentExperimentFile.clear();
	currentValidationFile.clear();
	if (currentExperimentTree)
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	if(typedExpParamCntnr)
	{
		delete typedExpParamCntnr;
		typedExpParamCntnr = NULL;
	}
	return true;
}

void ExperimentManager::cleanupExperimentObjects()
{
	if (lExperimentObjectList.isEmpty())
		return;
	int nCount = lExperimentObjectList.count();
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			if ((lExperimentObjectList[i].pObject))
			{
				//if (!(lExperimentObjectList[i].pObject->metaObject()->indexOfSignal(QMetaObject::normalizedSignature("LogToOutputWindow(QString)")) == -1))//Is the signal present?
				//{
				//	//Disconnect the signal
				//	disconnect(lExperimentObjectList[i].pObject, SIGNAL(LogToOutputWindow(QString)), this, SIGNAL(WriteToLogOutput(QString)));//Qt::QueuedConnection --> makes it asynchronyous
				//}
				//The same for "LogExpObjData(int,int,QString)"??
				//logExperimentObjectData(const int nObjectID,const int nTimerID, const QString data2Log);
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
		qDebug() << __FUNCTION__ << ":No Experiment loaded!";
		return false;
	}
	if (lExperimentObjectList.isEmpty())
		return true;
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
		qDebug() << __FUNCTION__ ":No Experiment loaded!";
	}
	if (lExperimentObjectList.isEmpty())
		return;
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
					if (lExperimentObjectList[i].nCurrentState != Experiment_SubObject_IsAborting)//Multiple Abort events could occur, catch only first one
					{
						connectExperimentObjects(true,lExperimentObjectList[i].nObjectID);//disconnect!
						lExperimentObjectList[i].pObject->deleteLater();
						lExperimentObjectList[i].nCurrentState = Experiment_SubObject_IsAborting;
					}
				}
				else if(nState == Experiment_SubObject_Stop)
				{
					if (lExperimentObjectList[i].nCurrentState != Experiment_SubObject_IsStopping)//Multiple Stop events could occur, catch only first one
					{
						connectExperimentObjects(true,lExperimentObjectList[i].nObjectID);//disconnect!
						lExperimentObjectList[i].pObject->deleteLater();
						lExperimentObjectList[i].nCurrentState = Experiment_SubObject_IsStopping;
					}
				}
				else if((nState == Experiment_SubObject_Stopped) || (nState == Experiment_SubObject_Destructing))
				{
					changeExperimentObjectsSignalSlots(true,i);
					lExperimentObjectList[i].pObject = NULL;
					lExperimentObjectList[i].nCurrentState = nState;
				}
				else//All other cases
				{
					lExperimentObjectList[i].nCurrentState = nState;//Set the new object state
				}
				//lExperimentObjectList[i].sStateHistory.nState.append(lExperimentObjectList[i].nCurrentState);
				//lExperimentObjectList[i].sStateHistory.sDateTimeStamp.append(getCurrentDateTimeStamp());
			}
			if (!((lExperimentObjectList[i].nCurrentState == Experiment_SubObject_Initialized)||(lExperimentObjectList[i].nCurrentState == Experiment_SubObject_Stopped) || 
				  (lExperimentObjectList[i].nCurrentState == Experiment_SubObject_Constructing)||(lExperimentObjectList[i].nCurrentState == Experiment_SubObject_Destructing) ))
			{
				nActiveExperimentObjects++;
			}			
		}
		if(nActiveExperimentObjects == 0)//No more active object? Then the experiment ended
			changeCurrentExperimentState(ExperimentManager_Stopped);
	}
}

bool ExperimentManager::invokeExperimentObjectsSlots(const QString &sSlotName)
{
	if (!currentExperimentTree)
	{
		qDebug() << __FUNCTION__ << ":No Experiment loaded!";
		return false;
	}
	if (lExperimentObjectList.isEmpty())
		return true;
	
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
					qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << sSlotFullName << ")!";		
					return false;
				}
			}

		}
		return true;
	}
	return true;
}

bool ExperimentManager::abortExperimentObjects()
{
	finalizeExperimentObjects();
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_OBJECT_ABORT));
	return bRetVal;
}

bool ExperimentManager::stopExperimentObjects()
{
	finalizeExperimentObjects();
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_OBJECT_STOP));
	return bRetVal;
}

bool ExperimentManager::initExperimentObjects()
{
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_OBJECT_INIT));
	return bRetVal;
}

bool ExperimentManager::startExperimentObjects(bool bRunFullScreen)
{
	changeExperimentObjectsSignalSlots(false);
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_OBJECT_START));
	return bRetVal;
}

bool ExperimentManager::configureExperiment()
{
	if (!currentExperimentTree)
	{
		qDebug() << __FUNCTION__ ":No Experiment loaded!";
		return false;
	}
	//Default Settings
	strcExperimentConfiguration.bDebugMode = false;
	strcExperimentConfiguration.nExperimentID = 0;
	strcExperimentConfiguration.nExperimentName = "";
	strcExperimentConfiguration.pExperimentManager = this;

	QStringList strList;
	strList.append(ROOT_TAG);
	strList.append(DEFINES_TAG);
	strList.append(EXPERIMENT_TAG);
	QDomNodeList tmpList;
	
	if (currentExperimentTree->getDocumentElements(strList,tmpList) >= 0)
	{
		int nNrOfObjects = tmpList.count();
		if (nNrOfObjects>0)
		{
			QDomNode tmpNode;
			QDomElement tmpElement;
			QString tmpString = "";

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
				expandExperimentBlockParameterValue(tmpString);
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
		return true;
	}
	return false;
}

void ExperimentManager::setDebugMode(bool mode)
{
	strcExperimentConfiguration.bDebugMode = mode;
}

bool ExperimentManager::getDebugMode()
{
	return strcExperimentConfiguration.bDebugMode;
}

void ExperimentManager::setExperimentName(QString name)
{
	strcExperimentConfiguration.nExperimentName = name;
}
QString ExperimentManager::getExperimentName()
{
	return strcExperimentConfiguration.nExperimentName;
}

bool ExperimentManager::finalizeExperimentObjects()
{
	return initializeExperiment(true);
}

bool ExperimentManager::initializeExperiment(bool bFinalize)
{
	if (!currentExperimentTree)
	{
		qDebug() << __FUNCTION__ "(" << bFinalize << ")::No Experiment loaded!";
		return false;
	}
	QStringList strList;
	strList.clear();
	strList.append(ROOT_TAG);
	if (bFinalize)
		strList.append(FINALIZATIONS_TAG); 
	else
		strList.append(INITIALIZATIONS_TAG);
	strList.append(OBJECT_TAG);
	if (currentExperimentTree->getDocumentElements(strList,ExperimentObjectDomNodeList) >= 0)
	{
		int nNrOfObjects = ExperimentObjectDomNodeList.count();
		if (nNrOfObjects>0)
		{
			QDomNode tmpNode;
			QDomElement tmpElement;
			QDomNodeList tmpObjectNodeList;
			QString tmpString;

			int nObjectID = -1;
			int nParameterID = -1;
			QObject *pSourceObject = NULL;
			QString sType = "";
			QString sSignature = "";
			QList<QString> sParameterNames;
			QList<QString> sParameterValues;
			QList<QString> sParameterTypes;

			for(int i=0;i<nNrOfObjects;i++)
			{
				sParameterNames.clear();
				sParameterTypes.clear();
				sParameterValues.clear();
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
					nObjectID = tmpString.toInt();
					if (!(nObjectID >= 0))
						break;
					pSourceObject = getObjectElementById(nObjectID);
					if (pSourceObject == NULL)
						continue;

					tmpElement = tmpNode.firstChildElement(INIT_FINIT_TYPE_TAG);
					tmpString = tmpElement.text();
					if (tmpString.isEmpty())
						break;
					if (!(tmpString == INIT_FINIT_TYPE_SLOT_TAG))
						break;
					sType = tmpString;

					tmpElement = tmpNode.firstChildElement(INIT_FINIT_SIGNATURE_TAG);
					tmpString = tmpElement.text();
					if (tmpString.isEmpty())
						break;
					sSignature = tmpString;

					const QMetaObject* sourceMetaObject = pSourceObject->metaObject();

					tmpElement = tmpNode.firstChildElement(PARAMETERS_TAG);
					if(tmpElement.isElement())
					{
						tmpObjectNodeList = tmpElement.elementsByTagName(PARAMETER_TAG);//Retrieve all the parameters
						int nParameterCount = tmpObjectNodeList.count();
						if (nParameterCount>0)
						{
							for (int j=0;j<nParameterCount;j++)//For each parameter
							{
								tmpString = "";
								tmpNode = tmpObjectNodeList.at(j);
								tmpElement = tmpNode.toElement();
								if(!tmpElement.hasAttribute(ID_TAG))
									continue;
								tmpString =tmpElement.attribute(ID_TAG,"");//Correct ParameterID?
								if (tmpString.isEmpty())
									continue;
								nParameterID = tmpString.toInt();
								if (!(nParameterID >= 0))
									continue;

								tmpElement = tmpNode.firstChildElement(NAME_TAG);
								tmpString = tmpElement.text();
								if (tmpString.isEmpty())
									continue;

								tmpElement = tmpNode.firstChildElement(MEMBER_TYPE_TAG);
								tmpString = tmpElement.text();
								if (tmpString.isEmpty())
									continue;

								tmpElement = tmpNode.firstChildElement(VALUE_TAG);
								tmpString = tmpElement.text();
								if (tmpString.isEmpty())
									continue;
								expandExperimentBlockParameterValue(tmpString);

								sParameterValues.append(tmpString);
								sParameterNames.append(tmpNode.firstChildElement(NAME_TAG).text());
								sParameterTypes.append(tmpNode.firstChildElement(MEMBER_TYPE_TAG).text());
							}// end of parameter loop
							int nArgCount = sParameterNames.count();						
							QByteArray normType;
							//bool bSucceeded;
							int typeId;
							//QGenericArgument sArguments[MAX_INVOKE_ARG_COUNT];// this doesn't work! weird bug??
							QGenericArgument sArguments0;
							QGenericArgument sArguments1;
							QGenericArgument sArguments2;
							QGenericArgument sArguments3;
							QGenericArgument sArguments4;
							QGenericArgument sArguments5;
							QGenericArgument sArguments6;
							QGenericArgument sArguments7;
							QGenericArgument sArguments8;
							QGenericArgument sArguments9;

							for(int k=0;k<MAX_INVOKE_ARG_COUNT;k++)//max arguments
							{							
								//sArguments.append(QGenericArgument());	
								if (k<nArgCount)
								{
									normType = QMetaObject::normalizedType(sParameterTypes[k].toLatin1());
									typeId = QMetaType::type(normType);								

									if(typeId==QMetaType::Bool)//QMetaType::Type
									{
										// below is dirty, but array doesn't work, passes wrong value to invoked function! weird bug??
										if(k==0)
											sArguments0 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==1)
											sArguments1 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==2)
											sArguments2 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==3)
											sArguments3 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==4)
											sArguments4 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==5)
											sArguments5 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==6)
											sArguments6 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==7)
											sArguments7 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==8)
											sArguments8 = Q_ARG(bool,sParameterValues[k].toInt());
										else if(k==9)
											sArguments9 = Q_ARG(bool,sParameterValues[k].toInt());
									}
									else if(typeId==QMetaType::Int)
									{
										// below is dirty, but array doesn't work, passes wrong value to invoked function! weird bug??
										if(k==0)
											sArguments0 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==1)
											sArguments1 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==2)
											sArguments2 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==3)
											sArguments3 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==4)
											sArguments4 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==5)
											sArguments5 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==6)
											sArguments6 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==7)
											sArguments7 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==8)
											sArguments8 = Q_ARG(int,sParameterValues[k].toInt());
										else if(k==9)
											sArguments9 = Q_ARG(int,sParameterValues[k].toInt());
									}
									else if(typeId==QMetaType::Short)
									{ 
										// below is dirty, but array doesn't work, passes wrong value to invoked function! weird bug??
										if(k==0)
											sArguments0 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==1)
											sArguments1 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==2)
											sArguments2 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==3)
											sArguments3 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==4)
											sArguments4 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==5)
											sArguments5 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==6)
											sArguments6 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==7)
											sArguments7 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==8)
											sArguments8 = Q_ARG(short,sParameterValues[k].toShort());
										else if(k==9)
											sArguments9 = Q_ARG(short,sParameterValues[k].toShort());
									}
									else if(typeId==QMetaType::Double)
									{
										// below is dirty, but array doesn't work, passes wrong value to invoked function! weird bug??
										if(k==0)
											sArguments0 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==1)
											sArguments1 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==2)
											sArguments2 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==3)
											sArguments3 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==4)
											sArguments4 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==5)
											sArguments5 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==6)
											sArguments6 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==7)
											sArguments7 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==8)
											sArguments8 = Q_ARG(double,sParameterValues[k].toDouble());
										else if(k==9)
											sArguments9 = Q_ARG(double,sParameterValues[k].toDouble());
									}
									else if(typeId==QMetaType::Long)
									{
										// below is dirty, but array doesn't work, passes wrong value to invoked function! weird bug??
										if(k==0)
											sArguments0 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==1)
											sArguments1 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==2)
											sArguments2 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==3)
											sArguments3 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==4)
											sArguments4 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==5)
											sArguments5 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==6)
											sArguments6 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==7)
											sArguments7 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==8)
											sArguments8 = Q_ARG(long,sParameterValues[k].toLong());
										else if(k==9)
											sArguments9 = Q_ARG(long,sParameterValues[k].toLong());
									}
									else//In all other cases we marshal to QString and give that a try... 
									{
										qWarning() << "initializeExperimentObjects(" << bFinalize << ")::Undefined argument (typeId=" << typeId << "), switching to QString to create a generic argument...";
										// below is dirty, but array doesn't work, passes wrong value to invoked function! weird bug??
										if(k==0)
											sArguments0 = Q_ARG(QString,sParameterValues[k]);
										else if(k==1)
											sArguments1 = Q_ARG(QString,sParameterValues[k]);
										else if(k==2)
											sArguments2 = Q_ARG(QString,sParameterValues[k]);
										else if(k==3)
											sArguments3 = Q_ARG(QString,sParameterValues[k]);
										else if(k==4)
											sArguments4 = Q_ARG(QString,sParameterValues[k]);
										else if(k==5)
											sArguments5 = Q_ARG(QString,sParameterValues[k]);
										else if(k==6)
											sArguments6 = Q_ARG(QString,sParameterValues[k]);
										else if(k==7)
											sArguments7 = Q_ARG(QString,sParameterValues[k]);
										else if(k==8)
											sArguments8 = Q_ARG(QString,sParameterValues[k]);
										else if(k==9)
											sArguments9 = Q_ARG(QString,sParameterValues[k]);
									}
								}
								else
								{
									break;
								}
							}
							if(!(sourceMetaObject->invokeMethod(pSourceObject,sSignature.toLatin1(),sArguments0,sArguments1,sArguments2,sArguments3,sArguments4,sArguments5,sArguments6,sArguments7,sArguments8,sArguments9)))
							{
								//QStringList methods;
								//for(int i = sourceMetaObject->methodOffset(); i < sourceMetaObject->methodCount(); ++i)
								//	methods << QString::fromLatin1(sourceMetaObject->method(i).signature());
								qDebug() << __FUNCTION__ << "(" << bFinalize << ")::Could not invoke the Method(" << sSignature << ")!";
								//bSucceeded = true;
								//break;							

								//int id = QMetaType::type(QString("QIODevice::OpenMode").toLatin1());//OPENMODE_ENUM_META_TYPE_NAME
								//if (id != -1) {
								//	void *myClassPtr = QMetaType::construct(id);
								//	//myClassPtr->setValue("ReadOnly");
								//	QVariant tmpVariant = QVariant::fromValue(myClassPtr);
								//	QIODevice::OpenMode *s = (QIODevice::OpenMode *)myClassPtr;
								//	//s = (QIODevice::OpenMode)*myClassPtr;
								//	//QVariant var;
								//	//var.setValue(s); // copy s into the variant

								//	//...

								//		// retrieve the value
								//		//MyStruct s2 = var.value<MyStruct>();

								//	QMetaType::destroy(id, myClassPtr);
								//	myClassPtr = 0;
								//}

								return false;
							}
							continue;
						}
						else//No parameters?
						{
							if(!(sourceMetaObject->invokeMethod(pSourceObject,sSignature.toLatin1())))
							{
								qDebug() << __FUNCTION__ << "(" << bFinalize << ")::Could not invoke the Method(" << sSignature << ")!";
								return false;
							}
							continue;
						}
					}
					else//No Parameters?
					{
						if(!(sourceMetaObject->invokeMethod(pSourceObject,sSignature.toLatin1())))
						{
							qDebug() << "initializeExperimentObjects(" << bFinalize << ")::Could not invoke the Method(" << sSignature << ")!";
						}
						continue;
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool ExperimentManager::expandExperimentBlockParameterValue(QString &sValue)
{
	if (!sValue.isEmpty())
	{		
		int nLastIndex = sValue.lastIndexOf("}");
		if(nLastIndex > 1)
		{
			int nFirstIndex = sValue.lastIndexOf("{");
			if((nFirstIndex >= 0) && (nFirstIndex < nLastIndex))
			{
				QVariant varResult = "";
				if(getScriptContextValue(sValue.mid(nFirstIndex+1,nLastIndex-nFirstIndex-1),varResult))
				{
					sValue.replace(nFirstIndex,nLastIndex-nFirstIndex+1,varResult.toString());
					return true;
				}
			}	
		}
	}
	return false;
}

bool ExperimentManager::connectExperimentObjects(bool bDisconnect, int nObjectID)
{//nObjectID only implemented when bDisconnect = true!
	if (!currentExperimentTree)
	{
		qDebug() << __FUNCTION__ << "::No Experiment loaded!";
		return false;
	}
	QStringList strList;
	strList.clear();
	strList.append(ROOT_TAG);
	strList.append(CONNECTIONS_TAG);
	strList.append(OBJECT_TAG);
	if (currentExperimentTree->getDocumentElements(strList,ExperimentObjectDomNodeList) >= 0)
	{
		int nNrOfObjects = ExperimentObjectDomNodeList.count();
		if (nNrOfObjects > 0)
		{
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
		}
		return true;
	}
	return false;
}

QObject *ExperimentManager::getObjectElementById(int nID)
{
	if (nID >= 0)
	{
		if (lExperimentObjectList.isEmpty())
			return NULL;
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

//QHash<QString, QString> *ExperimentManager::getObjectBlockParamListById(int nID)
tParsedParameterList *ExperimentManager::getObjectBlockParamListById(int nID)
{
	if (nID >= 0)
	{
		if (lExperimentObjectList.isEmpty())
			return NULL;
		int nObjectCount = lExperimentObjectList.count();
		if (nObjectCount>0)
		{
			for (int i=0;i<nObjectCount;i++)
			{
				if (lExperimentObjectList.at(i).nObjectID == nID)
				{
					return(lExperimentObjectList.at(i).ExpBlockParams);
				}
			}
		}
	}
	return NULL;
}

bool ExperimentManager::createExperimentObjects()
{
	if (!currentExperimentTree)
	{
		qDebug() << __FUNCTION__ << "::No Experiment loaded!";
		return false;
	}
	QStringList strList;
	cleanupExperimentObjects();
	strList.clear();
	strList.append(ROOT_TAG);
	strList.append(DECLARATIONS_TAG);
	strList.append(OBJECT_TAG);
	if (currentExperimentTree->getDocumentElements(strList,ExperimentObjectDomNodeList)>=0)
	{
		int nNrOfObjects = ExperimentObjectDomNodeList.count();
		if (nNrOfObjects > 0)
		{
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
				if (currentExperimentTree->getDocumentElements(strList,ExperimentBlockTrialsDomNodeList) >= 0)
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

						if (currentScriptEngine)
						{
							if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_MAKETHISAVAILABLEINSCRIPT_FULL)) == -1))//Is the slot present?
							{
								//Invoke the slot
								bRetVal = true;
								if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_MAKETHISAVAILABLEINSCRIPT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QString, tmpElement.sObjectName),Q_ARG(QObject*, (QObject*)currentScriptEngine))))
								{
									qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_MAKETHISAVAILABLEINSCRIPT << "()" << ")!";		
									return false;
								}		
							}
						}

						//bool bResult = false;
						//if (!(metaObject->indexOfSignal(QMetaObject::normalizedSignature("LogToOutputWindow(QString)")) == -1))//Is the signal present?
						//{
						//	//Connect the signal
						//	bResult = connect(tmpElement.pObject, SIGNAL(LogToOutputWindow(QString)), this, SIGNAL(WriteToLogOutput(QString)));//Qt::QueuedConnection --> makes it asynchronyous
							//bResult = true;
						//}

						//if (!(metaObject->indexOfSignal(QMetaObject::normalizedSignature("LogExpObjData(int,int,QString)")) == -1))//Is the signal present?
						//{
						//	//Connect the signal
						//	bResult = connect(tmpElement.pObject, SIGNAL(LogExpObjData(int,int,QString)), this, SLOT(logExperimentObjectData(int,int,QString)));//Qt::QueuedConnection --> makes it asynchronyous
						//}

						if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETMETAOBJECT_FULL)) == -1))//Is the slot present?
						{
							//Invoke the slot
							bRetVal = true;
							if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETMETAOBJECT, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal))))
							{
								qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETMETAOBJECT << "()" << ")!";		
								return false;
							}		
						}

						if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTOBJECTID_FULL)) == -1))//Is the slot present?
						{
							//Invoke the slot
							bRetVal = true;
							if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETEXPERIMENTOBJECTID, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(int, tmpElement.nObjectID))))
							{
								qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETEXPERIMENTOBJECTID << "()" << ")!";		
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
								qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETBLOCKTRIALDOMNODELIST << "()" << ")!";		
								return false;
							}		
						}

						tmpElement.nCurrentState = Experiment_SubObject_Initialized;//This is still an inactive state!
						//tmpElement.sStateHistory.nState.append(tmpElement.nCurrentState);
						//tmpElement.sStateHistory.sDateTimeStamp.append(getCurrentDateTimeStamp());
						lExperimentObjectList.append(tmpElement);					
					}
				}
			}
		}
		return true;
	}
	return false;
}

bool ExperimentManager::getScriptContextValue(const QString &sScriptContextStatement, QVariant &sScriptContextReturnValue)
{
	//if(sScriptContextStatement.left(3).toLower() == QString(QString(EXPERIMENTMANAGER_SCRIPTCONTEXT_NAME) + ".").toLower())
	//{//Here we have direct access to this (--> the current Experiment Manager used)
	//	QString sMethod = sScriptContextStatement.mid(3);
	//	QString sRetVal;
	//	QVariant vRetVal;
	//	bool a = QMetaObject::invokeMethod( this, sMethod.toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(QVariant, (QVariant)vRetVal));//Q_RETURN_ARG(QString, sRetVal));//(QVariant, sScriptContextReturnValue));
	//	sScriptContextReturnValue = sRetVal;
	//	return a;
	//}

	if (!currentScriptEngine)
	{
		QString tmpString = "... Could not expand the script object (" + sScriptContextStatement + "), the script engine is not ready!";
		emit WriteToLogOutput(tmpString);
		qDebug() << tmpString;
	}
	if (!currentScriptEngine->canEvaluate(sScriptContextStatement))
	{
		QString tmpString = "... Could not evaluate the script object (" + sScriptContextStatement + ")!";
		emit WriteToLogOutput(tmpString);
		qDebug() << tmpString;
	}
	else
	{
		QScriptValue tmpScriptValue = currentScriptEngine->evaluate(sScriptContextStatement);
		sScriptContextReturnValue = tmpScriptValue.toVariant();
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

ContainerDlg::ContainerDlg(QWidget *parent) : QDialog(parent)
{
	//Setting a black background
	QPalette pal = palette();
	pal.setColor(QPalette::Window, QColor(0, 0, 0));
	setPalette(pal);
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