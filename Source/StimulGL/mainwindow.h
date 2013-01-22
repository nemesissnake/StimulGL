//StimulGL
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


/****************************************************************************
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define QT_NO_PLUGIN_CHECK

#include <QString>
#include <QStringList>
#include <QDir>
#include <QtWidgets>
#include <QTime>
#include <QDateTime> //QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat())!!!!!

#include "documentwindow.h"
#include "documentmanager.h"
#include "plugininterface.h"
#include "plugincollection.h"
#include "qtscriptengine.h"
#include "mainappinfo.h"
#include "sciFindDialog.h"
#include "assistant.h"

class SvgView;
class DeviceControl;

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsRectItem;
class QSplashScreen;
QT_END_NAMESPACE

//!  The StimulGL Main Application. 
/*!
  The StimulGL Main Application can be directly accessed within the script by using the default 'StimulGL' object.
*/
class MainWindow : public DocumentWindow//QMainWindow
{
    Q_OBJECT
	Q_CLASSINFO("ScriptAPIClassName", "StimulGL");//Can't use defines here!, moc doesn't handle defines

public:
	MainWindow();
	//~MainWindow();//see void MainWindow::closeEvent(QCloseEvent *event)!

signals:
	//void AbortScript();
		
public slots:
	bool receiveExchangeMessage(const QString &sMessage);
	QVariant invokeJavaScriptConfigurationFile(const QString &sCode);//undocumented!
	void showJavaScriptConfigurationFile();//undocumented!
	void setStartupFiles(const QString &path = QString());
	void openFiles(const QString &fileToLoad = QString(), const QStringList &filesToLoad = QStringList());
	void executeScript();
	QString getSelectedScriptFileLocation();
	QString getApplicationRootDirPath();
	QString getSelectedScriptFileName();
	QString getEnvironmentVariabele(QString strName);
	void closeSelectedScriptFile(bool bAutoSaveChanges = false);
	//void debugScript();
	bool initialize(GlobalApplicationInformation::MainProgramModeFlags mainFlags = 0);
	void write2OutputWindow(const QString &text2Write = "");
	void clearOutputWindow();
	void processEvents() {qApp->processEvents();};
	void cleanupScript();
	void activateMainWindow();
	
	void find(bool useParams = false, QString strFindString = "", DocFindFlags findFlags = _DocFindFlags());
	void replace(bool bReplaceAll = false, bool useParams = false, QString strFindString = "", QString strReplaceString = "", DocFindFlags findFlags = _DocFindFlags());
	void findNext();
	void findPrev();
#ifdef DEBUG
	QString testFunction(QString inp = "");
#endif

private slots:
	void returnToOldMaxMinSizes();
	bool restartScriptEngine();
	void abortScript();
	void setupContextMenus();
	void DebugcontextMenuEvent(const QPoint &pos);
	void clearDebugger();
	void setupScriptEngine();
	void setScriptRunningStatus(GlobalApplicationInformation::ActiveScriptMode state);
	void showPluginGUI();
	void openOptionsDialog();
	void aboutStimulGL();
	void showDocumentation();
	void openRecentFile();
	bool closeSubWindow(bool bAutoSaveChanges = false);
	void newFile();
	void save();
	void saveAs();
	void cut();
	void copy();
	void paste();
	void lineComment();
	void findImpl(bool bReplace, bool useParams = false, QString strFindString = "", QString strReplaceString = "", DocFindFlags findFlags = _DocFindFlags(), bool bReplaceAll = false);
	void blockComment();
	void jumpToMatchingBrace();
	void selectToMatchingBrace();
	void duplicateLine();
	void moveLineUp();
	void deleteCurrentLine();
	void toUpperCase();
	void toLowerCase();
	void print();
	void updateMenuControls(QMdiSubWindow *subWindow);
	void setActiveSubWindow(QWidget *window);
	void updateWindowMenu();
	void updateMarkersMenu();
	void toggleMarker();
	void nextMarker();
	void prevMarker();
	void removeAllMarkers();
	void goToLine();
	void gotoMarker();
	void NumberOfLinesChanged(int nrOfLines);
	void CursorPositionChanged(int line, int col);
	void scriptLoaded(qint64 id);
	void scriptUnloaded(qint64 id);

private:
	//void registerFileTypeByDefinition(const QString &DocTypeName, const QString &DocTypeDesc, const QString &DocTypeExtension);

	QSize oldDockMaxSize, oldDockMinSize;
	QString strAdditionalFileExtensions;
	GlobalApplicationInformation::MainProgramModeFlags StimulGLFlags;
	GlobalApplicationInformation::ScriptRunMode StimulGLScriptRunMode;
	QSplashScreen *MainSplashScreen;
	Assistant *helpAssistant;
	QStringList startUpFiles;
    QAction *m_nativeAction;
    QAction *m_glAction;
    QAction *m_imageAction;
    QAction *m_highQualityAntialiasingAction;
    QAction *m_backgroundAction;
    QAction *m_outlineAction;
	QAction *openAction;
	QAction *quitAction;
	QAction *saveAction;
	QAction *optionsAction;
	QAction *saveAsAction;
	QAction *printAction;
	QAction *runScriptAction;
	//QAction *debugScriptAction;
	QAction *abortScriptAction;
	QAction *restartScriptEngineAction;
	QAction *newAction;
	QAction *newScriptAction;
	QAction *newSVGAction;
	QAction *cutAction;
	QAction *copyAction;
	QAction *clearDebuggerAction;
	QAction *pasteAction;
	QAction *goToLineAction;
	QAction *findAction;
	QAction *findNextAction;
	QAction *findPrevAction;
	QAction *replaceAction;
	QAction *goToMatchingBraceAction;
	QAction *selToMatchingBraceAction;
	QAction *lineCommentAction;
	QAction *blockCommentAction;
	QAction *duplicateLineAction;
	QAction *moveLineUpAction;
	QAction *deleteCurrentLineAction;
	QAction *toUpperCaseAction;
	QAction *toLowerCaseAction;
	QAction *closeAction;
	QAction *closeAllAction;
    QAction *tileAction;
    QAction *cascadeAction;
    QAction *nextAction;
    QAction *previousAction;
	QAction *addRemMarkerAction;
	QAction *nextMarkerAction;
	QAction *prevMarkerAction;
	QAction *remAllMarkerAction;
	QAction *aboutStimulGLAct;
	QAction *assistantAct;
	QAction *aboutQtAct;
	QAction *historyQtAct;
	QAction *separatorAct;

	QLabel *StatusPositionLabel;
	QLabel *StatusNameLabel;
	QLabel *StatusLinesLabel;

	QGraphicsScene *GraphScene;
	QGraphicsView *GraphView;
	SvgView *SVGPreviewer;

	bool DevicePluginsFound;
	bool ExtensionPluginsFound;
	bool PluginsFound;
	bool bMainWindowIsInitialized;

	QTScriptEngine *AppScriptEngine;
	GlobalApplicationInformation::ActiveScriptMode AppScriptStatus;
	qint64 currentRunningScriptID;	

	QPushButton button;
    QMenu *pluginsMenu;
    QMenu *devicePluginMenu;
	QMenu *extensionPluginMenu;
    QMenu *helpMenu;
	QMenu *recentFilesMenu;
	QMenu *fileMenu;
	//QMenu *viewMenu;
	QMenu *markersMenu;
	QMenu *editMenu;
	QMenu *toolsMenu;
	QMenu *windowMenu;
	QMenu *scriptMenu;
	//QMenu *debuggerMenu;

    //QAction *deviceAct1;
    //QAction *deviceAct2;
    //QAction *deviceAct3;
    //DeviceControl *devices;
	//QString curFile;
	//QSettings *settings;

	QDockWidget *debugLogDock;
	QDockWidget *debuggerDock;

	QListWidget *outputWindowList;
	MainWindow *debuggerMainWindow;
	
	DocumentManager *DocManager;
    QMdiArea *mdiArea;
	QSignalMapper *windowMapper;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *toolsToolBar;

    QString m_currentPath;
    QStringList pluginFileNames;
	PluginCollection *Plugins;
	GlobalApplicationInformation *globAppInfo;
	MainAppInformationStructure *mainAppInfoStruct;

	enum { MaxRecentFiles = 8 };
	QAction *recentFileActs[MaxRecentFiles];

	void showSplashMessage(const QString message);
	void setupMDI();
	void setupDocumentManager();
	void setAppDirectories();
	void createDockWindows();
	void setupStatusBar();
	bool setDefaultGLFormat();
    void createDefaultMenus();
	void setupHelpMenu();
    void setupDynamicPlugins();
	QAction* integratePlugin(QObject *plugin, PluginCollection *collection);
	void setupToolBars();
	void setRenderer();
	void newDocument(const GlobalApplicationInformation::DocType &docType, int &DocIndex, const QString &strExtension = "");
	//void setupSyntaxHighlighting(MdiChild *childWindow,MDIDocumentType tempFileType);
	void parseRemainingGlobalSettings();
	bool configureDebugger();
	bool configurePluginScriptEngine(const int nIndex);
	void writeMainWindowSettings();
	bool checkPluginCompatibility(QObject *plugin);
    bool popPluginIntoMenu(QObject *plugin);
	bool parseFile(const QFile &file);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);
	QMdiSubWindow *activeMdiChild();
	QString activeMdiChildFilePath();
	QMdiSubWindow *findMdiChild(const QString &fileName);
	void updateRecentFileList(const QString &fileName);
	void setDockSize(QDockWidget *dock, int setWidth, int setHeight);
	QScriptValue executeScriptContent(const QString &sContent);

public:
	bool extendAPICallTips(const QMetaObject* metaScriptObject = NULL);
	void setGlobalApplicationInformationObject(GlobalApplicationInformation *globAppInformation);
	void RecoverLastScreenWindowSettings();

protected:
	void closeEvent(QCloseEvent *event);
	virtual bool openDroppedFiles(const QStringList &pathList);	
};

#endif
