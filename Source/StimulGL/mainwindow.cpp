/****************************************************************************
**
****************************************************************************/

#include "mainwindow.h"
#include "mainappinfo.h"
#include "StimulGL.h"
#include "svgview.h"
#include "aboutqtdialog.h"  
#include "sleepthread.h"
#include "optionpage.h"
#include <QPluginLoader>
#include <QFileInfo>

#include "..\Plugins\ParallelPortDevice\parallelport.h"
#include "..\Plugins\ExperimentManager\ExperimentManager.h"

MainWindow::MainWindow() : QMainWindow(), SVGPreviewer(new SvgView)
{
	PluginsFound = false;
	DevicePluginsFound = false;
	ExtensionPluginsFound = false;

#ifndef QT_NO_DEBUG_OUTPUT	
	mainLogFile.open(MainAppInfo::appLogFilePath().toStdString().c_str(), std::ios::app);
	qInstallMsgHandler(MyOutputHandler);
	qDebug("SimulGL Main Constructor.");
#endif
//When compiling your final application, be sure to add the flags -DQT_NO_DEBUG and -DQT_NO_DEBUG_OUTPUT.  
//These will reduce the application size and ensure that the logfile isn't used. 
//Alternately, you can keep NO_DEBUG_OUTPUT out of the makefile if you still want the logfile.
}

MainWindow::~MainWindow()
{
#ifndef QT_NO_DEBUG_OUTPUT
	mainLogFile.close();	
#endif
}

void MainWindow::DebugcontextMenuEvent(const QPoint &pos)
{
	QMenu menu(this);
	menu.addAction(clearDebuggerAction);
	menu.exec(outputWindowList->viewport()->mapToGlobal(pos));
}

void MainWindow::setupContextMenus()
{
	outputWindowList->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(outputWindowList, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(DebugcontextMenuEvent(QPoint)));
}

bool MainWindow::initialize(MainAppInfo::MainProgramModeFlags mainFlags)
{
	//Default					= 0x00000,
	//DisablePlugins			= 0x00001,
	//DisableSplash				= 0x00002
	//QWaitCondition sleep;
	StimulGLFlags = mainFlags;
	AppScriptStatus = MainAppInfo::ActiveScriptMode::NoScript;
	if (StimulGLFlags.testFlag(MainAppInfo::DisableSplash) == false)
	{
		QPixmap pixmap(":/Resources/splash.png");
		MainSplashScreen = new QSplashScreen(pixmap);
		MainSplashScreen->show();
		showSplashMessage("Initializing Program...");
	}
	initAndParseGlobalSettings();//See also parseRemainingGlobalSettings()!
	showSplashMessage("Setup User Interface...");
	setupMDI();
	setWindowTitle(MainAppInfo::MainProgramTitle());
	setUnifiedTitleAndToolBarOnMac(true);
	createDockWindows();
	setAppDirectories();
	setupScriptEngine();
	setupStatusBar();
	createDefaultMenus();
	setupDynamicPluginMenus();
	setupHelpMenu();
	setupToolBars();
	updateMenuControls(0);
	parseRemainingGlobalSettings();
	setupContextMenus();
	if (startUpFiles.count()> 0) { openFiles(NULL,startUpFiles);}
	if (StimulGLFlags.testFlag(MainAppInfo::DisableSplash)==false)
	{
		MainSplashScreen->finish(this);
	}
#ifdef Q_OS_WIN
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
	return true;
}

void MainWindow::showSplashMessage(const QString message)
{
	if (StimulGLFlags.testFlag(MainAppInfo::DisableSplash) == false)
	{
		MainSplashScreen->showMessage(message,Qt::AlignBottom ,Qt::white);
	}	
}
void MainWindow::initAndParseGlobalSettings()//See also ParseRemainingGlobalSettings()!
{
	settings = new QSettings(MainAppInfo::MainOrganizationName(),MainAppInfo::MainProgramName());
	settings->beginGroup("General");
	if (settings->contains("DoNotLoadScriptExtension")) 
	{
		if (Qt::CheckState(settings->value("DoNotLoadScriptExtension").toInt())==0)
		{
			DoNotLoadScriptBindings = false;
		}
		else
		{
			DoNotLoadScriptBindings = true;
		}
	}
	else //key doesn't exist, default value here!
	{
		DoNotLoadScriptBindings = false;
	}
	settings->endGroup();
}
void MainWindow::setupMDI()
{
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
	DocManager = new DocumentManager (this);
	connect(DocManager, SIGNAL(NrOfLinesChanged(int)), this, SLOT(NumberOfLinesChanged(int)));
}

QScriptValue myPauseFunction(QScriptContext *ctx, QScriptEngine *eng)
{
	int nWaitTime = ctx->argument(0).toInteger();

	QMutex mutTmp;
	mutTmp.lock();
	QWaitCondition waitCondition;
	waitCondition.wait(&mutTmp, nWaitTime);
	return 0;
}

QScriptValue myThrowScriptErrorFunction(QScriptContext *ctx, QScriptEngine *eng)
{
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
	//DWORD Tone(2);//frequency, duration
	//Tone[0] = 100;
	//Tone[1] = 100;
	//for (int i = 0; i < ctx->argumentCount(); ++i) {
	//if (i > 0)
	//	result.append(" ");
	//result.append(ctx->argument(i).toString());
	//}
	qApp->beep();
	return 0;
}

QScriptValue myIncludeFunction(QScriptContext *ctx, QScriptEngine *eng)
{
	QString fileName = ctx->argument(0).toString();
	QString fileText;
	QFile file(fileName);

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

	ctx->setActivationObject(ctx->parentContext()->activationObject());
	ctx->setThisObject(ctx->parentContext()->thisObject());
	return eng->evaluate(fileText, fileName);
}

QScriptValue myPrintFunction(QScriptContext *context, QScriptEngine *engine)
{
	QString result;
	for (int i = 0; i < context->argumentCount(); ++i) {
		if (i > 0)
			result.append(" ");
		result.append(context->argument(i).toString());
	}
	QScriptValue calleeData = context->callee().data();
	QListWidget *outputObject = qobject_cast<QListWidget*>(calleeData.toQObject());
	outputObject->addItem(result);
	return engine->undefinedValue();//outputWindowList
}

QScriptValue myExitScriptFunction(QScriptContext *context, QScriptEngine *engine)
{
	context->setActivationObject(context->parentContext()->activationObject());
	context->setThisObject(context->parentContext()->thisObject());
	engine->collectGarbage();
	return 0;
}

void MainWindow::setupScriptEngine()
{
	AppScriptEngine = new QTScriptEngine(this);
	if (StimulGLFlags.testFlag(MainAppInfo::DisableAllPlugins) == false)
	{
		if (!DoNotLoadScriptBindings)
		{
			if (!AppScriptEngine->ImportScriptExtensions())
			{
				QMessageBox msgBox(QMessageBox::Warning,"QT Script Extensions","Some of the defined QT Script Extensions could not be loaded!",QMessageBox::Ok);
				msgBox.exec();
			}
		}
	}
	currentRunningScriptID = 0;
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
}

void MainWindow::scriptLoaded(qint64 id)
{
	currentRunningScriptID = id;
	setScriptRunningStatus(MainAppInfo::ActiveScriptMode::Executing);
}

void MainWindow::scriptUnloaded(qint64 id)
{
	currentRunningScriptID = 0;
	setScriptRunningStatus(MainAppInfo::ActiveScriptMode::Pending);
}

void MainWindow::setActiveSubWindow(QWidget *window)
{
	if (!window){return;}
	mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow *>(window));
}

void MainWindow::updateMenuControls(QMdiSubWindow *subWindow)
{
	//write2Debugger("updateMenuControls entered..."); this doesn't happen that often....
	QMdiSubWindow *currentSub = activeMdiChild();
	bool hasMdiChild = ((DocManager->count() > 0) && (currentSub != 0));

	StatusLinesLabel->setHidden(!hasMdiChild);
	StatusPositionLabel->setHidden(!hasMdiChild);
	StatusNameLabel->setHidden(!hasMdiChild);

	addRemMarkerAction->setEnabled(hasMdiChild);
	nextMarkerAction->setEnabled(hasMdiChild);
	prevMarkerAction->setEnabled(hasMdiChild);
	remAllMarkerAction->setEnabled(hasMdiChild);

	clearDebuggerAction->setEnabled(true);

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

	bool hasSelection = (hasMdiChild && (DocManager->getDocHandler(currentSub)->hasSelectedText()));
	cutAction->setEnabled(hasSelection);
	copyAction->setEnabled(hasSelection);
	if (hasMdiChild)
	{
		QString docTitle = strippedName(DocManager->getFileName(currentSub));
		if (docTitle == "")
		{
			docTitle = MainAppInfo::UntitledDocName();
		}		
		setWindowTitle(tr("%1 - %2").arg(MainAppInfo::MainProgramTitle()).arg(docTitle));
		MainAppInfo::DocType d;
		d = DocManager->getDocType(currentSub);
		switch (d)
		{
		case MainAppInfo::DOCTYPE_QSCRIPT:
			{
				//statusBar()->showMessage("DOCTYPE_QSCRIPT");
				if (currentRunningScriptID == 0)
				{
					setScriptRunningStatus(MainAppInfo::ActiveScriptMode::Pending);
				}
				printAction->setEnabled(true);
				break;
			}
		case MainAppInfo::DOCTYPE_SVG:
			{
				//statusBar()->showMessage("DOCTYPE_SVG");
				//setScriptRunningStatus(AppScriptStatus);
				if (currentRunningScriptID == 0)
				{
					setScriptRunningStatus(MainAppInfo::ActiveScriptMode::Pending);
				}
				printAction->setEnabled(true);
				break;
			}
		case MainAppInfo::DOCTYPE_UNDEFINED:
			{
				//statusBar()->showMessage("DOCTYPE_UNDEFINED");
				//setScriptRunningStatus(AppScriptStatus);
				printAction->setEnabled(false);
				break;
			}
		}
		NumberOfLinesChanged(DocManager->getDocHandler(currentSub)->lines());
		StatusNameLabel->setText(QString("%1").arg(docTitle));
	}
	else
	{
		setWindowTitle(tr("%1").arg(MainAppInfo::MainProgramTitle()));
		if (currentRunningScriptID == 0)
		{
			setScriptRunningStatus(MainAppInfo::ActiveScriptMode::NoScript);
		}
		printAction->setEnabled(false);
	}
}

void MainWindow::CursorPositionChanged(int line, int col) 
{
	StatusPositionLabel->setText(tr("Row: %1, Col: %2").arg(line+1).arg(col+1));
}

bool MainWindow::setDefaultGLFormat()
{
	QGLFormat StimulGLQGLFormat;
	StimulGLQGLFormat.setSwapInterval(1); // sync with vertical refresh
	StimulGLQGLFormat.setSampleBuffers(true);
	QGLFormat::setDefaultFormat(StimulGLQGLFormat);
	return true;
}

void MainWindow::createDefaultMenus()
{
	fileMenu = new QMenu(tr("&File"), this);
	editMenu = new QMenu(tr("&Edit"), this);
	markersMenu = new QMenu(tr("&Markers"), this);
	viewMenu = new QMenu(tr("&View"), this);
	scriptMenu = new QMenu(tr("&Script"), this);
	toolsMenu = new QMenu(tr("&Tools"), this);
	windowMenu = new QMenu(tr("&Window"), this);

	newScriptAction = new QAction(QIcon(":/Resources/new.png"),tr("&New QT Script"), this);
	newScriptAction->setStatusTip(tr("Create a new QT Script document"));
	newScriptAction->setData(MainAppInfo::DOCTYPE_QSCRIPT);
	connect(newScriptAction, SIGNAL(triggered()), this, SLOT(newFile()));
	fileMenu->addAction(newScriptAction);

	//newAction = new QAction(QIcon(":/Resources/new.png"),tr("&New Document"), this);
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

	openAction = new QAction(QIcon(":/Resources/open.png"),tr("&Open..."), this);
	openAction->setShortcut(QKeySequence(QKeySequence::Open));
	openAction->setStatusTip(tr("Open an existing file"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openFiles()));
	fileMenu->addAction(openAction);

	saveAction = new QAction(QIcon(":/Resources/save.png"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save the document to disk"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
	fileMenu->addAction(saveAction);

	saveAsAction = new QAction(tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
	fileMenu->addAction(saveAsAction);

	printAction = new QAction(QIcon(":/Resources/print.png"), tr("&Print..."), this);
	printAction->setShortcuts(QKeySequence::Print);
	printAction->setStatusTip(tr("Print the current document"));
	printAction->setEnabled(false);
	connect(printAction, SIGNAL(triggered()), this, SLOT(print()));
	fileMenu->addAction(printAction);

	for (int i = 0; i < MaxRecentFiles; ++i) {
		recentFileActs[i] = new QAction(this);
		recentFileActs[i]->setVisible(false);
		connect(recentFileActs[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
	}

	separatorAct = fileMenu->addSeparator();
	for (int i = 0; i < MaxRecentFiles; ++i)
		fileMenu->addAction(recentFileActs[i]);
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

	cutAction = new QAction(QIcon(":/Resources/cut.png"), tr("Cu&t"), this);
	cutAction->setShortcuts(QKeySequence::Cut);
	cutAction->setStatusTip(tr("Cut the current selection's contents to the clipboard"));
	connect(cutAction, SIGNAL(triggered()), this, SLOT(cut()));
	editMenu->addAction(cutAction);

	copyAction = new QAction(QIcon(":/Resources/copy.png"), tr("&Copy"), this);
	copyAction->setShortcuts(QKeySequence::Copy);
	copyAction->setStatusTip(tr("Copy the current selection's contents to the clipboard"));
	connect(copyAction, SIGNAL(triggered()), this, SLOT(copy()));
	editMenu->addAction(copyAction);

	pasteAction = new QAction(QIcon(":/Resources/paste.png"), tr("&Paste"), this);
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


	//searchAction = new QAction(tr("Search"), this);//QIcon(":/Resources/comment.png")
	//searchAction->setShortcut(tr("Ctrl+F"));
	//searchAction->setStatusTip(tr("Search through the current document."));
	//connect(searchAction, SIGNAL(triggered()), this, SLOT(search()));
	//editMenu->addAction(searchAction);

	editMenu->addSeparator();

	clearDebuggerAction = new QAction(QObject::tr("Clear Output"), 0);
	//clearDebuggerAction->setShortcut(QKeySequence(""));
	clearDebuggerAction->setStatusTip(tr("Clear the Debugger Output window."));
	connect(clearDebuggerAction, SIGNAL(triggered()), this, SLOT(clearDebugger()));
	editMenu->addAction(clearDebuggerAction);

	editMenu->addSeparator();

	//commentAction = new QAction(QIcon(":/Resources/comment.png"), tr("Comment"), this);
	//commentAction->setShortcut(tr("Ctrl+K"));
	//commentAction->setStatusTip(tr("Comment out the selected lines."));
	//connect(commentAction, SIGNAL(triggered()), this, SLOT(comment()));
	//editMenu->addAction(commentAction);

	//uncommentAction = new QAction(QIcon(":/Resources/uncomment.png"), tr("Uncomment"), this);
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
	selToMatchingBraceAction->setShortcut(QKeySequence("Shift+Ctrl+E"));
	selToMatchingBraceAction->setStatusTip(tr("Select to the matching brace."));
	connect(selToMatchingBraceAction, SIGNAL(triggered()), this, SLOT(selectToMatchingBrace()));
	editMenu->addAction(selToMatchingBraceAction);
	lineCommentAction = new QAction(QObject::tr("(Un)Comment the selected line(s)"), 0);
	lineCommentAction->setShortcut(QKeySequence("Ctrl+/"));
	lineCommentAction->setStatusTip(tr("(Un)Comment the selected line(s)."));
	connect(lineCommentAction, SIGNAL(triggered()), this, SLOT(lineComment()));
	editMenu->addAction(lineCommentAction);
	blockCommentAction = new QAction(QObject::tr("(Un)Comment the selected block"), 0);
	blockCommentAction->setShortcut(QKeySequence("Shift+Ctrl+/"));
	blockCommentAction->setStatusTip(tr("(Un)Comment the selected block."));
	connect(blockCommentAction, SIGNAL(triggered()), this, SLOT(blockComment()));
	editMenu->addAction(blockCommentAction);
	duplicateLineAction = new QAction(QObject::tr("Duplicate the current line"), 0);
	duplicateLineAction->setShortcut(QKeySequence("Ctrl+D"));
	duplicateLineAction->setStatusTip(tr("Duplicate the selected line."));
	connect(duplicateLineAction, SIGNAL(triggered()), this, SLOT(duplicateLine()));
	editMenu->addAction(duplicateLineAction);
	moveLineUpAction = new QAction(QObject::tr("Move the current line up"), 0);
	moveLineUpAction->setShortcut(QKeySequence("Ctrl+T"));
	moveLineUpAction->setStatusTip(tr("Move the current line up."));
	connect(moveLineUpAction, SIGNAL(triggered()), this, SLOT(moveLineUp()));
	editMenu->addAction(moveLineUpAction);
	deleteCurrentLineAction = new QAction(QObject::tr("Delete the current line"), 0);
	deleteCurrentLineAction->setShortcut(QKeySequence("Ctrl+L"));
	deleteCurrentLineAction->setStatusTip(tr("Delete the current line."));
	connect(deleteCurrentLineAction, SIGNAL(triggered()), this, SLOT(deleteCurrentLine()));
	editMenu->addAction(deleteCurrentLineAction);
	toUpperCaseAction = new QAction(QObject::tr("Change the line to UPPER CASE"), 0);
	toUpperCaseAction->setShortcut(QKeySequence("Ctrl+U"));
	toUpperCaseAction->setStatusTip(tr("Change the line to UPPER CASE."));
	connect(toUpperCaseAction, SIGNAL(triggered()), this, SLOT(toUpperCase()));
	editMenu->addAction(toUpperCaseAction);
	toLowerCaseAction = new QAction(QObject::tr("Change the line to lower case"), 0);
	toLowerCaseAction->setShortcut(QKeySequence("Ctrl+Shift+U"));
	toLowerCaseAction->setStatusTip(tr("Change the line to lower case."));
	connect(toLowerCaseAction, SIGNAL(triggered()), this, SLOT(toLowerCase()));
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

	runScriptAction = scriptMenu->addAction(QIcon(":/Resources/runScript.png"),tr("Run Script"));
	runScriptAction->setEnabled(false);
	runScriptAction->setShortcut(QKeySequence(tr("F5")));
	runScriptAction->setStatusTip(tr("Run the current script"));
	runScriptAction->setData(MainAppInfo::Execute);
	connect(runScriptAction, SIGNAL(triggered()), this, SLOT(executeScript()));

	debugScriptAction = scriptMenu->addAction(tr("Debug Script"));//QIcon(":/Resources/runScript.png"),tr("&Run Script"));
	debugScriptAction->setEnabled(false);//(false);
	debugScriptAction->setShortcut(QKeySequence(tr("F6")));
	debugScriptAction->setStatusTip(tr("Open the current script in the Debugger"));
	debugScriptAction->setData(MainAppInfo::Debug);
	connect(debugScriptAction, SIGNAL(triggered()), this, SLOT(executeScript()));

	abortScriptAction = scriptMenu->addAction(tr("Abort Script"));//QIcon(":/Resources/runScript.png"),tr("&Run Script"));
	abortScriptAction->setEnabled(false);//false);
	abortScriptAction->setShortcut(QKeySequence(tr("F7")));
	abortScriptAction->setStatusTip(tr("Stop the current script from running"));
	connect(abortScriptAction, SIGNAL(triggered()), this, SLOT(abortScript()));


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
	viewMenu->addAction(debugLogDock->toggleViewAction());
	viewMenu->addAction(debuggerDock->toggleViewAction());
	menuBar()->addMenu(viewMenu);//the view menu..........................................................

	optionsAction = toolsMenu->addAction(tr("O&ptions"));
	optionsAction->setShortcut(QKeySequence(tr("Ctrl+P")));
	optionsAction->setStatusTip(tr("Open the options dialog"));
	//optionsAction->setEnabled(true);
	connect(optionsAction, SIGNAL(triggered()), this, SLOT(openOptionsDialog()));
	menuBar()->addMenu(toolsMenu);//the tools menu..........................................................

	debuggerMenu = AppScriptEngine->createDebuggerMenu(menuBar());//debugger->createStandardMenu(menuBar());
	menuBar()->addMenu(debuggerMenu);
}

void MainWindow::setupHelpMenu()
{
	helpMenu = menuBar()->addMenu(tr("Help"));
	aboutStimulGLAct = new QAction(tr("About StimulGL"), this);
	aboutStimulGLAct->setStatusTip(tr("Show the StimulGL About Dialog"));
	helpMenu->addAction(aboutStimulGLAct);
	aboutStimulGLAct->setShortcut(QKeySequence(tr("F1")));
	connect(aboutStimulGLAct, SIGNAL(triggered()), this, SLOT(aboutStimulGL()));

	aboutQtAct = new QAction(tr("About Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt About Dialog"));
	helpMenu->addAction(aboutQtAct);
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	menuBar()->addMenu(helpMenu);//the help menu..........................................................
}

void MainWindow::write2OutputWindow(const QString &text2Write)
{
	outputWindowList->addItem(text2Write);
}

void MainWindow::clearDebugger()
{
	outputWindowList->clear();
}

void MainWindow::createDockWindows()
{
	debugLogDock = new QDockWidget(tr("Output"), this);
	debugLogDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea | Qt::BottomDockWidgetArea);	
	outputWindowList = new QListWidget(debugLogDock);
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

void MainWindow::setupDynamicPluginMenus()
{
	if (StimulGLFlags.testFlag(MainAppInfo::DisableAllPlugins) == false)
	{
		showSplashMessage("Loading Static Plugins...");
		Q_IMPORT_PLUGIN(parallelportplugin)// see below
		Q_IMPORT_PLUGIN(experimentmanagerplugin)// see below
		foreach (QObject *plugin, QPluginLoader::staticInstances())
		{
			popPluginIntoMenu(plugin);
		}
		showSplashMessage("Loading Dynamic Plugins...");
		foreach (QString fileName, QDir(MainAppInfo::pluginsDirPath()).entryList(QDir::Files)) {
			QPluginLoader loader(QDir(MainAppInfo::pluginsDirPath()).absoluteFilePath(fileName));
			QObject *plugin = loader.instance();//The QObject provided by the plugin, if it was compiled against an incompatible version of the Qt library, QPluginLoader::instance() returns a null pointer.
			if (plugin) {
				popPluginIntoMenu(plugin);
				pluginFileNames += fileName;
			}
		}
	}
	////example 1(static plugin):
	//see line Q_IMPORT_PLUGIN(parallelportplugin)
	//ParallelPort *a = new ParallelPort(888,NULL); //see Q_DECLARE_METATYPE(ParallelPort*), must include header etc
	//a->IsPortEcp();
	//delete a;
	//ExperimentManager *b = new ExperimentManager(this);
	//b->openExperiment();
	//delete b;
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

	ExtensionInterface *iExtension = qobject_cast<ExtensionInterface *>(plugin);//For each plugin (static or dynamic), we check which interfaces it implements using qobject_cast()
	if (iExtension) 
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
		collection->GetInterface(tmpIndex)->ConfigureScriptEngine(* AppScriptEngine->eng);
		return action1;
	}
	return 0;
}

void MainWindow::setAppDirectories()
{
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

void MainWindow::setScriptRunningStatus(MainAppInfo::ActiveScriptMode state)
{
	AppScriptStatus = state;
	switch (state)
	{
	case MainAppInfo::Debugging:
		{ 
			runScriptAction->setEnabled(false);
			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(true);
			break; 
		}	
	case MainAppInfo::Executing:
		{ 
			runScriptAction->setEnabled(false);
			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(true);			
			break; 
		}	
	case MainAppInfo::NoScript:
		{ 
			runScriptAction->setEnabled(false);
			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(false);			
			break; 
		}	
	case MainAppInfo::Pending:
		{ 
			runScriptAction->setEnabled(true);
			debugScriptAction->setEnabled(true);
			abortScriptAction->setEnabled(false);			
			break; 
		}	
	case MainAppInfo::Stopping:
		{ 
			runScriptAction->setEnabled(false);
			debugScriptAction->setEnabled(false);
			abortScriptAction->setEnabled(true);			
			break; 
		}	
	}
}

QString MainWindow::getSelectedScriptFileLocation()
{
	return DocManager->getFilePath(activeMdiChild());
}

QString MainWindow::getSelectedScriptFileName()
{
	return DocManager->getFileName(activeMdiChild(),true);
}

void MainWindow::closeSelectedScriptFile()
{
	closeSubWindow();
}

void MainWindow::executeScript()
{
	QAction *action = qobject_cast<QAction *>(sender());
	StimulGLScriptRunMode = (MainAppInfo::ScriptRunMode)action->data().toInt(); 
	switch (DocManager->getDocType(activeMdiChild()))
	{
	case MainAppInfo::DOCTYPE_QSCRIPT:
		{ break; }
	case MainAppInfo::DOCTYPE_SVG: {
			if (DocManager->getDocHandler(activeMdiChild())->isModified()){
				QMessageBox::StandardButton ret;
				ret = QMessageBox::warning(this, tr("Save Changes?"),
					tr("'%1' has unsaved changes that need to be saved to execute\n"
					"Do you want to save your changes first?")
					.arg(QFileInfo(activeMdiChildFilePath()).fileName()),
					QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		 		if (ret == QMessageBox::Save){
					save();
					SVGPreviewer->openFile(DocManager->getFileName(activeMdiChild()));
					//SVGPreviewer->openFile(activeMdiChildFilePath()); abov code worked, this one didn't received an empty path...
					SVGPreviewer->showMaximized();
					//SVGPreviewer->showFullScreen();
				}
			}
			else {
				SVGPreviewer->openFile(DocManager->getFileName(activeMdiChild()));
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
	default: {//none of them 
			return;
			break;
		}
	}

	clearDebugger();
	QTime t;

	//AppScriptEngine->

	if (StimulGLScriptRunMode == MainAppInfo::Debug)
	{
		if (!AppScriptEngine->eng->canEvaluate(DocManager->getDocHandler(activeMdiChild())->text()))
		{
			write2OutputWindow("... DebugScript -> Script syntax error ...");
		}
	}
	
	//{
		write2OutputWindow("... Script started Evaluating on " + t.currentTime().toString() + "...");
		t.start();

		//AppScriptEngine->executeScript(DocManager->getDocHandler(activeMdiChild())->text());
		//return;
		QScriptValue result;
		result = AppScriptEngine->eng->evaluate(DocManager->getDocHandler(activeMdiChild())->text());
		QString strResult;
		strResult = result.toString();
		if (result.isError()) 
		{
			write2OutputWindow("... Script stopped Evaluating due to error on line " + result.property("lineNumber").toString() + ": --> " + result.toString() + "...");
			DocManager->getDocHandler(activeMdiChild())->setCursorPosition(result.property("lineNumber").toInteger()-1,0);
			return;
		}
		else
		{
			int timeElapsed = t.elapsed();
			write2OutputWindow("... Total Time(ms): " + QString::number(timeElapsed));
			write2OutputWindow("... Script fully Evaluated on " + t.currentTime().toString() + " ...");
		}
	//}	
	AppScriptEngine->eng->collectGarbage();
	outputWindowList->scrollToBottom();

	////if (AppScriptEngine->eng->canEvaluate("create"))
	////{
	////	QScriptValue createFunc = AppScriptEngine->eng->evaluate("create");
	////	if (AppScriptEngine->eng->hasUncaughtException()) {
	////		QScriptValue exception = AppScriptEngine->eng->uncaughtException();
	////		//QMessageBox::critical(0, "Script error", QString("Script threw an uncaught exception while looking for create func: ") + exception.toString());
	////		write2Debugger("... No create Function found in script ...");
	////		//return;
	////	}
	////	else
	////	{
	////		if (!createFunc.isFunction()) {
	////			QMessageBox::critical(0, "Script Error", "create Function is not a function!");
	////		}
	////		else
	////		{
	////			createFunc.call(AppScriptThisObject);
	////		}		
	////	}
	////}

	//if (AppScriptEngine->eng->hasUncaughtException())	{
	//	QScriptValue exception = AppScriptEngine->eng->uncaughtException();
	//	QMessageBox::critical(0, "Script error", QString("Script threw an uncaught exception while looking for create func: ") + exception.toString());
	//	return;
	//}
	// now emit our test signal:
	// emit AbortScript();

}

//void MainWindow::setCurrentScriptMode()
//{
//ActiveScriptModes
//
//}
//
//void MainWindow::getCurrentScriptMode()
//{
//
//}

void MainWindow::abortScript()
{
	//AppScriptEngine->eng->currentContext()->throwError("bbbb");
	AppScriptEngine->eng->collectGarbage();
	//AppScriptEngine->agent->exceptionThrow(currentRunningScriptID,"bbbbbb",true);
	//AppScriptEngine->agent->scriptUnload(currentRunningScriptID);

	//scriptUnload

	//QScriptContext::throwError ("endless loop ...");

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!this needs to be done

//	AppScriptEngine->agent->exceptionThrow(currentRunningScriptID,"blaaaaaa",false));

	//emit AbortScript();
	////AppScriptEngine->eng->th

	//if (AppScriptEngine->eng->isEvaluating())
	//{
	//	AppScriptEngine->eng->abortEvaluation();
	//}
	//else
	//{
	//	//AppScriptEngine->agent->exceptionThrow()//scriptUnload();// ->~QScriptEngine();
	//}
}

void MainWindow::openOptionsDialog()
{
	int returnVal;
	OptionPage MainOptionPage(*settings,this);
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

//void MainWindow::aboutQtPlugins()
//{
//	PluginDialog dialog(pluginsDir.path(), pluginFileNames, this);
//	dialog.exec();
//}

void MainWindow::aboutStimulGL()
{
	aboutQTDialog aboutQTDlg(MainAppInfo::MainProgramTitle(),pluginFileNames,MainAppInfo::pluginsDirPath(), pluginFileNames, this);
	aboutQTDlg.exec();
}

void MainWindow::setStartupFiles(const QString &path)
{
	startUpFiles = path.split(";");
}

void MainWindow::openFiles(const QString &fileToLoad, const QStringList &filesToLoad)
{
	QStringList fileNames;
	//QString fileExtList("QT Script files (*.qs);;"
	//					"SVG files (*.svg *.svgz *.svg.gz);;"
	//					"Any files (*)");
	if ((fileToLoad.isNull()) && (filesToLoad.count() == 0))
	{
		fileNames = QFileDialog::getOpenFileNames(this, tr("Open File(s)"),m_currentPath, MainAppInfo::getFileExtList());
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

/*void MainWindow::setupSyntaxHighlighting(MdiChild *childWindow,MDIDocumentType tempFileType)
{
	QFont font;

	switch (tempFileType)
	{
	case DOCTYPE_QSCRIPT:
		{
			font.setFamily("Arial");
			font.setFixedPitch(true);
			font.setPointSize(11);
			childWindow->setFont(font);
			SyntaxHL = new SyntaxHighlighter(childWindow->document());
			break;
		}
	case DOCTYPE_SVG:
		{
			font.setFamily("Arial");
			font.setFixedPitch(true);
			font.setPointSize(11);
			childWindow->setFont(font);
			break;
		}
	case DOCTYPE_UNDEFINED:
		{
			font.setFamily("Arial");
			font.setFixedPitch(true);
			font.setPointSize(11);
			childWindow->setFont(font);
			break;
		}
	default://none of them
		{
			font.setFamily("Arial");
			font.setFixedPitch(true);
			font.setPointSize(11);
			childWindow->setFont(font);
			break;
		}
	}	
}*/



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
	MainAppInfo::DocType tempDocType = DocManager->getDocType(fileExtension);
	int DocIndex;
	newDocument(tempDocType,DocIndex);

	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (DocManager->loadFile(DocIndex,file.fileName()))
	{	
		//newChild->showMaximized();
		setCurrentFile(file.fileName());
	//	setupSyntaxHighlighting(child,tempFileType);
		statusBar()->showMessage(tr("File loaded"), 2000);
	} 
	else 
	{
		mdiArea->removeSubWindow(findMdiChild(file.fileName()));
		//newChild->close();
		//delete newChild;
		statusBar()->showMessage(tr("Could not load File"), 2000);
		QApplication::restoreOverrideCursor();
		return false;
	}
	QApplication::restoreOverrideCursor();
	return true;
}
void MainWindow::setRenderer()
{
	settings->beginGroup("Rendering");
	if (settings->contains("RenderType"))	
	{
		int type;
		type = settings->value("RenderType").toInt();
		switch (type)
		{
		case 1://Native
			{
				SVGPreviewer->setHighQualityAntialiasing(false);
				SVGPreviewer->setRenderer(SvgView::Native);
				break;
			}
		case 2://OpenGL
			{
				if (settings->contains("HQAntiAlias"))
				{
					SVGPreviewer->setHighQualityAntialiasing(settings->value("HQAntiAlias").toBool());
				}
				else
				{
					SVGPreviewer->setHighQualityAntialiasing(false);
				}
				SVGPreviewer->setRenderer(SvgView::OpenGL);
				break;
			}
		case 3://Image
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
	settings->endGroup();
}

void MainWindow::setCurrentFile(const QString &fileName)
{
	if (fileName.isEmpty())
		setWindowTitle(MainAppInfo::MainProgramTitle());
	else if (fileName == MainAppInfo::UntitledDocName())
	{
		setWindowTitle(tr("%1 - %2").arg(MainAppInfo::MainProgramTitle()).arg(MainAppInfo::UntitledDocName()));
	}	
	else if (!fileName.startsWith(":/")) 
	{
		m_currentPath = fileName;
		setWindowTitle(tr("%1 - %2").arg(MainAppInfo::MainProgramTitle()).arg(strippedName(m_currentPath)));
		updateRecentFileList(fileName);
	}
}

void MainWindow::updateRecentFileList(const QString &fileName)
{
	QStringList files = settings->value("recentFileList").toStringList();
	files.removeAll(fileName);
	files.prepend(fileName);
	while (files.size() > MaxRecentFiles)
		files.removeLast();

	settings->setValue("recentFileList", files);

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
	QStringList files = settings->value("recentFileList").toStringList();

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
	MainAppInfo::DocType docType;
	if (action)
	{docType = (MainAppInfo::DocType)action->data().toInt();}
	else
	{docType = MainAppInfo::DOCTYPE_UNDEFINED;}	

	int DocIndex;
	newDocument(docType,DocIndex);
	setCurrentFile(MainAppInfo::UntitledDocName());
	//	setupSyntaxHighlighting(child,tempFileType);
	statusBar()->showMessage(tr("New File created"), 2000);
}

void MainWindow::newDocument(const MainAppInfo::DocType &docType, int &DocIndex)
{
	DocIndex = 0;
	QsciScintilla *newChild = DocManager->add(docType,DocIndex);
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
	QString tmpFileName = DocManager->getFileName(activeMdiChild());
	if (activeMdiChild())
	{
		if (tmpFileName == "")
		{
			return saveAs();
		}
		else
		{
			if (DocManager->saveFile(activeMdiChild(),tmpFileName))
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
	if (activeMdiChild())
	{
		QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),DocManager->getFileName(activeMdiChild()),MainAppInfo::getFileExtList());
		if (fileName.isEmpty())
		{
			return;
		}
		else
		{
			if (DocManager->saveFile(activeMdiChild(),fileName))
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
	if (DocManager->getDocHandler(activeMdiChild()))
		DocManager->getDocHandler(activeMdiChild())->cut();
}

void MainWindow::copy()
{
	if (DocManager->getDocHandler(activeMdiChild()))
		DocManager->getDocHandler(activeMdiChild())->copy();
}

void MainWindow::paste()
{
	if (DocManager->getDocHandler(activeMdiChild()))
		DocManager->getDocHandler(activeMdiChild())->paste();
}

void MainWindow::lineComment()
{
	DocManager->getDocHandler(activeMdiChild())->toggleLineComment();
}
void MainWindow::blockComment()
{
	DocManager->getDocHandler(activeMdiChild())->toggleBlockComment();
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
	//windowMenu->addAction(separatorAction);

	QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
	separatorAct->setVisible(!windows.isEmpty());

	for (int i = 0; i < windows.size(); ++i) {
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

		CustomQsciScintilla *tmpSci;
		tmpSci = DocManager->getDocHandler(activeMdiChild());

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
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	if ( tmpSci && !tmpSci == 0 && a ) {
		int line = a->text().section(':', 0, 0).toInt();
		tmpSci->gotoLine(line - 1);
	}
}

void MainWindow::setupToolBars()
{
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
	toolsToolBar->addAction(debugScriptAction);
	toolsToolBar->addAction(abortScriptAction);
}

void MainWindow::parseRemainingGlobalSettings()
{
	QPoint pos = settings->value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings->value("size", QSize(400, 400)).toSize();
	move(pos);
	resize(size);
	setRenderer();

	settings->beginGroup("Debugging");
	if (settings->contains("OpenExtDebug"))	
	{
		AppScriptEngine->ConfigureDebugger(settings->value("OpenExtDebug").toBool());
	}
	else//default
	{
		AppScriptEngine->ConfigureDebugger(false);
	}
	settings->endGroup();
	settings->beginGroup("General");
	if (settings->contains("OpenExtDebug"))	
	{
		AppScriptEngine->ConfigureDebugger(settings->value("OpenExtDebug").toBool());
	}
	else//default
	{
		AppScriptEngine->ConfigureDebugger(false);
	}

	settings->endGroup();
}

void MainWindow::writeMainWindowSettings()
{
	settings->setValue("pos", pos());
	settings->setValue("size", size());
}

bool MainWindow::closeSubWindow()
{
	QAction *action = qobject_cast<QAction *>(sender());
	if (action == 0)//direct function call, exit program
	{
		QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
		for (int i = 0; i < windows.size(); ++i) {
			mdiArea->setActiveSubWindow(windows.at(i));
			if (!DocManager->maybeSave(windows.at(i))) 
			{
				return false;
			}
		}
		DocManager->setAllUnmodified();//to prevent a maybesave() check loop
		return true;
	}
	if (action->data().toString() == "Close")//From menu(Close)
	{
		if (!DocManager->maybeSave(activeMdiChild())) 
		{
			return false;
		}
		DocManager->remove(activeMdiChild());
		mdiArea->closeActiveSubWindow();
		return true;
	}
	else if (action->data().toString() == "CloseAll")//From menu(Close All)
	{
		QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
		for (int i = 0; i < windows.size(); ++i) {
			mdiArea->setActiveSubWindow(windows.at(i));
			if (!DocManager->maybeSave(windows.at(i))) 
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
		if (!DocManager->maybeSave(activeMdiChild())) 
		{
			return false;
		}
		DocManager->remove(activeMdiChild());
		mdiArea->closeActiveSubWindow();
		return true;
	}
	return false;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if(!closeSubWindow())
	{
		event->ignore();
		return;
	}
	mdiArea->closeAllSubWindows();
	if (activeMdiChild()) {
		event->ignore();
	} else {
		writeMainWindowSettings();
		event->accept();
	}
	if(Plugins)
		delete Plugins;
#ifdef Q_OS_WIN
	timeEndPeriod(1);
#endif
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
	DocManager->getDocHandler(activeMdiChild())->moveToMatchingBrace();
}

void MainWindow::selectToMatchingBrace()
{
	DocManager->getDocHandler(activeMdiChild())->selectToMatchingBrace();
}

void MainWindow::duplicateLine()
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	if (tmpSci->hasSelectedText())
	{tmpSci->SendScintilla(QsciScintilla::SCI_SELECTIONDUPLICATE);}
	else
		{tmpSci->SendScintilla(QsciScintilla::SCI_LINEDUPLICATE);}
}

void MainWindow::moveLineUp()
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->SendScintilla(QsciScintilla::SCI_LINETRANSPOSE);
	tmpSci->SendScintilla(QsciScintilla::SCI_LINEUP);
}

void MainWindow::deleteCurrentLine()
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->SendScintilla(QsciScintilla::SCI_LINEDELETE);
}

void MainWindow::toUpperCase()
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->SendScintilla(QsciScintilla::SCI_UPPERCASE);
}

void MainWindow::toLowerCase()
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->SendScintilla(QsciScintilla::SCI_LOWERCASE);
}

void MainWindow::print()
{
#ifndef QT_NO_PRINTDIALOG
	if (DocManager->getDocHandler(activeMdiChild())->print())
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
#endif
}

void MainWindow::toggleMarker() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->toggleMarker();
	//initMarkersMenu();
}

void MainWindow::nextMarker() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->nextMarker();
}

void MainWindow::prevMarker() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->prevMarker();
}

void MainWindow::removeAllMarkers() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	tmpSci->removeAllMarkers();
}

void MainWindow::goToLine() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	bool ok = false;
	int line = QInputDialog::getInteger(mdiArea, tr("Go to line"), 
		tr("Go to line") + QString(" (1 - %1):").arg(tmpSci->lineCount()), 
		1, 1, tmpSci->lineCount(), 1, &ok);
	if ( ok )
		tmpSci->gotoLine(line - 1);
}

void MainWindow::findImpl(bool replc) 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());
	DocFindFlags flags(replc);
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
	if ( DocManager->getFindParams(activeMdiChild(),str1, str2, flags) ) {
		if ( flags.replace ) {
			tmpSci->replace(str1, str2, flags);
		}
		else {
			tmpSci->find(str1, flags);
		}
	}	
}

void MainWindow::find() 
{
	findImpl(false);
}

void MainWindow::replace() 
{
	findImpl(true);
}

void MainWindow::findNext() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());

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

void MainWindow::findPrev() 
{
	CustomQsciScintilla *tmpSci;
	tmpSci = DocManager->getDocHandler(activeMdiChild());

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