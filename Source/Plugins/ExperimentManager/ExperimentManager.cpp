#include "ExperimentManager.h"
#include <QFileDialog>
//#include "..\ParallelPortDevice\parallelport.h"

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
	m_RunFullScreen = true;
	m_ExpFileName = "";
	m_DebugMode = false;
	currentExperimentTree = NULL;
	RegisterMetaTypes();
}

ExperimentManager::~ExperimentManager()
{
	cleanupExperiment();
}

void ExperimentManager::RegisterMetaTypes()
{
	qRegisterMetaType<RetinoMap_glwidget>("RetinoMap_glwidget");
}

//void ExperimentManager::Init()
//{
	//StimGLGraphScene->Init();
//}

//void ExperimentManager::Show()
//{
	//GraphView = new QGraphicsView(StimGLGraphScene);
	//GraphView->setWindowModality(Qt::WindowModal);
	//connect(StimGLGraphScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
	//GraphView->showFullScreen();
//}

//void ExperimentManager::setExampleProperty( short sExampleProperty )
//{
//	m_ExampleProperty = sExampleProperty;
//	emit ExampleSignalTriggered(m_ExampleProperty);//Signal that the value has been changed
//}

//short ExperimentManager::getExampleProperty() const
//{
//	return m_ExampleProperty;
//}

//bool ExperimentManager::eventFilter(QObject *target, QEvent *event)
//{
//	if (target == stimContainerDlg) 
//	{
//		if (event->type() == QEvent::KeyPress) 
//		{
//			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//			switch (keyEvent->key())
//			{
//			case Qt::Key_Escape:
//				if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->modifiers() & Qt::AltModifier))
//				{
//					emit UserWantsToClose();
//					abortExperiment();
//				}
//				break;
//			}
//
//			//if(keyEvent->text() == ui.USBCharEdit->text()) // for fMRI triggering via USB port (simulated keyboard)
//			//{
//			//	updateTriggerCount();
//			//	return true;
//			//}
//		}
//	}
//	//return QDialog::eventFilter(target, event);
//	return true;
//}

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

bool ExperimentManager::runExperiment()
{
	if (!currentExperimentTree)
	{
		qDebug() << "runExperiment::No Experiment loaded!";
		return false;
	}
	//m_ForceExperimentToStop = false;
#ifdef Q_OS_WIN
	bool ret = SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#else
	QThread::currentThread()->setPriority(QThread::HighPriority);  
	// QThread::TimeCriticalPriority);
#endif

	if (!createExperimentObjects())
	{
		return false;
	}

	//stimContainerDlg = new ContainerDlg();//parent parameter?
	//stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	////retinoMapWdg = new RetinoMap_glwidget(stimContainerDlg);
	////retinoMapWdg->setObjectName("RetinoMap_RenderWidgetGL");
	//stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	//if(!retinoMapWdg->format().doubleBuffer())// check whether we have double buffering, otherwise cancel
	//{
	//	qDebug() << "No Double Buffering available!";
	//	stimContainerDlg->deleteLater();//Schedules this object for deletion, the object will be deleted when control returns to the event loop
	//	return false;
	//}
	//int swap_interval = retinoMapWdg->format().swapInterval(); // support for vertical retrace sync?
	//QVBoxLayout *mainLayout = new QVBoxLayout;
	//mainLayout->setAlignment(Qt::AlignCenter);
	//mainLayout->setMargin(0);
	//mainLayout->addWidget(retinoMapWdg);
	//stimContainerDlg->setLayout(mainLayout);
	//retinoMapWdg->setFixedSize(1000,1000);
	//retinoMapWdg->setDebugMode(true);
	//retinoMapWdg->startAnimationTimer(50);
	//if(m_RunFullScreen)
	//	stimContainerDlg->showFullScreen();
	//else
	//	stimContainerDlg->show();
	//QDesktopWidget *dt = QApplication::desktop();
	//QCursor::setPos(dt->width(), dt->height()/2); // not at bottom because then mouse movement on Mac would show dock		
	//return retinoMapWdg->start();
	if(!startExperimentObjects(m_RunFullScreen))
	{
		return false;
	}
}

void ExperimentManager::abortExperiment()
{
	if(!stopExperimentObjects())
		qDebug() << "ExperimentManager::abortExperiment Could not stop the Experiment Objects";
	cleanupExperiment();
	QThread::currentThread()->setPriority(QThread::NormalPriority);
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
			QMetaType::destroy(lExperimentObjectList[i].nMetaID, lExperimentObjectList[i].pObject);
			lExperimentObjectList[i].pObject = NULL;
			lExperimentObjectList[i].nMetaID = -1;
			lExperimentObjectList[i].nObjectID = -1;
			lExperimentObjectList[i].sObjectName = "";
		}
		lExperimentObjectList.clear();
	}
}

bool ExperimentManager::changeExperimentObjectsSignalSlots(bool bDisconnect)
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
			if(bDisconnect)
			{
				disconnect(lExperimentObjectList[i].pObject, SIGNAL(UserWantsToClose(void)), this, SLOT(abortExperiment(void)));
			}
			else
			{
				connect(lExperimentObjectList[i].pObject, SIGNAL(UserWantsToClose(void)), this, SLOT(abortExperiment(void)));
				connect(lExperimentObjectList[i].pObject, SIGNAL(StateHasChanged(ExperimentObjectState)), this, SLOT(changeExperimentObjectState(ExperimentObjectState)));
				//connect(lExperimentObjectList[i].pObject, SIGNAL(UserWantsToClose(void)), this, SLOT(abortExperiment(void)));
			}
		}
	}
	return true;
}

void ExperimentManager::changeExperimentObjectState(ExperimentObjectState nState)
{
	if (!currentExperimentTree)
	{
		qDebug() << "invokeExperimentObjectSlot::No Experiment loaded!";
	}
	int nCount = lExperimentObjectList.count();
	if (nCount>0)
	{
		for (int i=0;i<nCount;i++)
		{
			if (QObject::sender() == lExperimentObjectList.at(i).pObject)
			{
				lExperimentObjectList[i].nState = nState;
				return;
			}
			
			//////Query the properties(only Q_PROPERTY)
			////QStringList properties;
			////QString nolist;
			////for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
			////{
			////	properties << QString::fromLatin1(metaObject->property(i).name());
			////	nolist = QString::fromLatin1(metaObject->property(i).name());
			////}
			//////Query the methods(only public slots!)
			////QStringList methods;
			////for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
			////	methods << QString::fromLatin1(metaObject->method(i).signature());

			//if (!(metaObject->indexOfMethod(sSlotFullName.toLatin1()) == -1))//Is the slot present?
			//{
			//	//Invoke the start() slot
			//	if(!metaObject->invokeMethod(lExperimentObjectList.at(i).pObject, sSlotName.toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))//, Q_ARG(QString, "sqrt"), Q_ARG(int, 42), Q_ARG(double, 9.7));
			//		//if(!QMetaObject::invokeMethod(lExperimentObjectList[i].pObject, sMethod.toLatin1(), Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))//, Q_ARG(QString, "sqrt"), Q_ARG(int, 42), Q_ARG(double, 9.7));
			//	{
			//		qDebug() << "invokeExperimentObjectsSlots::Could not invoke the slot(" << sSlotFullName << ")!";		
			//		return false;
			//	}
			//}

		}
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
			const QMetaObject* metaObject = lExperimentObjectList.at(i).pObject->metaObject();
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

bool ExperimentManager::stopExperimentObjects()
{
	bool bRetVal = invokeExperimentObjectsSlots(QString("stop"));
	changeExperimentObjectsSignalSlots(true);
	return bRetVal;
}

bool ExperimentManager::startExperimentObjects(bool bRunFullScreen)
{
	changeExperimentObjectsSignalSlots(false);
	bool bRetVal = invokeExperimentObjectsSlots(QString("start"));
	return bRetVal;
}

bool ExperimentManager::createExperimentObjects()
{
	if (!currentExperimentTree)
	{
		qDebug() << "createExperimentObjects::No Experiment loaded!";
		return false;
	}
	cleanupExperimentObjects();
	if (currentExperimentTree->getDocumentElements(OBJECT_TAG,ExperimentObjectDomNodeList))
	{
		int nNrOfObjects = ExperimentObjectDomNodeList.count();
		QDomNode tmpNode;
		QDomElement tmpElement;
		QString tmpString;

		for(int i=0;i<nNrOfObjects;i++)
		{
			tmpNode = ExperimentObjectDomNodeList.at(i);
			if (tmpNode.isElement()) 
			{
				tmpString = ID_TAG;
				tmpElement = tmpNode.firstChildElement(tmpString);
				int nID = tmpElement.text().toInt();
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
					objectElement tmpElement;
					tmpElement.nObjectID = nID;
					tmpElement.nMetaID = metaID;
					tmpElement.sObjectName = sName;
					tmpElement.pObject = static_cast< QObject* > ( QMetaType::construct(metaID) );
					tmpElement.nState = ExperimentObject_Initialized;
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
	QPalette pal = palette();
	pal.setColor(QPalette::Window, QColor(16, 16, 16));
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