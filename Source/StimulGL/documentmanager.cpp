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


#include "documentmanager.h"
#include "mainappinfo.h"
#include "StimulGL.h"
#include <QColor>

DocumentManager::DocumentManager(QObject *parent)
	: QObject(parent)
{
	DocCount = 0;
	DocModifiedMapper = NULL;
	NrOfLinesChangedMapper = NULL;
	DocModifiedMapper = new QSignalMapper(this);
	strFileExtensionList = "";
}

DocumentManager::~DocumentManager()
{
	if (DocModifiedMapper)
		delete DocModifiedMapper;
	if (NrOfLinesChangedMapper)
		delete NrOfLinesChangedMapper;
}

CustomQsciScintilla *DocumentManager::getDocHandler(const int &DocIndex)
{
	if (DocCount > 0)
	{
		return QScintillaChildren.at(DocIndex);
	}
	else
	{
		return 0;
	}
}

CustomQsciScintilla *DocumentManager::getDocHandler(QMdiSubWindow *subWindow)
{
	int i;
	for (i=0;i<DocCount;i++)
	{
		if (SubWindowChildren.at(i) == subWindow)
		{
			return QScintillaChildren.at(i);
		}
	}
	return 0;
}

GlobalApplicationInformation::DocType DocumentManager::getDocType(QMdiSubWindow *subWindow)
{
	int i;
	for (i=0;i<DocCount;i++)
	{
		if (SubWindowChildren.at(i) == subWindow)
		{
			return ChildrenDocTypes.at(i);
		}
	}
	return GlobalApplicationInformation::DOCTYPE_UNDEFINED;
}

QString DocumentManager::getFileName(int DocIndex, bool bFileNameOnly)
{
	if (bFileNameOnly)
	{
		QFileInfo fInfo(ChildrenFileNames.at(DocIndex));
		return fInfo.fileName();
	}
	return ChildrenFileNames.at(DocIndex); 
}

QString DocumentManager::getFilePath(QMdiSubWindow *subWindow)
{
	int i;
	for (i=0;i<DocCount;i++)
	{
		if (SubWindowChildren.at(i) == subWindow)
		{
			if (ChildrenFileNames.at(i).isEmpty())
					return "";
			QFileInfo fInfo(ChildrenFileNames.at(i));
			return fInfo.absolutePath();
		}
	}
	return "";
}

QString DocumentManager::getFileName(QMdiSubWindow *subWindow, bool bFileNameOnly)
{
	int i;
	for (i=0;i<DocCount;i++)
	{
		if (SubWindowChildren.at(i) == subWindow)
		{
			if (bFileNameOnly)
			{
				QFileInfo fInfo(ChildrenFileNames.at(i));
				return fInfo.fileName();
			}
			return ChildrenFileNames.at(i);
		}
	}
	return "";
}

int DocumentManager::getDocIndex(QMdiSubWindow *subWindow)
{
	if(subWindow == NULL)
		return -1;
	for (int i=0;i<DocCount;i++)
	{
		if (SubWindowChildren.at(i) == subWindow)
		{
			return i;
		}
	}
	return -1;
}

int DocumentManager::count(void)
{
	return DocCount;
}

GlobalApplicationInformation::DocType DocumentManager::getDocType(const QString strExtension)
{
	QString tmpExt = strExtension.toLower();
	if (tmpExt == "qs")
	{
		return GlobalApplicationInformation::DOCTYPE_QSCRIPT;
	}
	else if (tmpExt == "js")
	{
		return GlobalApplicationInformation::DOCTYPE_JAVASCRIPT;
	}
	else if ((tmpExt == "gz") || (tmpExt == "svg") || (tmpExt == "svgz")) 
	{
		return GlobalApplicationInformation::DOCTYPE_SVG;
	}
	else if(getKnownDocumentFileHandlerIndex(tmpExt) >= 0)
	{
		return GlobalApplicationInformation::DOCTYPE_PLUGIN_DEFINED;
	}
	else//extension not found
	{
		return GlobalApplicationInformation::DOCTYPE_UNDEFINED;
	}
}

bool DocumentManager::getLexer(QsciLexer *lexer, const QString &lexerName, QObject *parent)
{
	//QsciLexer* lexer = 0;	
	QString fileName = "";
	QDir dir(MainAppInfo::apiDirPath());
	bool bRetVal = false;

	//((QsciLexerCPP*)lexer).setFoldComments(true);
	//loadAutocompletionAPI("JavaScript", lexer);
	//fileName.replace(QString("+"), "plus").replace(QString("#"), "sharp");

	if (lexerName == "qscript")
	{
		lexer = new QsciLexerJavaScript(parent);
		fileName = "qscript.api";
		bRetVal = true;
	}
	else if (lexerName == "text")
	{
		//lexer = new QsciLexer(parent);
		bRetVal = false;
	}
	else
	{
		bRetVal = false;
	}

	if (bRetVal)
	{
		//only when a specific lexer is defined...
		if ( dir.entryList(QDir::Files).contains(fileName) ) 
		{
			QsciAPIs* apis = new QsciAPIs(lexer);
			if ( apis->load(dir.absoluteFilePath(fileName)) ) 
			{
				//apis->add("text");
				apis->prepare();
				lexer->setAPIs(apis);				
			}
			else 
			{
				delete apis;
			}
		}
	}
	return bRetVal;
}

bool DocumentManager::customizeDocumentStyle(CustomQsciScintilla *custQsci, GlobalApplicationInformation::DocTypeStyle dStyle, const QString &strAPIFileName)
{
	QColor cPaper(STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_RED,STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_GREEN,STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_BLUE);
	QDir dir(MainAppInfo::apiDirPath());

	switch (dStyle)
	{
		case GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED:
		{
			custQsci->setFolding(QsciScintilla::NoFoldStyle);
			custQsci->setAutoCompletionSource(QsciScintilla::AcsAll);
			custQsci->setPaper(cPaper);
			custQsci->setBraceMatching(QsciScintilla::SloppyBraceMatch);//before or after cursor
			custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
			custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
			return true;
		}
		case GlobalApplicationInformation::DOCTYPE_STYLE_PLAINTEXT:
			{
				custQsci->setFolding(QsciScintilla::NoFoldStyle);
				custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
				custQsci->setPaper(cPaper);
				custQsci->setBraceMatching(QsciScintilla::NoBraceMatch);
				custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
				custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
				return true;
			}
		case GlobalApplicationInformation::DOCTYPE_STYLE_ECMA:
		{
			custQsci->setFolding(QsciScintilla::CircledTreeFoldStyle,2);
			custQsci->setAutoCompletionSource(QsciScintilla::AcsAll);
			QsciLexer *Qjslexer = new QsciLexerJavaScript(custQsci);
			custQsci->setLexer(Qjslexer);
			Qjslexer->setPaper(cPaper);//Here we need to set it again because the Lexer overwrites the previously stored settings.
			if(strAPIFileName.isEmpty() == false)
			{
				if ( dir.entryList(QDir::Files).contains(strAPIFileName) ) 
				{
					QsciAPIs* apis = new QsciAPIs(Qjslexer);
					if ( apis->load(dir.absoluteFilePath(strAPIFileName)) ) 
					{
						custQsci->setCallTipsStyle(QsciScintilla::CallTipsStyle::CallTipsNoAutoCompletionContext);
						QStringList apiEntries = getAdditionalApiEntries();
						if (apiEntries.isEmpty() == false)
						{
							for (int i=0;i<apiEntries.count();i++)
							{
								apis->add(apiEntries.at(i));
							}
						}
						apis->prepare();
						Qjslexer->setAPIs(apis);
					}
					else 
					{
						delete apis;
					}
				}
			}
			custQsci->setBraceMatching(QsciScintilla::SloppyBraceMatch);//before or after cursor
			custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
			custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
			return true;
		}
		case GlobalApplicationInformation::DOCTYPE_STYLE_XML:
		{
			custQsci->setFolding(QsciScintilla::CircledTreeFoldStyle,2);
			custQsci->setAutoCompletionSource(QsciScintilla::AcsAll);
			QsciLexer *QxmlLexer = new QsciLexerXML(custQsci);
			custQsci->setLexer(QxmlLexer);
			QxmlLexer->setPaper(cPaper);//Here we need to set it again because the Lexer overwrites the previously stored settings.
			if(strAPIFileName.isEmpty() == false)
			{
				if ( dir.entryList(QDir::Files).contains(strAPIFileName) ) 
				{
					QsciAPIs* apis = new QsciAPIs(QxmlLexer);
					if ( apis->load(dir.absoluteFilePath(strAPIFileName)) ) 
					{
						apis->prepare();
						QxmlLexer->setAPIs(apis);
					}
					else 
					{
						delete apis;
					}
				}
			}
			custQsci->setBraceMatching(QsciScintilla::SloppyBraceMatch);//before or after cursor
			custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
			custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
			return true;
		}
		case GlobalApplicationInformation::DOCTYPE_STYLE_QML:
			{
				custQsci->setFolding(QsciScintilla::CircledTreeFoldStyle,2);
				custQsci->setAutoCompletionSource(QsciScintilla::AcsAll);
				QsciLexer *Qjslexer = new QsciLexerJavaScript(custQsci);
				custQsci->setLexer(Qjslexer);
				Qjslexer->setPaper(cPaper);//Here we need to set it again because the Lexer overwrites the previously stored settings.
				if(strAPIFileName.isEmpty() == false)
				{
					if ( dir.entryList(QDir::Files).contains(strAPIFileName) ) 
					{
						QsciAPIs* apis = new QsciAPIs(Qjslexer);
						if ( apis->load(dir.absoluteFilePath(strAPIFileName)) ) 
						{
							custQsci->setCallTipsStyle(QsciScintilla::CallTipsStyle::CallTipsNoAutoCompletionContext);
							QStringList apiEntries = getAdditionalApiEntries();
							if (apiEntries.isEmpty() == false)
							{
								for (int i=0;i<apiEntries.count();i++)
								{
									apis->add(apiEntries.at(i));
								}
							}
							apis->prepare();
							Qjslexer->setAPIs(apis);
						}
						else 
						{
							delete apis;
						}
					}
				}
				custQsci->setBraceMatching(QsciScintilla::SloppyBraceMatch);//before or after cursor
				custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
				custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
				return true;
			}
	}
	return false;
}

CustomQsciScintilla *DocumentManager::add(GlobalApplicationInformation::DocType docType,int &DocIndex, const QString &strExtension)
{
	QColor cPaper(STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_RED,STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_GREEN,STIMULGL_DEFAULT_WINDOW_BACKGROUND_COLOR_BLUE);
	CustomQsciScintilla *custQsci = new CustomQsciScintilla(docType);
	custQsci->setPaper(QColor(255,255,255));
	custQsci->setAutoIndent(true);
	custQsci->setAutoCompletionThreshold(2);
	custQsci->setCaretLineVisible(true);//selected line
	custQsci->setCaretLineBackgroundColor(QColor(240,240,255));
	custQsci->setMarginLineNumbers(1, true);
	custQsci->setMarginSensitivity(1, true);
	custQsci->setMarginWidth(2, 12);//	Show the first two margins (number 1 and 2) --> (binary mask 00000110 == 6)	
	custQsci->setMarginMarkerMask(1, 6);//	Set the 1st margin to accept markers	
	custQsci->markerDefine(QsciScintilla::RightTriangle, 1);
	custQsci->markerDefine(QsciScintilla::Background, 2);
	custQsci->setMarkerForegroundColor(QColor(100, 100, 100));
	custQsci->setMarkerBackgroundColor(QColor(160,180,200));
	custQsci->setSelectionBackgroundColor(QColor(150,150,155));//selected text
	custQsci->setSelectionForegroundColor(QColor(100,100,100));
	custQsci->setPaper(cPaper);//Here we make the paper background color light Grey instead of White (Better for the subject watching...)
	NrOfLinesChangedMapper = new QSignalMapper(this);
	NrOfLinesChangedMapper->setMapping(custQsci, custQsci);//QScintillaChildren.at(DocCount)
	connect(custQsci, SIGNAL(linesChanged()),NrOfLinesChangedMapper, SLOT (map()));
	connect(NrOfLinesChangedMapper, SIGNAL(mapped(QWidget *)), this, SLOT(updateLineNumbers(QWidget *)));
	connect(custQsci, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)), SLOT(onMarginClicked(int, int, Qt::KeyboardModifiers)));

	switch (docType)
	{
	case GlobalApplicationInformation::DOCTYPE_QSCRIPT:
		{
			bool bResult = customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_ECMA,"qscript.api");
			break;
		}
	case GlobalApplicationInformation::DOCTYPE_JAVASCRIPT:
		{
			bool bResult = customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_ECMA,"javascript.api");
			break;
		}		
	case GlobalApplicationInformation::DOCTYPE_SVG:
		{
			custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
			break;
		}
	case GlobalApplicationInformation::DOCTYPE_PLUGIN_DEFINED:
		{
			custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
			if(pluginDocHandlerStore.strDocHandlerInfoList.isEmpty() == false)
			{
				if(pluginDocHandlerStore.strDocHandlerInfoList.count() >= 1);
				for (int i=0;i<pluginDocHandlerStore.strDocHandlerInfoList.count();i++)
				{
					if(pluginDocHandlerStore.strDocHandlerInfoList.at(i).split("|",QString::SkipEmptyParts).at(0).toLower() == strExtension.toLower())
					{
						if(pluginDocHandlerStore.pluginObject.at(i))
						{
							QObject* pluginObject = pluginDocHandlerStore.pluginObject.at(i);
							if(pluginObject)
							{
								bool bResult = false;
								GlobalApplicationInformation::DocTypeStyle sDocStyle = GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED;
								if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_TYPESTYLE_FULL)) == -1))//Is the slot present?
								{
									
									int nTmpRetVal = 0;
									bResult = QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_TYPESTYLE),Qt::DirectConnection, Q_RETURN_ARG(int,nTmpRetVal), Q_ARG(QString,strExtension.toLower()));				
									if(bResult)
										sDocStyle = (GlobalApplicationInformation::DocTypeStyle)nTmpRetVal;						
								}

								QString sAPIFileName = "";
								if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_API_FILENAME_FULL)) == -1))//Is the slot present?
								{
									bResult = QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_API_FILENAME),Qt::DirectConnection, Q_RETURN_ARG(QString,sAPIFileName), Q_ARG(QString,strExtension.toLower()));		
								}
								bResult = customizeDocumentStyle(custQsci,sDocStyle,sAPIFileName);
								break;
							}
							break;
						}
						break;
					}
				}
			}
			break;
		}
	case GlobalApplicationInformation::DOCTYPE_UNDEFINED:
		{
			custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
			break;
		}
	default://if none of the above match...
		{
			custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
			break;
		}
	}
	QScintillaChildren.append(custQsci);
	ChildrenDocTypes.append(docType);
	ChildrenFileNames.append("");
	ChildrenModification.append(false);
	DocIndex = DocCount;
	DocCount++;
	return custQsci;
}

int DocumentManager::addAdditionalApiEntry(const QString &entry)
{
	if(additionalApiEntries.contains(entry,Qt::CaseSensitive) == false)
	{
		additionalApiEntries.append(entry);
		//qDebug() << additionalApiEntries.at(additionalApiEntries.count()-1);
		return additionalApiEntries.count() -1;
	}
	return -1;
}

void DocumentManager::onMarginClicked (int margin, int line, Qt::KeyboardModifiers state) 
{
	CustomQsciScintilla *tmpScintilla = dynamic_cast<CustomQsciScintilla *>(sender());
	if ( margin == 1 )//	margin that contains line numbers 
	{
		tmpScintilla->toggleMarker(line);
	}
}

void DocumentManager::updateLineNumbers(QWidget *tmpSci) 
{
	CustomQsciScintilla *tmpScintilla = dynamic_cast<CustomQsciScintilla *>(tmpSci);
	int nrOfLines = tmpScintilla->lines();
	QString str = QString("00%1").arg(nrOfLines);
	tmpScintilla->setMarginWidth(1, str);
	emit NrOfLinesChanged(nrOfLines);
}

bool DocumentManager::setSubWindow(int DocIndex, QMdiSubWindow *subWindow)
{
	if (SubWindowChildren.count() > DocIndex)
	{
		SubWindowChildren.replace(DocIndex,subWindow);
	}
	else if (SubWindowChildren.count() == DocIndex)
	{
		SubWindowChildren.append(subWindow);
	}
	else 
	{
		return false;
	}
	DocModifiedMapper->setMapping(QScintillaChildren.at(DocIndex), SubWindowChildren.at(DocIndex));
	bool bResult = false;
	bResult = connect(QScintillaChildren.at(DocIndex), SIGNAL(textChanged()),	DocModifiedMapper, SLOT(map()));
	bResult = connect(DocModifiedMapper, SIGNAL(mapped(QWidget *)), this, SLOT(documentWasModified(QWidget *)));

	QScintillaChildren.at(DocIndex)->setManagerObject(this,subWindow);
	return true;
}

bool DocumentManager::loadFile(int DocIndex, const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(0, tr("Read Error"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
		return false;
	}
	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	QScintillaChildren.at(DocIndex)->setText(in.readAll());
	setModFlagAndTitle(DocIndex,false);
	setFileName(DocIndex,fileName);
	QApplication::restoreOverrideCursor();
	return true;	
}

void DocumentManager::documentWasModified(QWidget *subWindow)
{
	QMdiSubWindow *tmpSubWindow = qobject_cast<QMdiSubWindow *>(subWindow);
	int tmpIndex = getDocIndex(tmpSubWindow);    
	if (tmpIndex >= 0)
	{
		if (ChildrenModification.at(tmpIndex) == false)
		{
			setModFlagAndTitle(tmpIndex,true);
		}
	}
}

int DocumentManager::getDocIndex(const QString &DocName)
{
	if(!(DocName==""))
	{
		for (int i=0;i<DocCount;i++)
		{
			if (ChildrenFileNames.at(i) == DocName)
			{
				return i;
			}
		}
	}
	return -1;
}

void DocumentManager::setModFlagAndTitle(const int &DocIndex,bool hasChanges)
{
	ChildrenModification.replace(DocIndex,hasChanges);
	QScintillaChildren.at(DocIndex)->setModified(hasChanges);
	QString tmpTitle = ChildrenFileNames.at(DocIndex);
	if (tmpTitle != "")
	{
		if (hasChanges)
		{
			QScintillaChildren.at(DocIndex)->setWindowTitle(tmpTitle + "*");
		}
		else
		{
			QScintillaChildren.at(DocIndex)->setWindowTitle(tmpTitle);
		}
		
	}
}

void DocumentManager::setFileName(int DocIndex, QString fileName)
{
	fileName = 	QFileInfo(fileName).canonicalFilePath();
	ChildrenFileNames.replace(DocIndex,fileName);
	setModFlagAndTitle(DocIndex,false);
	//QScintillaChildren.at(DocIndex)->setWindowTitle(fileName);	
}

bool DocumentManager::appendKnownFileExtensionList(QString strFileExtLst)
{
	strFileExtensionList = strFileExtensionList + strFileExtLst;
	return true;
}

bool DocumentManager::appendKnownDocumentFileHandlerList(const QString &strDocHandlerInfo, QObject *pluginObject)
{
	if(pluginObject)
	{
		if(pluginDocHandlerStore.strDocHandlerInfoList.contains(strDocHandlerInfo) == false)
		{
			pluginDocHandlerStore.strDocHandlerInfoList.append(strDocHandlerInfo);
			pluginDocHandlerStore.pluginObject.append(pluginObject);
			return true;
		}
	}
	return false;
}

QObject *DocumentManager::getKnownDocumentFileHandlerInformation(const int &nIndex, QString &strDocHndlrName)
{
	//See below function!
	if (pluginDocHandlerStore.strDocHandlerInfoList.count() > nIndex)
	{
		QStringList tmpList = pluginDocHandlerStore.strDocHandlerInfoList.at(nIndex).split("|",QString::SkipEmptyParts);
		if (tmpList.count() > 1)
		{
			strDocHndlrName = tmpList.at(1);
			return pluginDocHandlerStore.pluginObject.at(nIndex);
		}
	}
	return NULL;
}

int DocumentManager::getKnownDocumentFileHandlerIndex(const QString &strExtension)
{
	//See above function!
	int nCount = pluginDocHandlerStore.strDocHandlerInfoList.count();
	if (nCount > 0)
	{
		QStringList tmpList;
		for (int i=0;i<nCount;i++)
		{
			tmpList = pluginDocHandlerStore.strDocHandlerInfoList.at(i).split("|",QString::SkipEmptyParts);
			if (tmpList.count() > 1)
			{
				if(tmpList.at(0) == strExtension)
					return i;
			}
		}
	}
	return -1;
}

bool DocumentManager::saveFile(int DocIndex, QString fileName)
{
	if (fileName == "")
	{
		fileName = getFileName(DocIndex);
		if (fileName == "")//Untitled document
		{
			fileName = QFileDialog::getSaveFileName(0, tr("Save As"),qApp->applicationDirPath(),strFileExtensionList);
			if (fileName.isEmpty())
			{
				return false;
			}
		}
	}
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(0, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << QScintillaChildren.at(DocIndex)->text();
	QScintillaChildren.at(DocIndex)->setModified(false);
	QApplication::restoreOverrideCursor();

	setFileName(DocIndex,fileName);
	return true;
}

bool DocumentManager::saveFile(QMdiSubWindow *subWindow, QString fileName )
{
	return saveFile(getDocIndex(subWindow),fileName);
}

bool DocumentManager::maybeSave(QMdiSubWindow *subWindow, bool bAutoSaveChanges)
{
	int i;
	if(subWindow == 0)
	{
		return true;
	}
	else
	{
		for (i=0;i<DocCount;i++)
		{
			if (SubWindowChildren.at(i) == subWindow)
			{
				if (QScintillaChildren.at(i)->isModified()) {
					if(!bAutoSaveChanges)
					{
						int ret = QMessageBox::warning(0, ChildrenFileNames.at(i),
							tr("The document has been modified.\n"
							"Do you want to save your changes?"),
							QMessageBox::Yes | QMessageBox::Default,
							QMessageBox::No,
							QMessageBox::Cancel | QMessageBox::Escape);
						if (ret == QMessageBox::Yes)
							return saveFile(i);
						else if (ret == QMessageBox::Cancel)
							return false;
					}
					else
					{
						return saveFile(i);
					}
				}
				return true;
			}
		}
		return true;
	}
}

bool DocumentManager::remove(QMdiSubWindow *subWindow)
{
	int i;
 	for (i=0;i<DocCount;i++)
	{
		if (SubWindowChildren.at(i) == subWindow)
		{
			ChildrenFileNames.removeAt(i);
			ChildrenDocTypes.removeAt(i);
			QScintillaChildren.removeAt(i);
			SubWindowChildren.removeAt(i);
			ChildrenModification.removeAt(i);
			DocCount --;
			return true;
		}
	}
	return false;
}

void DocumentManager::setAllUnmodified()
{
	for (int i=0;i<DocCount;i++)
	{
		QScintillaChildren.at(i)->setModified(false);
	}
}

bool DocumentManager::getFindParams(QMdiSubWindow *subWindow,QString& str1, QString& str2, DocFindFlags& flags) 
{
	sciFindDialog dlg(subWindow, flags.replace);
	dlg.setText(str1);
	if ( dlg.exec() == QDialog::Accepted ) {
		str1 = dlg.text();
		flags = dlg.flags();
		if ( dlg.isReplaceMode() ) {
			str2 = dlg.replaceTo();
			flags.replace = true;
		}
		lastFlags_ = flags;
		return true;
	}
	return false;
}

QString DocumentManager::lastFindText() const 
{
	return sciFindDialog::lastText();
}

DocFindFlags DocumentManager::lastFlags() const 
{
	return lastFlags_;
}