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

#include "QML2Viewer.h"
//#include "ModelIndexProvider.h"
#include <QQmlError>
#include <QQmlEngine>
#include <QMetaObject>

/*! \brief The QML2Viewer constructor.
*
*   You do not need to specify the parent object. 
*	The StimulGL script engine automatically retrieves the parent role
*/
QML2Viewer::QML2Viewer(QObject *parent) : ExperimentEngine(parent)
{
	initialize();
}

/*! \brief The QML2Viewer destructor.
*
*   You do not need call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
QML2Viewer::~QML2Viewer()
{
	//disconnect(quick2Viewer, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(onStatusChanged(QDeclarativeView::Status)));
	bool bResult = disconnect(quick2Viewer, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(onStatusChanged(QQuickView::Status)));
	//disconnect(quick2Viewer, &QtQuick2ApplicationViewer::ViewClosed, this, &QML2Viewer::onQuickViewClosed);
	//if (imgLstModel)
	//{
	//	delete imgLstModel;
	//	imgLstModel = NULL;
	//}	
	if (rootObject)
	{
		//delete rootObject; only a pointer..
		rootObject = NULL;
	}
	if (quick2Viewer)
	{
		delete quick2Viewer;
		quick2Viewer = NULL;
	}
	//if(quick2Container)
	//{
	//	delete quick2Container;
	//	quick2Container = NULL;
	//}
	//if (qmlErrorHandler)
	//{
	//	delete qmlErrorHandler;
	//	qmlErrorHandler = NULL;
	//}
	if(tmpFile.exists())
		if(tmpFile.remove() == false)
			qWarning() << __FUNCTION__ << "Could not remove a temporarily file(" << tmpFile.fileName() << ")"; 
}

bool QML2Viewer::executeQML2Document(const QString &strSource, bool bIsFile)//QVBoxLayout *layout) 
{
	if(bIsFile)
		qmlMainFilePath = strSource;
	else
		qmlMainFilePath = "";
	if(initObject())
	{
		if(bIsFile)
		{
			return startObject();
		}
		else
		{
			qml2EventRoutine(false,strSource);
			return true;
		}
	}
	return false;
}

bool QML2Viewer::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		QScriptEngine *currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		setScriptEngine(currentScriptEngine);
		return true;
	}
	return false;
}

QScriptValue QML2Viewer::ctor_QML2Viewer(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new QML2Viewer(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

void QML2Viewer::initialize()
{
	//Place your custom initialization code here:
	
	//Default values if none defined.
	experimentManager = NULL;
	nQML2ViewerID = -1;
	rectScreenRes = QApplication::desktop()->screenGeometry();
	quick2Viewer = NULL;
	//quick2Container = NULL;
	rootObject = NULL;
}

void QML2Viewer::parseExperimentObjectBlockParameters(bool bInit, bool bSetOnlyToDefault)
{
	if (bInit)
	{	
		colorBackground = QColor(87,87,87);//gives "#575757";
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQML2ViewerID,QML2VIEWER_BACKGROUNDCOLOR,colorBackground);
		QString qmlMainFilePath = "";
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQML2ViewerID,QML2VIEWER_MAINFILEPATH,qmlMainFilePath);
		stimHeigthPixelAmount = rectScreenRes.height();
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQML2ViewerID,QML2VIEWER_HEIGHT_PIXEL_AMOUNT,stimHeigthPixelAmount);
		stimWidthPixelAmount = rectScreenRes.width();
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQML2ViewerID,QML2VIEWER_WIDTH_PIXEL_AMOUNT,stimWidthPixelAmount);
	} 
	else
	{
		ParsedParameterDefinition pParDef;//((pParDef.bIsInitialized) && (pParDef.bHasChanged == false))
		bResolutionChanged = false;
		pParDef = getExpObjectBlockParameter(nQML2ViewerID,QML2VIEWER_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount));
		if (pParDef.bHasChanged)
		{
			stimHeigthPixelAmount = pParDef.sValue.toInt();
			bResolutionChanged = true;
		}
		pParDef = getExpObjectBlockParameter(nQML2ViewerID,QML2VIEWER_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount));
		if (pParDef.bHasChanged)
		{
			stimWidthPixelAmount = pParDef.sValue.toInt();
			bResolutionChanged = true;
		}
		//if (bResolutionChanged)
			//setStimuliResolution(stimWidthPixelAmount,stimHeigthPixelAmount);

		pParDef = getExpObjectBlockParameter(nQML2ViewerID,QML2VIEWER_BACKGROUNDCOLOR,colorBackground.name());
		if (pParDef.bHasChanged)
		{
			colorBackground = QColor(pParDef.sValue.toLower());
			//GlWidgetPallette = glWidget->palette();
			//GlWidgetPallette.setColor(glWidget->backgroundRole(), colorBackground);
			//glWidget->setPalette(GlWidgetPallette);
		}
		pParDef = getExpObjectBlockParameter(nQML2ViewerID,QML2VIEWER_MAINFILEPATH,qmlMainFilePath);
		if (pParDef.bHasChanged)
		{
			qmlMainFilePath = pParDef.sValue;
		}
	}
}

bool QML2Viewer::initObject()
{
	//quick2Container = new Quick2ContainerWidget();
	quick2Viewer = new QtQuick2ApplicationViewer();

	//qobject_cast<QQuickWindow*>(quick2Viewer)->installEventFilter(this);
	quick2Viewer->installEventFilter(this);
	//quick2Container->setWindow(quick2Viewer);
	bool bResult = connect(quick2Viewer, SIGNAL(statusChanged(QQuickView::Status)), this, SLOT(onStatusChanged(QQuickView::Status)));
	//bResult = connect(quick2Viewer, SIGNAL(ViewClosed()), this, SLOT(onQuickViewClosed()));
	//quick2Container->show();
	//if (!imgLstModel)
	//{
	//	imgLstModel = new ImageListModel();
	//	qmlViewer->engine()->addImageProvider(DEFAULT_IMAGEBUFFER_NAME, new ModelIndexProvider(*imgLstModel));//Qt::DisplayRole
	//}	
	QString extPluginPath = MainAppInfo::qmlExtensionsPluginDirPath();
	QStringList importPaths = quick2Viewer->engine()->importPathList();
	if (!importPaths.contains(extPluginPath))
	{
		quick2Viewer->engine()->addImportPath(extPluginPath);
		qDebug() << __FUNCTION__ "::Added the QML2 extension Plugin path (" << extPluginPath << ").";
	}
	//qmlErrorHandler = new QmlErrorHandler(*qmlViewer,this);
	parseExperimentObjectBlockParameters(true,false);
	setExperimentMetaObject();
	return true;
}

bool QML2Viewer::eventFilter(QObject *target, QEvent *event)
{
	if (target == quick2Viewer)
	{
		if (event->type() == QEvent::KeyPress) 
		{
			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

			switch (keyEvent->key())
			{
			case Qt::Key_A://Key_Escape:	//To abort the experiment
				if((keyEvent->modifiers() & Qt::ControlModifier))// && (keyEvent->modifiers() & Qt::AltModifier))
				{
					emit ExperimentEngine::UserWantsToClose();
				}
				break;
				//case Qt::Key_Tab:
				//	setExperimentObjectReadyToUnlock();
				//	break;
			case Qt::Key_Alt:	//To start the experiment
				ExperimentEngine::setExperimentObjectReadyToUnlock();
				//if(target == alternativeContainerDlg)//here we don't have a trigger object because of the lack of the ExperimentManager, so we trigger it automatically to start
				//	tStimTimer.singleShot(100, this, SLOT(incrementExternalTrigger()));//incrementExternalTrigger();
				break;
			}
		}
	}
	return false;
}

bool QML2Viewer::startObject()
{	
	qml2EventRoutine(false);
	return true;
}

bool QML2Viewer::stopObject()
{
	//if (qmlErrorHandler)
	//{
	//	if (!qmlErrorHandler->errorOccured()) 
	//	{
	//		int a = 9;
	//	} 
	//	else 
	//	{
	//		int a = 96;
	//	}
	//}
	//disconnect(&tWidgetUpdateLoopTimer, SIGNAL(timeout()), this, SLOT(callAnimate()));
	//tWidgetUpdateLoopTimer.stop();
	return true;
}

bool QML2Viewer::setObjectID(int nObjID)
{
	nQML2ViewerID = nObjID;//or use getObjectID();//Because we need it often we'll buffer it here
	return true;
}

bool QML2Viewer::initObjectBlockTrial()
{
	parseExperimentObjectBlockParameters(false);
	return true;
}

QVariant QML2Viewer::invokeQml2Method(QString strRootObjectName, QString strMethodName, QVariant inputValue1, QVariant inputValue2, QVariant inputValue3, QVariant inputValue4, QVariant inputValue5, QVariant inputValue6, QVariant inputValue7, QVariant inputValue8, QVariant inputValue9)
{
/*! \brief Invokes a QML Method.
 *
 *  This function can invoke/call a defined function within the current QML context.
 * @param strRootObjectName the name of the root item containing the function definition in the QML context.
 * @param strMethodName the name of the function to be called
 * @param inputValue1 - these parameters are automatically passed to the function if they are defined.
 * @return a QVariant containing the invoked function return value converted to a QVariant.
 */
	if (!rootObject)
	{
		//rootObject = dynamic_cast<QObject *>(quick2Viewer->rootObject());// get root object
		if (!rootObject)
		{
			qDebug() << __FUNCTION__ "::Failed to retrieve the root QML object.";
			return NULL;
		}
	}
	QObject *methodsContainerObject;
	if (strRootObjectName.isEmpty())
		methodsContainerObject = rootObject;
	else
		methodsContainerObject = rootObject->findChild<QObject *>(strRootObjectName);
	if (methodsContainerObject)  // element found
	{
		QVariant returnedValue;
		bool bInvokeSucceeded = false;

		if ((inputValue1.type() == 0) && (inputValue1.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue));	
		} 
		else if ((inputValue2.type() == 0) && (inputValue2.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1));
		}
		else if ((inputValue3.type() == 0) && (inputValue3.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2));
		}
		else if ((inputValue4.type() == 0) && (inputValue4.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3));
		}
		else if ((inputValue5.type() == 0) && (inputValue5.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3), Q_ARG(QVariant, inputValue4));
		}
		else if ((inputValue6.type() == 0) && (inputValue6.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3), Q_ARG(QVariant, inputValue4), Q_ARG(QVariant, inputValue5));
		}
		else if ((inputValue7.type() == 0) && (inputValue7.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3), Q_ARG(QVariant, inputValue4), Q_ARG(QVariant, inputValue5), Q_ARG(QVariant, inputValue6));
		}
		else if ((inputValue8.type() == 0) && (inputValue8.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3), Q_ARG(QVariant, inputValue4), Q_ARG(QVariant, inputValue5), Q_ARG(QVariant, inputValue6), Q_ARG(QVariant, inputValue7));
		}
		else if ((inputValue9.type() == 0) && (inputValue9.value<int>() == NULL))
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3), Q_ARG(QVariant, inputValue4), Q_ARG(QVariant, inputValue5), Q_ARG(QVariant, inputValue6), Q_ARG(QVariant, inputValue7), Q_ARG(QVariant, inputValue8));
		}
		else
		{
			bInvokeSucceeded = QMetaObject::invokeMethod(methodsContainerObject, strMethodName.toLatin1(), Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, inputValue1), Q_ARG(QVariant, inputValue2), Q_ARG(QVariant, inputValue3), Q_ARG(QVariant, inputValue4), Q_ARG(QVariant, inputValue5), Q_ARG(QVariant, inputValue6), Q_ARG(QVariant, inputValue7), Q_ARG(QVariant, inputValue8), Q_ARG(QVariant, inputValue9));
		}

		if(bInvokeSucceeded)
		{
			//QString ss = returnedValue.type();
			//if (returnedValue.type() == QMetaType::QVariant)
			//{
			//}
			//QString sRetVal = returnedValue.toString();
			//qDebug() << "QML method returned:" << sRetVal;
			return returnedValue;// QScriptValue();//sRetVal);
		}
	}
	qDebug() << __FUNCTION__ "::Failed to invoke the QML method(" << strMethodName << ")";
	return NULL;
}

//void QML2Viewer::onQuickViewClosed()
//{

//}

void QML2Viewer::onStatusChanged(QQuickView::Status status)
{
	if (status == QQuickView::Error)
	{
		QList<QQmlError> errList;
		errList = quick2Viewer->errors();
		for (int i=0;i<errList.count();i++)
		{
			QString errMessage = "... QML error at(col " + QString::number(errList.at(i).column()) + ", line " + QString::number(errList.at(i).line()) + "): " + errList.at(i).description() + " ...";
			if(experimentManager)
				experimentManager->sendToMainAppLogOutput(errMessage);
			else
				QMetaObject::invokeMethod(MainAppInfo::getMainWindow(), MainAppInfo::getMainWindowLogSlotName().toLatin1(), Qt::DirectConnection, Q_ARG(QString, errMessage));
		}
	}
}

void QML2Viewer::qml2EventRoutine(bool dShowWidget, QString strContent)
{
	//quick2Viewer->setVisible(false);
	//if (bResolutionChanged)
	//{
	//	bResolutionChanged = false;
		//quick2Viewer->setVisible(false);
	//	quick2Viewer->resize((int)stimWidthPixelAmount,(int)stimHeigthPixelAmount);//rectScreenRes.width(),rectScreenRes.height());		
	//}
	
	QFileInfo fi(qmlMainFilePath);
	QUrl fileUrl = QUrl::fromLocalFile(fi.canonicalFilePath());
	//QString test = fi.canonicalFilePath();
	//QString test2 = quick2Viewer->source().toString();
	if(strContent.isEmpty() == false)
	{
		int nRetries = 1;
		QString filename("tmp");
		tmpFile.setFileName(filename + QString::number(nRetries) + ".txt");
		while (!tmpFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			nRetries++;
			tmpFile.setFileName(filename + QString::number(nRetries) + ".txt");
			if (nRetries == 10)
			{
				QString errMessage = __FUNCTION__ + QString(": Could not create temporarily file.");
				QMetaObject::invokeMethod(MainAppInfo::getMainWindow(), MainAppInfo::getMainWindowLogSlotName().toLatin1(), Qt::DirectConnection, Q_ARG(QString, errMessage));
				this->abortExperimentObject();//this seems to work...
				return;
			}
		}
		tmpFile.write(strContent.toLatin1());
		tmpFile.close();
		fi.setFile(tmpFile.fileName());
		qmlMainFilePath = fi.canonicalFilePath();
		fileUrl = QUrl::fromLocalFile(fi.canonicalFilePath());
	}
	if (quick2Viewer->source() != fileUrl)
	{
		//QString test = quick2Viewer->source().toString();
		//test = fileUrl.toString();

		//quick2Viewer->setVisible(false);
		quick2Viewer->setMainQmlFile(qmlMainFilePath);//fi.canonicalFilePath());//fileUrl.toString());//setSource(fileUrl);
		
		if (quick2Viewer->status() == QQuickView::Error)
		{
			this->stopObject();
			if(experimentManager)
			{
				experimentManager->abortExperiment();
				return;
			}
			this->abortExperimentObject();//this seems to work...
			return;
		}
		//rootObject = dynamic_cast<QObject *>(quick2Viewer->rootObject());// get root object
		//quick2Viewer->resize((int)stimWidthPixelAmount,(int)stimHeigthPixelAmount);//rectScreenRes.width(),rectScreenRes.height());
	}	
	
	//if (dShowWidget)
	//{
	quick2Viewer->show();
		//quick2Container->show();
		//quick2Viewer->showExpanded();//->showFullScreen();//Fastest uncomment this
		//glWidget->setFocus();
	//}
}

bool QML2Viewer::setExperimentManager(ExperimentManager *expManager)
{
	if(experimentManager!=expManager)
	{
		experimentManager = expManager;
		ExperimentEngine::setExperimentManager(expManager);//Important!
	}
	return true;
}

//QString QML2Viewer::addPixmapToImageBuffer(const QPixmap &pixmap)
//{
///*! \brief Adds a QPixmap to the internal Image Buffer.
// *
// *  This function can add a QPixmap to the internal Image Buffer.
// *  These images can then be rapidly presented because they are already loaded in memory.
// * @param pixmap the QPixmap to be added to the internal Image Buffer.
// * @return a Unique string created by the Image Buffer holding a key to access the newly added image.
// * See QML2Viewer::getPixmapFromImageBuffer and QML2Viewer::updatePixmapFromImageBuffer.
// */
//	if (!imgLstModel)
//		return false;
//	return imgLstModel->addPixmap(pixmap);
//}

//bool QML2Viewer::getPixmapFromImageBuffer(QPixmap *pixmap, const QString &ID)
//{
///*! \brief Retrieves a QPixmap to the internal Image Buffer.
// *
// *  This function can retrieve a QPixmap to the internal Image Buffer.
// * @param pixmap the retrieved QPixmap from the internal Image Buffer.
// * @return a boolean value representing whether the function executed successfully.
// * See QML2Viewer::addPixmapToImageBuffer.
// */
//	if (!imgLstModel)
//		return false;
//	if(imgLstModel->getPixmap(*pixmap,ID))
//	{
//		return true;
//	}
//	return false;
//}

//bool QML2Viewer::updatePixmapFromImageBuffer(QPixmap *pixmap, const QString &ID)
//{
///*! \brief Updates a QPixmap inside the internal Image Buffer.
// *
// *  This function can update a QPixmap that is stored in the internal Image Buffer.
// * @param pixmap the new QPixmap to which the stored QPixmap inside the Image Buffer should update to.
// * @param ID the String containing the unique String previously created by the Image Buffer.
// * @return a boolean value representing whether the function executed successfully.
// * See QML2Viewer::addPixmapToImageBuffer.
// */
//	if (!imgLstModel)
//		return false;
//	if(imgLstModel->updatePixmap(*pixmap,ID))
//	{
//		return true;
//	}
//	return false;
//}

//bool QML2Viewer::removePixmapFromImageBuffer(const QString &ID)
//{
///*! \brief Removes a QPixmap inside the internal Image Buffer.
// *
// *  This function can remove a QPixmap that is stored in the internal Image Buffer.
// * @param ID the String containing the unique String previously created by the Image Buffer.
// * @return a boolean value representing whether the function executed successfully.
// * See QML2Viewer::addPixmapToImageBuffer.
// */
//	if (!imgLstModel)
//		return false;
//	if(imgLstModel->removePixmap(ID))
//	{
//		return true;
//	}
//	return false;
//}