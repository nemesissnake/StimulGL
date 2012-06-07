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

qmlWidget::qmlWidget(QWidget *parent) : GLWidgetWrapper(parent)
{
	currentScriptEngine = NULL;
	initialize();
	GLWidgetWrapper::setupLayout(this);
}

qmlWidget::~qmlWidget()
{
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
}

bool qmlWidget::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
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
	rootObject = NULL;
}

void qmlWidget::parseExperimentObjectBlockParameters(bool bInit)
{	
	int tmpInteger = -1;
	if (bInit)
	{	
		QString tmpString = "";

		tmpString = QColor(87,87,87).name();//gives "#575757";
		colorBackground = QColor(tmpString);
		insertExperimentObjectBlockParameter(nQMLWidgetID,GLWIDGET_BACKGROUNDCOLOR,tmpString);
		tmpString = "";
		QString qmlMainFilePath = tmpString;
		insertExperimentObjectBlockParameter(nQMLWidgetID,QML_WIDGET_MAINFILEPATH,tmpString);
		stimHeigthPixelAmount = rectScreenRes.height();
		insertExperimentObjectBlockParameter(nQMLWidgetID,GLWIDGET_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount));
		stimWidthPixelAmount = stimHeigthPixelAmount;
		insertExperimentObjectBlockParameter(nQMLWidgetID,GLWIDGET_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount));
		nWidgetMaxEvenTime = 5;
		insertExperimentObjectBlockParameter(nQMLWidgetID,QML_WIDGET_MAX_EVENT_TIME,QString::number(nWidgetMaxEvenTime));		
	} 
	else
	{
		ParsedParameterDefinition pParDef;//((pParDef.bIsInitialized) && (pParDef.bHasChanged == false))
		bResolutionChanged = false;
		pParDef = getExperimentObjectBlockParameter(nQMLWidgetID,GLWIDGET_HEIGHT_PIXEL_AMOUNT,QString::number(stimHeigthPixelAmount));
		if (pParDef.bHasChanged)
		{
			stimHeigthPixelAmount = pParDef.sValue.toInt();
			bResolutionChanged = true;
		}
		pParDef = getExperimentObjectBlockParameter(nQMLWidgetID,GLWIDGET_WIDTH_PIXEL_AMOUNT,QString::number(stimWidthPixelAmount));
		if (pParDef.bHasChanged)
		{
			stimWidthPixelAmount = pParDef.sValue.toInt();
			bResolutionChanged = true;
		}
		if (bResolutionChanged)
			setStimuliResolution(stimWidthPixelAmount,stimHeigthPixelAmount);

		pParDef = getExperimentObjectBlockParameter(nQMLWidgetID,GLWIDGET_BACKGROUNDCOLOR,colorBackground.name());
		if (pParDef.bHasChanged)
		{
			colorBackground = QColor(pParDef.sValue);
			GlWidgetPallette = glWidget->palette();
			GlWidgetPallette.setColor(glWidget->backgroundRole(), colorBackground);
			glWidget->setPalette(GlWidgetPallette);
		}
		pParDef = getExperimentObjectBlockParameter(nQMLWidgetID,QML_WIDGET_MAINFILEPATH,qmlMainFilePath);
		if (pParDef.bHasChanged)
		{
			qmlMainFilePath = pParDef.sValue;
		}
		pParDef = getExperimentObjectBlockParameter(nQMLWidgetID,QML_WIDGET_MAX_EVENT_TIME,QString::number(nWidgetMaxEvenTime));
		if (pParDef.bHasChanged)
		{
			nWidgetMaxEvenTime = pParDef.sValue.toInt();
			tWidgetUpdateLoopTimer.setInterval(nWidgetMaxEvenTime);
		}
	}
}

bool qmlWidget::initObject()
{
	//if (currExpConfStruct)
	//{
	//	if (currExpConfStruct->pExperimentManager)
	//	{
	//		currExpConfStruct->pExperimentManager->logExperimentObjectData(nQMLWidgetID,0,__FUNCTION__,"","swapInterval() = ", QString::number(this->format().swapInterval()));
	//	}
	//}
	qmlViewer = new QDeclarativeView(this);		
	qmlViewer->setAttribute(Qt::WA_OpaquePaintEvent);// Qt::WA_OpaquePaintEvent basically implies that you'll repaint everything as necessary yourself (which QML is well behaved with).
	qmlViewer->setAttribute(Qt::WA_NoSystemBackground);//Qt::WA_NoSystemBackground tells Qt to nicely not paint the background.)
	qmlViewer->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	qmlViewer->viewport()->setAttribute(Qt::WA_NoSystemBackground);
	qmlViewer->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	qmlViewer->setResizeMode(QDeclarativeView::SizeViewToRootObject);//SizeViewToRootObject);//SizeRootObjectToView );
	QGLFormat format = QGLFormat::defaultFormat();
	format.setSampleBuffers(false);
	glWidget = new QGLWidget(format,this,this);
	glWidget->setAutoFillBackground(true);	//here we must use this functionality	
	qmlViewer->setViewport(glWidget);	
	
	parseExperimentObjectBlockParameters(true);
	lastTriggerNumber = -1;
	return true;
}

bool qmlWidget::startObject()
{	
	qmlEventRoutine(false);
	return true;
}

bool qmlWidget::stopObject()
{
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
	qmlEventRoutine();
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

void qmlWidget::qmlEventRoutine(bool dShowWidget)
{
	if (bResolutionChanged)
	{
		qmlViewer->setVisible(false);
		qmlViewer->resize((int)stimWidthPixelAmount,(int)stimHeigthPixelAmount);//rectScreenRes.width(),rectScreenRes.height());		
	}
	if (qmlViewer->source() != QUrl::fromLocalFile(qmlMainFilePath))
	{
		qmlViewer->setVisible(false);
		qmlViewer->setSource(QUrl::fromLocalFile(qmlMainFilePath));//QUrl::fromLocalFile("MyItem.qml"));
		rootObject = dynamic_cast<QObject *>(qmlViewer->rootObject());// get root object
		qmlViewer->resize((int)stimWidthPixelAmount,(int)stimHeigthPixelAmount);//rectScreenRes.width(),rectScreenRes.height());
	}	
	if (dShowWidget)
		qmlViewer->showFullScreen();//Fastest
}

