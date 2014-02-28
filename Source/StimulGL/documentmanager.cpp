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


#include "documentmanager.h"
#include "mainappinfo.h"
#include "StimulGL.h"
#include <QColor>
#include <QFileInfo>

DocumentManager::DocumentManager(QObject *parent)
	: QObject(parent)
{
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

QWidget *DocumentManager::getDocHandler(const int &nDocIndex)
{
	if (lChildDocuments.count() > nDocIndex)
	{
		return lChildDocuments.at(nDocIndex).pMDISubWin;
	}
	else
	{
		return NULL;
	}
}

QWidget *DocumentManager::getDocHandler(QMdiSubWindow *subWindow)
{
	if(lChildDocuments.isEmpty() == false)
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				return lChildDocuments.at(i).pMDISubWin;
			}
		}
	}
	return NULL;
}

GlobalApplicationInformation::DocType DocumentManager::getDocType(QMdiSubWindow *subWindow)
{	
	if(lChildDocuments.isEmpty() == false)
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				return lChildDocuments.at(i).dDocType;
			}
		}
	}
	return GlobalApplicationInformation::DOCTYPE_UNDEFINED;
}

QString DocumentManager::getFileName(int DocIndex, bool bFileNameOnly)
{
	if(lChildDocuments.count() > DocIndex)
	{
		if (bFileNameOnly)
			return QFileInfo(lChildDocuments.at(DocIndex).sFileName).fileName();	
		return lChildDocuments.at(DocIndex).sFileName;
	}
	return "";	 
}

QString DocumentManager::getFilePath(QMdiSubWindow *subWindow)
{
	if(lChildDocuments.isEmpty()==false)
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				if (lChildDocuments.at(i).sFileName.isEmpty())
					return "";
				return QFileInfo(lChildDocuments.at(i).sFileName).absolutePath();
			}
		}
	}
	return "";
}

QString DocumentManager::getFileName(QMdiSubWindow *subWindow, bool bFileNameOnly)
{
	if(lChildDocuments.isEmpty() == false)
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				if (bFileNameOnly)
					return QFileInfo(lChildDocuments.at(i).sFileName).fileName();
				return lChildDocuments.at(i).sFileName;
			}
		}
	}
	return "";
}

int DocumentManager::getDocIndex(QMdiSubWindow *subWindow)
{
	if(subWindow == NULL)
		return -1;
	if(lChildDocuments.isEmpty() == false)
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				return i;
			}
		}
	}
	return -1;
}

int DocumentManager::count(void)
{
	return lChildDocuments.count();
}

GlobalApplicationInformation::DocType DocumentManager::getDocType(const QString strExtension)
{
	QString tmpExt = strExtension.toLower();
	if (tmpExt == "qs")
	{
		return GlobalApplicationInformation::DOCTYPE_QTSCRIPT;
	}
	else if (tmpExt == "js")
	{
		return GlobalApplicationInformation::DOCTYPE_JAVASCRIPT;
	}
	else if ((tmpExt == "gz") || (tmpExt == "svg") || (tmpExt == "svgz")) 
	{
		return GlobalApplicationInformation::DOCTYPE_SVG;
	}
	else if (tmpExt == "xml") 
	{
		return GlobalApplicationInformation::DOCTYPE_XML;
	}
	else if (tmpExt == "py") 
	{
		return GlobalApplicationInformation::DOCTYPE_PYTHON;
	}
	else if (tmpExt == "perl") 
	{
		return GlobalApplicationInformation::DOCTYPE_PERL;
	}
	else if ((tmpExt == "c") || (tmpExt == "cpp") || (tmpExt == "h")) 
	{
		return GlobalApplicationInformation::DOCTYPE_CPP;
	}
	else if ((tmpExt == "htm") || (tmpExt == "html")) 
	{
		return GlobalApplicationInformation::DOCTYPE_HTML;
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

	if(dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED)
	{
		custQsci->setFolding(QsciScintilla::NoFoldStyle);
		custQsci->setAutoCompletionSource(QsciScintilla::AcsAll);
		custQsci->setPaper(cPaper);
		custQsci->setBraceMatching(QsciScintilla::SloppyBraceMatch);//before or after cursor
		custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
		custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
		return true;
	}
	else if(dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_PLAINTEXT)
	{
		custQsci->setFolding(QsciScintilla::NoFoldStyle);
		custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
		custQsci->setPaper(cPaper);
		custQsci->setBraceMatching(QsciScintilla::NoBraceMatch);
		custQsci->setMatchedBraceBackgroundColor(QColor(255,255,120));
		custQsci->setMatchedBraceForegroundColor(QColor(0,0,255));
		return true;
	}
	else if(dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_XML)
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
	else if( (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_ECMA) || (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_QML) ||
			 (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_PYTHON) || (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_PERL) ||
			 (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_CPP) || (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_HTML) )
	{
		custQsci->setFolding(QsciScintilla::CircledTreeFoldStyle,2);
		custQsci->setAutoCompletionSource(QsciScintilla::AcsAll);

		QsciLexer *tmpLexer = NULL;
		if(dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_ECMA)
			tmpLexer = new QsciLexerJavaScript(custQsci);
		else if (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_QML)
			tmpLexer = new QsciLexerJavaScript(custQsci);
		else if (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_PYTHON)
			tmpLexer = new QsciLexerPython(custQsci);
		else if (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_PERL)
			tmpLexer = new QsciLexerPerl(custQsci);
		else if (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_CPP)
			tmpLexer = new QsciLexerCPP(custQsci);
		else if (dStyle == GlobalApplicationInformation::DOCTYPE_STYLE_HTML)
			tmpLexer = new QsciLexerHTML(custQsci);

		custQsci->setLexer(tmpLexer);
		tmpLexer->setPaper(cPaper);//Here we need to set it again because the Lexer overwrites the previously stored settings.
		if(strAPIFileName.isEmpty() == false)
		{
			if ( dir.entryList(QDir::Files).contains(strAPIFileName) ) 
			{
				QsciAPIs* apis = new QsciAPIs(tmpLexer);
				if ( apis->load(dir.absoluteFilePath(strAPIFileName)) ) 
				{
					custQsci->setCallTipsStyle(QsciScintilla::CallTipsNoAutoCompletionContext);
					QStringList apiEntries = getAdditionalApiEntries();
					if (apiEntries.isEmpty() == false)
					{
						for (int i=0;i<apiEntries.count();i++)
						{
							apis->add(apiEntries.at(i));
						}
					}
					apis->prepare();
					tmpLexer->setAPIs(apis);
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
	return false;
}

QWidget *DocumentManager::add(GlobalApplicationInformation::DocType docType,int &DocIndex, const QString &strExtension, const QString &strCanonicalFilePath)
{
	GlobalApplicationInformation::DocTypeStyle sDocStyle = GlobalApplicationInformation::DOCTYPE_STYLE_UNDEFINED;
	QObject* pluginObject = NULL;
	QWidget* editorObject = NULL;
	QString sAPIFileName = "";
	strDocManagerDocument tmpChildDoc;
	bool bResult = false;

	if(docType == GlobalApplicationInformation::DOCTYPE_PLUGIN_DEFINED)
	{
		if(pluginDocHandlerStore.strDocHandlerInfoList.isEmpty() == false)
		{
			for (int i=0;i<pluginDocHandlerStore.strDocHandlerInfoList.count();i++)
			{
				if(pluginDocHandlerStore.strDocHandlerInfoList.at(i).split("|",QString::SkipEmptyParts).at(0).toLower() == strExtension.toLower())
				{
					if(pluginDocHandlerStore.pluginObject.at(i))
					{
						pluginObject = pluginDocHandlerStore.pluginObject.at(i);
						if(pluginObject)
						{							
							if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_TYPESTYLE_FULL)) == -1))//Is the slot present?
							{

								int nTmpRetVal = 0;
								bResult = QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_TYPESTYLE),Qt::DirectConnection, Q_RETURN_ARG(int,nTmpRetVal), Q_ARG(QString,strExtension.toLower()));				
								if(bResult)
									sDocStyle = (GlobalApplicationInformation::DocTypeStyle)nTmpRetVal;						
							}
							if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_API_FILENAME_FULL)) == -1))//Is the slot present?
							{
								bResult = QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_API_FILENAME),Qt::DirectConnection, Q_RETURN_ARG(QString,sAPIFileName), Q_ARG(QString,strExtension.toLower()));		
							}
							if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_TYPEEDITOR_FULL)) == -1))//Is the slot present?
							{
								bResult = QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_GETADDFILE_TYPEEDITOR),Qt::DirectConnection, Q_RETURN_ARG(QWidget *,editorObject), Q_ARG(QString,strExtension.toLower()), Q_ARG(QString,strCanonicalFilePath));
							}	
							break;
						}
						break;
					}
					break;
				}
			}
		}
	}

	if(editorObject == NULL)
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
		NrOfLinesChangedMapper->setMapping(custQsci, custQsci);//QWidgetChildren.at(DocCount)
		connect(custQsci, SIGNAL(linesChanged()),NrOfLinesChangedMapper, SLOT (map()));
		connect(NrOfLinesChangedMapper, SIGNAL(mapped(QWidget *)), this, SLOT(updateLineNumbers(QWidget *)));
		connect(custQsci, SIGNAL(marginClicked(int, int, Qt::KeyboardModifiers)), SLOT(onMarginClicked(int, int, Qt::KeyboardModifiers)));

		switch (docType)
		{
		case GlobalApplicationInformation::DOCTYPE_QTSCRIPT:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_ECMA,"qscript.api");
				break;
			}
		case GlobalApplicationInformation::DOCTYPE_JAVASCRIPT:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_ECMA,"javascript.api");
				break;
			}		
		case GlobalApplicationInformation::DOCTYPE_SVG:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_XML,"xml.api");
				custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
				break;
			}
		case GlobalApplicationInformation::DOCTYPE_XML:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_XML,"xml.api");
				break;
			}	
		case GlobalApplicationInformation::DOCTYPE_PYTHON:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_PYTHON,"python.api");
				break;
			}	
		case GlobalApplicationInformation::DOCTYPE_PERL:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_PERL,"perl.api");
				break;
			}	
		case GlobalApplicationInformation::DOCTYPE_CPP:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_CPP,"cplusplus.api");
				break;
			}	
		case GlobalApplicationInformation::DOCTYPE_HTML:
			{
				customizeDocumentStyle(custQsci,GlobalApplicationInformation::DOCTYPE_STYLE_HTML,"html.api");
				break;
			}	
		case GlobalApplicationInformation::DOCTYPE_PLUGIN_DEFINED:
			{
				custQsci->setAutoCompletionSource(QsciScintilla::AcsNone);
				if(pluginObject)
					bResult = customizeDocumentStyle(custQsci,sDocStyle,sAPIFileName);
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
		tmpChildDoc.pWidget = custQsci;
		editorObject = qobject_cast<QWidget*>(custQsci);
		//DocIndex = DocCount;
		//DocCount++;
		//return custQsci;
	}
	else
	{
		tmpChildDoc.pWidget = editorObject;
	}
	tmpChildDoc.dDocType = docType;
	tmpChildDoc.sFileName = "";
	tmpChildDoc.bIsModified = false;
	DocIndex = lChildDocuments.count();
	lChildDocuments.append(tmpChildDoc);
	return editorObject;
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
	Q_UNUSED(state);
	CustomQsciScintilla *tmpScintilla = dynamic_cast<CustomQsciScintilla *>(sender());
	if ( margin == 1 )//	margin that contains line numbers 
	{
		tmpScintilla->toggleMarker(line);
		emit MarkerToggled(line);
	}
}

void DocumentManager::updateLineNumbers(QWidget *tmpSci) 
{
	CustomQsciScintilla *tmpScintilla = qobject_cast<CustomQsciScintilla *>(tmpSci);
	if(tmpSci)
	{
		int nrOfLines = tmpScintilla->lines();
		QString str = QString("00%1").arg(nrOfLines);
		tmpScintilla->setMarginWidth(1, str);
		emit NrOfLinesChanged(nrOfLines);
	}
}

bool DocumentManager::setSubWindow(int DocIndex, QMdiSubWindow *subWindow)
{
	if (lChildDocuments.count() > DocIndex)
	{
		lChildDocuments[DocIndex].pMDISubWin = subWindow;
	}
	else if (lChildDocuments.count() == DocIndex)
	{
		strDocManagerDocument tmpDoc;
		tmpDoc.pMDISubWin = subWindow;
		lChildDocuments.append(tmpDoc);		
	}
	else 
	{
		return false;
	}
	bool bResult = false;	
	if(lChildDocuments.count() > DocIndex)
	{
		if(lChildDocuments.at(DocIndex).pWidget)
		{
			if (lChildDocuments.at(DocIndex).pWidget->metaObject()->indexOfSignal(lChildDocuments.at(DocIndex).pWidget->metaObject()->normalizedSignature("textChanged()")) != -1)//Is the signal present?
			{
				bResult = connect(lChildDocuments.at(DocIndex).pWidget, SIGNAL(textChanged()),	DocModifiedMapper, SLOT(map()));
				DocModifiedMapper->setMapping(lChildDocuments.at(DocIndex).pWidget, lChildDocuments.at(DocIndex).pMDISubWin);
			}
		}
	}	
	bResult = connect(DocModifiedMapper, SIGNAL(mapped(QWidget *)), this, SLOT(documentWasModified(QWidget *)));

	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(lChildDocuments.at(DocIndex).pWidget);
	if(tmpCustomQsciScintilla)
		tmpCustomQsciScintilla->setManagerObject(this,subWindow);
	return true;
}

bool DocumentManager::loadFile(int DocIndex, const QString &fileName)
{
	QFile file(fileName);
	if(file.exists() == false)
		return false;
	
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(lChildDocuments.at(DocIndex).pWidget);
	bool bRetval = false;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if(tmpCustomQsciScintilla)
	{
		if (!file.open(QFile::ReadOnly | QFile::Text)) 
		{
			QMessageBox::warning(0, tr("Read Error"), tr("Cannot read file %1:\n%2.").arg(fileName).arg(file.errorString()));
			QApplication::restoreOverrideCursor();
			return false;
		}
		QTextStream in(&file);
		tmpCustomQsciScintilla->setText(in.readAll());
		bRetval = true;
	}
	else
	{
		if(pluginDocHandlerStore.pluginObject.at(DocIndex))
		{
			QObject *pluginObject = pluginDocHandlerStore.pluginObject.at(DocIndex);
			if(pluginObject)
			{							
				if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_LOADADDFILE_FULL)) == -1))//Is the slot present?
				{
					QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_LOADADDFILE),Qt::DirectConnection, Q_RETURN_ARG(bool,bRetval), Q_ARG(QString,fileName));
				}
				if(bRetval)
				{
					int nSignalIndex = pluginObject->metaObject()->indexOfSignal(QMetaObject::normalizedSignature(FUNC_PLUGIN_DOCUMENTHASCHANGED_FULL));
					int nSlotIndex = this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STIMULGL_CONFDOCMODSETTING_FULL));
					if ((nSignalIndex >= 0) && (nSlotIndex >= 0))//Is the signal&slot present?
						QMetaObject::connect(pluginObject,nSignalIndex,this,nSlotIndex,Qt::DirectConnection);

					nSignalIndex = pluginObject->metaObject()->indexOfSignal(QMetaObject::normalizedSignature(FUNC_PLUGIN_DOCUMENTISCLOSING_FULL));
					nSlotIndex = this->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_STIMULGL_PROCDOCCLOSING_FULL));
					if ((nSignalIndex >= 0) && (nSlotIndex >= 0))//Is the signal&slot present?
						QMetaObject::connect(pluginObject,nSignalIndex,this,nSlotIndex,Qt::DirectConnection);
				}
			}
		}
	}
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
		if (lChildDocuments.at(tmpIndex).bIsModified == false)
		{
			setModFlagAndTitle(tmpIndex,true);
		}
	}
}

void DocumentManager::configureDocumentModifiedSetting(const QString &sCanoFilePath, const bool &bIsModified)
{
	if(sCanoFilePath.isEmpty() == false)
	{
		int tmpIndex = getDocIndex(sCanoFilePath);    
		if (tmpIndex >= 0)
		{
			if (lChildDocuments.at(tmpIndex).bIsModified != bIsModified)
			{
				setModFlagAndTitle(tmpIndex,bIsModified);
			}
		}
	}
}

void DocumentManager::processDocumentClosing(const QString &sCanoFilePath, const bool &bAutoSaveChanges)
{
	bool bResult = false;
	if(sCanoFilePath.isEmpty() == false)
	{
		int tmpIndex = getDocIndex(sCanoFilePath);    
		if (tmpIndex >= 0)
		{
			if(lChildDocuments.at(tmpIndex).sFileName != sCanoFilePath)//file target changed?
				lChildDocuments[tmpIndex].sFileName = sCanoFilePath;
			bResult = maybeSave(lChildDocuments.at(tmpIndex).pMDISubWin,bAutoSaveChanges);
			//if(bResult)
			//{
				if(pluginDocHandlerStore.pluginObject.at(tmpIndex))
				{
					QObject *pluginObject = pluginDocHandlerStore.pluginObject.at(tmpIndex);
					if(pluginObject)
					{							
						if (!(pluginObject->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_REMOVEADDFILE_FULL)) == -1))//Is the slot present?
						{
							bool bRetval;
							QMetaObject::invokeMethod(pluginObject,QMetaObject::normalizedSignature(FUNC_PLUGIN_REMOVEADDFILE),Qt::DirectConnection, Q_RETURN_ARG(bool,bRetval), Q_ARG(QString,sCanoFilePath));
						}						
					}
				//}
				remove(lChildDocuments.at(tmpIndex).pMDISubWin);
			}
		}
	}
}

int DocumentManager::getDocIndex(const QString &DocName)
{
	if(!(DocName=="") && (lChildDocuments.isEmpty()==false))
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).sFileName == DocName)
			{
				return i;
			}
		}
	}
	return -1;
}

void DocumentManager::setModFlagAndTitle(const int &DocIndex,bool hasChanges)
{
	if(lChildDocuments.isEmpty())
		return;
	if(lChildDocuments.count() > DocIndex)
	{
		lChildDocuments[DocIndex].bIsModified = hasChanges;
		CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(lChildDocuments.at(DocIndex).pWidget);
		if(tmpCustomQsciScintilla)
			tmpCustomQsciScintilla->setModified(hasChanges);

		QString tmpTitle = lChildDocuments.at(DocIndex).sFileName;
		if (tmpTitle.isEmpty() == false)
		{
			if (hasChanges)
			{
				lChildDocuments.at(DocIndex).pWidget->setWindowTitle(tmpTitle + "*");
			}
			else
			{
				//if(tmpCustomQsciScintilla)
				lChildDocuments.at(DocIndex).pWidget->setWindowTitle(tmpTitle);
			}		
		}
	}
}

void DocumentManager::setFileName(int DocIndex, QString fileName)
{
	if(lChildDocuments.isEmpty())
		return;
	if(lChildDocuments.count()> DocIndex)
	{
		QFileInfo fileInformation(fileName);
		fileName = 	fileInformation.canonicalFilePath();
		lChildDocuments[DocIndex].sFileName = fileName;
		setModFlagAndTitle(DocIndex,false);
		//QWidgetChildren.at(DocIndex)->setWindowTitle(fileName);	
	}
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
			if (tmpList.count() > 0)
			{
				if(tmpList.at(0) == strExtension)
					return i;
			}
		}
	}
	return -1;
}

bool DocumentManager::saveFile(int DocIndex, QString fileName, bool *bReparseDocumentContentNeeded)
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

	QApplication::setOverrideCursor(Qt::WaitCursor);
	
	CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(lChildDocuments.at(DocIndex).pWidget);
	if(tmpCustomQsciScintilla)
	{
		QTextStream out(&file);
		out << tmpCustomQsciScintilla->text();
		tmpCustomQsciScintilla->setModified(false);
	}
	else if(lChildDocuments.at(DocIndex).pWidget)
	{
		if (!(lChildDocuments.at(DocIndex).pWidget->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_WIDGET_SAVEFILE_FULL)) == -1))//Is the slot present?
		{
			QMetaObject::invokeMethod(lChildDocuments.at(DocIndex).pWidget,QMetaObject::normalizedSignature(FUNC_PLUGIN_WIDGET_SAVEFILE),Qt::DirectConnection, Q_ARG(QString,fileName));
		}
	}
	
	QApplication::restoreOverrideCursor();
	
	setFileName(DocIndex,fileName);

	if(bReparseDocumentContentNeeded != NULL)
	{
		QFileInfo fileInformation(fileName);
		QString strNewExtension = fileInformation.completeSuffix();
		GlobalApplicationInformation::DocType newDocType = getDocType(strNewExtension);
		if(lChildDocuments.at(DocIndex).dDocType != newDocType)
		{
			*bReparseDocumentContentNeeded = true;
		}
		else
		{
			*bReparseDocumentContentNeeded = false;
		}		
	}
	return true;
}

bool DocumentManager::saveFile(QMdiSubWindow *subWindow, QString fileName, bool *bReparseDocumentContentNeeded)
{
	int nDocIndex = getDocIndex(subWindow);
	bool bResult = saveFile(nDocIndex,fileName,bReparseDocumentContentNeeded);
	return bResult;
}

bool DocumentManager::maybeSave(QMdiSubWindow *subWindow, bool bAutoSaveChanges)
{
	if(subWindow == NULL)
	{
		return true;
	}
	else
	{
		if(lChildDocuments.isEmpty())
			return true;
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				CustomQsciScintilla *tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(lChildDocuments.at(i).pWidget);
				if (tmpCustomQsciScintilla)
				{
					if (tmpCustomQsciScintilla->isModified()) 
					{
						if(!bAutoSaveChanges)
						{
							int ret = QMessageBox::warning(0, lChildDocuments.at(i).sFileName,
								tr(WARNING_DOCCHANGEDSAVEFILE_STRING),
								QMessageBox::Yes | QMessageBox::Default,
								QMessageBox::No,
								QMessageBox::Cancel | QMessageBox::Escape);
							if (ret == QMessageBox::Yes)
							{
								return saveFile(i);
							}
							else if (ret == QMessageBox::Cancel)
							{
								return false;
							}
						}
						else
						{
							return saveFile(i);
						}
					}
					return true;
				}
				else if(lChildDocuments.at(i).pWidget)//this is coming from a plug-ins document handler..
				{
					if(lChildDocuments.at(i).bIsModified)
					{
						bool bDoSave = false;
						if(!bAutoSaveChanges)
						{
							int ret = QMessageBox::warning(0, lChildDocuments.at(i).sFileName,
								tr(WARNING_DOCCHANGEDSAVEFILE_STRING),
								QMessageBox::Yes | QMessageBox::Default,
								QMessageBox::No,
								QMessageBox::Cancel | QMessageBox::Escape);
							if (ret == QMessageBox::Yes)
							{
								bDoSave = true;
							}
							else if (ret == QMessageBox::Cancel)
							{
								return false;
							}
						}
						else
						{
							bDoSave = true;
						}
						if(bDoSave)
						{
							int nSlotIndex = lChildDocuments.at(i).pWidget->metaObject()->indexOfMethod(QMetaObject::normalizedSignature(FUNC_PLUGIN_WIDGET_SAVEFILE_FULL));
							if (nSlotIndex >= 0)//Is the slot present?
								QMetaObject::invokeMethod(lChildDocuments.at(i).pWidget,FUNC_PLUGIN_WIDGET_SAVEFILE,Qt::DirectConnection,Q_ARG(QString, lChildDocuments.at(i).sFileName));
							return true;
						}

					}
					return true;
				}
			}
		}
		return true;
	}
}

bool DocumentManager::remove(QMdiSubWindow *subWindow)
{
	if(lChildDocuments.isEmpty() == false)
	{
		for (int i=0;i<lChildDocuments.count();i++)
		{
			if (lChildDocuments.at(i).pMDISubWin == subWindow)
			{
				lChildDocuments.removeAt(i);
				return true;
			}
		}
	}
	return false;
}

void DocumentManager::setAllUnmodified()
{
	if(lChildDocuments.isEmpty() == false)
	{
		CustomQsciScintilla *tmpCustomQsciScintilla;
		for (int i=0;i<lChildDocuments.count();i++)
		{
			tmpCustomQsciScintilla = qobject_cast<CustomQsciScintilla*>(lChildDocuments.at(i).pWidget);
			if (tmpCustomQsciScintilla)
				tmpCustomQsciScintilla->setModified(false);
		}
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