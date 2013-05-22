//StimulGL
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

#include "mainwindow.h"
#include "mainappinfo.h"
#include "globalapplicationinformation.h"
#include "StimulGL.h"
#include "svgview.h"
#include "aboutqtdialog.h"  
#include "sleepthread.h"
#include "optionpage.h"
#include <QPluginLoader>
#include <QtPlugin>
#include <QFileInfo>
#include <QMetaObject>
#include <QtGlobal>
#include "OutputListDelegate.h"

#include "../Plugins/ParallelPortDevice/parallelport.h"
#include "../Plugins/ExperimentManager/ExperimentManager.h"
#include "../Plugins/KeyBoardDevice/KeyBoardCapture.h"

MainWindow::MainWindow() : DocumentWindow(), SVGPreviewer(new SvgView)
{
	Plugins = NULL;
	tcpServer = NULL;
	clientConnection = NULL;
	networkDataBlockSize = 0;
	DocManager = NULL;
	AppScriptEngine = NULL;
	globAppInfo = NULL;
	mainAppInfoStruct = NULL;
	PluginsFound = false;
	DevicePluginsFound = false;
	ExtensionPluginsFound = false;
	helpAssistant = new Assistant;
	bMainWindowIsInitialized = false;
	resetContextState();

#ifndef QT_NO_DEBUG_OUTPUT	
	qInstallMessageHandler((QtMessageHandler)MainAppInfo::MyOutputHandler);
	qDebug("StimulGL Main Constructor.");
#endif
//When compiling your final application, be sure to add the flags -DQT_NO_DEBUG and -DQT_NO_DEBUG_OUTPUT.  
//These will reduce the application size and ensure that the logfile isn't used. 
//Alternately, you can keep NO_DEBUG_OUTPUT out of the makefile if you still want the logfile.
}

MainWindow::~MainWindow()//see void MainWindow::closeEvent(QCloseEvent *event)!
{
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if(!closeSubWindow())
	{
		event->ignore();
		return;
	}
	mdiArea->closeAllSubWindows();
	if (activeMdiChild()) 
	{
		event->ignore();
	} 
	else 
	{
		writeMainWindowSettings();
		event->accept();
	}
	delete helpAssistant;
#ifndef QT_NO_DEBUG_OUTPUT
	MainAppInfo::CloseMainLogFile();	
#endif
	if(Plugins)
		delete Plugins;
#ifdef Q_WS_WIN
	timeEndPeriod(1);
#endif
	MainAppInfo::SetMainWindow(NULL);
	if(DocManager)
	{
		disconnect(DocManager, SIGNAL(DocumentManagerOutput(QString)), this, SLOT(write2OutputWindow(QString)));
		disconnect(DocManager, SIGNAL(NrOfLinesChanged(int)), this, SLOT(NumberOfLinesChanged(int)));	
		delete DocManager;
	}
	if(globAppInfo)
	{
		delete globAppInfo;
		globAppInfo = NULL;
	}
	if(mainAppInfoStruct)
	{
		delete mainAppInfoStruct;
		mainAppInfoStruct = NULL;
	}
	shutdownNetworkServer();
}

bool MainWindow::setContextState(const QString &sContextName)
{
	if(sContextName == "")
		return false;
	if(currentScriptEngineContexes.isEmpty())
		return true;
	for(int i=0;i<currentScriptEngineContexes.count();i++)
	{
		if(currentScriptEngineContexes.at(i).sContextName == sContextName)//Does it exist?
		{
			pCurrentSetContextState.first = sContextName;
			pCurrentSetContextState.second = lCurrentRunningScriptIDList.last();
			return true;
		}
	}	
	return false;
}

bool MainWindow::resetContextState(const quint64 &nScriptId)
{
	if(nScriptId == 0)
	{
		pCurrentSetContextState.first = "";
		pCurrentSetContextState.second = 0;
		return true;
	}
	else if(pCurrentSetContextState.second == nScriptId)
	{
		pCurrentSetContextState.first = "";
		pCurrentSetContextState.second = 0;
		return true;
	}
	return false;
}

bool MainWindow::deleteContextState(const QString &sContextName)
{
	if(sContextName == "")
		return false;
	if(currentScriptEngineContexes.isEmpty() == false)
	{		
		for(int i=0;i<currentScriptEngineContexes.count();i++)
		{
			if(currentScriptEngineContexes.at(i).sContextName == sContextName)//Does it exist?
			{
				currentScriptEngineContexes.removeAt(i);
				return true;
			}
		}
	}		
	return false;
}

bool MainWindow::saveContextState(const QString &sContextName)
{
	if(AppScriptEngine)
	{
		if(AppScriptEngine->eng)
		{
			if(sContextName == "")
				return false;
			QScriptValue act_Object = AppScriptEngine->eng->currentContext()->parentContext()->activationObject();
			QScriptValue this_Object = AppScriptEngine->eng->currentContext()->parentContext()->thisObject();

			if(currentScriptEngineContexes.isEmpty() == false)
			{		
				for(int i=0;i<currentScriptEngineContexes.count();i++)
				{
					if(currentScriptEngineContexes.at(i).sContextName == sContextName)//Should we overwrite?
					{
						currentScriptEngineContexes[i].activationObject = act_Object;
						currentScriptEngineContexes[i].thisObject = this_Object;
						return true;
					}
				}
			}
			QScriptContextStructure tmpStruct;
			tmpStruct.sContextName = sContextName;
			tmpStruct.activationObject = act_Object;
			tmpStruct.thisObject = this_Object;
			currentScriptEngineContexes.append(tmpStruct);
			return true;
		}
	}
	return false;
}

#ifdef DEBUG
QString MainWindow::testFunction(QString inp)
{
	//if(AppScriptEngine)
	//{
	//}
	return inp;
}
#endif

bool MainWindow::openDroppedFiles(const QStringList &pathList)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	this->openFiles(NULL,pathList);
	return true;
}

QVariant MainWindow::invokeJavaScriptConfigurationFile(const QString &sCode)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	return globAppInfo->invokeJavaScriptConfigurationFile(sCode);
}

void MainWindow::showJavaScriptConfigurationFile()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	globAppInfo->showJavaScriptConfigurationFile();
}

bool MainWindow::receiveExchangeMessage(const QString &sMessage)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	bool bWasNotJetInitialized = false;
	while(bMainWindowIsInitialized == false)
	{
		processEvents();
		bWasNotJetInitialized = true;
	}
	if(bWasNotJetInitialized)
		write2OutputWindow("-> SocketData (delayed) Received [" + sMessage + "]");
	else
		write2OutputWindow("-> SocketData Received [" + sMessage + "]");
	
	QScriptValue scriptVal = executeScriptContent(sMessage);

	QString strResult = scriptVal.toString();
	if (scriptVal.isError()) 
	{
		write2OutputWindow("... Script stopped Evaluating due to an error: --> " + strResult + "...");
	}
	else
	{
		write2OutputWindow("-> SocketData Successfully executed by the Script Engine.");
	}
	return true;
}

void MainWindow::DebugcontextMenuEvent(const QPoint &pos)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	QMenu menu(this);
	menu.addAction(clearDebuggerAction);
	menu.addAction(copyDebuggerAction);
	menu.exec(outputWindowList->viewport()->mapToGlobal(pos));
}

void MainWindow::setupContextMenus()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	outputWindowList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(outputWindowList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(DebugcontextMenuEvent(QPoint)));
}

bool MainWindow::initialize(GlobalApplicationInformation::MainProgramModeFlags mainFlags)
{
	StimulGLFlags = mainFlags;
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	//Default					= 0x00000,
	//DisablePlugins			= 0x00001,
	//DisableSplash				= 0x00002
	//QWaitCondition sleep;
	qDebug() << "Starting and initializing" << MAIN_PROGRAM_FULL_NAME;
	//registerFileTypeByDefinition("StimulGL.QtScript","StimulGL Qt Script File",".qs");
	//MainAppInfo::Initialize(this);
	//QApplication::setGraphicsSystem("opengl");//"raster");
    AppScriptStatus = GlobalApplicationInformation::NoScript;

	if (StimulGLFlags.testFlag(GlobalApplicationInformation::DisableSplash) == false)
	{
		QPixmap pixmap(":/resources/splash.png");
		MainSplashScreen = new QSplashScreen(pixmap);
		MainSplashScreen->show();
		showSplashMessage("Initializing Program...");
	}
	showSplashMessage("Setup User Interface...");
	setupMDI();
	setWindowTitle(globAppInfo->getTitle());//  MainAppInfo::MainProgramTitle());
	setUnifiedTitleAndToolBarOnMac(true);
	createDockWindows();
	setAppDirectories();
	setupScriptEngine();
	setupStatusBar();
	createDefaultMenus();
	setupDynamicPlugins();
	setupHelpMenu();
	setupToolBars();
	updateMenuControls(0);
	parseRemainingGlobalSettings();
	setupContextMenus();
	if (startUpFiles.count()> 0) { openFiles(NULL,startUpFiles);}
	if (StimulGLFlags.testFlag(GlobalApplicationInformation::DisableSplash)==false)
	{
		MainSplashScreen->finish(this);
	}
#ifdef Q_WS_WIN
	if (timeBeginPeriod(1) == TIMERR_NOCANDO)
		qWarning() << "Could not start the time period!";
#endif
	setDefaultGLFormat();
	statusBar()->showMessage(tr("Ready..."), 2000);
	//if (QApplication::desktop()->numScreens() > 1) {
	//	QLabel *label = new QLabel("Hello");
	//	label->setGeometry(QApplication::desktop()->availableGeometry(1));
	//	label->showMaximized();
	//}
	bMainWindowIsInitialized = true;
	if (globAppInfo->shouldEnableNetworkServer())
		setupNetworkServer(globAppInfo->getHostAddress(),globAppInfo->getHostPort());
	return true;
}

//void MainWindow::registerFileTypeByDefinition(const QString &DocTypeName, const QString &DocTypeDesc, const QString &DocTypeExtension)
//{
//	//registerFileType("GiMdi.Document",          // Document type name
//	//	"MDI Text Editor Document",// User readable file type name
//	//	".gidoc",                  // file extension
//	//	0,                         // index of the icon to use for the files.
//	//	true);                     // register for DDE events
//	registerFileType(DocTypeName, DocTypeDesc, DocTypeExtension, 0, true);
//	//registerFileType("GiMdi.Document", "MDI Text Editor Document", ".gidoc", 0, true);
//	registerCommand("fancyCommand", DocTypeName, "-fancy %1", "[fancy(\"%1\")]");
//	enableShellOpen();
//	//QMessageBox msgBox(QMessageBox::Information,"Info","enableShellOpen() called",QMessageBox::Ok,this);
//	//msgBox.exec();
//}

void MainWindow::showSplashMessage(const QString message)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if (StimulGLFlags.testFlag(GlobalApplicationInformation::DisableSplash) == false)
	{
		MainSplashScreen->showMessage(message,Qt::AlignBottom ,Qt::white);
	}	
}

void MainWindow::setGlobalApplicationInformationObject(GlobalApplicationInformation *globAppInformation)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	globAppInfo = globAppInformation;
	mainAppInfoStruct = new MainAppInformationStructure(globAppInformation->getMainAppInformationStructure());
}

void MainWindow::setupMDI()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	mdiArea = new QMdiArea;
	mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdiArea);
	connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)), this, SLOT(updateMenuControls(QMdiSubWindow *)));
	windowMapper = new QSignalMapper(this);
	connect(windowMapper, SIGNAL(mapped(QWidget *)), this, SLOT(setActiveSubWindow(QWidget *)));
	setupDocumentManager();
}

void MainWindow::setupDocumentManager()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	DocManager = new DocumentManager (this);
	connect(DocManager, SIGNAL(DocumentManagerOutput(QString)), this, SLOT(write2OutputWindow(QString)));
	DocManager->appendKnownFileExtensionList(MainAppInfo::getDefaultFileExtList());
	connect(DocManager, SIGNAL(NrOfLinesChanged(int)), this, SLOT(NumberOfLinesChanged(int)));
}

QScriptValue myPauseFunction(QScriptContext *ctx, QScriptEngine *eng)
{
	Q_UNUSED(eng);
	int nWaitTime = ctx->argument(0).toInteger();

	QMutex mutTmp;
	mutTmp.lock();
	QWaitCondition waitCondition;
	waitCondition.wait(&mutTmp, nWaitTime);
	mutTmp.unlock();
	return 0;
}

QScriptValue myThrowScriptErrorFunction(QScriptContext *ctx, QScriptEngine *eng)
{
	Q_UNUSED(eng);
	QString result;
	for (int i = 0; i < ctx->argumentCount(); ++i) {
		if (i > 0)
			result.append(" ");
		result.append(ctx->argument(i).toString());
	}
	ctx->throwError(result);
	return 0;
}

QScriptValue myBeepFunction(QScriptContext *ctx, QScriptEngine *eng)
{
	Q_UNUSED(eng);
	Q_UNUSED(ctx);
	//DWORD Tone(2);//frequency, duration
	//Tone[0] = 100;
	//Tone[1] = 100;
	//for (int i = 0; i < ctx->argumentCount(); ++i) {
	//if (i > 0)
	//	result.append(" ");
	//result.append(ctx->argument(i).toString());
	//}
	qApp->beep();

	//system("beep -f 200 -l 100");
	return 0;
}

QScriptValue myIncludeFunction(QScriptContext *ctx, QScriptEngine *eng)
{
	QString fileName = ctx->argument(0).toString();
	QString fileText;
	QFile file(fileName);
	if(file.exists())
	{
		if ( file.open(QFile::ReadOnly | QFile::Truncate) ) 
		{       
			// file opened successfully
			QTextStream textStream(&file);        // use a text stream
			// until end of file...
			//while ( !textStream.eof() ) {           
			// read and parse the command line
			//fileText = textStream.readLine();         // line of text excluding '\n'
			// do something with the line
			//...
			//}

			fileText = textStream.readAll();
			// Close the file
			file.close();
		}
	}
	ctx->setActivationObject(ctx->parentContext()->activationObject());
	ctx->setThisObject(ctx->parentContext()->thisObject());
	return eng->evaluate(fileText, fileName);
}

QScriptValue myPrintFunction(QScriptContext *context, QScriptEngine *engine)
{
	QString result;
	for (int i = 0; i < context->argumentCount(); ++i)
	{
		if (i > 0)
			result.append(" ");
		result.append(context->argument(i).toString());
	}
	QScriptValue calleeData = context->callee().data();
	QListWidget *outputObject = qobject_cast<QListWidget*>(calleeData.toQObject());
	outputObject->addItem(result);
	return engine->undefinedValue();

	outputObject->setItemDelegate(new OutputListDelegate(outputObject));
	QListWidgetItem *item = new QListWidgetItem();
	item->setData(Qt::DisplayRole, "Title");
	item->setData(Qt::UserRole + 1, "Description");
	outputObject->addItem(item);

	return engine->undefinedValue();//outputWindowList
}

QScriptValue myExitScriptFunction(QScriptContext *context, QScriptEngine *engine)
{
	context->setActivationObject(context->parentContext()->activationObject());
	context->setThisObject(context->parentContext()->thisObject());
	engine->collectGarbage();
	return 0;
}

#ifdef DEBUG
QScriptValue myScriptTestFunction(QScriptContext *context, QScriptEngine *engine)
{
	Q_UNUSED(engine);
	Q_UNUSED(context);
	return NULL;
}
#endif

void MainWindow::setupNetworkServer(const QString &sAddress, quint16 port)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if (globAppInfo->shouldEnableNetworkServer())
	{
		tcpServer = new QTcpServer(this);

		QHostAddress address = QHostAddress::Any;
		if(sAddress != "")
			address.setAddress(sAddress); 
		if (!tcpServer->listen(address,port)) 
		{
			QString sInfo = QString("Unable to start the network server: %1.").arg(tcpServer->errorString());
			qDebug() << __FUNCTION__ << sInfo;
			//write2OutputWindow(QString("Unable to start the network server: %1.").arg(tcpServer->errorString()));
			QMessageBox msgBox(QMessageBox::Warning,"Network Server",sInfo, QMessageBox::Ok);
			msgBox.exec();
			return;
		}
		QString ipAddress;
		QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
		// use the first non-localhost IPv4 address
		for (int i = 0; i < ipAddressesList.size(); ++i) {
			if (ipAddressesList.at(i) != QHostAddress::LocalHost &&	ipAddressesList.at(i).toIPv4Address()) 
			{
					ipAddress = ipAddressesList.at(i).toString();
					break;
			}
		}
		// if we did not find one, use IPv4 localhost
		if (ipAddress.isEmpty())
			ipAddress = QHostAddress(QHostAddress::LocalHost).toString();

		connect(tcpServer, SIGNAL(newConnection()), this, SLOT(newIncomingServerConnection()));
		QString usedAddress = sAddress;
		if(usedAddress == "")
			usedAddress = QString("%1").arg(ipAddress);
		QString sInfo = QString("Network Server started @ %1:%2").arg(usedAddress).arg(tcpServer->serverPort());
		qDebug() << sInfo;
		//write2OutputWindow(sInfo);
		//QMessageBox msgBox(QMessageBox::Information,"Network Server",QString("Network Server started @ : %1:%2").arg(ipAddress).arg(tcpServer->serverPort()), QMessageBox::Ok);
		//msgBox.exec();
		return;
	}
}

void MainWindow::shutdownNetworkServer()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if(tcpServer)
	{
		tcpServer->close();
		delete tcpServer;
		tcpServer = NULL;
	}
	//if(clientConnection) // is automatically performed!
	//{
	//	//if(clientConnection->isc)
	//	//clientConnection->disconnectFromHost();
	//	//clientConnection->close();
	//	delete clientConnection;
	//	clientConnection = NULL;
	//}
}

void MainWindow::newIncomingServerConnection()
{
	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_5_0);
	out << (quint16)0;
	QString sInfo = QString("Welcome you're connected to StimulGL @ %1:%2").arg(tcpServer->serverAddress().toString()).arg(tcpServer->serverPort());
	out << sInfo; 
	out.device()->seek(0);
	out << (quint16)(block.size() - sizeof(quint16));

	clientConnection = tcpServer->nextPendingConnection();
	clientConnection->setSocketOption(QAbstractSocket::LowDelayOption, 1);
	clientConnection->setSocketOption(QAbstractSocket::KeepAliveOption, 1);
	connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
	connect(clientConnection, SIGNAL(readyRead()), this, SLOT(receivedNetworkData()));
	connect(clientConnection, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorNetworkData(QAbstractSocket::SocketError)));
	
	sInfo = QString("The Network Server accepted an incoming connection");
	qDebug() << sInfo;
	
	clientConnection->write(block);
}

void MainWindow::receivedNetworkData()
{
	QDataStream in(clientConnection);
	in.setVersion(QDataStream::Qt_5_0);
	if (networkDataBlockSize == 0) 
	{
		if (clientConnection->bytesAvailable() < (int)sizeof(quint16))
			return;
		in >> networkDataBlockSize;
	}
	if (clientConnection->bytesAvailable() < networkDataBlockSize)
		return;
	QString sAvailableData;
	in >> sAvailableData;
	emit NetworkDataAvailable(sAvailableData);
	networkDataBlockSize = 0;

	QScriptValue scriptVal = executeScriptContent(sAvailableData);
	QString strResult = scriptVal.toString();
	if (scriptVal.isError()) 
	{
		write2OutputWindow("... Script stopped Evaluating due to an error: --> " + strResult + "...");
	}
	else
	{
		write2OutputWindow("-> SocketData Successfully executed by the Script Engine.");
	}
}

void MainWindow::errorNetworkData(QAbstractSocket::SocketError socketError)
{
     switch (socketError) 
	 {
     case QAbstractSocket::RemoteHostClosedError:
		 qDebug() << __FUNCTION__ << "The client disconnected the network connection.";
         break;
     case QAbstractSocket::HostNotFoundError:
		 qDebug() << __FUNCTION__ << "The host was not found. Please check the host name and port settings.";
         break;
     case QAbstractSocket::ConnectionRefusedError:
         qDebug() << __FUNCTION__ << "The connection was refused by the peer. "
                                     "Make sure the network server is running, "
                                     "and check that the host name and port "
                                     "settings are correct.";
         break;
     default:
         qDebug() << __FUNCTION__ << QString("The following Network error occurred: %1.").arg(clientConnection->errorString());
     }
}

void MainWindow::setupScriptEngine()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	//qDebug() << "setupScriptEngine() - start";
	if(AppScriptEngine)
	{
		delete AppScriptEngine;
		AppScriptEngine = NULL;
	}
	AppScriptEngine = new QTScriptEngine(this);
	if (StimulGLFlags.testFlag(GlobalApplicationInformation::DisableAllScriptBindings) == false)
	{
		if (globAppInfo->shouldLoadScriptBindings())
		{
			if (!AppScriptEngine->ImportScriptExtensions())
			{
				QMessageBox msgBox(QMessageBox::Warning,"QT Script Extensions","Some of the defined QT Script Extensions could not be loaded!", QMessageBox::Ok);
				msgBox.exec();
			}
		}
	}
	lCurrentRunningScriptIDList.clear();
	//currentRunningScriptID = 0;
	connect(AppScriptEngine, SIGNAL(ScriptLoaded(qint64)), this, SLOT(scriptLoaded(qint64)));
	connect(AppScriptEngine, SIGNAL(ScriptUnloaded(qint64)), this, SLOT(scriptUnloaded(qint64)));

	QScriptValue logVal = AppScriptEngine->eng->newFunction(myPrintFunction);//wrap function to QScriptValue
	logVal.setData(AppScriptEngine->eng->newQObject(outputWindowList));//this data is not script-accessible, but the newQObject can be retrieved using QScriptContext::callee() 
	AppScriptEngine->eng->globalObject().setProperty("Log", logVal);

	QScriptValue beepVal = AppScriptEngine->eng->newFunction(myBeepFunction);
	AppScriptEngine->eng->globalObject().setProperty("Beep", beepVal);

	QScriptValue pauseVal = AppScriptEngine->eng->newFunction(myPauseFunction);
	AppScriptEngine->eng->globalObject().setProperty("Pause", pauseVal);

	QScriptValue throwVal = AppScriptEngine->eng->newFunction(myThrowScriptErrorFunction);
	AppScriptEngine->eng->globalObject().setProperty("ThrowError", throwVal);

	QScriptValue includeVal = AppScriptEngine->eng->newFunction(myIncludeFunction);
	AppScriptEngine->eng->globalObject().setProperty("Include", includeVal);

	QScriptValue exitVal = AppScriptEngine->eng->newFunction(myExitScriptFunction);
	AppScriptEngine->eng->globalObject().setProperty("ExitScript", exitVal);

	QScriptValue AppScriptThisObject = AppScriptEngine->eng->newQObject(this);
	AppScriptEngine->eng->globalObject().setProperty("StimulGL", AppScriptThisObject);

#ifdef DEBUG
	QScriptValue TestFunctionVal = AppScriptEngine->eng->newFunction(myScriptTestFunction);
	AppScriptEngine->eng->globalObject().setProperty("Test", TestFunctionVal);
#endif

	//We must declare it so that the type will be known to QMetaType, find "Q_DECLARE_METATYPE(DocFindFlags)"
	//Q_DECLARE_METATYPE(DocFindFlags)
	//Next, the DocFindFlags conversion functions. We represent the DocFindFlags value as a script object and just copy the properties, see below after this function body!

	//Now we can register DocFindFlags with the engine:	
	qScriptRegisterMetaType(AppScriptEngine->eng, sciFindDialog::DocFindFlagstoScriptValue, sciFindDialog::DocFindFlagsfromScriptValue);
	
	//Working with DocFindFlags values is now easy:
	////DocFindFlags s = qscriptvalue_cast<DocFindFlags>(AppScriptEngine->eng->currentContext()->argument(0));
	////DocFindFlags s2;
	////s2.x = s.x + 10;
	////s2.y = s.y + 20;
	////QScriptValue v = engine->toScriptValue(s2);
	//If you want to be able to construct values of your custom type from script code, you have to register a constructor function for the type. For example:
	//QScriptValue createMyStruct(QScriptContext *, QScriptEngine *engine)
	//{
	//	MyStruct s;
	//	s.x = 123;
	//	s.y = 456;
	//	return engine->toScriptValue(s);
	//}
	//QScriptValue ctor = AppScriptEngine->eng.newFunction(createMyStruct);
	//engine.globalObject().setProperty("MyStruct", ctor);
	QScriptValue ctor = AppScriptEngine->eng->newFunction(sciFindDialog::DocFindFlagsConstructor);
	AppScriptEngine->eng->globalObject().setProperty("DocFindFlags", ctor);

	//int nShortListId = 
		qScriptRegisterSequenceMetaType<QList<short>>(AppScriptEngine->eng);
	//int nFloatListId = 
		qScriptRegisterSequenceMetaType<QList<float>>(AppScriptEngine->eng);
	//int nDoubleListId = 
		qScriptRegisterSequenceMetaType<QList<double>>(AppScriptEngine->eng);
}

bool MainWindow::restartScriptEngine()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	QMdiSubWindow *tmpSubWindow = activeMdiChild();
	setupScriptEngine();
	for (int i=0;i<Plugins->Count()-1;i++)
	{
		configurePluginScriptEngine(i);
	}
	configureDebugger();
	updateMenuControls(tmpSubWindow);
	resetContextState();
	return true;
}

void MainWindow::scriptLoaded(qint64 id)
{
	lCurrentRunningScriptIDList.append(id);
    setScriptRunningStatus(GlobalApplicationInformation::Executing);
}

void MainWindow::scriptUnloaded(qint64 id)
{
	int nIndex = lCurrentRunningScriptIDList.indexOf(id);
	if(nIndex >= 0)
	{
		lCurrentRunningScriptIDList.removeAt(nIndex);
	}
	if(lCurrentRunningScriptIDList.isEmpty())
		resetContextState();
	else
		resetContextState(id);
    setScriptRunningStatus(GlobalApplicationInformation::Pending);
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window)
		return;
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::updateMenuControls(QMdiSubWindow *subWindow)
{
	Q_UNUSED(subWindow);
	//write2Debugger("updateMenuControls entered..."); this doesn't happen that often....
	QMdiSubWindow *currentSub = activeMdiChild();
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentSub));
	bool hasMdiChild = ((DocManager->count() > 0) && (currentSub != 0));
	if((tmpCustomQsciScintilla) || (hasMdiChild == false))
	{
		StatusLinesLabel->setHidden(!hasMdiChild);
		StatusPositionLabel->setHidden(!hasMdiChild);
		StatusNameLabel->setHidden(!hasMdiChild);

		addRemMarkerAction->setEnabled(hasMdiChild);
		nextMarkerAction->setEnabled(hasMdiChild);
		prevMarkerAction->setEnabled(hasMdiChild);
		remAllMarkerAction->setEnabled(hasMdiChild);

		clearDebuggerAction->setEnabled(true);
		copyDebuggerAction->setEnabled(true);

		goToLineAction->setEnabled(hasMdiChild);
		goToMatchingBraceAction->setEnabled(hasMdiChild);
		selToMatchingBraceAction->setEnabled(hasMdiChild);
		lineCommentAction->setEnabled(hasMdiChild);
		blockCommentAction->setEnabled(hasMdiChild);
		duplicateLineAction->setEnabled(hasMdiChild);
		moveLineUpAction->setEnabled(hasMdiChild);
		deleteCurrentLineAction->setEnabled(hasMdiChild);
		toUpperCaseAction->setEnabled(hasMdiChild);
		toLowerCaseAction->setEnabled(hasMdiChild);

		//searchAction->setEnabled(hasMdiChild);
		saveAction->setEnabled(hasMdiChild);
		saveAsAction->setEnabled(hasMdiChild);
		pasteAction->setEnabled(hasMdiChild);
		closeAction->setEnabled(hasMdiChild);
		closeAllAction->setEnabled(hasMdiChild);
		tileAction->setEnabled(hasMdiChild);
		cascadeAction->setEnabled(hasMdiChild);
		nextAction->setEnabled(hasMdiChild);
		previousAction->setEnabled(hasMdiChild);
		separatorAct->setVisible(hasMdiChild);

		bool hasSelection = false;
		CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentSub));
		if(tmpCustomQsciScintilla)
			hasSelection = (hasMdiChild && (tmpCustomQsciScintilla->hasSelectedText()));

		cutAction->setEnabled(hasSelection);
		copyAction->setEnabled(hasSelection);
	}
	else
	{
		StatusLinesLabel->setHidden(true);
		StatusPositionLabel->setHidden(true);
		StatusNameLabel->setHidden(true);
		addRemMarkerAction->setEnabled(false);
		nextMarkerAction->setEnabled(false);
		prevMarkerAction->setEnabled(false);
		remAllMarkerAction->setEnabled(false);
		clearDebuggerAction->setEnabled(true);
		copyDebuggerAction->setEnabled(true);
		goToLineAction->setEnabled(false);
		goToMatchingBraceAction->setEnabled(false);
		selToMatchingBraceAction->setEnabled(false);
		lineCommentAction->setEnabled(false);
		blockCommentAction->setEnabled(false);
		duplicateLineAction->setEnabled(false);
		moveLineUpAction->setEnabled(false);
		deleteCurrentLineAction->setEnabled(false);
		toUpperCaseAction->setEnabled(false);
		toLowerCaseAction->setEnabled(false);
		//searchAction->setEnabled(hasMdiChild);
		saveAction->setEnabled(hasMdiChild);
		saveAsAction->setEnabled(hasMdiChild);
		pasteAction->setEnabled(false);
		closeAction->setEnabled(hasMdiChild);
		closeAllAction->setEnabled(hasMdiChild);
		tileAction->setEnabled(hasMdiChild);
		cascadeAction->setEnabled(hasMdiChild);
		nextAction->setEnabled(hasMdiChild);
		previousAction->setEnabled(hasMdiChild);
		separatorAct->setVisible(hasMdiChild);

		cutAction->setEnabled(false);
		copyAction->setEnabled(false);	

		findAction->setEnabled(false);
		findNextAction->setEnabled(false);
		findPrevAction->setEnabled(false);
		replaceAction->setEnabled(false);
	}
	if (hasMdiChild)
	{
		QString docTitle = strippedName(DocManager->getFileName(currentSub));
		if (docTitle == "")
			docTitle = MainAppInfo::UntitledDocName();
		setWindowTitle(tr("%1 - %2").arg(globAppInfo->getTitle()).arg(docTitle));
		GlobalApplicationInformation::DocType d;
		d = DocManager->getDocType(currentSub);
		printAction->setEnabled(false);
		switch (d)
		{
		case GlobalApplicationInformation::DOCTYPE_QSCRIPT:
			{
				if(lCurrentRunningScriptIDList.isEmpty())
					setScriptRunningStatus(GlobalApplicationInformation::Pending);
				if(tmpCustomQsciScintilla)
					printAction->setEnabled(true);
				break;
			}
		case GlobalApplicationInformation::DOCTYPE_JAVASCRIPT:
			{
				if(lCurrentRunningScriptIDList.isEmpty())
					setScriptRunningStatus(GlobalApplicationInformation::Pending);
				if(tmpCustomQsciScintilla)
					printAction->setEnabled(true);
				break;
			}			
		case GlobalApplicationInformation::DOCTYPE_SVG:
			{
				if(lCurrentRunningScriptIDList.isEmpty())
					setScriptRunningStatus(GlobalApplicationInformation::Pending);
				if(tmpCustomQsciScintilla)
					printAction->setEnabled(true);
				break;
			}
		case GlobalApplicationInformation::DOCTYPE_PLUGIN_DEFINED:
			{
				if(lCurrentRunningScriptIDList.isEmpty())
					setScriptRunningStatus(GlobalApplicationInformation::Pending);
				if(tmpCustomQsciScintilla)
					printAction->setEnabled(true);
				break;
			}
		case GlobalApplicationInformation::DOCTYPE_UNDEFINED:
			{
				if(tmpCustomQsciScintilla)
					printAction->setEnabled(false);
				break;
			}
		}

		CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentSub));
		if(tmpCustomQsciScintilla)
			NumberOfLinesChanged(tmpCustomQsciScintilla->lines());
		StatusNameLabel->setText(QString("%1").arg(docTitle));
	}
	else
	{
		setWindowTitle(tr("%1").arg(globAppInfo->getTitle()));
		if(lCurrentRunningScriptIDList.isEmpty())
			setScriptRunningStatus(GlobalApplicationInformation::NoScript);
	}
}

void MainWindow::CursorPositionChanged(int line, int col) 
{
	StatusPositionLabel->setText(tr("Row: %1, Col: %2").arg(line+1).arg(col+1));
}

bool MainWindow::setDefaultGLFormat()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	QGLFormat StimulGLQGLFormat;
	StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
	StimulGLQGLFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(StimulGLQGLFormat);
	return true;
}

void MainWindow::createDefaultMenus()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	fileMenu = new QMenu(tr("&File"), this);
	editMenu = new QMenu(tr("&Edit"), this);
	markersMenu = new QMenu(tr("&Markers"), this);
	//viewMenu = new QMenu(tr("&View"), this);
	scriptMenu = new QMenu(tr("&Script"), this);
	toolsMenu = new QMenu(tr("&Tools"), this);
	windowMenu = new QMenu(tr("&Window"), this);

	newScriptAction = new QAction(QIcon(":/resources/new.png"),tr("&New QT Script"), this);
	newScriptAction->setStatusTip(tr("Create a new QT Script document"));
	newScriptAction->setData(GlobalApplicationInformation::DOCTYPE_QSCRIPT);
	connect(newScriptAction, SIGNAL(triggered()), this, SLOT(newFile()));
	fileMenu->addAction(newScriptAction);

	//newAction = new QAction(QIcon(":/resources/new.png"),tr("&New Document"), this);
	//newAction->setShortcuts(QKeySequence::New);
	//newAction->setStatusTip(tr("Create a new document"));
	//newAction->setData(DOCTYPE_UNDEFINED);
	//connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
	//fileMenu->addAction(newAction);

	//newSVGAction = new QAction(tr("&New SVG Document"), this);
	//newSVGAction->setStatusTip(tr("Create a new SVG document"));
	//newSVGAction->setData(DOCTYPE_SVG);
	//connect(newSVGAction, SIGNAL(triggered()), this, SLOT(newFile()));
	//fileMenu->addAction(newSVGAction);

	openAction = new QAction(QIcon(":/resources/open.png"),tr("&Open..."), this);
	openAction->setShortcut(QKeySequence(QKeySequence::Open));
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFiles()));
	fileMenu->addAction(openAction);

	saveAction = new QAction(QIcon(":/resources/save.png"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the document to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	fileMenu->addAction(saveAction);

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
	fileMenu->addAction(saveAsAction);

	printAction = new QAction(QIcon(":/resources/print.png"), tr("&Print..."), this);
	printAction->setShortcuts(QKeySequence::Print);
	printAction->setStatusTip(tr("Print the current document"));
	printAction->setEnabled(false);
	connect(printAction, SIGNAL(triggered()), this, SLOT(print()));
	fileMenu->addAction(printAction);

	for (int i = 0; i < MaxRecentFiles; ++i) 
	{
		recentFileActs.append(new QAction(this));
		recentFileActs.last()->setVisible(false);
		connect(recentFileActs.last(), SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}

	separatorAct = fileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
	{
		fileMenu->addAction(recentFileActs[i]);
	}
	fileMenu->addSeparator();
	updateRecentFileActions();

	closeAction = new QAction(tr("Cl&ose"), this);
	closeAction->setData("Close");
	closeAction->setShortcut(tr("Ctrl+F4"));
	closeAction->setStatusTip(tr("Close the active window"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(closeSubWindow()));//mdiArea, SLOT(closeActiveSubWindow()));
	fileMenu->addAction(closeAction);

	closeAllAction = new QAction(tr("Close &All"), this);
	closeAllAction->setData("CloseAll");
	closeAllAction->setStatusTip(tr("Close all the windows"));
	connect(closeAllAction, SIGNAL(triggered()), this, SLOT(closeSubWindow())); //mdiArea, SLOT(closeAllSubWindows()));
	fileMenu->addAction(closeAllAction);

	quitAction = new QAction(tr("E&xit"), this);
	quitAction->setShortcut(QKeySequence(tr("Ctrl+Q")));
	quitAction->setStatusTip(tr("Exit the application"));
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));
	fileMenu->addAction(quitAction);
	menuBar()->addMenu(fileMenu);//the file menu..........................................................

	cutAction = new QAction(QIcon(":/resources/cut.png"), tr("Cu&t"), this);
	cutAction->setShortcuts(QKeySequence::Cut);
	cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
	connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
	editMenu->addAction(cutAction);

	copyAction = new QAction(QIcon(":/resources/copy.png"), tr("&Copy"), this);
	copyAction->setShortcuts(QKeySequence::Copy);
	copyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
	editMenu->addAction(copyAction);

	pasteAction = new QAction(QIcon(":/resources/paste.png"), tr("&Paste"), this);
	pasteAction->setShortcuts(QKeySequence::Paste);
	pasteAction->setStatusTip(tr("Paste the clipboard's contents into the current selection"));
	connect(pasteAction, SIGNAL(triggered()), this, SLOT(paste()));
	editMenu->addAction(pasteAction);

	editMenu->addSeparator();

	findAction = new QAction(QIcon(""), tr("Find"), this);
	findAction->setShortcuts(QKeySequence::Find);
	findAction->setStatusTip(tr("Find"));
	connect(findAction, SIGNAL(triggered()), this, SLOT(find()));
	editMenu->addAction(findAction);

	findNextAction = new QAction(QIcon(""), tr("Find Next"), this);
	findNextAction->setShortcuts(QKeySequence::FindNext);
	findNextAction->setStatusTip(tr("Find Next"));
	connect(findNextAction, SIGNAL(triggered()), this, SLOT(findNext()));
	editMenu->addAction(findNextAction);

	findPrevAction = new QAction(QIcon(""), tr("Find Previous"), this);
	findPrevAction->setShortcuts(QKeySequence::FindPrevious);
	findPrevAction->setStatusTip(tr("Find Previous"));
	connect(findPrevAction, SIGNAL(triggered()), this, SLOT(findPrev()));
	editMenu->addAction(findPrevAction);

	replaceAction = new QAction(QIcon(""), tr("Replace"), this);
	replaceAction->setShortcuts(QKeySequence::Replace);
	replaceAction->setStatusTip(tr("Replace"));
	connect(replaceAction, SIGNAL(triggered()), this, SLOT(replace()));
	editMenu->addAction(replaceAction);
	
	//searchAction = new QAction(tr("Search"), this);//QIcon(":/resources/comment.png")
	//searchAction->setShortcut(tr("Ctrl+F"));
	//searchAction->setStatusTip(tr("Search through the current document."));
	//connect(searchAction, SIGNAL(triggered()), this, SLOT(search()));
	//editMenu->addAction(searchAction);

	editMenu->addSeparator();

	copyDebuggerAction = new QAction(QObject::tr("Copy Selected Output Item(s)"), 0);
	//copyDebuggerAction->setShortcut(QKeySequence(""));
	copyDebuggerAction->setStatusTip(tr("Copy the Selected Debugger Output window line(s)."));
	connect(copyDebuggerAction, SIGNAL(triggered()), this, SLOT(copyDebugger()));
	editMenu->addAction(copyDebuggerAction);

	clearDebuggerAction = new QAction(QObject::tr("Clear All Output Item(s)"), 0);
	//clearDebuggerAction->setShortcut(QKeySequence(""));
	clearDebuggerAction->setStatusTip(tr("Clear the Debugger Output window."));
	connect(clearDebuggerAction, SIGNAL(triggered()), this, SLOT(clearDebugger()));
	editMenu->addAction(clearDebuggerAction);

	editMenu->addSeparator();

	//commentAction = new QAction(QIcon(":/resources/comment.png"), tr("Comment"), this);
	//commentAction->setShortcut(tr("Ctrl+K"));
	//commentAction->setStatusTip(tr("Comment out the selected lines."));
	//connect(commentAction, SIGNAL(triggered()), this, SLOT(comment()));
	//editMenu->addAction(commentAction);

	//uncommentAction = new QAction(QIcon(":/resources/uncomment.png"), tr("Uncomment"), this);
	//uncommentAction->setShortcut(tr("Ctrl+U"));
	//uncommentAction->setStatusTip(tr("Uncomment out the selected lines."));
	//connect(uncommentAction, SIGNAL(triggered()), this, SLOT(unComment()));
	//editMenu->addAction(uncommentAction);
	goToLineAction = new QAction(QObject::tr("Go to Line"), 0);
	//goToLineAction->setShortcut(QKeySequence(""));
	goToLineAction->setStatusTip(tr("Go to Line."));
	connect(goToLineAction, SIGNAL(triggered()), this, SLOT(goToLine()));
	editMenu->addAction(goToLineAction);
	goToMatchingBraceAction = new QAction(QObject::tr("Jump to the matching brace"), 0);
	goToMatchingBraceAction->setShortcut(QKeySequence("Ctrl+E"));
	goToMatchingBraceAction->setStatusTip(tr("Jump to the matching brace."));
	connect(goToMatchingBraceAction, SIGNAL(triggered()), this, SLOT(jumpToMatchingBrace()));
	editMenu->addAction(goToMatchingBraceAction);
	selToMatchingBraceAction = new QAction(QObject::tr("Select to the matching brace"), 0);
	selToMatchingBraceAction->setShortcut(QKeySequence(tr("Shift+Ctrl+E")));
	selToMatchingBraceAction->setStatusTip(tr("Select to the matching brace."));
	connect(selToMatchingBraceAction, SIGNAL(triggered()), this, SLOT(selectToMatchingBrace()));
	editMenu->addAction(selToMatchingBraceAction);
	lineCommentAction = new QAction(QObject::tr("(Un)Comment the selected line(s)"), 0);
	lineCommentAction->setShortcut(QKeySequence(tr("Ctrl+k")));
	lineCommentAction->setStatusTip(tr("(Un)Comment the selected line(s)."));
	connect(lineCommentAction, SIGNAL(triggered()), this, SLOT(lineComment()));
	editMenu->addAction(lineCommentAction);
	blockCommentAction = new QAction(QObject::tr("(Un)Comment the selected block"), 0);
	blockCommentAction->setShortcut(QKeySequence(tr("Ctrl+b")));
	blockCommentAction->setStatusTip(tr("(Un)Comment the selected block."));
	connect(blockCommentAction, SIGNAL(triggered()), this, SLOT(blockComment()));
	editMenu->addAction(blockCommentAction);
	duplicateLineAction = new QAction(QObject::tr("Duplicate the current line"), 0);
	duplicateLineAction->setShortcut(QKeySequence(tr("Ctrl+D")));
	duplicateLineAction->setStatusTip(tr("Duplicate the selected line."));
	connect(duplicateLineAction, SIGNAL(triggered()), this, SLOT(duplicateLine()));
	editMenu->addAction(duplicateLineAction);
	moveLineUpAction = new QAction(QObject::tr("Move the current line up"), 0);
	moveLineUpAction->setShortcut(QKeySequence(tr("Ctrl+T")));
	moveLineUpAction->setStatusTip(tr("Move the current line up."));
	connect(moveLineUpAction, SIGNAL(triggered()), this, SLOT(moveLineUp()));
	editMenu->addAction(moveLineUpAction);
	deleteCurrentLineAction = new QAction(QObject::tr("Delete the current line"), 0);
	deleteCurrentLineAction->setShortcut(QKeySequence(tr("Ctrl+L")));
	deleteCurrentLineAction->setStatusTip(tr("Delete the current line."));
	connect(deleteCurrentLineAction, SIGNAL(triggered()), this, SLOT(deleteCurrentLine()));
	editMenu->addAction(deleteCurrentLineAction);
	toUpperCaseAction = new QAction(QObject::tr("Change the line to UPPER CASE"), 0);
	toUpperCaseAction->setShortcut(QKeySequence(tr("Ctrl+U")));
	toUpperCaseAction->setStatusTip(tr("Change the line to UPPER CASE."));
	//connect(toUpperCaseAction, SIGNAL(triggered()), this, SLOT(toUpperCase()));
	editMenu->addAction(toUpperCaseAction);
	toLowerCaseAction = new QAction(QObject::tr("Change the line to lower case"), 0);
	toLowerCaseAction->setShortcut(QKeySequence(tr("Ctrl+Shift+U")));
	toLowerCaseAction->setStatusTip(tr("Change the line to lower case."));
	//connect(toLowerCaseAction, SIGNAL(triggered()), this, SLOT(toLowerCase()));
	editMenu->addAction(toLowerCaseAction);
	menuBar()->addMenu(editMenu);//the edit menu..........................................................

	tileAction = new QAction(tr("&Tile"), this);
	tileAction->setStatusTip(tr("Tile the windows"));
	connect(tileAction, SIGNAL(triggered()), mdiArea, SLOT(tileSubWindows()));

	cascadeAction = new QAction(tr("&Cascade"), this);
	cascadeAction->setStatusTip(tr("Cascade the windows"));
	connect(cascadeAction, SIGNAL(triggered()), mdiArea, SLOT(cascadeSubWindows()));

	nextAction = new QAction(tr("Ne&xt"), this);
	nextAction->setShortcuts(QKeySequence::NextChild);
	nextAction->setStatusTip(tr("Move the focus to the next window"));
	connect(nextAction, SIGNAL(triggered()), mdiArea, SLOT(activateNextSubWindow()));

	previousAction = new QAction(tr("Pre&vious"), this);
	previousAction->setShortcuts(QKeySequence::PreviousChild);
	previousAction->setStatusTip(tr("Move the focus to the previous window"));
	connect(previousAction, SIGNAL(triggered()), mdiArea, SLOT(activatePreviousSubWindow()));
	menuBar()->addMenu(windowMenu);
	updateWindowMenu();
	connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));//the window menu..........................................................

	addRemMarkerAction = new QAction(tr("Add/Remove marker"), this);
	//addRemMarkerAction->setShortcuts(QKeySequence::NextChild);
	addRemMarkerAction->setStatusTip(tr("Add/Remove marker"));
	connect(addRemMarkerAction, SIGNAL(triggered()), this, SLOT(toggleMarker()));
	//markersMenu->addAction(addRemMarkerAction);

	nextMarkerAction = new QAction(tr("Next marker"), this);
	//nextMarkerAction->setShortcuts(QKeySequence::NextChild);
	nextMarkerAction->setStatusTip(tr("Next marker"));
	connect(nextMarkerAction, SIGNAL(triggered()), this, SLOT(nextMarker()));
	//markersMenu->addAction(nextMarkerAction);

	prevMarkerAction = new QAction(tr("Previous marker"), this);
	//prevMarkerAction->setShortcuts(QKeySequence::NextChild);
	prevMarkerAction->setStatusTip(tr("Previous marker"));
	connect(prevMarkerAction, SIGNAL(triggered()), this, SLOT(prevMarker()));
	//markersMenu->addAction(prevMarkerAction);

	remAllMarkerAction = new QAction(tr("Remove all markers"), this);
	//remAllMarkerAction->setShortcuts(QKeySequence::NextChild);
	remAllMarkerAction->setStatusTip(tr("Remove all markers"));
	connect(remAllMarkerAction, SIGNAL(triggered()), this, SLOT(removeAllMarkers()));
	//markersMenu->addAction(remAllMarkerAction);

	connect(markersMenu, SIGNAL(aboutToShow()), SLOT(updateMarkersMenu()));

	menuBar()->addMenu(markersMenu);
	updateMarkersMenu();//the markers menu..........................................................

	runScriptAction = scriptMenu->addAction(QIcon(":/resources/runScript.png"),tr("Run Script"));
	runScriptAction->setEnabled(false);
	runScriptAction->setShortcut(QKeySequence(tr("F5")));
	runScriptAction->setStatusTip(tr("Run the current script"));
	runScriptAction->setData(GlobalApplicationInformation::Execute);
	connect(runScriptAction, SIGNAL(triggered()), this, SLOT(executeScript()));

	//debugScriptAction = scriptMenu->addAction(tr("Debug Script"));//QIcon(":/resources/runScript.png"),tr("&Run Script"));
	//debugScriptAction->setEnabled(false);//(false);
	//debugScriptAction->setShortcut(QKeySequence(tr("F6")));
	//debugScriptAction->setStatusTip(tr("Open the current script in the Debugger"));
	//debugScriptAction->setData(GlobalApplicationInformation::Debug);
	//connect(debugScriptAction, SIGNAL(triggered()), this, SLOT(executeScript()));

	abortScriptAction = scriptMenu->addAction(tr("Abort Script"));//QIcon(":/resources/runScript.png"),tr("&Run Script"));
	abortScriptAction->setEnabled(false);//false);
	abortScriptAction->setShortcut(QKeySequence(tr("F7")));
	abortScriptAction->setStatusTip(tr("Stop the current script from running"));
	connect(abortScriptAction, SIGNAL(triggered()), this, SLOT(abortScript()));

	restartScriptEngineAction = scriptMenu->addAction(tr("Restart Script Engine"));
	restartScriptEngineAction->setEnabled(false);//false);
	restartScriptEngineAction->setShortcut(QKeySequence(tr("F9")));
	restartScriptEngineAction->setStatusTip(tr("Force a restart of the script engine"));
	connect(restartScriptEngineAction, SIGNAL(triggered()), this, SLOT(restartScriptEngine()));


	menuBar()->addMenu(scriptMenu);//the script menu..........................................................

	//m_backgroundAction = viewMenu->addAction(tr("&Background"));
	//m_backgroundAction->setStatusTip(tr("Toggle Background"));
	//m_backgroundAction->setEnabled(false);
	//m_backgroundAction->setCheckable(true);
	//m_backgroundAction->setChecked(false);
	//connect(m_backgroundAction, SIGNAL(toggled(bool)), SVGPreviewer, SLOT(setViewBackground(bool)));

	//m_outlineAction = viewMenu->addAction(tr("&Outline"));
	//m_outlineAction->setStatusTip(tr("Toggle Outline"));
	//m_outlineAction->setEnabled(false);
	//m_outlineAction->setCheckable(true);
	//m_outlineAction->setChecked(true);
	//connect(m_outlineAction, SIGNAL(toggled(bool)), SVGPreviewer, SLOT(setViewOutline(bool)));
	
	//viewMenu->addAction(debugLogDock->toggleViewAction());
	//viewMenu->addAction(debuggerDock->toggleViewAction());
	//menuBar()->addMenu(viewMenu);//the view menu..........................................................

	optionsAction = toolsMenu->addAction(tr("O&ptions"));
	optionsAction->setShortcut(QKeySequence(tr("Ctrl+P")));
	optionsAction->setStatusTip(tr("Open the options dialog"));
	//optionsAction->setEnabled(true);
	connect(optionsAction, SIGNAL(triggered()), this, SLOT(openOptionsDialog()));
	menuBar()->addMenu(toolsMenu);//the tools menu..........................................................

	//debuggerMenu = AppScriptEngine->createDebuggerMenu(menuBar());//debugger->createStandardMenu(menuBar());
	//menuBar()->addMenu(debuggerMenu);
}

void MainWindow::setupHelpMenu()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	helpMenu = menuBar()->addMenu(tr("Help"));
	
	assistantAct = new QAction(tr("Help Contents"), this);
	assistantAct->setStatusTip(tr("Show the StimulGL Documentation"));
	helpMenu->addAction(assistantAct);
	assistantAct->setShortcut(QKeySequence::HelpContents);
	connect(assistantAct, SIGNAL(triggered()), this, SLOT(showDocumentation()));
	
	aboutStimulGLAct = new QAction(tr("About StimulGL"), this);
	aboutStimulGLAct->setStatusTip(tr("Show the StimulGL About Dialog"));
	helpMenu->addAction(aboutStimulGLAct);
	aboutStimulGLAct->setShortcut(QKeySequence(tr("F2")));
	connect(aboutStimulGLAct, SIGNAL(triggered()), this, SLOT(aboutStimulGL()));

	aboutQtAct = new QAction(tr("About Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt About Dialog"));
	helpMenu->addAction(aboutQtAct);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	historyQtAct = new QAction(tr("StimulGL History"), this);
	historyQtAct->setStatusTip(tr("Show the StimulGL History"));
	helpMenu->addAction(historyQtAct);
	connect(historyQtAct, SIGNAL(triggered()), this, SLOT(showJavaScriptConfigurationFile()));

	menuBar()->addMenu(helpMenu);//the help menu..........................................................
}

void MainWindow::showDocumentation()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	helpAssistant->showDocumentation("index.html", globAppInfo);
}

/*! \brief Appends a text String to the Output Log Window.
 *
 * This function appends a provided String to the Output Log Window.
 * @param text2Write a String value holding the text that should be appended.
 */
void MainWindow::write2OutputWindow(const QString &text2Write)// See the defined MAIN_PROGRAM_LOG_SLOT_NAME
{
	if(bMainWindowIsInitialized)
		outputWindowList->addItem(text2Write);
}

/*! \brief Clears the Output Log Window.
 *
 * This function clears the Output Log Window.
 */
void MainWindow::clearOutputWindow() 
{
	clearDebugger();
}

void MainWindow::clearDebugger()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	outputWindowList->clear();
}

void MainWindow::copyDebugger()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	QList<QPair<QString,int>> lListTextAndRows;
	QList <QListWidgetItem *> listItems;
	QString sResult = "";
	int nRow,i;
	listItems = outputWindowList->selectedItems();
	if(listItems.isEmpty())
		return;
	for(i=0;i<listItems.size();i++) 
	{
		nRow = outputWindowList->row(listItems[i]);//0 means false
		lListTextAndRows.append(qMakePair(listItems[i]->text(),nRow));
	}
	qSort(lListTextAndRows.begin(), lListTextAndRows.end(), QPairSecondComparer());	
	for(i=0;i<lListTextAndRows.size();i++) 
	{
		sResult.append(lListTextAndRows.at(i).first);
		if(i<lListTextAndRows.size()-1)
			sResult.append("\n");
	}
	QApplication::clipboard()->setText(sResult);
}

void MainWindow::createDockWindows()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	debugLogDock = new QDockWidget(tr("Output"), this);
	debugLogDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);	
	outputWindowList = new QListWidget(debugLogDock);
	outputWindowList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//QString tmpColor = QString::number(STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_RED) + "," + QString::number(STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_GREEN) + "," + QString::number(STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_BLUE);
	//outputWindowList->setStyleSheet("* { background-color:rgb(" + tmpColor + "); padding: 10px ; color:rgb(136,0,21)}");
	debugLogDock->setWidget(outputWindowList);
	addDockWidget(Qt::RightDockWidgetArea, debugLogDock);

	//viewMenu->addAction(debugLogDock->toggleViewAction());
	//connect(debugList, SIGNAL(currentTextChanged(const QString &)),
	//	this, SLOT(insertCustomer(const QString &)));

//////////////////////////////////////////////////////////////////////////

	debuggerDock = new QDockWidget(tr("Debugger"), this);
	debuggerDock->setAllowedAreas(Qt::TopDockWidgetArea | Qt::RightDockWidgetArea | Qt::LeftDockWidgetArea | Qt::BottomDockWidgetArea);
	
//	stackWidget = AppScriptEngine->debugger->widget(QScriptEngineDebugger::StackWidget);


	//debuggerMainWindow = AppScriptEngine->DebuggerStandardWindow();//debugger->standardWindow();
	
	
	//debuggerMainWindow->setWindowFlags(Qt::WindowFlags::)
	//debuggerMainWindow->showMaximized();
	//debugList2 = new QListWidget(debuggerDock);
	//QLayout *layout = new QHBoxLayout;
	//layout->addWidget(debugList2);
	//layout->addWidget(codeWindow);
	//layout->addWidget(stackWidget);

	//debuggerDock->setWidget(stackWidget);//codeWindow);//debugList2);

	//stackWidget->setParent(debuggerDock);
	//debuggerMainWindow->setParent(debuggerDock);

	//debuggerDock->setLayout(layout);
	addDockWidget(Qt::BottomDockWidgetArea, debuggerDock);
}

void MainWindow::setupDynamicPlugins()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if (StimulGLFlags.testFlag(GlobalApplicationInformation::DisableAllPlugins) == false)
	{
		extendAPICallTips(this->metaObject());
		showSplashMessage("Loading Static Plugins...");
        Q_IMPORT_PLUGIN(ParallelPortPlugin)// see below
        Q_IMPORT_PLUGIN(ExperimentManagerPlugin)// see below
		Q_IMPORT_PLUGIN(KeyBoardPlugin)// see below

		//Q_UNUSED(qt_static_plugin_ExperimentManagerPlugin);
		//Q_UNUSED(qt_static_plugin_KeyBoardPlugin);
		//Q_UNUSED(qt_static_plugin_ParallelPortPlugin);

		bool bRetVal = false;
		QString strRetVal = "";
		const QMetaObject* metaObject = NULL;

		foreach (QObject *plugin, QPluginLoader::staticInstances())
		{
			DeviceInterface *iDevice = qobject_cast<DeviceInterface *>(plugin);
			if (iDevice) 
			{
				iDevice->fetchGlobalAppInfo();
			}
			else 
			{
				ExtensionInterface *iExtension = qobject_cast<ExtensionInterface *>(plugin);
				if (iExtension) 
				{
					iExtension->fetchGlobalAppInfo();
				}				
			}
			metaObject = plugin->metaObject();
			//QStringList methods;
			//for(i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
			//{
			//	methods << QString::fromLatin1(metaObject->method(i).signature());
			//	DocManager->addAdditionalApiEntry(QString::fromLatin1(metaObject->method(i).signature()));
			//}
			if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_ISCOMPATIBLE_FULL).toLatin1())) == -1))//Is the slot present?
			{				
				if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_GETSCRIPTMETAOBJECT_FULL).toLatin1())) == -1))//Is the slot present?
				{
					QObject *pointerQObject = NULL;
					const QMetaObject* metaScriptObject;
					int i = 0;
					bool bResult;
					while(true)
					{
						//Invoke the slot
						pointerQObject = NULL;
						bResult = metaObject->invokeMethod(plugin, FUNC_PLUGIN_GETSCRIPTMETAOBJECT,Qt::DirectConnection, Q_RETURN_ARG(QObject*,(QObject*)pointerQObject), Q_ARG(int,(int)i));				
						if(bResult)
						{
							if(pointerQObject == NULL)
								break;
							if(i > 99)
							{
								qDebug() << __FUNCTION__ << "::Wrong Plugin(" << metaObject->className() << ") implementation, no limit or too many meta-objects(" << FUNC_PLUGIN_GETSCRIPTMETAOBJECT << ")!";
								break;
							}
							metaScriptObject = (const QMetaObject*) pointerQObject;
							if (metaScriptObject && bResult)
								extendAPICallTips(metaScriptObject);
						}
						else
						{
							break;
						}
						i++;
					}
				}

				popPluginIntoMenu(plugin);
				//Additional File Extensions defined?
				if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_GETADDFILEEXT_FULL).toLatin1())) == -1))//Is the slot present?
				{
					//Invoke the slot
					QStringList strRetValList;
					metaObject->invokeMethod(plugin, FUNC_PLUGIN_GETADDFILEEXT,Qt::DirectConnection, Q_RETURN_ARG(QStringList, strRetValList));//if(!metaObject->invokeMethod(plugin, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))				
					for (int i=0;i<strRetValList.count();i++)
					{
						DocManager->appendKnownFileExtensionList(strRetValList[i]);
					}

					if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS_FULL).toLatin1())) == -1))//Is the slot present?
					{
						//Invoke the slot
						metaObject->invokeMethod(plugin, FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS,Qt::DirectConnection, Q_RETURN_ARG(QStringList, strRetValList));			
						for (int i=0;i<strRetValList.count();i++)
						{
							DocManager->appendKnownDocumentFileHandlerList(strRetValList[i],plugin);
							//QString a = strRetValList[i];
						}
					}
				}
			}
			else
			{
				qDebug() << __FUNCTION__ << ", Could not invoke the Static Plugin slot(" << QString(FUNC_PLUGIN_ISCOMPATIBLE_FULL) << ")!";	
			}
			metaObject = NULL;
			bRetVal = false;
		}
		showSplashMessage("Loading Dynamic Plugins...");
		foreach (QString fileName, QDir(MainAppInfo::pluginsDirPath()).entryList(QDir::Files)) 
		{
			QPluginLoader loader(QDir(MainAppInfo::pluginsDirPath()).absoluteFilePath(fileName));
			QObject *plugin = loader.instance();//The QObject provided by the plugin, if it was compiled against an incompatible version of the Qt library, QPluginLoader::instance() returns a null pointer.
			if (plugin) 
			{
				DeviceInterface *iDevice = qobject_cast<DeviceInterface *>(plugin);
				if (iDevice) 
				{
					iDevice->fetchGlobalAppInfo();
				}
				else 
				{
					ExtensionInterface *iExtension = qobject_cast<ExtensionInterface *>(plugin);
					if (iExtension) 
					{
						iExtension->fetchGlobalAppInfo();
					}				
				}
				metaObject = plugin->metaObject();
				//QStringList properties;
				//for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
				//	properties << QString::fromLatin1(metaObject->method(i).signature());
				if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_ISCOMPATIBLE_FULL).toLatin1())) == -1))//Is the slot present?
				{
					//qWarning() << __FUNCTION__ << ", Checking plugin compatibility(" << fileName << ")...";	
					//Invoke the slot
					metaObject->invokeMethod(plugin, FUNC_PLUGIN_ISCOMPATIBLE,Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal));//if(!metaObject->invokeMethod(plugin, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))				
					if (bRetVal)
					{
						bool bResult;
						QObject *pointerQObject = NULL;
						const QMetaObject* metaScriptObject = NULL;
						int i=0;
						while(true)
						{
							//Invoke the slot
							pointerQObject = NULL;
							bResult = metaObject->invokeMethod(plugin, FUNC_PLUGIN_GETSCRIPTMETAOBJECT,Qt::DirectConnection, Q_RETURN_ARG(QObject*,(QObject*)pointerQObject), Q_ARG(int,(int)i));				
							if(bResult)
							{
								if(metaScriptObject == NULL)
									break;
								if(i > 99)
								{
									qDebug() << __FUNCTION__ << "::Wrong Plugin(" << metaObject->className() << ") implementation, no limit or too many meta-objects(" << FUNC_PLUGIN_GETSCRIPTMETAOBJECT << ")!";
									break;
								}
								metaScriptObject = (const QMetaObject*) pointerQObject;
								if (metaScriptObject && bResult)
									extendAPICallTips(metaScriptObject);
							}
							else
							{
								break;
							}
							i++;
						}
						
						//qWarning() << __FUNCTION__ << ", Plugin is compatibility(" << fileName << ")";
						if (popPluginIntoMenu(plugin))
						{
							pluginFileNames += fileName;
							//Additional File Extensions defined?
							if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_GETADDFILEEXT_FULL).toLatin1())) == -1))//Is the slot present?
							{
								//Invoke the slot
								QStringList strRetValList;
								metaObject->invokeMethod(plugin, FUNC_PLUGIN_GETADDFILEEXT,Qt::DirectConnection, Q_RETURN_ARG(QStringList, strRetValList));//if(!metaObject->invokeMethod(plugin, Qt::DirectConnection, Q_RETURN_ARG(bool, bRetVal)))				
								for (int i=0;i<strRetValList.count();i++)
								{
									DocManager->appendKnownFileExtensionList(strRetValList[i]);
								}	
								if (!(metaObject->indexOfMethod(QMetaObject::normalizedSignature(QString(FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS_FULL).toLatin1())) == -1))//Is the slot present?
								{
									//Invoke the slot
									metaObject->invokeMethod(plugin, FUNC_PLUGIN_GETADDFILE_SLOT_HANDLERS,Qt::DirectConnection, Q_RETURN_ARG(QStringList, strRetValList));			
									for (int i=0;i<strRetValList.count();i++)
									{
										DocManager->appendKnownDocumentFileHandlerList(strRetValList[i],plugin);
										//QString a = strRetValList[i];
									}
								}
							}
						}
					}
					else
					{
						qDebug() << __FUNCTION__ << "::The Dynamic Plugin is incompatible(" << fileName << ")!"; //metaObject->className() << ")!";
					}
				}
				else
				{
					qDebug() << __FUNCTION__ << "::Could not invoke the Dynamic Plugin slot(" << fileName << ", " << QString(FUNC_PLUGIN_ISCOMPATIBLE_FULL) << ")!";	
				}
				metaObject = NULL;
				bRetVal = false;
			}
			else
			{
				qDebug() << __FUNCTION__ << "::Could not load the Dynamic Plugin (" << fileName << ")!";
			}
		}
	}
	DocManager->appendKnownFileExtensionList(MAIN_PROGRAM_POST_FILESEXTENSION_LIST);
	////example 1(static plugin):
	//see line Q_IMPORT_PLUGIN(parallelportplugin)
	//ParallelPort *a = new ParallelPort(888,NULL); //see Q_DECLARE_METATYPE(ParallelPort*), must include header etc
	//a->IsPortEcp();
	//delete a;
	//ExperimentManager *b = new ExperimentManager(this);
	//b->openExperiment();
	//delete b;
}

bool MainWindow::extendAPICallTips(const QMetaObject* metaScriptObject)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if (metaScriptObject)
	{
		// <item signature> ::SuperClassName||ScriptClassName::Type::ReturnType::Description

		int i;
		QString tmpString;
		QString sScriptClassName;
		QString sSuperClassName;
		QStringList sClassInfoList;
		QString sDivider = "::";

		sScriptClassName = metaScriptObject->className();
		sSuperClassName = metaScriptObject->superClass()->className();

		//int amount = 
			metaScriptObject->classInfoCount();
		for(i = metaScriptObject->classInfoOffset(); i < metaScriptObject->classInfoCount(); ++i)
		{
			if (QString::fromLatin1(metaScriptObject->classInfo(i).name()) == QString(SCRIPT_API_CLASS_NAME_TAG))
			{
				sScriptClassName = QString::fromLatin1(metaScriptObject->classInfo(i).value());				
			}			
		}
		DocManager->addAdditionalApiEntry(sScriptClassName + " " + sDivider + sSuperClassName);
		
		for(i = metaScriptObject->methodOffset(); i < metaScriptObject->methodCount(); ++i)
		{
			sClassInfoList << QString::fromLatin1(metaScriptObject->method(i).methodSignature());

			//is this an member signal or slot?
			QString mType = METHOD_TYPE_MEMBER_TAG;
			if(metaScriptObject->indexOfSignal(sClassInfoList.at(sClassInfoList.count()-1).toLatin1()) >= 0)
				mType = METHOD_TYPE_SIGNAL_TAG;
			else if(metaScriptObject->indexOfSlot(sClassInfoList.at(sClassInfoList.count()-1).toLatin1()) >= 0)
				mType = METHOD_TYPE_SLOT_TAG;
			DocManager->addAdditionalApiEntry(QString::fromLatin1(metaScriptObject->method(i).methodSignature()) + " " + sDivider + sScriptClassName + sDivider + mType);
		}

		for(i = metaScriptObject->propertyOffset(); i < metaScriptObject->propertyCount(); ++i)
		{
			sClassInfoList << QString::fromLatin1(metaScriptObject->property(i).name());
			DocManager->addAdditionalApiEntry(QString::fromLatin1(metaScriptObject->property(i).name()) + " " + sDivider + sScriptClassName + sDivider + METHOD_TYPE_PROPERTY_TAG);
		}

		for(i = metaScriptObject->enumeratorOffset(); i < metaScriptObject->enumeratorCount(); ++i)
		{
			sClassInfoList << QString::fromLatin1(metaScriptObject->enumerator(i).name());
			//DocManager->addAdditionalApiEntry(QString::fromLatin1(metaScriptObject->method(i).signature()) + " " + sDivider + sScriptClassName + sDivider + METHOD_TYPE_ENUMERATOR_TAG);
		}

		//QStringList constructor;
		//int constructorcount = metaScriptObject->constructorCount();
		//if(constructorcount>0)
		//	constructorcount = constructorcount;
		//for(i = metaScriptObject->indexOfConstructor(); i < metaScriptObject->constructorCount(); ++i)
		//{
		//	constructor << QString::fromLatin1(metaScriptObject->method(i).signature());
		//	DocManager->addAdditionalApiEntry(QString::fromLatin1(metaScriptObject->method(i).signature()));
		//}
		return true;
	}
	return false;
};

bool MainWindow::checkPluginCompatibility(QObject *plugin)
{
	bool bRetval = false;

	DeviceInterface *iDevice = qobject_cast<DeviceInterface *>(plugin);
	if (iDevice) 
	{
		bRetval = iDevice->IsCompatible();
	}

	ExtensionInterface *iExtension = qobject_cast<ExtensionInterface *>(plugin);
	if (iExtension) 
	{
		bRetval = iExtension->IsCompatible();
	}

	//qobject_cast<DeviceInterface *>(PlgInterfaces.at(index));

	//plugin->ConfigureScriptEngine(* AppScriptEngine->eng);
	////collection->GetLoaderName(tmpIndex) +")");
	////collection->GetInterface(tmpIndex)->ConfigureScriptEngine(* AppScriptEngine->eng);

	return bRetval;
}

bool MainWindow::popPluginIntoMenu(QObject *plugin)
{
	QAction *pluginAction;

	if (!PluginsFound)
	{
		pluginsMenu  = menuBar()->addMenu(tr("&Plugins"));
		Plugins = new PluginCollection(this);
		menuBar()->addMenu(pluginsMenu);//the Plugins menu..........................................................
		PluginsFound = true;
	}

	DeviceInterface *iDevice = qobject_cast<DeviceInterface *>(plugin);//For each plugin (static or dynamic), we check which interfaces it implements using qobject_cast()
	if (iDevice) 
	{
		if(checkPluginCompatibility(plugin))
		{
			if (!DevicePluginsFound)
			{
				devicePluginMenu = pluginsMenu->addMenu(tr("&Device Plugins"));
				DevicePluginsFound = true;
			}
			pluginAction = integratePlugin(plugin,Plugins);
			devicePluginMenu->addAction(pluginAction);
			pluginsMenu->addMenu(devicePluginMenu);//the devices menu..........................................................
			return true;
		}
		return false;
	}

	ExtensionInterface *iExtension = qobject_cast<ExtensionInterface *>(plugin);//For each plugin (static or dynamic), we check which interfaces it implements using qobject_cast()
	if (iExtension) 
	{
		if(checkPluginCompatibility(plugin))
		{
			if (!ExtensionPluginsFound)
			{
				extensionPluginMenu = pluginsMenu->addMenu(tr("&Extension Plugins"));
				ExtensionPluginsFound = true;
			}
			pluginAction = integratePlugin(plugin,Plugins);
			extensionPluginMenu->addAction(pluginAction);
			pluginsMenu->addMenu(extensionPluginMenu);//the extension menu..........................................................
			return true;
		}
		return false;
	}
	return false;
}

QAction* MainWindow::integratePlugin(QObject *plugin, PluginCollection *collection)
{
	int tmpIndex;
	tmpIndex = Plugins->Add(plugin);
	if (tmpIndex >=0)//if Add succeeded...
	{
		QAction *action1 = new QAction(collection->GetInterface(tmpIndex)->GetPluginInformation(), plugin);
		action1->setData(collection->GetLoaderName(tmpIndex));
		connect(action1, SIGNAL(triggered()), this, SLOT(showPluginGUI()));
		action1->setStatusTip(tr("Show the Plugins UI(") + collection->GetLoaderName(tmpIndex) +")");
		//collection->GetInterface(tmpIndex)->ConfigureScriptEngine(* AppScriptEngine->eng);
		configurePluginScriptEngine(tmpIndex);
		return action1;
	}
	return 0;
}

bool MainWindow::configurePluginScriptEngine(const int nIndex)
{
	return Plugins->GetInterface(nIndex)->ConfigureScriptEngine(* AppScriptEngine->eng);
}

void MainWindow::setAppDirectories()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	QStringList paths = qApp->libraryPaths();
	paths <<  MainAppInfo::pluginsDirPath();// pluginsDir.absolutePath();
#ifndef QT_NO_DEBUG_OUTPUT	
	qDebug() << "Plugin path defined: " << MainAppInfo::pluginsDirPath();	
#endif
	qApp->setLibraryPaths(paths);
}

void MainWindow::showPluginGUI()
{
	QAction *action = qobject_cast<QAction *>(sender());
	Plugins->GetInterface(Plugins->GetIndex(action->data().toString()))->ShowGUI();	
}

void MainWindow::setScriptRunningStatus(GlobalApplicationInformation::ActiveScriptMode state)
{
	AppScriptStatus = state;
	switch (state)
	{
	case GlobalApplicationInformation::Debugging:
		{ 
			runScriptAction->setEnabled(false);
//			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(true);
			restartScriptEngineAction->setEnabled(true);
			break; 
		}	
	case GlobalApplicationInformation::Executing:
		{ 
			runScriptAction->setEnabled(false);
//			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(true);
			restartScriptEngineAction->setEnabled(true);
			break; 
		}	
	case GlobalApplicationInformation::NoScript:
		{ 
			runScriptAction->setEnabled(false);
//			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(false);
			restartScriptEngineAction->setEnabled(false);
			break; 
		}	
	case GlobalApplicationInformation::Pending:
		{ 
			runScriptAction->setEnabled(true);
//			debugScriptAction->setEnabled(true);
			abortScriptAction->setEnabled(false);
			restartScriptEngineAction->setEnabled(false);
			break; 
		}	
	case GlobalApplicationInformation::Stopping:
		{ 
			runScriptAction->setEnabled(false);
//			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(true);
			restartScriptEngineAction->setEnabled(true);
			break; 
		}	
	}
}

/*! \brief Returns the path to the current active document.
 *
 * This function returns the path to the document that is currently opened and active.
 * @return a String value holding a path to the requested document.
 */
QString MainWindow::getSelectedScriptFileLocation()
{
	return DocManager->getFilePath(activeMdiChild());
}

/*! \brief Returns the path to the StimulGL Root directory.
 *
 * This function returns the path to the StimulGL Root directory, where the binary is running in.
 * @return a String value holding the path to the Root directory.
 */
QString MainWindow::getApplicationRootDirPath() 
{
	return QDir(QCoreApplication::applicationDirPath()).absolutePath();
}

/*! \brief Returns the filename of the current active document.
 *
 * This function returns the filename of the document that is currently opened and active.
 * @return a String value holding the filename of the requested document.
 */
QString MainWindow::getSelectedScriptFileName()
{
	return DocManager->getFileName(activeMdiChild(),true);
}

/*! \brief Returns the requested Environment Variable.
 *
 * This function can return a Environment Variable value by providing the name of the variable.
 * @param strName a String value holding the name of the requested Environment Variable.
 * @return a String value holding the value of the requested Environment Variable.
 */
QString MainWindow::getEnvironmentVariabele(QString strName) 
{
	return QProcessEnvironment::systemEnvironment().value(strName);
}

/*! \brief Closes the current active document.
 *
 * This function closes the document that is currently opened and active.
 * @param bAutoSaveChanges a boolean value determing whether the document should first save unsaved changes or not.
 */
void MainWindow::closeSelectedScriptFile(bool bAutoSaveChanges)
{
	closeSubWindow(bAutoSaveChanges);
}

/*! \brief Executes the current active document.
 *
 * This function executes the current active document (file that is opened and active).
 */
void MainWindow::executeScript()
{
	QAction *action = qobject_cast<QAction *>(sender());
	StimulGLScriptRunMode = (GlobalApplicationInformation::ScriptRunMode)action->data().toInt();
	QMdiSubWindow *currentActiveWindow = activeMdiChild();
	QString strFileName = DocManager->getFileName(currentActiveWindow);

	switch (DocManager->getDocType(currentActiveWindow))
	{
		case GlobalApplicationInformation::DOCTYPE_QSCRIPT:
		{ 
			break; 
		}
		case GlobalApplicationInformation::DOCTYPE_JAVASCRIPT:
		{ 
			break; 
		}		
		case GlobalApplicationInformation::DOCTYPE_SVG: 
		{
			CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentActiveWindow));
			if ((tmpCustomQsciScintilla) && (tmpCustomQsciScintilla->isModified()))
			{
				QMessageBox::StandardButton ret;
				ret = QMessageBox::warning(this, tr("Save Changes?"),
					tr("'%1' has unsaved changes that need to be saved to execute\n"
					"Do you want to save your changes first?")
					.arg(QFileInfo(activeMdiChildFilePath()).fileName()),
					QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		 		if (ret == QMessageBox::Save)
				{
					save();
					SVGPreviewer->openFile(DocManager->getFileName(currentActiveWindow));
					//SVGPreviewer->openFile(activeMdiChildFilePath()); abov code worked, this one didn't received an empty path...
					SVGPreviewer->showMaximized();
					//SVGPreviewer->showFullScreen();
				}
			}
			else 
			{
				SVGPreviewer->openFile(DocManager->getFileName(currentActiveWindow));
				//SVGPreviewer->openFile(activeMdiChildFilePath()); abov code worked, this one didn't received an empty path...
				SVGPreviewer->showMaximized();
				//GraphScene = new QGraphicsScene();
				//QGraphicsRectItem *rect = GraphScene->addRect(QRectF(0, 0, 100, 100));
				//QGraphicsItem *item = GraphScene->itemAt(50, 50);
				//GraphView = new QGraphicsView(GraphScene);
				//GraphView->showMaximized();
			}
			return;
			break;
		}
		case GlobalApplicationInformation::DOCTYPE_PLUGIN_DEFINED:
		{
			clearDebugger();
			//QTime t;
			QDir::setCurrent(getSelectedScriptFileLocation());

			QString strDocumentContent = "";
			CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentActiveWindow));
			if (tmpCustomQsciScintilla)
				strDocumentContent = tmpCustomQsciScintilla->text();
			QString strExtension = QFileInfo(DocManager->getFileName(currentActiveWindow)).completeSuffix();
			QString strDocHandlerSlotName = "";
			QObject* pluginObject = DocManager->getKnownDocumentFileHandlerInformation(DocManager->getKnownDocumentFileHandlerIndex(strExtension),strDocHandlerSlotName);
			
			if(pluginObject)
			{
				if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(strDocHandlerSlotName.toLatin1())) == -1))//Is the slot present?
				{
					//Invoke the slot
					bool bResult = false;
					bool bRetVal = false;
					QString shortSignature = strDocHandlerSlotName.left(strDocHandlerSlotName.indexOf("("));
					bResult = QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(shortSignature.toLatin1()),Qt::DirectConnection, Q_RETURN_ARG(bool,bRetVal), Q_ARG(QString,strDocumentContent), Q_ARG(QString,getSelectedScriptFileLocation()));				
					//if(bResult)
					//{
					//	break;
					//}
					//else
					//{
					//	break;
					//}
				}
			}
			return;
			break;
		}
		default: //none of them
		{ 
			return;
			break;
		}
	}

	clearDebugger();
	QTime t;
	QDir::setCurrent(getSelectedScriptFileLocation());

	QString strScriptProgram = "";
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentActiveWindow));
	if (tmpCustomQsciScintilla)
		strScriptProgram = tmpCustomQsciScintilla->text();
	if (StimulGLScriptRunMode == GlobalApplicationInformation::Debug)
	{
		if (!AppScriptEngine->eng->canEvaluate(strScriptProgram))
		{
			write2OutputWindow("... DebugScript -> Script syntax error ...");
		}
	}
	QScriptSyntaxCheckResult syntaxChkResult = AppScriptEngine->eng->checkSyntax(strScriptProgram);
	bool bSyntaxValidated = false;
	if (syntaxChkResult.state() == QScriptSyntaxCheckResult::Error)
	{
		write2OutputWindow("... Invalid Script Syntax, the program contains a syntax error ...");
	}
	else if (syntaxChkResult.state() == QScriptSyntaxCheckResult::Intermediate)
	{
		write2OutputWindow("... Invalid Script Syntax, the program is incomplete ...");
	}
	else if(syntaxChkResult.state() == QScriptSyntaxCheckResult::Valid)
	{
		write2OutputWindow("... Valid Script Syntax ...");
		bSyntaxValidated = true;
	}
	if(!bSyntaxValidated)
	{
		write2OutputWindow("... Script Syntax error at(line " + QString::number(syntaxChkResult.errorLineNumber()) + ", col " + QString::number(syntaxChkResult.errorColumnNumber()) + "): " + syntaxChkResult.errorMessage() + " ...");
		CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentActiveWindow));
		if (tmpCustomQsciScintilla)
			tmpCustomQsciScintilla->setCursorPosition(syntaxChkResult.errorLineNumber()-1,syntaxChkResult.errorColumnNumber());
	}
	else
	{
		write2OutputWindow("... Script started Evaluating on " + t.currentTime().toString() + "...");
		t.start();

		QScriptValue result;

		CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(currentActiveWindow));
		if (tmpCustomQsciScintilla)
			result = executeScriptContent(tmpCustomQsciScintilla->text());
		QString strResult;
		strResult = result.toString();
		if (result.isError()) 
		{
			write2OutputWindow("... Script stopped Evaluating due to error on line " + result.property("lineNumber").toString() + ": --> " + result.toString() + "...");
			if (tmpCustomQsciScintilla)
				tmpCustomQsciScintilla->setCursorPosition(result.property("lineNumber").toInteger()-1,0);
		}
		else
		{
			int timeElapsed = t.elapsed();
			write2OutputWindow("... Total Time(ms): " + QString::number(timeElapsed));
			write2OutputWindow("... Script fully Evaluated on " + t.currentTime().toString() + " ...");
		}
	}	
	AppScriptEngine->eng->collectGarbage();
	outputWindowList->scrollToBottom();
}

QScriptValue MainWindow::executeScriptContent(const QString &sContent)
{
	if(AppScriptEngine)
	{
		if(AppScriptEngine->eng)
		{
			if(pCurrentSetContextState.first != "")
			{
				if(currentScriptEngineContexes.isEmpty() == false)
				{		
					for(int i=0;i<currentScriptEngineContexes.count();i++)
					{
						if(currentScriptEngineContexes.at(i).sContextName == pCurrentSetContextState.first)//Should we overwrite?
						{
							QScriptContext *tmpContext = AppScriptEngine->eng->currentContext();
							tmpContext->setActivationObject(currentScriptEngineContexes[i].activationObject);
							tmpContext->setThisObject(currentScriptEngineContexes[i].thisObject);
							break;
						}
					}
				}
			}
			if (!AppScriptEngine->eng->canEvaluate(sContent))
			{
				QString tmpString = "... Could not evaluate the script object (" + sContent + ")!";
				write2OutputWindow(tmpString);
				qDebug() << __FUNCTION__ << tmpString;
				return NULL;
			}
			else
			{
				QScriptValue tmpScriptValue = AppScriptEngine->eng->evaluate(sContent);
				//QScriptValue tmpScriptValue = currentScriptEngine->evaluate(adjScriptContextStatement);
				if(AppScriptEngine->eng->hasUncaughtException())
				{
					QString tmpString = "... Evaluate script::UncaughtException() (" + sContent + ")!";
					write2OutputWindow(tmpString);
					qDebug() << __FUNCTION__ << tmpString;
				}
				//sScriptContextReturnValue = tmpScriptValue.toVariant();
				return tmpScriptValue;
			}
		}
	}
	return NULL;
}

/*! \brief Forces the script engine to perform a garbage collection.
 *
 * This function forces the script engine to perform a garbage collection and is therefore a safe and good practice to execute as last command before ending the script.
 */
void MainWindow::cleanupScript()
{
	QTimer::singleShot(10, this, SLOT(abortScript()));
	emit CleanUpScriptExecuted();
}

/*! \brief Forces the StimulGL User Interface to become activated.
 *
 * This function forces the the StimulGL User Interface to become activated. An active window is a visible top-level window that has the keyboard input focus.
 * It is the same operation as clicking the mouse on the title bar of a top-level window.
 */
void MainWindow::activateMainWindow()
{
#ifdef Q_WS_WIN
	::SetWindowPos(effectiveWinId(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	::SetWindowPos(effectiveWinId(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
#endif
	this->raise();
	this->activateWindow();
}

void MainWindow::abortScript()
{
	AppScriptEngine->eng->collectGarbage();
}

void MainWindow::openOptionsDialog()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	int returnVal;
	OptionPage MainOptionPage(this, globAppInfo);
	returnVal = MainOptionPage.exec();

	switch (returnVal) {
	case QDialog::Accepted:
		// Ok was clicked
		parseRemainingGlobalSettings();
		break;
	case QDialog::Rejected:
		// Cancel was clicked
		break;
	default:
		// should never be reached
		break;
	}		
	return;
}

void MainWindow::aboutStimulGL()
{
	aboutQTDialog aboutQTDlg(globAppInfo->getTitle(),pluginFileNames,MainAppInfo::pluginsDirPath(), pluginFileNames, this);
	aboutQTDlg.exec();
}

void MainWindow::setStartupFiles(const QString &path)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	startUpFiles = path.split(";");
}


/*! \brief Opens one or more files.
 *
 *  This function can open one or more files.
 * @param fileToLoad a String containing a single path to a file that should be loaded.
 * @param filesToLoad a String Array containing multiple Strings containing the paths to the files that should be loaded.
 */
void MainWindow::openFiles(const QString &fileToLoad, const QStringList &filesToLoad)
{
	QStringList fileNames;
	//QString fileExtList("QT Script files (*.qs);;"
	//					"SVG files (*.svg *.svgz *.svg.gz);;"
	//					"Any files (*)");
	if ((fileToLoad.isNull()) && (filesToLoad.count() == 0))
	{
		QString sPath = m_currentPath;
		if(sPath.isEmpty())
			sPath = MainAppInfo::appExampleDirPath();			
		fileNames = QFileDialog::getOpenFileNames(this, tr("Open File(s)"),sPath,DocManager->getKnownFileExtensionList());
	}
	
	if (!fileToLoad.isNull())
	{
		fileNames.append(fileToLoad);
	}

	if (filesToLoad.count() > 0)
	{
		foreach (QString file, filesToLoad)
		{
			fileNames.append(file);
		}
	}


	foreach (QString fileName, fileNames)
	{
		if (!fileName.isEmpty()) 
		{
			QFile file(fileName);
			if (!file.exists()) {
				QMessageBox::critical(this, tr("Open File"),
					QString("Could not open file '%1'.").arg(fileName));

				//m_outlineAction->setEnabled(false);
				//m_backgroundAction->setEnabled(false);
				return;
			}
			parseFile(file);
			//m_outlineAction->setEnabled(true);
			//m_backgroundAction->setEnabled(true);
			//resize(SVGPreviewer->sizeHint() + QSize(80, 80 + menuBar()->height()));
		}
	}
}

bool MainWindow::parseFile(const QFile &file)
{
	QMdiSubWindow *existing = findMdiChild(file.fileName());
	if (existing) {
		mdiArea->setActiveSubWindow(existing);
		statusBar()->showMessage(tr("File already opened"), 2000);
		return true;
	}
	QFileInfo fileInfo(file);
	QString fileExtension;
	fileExtension = fileInfo.suffix();
	GlobalApplicationInformation::DocType tempDocType = DocManager->getDocType(fileExtension);
	int DocIndex;
	newDocument(tempDocType,DocIndex,fileExtension);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (DocManager->loadFile(DocIndex,file.fileName()))
	{	
		setCurrentFile(file.fileName());
		statusBar()->showMessage(tr("File loaded"), 2000);
	} 
	else 
	{
		mdiArea->removeSubWindow(findMdiChild(file.fileName()));
		statusBar()->showMessage(tr("Could not load File"), 2000);
		QApplication::restoreOverrideCursor();
		return false;
	}
	QApplication::restoreOverrideCursor();
	return true;
}

void MainWindow::setRenderer()
{
	if (globAppInfo->checkRegistryInformation(REGISTRY_RENDERTYPE))	
	{
		SvgView::RendererType type;
		type = (SvgView::RendererType)globAppInfo->getRegistryInformation(REGISTRY_RENDERTYPE).toInt();
		switch (type)
		{
		case SvgView::Native://Native
			{
				SVGPreviewer->setHighQualityAntialiasing(false);
				SVGPreviewer->setRenderer(SvgView::Native);
				break;
			}
		case SvgView::OpenGL://OpenGL
			{
				if (globAppInfo->checkRegistryInformation(REGISTRY_HQANTIALIAS))
				{
					SVGPreviewer->setHighQualityAntialiasing(globAppInfo->getRegistryInformation(REGISTRY_HQANTIALIAS).toInt());
				}
				else
				{
					SVGPreviewer->setHighQualityAntialiasing(false);
				}
				SVGPreviewer->setRenderer(SvgView::OpenGL);
				break;
			}
		case SvgView::Image://Image
			{
				SVGPreviewer->setHighQualityAntialiasing(false);
				SVGPreviewer->setRenderer(SvgView::Image);
				break;
			}
		}
	}
	else //key doesn't exist, default value!
	{
		SVGPreviewer->setRenderer(SvgView::Native);
	}
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	if (fileName.isEmpty())
		setWindowTitle(globAppInfo->getTitle());//MainAppInfo::MainProgramTitle());
	else if (fileName == MainAppInfo::UntitledDocName())
	{
		setWindowTitle(tr("%1 - %2").arg(globAppInfo->getTitle()).arg(MainAppInfo::UntitledDocName()));
	}	
	else if (!fileName.startsWith(":/")) 
	{
		m_currentPath = fileName;
		setWindowTitle(tr("%1 - %2").arg(globAppInfo->getTitle()).arg(strippedName(m_currentPath)));
		updateRecentFileList(fileName);
	}
}

void MainWindow::updateRecentFileList(const QString &fileName)
{
	QStringList files;
	if(globAppInfo->checkRegistryInformation(REGISTRY_RECENTFILELIST))
		files = globAppInfo->getRegistryInformation(REGISTRY_RECENTFILELIST).toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	globAppInfo->setRegistryInformation(REGISTRY_RECENTFILELIST,files,"stringlist");

	foreach (QWidget *widget, QApplication::topLevelWidgets()) {
		MainWindow *mainWin = qobject_cast<MainWindow *>(widget);
		if (mainWin)
			mainWin->updateRecentFileActions();
	}
}

void MainWindow::openRecentFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action)
		openFiles(action->data().toString());
}

void MainWindow::updateRecentFileActions()
{
	QStringList files;
	if(globAppInfo->checkRegistryInformation(REGISTRY_RECENTFILELIST))
		files = globAppInfo->getRegistryInformation(REGISTRY_RECENTFILELIST).toStringList();

	int numRecentFiles = qMin(files.size(), (int)MaxRecentFiles);

	for (int i = 0; i < numRecentFiles; ++i) {
		QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(files[i]));
		recentFileActs[i]->setText(text);
		recentFileActs[i]->setData(files[i]);
		recentFileActs[i]->setVisible(true);
	}
	for (int j = numRecentFiles; j < MaxRecentFiles; ++j)
		recentFileActs[j]->setVisible(false);

	separatorAct->setVisible(numRecentFiles > 0);
}

QString MainWindow::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}

void MainWindow::newFile()
{
	QAction *action = qobject_cast<QAction *>(sender());
	GlobalApplicationInformation::DocType docType;
	if (action)
	{docType = (GlobalApplicationInformation::DocType)action->data().toInt();}
	else
	{docType = GlobalApplicationInformation::DOCTYPE_UNDEFINED;}	

	int DocIndex;
	newDocument(docType,DocIndex);
	setCurrentFile(MainAppInfo::UntitledDocName());
	//	setupSyntaxHighlighting(child,tempFileType);
	statusBar()->showMessage(tr("New File created"), 2000);
}

void MainWindow::newDocument(const GlobalApplicationInformation::DocType &docType, int &DocIndex, const QString &strExtension)
{
	DocIndex = 0;
	//QsciScintilla *newChild = qobject_cast<QsciScintilla *>(DocManager->add(docType,DocIndex,strExtension));
	QWidget *newChild = DocManager->add(docType,DocIndex,strExtension);
	QMdiSubWindow *subWindow = mdiArea->addSubWindow(newChild);
	subWindow->setAttribute(Qt::WA_DeleteOnClose);
	DocManager->setSubWindow(DocIndex,subWindow);
	connect(newChild, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
	connect(newChild, SIGNAL(copyAvailable(bool)),	copyAction, SLOT(setEnabled(bool)));
	connect(newChild, SIGNAL(cursorPositionChanged(int, int)), this, SLOT(CursorPositionChanged(int, int)));
	newChild->showMaximized();
}

void MainWindow::setupStatusBar()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	StatusPositionLabel = new QLabel("");
	StatusNameLabel = new QLabel("");
	StatusLinesLabel = new QLabel("");
	StatusPositionLabel->setToolTip(QObject::tr("Cursor position"));
	StatusNameLabel->setToolTip(QObject::tr("Document name"));
	StatusLinesLabel->setToolTip(QObject::tr("Number of Lines"));
	this->statusBar()->addPermanentWidget(StatusPositionLabel);
	this->statusBar()->addPermanentWidget(StatusNameLabel);
	this->statusBar()->addPermanentWidget(StatusLinesLabel);
}

void MainWindow::NumberOfLinesChanged(int nrOfLines)
{
	StatusLinesLabel->setText(QString("Lines: %1").arg(nrOfLines));
}

void MainWindow::save()
{
	QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
	QString tmpFileName = DocManager->getFileName(tmpMdiChildPointer);
	if (tmpMdiChildPointer)
	{
		if (tmpFileName == "")
		{
			return saveAs();
		}
		else
		{
			if (DocManager->saveFile(tmpMdiChildPointer,tmpFileName))
			{
				statusBar()->showMessage(tr("File saved"), 2000);
				return;
			}
			return;
		}
	}
}

void MainWindow::saveAs()
{
	QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
	if (tmpMdiChildPointer)
	{
		//QString selExt = "SVG files (*.svg *.svgz *.svg.gz)";
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),DocManager->getFileName(tmpMdiChildPointer));//,DocManager->getKnownFileExtensionList(),&selExt);
		if (fileName.isEmpty())
		{
			return;
		}
		else
		{
			if (DocManager->saveFile(tmpMdiChildPointer,fileName))
			{
				setCurrentFile(fileName);
				statusBar()->showMessage(tr("File saved"), 2000);
				return;
			}
			return;
		}
	}
}

void MainWindow::cut()
{
	QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(tmpMdiChildPointer));
	if (tmpCustomQsciScintilla)
		tmpCustomQsciScintilla->cut();
}

void MainWindow::copy()
{
	QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(tmpMdiChildPointer));
	if (tmpCustomQsciScintilla)
		tmpCustomQsciScintilla->copy();
}

void MainWindow::paste()
{
	QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(tmpMdiChildPointer));
	if (tmpCustomQsciScintilla)
		tmpCustomQsciScintilla->paste();
}

void MainWindow::lineComment()
{
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpCustomQsciScintilla)
		tmpCustomQsciScintilla->toggleLineComment();
}
void MainWindow::blockComment()
{
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpCustomQsciScintilla)
		tmpCustomQsciScintilla->toggleBlockComment();
}
void MainWindow::updateWindowMenu()
{
	windowMenu->clear();
	windowMenu->addAction(closeAction);
	windowMenu->addAction(closeAllAction);
	windowMenu->addSeparator();
	windowMenu->addAction(tileAction);
	windowMenu->addAction(cascadeAction);
	windowMenu->addSeparator();
	windowMenu->addAction(nextAction);
	windowMenu->addAction(previousAction);

	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	separatorAct->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i) 
	{
		QMdiSubWindow *child = qobject_cast<QMdiSubWindow *>(windows.at(i));
		QString text = QFileInfo(DocManager->getFileName(child)).fileName();
		if (text == "")
		{
			text = MainAppInfo::UntitledDocName();
		}
		if (i < 9) {
			text = tr("&%1 %2").arg(i + 1).arg(text);
		} else {
			text = tr("%1 %2").arg(i + 1).arg(text);
		}
		QAction *action  = windowMenu->addAction(text);
		action->setCheckable(true);
		//action ->setChecked(child == DocManager->getDocHandler(activeMdiChild()));
		action->setChecked(windows.at(i) == activeMdiChild());
		connect(action, SIGNAL(triggered()), windowMapper, SLOT(map()));
		windowMapper->setMapping(action, windows.at(i));
	}
}

void MainWindow::updateMarkersMenu()
{
	if (markersMenu) 
	{
		markersMenu->clear();

		CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
		markersMenu->addAction(addRemMarkerAction);
		markersMenu->addAction(nextMarkerAction);
		markersMenu->addAction(prevMarkerAction);
		markersMenu->addAction(remAllMarkerAction);

		if ( tmpSci && !tmpSci==0 ) 
		{
			QList<int> list = tmpSci->markers();
			if ( !list.isEmpty() ) {
				markersMenu->addSeparator();
				foreach (int line, list) {
					QString lineStr = tmpSci->getLine(line).simplified();
					if ( lineStr.length() > 40 )
						lineStr = lineStr.left(40) + " ...";
					markersMenu->addAction(QString("%1: %2").arg(line + 1).arg(lineStr), this, SLOT(gotoMarker()));
				}
			}
		}
	}
}

void MainWindow::gotoMarker() 
{
	QAction* a = qobject_cast<QAction*>(sender());
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if ( tmpSci && !tmpSci == 0 && a ) {
		int line = a->text().section(':', 0, 0).toInt();
		tmpSci->gotoLine(line - 1);
	}
}

void MainWindow::setupToolBars()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->addAction(newScriptAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->addAction(cutAction);
	editToolBar->addAction(copyAction);
	editToolBar->addAction(pasteAction);

	toolsToolBar = addToolBar(tr("Tools"));
	toolsToolBar->addAction(runScriptAction);
	//toolsToolBar->addAction(debugScriptAction);
	toolsToolBar->addAction(abortScriptAction);
	toolsToolBar->addAction(restartScriptEngineAction);
}

void MainWindow::parseRemainingGlobalSettings()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	setRenderer();
	configureDebugger();
}

void MainWindow::RecoverLastScreenWindowSettings()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if(globAppInfo->checkRegistryInformation(REGISTRY_MAINWINDOWPOS))
	{
		QPoint tmpPoint = globAppInfo->getRegistryInformation(REGISTRY_MAINWINDOWPOS).toPoint();
		this->move(tmpPoint);
	}
	if(globAppInfo->checkRegistryInformation(REGISTRY_MAINWINDOWSIZE))
	{
		QSize tmpSize = globAppInfo->getRegistryInformation(REGISTRY_MAINWINDOWSIZE).toSize();
		this->resize(tmpSize);
	}
	if(globAppInfo->checkRegistryInformation(REGISTRY_DEBUGWINDOWWIDTH))
	{
		int tmpWidth = globAppInfo->getRegistryInformation(REGISTRY_DEBUGWINDOWWIDTH).toInt();
		setDockSize(debugLogDock,tmpWidth,debugLogDock->height());
	}
}

void MainWindow::returnToOldMaxMinSizes()
{
	debugLogDock->setMinimumSize(oldDockMinSize);
	debugLogDock->setMaximumSize(oldDockMaxSize);
}

void MainWindow::setDockSize(QDockWidget *dock, int setWidth, int setHeight)
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	oldDockMaxSize=dock->maximumSize();
	oldDockMinSize=dock->minimumSize();

	if (setWidth>=0)
		if (dock->width()<setWidth)
			dock->setMinimumWidth(setWidth);
		else dock->setMaximumWidth(setWidth);
		if (setHeight>=0)
			if (dock->height()<setHeight)
				dock->setMinimumHeight(setHeight);
			else dock->setMaximumHeight(setHeight);

			QTimer::singleShot(1, this, SLOT(returnToOldMaxMinSizes()));
}

bool MainWindow::configureDebugger()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	if(globAppInfo->checkRegistryInformation(REGISTRY_OPENINEXTERNALDEBUGGER))
		AppScriptEngine->ConfigureDebugger((bool)globAppInfo->getRegistryInformation(REGISTRY_OPENINEXTERNALDEBUGGER).toInt());
	else//default
		AppScriptEngine->ConfigureDebugger(false);
	return true;
}

void MainWindow::writeMainWindowSettings()
{
	if(StimulGLFlags & GlobalApplicationInformation::VerboseMode)
		qDebug() << "Verbose Mode: " << __FUNCTION__;
	globAppInfo->setRegistryInformation(REGISTRY_MAINWINDOWPOS, pos(), "point");
	globAppInfo->setRegistryInformation(REGISTRY_MAINWINDOWSIZE, size(), "size");
	globAppInfo->setRegistryInformation(REGISTRY_DEBUGWINDOWWIDTH, debugLogDock->width(), "int");
}

bool MainWindow::closeSubWindow(bool bAutoSaveChanges)
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action == 0)//direct function call, exit program
	{
		QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
		for (int i = 0; i < windows.size(); ++i) {
			mdiArea->setActiveSubWindow(windows.at(i));
			if (!DocManager->maybeSave(windows.at(i),bAutoSaveChanges)) 
			{
				return false;
			}
		}
		DocManager->setAllUnmodified();//to prevent a maybesave() check loop
		return true;
	}
	if (action->data().toString() == "Close")//From menu(Close)
	{
		QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
		if (!DocManager->maybeSave(tmpMdiChildPointer,bAutoSaveChanges)) 
		{
			return false;
		}
		DocManager->remove(tmpMdiChildPointer);
		mdiArea->closeActiveSubWindow();
		return true;
	}
	else if (action->data().toString() == "CloseAll")//From menu(Close All)
	{
		QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
		for (int i = 0; i < windows.size(); ++i) {
			mdiArea->setActiveSubWindow(windows.at(i));
			if (!DocManager->maybeSave(windows.at(i),bAutoSaveChanges)) 
			{
				return false;
			}
		}
		DocManager->setAllUnmodified();//to prevent a maybesave() check loop
		mdiArea->closeAllSubWindows();
		return true;
	}
	else //This must be called from the script?
	{
		QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
		if (!DocManager->maybeSave(tmpMdiChildPointer,bAutoSaveChanges)) 
		{
			return false;
		}
		DocManager->remove(tmpMdiChildPointer);
		mdiArea->closeActiveSubWindow();
		return true;
	}
	return false;
}

QString MainWindow::activeMdiChildFilePath()
{
	return mdiArea->activeSubWindow()->windowFilePath();
}

QMdiSubWindow *MainWindow::activeMdiChild()
{
	return mdiArea->activeSubWindow();
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
	QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();
	foreach (QMdiSubWindow *window, mdiArea->subWindowList()) 
	{
		if (DocManager->getFileName(window) == canonicalFilePath)
			return window;
	}
	return 0;
}

void MainWindow::jumpToMatchingBrace()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->moveToMatchingBrace();
}

void MainWindow::selectToMatchingBrace()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->selectToMatchingBrace();
}

void MainWindow::duplicateLine()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
	{
		if(tmpSci->hasSelectedText())
			tmpSci->SendScintilla(QsciScintilla::SCI_SELECTIONDUPLICATE);
		else
			tmpSci->SendScintilla(QsciScintilla::SCI_LINEDUPLICATE);
	}
}

void MainWindow::moveLineUp()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
	{
		tmpSci->SendScintilla(QsciScintilla::SCI_LINETRANSPOSE);
		tmpSci->SendScintilla(QsciScintilla::SCI_LINEUP);
	}
}

void MainWindow::deleteCurrentLine()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->SendScintilla(QsciScintilla::SCI_LINEDELETE);
}

void MainWindow::toUpperCase()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->SendScintilla(QsciScintilla::SCI_UPPERCASE);
}

void MainWindow::toLowerCase()
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->SendScintilla(QsciScintilla::SCI_LOWERCASE);
}

void MainWindow::print()
{
#ifndef QT_NO_PRINTDIALOG
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
	{
		if (tmpSci->print())
		{
			statusBar()->showMessage(tr("Printing..."), 2000);
		}
//	QTextDocument *document = activeMdiChild()->sciObject->document();
//	QPrinter printer;
//
//	QPrintDialog *dlg = new QPrintDialog(&printer, this);
//	if (dlg->exec() != QDialog::Accepted)
//		return;
//	document->print(&printer);
//	
	}
#endif
}

void MainWindow::toggleMarker() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->toggleMarker();
	//initMarkersMenu();
}

void MainWindow::nextMarker() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->nextMarker();
}

void MainWindow::prevMarker() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->prevMarker();
}

void MainWindow::removeAllMarkers() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
		tmpSci->removeAllMarkers();
}

void MainWindow::goToLine() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
	{
		bool ok = false;
		int line = QInputDialog::getInt(mdiArea, tr("Go to line"), 
			tr("Go to line") + QString(" (1 - %1):").arg(tmpSci->lineCount()), 
			1, 1, tmpSci->lineCount(), 1, &ok);
		if ( ok )
			tmpSci->gotoLine(line - 1);
	}
}

void MainWindow::findImpl(bool bReplace, bool useParams, QString strFindString, QString strReplaceString, DocFindFlags findFlags, bool bReplaceAll) 
{
	QMdiSubWindow *tmpMdiChildPointer = activeMdiChild();
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(tmpMdiChildPointer));
	if (tmpSci)
	{	
		DocFindFlags flags(bReplace);
		QString str1, str2;
		int line1, col1, line2, col2;
		tmpSci->getSelection(&line1, &col1, &line2, &col2);
		if ( line1 == line2 ) {
			if ( col1 != col2 ) {
				str1 = tmpSci->selectedText();
			}
			else {
				str1 = tmpSci->wordUnderCursor();
			}
		}
		if(useParams)
		{
			if ( bReplace ){//findFlags.replace ) {
				tmpSci->replace(strFindString,strReplaceString, findFlags, bReplaceAll);
			}
			else {
				tmpSci->find(strFindString, findFlags);
			}
		}
		else
		{
			if ( DocManager->getFindParams(tmpMdiChildPointer,str1, str2, flags) ) {
				if ( flags.replace ) {
					tmpSci->replace(str1, str2, flags, bReplaceAll);
				}
				else {
					tmpSci->find(str1, flags);
				}
			}	
		}
	}
}

/*! \brief Searches for a provided String inside the currently active document.
 *
 * This function searches for a provided String of text inside the currently active document. Furthermore this action can be specified with special flags.
 * @param useParams a boolean value that determines whether the optional provided parameters should be used. If useParams is false then a Search window appears allowing the user to
 * make further changes. If some text from the document was selected than this String is automatically used for the find String, otherwise the word around the cursor position is used.
 * If useParams is true then no Search window appears and the optional provided parameters are used for the search action, the first found occurrence (starting from the cursor position) 
 * of the find result is then automatically selected.
 * @param strFindString a String holding the text value to use.
 * @param DocFindFlags a String holding the text value to use, you can create this structure in the script like: 
 * \code 
 * var varName=DocFindFlags; 
 * varName.backwards = true; 
 * \endcode 
 * see _DocFindFlags.
 */
void MainWindow::find(bool useParams, QString strFindString, DocFindFlags findFlags) 
{
	findImpl(false,useParams,strFindString,"",findFlags,false);
}

/*! \brief Searches and replaces a provided String inside the currently active document.
 *
 * This function searches and replaces a provided String of text inside the currently active document. Furthermore this action can be specified with special flags.
 * @param bReplaceAll a boolean value that determines whether all occurrences of the found String should be replaced.
 * @param useParams a boolean value that determines whether the optional provided parameters should be used. If useParams is false then a Replace window appears allowing the user to
 * make further changes. If some text from the document was selected than this String is automatically used for the find String, otherwise the word around the cursor position is used.
 * If useParams is true then no Search window appears and the optional provided parameters are used for the search action, the first found occurrence (starting from the cursor position) 
 * of the find result is then automatically selected.
 * @param strFindString a String holding the String value to search for.
 * @param strReplaceString a String holding the text value to replace the found String strFindString with.
 * @param DocFindFlags a String holding the text value to use, you can create this structure in the script like: 
 * \code 
 * var varName=DocFindFlags; 
 * varName.backwards = true; 
 * \endcode 
 * see _DocFindFlags.
 */
void MainWindow::replace(bool bReplaceAll, bool useParams, QString strFindString, QString strReplaceString, DocFindFlags findFlags) 
{
	findImpl(true,useParams,strFindString,strReplaceString,findFlags,bReplaceAll);
}

/*! \brief Searches for the last provided String inside the currently active document.
 *
 * This function searches (forwards) for the last provided String of text inside the currently active document starting from the current cursor position.
 */
void MainWindow::findNext() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(DocManager->getDocHandler(activeMdiChild()));
	if (tmpSci)
	{	
		QString lastText = DocManager->lastFindText();
		if ( lastText.isEmpty() ) {
			find();
		}
		else {
			DocFindFlags flags = DocManager->lastFlags();
			flags.replace = false;
			flags.backwards = false;
			tmpSci->find(lastText, flags);
		}
	}
}

/*! \brief Searches for the last provided String inside the currently active document.
 *
 * This function searches (backwards) for the last provided String of text inside the currently active document starting from the current cursor position.
 */
void MainWindow::findPrev() 
{
	CustomQsciScintilla *tmpSci = qobject_cast<CustomQsciScintilla*>(activeMdiChild());
	if (tmpSci)
	{
		QString lastText = DocManager->lastFindText();
		if ( lastText.isEmpty() ) {
			find();
		}
		else {
			DocFindFlags flags = DocManager->lastFlags();
			flags.replace = false;
			flags.backwards = true;
			tmpSci->find(lastText, flags);
		}
	}
}
