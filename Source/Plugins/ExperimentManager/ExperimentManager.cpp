//ExperimentManagerplugin
//Copyright (C) 2014  Sven Gijsen
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

#include <QFileDialog>
#include <QWaitCondition>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QGridLayout>
#include "ExperimentManager.h"
#include "ImageProcessor.h"
#include "prtformatmanager.h"
#include "TriggerTimer.h"
#include "RetinotopyMapper.h"
#include "QML2Viewer.h"
#include "ExperimentTreeItem.h"
#include "ExperimentUserInterface/ExperimentGraphicEditor.h"
#include "ExperimentUserInterFace/ExperimentParameterWidgets.h"

QScriptValue ExperimentManager::ctor__experimentManager(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
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
	sExperimentOutputDataPostString = "";
	currentExperimentTree = NULL;
	cExperimentBlockTrialStructure = NULL;
	expDataLogger = NULL;
	ExpGraphicEditor = NULL;
	expParamWidgets = NULL;
	RegisterMetaTypes();
	expParamWidgets = ExperimentParameterWidgets::instance();
	changeCurrentExperimentState(ExperimentManager_Constructed);
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

QScriptValue ExperimentManager::ctor__experimentStateEnum(QScriptContext *context, QScriptEngine *engine)
{
	int nValue = context->argument(0).toInteger();	
	if (context->isCalledAsConstructor()) 
	{
		return engine->newVariant(0);	
	}
	else
	{
		return ExperimentManager::ExperimentState(nValue);
	}
}

#ifndef QT_NO_DEBUG

QString ExperimentManager::Test(const QString &sInput)
{
	//if(currentScriptEngine)
	//{
	//if(ExpGraphicEditor == NULL)
	ExpGraphicEditor = new ExperimentGraphicEditor();
	ExpGraphicEditor->setExperimentManager(this);
	if(currentExperimentTree)
		ExpGraphicEditor->setExperimentTreeModel(currentExperimentTree);
	ExpGraphicEditor->showMaximized();
	//}
	return sInput;
}
#endif

void ExperimentManager::RegisterMetaTypes()
{//To register the Objects to the Meta, so they can be accessed trough an *.exml file
	//qRegisterMetaType<RetinoMap_glwidget>(RETINOMAP_WIDGET_NAME);
	//qRegisterMetaType<qmlWidget>(QML_WIDGET_NAME);
	qRegisterMetaType<RetinotopyMapper>(RETINOTOPYMAPPER_NAME);
	qRegisterMetaType<QML2Viewer>(QML2VIEWER_NAME);
	qRegisterMetaType<TriggerTimer>(TRIGGERTIMER_NAME);
	qRegisterMetaType<TriggerTimer>(EXPERIMENTTIMER_NAME);
	qRegisterMetaType<ImageProcessor>(IMAGEPROCESSOR_NAME);	
	qRegisterMetaType<PrtFormatManager>(PRTFORMATMANAGER_NAME);
	qRegisterMetaType<PrtFormatManager>(RANDOMGENERATOR_NAME);
	qRegisterMetaType<ExperimentStructuresNameSpace::strcExperimentStructureState>(CEXPERIMENTSTRUCTURESTATE_NAME);
	qRegisterMetaType<cExperimentStructure>(CEXPERIMENTSTRUCTURE_NAME);
	qRegisterMetaType<cBlockStructure>(CBLOCKSTRUCTURE_NAME);
	qRegisterMetaType<cLoopStructure>(CLOOPSTRUCTURE_NAME);
}

void ExperimentManager::cleanupSingletons()
{
	if (expParamWidgets)
	{
		expParamWidgets->drop();
	}
}

void ExperimentManager::sendToMainAppLogOutput(const QString &strText2Write)
{
	emit WriteToLogOutput(strText2Write);
}

bool ExperimentManager::insertExperimentObjectBlockParameter(const int nObjectID,const QString sName,const QString sValue, bool bIsInitializing, bool bIsCustom)
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
						ParsedParameterDefinition tmpParDef;
						tmpParDef.bHasChanged = true;
						tmpParDef.sValue = sValue;
						tmpParDef.bIsInitialized = bIsInitializing;
						tmpParDef.bIsCustom = bIsCustom;
						lExperimentObjectList[i].ExpBlockParams->insert(sName,tmpParDef);
						tmpParDef.sValue = "";
						tmpParDef =  lExperimentObjectList[i].ExpBlockParams->value(sName);
						return true;
					}
				}
			}
		}
	}
	return false;
}

bool ExperimentManager::getExperimentObjectBlockParameter(const int nObjectID,const QString sName, ParsedParameterDefinition &strcParDef)
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
						if ((lExperimentObjectList[i].ExpBlockParams == NULL) || (lExperimentObjectList[i].ExpBlockParams->isEmpty()))
							return false;
						if (lExperimentObjectList[i].ExpBlockParams->contains(sName))
						{
							strcParDef = lExperimentObjectList[i].ExpBlockParams->value(sName,strcParDef);
							if (strcParDef.bIsInitialized == true)
							{
								//Important, here we need to set the bFirstRequire option to false for future requests, but return the parameter with true
								ParsedParameterDefinition tmpPPD(strcParDef);
								tmpPPD.bIsInitialized = false;
								tmpPPD.bIsCustom = false;
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

bool ExperimentManager::getExperimentObjectScriptValue(const int &nObjectID,const QString &sKeyName,QScriptValue &sScriptValue)
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
						if ((lExperimentObjectList[i].ExpBlockParams == NULL) || (lExperimentObjectList[i].ExpBlockParams->isEmpty()))
							return false;
						if (lExperimentObjectList[i].ExpBlockParams->contains(sKeyName.toLower()))
						{
							if(lExperimentObjectList[i].typedExpParamCntnr)
								return lExperimentObjectList[i].typedExpParamCntnr->getExperimentParameter(sKeyName,sScriptValue);
							else
								return false;
						}
					}
				}
			}
		}
	}
	return false;
}

bool ExperimentManager::setExperimentObjectFromScriptValue(const int &nObjectID,const QString &sKeyName,const QScriptValue &sScriptValue)
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
						if ((lExperimentObjectList[i].ExpBlockParams == NULL) || (lExperimentObjectList[i].ExpBlockParams->isEmpty()))
							return false;
						//int nCount = lExperimentObjectList[i].ExpBlockParams->count();
						if (lExperimentObjectList[i].ExpBlockParams->contains(sKeyName.toLower()))
						{
							if(lExperimentObjectList[i].typedExpParamCntnr)
							{
								QString tmpString = sScriptValue.toString();
								if(expandExperimentBlockParameterValue(tmpString))
									return lExperimentObjectList[i].typedExpParamCntnr->setExperimentParameter(sKeyName,tmpString);
								return lExperimentObjectList[i].typedExpParamCntnr->setExperimentParameter(sKeyName,sScriptValue);
							}
							return false;
						}
					}
				}
			}
		}
	}
	return false;
}

bool ExperimentManager::logExperimentObjectData(const int &nObjectIndex, const int &nTimerIndex, const QString &strFunction, const QString &strTag, const QString &strMessage, const QString &strValue)
{
	if (expDataLogger)
	{
		expDataLogger->setLogVars(nObjectIndex, nTimerIndex, strFunction, strTag, strMessage, strValue);
		return true;
	}
	return false;
}

int ExperimentManager::createExperimentTimer()
{
	if (expDataLogger)
	{
		return expDataLogger->createTimer();
	}
	return -1;
}

bool ExperimentManager::startExperimentTimer(int nIndex)
{
	if (expDataLogger)
	{
		return expDataLogger->startTimer(nIndex);
	}
	return false;
}

double ExperimentManager::restartExperimentTimer(int nIndex)
{
	if ((expDataLogger) && (nIndex >= 0))
	{
		return expDataLogger->restartTimer(nIndex);
	}
	return -1;
}

double ExperimentManager::elapsedExperimentTimerTime(int nIndex)
{
	if ((expDataLogger) && (nIndex >= 0))
	{
		return expDataLogger->elapsedTimerTime(nIndex);
	}
	return -1;
}

QString ExperimentManager::getCurrentExperimentState()
{
	return experimentStateHash.key(getCurrExperimentState(),UNKNOWNENUMSTRING);
}

QString ExperimentManager::getCurrentDateTimeStamp()
{
	return (QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()));
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

bool ExperimentManager::setExperimentFileName(const QString sExpFileName)
{
	m_ExpFileName = sExpFileName;
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
	if(currentExperimentTree->write(fileName))
	{
		return true;
	}
	return false;
}

bool ExperimentManager::loadExperiment(QString strSource, bool bIsFile)
{
	if (currentExperimentTree)
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	currentExperimentTree = new ExperimentTreeModel();
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
		currentExperimentFile = strSource.toLatin1();
	}

	QFile validationFile(MainAppInfo::appXsdFilePath() + QString(PLUGIN_EXMLDOC_VALIDATION_NAME));//MainAppInfo::appXsdFilePath()
	if (validationFile.open(QFile::ReadOnly | QFile::Text)) 
		currentValidationFile = validationFile.readAll();
	validationFile.close();

	if (currentExperimentTree->read(currentExperimentFile))
	{
		setExperimentFileName(fileName);
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return prePassiveParseExperiment();
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
		WriteAndCloseExperimentOutputData(sExperimentOutputDataPostString);
		cleanupExperiment();
	}
}

bool ExperimentManager::validateExperiment()
{
	if (!currentExperimentTree)
	{
		if((loadExperiment("") == false) || (!currentExperimentTree))
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
	if((getCurrExperimentState() != ExperimentManager_Loaded) && (getCurrExperimentState() != ExperimentManager_PreParsed))
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
		//int nLine = messageHandler.line();

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
#ifdef Q_OS_WIN32 //Are we on Windows?
	//bool ret = 
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#else
	QThread::currentThread()->setPriority(QThread::HighPriority);  
	// QThread::TimeCriticalPriority);
#endif

	if(!prePassiveParseExperiment())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}

	if(!createExperimentObjects())
	{
		changeCurrentExperimentState(ExperimentManager_PreParsed);
		return false;
	}
	
	if(!connectExperimentObjects())
	{
		changeCurrentExperimentState(ExperimentManager_PreParsed);
		return false;
	}
	changeCurrentExperimentState(ExperimentManager_Configured);

	if(!initializeExperiment())
	{
		changeCurrentExperimentState(ExperimentManager_PreParsed);
		return false;
	}

	initializeDataLogger();//below should be after createExperimentObjects()

	if(!initExperimentObjects())
	{
		changeCurrentExperimentState(ExperimentManager_PreParsed);
		return false;
	}

	changeCurrentExperimentState(ExperimentManager_Initialized);
	changeCurrentExperimentState(ExperimentManager_IsStarting);

	if (expDataLogger)
		expDataLogger->startTimer(nExperimentTimerIndex);

	logExperimentObjectData(0,nExperimentTimerIndex,__FUNCTION__,"","FileName = ", getExperimentFileName());
	
	if(!startExperimentObjects(m_RunFullScreen))
	{
		changeCurrentExperimentState(ExperimentManager_PreParsed);
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
	if((getCurrExperimentState()==ExperimentManager_Started) || (getCurrExperimentState()==ExperimentManager_IsStarting))
	{
		changeCurrentExperimentState(ExperimentManager_IsStopping);
		if(!abortExperimentObjects())
			qDebug() << __FUNCTION__ << ": Could not abort the Experiment Objects";
		QThread::currentThread()->setPriority(QThread::NormalPriority);
	}
}

void ExperimentManager::stopExperiment()
{
	if(getCurrExperimentState()==ExperimentManager_Started)
	{
		changeCurrentExperimentState(ExperimentManager_IsStopping);
		if(!stopExperimentObjects())
			qDebug() << "ExperimentManager::stopExperiment Could not stop the Experiment Objects";
		QThread::currentThread()->setPriority(QThread::NormalPriority);
	}
}

bool ExperimentManager::WriteAndCloseExperimentOutputData(const QString &postFileName)
{
	if (expDataLogger)
	{
		QString strTemp; 
		if(postFileName == "")			
			strTemp = MainAppInfo::outputsDirPath() + "/" + QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()) + QString("_") + DEFAULT_OUTPUTFILE;
		else
			strTemp = MainAppInfo::outputsDirPath() + "/" + QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat()) + QString("_") + postFileName + ".txt";
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
	//if(visExpEditor)
	//{
	//	visExpEditor->close();
	//	delete visExpEditor;
	//	visExpEditor = NULL;

	//}
	//if(expStructVisualizer)
	//{
	//	expStructVisualizer->close();
	//	delete expStructVisualizer;
	//	expStructVisualizer = NULL;
	//}
	if (currentExperimentTree)
	{
		delete currentExperimentTree;
		currentExperimentTree = NULL;
	}
	if(cExperimentBlockTrialStructure)
	{
		delete cExperimentBlockTrialStructure;
		cExperimentBlockTrialStructure = NULL;
	}
	if(ExpGraphicEditor)
	{
		delete ExpGraphicEditor;
		ExpGraphicEditor = NULL;
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
			if(lExperimentObjectList[i].typedExpParamCntnr)
			{
				delete lExperimentObjectList[i].typedExpParamCntnr;
				lExperimentObjectList[i].typedExpParamCntnr = NULL;
			}
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
			//QString sName = lExperimentObjectList.at(i).sObjectName;
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
	Q_UNUSED(bRunFullScreen);
	changeExperimentObjectsSignalSlots(false);
	bool bRetVal = invokeExperimentObjectsSlots(QString(FUNC_OBJECT_START));
	return bRetVal;
}

bool ExperimentManager::prePassiveParseExperiment()
{
	if (!validateExperiment())
		return false;

	if (!configureExperiment())
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}
	if (!createExperimentStructure(ExperimentBlockTrialsTreeItemList,currentExperimentTree,cExperimentBlockTrialStructure))
	{
		changeCurrentExperimentState(ExperimentManager_Loaded);
		return false;
	}
	changeCurrentExperimentState(ExperimentManager_PreParsed);
	return true;
}

bool ExperimentManager::createExperimentStructure(QList<ExperimentTreeItem*> &lExpTreeItems, ExperimentTreeModel *expTreeModel, cExperimentStructure* cExpBlockTrialStruct)
{
	if ((expTreeModel == NULL) || (cExpBlockTrialStruct == NULL))
	{
		qDebug() << __FUNCTION__ << "::Could not create the Experiment Structure!";
		return false;
	}
	QStringList strList;
	strList.clear();
	strList.append(ROOT_TAG);
	strList.append(ACTIONS_TAG);
	strList.append(BLOCKTRIALS_TAG);
	strList.append(BLOCK_TAG);

	if(ExperimentTreeModel::getStaticTreeElements(strList, lExpTreeItems,expTreeModel->getRootItem()) > 0)
	{
		if(createExperimentStructureFromTreeItemList(lExpTreeItems, cExpBlockTrialStruct))		
		{
			if(cExpBlockTrialStruct->prepareExperiment() == false)
				qDebug() << __FUNCTION__ << "::Could not prepare Experiment Structure!";
			else
				return true;
		}
		else
		{
			qDebug() << __FUNCTION__ << "::Could not parse Experiment Dom Node List!";
		}
	}
	return false;
}

bool ExperimentManager::configureExperiment()
{
	if (!currentExperimentTree)
	{
		qDebug() << __FUNCTION__ ":No Experiment loaded!";
		return false;
	}

	if(cExperimentBlockTrialStructure)
	{
		delete cExperimentBlockTrialStructure;
		cExperimentBlockTrialStructure = NULL;
	}
	//Default Settings
	cExperimentBlockTrialStructure = new cExperimentStructure();
	cExperimentBlockTrialStructure->setExperimentID(0);
	cExperimentBlockTrialStructure->setExperimentName("");
	cExperimentBlockTrialStructure->setExperimentDebugMode(false);

	QStringList strList;
	strList.append(ROOT_TAG);
	strList.append(DEFINES_TAG);
	strList.append(EXPERIMENT_TAG);	
	
	QList<ExperimentTreeItem*> lExpTreeItems;
	if(currentExperimentTree->getTreeElements(strList, lExpTreeItems) > 0)
	{
		int nNrOfObjects = lExpTreeItems.count();
		if (nNrOfObjects>0)
		{
			ExperimentTreeItem* tmpTreeItem;
			ExperimentTreeItem* tmpTreeSubItem;
			QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
			QString tmpString = "";
			//QVariant tmpVariant;

			tmpTreeItem = lExpTreeItems.at(0);//We'll only use the first one by now and ignore the rest			
			if (tmpTreeItem->hasChildren()) 
			{
				tTmpTreeItemDefs = tmpTreeItem->getDefinitions();
				if(tTmpTreeItemDefs.contains(ID_TAG))
				{
					tmpString = tTmpTreeItemDefs[ID_TAG].value.toString();
					if (!tmpString.isEmpty())//Correct ObjectID?
						cExperimentBlockTrialStructure->setExperimentID(tmpString.toInt());
				}

				tmpTreeSubItem = tmpTreeItem->firstChild(NAME_TAG);//tmpNode.firstChildElement(NAME_TAG);
				tmpString = tmpTreeSubItem->getValue();
				if (!tmpString.isEmpty())
					cExperimentBlockTrialStructure->setExperimentName(tmpString);

				tmpTreeSubItem = tmpTreeItem->firstChild(DEBUGMODE_TAG);
				tmpString = tmpTreeSubItem->getValue();				
				if (!tmpString.isEmpty())
				{
					expandExperimentBlockParameterValue(tmpString);
					if (tmpString == BOOL_TRUE_TAG)
						cExperimentBlockTrialStructure->setExperimentDebugMode(true);
					else
						cExperimentBlockTrialStructure->setExperimentDebugMode(false);
				}
				else
				{
					cExperimentBlockTrialStructure->setExperimentDebugMode(false);
				}
			}
		}
		return true;
	}
	return false;
}

void ExperimentManager::setDebugMode(bool mode)
{
	cExperimentBlockTrialStructure->setExperimentDebugMode(mode);
}

bool ExperimentManager::getDebugMode()
{
	return cExperimentBlockTrialStructure->getExperimentDebugMode();
}

void ExperimentManager::setExperimentName(QString sName)
{
	cExperimentBlockTrialStructure->setExperimentName(sName);
}
QString ExperimentManager::getExperimentName()
{
	return cExperimentBlockTrialStructure->getExperimentName();
}

void ExperimentManager::setExperimentOutputFilePostString(const QString &sPostString) 
{
	sExperimentOutputDataPostString = sPostString;
}

//VisualExperimentEditor *ExperimentManager::getVisualExperimentEditor()
//{
//	if(visExpEditor == NULL)
//		visExpEditor = new VisualExperimentEditor();
//	return visExpEditor;
//}

//bool ExperimentManager::showVisualExperimentEditor(cExperimentStructure *ExpStruct)
//{
//	if(ExpStruct == NULL)
//	{
//		if(cExperimentBlockTrialStructure == NULL)
//		{
//			//if(loadExperiment("",true) == false)
//			//	return false;
//			if (!prePassiveParseExperiment())
//				return false;
//		}
//		ExpStruct = cExperimentBlockTrialStructure;
//	}
//	if(visExpEditor == NULL)
//		visExpEditor = new VisualExperimentEditor();
//	bool bParseResult = visExpEditor->parseExperimentStructure(ExpStruct);
//	if(bParseResult)
//		visExpEditor->showMaximized();
//	return bParseResult;
//
//	//QGraphicsScene *gScene = new QGraphicsScene();
//	//QGraphicsView *gView = new QGraphicsView();//(gScene);
//	//QGridLayout *layout = new QGridLayout();
//
//	////gScene->addText("Hello, world!");	
//	//gView->setScene(gScene);
//	//gView->setRenderHint(QPainter::Antialiasing);
//	////    gView.backgroundBrush = new QBrush(new QPixmap("images/cheese.png"));
//	////    gView.cacheMode = new QGraphicsView.CacheMode(QGraphicsView.CacheBackground);
//	////	  gView->dragMode = QGraphicsView.ScrollHandDrag;
//	////    gView.viewportUpdateMode = QGraphicsView.FullViewportUpdate;	
//	////layout->addWidget(gView, 0, 0);
//	////gView->setLayout(layout);
//	//gView->setContextMenuPolicy(Qt::CustomContextMenu);
//	//gView->setWindowTitle("Experiment Graph Editor");
//	//gView->resize(400, 300);
//	//// ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
//
//	////ExperimentGraphBlock *b = new ExperimentGraphBlock(NULL, gScene);
//	////b->addPort("Block Action DoSomething", false, ExperimentGraphPort::NamePort);
//	////b->addPort("0: Block 1", false, ExperimentGraphPort::TypePort);
//	////b->addInputPort("input");
//	////b->addOutputPort("output");
//	////b = b->clone();
//	////b->setPos(150, 0);
//	////b = b->clone();
//	////b->setPos(150, 150);
//
//	//ExperimentGraphEditor *expGraphEditor = new ExperimentGraphEditor(this);
//	//expGraphEditor->install(gScene,gView);
//	//if(expGraphEditor->parseExperimentStructure(ExpStruct))
//	//{
//	//	//connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(saveFile()));
//	//	//connect(ui->action_Load, SIGNAL(triggered()), this, SLOT(loadFile()));
//	//	//connect(ui->action_Quit, SIGNAL(triggered()), qApp, SLOT(quit()));
//	//	//ui->toolBar->addAction("Add block", this, SLOT(addBlock()));
//	//	gView->showMaximized();
//	//	//delete gScene;
//	//	//delete gView;
//	//	//delete layout;
//	//}
//	//return true;
//}

cExperimentStructure *ExperimentManager::getExperimentStructure() 
{
	return cExperimentBlockTrialStructure;
}

bool ExperimentManager::setExperimentStructure(cExperimentStructure *expStruct) 
{
	if(cExperimentBlockTrialStructure)
	{
		//delete cExperimentBlockTrialStructure;
		//cExperimentBlockTrialStructure = NULL;
	}
	cExperimentBlockTrialStructure = expStruct;//new cExperimentStructure(expStruct);
	return true;
}

//QObject* ExperimentManager::getExperimentStructure() 
//{
//	return cExperimentBlockTrialStructure;
//}

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

	QList<ExperimentTreeItem*> lExpTreeItems;
	if(currentExperimentTree->getTreeElements(strList, lExpTreeItems) > 0)
	{
		int nNrOfObjects = ExperimentObjectTreeItemList.count();
		if (nNrOfObjects>0)
		{
			QString tmpString;
			int nObjectID = -1;
			int nParameterID = -1;
			QObject *pSourceObject = NULL;
			QString sType = "";
			QString sSignature = "";
			QString tmpNameString = "";
			QString tmpMemberString = "";
			int nParameterCount;
			QList<QString> sParameterNames;
			QList<QString> sParameterValues;
			QList<QString> sParameterTypes;
			ExperimentTreeItem* pExpTreeItem;
			QStringList lInitParametersSearchPath;
			lInitParametersSearchPath << PARAMETER_TAG;
			QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;

			for(int i=0;i<nNrOfObjects;i++)
			{
				sParameterNames.clear();
				sParameterTypes.clear();
				sParameterValues.clear();
				pExpTreeItem = ExperimentObjectTreeItemList.at(i);
				if (pExpTreeItem) 
				{
					tTmpTreeItemDefs = pExpTreeItem->getDefinitions();
					tmpString = "";
					if(tTmpTreeItemDefs.contains(ID_TAG))
						break;
					tmpString = tTmpTreeItemDefs[ID_TAG].value.toString();//Correct ObjectID?
					if (tmpString.isEmpty())
						break;
					nObjectID = tmpString.toInt();
					if (!(nObjectID >= 0))
						break;
					pSourceObject = getObjectElementById(nObjectID);
					if (pSourceObject == NULL)
						continue;

					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(INIT_FINIT_TYPE_TAG);
					if(pExpTreeItem==NULL)
						break;
					tmpString = pExpTreeItem->getValue();
					if (tmpString.isEmpty())
						break;
					if (!(tmpString == INIT_FINIT_TYPE_SLOT_TAG))
						break;
					sType = tmpString;

					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(INIT_FINIT_SIGNATURE_TAG);
					if(pExpTreeItem==NULL)
						break;
					tmpString = pExpTreeItem->getValue();
					if (tmpString.isEmpty())
						break;
					sSignature = tmpString;

					const QMetaObject* sourceMetaObject = NULL;
					sourceMetaObject = pSourceObject->metaObject();

					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(PARAMETERS_TAG);
					if(pExpTreeItem==NULL)
					{
						QList<ExperimentTreeItem*> tmpInitParameterTreeItemList;
						nParameterCount = ExperimentTreeModel::getStaticTreeElements(lInitParametersSearchPath, tmpInitParameterTreeItemList, pExpTreeItem);
						if(nParameterCount > 0)
						{
							for (int j=0;j<nParameterCount;j++)//For each parameter
							{
								tmpString = "";
								pExpTreeItem = tmpInitParameterTreeItemList.at(j);
								tTmpTreeItemDefs = pExpTreeItem->getDefinitions();

								if(!tTmpTreeItemDefs.contains(ID_TAG))
									continue;
								tmpString = tTmpTreeItemDefs[ID_TAG].value.toString();//Correct ParameterID?
								if (tmpString.isEmpty())
									continue;
								nParameterID = tmpString.toInt();
								if (!(nParameterID >= 0))
									continue;

								pExpTreeItem = tmpInitParameterTreeItemList.at(j)->firstChild(NAME_TAG);
								tmpNameString = pExpTreeItem->getValue();
								if (tmpNameString.isEmpty())
									continue;

								pExpTreeItem = tmpInitParameterTreeItemList.at(j)->firstChild(MEMBER_TYPE_TAG);
								tmpMemberString = pExpTreeItem->getValue();
								if (tmpMemberString.isEmpty())
									continue;

								pExpTreeItem = tmpInitParameterTreeItemList.at(j)->firstChild(VALUE_TAG);
								tmpString = pExpTreeItem->getValue();
								if (tmpString.isEmpty())
									continue;
								expandExperimentBlockParameterValue(tmpString);

								sParameterValues.append(tmpString);
								sParameterNames.append(tmpNameString);
								sParameterTypes.append(tmpMemberString);
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
					if(varResult.type() == QVariant::List)
					{
						//Let's assume a QStringList here...
						//QStringList stringList;
						QString sCombinedStringList;
						QListIterator<QVariant> it(varResult.toList());
						if (it.hasNext())
						{
							sCombinedStringList = it.next().toString();
							while (it.hasNext()) 
							{
								sCombinedStringList = sCombinedStringList + "," + it.next().toString();
									//stringList << it.next().toString();
							}
						}
						sValue.replace(nFirstIndex,nLastIndex-nFirstIndex+1,sCombinedStringList);//varResult.toString());
					}
					else
					{
						sValue.replace(nFirstIndex,nLastIndex-nFirstIndex+1,varResult.toString());
					}					
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
	if (currentExperimentTree->getTreeElements(strList,ExperimentObjectTreeItemList) > 0)
	{
		int nNrOfObjects = ExperimentObjectTreeItemList.count();
		if (nNrOfObjects > 0)
		{
			ExperimentTreeItem* pExpTreeItem;
			QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
			QString tmpString;
			int nSourceObjectID = -1;
			int nTargetObjectID = -1;
			QString sSourceType = "";
			QString sSourceSignature = "";
			QString sTargetType = "";
			QString sTargetSignature = "";
			QObject *pSourceObject = NULL;
			QObject *pTargetObject = NULL;
			QStringList lConnectionTargetSearchPath;
			int nTargetCount;
			lConnectionTargetSearchPath << CONNECTIONS_TARGET_TAG;

			for(int i=0;i<nNrOfObjects;i++)
			{
				pExpTreeItem = ExperimentObjectTreeItemList.at(i);
				if (pExpTreeItem) 
				{
					tmpString = "";
					tTmpTreeItemDefs = pExpTreeItem->getDefinitions();
					if(tTmpTreeItemDefs.contains(ID_TAG) == false)
						break;
					tmpString = tTmpTreeItemDefs[ID_TAG].value.toString();//Correct ObjectID?
					if (tmpString.isEmpty())
						break;
					nSourceObjectID = tmpString.toInt();
					if (!(nSourceObjectID >= 0))
						break;
					pSourceObject = getObjectElementById(nSourceObjectID);
					if (pSourceObject == NULL)
						continue;

					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(CONNECTIONS_TYPE_TAG);
					tmpString = pExpTreeItem->getValue();
					if (tmpString.isEmpty())
						break;
					if (!(tmpString == CONNECTIONS_TYPE_SIGNAL_TAG))
						break;
					sSourceType = tmpString;

					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(CONNECTIONS_SIGNATURE_TAG);
					tmpString = pExpTreeItem->getValue();
					if (tmpString.isEmpty())
						break;
					sSourceSignature = tmpString;
					
					QList<ExperimentTreeItem*> tmpConnectionTargetTreeItemList;
					nTargetCount = ExperimentTreeModel::getStaticTreeElements(lConnectionTargetSearchPath, tmpConnectionTargetTreeItemList, ExperimentObjectTreeItemList.at(i));
					if(nTargetCount > 0)
					{
						for (int j=0;j<nTargetCount;j++)//For each object
						{
							tmpString = "";
							pExpTreeItem = tmpConnectionTargetTreeItemList.at(j);
							tTmpTreeItemDefs = pExpTreeItem->getDefinitions();
							if(!tTmpTreeItemDefs.contains(ID_TAG))
								continue;
							tmpString = tTmpTreeItemDefs[ID_TAG].value.toString();//Correct ObjectID?
							if (tmpString.isEmpty())
								continue;
							nTargetObjectID = tmpString.toInt();
							if (!(nTargetObjectID >= 0))
								continue;
							pTargetObject = getObjectElementById(nTargetObjectID);
							if (pTargetObject == NULL)
								continue;

							pExpTreeItem = tmpConnectionTargetTreeItemList.at(j)->firstChild(CONNECTIONS_TYPE_TAG);
							tmpString = pExpTreeItem->getValue();
							if (tmpString.isEmpty())
								continue;
							if (!((tmpString == CONNECTIONS_TYPE_SIGNAL_TAG) || (tmpString == CONNECTIONS_TYPE_SLOT_TAG)))
								continue;
							sTargetType = tmpString;

							pExpTreeItem = tmpConnectionTargetTreeItemList.at(j)->firstChild(CONNECTIONS_SIGNATURE_TAG);
							tmpString = pExpTreeItem->getValue();
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

bool ExperimentManager::fetchExperimentBlockParamsFromTreeItemList(const int &nBlockNumber, const int &nObjectID)
{
	//bool bHasCurrentBlock = false;
	if (nObjectID >= 0)
	{
		if (!lExperimentObjectList.isEmpty())
		{
			int nObjectCount = lExperimentObjectList.count();
			for (int i=0;i<nObjectCount;i++)
			{
				if (lExperimentObjectList[i].nObjectID == nObjectID)
				{
					if(createExperimentBlockParamsFromTreeItemList(nBlockNumber,nObjectID,&ExperimentBlockTrialsTreeItemList,lExperimentObjectList[i].ExpBlockParams) < 0)
					{
						qDebug() << __FUNCTION__ << "::Could not create a Block Parameter List!";
						return false;
					}

					QString sParamName;
					QHashIterator<QString, ParsedParameterDefinition> hashIter(*lExperimentObjectList[i].ExpBlockParams);
					while (hashIter.hasNext()) 
					{
						hashIter.next();
						if(hashIter.value().bIsCustom)
						{
							sParamName = hashIter.key();
							//bool bResult = 
								insertExperimentObjectVariabelePointer(nObjectID,sParamName,hashIter.value().sValue,true);							
						}
					}
					return true;
				}
			}
		}
	}
	qDebug() << __FUNCTION__ << "::Could not create a Block Parameter List, no current block or object is invalid!";
	return false;
}

int ExperimentManager::createExperimentBlockParamsFromTreeItemList(const int &nBlockNumber, const int &nObjectID, QList<ExperimentTreeItem*> *pExpBlockTrialsTreeItems, tParsedParameterList *hParams)
{
	if (hParams == NULL)
		return -1;
	if(pExpBlockTrialsTreeItems == NULL)
		return -1;
	if(hParams->count() == 0)
		return -1;
	if(nObjectID < 0)
		return -1;
	if(nBlockNumber < 0)
		return -1;
	if (pExpBlockTrialsTreeItems->isEmpty())
		return -1;

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

	//if(cExperimentBlockTrialStructure == NULL)
	//	return -1;
	//int nBlockCount = cExperimentBlockTrialStructure->getBlockCount();
	int nBlockCount = pExpBlockTrialsTreeItems->count();
	if (nBlockCount <= nBlockNumber)
		return -1;
	//if (nBlockCount != pExpBlockTrialsDomNodeLst->count())
	//	return -1;


	ParsedParameterDefinition tmpParDef;
	QString tmpString;
	QString tmpValue;
	QStringList lObjectSearchPath;
	QStringList lParameterSearchPath;
	QStringList lCustomParameterSearchPath;
	lObjectSearchPath << OBJECT_TAG;
	lParameterSearchPath << OBJECT_TAG << PARAMETERS_TAG << PARAMETER_TAG;
	lCustomParameterSearchPath << OBJECT_TAG << CUSTOM_PARAMETERS_TAG << PARAMETER_TAG;

	ExperimentTreeItem* tmpTreeItem;

	for(int i=0;i<nBlockCount;i++)//Loop through the blocks
	{
		if(pExpBlockTrialsTreeItems->at(i)->hasChildren())
		{
			tmpTreeItem = pExpBlockTrialsTreeItems->at(i)->firstChild(BLOCKNUMBER_TAG);
			if(tmpTreeItem)//Is there a block_number defined?
			{
				if (nBlockNumber == tmpTreeItem->getValue().toInt())//Correct block number?
				{
					QList<ExperimentTreeItem*> tmpBlockTreeItemList;
					int nObjectListCount = ExperimentTreeModel::getStaticTreeElements(lObjectSearchPath, tmpBlockTreeItemList, pExpBlockTrialsTreeItems->at(i));
					if(nObjectListCount > 0)
					{
						QMap<QString, TreeItemDefinition> tTmpTreeObjectItemDefs;
						ExperimentTreeItem* pSubTreeItem;
						for (int j=0;j<nObjectListCount;j++)//For each object
						{
							tTmpTreeObjectItemDefs = tmpBlockTreeItemList.at(j)->getDefinitions();
							if(tTmpTreeObjectItemDefs.contains(ID_TAG))
							{
								if(nObjectID == tTmpTreeObjectItemDefs[ID_TAG].value.toInt())//Correct ObjectID?
								{
									int nResult = 0;
									pSubTreeItem = tmpBlockTreeItemList.at(j)->firstChild(PARAMETERS_TAG);
									if(pSubTreeItem)
									{										
										QList<ExperimentTreeItem*> tmpBlockParameterTreeItemList;
										int nParameterListCount = ExperimentTreeModel::getStaticTreeElements(lParameterSearchPath, tmpBlockParameterTreeItemList, tmpBlockTreeItemList.at(j));
										if(nParameterListCount > 0)
										{
											for (int k=0;k<nParameterListCount;k++)//For each parameter
											{
												pSubTreeItem = tmpBlockParameterTreeItemList.at(k)->firstChild(NAME_TAG);
												if(pSubTreeItem)
												{
													tmpString = pSubTreeItem->getValue().toLower();
													if (hParams->contains(tmpString))//Is the Parameter available in the predefined plugin list?
													{
														pSubTreeItem = tmpBlockParameterTreeItemList.at(k)->firstChild(VALUE_TAG);
														if(pSubTreeItem)
														{
															tmpValue = pSubTreeItem->getValue();
															expandExperimentBlockParameterValue(tmpValue);
															tmpParDef.sValue = tmpValue;
															tmpParDef.bHasChanged = true;
															hParams->insert(tmpString,tmpParDef);
															nResult++;
														}
													}
												}
											}
										}
									}

									//////////////////////////////////////////////////////////////////////////
									//Let's parse the custom parameters
									//////////////////////////////////////////////////////////////////////////

									pSubTreeItem = tmpBlockTreeItemList.at(j)->firstChild(CUSTOM_PARAMETERS_TAG);
									if(pSubTreeItem)
									{
										QList<ExperimentTreeItem*> tmpBlockCustomParameterTreeItemList;
										int nCustomParameterListCount = ExperimentTreeModel::getStaticTreeElements(lCustomParameterSearchPath, tmpBlockCustomParameterTreeItemList, tmpBlockTreeItemList.at(j));
										if (nCustomParameterListCount>0)
										{
											for (int k=0;k<nCustomParameterListCount;k++)//For each parameter
											{
												pSubTreeItem = tmpBlockCustomParameterTreeItemList.at(k)->firstChild(NAME_TAG);
												if(pSubTreeItem)
												{
													tmpString = pSubTreeItem->getValue().toLower();
													//if (hParams->contains(tmpString) == false)//Is the Parameter available in the predefined plugin list?
													//{
													//	qDebug() << __FUNCTION__ << "::creating a custom predefined EXML parameter: " << tmpString << "!";
													//}
													pSubTreeItem = tmpBlockCustomParameterTreeItemList.at(k)->firstChild(VALUE_TAG);
													if(pSubTreeItem)
													{
														tmpValue = pSubTreeItem->getValue();
														expandExperimentBlockParameterValue(tmpValue);
														tmpParDef.sValue = tmpValue;
														tmpParDef.bHasChanged = true;
														tmpParDef.bIsCustom = true;
														hParams->insert(tmpString,tmpParDef);
														nResult++;
													}
												}
											}
										}											
									}
									return nResult;
								}
							}
							else
							{
								return -1;//No ID tag
							}
						}//end object-for loop
					}
					else
					{
						return -1;//No Objects defined		
					}
				}
				else
				{
					continue;//Search next block
				}
			}
			else
			{
				continue;//Search next block
			}
		}
	}//end block-for loop
	return -1;
}

/*int ExperimentManager::createExperimentBlockParamsFromDomNodeList(const int &nBlockNumber, const int &nObjectID, QDomNodeList *pExpBlockTrialsDomNodeLst, tParsedParameterList *hParams)
{
	if (hParams == NULL)
		return -1;
	if(pExpBlockTrialsDomNodeLst == NULL)
		return -1;
	if(hParams->count() == 0)
		return -1;
	if(nObjectID < 0)
		return -1;
	if(nBlockNumber < 0)
		return -1;
	if (pExpBlockTrialsDomNodeLst->isEmpty())
		return -1;

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
	
	//if(cExperimentBlockTrialStructure == NULL)
	//	return -1;
	//int nBlockCount = cExperimentBlockTrialStructure->getBlockCount();
	int nBlockCount = pExpBlockTrialsDomNodeLst->count();
	if (nBlockCount <= nBlockNumber)
		return -1;
	//if (nBlockCount != pExpBlockTrialsDomNodeLst->count())
	//	return -1;
	
	QDomElement tmpElement;
	QDomNode tmpNode;
	ParsedParameterDefinition tmpParDef;
	QString tmpString;
	QString tmpValue;

	for(int i=0;i<nBlockCount;i++)//Loop through the blocks
	{
		if(pExpBlockTrialsDomNodeLst->at(i).hasChildNodes())
		{
			tmpElement = pExpBlockTrialsDomNodeLst->at(i).firstChildElement(BLOCKNUMBER_TAG);
			if(!tmpElement.isNull())//Is there a block_number defined?
			{
				if (nBlockNumber == tmpElement.text().toInt())//Correct block number?
				{
					QDomNodeList tmpObjectNodeList = pExpBlockTrialsDomNodeLst->at(i).toElement().elementsByTagName(OBJECT_TAG);//Retrieve all the objects
					int nObjectListCount = tmpObjectNodeList.count();
					if (nObjectListCount>0)
					{
						for (int j=0;j<nObjectListCount;j++)//For each object
						{
							if(tmpObjectNodeList.item(j).toElement().hasAttribute(ID_TAG))
							{
								if(nObjectID == tmpObjectNodeList.item(j).toElement().attribute(ID_TAG,"").toInt())//Correct ObjectID?
								{
									int nResult = 0;
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
															nResult++;
														}
													}
												}
											}
										}
									}
										
									//////////////////////////////////////////////////////////////////////////
									//Let's parse the custom parameters
									//////////////////////////////////////////////////////////////////////////

									if(tmpObjectNodeList.item(j).firstChildElement(CUSTOM_PARAMETERS_TAG).isElement())
									{
										tmpElement = tmpObjectNodeList.item(j).firstChildElement(CUSTOM_PARAMETERS_TAG);
										QDomNodeList tmpParameterNodeList = tmpElement.elementsByTagName(PARAMETER_TAG);//Retrieve all the parameters
										int nParameterListCount = tmpParameterNodeList.count();
										if (nParameterListCount>0)
										{
											for (int k=0;k<nParameterListCount;k++)//For each parameter
											{
												tmpElement = tmpParameterNodeList.item(k).firstChildElement(NAME_TAG);
												if(!tmpElement.isNull())
												{
													tmpString = tmpElement.text().toLower();
													//if (hParams->contains(tmpString) == false)//Is the Parameter available in the predefined plugin list?
													//{
													//	qDebug() << __FUNCTION__ << "::creating a custom predefined EXML parameter: " << tmpString << "!";
													//}
													tmpElement = tmpParameterNodeList.item(k).firstChildElement(VALUE_TAG);
													if(!tmpElement.isNull())
													{
														tmpValue = tmpElement.text();
														expandExperimentBlockParameterValue(tmpValue);
														tmpParDef.sValue = tmpValue;
														tmpParDef.bHasChanged = true;
														tmpParDef.bIsCustom = true;
														hParams->insert(tmpString,tmpParDef);
														nResult++;
													}
												}
											}
										}											
									}
									return nResult;
								}
							}
							else
							{
								return -1;//No ID tag
							}
						}//end object-for loop
					}
					else
					{
						return -1;//No Objects defined		
					}
				}
				else
				{
					continue;//Search next block
				}
			}
			else
			{
				continue;//Search next block
			}
		}
	}//end block-for loop
	return -1;
}*/

bool ExperimentManager::createExperimentStructureFromTreeItemList(const QList<ExperimentTreeItem*> &ExpBlockTrialsTreeItems, cExperimentStructure *expStruct)
{
	int nNrOfBlockTrials = ExpBlockTrialsTreeItems.count();
	expStruct->resetExperiment();
	cBlockStructure *tmpBlock = NULL;
	cLoopStructure *tmpLoop = NULL;
	ExperimentTreeItem *tmpTreeItem = NULL;
	bool bUpdateSucceeded = false;
	QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
	int nTemp;
	QString sTemp;

	for (int i=0;i<nNrOfBlockTrials;i++)
	{
		bUpdateSucceeded = false;
		if(ExpBlockTrialsTreeItems.at(i)->hasChildren())
		{
			tmpTreeItem = ExpBlockTrialsTreeItems.at(i);
			tTmpTreeItemDefs = tmpTreeItem->getDefinitions();
			if(tTmpTreeItemDefs.contains(ID_TAG))
			{
				tmpBlock = new cBlockStructure();
				tmpBlock->setBlockID(tTmpTreeItemDefs[ID_TAG].value.toInt());//Copy the BlockID
				tmpTreeItem = ExpBlockTrialsTreeItems.at(i)->firstChild(BLOCKNUMBER_TAG);
				if(tmpTreeItem)//Is there a block_number defined?
				{
					tmpBlock->setBlockNumber(tmpTreeItem->getValue().toInt());//Copy the BlockNumber
					if (tmpBlock->getBlockNumber()>=0)
					{
						tmpTreeItem = ExpBlockTrialsTreeItems.at(i)->firstChild(TRIALAMOUNT_TAG);
						if(tmpTreeItem)//Is there a TrialAmount defined?
							tmpBlock->setNumberOfTrials(tmpTreeItem->getValue().toInt());//Copy the TrialAmount
						else
							tmpBlock->setNumberOfTrials(1);//Default if not defined
						tmpTreeItem = ExpBlockTrialsTreeItems.at(i)->firstChild(INTERNALTRIGGERAMOUNT_TAG);
						if(tmpTreeItem)//Is there a TriggerAmount defined?
						{							
							sTemp = tmpTreeItem->getValue();
							if (!sTemp.isEmpty())
							{
								nTemp = sTemp.toInt();
								if (nTemp > 0)
									tmpBlock->setNumberOfInternalTriggers(nTemp);
							}
						}
						tmpTreeItem = ExpBlockTrialsTreeItems.at(i)->firstChild(EXTERNALTRIGGERAMOUNT_TAG);
						if(tmpTreeItem)//Is there a SubTriggerAmount defined?
						{
							sTemp = tmpTreeItem->getValue();
							if (!sTemp.isEmpty())
							{
								nTemp = sTemp.toInt();
								if (nTemp > 0)
									tmpBlock->setNumberOfExternalTriggers(nTemp);
							}
						}
						tmpTreeItem = ExpBlockTrialsTreeItems.at(i)->firstChild(NAME_TAG);
						if(tmpTreeItem)//Is there a Name defined?
							tmpBlock->setBlockName(tmpTreeItem->getValue());//Copy the Name
						//Are there any loops defined?
						ExperimentTreeItem *tmpLoopItem = ExpBlockTrialsTreeItems.at(i)->firstChild(LOOPS_TAG);
						if(tmpLoopItem)
						{
							if(tmpLoopItem->childCount() > 0)
							{
								int nTempLoopID = 0;
								QString sTempLoopName = "";
								int nTempLoopNumber = 0;
								int nTempLoopCount = 0;
								int nTempLoopTargetBlockID = 0;
								ExperimentTreeItem *tmpLoopTreeItemParams = NULL;
								bool bLoopParseResult = true;

								tmpTreeItem = tmpLoopItem->firstChild(LOOP_TAG);
								while(tmpTreeItem)
								{
									tTmpTreeItemDefs = tmpTreeItem->getDefinitions();
									if(tTmpTreeItemDefs.contains(ID_TAG))
									{
										if (tmpTreeItem->getName() == LOOP_TAG) 
										{
											bLoopParseResult = false;
											nTempLoopID = tTmpTreeItemDefs[ID_TAG].value.toInt();//Copy the LoopID
											tmpLoopTreeItemParams = tmpTreeItem->firstChild(NAME_TAG);
											if(tmpLoopTreeItemParams)//Is it defined?
											{
												sTempLoopName = tmpLoopTreeItemParams->getValue();//Copy the LoopName
												tmpLoopTreeItemParams = tmpTreeItem->firstChild(LOOP_NUMBER_TAG);
												if(tmpLoopTreeItemParams)//Is it defined?
												{
													nTempLoopNumber = tmpLoopTreeItemParams->getValue().toInt();//Copy the LoopNumber
													tmpLoopTreeItemParams = tmpTreeItem->firstChild(LOOP_AMOUNT_TAG);
													if(tmpLoopTreeItemParams)//Is it defined?
													{
														nTempLoopCount = tmpLoopTreeItemParams->getValue().toInt();//Copy the LoopNumber
														tmpLoopTreeItemParams = tmpTreeItem->firstChild(LOOP_TARGETBLOCKID_TAG);
														if(tmpLoopTreeItemParams)//Is it defined?
														{
															nTempLoopTargetBlockID = tmpLoopTreeItemParams->getValue().toInt();//Copy the LoopNumber
															tmpLoop = new cLoopStructure(nTempLoopID,nTempLoopNumber,nTempLoopTargetBlockID,sTempLoopName,nTempLoopCount);
															bLoopParseResult = tmpBlock->insertLoop(tmpLoop);////&cLoopStructure(nTempLoopID,nTempLoopNumber,nTempLoopTargetBlockID,sTempLoopName,nTempLoopCount));//tmpLoop);
														}
													}
												}
											}
										} 
										if(bLoopParseResult == false)
											qDebug() << __FUNCTION__ << "::Could not parse loop structure, false declared loop!";
										tmpTreeItem = tmpTreeItem->nextSiblingTreeItem();
									}
								}
							}
						}
						expStruct->insertBlock(tmpBlock);//Here we should make a copy to reserve and keep the memory space
						bUpdateSucceeded = true;
					}
				}
			}
		}
		if(!bUpdateSucceeded)
		{
			expStruct->resetExperiment();
			return false;
		}
	}
	return true;
}

/*bool ExperimentManager::createExperimentStructureFromDomNodeList(const QDomNodeList &ExpBlockTrialsDomNodeLst, cExperimentStructure *expStruct)
{
	int nNrOfBlockTrials = ExpBlockTrialsDomNodeLst.count();
	expStruct->resetExperiment();
	cBlockStructure *tmpBlock = NULL;
	cLoopStructure *tmpLoop = NULL;
	QDomElement tmpElement;
	bool bUpdateSucceeded = false;
	for (int i=0;i<nNrOfBlockTrials;i++)
	{
		bUpdateSucceeded = false;
		if(ExpBlockTrialsDomNodeLst.at(i).hasChildNodes())
		{
			tmpElement = ExpBlockTrialsDomNodeLst.at(i).toElement();
			if(tmpElement.hasAttribute(ID_TAG))
			{
				tmpBlock = new cBlockStructure();
				tmpBlock->setBlockID(tmpElement.attribute(ID_TAG,"").toInt());//Copy the BlockID
				tmpElement = ExpBlockTrialsDomNodeLst.at(i).firstChildElement(BLOCKNUMBER_TAG);
				if(!tmpElement.isNull())//Is there a block_number defined?
				{
					tmpBlock->setBlockNumber(tmpElement.text().toInt());//Copy the BlockNumber
					//tmpBlockTrialStruct.nBlockNumber = tmpElement.text().toInt();//Copy the BlockNumber
					if (tmpBlock->getBlockNumber()>=0)
					{
						tmpElement = ExpBlockTrialsDomNodeLst.at(i).firstChildElement(TRIALAMOUNT_TAG);
						if(!tmpElement.isNull())//Is there a TrialAmount defined?
							tmpBlock->setNumberOfTrials(tmpElement.text().toInt());//Copy the TrialAmount
						else
							tmpBlock->setNumberOfTrials(1);//Default if not defined
						tmpElement = ExpBlockTrialsDomNodeLst.at(i).firstChildElement(INTERNALTRIGGERAMOUNT_TAG);
						if(!tmpElement.isNull())//Is there a TriggerAmount defined?
						{
							if (!tmpElement.text().isEmpty())
							{
								if (tmpElement.text().toInt() > 0)
									tmpBlock->setNumberOfInternalTriggers(tmpElement.text().toInt());
							}
						}
						tmpElement = ExpBlockTrialsDomNodeLst.at(i).firstChildElement(EXTERNALTRIGGERAMOUNT_TAG);
						if(!tmpElement.isNull())//Is there a SubTriggerAmount defined?
						{
							if (!tmpElement.text().isEmpty())
							{
								if (tmpElement.text().toInt() > 0)
									tmpBlock->setNumberOfExternalTriggers(tmpElement.text().toInt());
							}
						}
						tmpElement = ExpBlockTrialsDomNodeLst.at(i).firstChildElement(NAME_TAG);
						if(!tmpElement.isNull())//Is there a Name defined?
							tmpBlock->setBlockName(tmpElement.text());//Copy the Name

						//Are there any loops defined?
						if(!ExpBlockTrialsDomNodeLst.at(i).firstChildElement(LOOPS_TAG).isNull())
						{
							if(ExpBlockTrialsDomNodeLst.at(i).firstChildElement(LOOPS_TAG).hasChildNodes())
							{
								int nTempLoopID = 0;
								QString sTempLoopName = "";
								int nTempLoopNumber = 0;
								int nTempLoopCount = 0;
								int nTempLoopTargetBlockID = 0;
								QDomElement tmpLoopElementParams;
								bool bLoopParseResult = true;

								tmpElement = ExpBlockTrialsDomNodeLst.at(i).firstChildElement(LOOPS_TAG).firstChildElement(LOOP_TAG);
								while (!tmpElement.isNull()) 
								{
									if(tmpElement.hasAttribute(ID_TAG))
									{
										if (tmpElement.tagName() == LOOP_TAG) 
										{
											bLoopParseResult = false;
											nTempLoopID = tmpElement.attribute(ID_TAG,"").toInt();//Copy the LoopID
											tmpLoopElementParams = tmpElement.firstChildElement(NAME_TAG);
											if(!tmpLoopElementParams.isNull())//Is it defined?
											{
												sTempLoopName = tmpLoopElementParams.text();//Copy the LoopName
												tmpLoopElementParams = tmpElement.firstChildElement(LOOP_NUMBER_TAG);
												if(!tmpLoopElementParams.isNull())//Is it defined?
												{
													nTempLoopNumber = tmpLoopElementParams.text().toInt();//Copy the LoopNumber
													tmpLoopElementParams = tmpElement.firstChildElement(LOOP_AMOUNT_TAG);
													if(!tmpLoopElementParams.isNull())//Is it defined?
													{
														nTempLoopCount = tmpLoopElementParams.text().toInt();//Copy the LoopNumber
														tmpLoopElementParams = tmpElement.firstChildElement(LOOP_TARGETBLOCKID_TAG);
														if(!tmpLoopElementParams.isNull())//Is it defined?
														{
															nTempLoopTargetBlockID = tmpLoopElementParams.text().toInt();//Copy the LoopNumber
															tmpLoop = new cLoopStructure(nTempLoopID,nTempLoopNumber,nTempLoopTargetBlockID,sTempLoopName,nTempLoopCount);
															bLoopParseResult = tmpBlock->insertLoop(tmpLoop);////&cLoopStructure(nTempLoopID,nTempLoopNumber,nTempLoopTargetBlockID,sTempLoopName,nTempLoopCount));//tmpLoop);
														}
													}
												}
											}
										} 
										if(bLoopParseResult == false)
											qDebug() << __FUNCTION__ << "::Could not parse loop structure, false declared loop!";
										tmpElement = tmpElement.nextSiblingElement();											
									}
								}
							}
						}
						expStruct->insertBlock(tmpBlock);//Here we should make a copy to reserve and keep the memory space
						bUpdateSucceeded = true;
					}
				}
			}
		}
		if(!bUpdateSucceeded)
		{
			expStruct->resetExperiment();
			//if(tmpBlock)
			//	delete tmpBlock;
			//if(tmpLoop)
			//	delete tmpLoop;
			return false;
		}
	}
	//if(tmpBlock)
	//	delete tmpBlock;
	//if(tmpLoop)
		//delete tmpLoop;
	return true;
}*/

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
	if (currentExperimentTree->getTreeElements(strList,ExperimentObjectTreeItemList)>=0)
	{
		int nNrOfObjects = ExperimentObjectTreeItemList.count();
		if (nNrOfObjects > 0)
		{
			ExperimentTreeItem *pExpTreeItem;
			QMap<QString, TreeItemDefinition> tTmpTreeItemDefs;
			//QString tmpString;
			for(int i=0;i<nNrOfObjects;i++)
			{
				pExpTreeItem = ExperimentObjectTreeItemList.at(i);
				if (pExpTreeItem) 
				{
					QString tmpString1 = "";
					tTmpTreeItemDefs = pExpTreeItem->getDefinitions();
					if(!tTmpTreeItemDefs.contains(ID_TAG))
						break;
					tmpString1 = tTmpTreeItemDefs[ID_TAG].value.toString();//Correct ObjectID?
					if (tmpString1.isEmpty())
						break;
					//tmpElement = tmpNode.firstChildElement(tmpString);
					int nID = tmpString1.toInt();//tmpElement.text().toInt();
					if (!(nID >= 0))
						break;
					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(CLASS_TAG);
					if(pExpTreeItem == NULL)
						break;
					QString sClass = pExpTreeItem->getValue();
					if (sClass.isEmpty())
						break;
					pExpTreeItem = ExperimentObjectTreeItemList.at(i)->firstChild(NAME_TAG);
					QString sName = "";
					if(pExpTreeItem)
						sName = pExpTreeItem->getValue();
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
						tmpElement.pObject = static_cast< QObject* > ( QMetaType::create(metaID) );//( QMetaType::construct(metaID) );
						tmpElement.typedExpParamCntnr = NULL;
						tmpElement.ExpBlockParams = NULL;
					
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

						if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTMANAGER_FULL)) == -1))//Is the slot present?
						{
							//Invoke the slot
							bRetVal = true;
							if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETEXPERIMENTMANAGER, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(ExperimentManager*, this))))
							{
								qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETEXPERIMENTMANAGER << "()" << ")!";		
								return false;
							}		
						}	

						//if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETEXPERIMENTCONFIGURATION_FULL)) == -1))//Is the slot present?
						//{
						//	//Invoke the slot
						//	bRetVal = true;
						//	if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETEXPERIMENTCONFIGURATION, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(ExperimentConfiguration*, &strcExperimentConfiguration))))
						//	{
						//		qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << FUNC_SETEXPERIMENTCONFIGURATION << "()" << ")!";		
						//		return false;
						//	}		
						//}	

						//if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(FUNC_SETBLOCKTRIALDOMNODELIST_FULL)) == -1))//Is the slot present?
						//{
						//	//Invoke the slot
						//	bRetVal = true;
						//	if(!(metaObject->invokeMethod(tmpElement.pObject, FUNC_SETBLOCKTRIALDOMNODELIST, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal),Q_ARG(QDomNodeList*, &ExperimentBlockTrialsDomNodeList))))
						//	{
						//		qDebug() << __FUNCTION__ << "::Could not invoke the slot(" << FUNC_SETBLOCKTRIALDOMNODELIST << "()" << ")!";		
						//		return false;
						//	}		
						//}

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
		return false;
	}
	else
	{
		QString adjScriptContextStatement = "StimulGL.executeScriptContent(\"" + sScriptContextStatement + "\");";
		//if (!currentScriptEngine->canEvaluate(adjScriptContextStatement))
		//{
		//	QString tmpString = "... Could not evaluate the script object (" + sScriptContextStatement + ")!";
		//	emit WriteToLogOutput(tmpString);
		//	qDebug() << tmpString;
		//	return false;
		//}
		//else
		//{
		//	//QScriptContext *tmpContext = currentScriptEngine->currentContext();
			QScriptValue tmpScriptValue = currentScriptEngine->evaluate(adjScriptContextStatement);
			//if(currentScriptEngine->hasUncaughtException())
			//{
			//	QString tmpString = "... Evaluate script::UncaughtException() (" + sScriptContextStatement + ")!";
			//	emit WriteToLogOutput(tmpString);
			//	qDebug() << tmpString;
			//}
			sScriptContextReturnValue = tmpScriptValue.toVariant();
			return true;
		//}
	}
	return false;
}

