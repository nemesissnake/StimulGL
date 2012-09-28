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


#ifndef DOCUMENTMANAGER_H
#define DOCUMENTMANAGER_H

#include <QObject>
#include <QFile>
#include <QDir>
#include <QtGui>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerxml.h>
#include "customqsciscintilla.h"
#include "scifinddialog.h"
#include "mainappinfo.h"

class DocumentManager : public QObject
{
	Q_OBJECT

signals:
	//void DocumentChanged();
	void NrOfLinesChanged(int);
	void DocumentManagerOutput(QString strText2Output);

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
	bool maybeSave(QMdiSubWindow *subWindow,bool bAutoSaveChanges = false);
	void setAllUnmodified();
	bool getFindParams(QMdiSubWindow *subWindow,QString& str1, QString& str2, DocFindFlags& flags);
	QString lastFindText() const;
	DocFindFlags lastFlags() const; 
	bool appendKnownFileExtensionList(QString strFileExtLst);
	QString getKnownFileExtensionList() {return strFileExtensionList;};
	int addAdditionalApiEntry(const QString &entry); 

public slots:
	void signalDocManagerOutput(QString strText2Output) {emit DocumentManagerOutput(strText2Output);};

private:
	int DocCount;
	DocFindFlags lastFlags_;
	QSignalMapper *DocModifiedMapper;
	QSignalMapper *NrOfLinesChangedMapper;
	QStringList ChildrenFileNames;
	QString strFileExtensionList;
	QList<MainAppInfo::DocType> ChildrenDocTypes;
	QList<CustomQsciScintilla *> QScintillaChildren;
	QList<QMdiSubWindow *> SubWindowChildren;
	QList<bool> ChildrenModification;
	QStringList additionalApiEntries;

	void setFileName(int DocIndex, QString fileName);
	bool getLexer(QsciLexer *lexer, const QString &lexerName, QObject *parent = 0);
	QStringList getAdditionalApiEntries() {return additionalApiEntries;};

private slots:
	void documentWasModified(QWidget *subWindow);
	void updateLineNumbers(QWidget *tmpSci);
	void onMarginClicked (int margin, int line, Qt::KeyboardModifiers state);
	//void updateLineNumbers(void);
};

#endif // DOCUMENTMANAGER_H
