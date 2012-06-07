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


#include "ExperimentManager_dialog.h"
#include "mainappinfo.h"

ExperimentManager_Dialog::ExperimentManager_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	//qmlViewer = NULL;
	//rootObject = NULL;
	//myClass = NULL;
	//imageViewerObject = NULL;
	//appData = NULL;
	//mainLayout = NULL;
	//stimContainerDlg = NULL;
	//qmlRegisterType<ImageViewer>("MyLibrary", 1, 0, "ImageViewer");
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
	connect(ui.btnExampleButton_2, SIGNAL(clicked()), this, SLOT(exampleButton_2_Pressed()));
	connect(ui.btnExampleButton_3, SIGNAL(clicked()), this, SLOT(exampleButton_3_Pressed()));
	connect(ui.btnExampleButton_4, SIGNAL(clicked()), this, SLOT(exampleButton_4_Pressed()));
	connect(ui.btnExampleButton_5, SIGNAL(clicked()), this, SLOT(exampleButton_5_Pressed()));
}

ExperimentManager_Dialog::~ExperimentManager_Dialog()
{
	cleanUp();
}

void ExperimentManager_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void ExperimentManager_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void ExperimentManager_Dialog::cleanUp()
{
	//if (rootObject)
	//{
	//	delete rootObject;
	//	rootObject = NULL;
	//}
	//if (myClass)
	//{
	//	delete myClass;
	//	myClass = NULL;
	//}
	//if (imageViewerObject)
	//{
	//	delete imageViewerObject;
	//	imageViewerObject = NULL;
	//}
	//if (appData)
	//{
	//	delete appData;
	//	appData = NULL;
	//}
	//if(qmlViewer)
	//{
	//	delete qmlViewer;
	//	qmlViewer = NULL;
	//}
	//if (mainLayout)
	//{
	//	delete mainLayout;
	//	mainLayout = NULL;
	//}
	//if (stimContainerDlg)
	//{
	//	stimContainerDlg->close();
	//	stimContainerDlg = NULL;
	//}	
	return;
}

void ExperimentManager_Dialog::exampleButton_Pressed()
{
	//QMessageBox msgBox;
	//msgBox.setText("This Gui has not been implemented jet.");
	//msgBox.exec();
	
	//qmlViewer = new QmlApplicationViewer(this);//this->ui.qmlWidgetHolder);//use this to embed
	//myClass = new MyClass();
	//qmlViewer->rootContext()->setContextProperty("myObject", myClass);
	//imageViewerObject = new ImageViewer();
	//qmlViewer->rootContext()->setContextProperty("imageViewer2", imageViewerObject);

	//qmlViewer->rootContext()->setContextProperty("currentDateTime", QDateTime::currentDateTime());

	//appData = new ApplicationData();
	//qmlViewer->rootContext()->setContextProperty("applicationData", appData);

	////qmlMainFilePath = MainAppInfo::appDirPath() + "/qml/Transitions/main.qml";
	//qmlMainFilePath = "E:\\Projects\\StimulGL\\Source\\Plugins\\ExperimentManager\\qml\\Transitions\\main.qml";
	//// Using QDeclarativeView
	//qmlViewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
	//qmlViewer->setSource(QUrl::fromLocalFile(qmlMainFilePath));//QUrl::fromLocalFile("MyItem.qml"));
	////qmlViewer->setMainQmlFile(qmlMainFilePath);//QLatin1String(MainAppInfo::appDirPath() + "/Transitions/main.qml"));
	////qmlViewer->showExpanded();	

	//qmlViewer->setAttribute(Qt::WA_OpaquePaintEvent);// Qt::WA_OpaquePaintEvent basically implies that you'll repaint everything as necessary yourself (which QML is well behaved with).
	//qmlViewer->setAttribute(Qt::WA_NoSystemBackground);//Qt::WA_NoSystemBackground tells Qt to nicely not paint the background.)
	//qmlViewer->viewport()->setAttribute(Qt::WA_OpaquePaintEvent);
	//qmlViewer->viewport()->setAttribute(Qt::WA_NoSystemBackground);
	//qmlViewer->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	//qmlViewer->setResizeMode( QDeclarativeView::SizeRootObjectToView );
	////qmlViewer->showFullScreen();



	//stimContainerDlg = new ContainerDlg(this);
	//stimContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	//stimContainerDlg->setAttribute(Qt::WA_PaintOnScreen);
	//stimContainerDlg->installEventFilter(this);//re-route all stimContainerDlg events to this->bool Retinotopic_Mapping::eventFilter(QObject *target, QEvent *event)
	//mainLayout = new QVBoxLayout(stimContainerDlg);
	//mainLayout->setAlignment(Qt::AlignCenter);
	//mainLayout->setMargin(0);
	//mainLayout->addWidget(qmlViewer,Qt::AlignCenter);
	////stimContainerDlg->setLayout(mainLayout);
	//
	////if(m_RunFullScreen)
	////stimContainerDlg->setWindowModality(Qt::WindowModality::ApplicationModal);

	////stimContainerDlg->show();
	//stimContainerDlg->showFullScreen();

	////move the mouse cursor outside the most right screen
	////QDesktopWidget *dt = QApplication::desktop();
	////QCursor::setPos(dt->width(), dt->height()/2); // not at bottom because then mouse movement on Mac would show dock
	//


	////StimGLScene = new StimulGLGraphScene(this);
	////if(StimGLScene->Init())
	////{
	////	GraphView = new QGraphicsView(StimGLScene);
	////	GraphView->setWindowModality(Qt::WindowModal);
	////	//connect(StimGLScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
	////	GraphView->showFullScreen();
	////}
}

//bool ExperimentManager_Dialog::eventFilter(QObject *target, QEvent *event)
//{
//	if (target == stimContainerDlg) 
//	{
//		if (event->type() == QEvent::KeyPress) 
//		{
//			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//
//			switch (keyEvent->key())
//			{
//			case Qt::Key_A://Key_Escape:	//To abort the experiment
//				if((keyEvent->modifiers() & Qt::ControlModifier))// && (keyEvent->modifiers() & Qt::AltModifier))
//				{
//					//emit UserWantsToClose();
//					int aa = 8;
//				}
//				break;
//				//case Qt::Key_Tab:
//				//	setExperimentObjectReadyToUnlock();
//				//	break;
//			case Qt::Key_Alt:	//To start the experiment
//				int bb = 8;
//				//setExperimentObjectReadyToUnlock();					
//				break;
//			}
//		}
//	}
//	return true;
//}

void ExperimentManager_Dialog::exampleButton_2_Pressed()
{
	//rootObject = dynamic_cast<QObject *>(qmlViewer->rootObject());// get root object
	//if((!rootObject) || (!myClass))
	//	return;
	//QObject *signalObject = rootObject->findChild<QObject *>(QString("item2"));
	//if (signalObject)  // element found
	//{
	//	bool bCouldConnect = QObject::connect(signalObject, SIGNAL(qmlSignal(QString)),myClass, SLOT(cppSignaledSlot(QString)));//Each new connection generates the slot!!
	//	if(!bCouldConnect)
	//		bCouldConnect = false;
	//}
}

void ExperimentManager_Dialog::exampleButton_3_Pressed()
{
	//// Using QDeclarativeComponent
	//QDeclarativeEngine engine;
	//QDeclarativeComponent component(&engine, qmlMainFilePath);//QUrl::fromLocalFile("MyItem.qml"));
	//QObject *object = component.create();	

	//QDeclarativeItem *item = qobject_cast<QDeclarativeItem*>(object);
	//item->setWidth(500);

	//object->setProperty("width", 500);
	//QDeclarativeProperty(object, "width").write(500);

	//delete object;
	//if(!rootObject)
	//	return;

	//QObject *image = rootObject->findChild<QObject *>(QString("theImage"));
	//if (image)  // element found
	//{
	//	image->setProperty("source", QUrl::fromLocalFile("E:\\Projects\\StimulGL\\Install\\qml\\Transitions\\Transitions80.png"));
	//} 
	//else 
	//{
	//	qDebug() << "'theImage' not found";
	//}
	//QObject *text = rootObject->findChild<QObject *>(QString("theText"));
	//if (text)  // element found
	//{
	//	text->setProperty("text", QString("Changed"));
	//} 
	//else 
	//{
	//	qDebug() << "'text' not found";
	//}
	//QObject *rectangle = rootObject->findChild<QObject *>(QString("theRectangle"));
	//if (rectangle)  // element found
	//{
	//	rectangle->setProperty("color", "red");
	//} 
	//else 
	//{
	//	qDebug() << "'rectangle' not found";
	//}

	//if(imageViewerObject)
	//{
	//	imageViewerObject->setImage(QUrl("sss"));
	//}

	//QObject *imageViewerObject_cast = rootObject->findChild<QObject *>(QString("theImageViewer"));
	//if (imageViewerObject_cast)  // element found
	//{
	//	imageViewerObject_cast->setProperty("image", "image_url");
	//} 
	//else 
	//{
	//	qDebug() << "'imageViewerObject' not found";
	//}

}

void ExperimentManager_Dialog::exampleButton_4_Pressed()
{
	//QDeclarativeEngine engine;
	//QDeclarativeComponent component(&engine, "MyItem.qml");
	//QObject *object = component.create();

	//QObject *propertyObject = rootObject->findChild<QObject *>(QString("propertyitem"));
	//if (propertyObject)  // element found
	//{
	//	qDebug() << "Property value:" << QDeclarativeProperty::read(propertyObject, "someNumber").toInt();
	//	QDeclarativeProperty::write(propertyObject, "someNumber", 5000);
	//	qDebug() << "Property value:" << propertyObject->property("someNumber").toInt();
	//	propertyObject->setProperty("someNumber", 100);
	//	qDebug() << "Property value:" << propertyObject->property("someNumber").toInt();
	//} 
	//else 
	//{
	//	qDebug() << "'propertyObject' not found";
	//}
}

void ExperimentManager_Dialog::exampleButton_5_Pressed()
{
	//rootObject->setProperty("width", 500);
	//QDeclarativeProperty(rootObject, "width").write(500);
	//qmlViewer->show();
	//QVariant returnedValue;
	//QVariant msg = "Hello from C++";
	//QObject *qmlFunction = rootObject->findChild<QObject *>(QString("theFuntionItem"));
	//if (qmlFunction)  // element found
	//{
	//	if(QMetaObject::invokeMethod(qmlFunction, "myQmlFunction", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg)))//rootObject, "myQmlFunction", Q_RETURN_ARG(QVariant, returnedValue), Q_ARG(QVariant, msg)))
	//	{
	//		QString sRetVal = returnedValue.toString();
	//		qDebug() << "QML function returned:" << sRetVal;
	//	}
	//	//delete object;
	//}
}

//void ExperimentManager_Dialog::on_GraphScene_Closed()
//{
//	//disconnect(StimGLScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
//	GraphView->close();
//	delete GraphView;
//	delete StimGLScene;
//	GraphView = NULL;
//	StimGLScene = NULL;
//}
