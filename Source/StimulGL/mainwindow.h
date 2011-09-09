/****************************************************************************
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//#define QT_NO_PLUGIN_CHECK

#include <QMainWindow>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QtGui>
#include <QTime>
#include <QDateTime> //QDateTime::currentDateTime().toString("yyyyMMddHHmmsszzz")!!!!!

#include "documentmanager.h"
#include "plugininterface.h"
#include "plugincollection.h"
#include "qtscriptengine.h"
#include "mainappinfo.h"
#include "sciFindDialog.h"

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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	MainWindow();
	~MainWindow();

signals:
	//void AbortScript();
		
public slots:
	void setStartupFiles(const QString &path = QString());
	void openFiles(const QString &fileToLoad = QString(), const QStringList &filesToLoad = QStringList());
	void executeScript();
	QString getSelectedScriptFileLocation();
	QString getSelectedScriptFileName();
	void closeSelectedScriptFile();
	void abortScript();
	//void debugScript();
	bool initialize(MainAppInfo::MainProgramModeFlags mainFlags = 0);

private slots:
	void setupContextMenus();
	void DebugcontextMenuEvent(const QPoint &pos);
	void write2Debugger(const QString &text2Write = "");
	void clearDebugger();
	void setupScriptEngine();
	void setScriptRunningStatus(MainAppInfo::ActiveScriptMode state);
	void showPluginGUI();
	void openOptionsDialog();
	void aboutStimulGL();
	void openRecentFile();
	bool closeSubWindow();
	void newFile();
	void save();
	void saveAs();
	void cut();
	void copy();
	void paste();
	void lineComment();
	void findImpl(bool replc);
	void find();
	void replace();
	void findNext();
	void findPrev();
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
	MainAppInfo::MainProgramModeFlags StimulGLFlags;
	MainAppInfo::ScriptRunMode StimulGLScriptRunMode;
	QSplashScreen *MainSplashScreen;
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
	QAction *debugScriptAction;
	QAction *abortScriptAction;
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
	QAction *aboutQtAct;
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
	bool DoNotLoadScriptBindings;

	QTScriptEngine *AppScriptEngine;
	MainAppInfo::ActiveScriptMode AppScriptStatus;
	qint64 currentRunningScriptID;	

	QPushButton button;
    QMenu *pluginsMenu;
    QMenu *devicePluginMenu;
	QMenu *extensionPluginMenu;
    QMenu *helpMenu;
	QMenu *recentFilesMenu;
	QMenu *fileMenu;
	QMenu *viewMenu;
	QMenu *markersMenu;
	QMenu *editMenu;
	QMenu *toolsMenu;
	QMenu *windowMenu;
	QMenu *scriptMenu;
	QMenu *debuggerMenu;

    //QAction *deviceAct1;
    //QAction *deviceAct2;
    //QAction *deviceAct3;
    //DeviceControl *devices;
	//QString curFile;
	QSettings *settings;

	QDockWidget *debugLogDock;
	QDockWidget *debuggerDock;

	QListWidget *outputWindowList;
	QMainWindow *debuggerMainWindow;
	
	DocumentManager *DocManager;
    QMdiArea *mdiArea;
	QSignalMapper *windowMapper;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *toolsToolBar;

    QString m_currentPath;
    //QDir pluginsDir;
    QStringList pluginFileNames;
	
	PluginCollection *Plugins;

	enum { MaxRecentFiles = 5 };
	QAction *recentFileActs[MaxRecentFiles];

	void showSplashMessage(const QString message);
	void initAndParseGlobalSettings();
	void setupMDI();
	void setupDocumentManager();
	void setAppDirectories();
	void createDockWindows();
	void setupStatusBar();
	bool setDefaultGLFormat();
    void createDefaultMenus();
	void setupHelpMenu();
    void setupDynamicPluginMenus();
	QAction* integratePlugin(QObject *plugin, PluginCollection *collection);
	void setupToolBars();
	void setRenderer();
	void newDocument(const MainAppInfo::DocType &docType, int &DocIndex);
	//void setupSyntaxHighlighting(MdiChild *childWindow,MDIDocumentType tempFileType);
	void parseRemainingGlobalSettings();
	void writeMainWindowSettings();
    bool popPluginIntoMenu(QObject *plugin);
	bool parseFile(const QFile &file);
	void setCurrentFile(const QString &fileName);
	void updateRecentFileActions();
	QString strippedName(const QString &fullFileName);
	QMdiSubWindow *activeMdiChild();
	QString activeMdiChildFilePath();
	QMdiSubWindow *findMdiChild(const QString &fileName);
	void updateRecentFileList(const QString &fileName);
	
	//std::ofstream mainLogFile;
	//void MyOutputHandler(QtMsgType type, const char *msg);

protected:
	void closeEvent(QCloseEvent *event);

};

#endif
