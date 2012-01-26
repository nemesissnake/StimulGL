#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QtGui>
#include "customqsciscintilla.h"
#include "scifinddialog.h"
#include "mainappinfo.h"

class DocumentManager : public QObject
{
	Q_OBJECT

signals:
	//void DocumentChanged();
	void NrOfLinesChanged(int);

public:

	DocumentManager(QObject *parent);
	~DocumentManager();

	CustomQsciScintilla *getDocHandler(const int &DocIndex);
	CustomQsciScintilla *getDocHandler(QMdiSubWindow *subWindow);
	int count(void);
	CustomQsciScintilla *add(MainAppInfo::DocType docType,int &DocIndex);
	bool remove(QMdiSubWindow *subWindow);
	bool setSubWindow(int DocIndex, QMdiSubWindow *subWindow);
	bool loadFile(int DocIndex, const QString &fileName);
	MainAppInfo::DocType getDocType(QMdiSubWindow *subWindow);
	MainAppInfo::DocType getDocType(const QString strExtension);
	QString getFileName(QMdiSubWindow *subWindow, bool bFileNameOnly = false);
	QString getFileName(int DocIndex, bool bFileNameOnly = false);
	QString getFilePath(QMdiSubWindow *subWindow);
	int getDocIndex(QMdiSubWindow *subWindow);
	int getDocIndex(const QString &DocName);
	bool saveFile(int DocIndex, QString fileName = "");
	bool saveFile(QMdiSubWindow *subWindow,  QString fileName = "");
	void setModFlagAndTitle(const int &DocIndex,bool hasChanges);
	bool maybeSave(QMdiSubWindow *subWindow);
	void setAllUnmodified();
	bool getFindParams(QMdiSubWindow *subWindow,QString& str1, QString& str2, DocFindFlags& flags);
	QString lastFindText() const;
	DocFindFlags lastFlags() const; 

private:
	int DocCount;
	DocFindFlags lastFlags_;
	QSignalMapper *DocModifiedMapper;
	QSignalMapper *NrOfLinesChangedMapper;
	QStringList ChildrenFileNames;
	QList<MainAppInfo::DocType> ChildrenDocTypes;
	QList<CustomQsciScintilla *> QScintillaChildren;
	QList<QMdiSubWindow *> SubWindowChildren;
	QList<bool> ChildrenModification;

	void setFileName(int DocIndex, QString fileName);
	bool getLexer(QsciLexer *lexer, const QString &lexerName, QObject *parent = 0);

	void changeColors(CustomQsciScintilla *currScintilla = NULL);

private slots:
	void documentWasModified(QWidget *subWindow);
	void updateLineNumbers(QWidget *tmpSci);
	void onMarginClicked (int margin, int line, Qt::KeyboardModifiers state);
	//void updateLineNumbers(void);
};

#endif // DOCUMENTMANAGER_H
