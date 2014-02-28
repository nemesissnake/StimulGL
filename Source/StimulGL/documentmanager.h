//StimulGL
//Copyright (C) 2014  Sven Gijsen
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
#include <QtWidgets>

#include <Qsci/qsciapis.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexerxml.h>
#include "customqsciscintilla.h"
#include "scifinddialog.h"
#include "mainappinfo.h"

#define WARNING_DOCCHANGEDSAVEFILE_STRING	"The document has been modified.\nDo you want to save your changes?"

class DocumentManager : public QObject
{
	Q_OBJECT

signals:
	//void DocumentChanged();
	void NrOfLinesChanged(int);
	void MarkerToggled(int);
	void DocumentManagerOutput(QString strText2Output);

public:

	DocumentManager(QObject *parent);
	~DocumentManager();

	struct strDocManagerDocument
	{
		QString sFileName;
		GlobalApplicationInformation::DocType dDocType;
		QWidget * pWidget;
		QMdiSubWindow * pMDISubWin;
		bool bIsModified;
		strDocManagerDocument()
		{
			sFileName = "";
			dDocType = GlobalApplicationInformation::DOCTYPE_UNDEFINED;
			pWidget = NULL;
			pMDISubWin = NULL;
			bIsModified = false;
		}
	};

	struct strcPluginDocHandlerInfo
	{
		QStringList strDocHandlerInfoList;
		QList<QObject *> pluginObject;
	};  

	QWidget *getDocHandler(const int &DocIndex);
	QWidget *getDocHandler(QMdiSubWindow *subWindow);
	int count(void);
	//CustomQsciScintilla *add(GlobalApplicationInformation::DocType docType,int &DocIndex, const QString &strExtension);
	QWidget *add(GlobalApplicationInformation::DocType docType,int &DocIndex, const QString &strExtension, const QString &strCanonicalFilePath = "");
	bool remove(QMdiSubWindow *subWindow);
	bool setSubWindow(int DocIndex, QMdiSubWindow *subWindow);
	bool loadFile(int DocIndex, const QString &fileName);
	GlobalApplicationInformation::DocType getDocType(QMdiSubWindow *subWindow);
	GlobalApplicationInformation::DocType getDocType(const QString strExtension);
	QString getFileName(QMdiSubWindow *subWindow, bool bFileNameOnly = false);
	QString getFileName(int DocIndex, bool bFileNameOnly = false);
	QString getFilePath(QMdiSubWindow *subWindow);
	int getDocIndex(QMdiSubWindow *subWindow);
	int getDocIndex(const QString &DocName);
	bool saveFile(int DocIndex, QString fileName = "", bool *bReparseDocumentContentNeeded = NULL);
	bool saveFile(QMdiSubWindow *subWindow,  QString fileName = "", bool *bReparseDocumentContentNeeded = NULL);
	void setModFlagAndTitle(const int &DocIndex,bool hasChanges);
	bool maybeSave(QMdiSubWindow *subWindow,bool bAutoSaveChanges = false);
	void setAllUnmodified();
	bool getFindParams(QMdiSubWindow *subWindow,QString& str1, QString& str2, DocFindFlags& flags);
	QString lastFindText() const;
	DocFindFlags lastFlags() const; 
	bool appendKnownFileExtensionList(QString strFileExtLst);
	QString getKnownFileExtensionList() {return strFileExtensionList;};
	bool appendKnownDocumentFileHandlerList(const QString &strDocHandlerInfo, QObject *pluginObject);
	int getKnownDocumentFileHandlerIndex(const QString &strExtension);
	QObject *getKnownDocumentFileHandlerInformation(const int &nIndex, QString &strDocHndlrName);
	int addAdditionalApiEntry(const QString &entry); 

public slots:
	void signalDocManagerOutput(QString strText2Output) {emit DocumentManagerOutput(strText2Output);};	

private:
	DocFindFlags lastFlags_;
	QSignalMapper *DocModifiedMapper;
	QSignalMapper *NrOfLinesChangedMapper;
	QString strFileExtensionList;
	strcPluginDocHandlerInfo pluginDocHandlerStore;	
	QList<strDocManagerDocument> lChildDocuments;
	QStringList additionalApiEntries;

	void setFileName(int DocIndex, QString fileName);
	bool getLexer(QsciLexer *lexer, const QString &lexerName, QObject *parent = 0);
	QStringList getAdditionalApiEntries() {return additionalApiEntries;};
	bool customizeDocumentStyle(CustomQsciScintilla *custQsci,GlobalApplicationInformation::DocTypeStyle dStyle = GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED, const QString &strAPIFileName = "");

private slots:
	void documentWasModified(QWidget *subWindow);
	void updateLineNumbers(QWidget *tmpSci);
	void onMarginClicked (int margin, int line, Qt::KeyboardModifiers state);
	void configureDocumentModifiedSetting(const QString &sCanoFilePath, const bool &bIsModified);
	void processDocumentClosing(const QString &sCanoFilePath, const bool &bAutoSaveChanges);
	//void updateLineNumbers(void);
};

#endif // DOCUMENTMANAGER_H
