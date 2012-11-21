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

#include "qmlWidget.h"
#include <QDomNodeList>
#include "ExperimentManager.h"
#include "ModelIndexProvider.h"
#include <QMessageBox>
#include <QMetaObject>

qmlWidget::qmlWidget(QWidget *parent) : GLWidgetWrapper(parent), parentWidget(parent)
{
	initialize();
}

qmlWidget::~qmlWidget()
{
	//qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,20);//To make sure that the qmlEventRoutine(bool dShowWidget) function is finished in case of an qml error!
	disconnect(qmlViewer, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(onStatusChanged(QDeclarativeView::Status)));

	if (imgLstModel)
	{
		delete imgLstModel;
		imgLstModel = NULL;
	}	
	if (glWidget)
	{
		delete glWidget;
		glWidget = NULL;
	}
	if (rootObject)
	{
		//delete rootObject; only a pointer..
		rootObject = NULL;
	}
	if (stimuliPainter)
	{
		delete stimuliPainter;
		stimuliPainter = NULL;
	}
	if (qmlViewer)
	{
		delete qmlViewer;
		qmlViewer = NULL;
	}
	if (qmlErrorHandler)
	{
		delete qmlErrorHandler;
		qmlErrorHandler = NULL;
	}
	if(tmpFile.exists())
		if(tmpFile.remove() == false)
			qWarning() << __FUNCTION__ << "Could not remove a temporarily file(" << tmpFile.fileName() << ")"; 
	parentWidget = NULL;
}

bool qmlWidget::executeQMLDocument(const QString &strSource, QDialog *ContainerDlg, bool bIsFile)//QVBoxLayout *layout) 
{
	if(bIsFile)
		qmlMainFilePath = strSource;
	else
		qmlMainFilePath = "";
	if(initObject())
	{
		if (ContainerDlg)
		{
			setAlternativeContainerDialog(ContainerDlg);
			ContainerDlg->installEventFilter(this);//re-route all ContainerDlg events to this->bool GLWidgetWrapper::eventFilter(QObject *target, QEvent *event)
		}
		if(bIsFile)
		{
			return startObject();
		}
		else
		{
			qmlEventRoutine(false,strSource);
			return true;
		}
	}
	return false;
}

bool qmlWidget::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
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

QScriptValue qmlWidget::ctor_QmlWidget(QScriptContext* context, QScriptEngine* engine)
{
	return engine->newQObject(new qmlWidget(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

void qmlWidget::initialize()
{
	//Place your custom initialization code here:
	
	//Default values if none defined.
	expSnapshot.currExpBlock = 0;
	expSnapshot.currExpBlockTrialFrame = 0;
	expSnapshot.currExpBlockTrialTrigger = 0;
	expSnapshot.currExpBlockTrialInternalTriggerAmount = 0;
	expSnapshot.currExpTrial = 0;
	expSnapshot.currExpInternalTrigger = 0;
	expSnapshot.currExpExternalTrigger = 0;
	expSnapshot.elapsedTrialTime = 0;

	nQMLWidgetID = -1;
	rectScreenRes = getScreenResolution();
	lastTriggerNumber = -1;
	stimuliPainter = NULL;
	currExpConfStruct = NULL;
	qmlViewer = NULL;
	glWidget = NULL;
	imgLstModel = NULL;
	rootObject = NULL;
	qmlErrorHandler = NULL;
}

void qmlWidget::parseExperimentObjectBlockParameters(bool bInit, bool bSetOnlyToDefault)
{
	if (bInit)
	{	
		colorBackground = QColor(87,87,87);//gives "#575757";
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQMLWidgetID,GLWIDGET_BACKGROUNDCOLOR,colorBackground);
		QString qmlMainFilePath = "";
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQMLWidgetID,QML_WIDGET_MAINFILEPATH,qmlMainFilePath);
		stimHeigthPixelAmount = rectScreenRes.height();
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQMLWidgetID,GLWIDGET_HEIGHT_PIXEL_AMOUNT,stimHeigthPixelAmount);
		//stimWidthPixelAmount = stimHeigthPixelAmount; removed, not needed here!
		stimWidthPixelAmount = rectScreenRes.width();
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQMLWidgetID,GLWIDGET_WIDTH_PIXEL_AMOUNT,stimWidthPixelAmount);
		nWidgetMaxEvenTime = 5;
		if(!bSetOnlyToDefault)
			insertExpObjectParameter(nQMLWidgetID,QML_WIDGET_MAX_EVENT_TIME,nWidgetMaxEvenTime);		
	} 
	else
	{
		ParsedParameterDefinition pParDef;//((pParDef.bIsInitialized) && (pParDef.bHasChanged == false))
		bResolutionChanged = false;
		pParDef = getExpObjectBlockParameter(nQMLWidgetID,GLWIDGET_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount));
		if (pParDef.bHasChanged)
		{
			stimHeigthPixelAmount = pParDef.sValue.toInt();
			bResolutionChanged = true;
		}
		pParDef = getExpObjectBlockParameter(nQMLWidgetID,GLWIDGET_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount));
		if (pParDef.bHasChanged)
		{
			stimWidthPixelAmount = pParDef.sValue.toInt();
			bResolutionChanged = true;
		}
		if (bResolutionChanged)
			setStimuliResolution(stimWidthPixelAmount,stimHeigthPixelAmount);

		pParDef = getExpObjectBlockParameter(nQMLWidgetID,GLWIDGET_BACKGROUNDCOLOR,colorBackground.name());
		if (pParDef.bHasChanged)
		{
			colorBackground = QColor(pParDef.sValue.toLower());
			GlWidgetPallette = glWidget->palette();
			GlWidgetPallette.setColor(glWidget->backgroundRole(), colorBackground);
			glWidget->setPalette(GlWidgetPallette);
		}
		pParDef = getExpObjectBlockParameter(nQMLWidgetID,QML_WIDGET_MAINFILEPATH,qmlMainFilePath);
		if (pParDef.bHasChanged)
		{
			qmlMainFilePath = pParDef.sValue;
		}
		pParDef = getExpObjectBlockParameter(nQMLWidgetID,QML_WIDGET_MAX_EVENT_TIME,QString::number(nWidgetMaxEvenTime));
		if (pParDef.bHasChanged)
		{
			nWidgetMaxEvenTime = pParDef.sValue.toInt();
			tWidgetUpdateLoopTimer.setInterval(nWidgetMaxEvenTime);
		}
	}
}

bool qmlWidget::initObject()
{
	if (parentWidget == NULL)
	{
		GLWidgetWrapper::setupLayout(this);
	} 
	//if (currExpConfStruct)
	//{
	//	if (currExpConfStruct->pExperimentManager)
	//	{
	//		currExpConfStruct->pExperimentManager->logExperimentObjectData(nQMLWidgetID,0,__FUNCTION__,"","swapInterval() = ", QString::number(this->format().swapInterval()));
	//	}
	//}
	qmlViewer = new QDeclarativeView(this);
	qmlViewer->setAttribute(Qt::WA_OpaquePaintEvent);// Qt::WA_OpaquePaintEvent basically implies that you'll re-paint everything as necessary yourself (which QML is well behaved with).
	qmlViewer->setAttribute(Qt::WA_NoSystemBackground);//Qt::WA_NoSystemBackground tells Qt to nicely not paint the background.)
	qmlViewer->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	qmlViewer->viewport()->setAttribute(Qt::WA_NoSystemBackground);
	qmlViewer->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	qmlViewer->setResizeMode(QDeclarativeView::SizeViewToRootObject);//SizeViewToRootObject);//SizeRootObjectToView );

	QGLFormat format = QGLFormat::defaultFormat();
	format.setSampleBuffers(false);

	////Workaround to have TextInput correctly rendered in Win7 is to explicitly define QGLWidget with double buffer and direct rendering.
	//QGLFormat format = QGLFormat(QGL::DirectRendering | QGL::DoubleBuffer);
	//format.setSampleBuffers(true);
	//QGLWidget *glWidget = new QGLWidget(format);
	//bofol a = format.doubleBuffer();//true
	//bofol b = format.directRendering();//true
	//bofol c = format.sampleBuffers();//false
	//format.setDoubleBuffer(true);
	//format.setDirectRendering(true);
	//format.setSampleBuffers(true);
	//a = format.doubleBuffer();
	//b = format.directRendering();
	//c = format.sampleBuffers();
	//qDebug() << a << b << c;

	glWidget = new QGLWidget(format,this,this);
	glWidget->setAutoFillBackground(true);	//here we must use this functionality
	//if (parentWidget == NULL)
		qmlViewer->setViewport(glWidget);//uncomment this

	connect(qmlViewer, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(onStatusChanged(QDeclarativeView::Status)));
		
	//QPainter::RenderHints from = qmlViewer->renderHints();
	//qmlViewer->setRenderHints(QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform | QPainter::HighQualityAntialiasing);
	//QPainter::RenderHints to = qmlViewer->renderHints();
	////Antialiasing = 0x01,
	////TextAntialiasing = 0x02,
	////SmoothPixmapTransform = 0x04,
	////HighQualityAntialiasing = 0x08,
	////NonCosmeticDefaultPen = 0x10
	////QMessageBox::warning(NULL, QString("renderHints()"), QString("From: ") + QString::number((int)from) + QString(", To: ") + QString::number((int)to));
	//qDebug() << QString("From: ") + QString::number((int)from) + QString(", To: ") + QString::number((int)to);
		
	if (!imgLstModel)
	{
		imgLstModel = new ImageListModel();
		qmlViewer->engine()->addImageProvider(DEFAULT_IMAGEBUFFER_NAME, new ModelIndexProvider(*imgLstModel));//Qt::DisplayRole
	}
	
	if (qmlViewer)
	{
		QString extPluginPath = MainAppInfo::qmlExtensionsPluginDirPath();
		QStringList importPaths = qmlViewer->engine()->importPathList();
		if (!importPaths.contains(extPluginPath))
		{
			qmlViewer->engine()->addImportPath(extPluginPath);
			qDebug() << __FUNCTION__ "::Added the QML extension plugin path (" << extPluginPath << ").";
		}
		qmlErrorHandler = new QmlErrorHandler(*qmlViewer,this);
	}	
	if (parentWidget == NULL)
	{
		parseExperimentObjectBlockParameters(true,false);
	}
	else
	{
		parseExperimentObjectBlockParameters(true,true);
		setExperimentMetaObject();
	}
	lastTriggerNumber = -1;
	return true;
}

//void qmlWidget::processQMLEngineWarning(const QList<QDeclarativeError> & warnings)
//{
//
//}

QString qmlWidget::addPixmapToImageBuffer(const QPixmap &pixmap)
{
	if (!imgLstModel)
		return false;
	return imgLstModel->addPixmap(pixmap);
}

bool qmlWidget::getPixmapFromImageBuffer(QPixmap *pixmap, const QString &ID)
{
	if (!imgLstModel)
		return false;
	if(imgLstModel->getPixmap(*pixmap,ID))
	{
		return true;
	}
	return false;
}

bool qmlWidget::updatePixmapFromImageBuffer(QPixmap *pixmap, const QString &ID)
{
	if (!imgLstModel)
		return false;
	if(imgLstModel->updatePixmap(*pixmap,ID))
	{
		return true;
	}
	return false;
}

bool qmlWidget::removePixmapFromImageBuffer(const QString &ID)
{
	if (!imgLstModel)
		return false;
	if(imgLstModel->removePixmap(ID))
	{
		return true;
	}
	return false;
}

bool qmlWidget::startObject()
{	
	qmlEventRoutine(false);
	return true;
}

bool qmlWidget::stopObject()
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
	disconnect(&tWidgetUpdateLoopTimer, SIGNAL(timeout()), this, SLOT(callAnimate()));
	tWidgetUpdateLoopTimer.stop();
	return true;
}

bool qmlWidget::setObjectConfiguration(QObject *pExpConfStruct)//ExperimentConfiguration *pExpConfStruct)
{
	currExpConfStruct = reinterpret_cast<ExperimentConfiguration *>(pExpConfStruct);
	//currExpConfStruct->pExperimentManager->getExperimentObjectBlockParameter()
	return true;
}

bool qmlWidget::setObjectID(int nObjID)
{
	nQMLWidgetID = nObjID;//or use getObjectID();//Because we need it often we'll buffer it here
	return true;
}

bool qmlWidget::initObjectBlockTrial()
{
	parseExperimentObjectBlockParameters(false);
	//Some variable initializations
	//firstBlockTrialPaintFrame = true;	
	return true;
}

QVariant qmlWidget::invokeQmlMethod(QString strRootObjectName, QString strMethodName, QVariant inputValue1, QVariant inputValue2, QVariant inputValue3, QVariant inputValue4, QVariant inputValue5, QVariant inputValue6, QVariant inputValue7, QVariant inputValue8, QVariant inputValue9)
{
	if (!rootObject)
	{
		rootObject = dynamic_cast<QObject *>(qmlViewer->rootObject());// get root object
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

void qmlWidget::callAnimate()
{  
	animate(true);
	qmlEventRoutine(false);
}

bool qmlWidget::paintObject(int paintFlags, QObject *paintEventObject)//Only gets called once during whole experiment!
{
	//if (!firstBlockTrialPaintFrame)
	//	return true;
	if(isDebugMode())
		currExpConfStruct->pExperimentManager->logExperimentObjectData(nQMLWidgetID,0,__FUNCTION__,"","Starting to paint the object for the first time");
	QPaintEvent *event = reinterpret_cast<QPaintEvent *>(paintEventObject);//qobject_cast<QPaintEvent *>(paintEventObject);
	GLWidgetPaintFlags currentPaintFlags = (GLWidgetPaintFlags)paintFlags;
	//getCurrentExperimentProgressSnapshot(&expSnapshot);
	//int elapsedTrialTime = (int)expSnapshot.elapsedTrialTime;
	if (stimuliPainter == NULL)
	{
		stimuliPainter = new QPainter(this);//Constructor automatically calls begin()
	}
	else
	{
		stimuliPainter->begin(this);
	}
	//if (expSnapshot.currExpBlockTrialFrame == 0)
	//{
		//if(isDebugMode())
		//{
		stimuliPainter->fillRect(event->rect(), colorBackground);//brushBackground);
		qmlEventRoutine();
		connect(&tWidgetUpdateLoopTimer, SIGNAL(timeout()), SLOT(callAnimate()));
		tWidgetUpdateLoopTimer.setInterval(nWidgetMaxEvenTime);
		tWidgetUpdateLoopTimer.start();
	//}
	//firstBlockTrialPaintFrame = false;
	stimuliPainter->end();
	return true;
}

void qmlWidget::onStatusChanged(QDeclarativeView::Status status)
{
	if (status == QDeclarativeView::Error)
	{
		QList<QDeclarativeError> errList;
		errList = qmlViewer->errors();
		for (int i=0;i<errList.count();i++)
		{
			QString errMessage = "... QML error at(col " + QString::number(errList.at(i).column()) + ", line " + QString::number(errList.at(i).line()) + "): " + errList.at(i).description() + " ...";
			if(currExpConfStruct)
			{
				if(currExpConfStruct->pExperimentManager)
					currExpConfStruct->pExperimentManager->sendToMainAppLogOutput(errMessage);
				else
					QMetaObject::invokeMethod(MainAppInfo::getMainWindow(), MainAppInfo::getMainWindowLogSlotName().toLatin1(), Qt::DirectConnection, Q_ARG(QString, errMessage));
			}
			else
			{
				QMetaObject::invokeMethod(MainAppInfo::getMainWindow(), MainAppInfo::getMainWindowLogSlotName().toLatin1(), Qt::DirectConnection, Q_ARG(QString, errMessage));
				break;
			}
		}
	}
}

void qmlWidget::qmlEventRoutine(bool dShowWidget, QString strContent)
{
	//qmlViewer->setVisible(false);
	if (bResolutionChanged)
	{
		bResolutionChanged = false;
		qmlViewer->setVisible(false);
		qmlViewer->resize((int)stimWidthPixelAmount,(int)stimHeigthPixelAmount);//rectScreenRes.width(),rectScreenRes.height());		
	}
	
	QFileInfo fi(qmlMainFilePath);
	QUrl fileUrl = QUrl::fromLocalFile(fi.canonicalFilePath());
	//QString test = fi.canonicalFilePath();
	//QString test2 = qmlViewer->source().toString();
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
		tmpFile.write(strContent.toAscii());
		tmpFile.close();
		fi.setFile(tmpFile.fileName());
		qmlMainFilePath = fi.canonicalFilePath();
		fileUrl = QUrl::fromLocalFile(fi.canonicalFilePath());
	}
	if (qmlViewer->source() != fileUrl)
	{
		qmlViewer->setVisible(false);
		qmlViewer->setSource(fileUrl);
		
		if (qmlViewer->status() == QDeclarativeView::Error)
		{
			this->stopObject();
			if(currExpConfStruct)
			{
				if(currExpConfStruct->pExperimentManager)
				{
					currExpConfStruct->pExperimentManager->abortExperiment();
					return;
				}
			}
			//bool bInvokeSucceeded = QMetaObject::invokeMethod(currExpConfStruct->pExperimentManager, "abortExperiment",Qt::QueuedConnection);	
			//currExpConfStruct->pExperimentManager->abortExperiment();

			this->abortExperimentObject();//this seems to work...
			return;
		}
		rootObject = dynamic_cast<QObject *>(qmlViewer->rootObject());// get root object
		qmlViewer->resize((int)stimWidthPixelAmount,(int)stimHeigthPixelAmount);//rectScreenRes.width(),rectScreenRes.height());
	}	
	
	if (dShowWidget)
	{
		qmlViewer->showFullScreen();//Fastest uncomment this
		glWidget->setFocus();
	}
}

