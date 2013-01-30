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


#include "customqsciscintilla.h"
#include "documentmanager.h"
#include <QPrintDialog>

CustomQsciScintilla::CustomQsciScintilla(GlobalApplicationInformation::DocType docType)
{	

}

CustomQsciScintilla::~CustomQsciScintilla()
{

}

#ifdef DEBUG

QString CustomQsciScintilla::testFunction(QString inp)
{
	return inp;
	//bool b1 = this->isCallTipActive();
	//this->callTip();
	//bool b2 = this->isCallTipActive();
	
	//callTip();
	
	//return QString("done");

	//this->zoomIn(50);
	
	//void SCN_AUTOCCHARDELETED();
	//bool b4 = connect(this, SIGNAL(textChanged()), SLOT(CustomQsciScintilla::test(QString)));
	//bool b4 = connect(this, SIGNAL(textChanged()), this, SLOT(testFunction()));
	//QStringList aa;
	//aa << "Arial" << "Helvetica" << "Times" << "Courier";
	//this->showUserList(0, aa);

	bool b3 = connect(this, SIGNAL(SCN_CHARADDED(int)), this, SLOT(handleCharAdded(int)));
	connect(this,SIGNAL(SCN_CALLTIPCLICK(int)),SLOT(handleCallTipClick(int)));
	connect(this,SIGNAL(SCN_AUTOCSELECTION(const char *,int)),SLOT(handleAutoCompletionSelection()));
	connect(this,SIGNAL(SCN_USERLISTSELECTION(const char *,int)),SLOT(handleUserListSelection(const char *,int)));

	return QString("done");

	CallTipsStyle a = callTipsStyle();// != CallTipsNone);
	bool b = isCallTipActive();
	int pos = SendScintilla(SCI_GETCURRENTPOS);

	QString ct("sometext\nnextline");
	QByteArray ct_ba = ct.toLatin1();
	const char *cts = ct_ba.data();
	SendScintilla(SCI_CALLTIPSHOW, 50.0, cts);
	int listType = 0;
	//SendScintilla(SCI_USERLISTSHOW,listType,cts);//int listType, const char *list)
	
	QStringList c;
	c << "Arial" << "Helvetica" << "Times" << "Courier";
	showUserList(2,c);
	//SCI_SETKEYWORDS(int keyWordSet, const char *keyWordList)
	//SendScintilla(SCI_SETKEYWORDS, listType, g_cppKeyWords );
	return QString("done");
}

#endif

void CustomQsciScintilla::setManagerObject(QObject *managerObject,QObject *childWindowObject)
{
	manager = managerObject;//qobject_cast<DocumentManager *>(managerObject);
	childWindow = childWindowObject;//qobject_cast<QMdiSubWindow *>(childWindowObject);
	searchStartingLine_ = -1;
	searchStartingCol_ = -1;
	searchSteppedOver_ = false;
	searchStartingScroll_ = 0;
}

void CustomQsciScintilla::closeEvent(QCloseEvent *event)
{
	DocumentManager *tmpDocObject = qobject_cast<DocumentManager *>(manager);
	QMdiSubWindow *tmpWinObject = qobject_cast<QMdiSubWindow *>(childWindow);
	if (tmpDocObject->maybeSave(tmpWinObject,false)) 
	{
		tmpDocObject->remove(tmpWinObject);
		event->accept();
	} 
	else 
	{
		event->ignore();
	}
}

void CustomQsciScintilla::toggleMarker() 
{
	int line, col;
	this->getCursorPosition(&line, &col);
	toggleMarker(line);
}

void CustomQsciScintilla::toggleMarker(int line) 
{
	if ( this->markersAtLine(line) & 2 ) 
	{
		this->markerDelete(line, 1);
	}
	else 
	{
		this->markerAdd(line, 1);
	}
	//	Markers will be added to/deleted from the 2nd this 
	//	automatically since they share the same document
	//  this->toggleMarker(line);
}

void CustomQsciScintilla::toggleLineComment()
{

	QString comment = "//";
	if (this->hasSelectedText()) 
	{
		int line1, col1, line2, col2, curLine, curCol;
		this->getSelection(&line1, &col1, &line2, &col2);
		this->getCursorPosition(&curLine, &curCol);

		QString str1 = text(line1);
		QString ln = str1.simplified();
		bool toComment = true;
		if ( ln.startsWith(comment) ) {
			toComment = false;
		}
		if ( col2 == 0 )
			--line2;
		this->beginUndoAction();
		for ( int line = line1; line <= line2; ++line ) 
		{
			str1 = text(line);
			if ( toComment ) 
			{
				if ( !str1.simplified().startsWith(comment) )
					commentLine(line, str1, comment);
			}
			else 
			{
				if ( str1.simplified().startsWith(comment) )
					uncommentLine(line, str1, comment);
			}
		}
		this->endUndoAction();
		if ( curCol > 0 )
			this->setCursorPosition(curLine, curCol + comment.length() * (toComment ? 1 : -1) );
		else
			this->setCursorPosition(curLine, curCol);
	}
	else 
	{
		int line1, col1;
		this->getCursorPosition(&line1, &col1);
		QString str1 = text(line1);

		QString ln = str1.simplified();
		if ( ln.startsWith(comment) ) {
			uncommentLine(line1, str1, comment);
			this->setCursorPosition(line1, col1 - comment.length());
		}
		else 
		{
			commentLine(line1, str1, comment);
			this->setCursorPosition(line1, col1 + comment.length());
		}
	}
}

void CustomQsciScintilla::toggleBlockComment()
{
	QString commBeg = "/*";
	QString	commEnd = "*/";
	
	if (this->hasSelectedText())
	{
		int line1, col1, line2, col2, curLine, curCol;
		this->getSelection(&line1, &col1, &line2, &col2);
		this->getCursorPosition(&curLine, &curCol);

		QString str1 = this->selectedText();
		bool toComment = true;
		if ( str1.startsWith(commBeg) && str1.endsWith(commEnd) )
			toComment = false;

		QString str2;
		if ( toComment )
			str2 = commBeg + str1 + commEnd;
		else {
			str2 = str1;
			str2.chop(commEnd.length());
			str2.remove(0, commBeg.length());
		}
		replaceSelectedText(str2);
		if ( line1 == line2 ) {
			if ( curCol == col1 )
				this->setCursorPosition(curLine, curCol);
			else
				this->setCursorPosition(curLine, curCol + (commBeg.length() + commEnd.length()) * (toComment ? 1 : -1));
		}
		else {
			if ( curLine == line2 && curCol == col2)
				this->setCursorPosition(curLine, curCol + commEnd.length() * (toComment ? 1 : -1) );
			else
				this->setCursorPosition(curLine, curCol);
		}
	}
}

void CustomQsciScintilla::commentLine(int line, const QString& str1, const QString& comment) 
{
	QString str2 = comment + str1;
	this->setSelection(line, 0, line + 1, 0);
	replaceSelectedText(str2);
}

void CustomQsciScintilla::uncommentLine(int line, const QString& str1, const QString& comment) 
{
	int pos = str1.indexOf(comment);
	QString str2 = str1;
	str2.replace(pos, comment.length(), "");
	this->setSelection(line, 0, line + 1, 0);
	replaceSelectedText(str2);
}

void CustomQsciScintilla::replaceSelectedText(const QString& text) 
{
	this->beginUndoAction();
	removeSelectedText();
	insertText(text);
	this->endUndoAction();
}

void CustomQsciScintilla::insertText(const QString& text) 
{
	int row, col;
	getCursorPosition(&row, &col);//is this correct??????
	int newLines = text.count(QRegExp("\r\n|\n|\r"));
	this->insert(text);
	if ( newLines == 0 )
		setCursorPosition(row, col + text.length());
}

bool CustomQsciScintilla::print() 
{
	QsciPrinter prn;
	QPrintDialog dlg(&prn, this->parentWidget());
	if (dlg.exec() == QDialog::Accepted) {
		prn.setWrapMode(QsciScintilla::WrapNone); //WrapWord

		int line1(-1), line2(-1), col1(-1), col2(-1);
		//////////JuffScintilla* edit = getActiveEdit();
		if ( this ) //this.edit?
		{
			//QsciLexer* lexer = this->lexer();
			///////////////if ( !PrintSettings::keepBgColor() ) {
			//lexer->setDefaultPaper(Qt::white);
			//lexer->setPaper(Qt::white);
			//lexer->setDefaultColor(Qt::black);
			///////////////if ( !PrintSettings::keepColors() ) {
			//lexer->setColor(Qt::black);
			this->getSelection(&line1, &col1, &line2, &col2);
			if (line1 >=0 && line2 >= 0 && col1 >= 0 && col2 >= 0) 
			{
				/////////	We have selection. Print it.
				--line2;
				prn.printRange(this, line1, line2);
			}
			else 
			{
				/////////	We don't have selection. Print the whole text.
				prn.printRange(this, 0);
			}
			//QFont font = TextDocSettings::font();
			//LexerStorage::instance()->updateLexers(font);
			return true;
		}
	}	
	return false;
}

void CustomQsciScintilla::nextMarker() 
{
	int row(-1), col(-1);
	this->getCursorPosition(&row, &col);

	int mLine = this->markerFindNext(row + 1, 2);
	if ( mLine >= 0 ) {
		gotoLine(mLine);
	}
	else {
		mLine = this->markerFindNext(0, 2);
		if ( mLine >= 0 ) {
			gotoLine(mLine);
		}
	}
}

void CustomQsciScintilla::prevMarker() 
{
	int row(-1), col(-1);
	this->getCursorPosition(&row, &col);

	int mLine = this->markerFindPrevious(row - 1 , 2);
	if ( mLine >= 0 ) {
		gotoLine(mLine);
	}
	else {
		mLine = this->markerFindPrevious(lineCount() - 1, 2);
		if ( mLine >= 0 ) {
			gotoLine(mLine);
		}
	}
}

void CustomQsciScintilla::removeAllMarkers() 
{
	this->markerDeleteAll();
}

void CustomQsciScintilla::gotoLine(int line) 
{
	this->setCursorPosition(line, 0);
}

int CustomQsciScintilla::lineCount() const 
{
	return this->lines();
}

QList<int> CustomQsciScintilla::markers() const 
{
	QList<int> list;
	int line = 0;
	while ( (line = this->markerFindNext(line, 2)) >= 0 ) 
	{
		list << line++;
	}
	return list;
}

QString CustomQsciScintilla::getLine(int line) const 
{
	return this->text(line);
}

QString CustomQsciScintilla::wordUnderCursor() 
{
	int line, col;
	getCursorPosition(&line, &col);
	QString str = text(line);
	int startPos = str.left(col).lastIndexOf(QRegExp("\\b"));
	int endPos = str.indexOf(QRegExp("\\b"), col);
	if ( startPos >= 0 && endPos >= 0 && endPos > startPos )
		return str.mid(startPos, endPos - startPos);
	else
		return "";
}

void CustomQsciScintilla::find(const QString& str, const DocFindFlags& flags) 
{
	if ( str.isEmpty() )
		return;
	this->getCursorPosition(&(this->searchStartingLine_), &(this->searchStartingCol_));
	this->searchSteppedOver_ = false;
	this->searchStartingScroll_ = this->verticalScrollBar()->value();
	startFind(str, flags);
}

bool CustomQsciScintilla::findsci(const QString& s, const DocFindFlags& flags) 
{
	if ( flags.multiLine )
		return findML(s, flags);

	QString str(s);
	QString text = this->text();
	QStringList lines = text.split(QRegExp("\r\n|\n|\r"));
	int row(-1), col(-1);
	this->getCursorPosition(&row, &col);
	int lineIndex(0);
	if (row < 0 || col < 0)
		return false;

	if ( !flags.backwards ) {
		foreach (QString line, lines) {
			if ( lineIndex < row ) {
			}
			else {
				int indent(0);
				if ( lineIndex == row ) {
					line = line.right(line.length() - col);
					indent = col;
				}
				int index(-1);
				QRegExp regExp;
				if ( flags.wholeWords ) {
					regExp = QRegExp(QString("\\b%1\\b").arg(QRegExp::escape(str)));
				}
				else
					regExp = QRegExp(str);
				regExp.setCaseSensitivity(flags.matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
				if ( flags.isRegExp || flags.wholeWords ) {
					index = line.indexOf(regExp);
				}
				else {
					if ( !flags.matchCase ) {
						str = str.toLower();
						line = line.toLower();
					}
					index = line.indexOf(str);
				}

				if ( index >= 0 ) {
					if ( flags.isRegExp ) {
						this->setSelection(lineIndex, index + indent, lineIndex, index + indent + regExp.matchedLength());
						this->ensureCursorVisible();
					}
					else {
						this->setSelection(lineIndex, index + indent, lineIndex, index + indent + str.length());
						this->ensureCursorVisible();
					}
					return true;
				}
			}
			++lineIndex;
		}
	}
	else {
		QStringList::iterator it = lines.end();
		it--;
		int lineIndex = lines.count() - 1;
		while ( lineIndex >= 0 ) {
			if ( lineIndex > row ) {
			}
			else {
				QString line = *it;
				if ( lineIndex == row ) {
					line = line.left(col);
				}

				int index(-1);
				QRegExp regExp;
				if ( flags.wholeWords )
					regExp = QRegExp(QString("\\b%1\\b").arg(QRegExp::escape(str)));
				else
					regExp = QRegExp(str);
				regExp.setCaseSensitivity(flags.matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
				if ( flags.isRegExp || flags.wholeWords )
					index = line.lastIndexOf(regExp);
				else {
					if ( !flags.matchCase ) {
						str = str.toLower();
						line = line.toLower();
					}
					index = line.lastIndexOf(str);
				}

				if ( index >= 0 ) {
					if ( flags.isRegExp ) {
						this->setSelection(lineIndex, index, lineIndex, index + regExp.matchedLength());
						this->ensureCursorVisible();
					}
					else {
						this->setSelection(lineIndex, index, lineIndex, index + str.length());
						this->ensureCursorVisible();
					}
					return true;
				}
			}
			lineIndex--;
			it--;
		}
	}

	return false;
}

bool CustomQsciScintilla::findML(const QString& s, const DocFindFlags& flags) 
{
	QString text = this->text();
	QRegExp rx(s);

	long pos = -1;
	if ( flags.backwards ) {
		long cPos;
		if ( hasSelectedText() ) {
			int line1, col1, line2, col2;
			getSelection(&line1, &col1, &line2, &col2);
			cPos = lineColToPos(line1, col1);
		}
		else {
			cPos = curPos();
		}

		pos = text.left(cPos).lastIndexOf(rx);
	}
	else {
		long cPos;
		if ( hasSelectedText() ) {
			int line1, col1, line2, col2;
			getSelection(&line1, &col1, &line2, &col2);
			cPos = lineColToPos(line2, col2);
		}
		else {
			cPos = curPos();
		}

		pos = text.indexOf(rx, cPos);
	}

	if ( pos >= 0 ) {
		int line1, col1, line2, col2;
		posToLineCol(pos, line1, col1);
		posToLineCol(pos + rx.matchedLength(), line2, col2);
		setSelection(line1, col1, line2, col2);
		ensureCursorVisible();
		return true;
	}
	else {
		return false;
	}
}

void CustomQsciScintilla::posToLineCol(long pos, int& line, int& col) const 
{
	line = SendScintilla(SCI_LINEFROMPOSITION, pos);
	long linpos = SendScintilla(SCI_POSITIONFROMLINE, line);
	col = (int)(pos - linpos);
}

long CustomQsciScintilla::lineColToPos(int line, int col) const 
{
	long linpos = SendScintilla(SCI_POSITIONFROMLINE, line);
	return linpos + col;
}

long CustomQsciScintilla::curPos() const 
{
	int line, col;
	getCursorPosition(&line, &col);
	return lineColToPos(line, col);
}

void CustomQsciScintilla::startFind(const QString& str, const DocFindFlags& flags) 
{
	prepareForFind(str, flags);
	bool found = this->findsci(str, flags);
	if ( !found ) {//	not found		
		if ( !this->searchSteppedOver_ ) {
			this->searchSteppedOver_ = true;
			stepOver(flags.backwards);
			startFind(str, flags);
		}
		else {
			this->setCursorPosition(this->searchStartingLine_, this->searchStartingCol_);
			this->verticalScrollBar()->setValue(this->searchStartingScroll_);
			QMessageBox::information(0, "Information", QString("Text '%1' was not found").arg(str));
			return;
		}
	}
}

void CustomQsciScintilla::stepOver(bool back) 
{
	int row(0), col(0);
	if ( back ) {
		row = this->lines() - 1;
		col = this->text(row).length();
	}
	else {
		row = 0;
		col = 0;
	}
	this->setCursorPosition(row, col);
}

void CustomQsciScintilla::prepareForFind(const QString& s, const DocFindFlags& flags) 
{
	QString str(s);
	if ( flags.backwards && this->hasSelectedText() ) {
		int fromRow, fromCol, toRow, toCol;
		this->getSelection(&fromRow, &fromCol, &toRow, &toCol);
		if ( fromRow == toRow ) {
			QString selection = this->selectedText();
			if ( flags.isRegExp ) {
				QRegExp r(str);
				r.setCaseSensitivity(flags.matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
				if ( r.exactMatch(selection) ) {
					this->setCursorPosition(fromRow, fromCol);
				}
			}
			else {
				if ( !flags.matchCase ) {
					str = str.toLower();
					selection = selection.toLower();
				}
				if ( selection.compare(str) == 0 ) {
					this->setCursorPosition(fromRow, fromCol);
				}
			}
		}
	}
}

void CustomQsciScintilla::replace(const QString& str1, const QString& str2, const DocFindFlags& flags, bool replaceAll) 
{
	if ( str1.isEmpty() )
		return;
	prepareForFind(str1, flags);

	this->getCursorPosition(&(this->searchStartingLine_), &(this->searchStartingCol_));
	this->searchSteppedOver_ = false;
	this->searchStartingScroll_ = this->verticalScrollBar()->value();

	int count = 0;
	startReplace(str1, str2, flags, replaceAll, count);
}

void CustomQsciScintilla::startReplace(const QString& str1, const QString& str2, const DocFindFlags& flags, bool& replaceAll, int& count) //JuffScintilla* edit
{//return -1 in case of cancel or the amount of replacements (0|1)
	bool cancelled = false;
	int nResult;
	while ( this->findsci(str1, flags) ) {
		nResult = doReplace(str1, str2, flags, replaceAll);
		if ( nResult == -1 ) {
			cancelled = true;
			break;
		}
		else if ( nResult >= 0 ) {
			count = count + nResult;
		}
	}
	if ( !cancelled ) {
		//	reached the end or the beginning
		if ( !this->searchSteppedOver_ ) {
			this->searchSteppedOver_ = true;
			stepOver(flags.backwards);
			startReplace(str1, str2, flags, replaceAll, count);
		}
		else {
			this->setCursorPosition(this->searchStartingLine_, this->searchStartingCol_);
			this->verticalScrollBar()->setValue(this->searchStartingScroll_);
			//QMessageBox::information(0, "Information", QString("Replacement finished (%1 replacements were made)").arg(count));
			DocumentManager *tmpDocObject = qobject_cast<DocumentManager *>(manager);
			tmpDocObject->signalDocManagerOutput(QString("Replacement finished (%1 replacements were made)").arg(count));
			return;
		}
	}
}

int CustomQsciScintilla::doReplace(const QString& str1, const QString& str2, const DocFindFlags& flags, bool& replaceAll) 
{
	int nReplacements = 0;
	QString selectedText = this->selectedText();
	QString targetText(str2);
	if ( flags.isRegExp ) {
		QRegExp regExp(str1);
		regExp.setCaseSensitivity(flags.matchCase ? Qt::CaseSensitive : Qt::CaseInsensitive);
		if ( regExp.exactMatch(selectedText) ) {
			QStringList matches = regExp.capturedTexts();
			int n = matches.size();
			for ( int i = 0; i < n; ++i ) {
				targetText.replace(QString("\\%1").arg(i), matches[i]);
			}
		}
		else
			return -1;
	}
	if ( !replaceAll ) {
		//ask for confirmation if replace all hasn't been chosen yet
		
		//QMessageBox::information(0, "Information", QString("Replacement finished (%1 replacements were made)").arg(count));
		QMessageBox::StandardButton btn = QMessageBox::question(0, QString("Confirmation"), QString("Replace this text?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::YesToAll | QMessageBox::Cancel, QMessageBox::Yes);

		if ( btn == QMessageBox::Cancel )
			return -1;
		//else if ( btn == QMessageBox::No ) {
		//	nSkippedUserReplacements++;
		//}
		else if ( btn == QMessageBox::Yes ) {
			this->replaceSelected(targetText, flags.backwards);
			nReplacements++;
			this->selectAll(false);
		}
		else if ( btn == QMessageBox::YesToAll ) {
			this->replaceSelected(targetText, flags.backwards);
			nReplacements++;
			this->selectAll(false);
			replaceAll = true;
		}
	}
	else {
		//just replace, because there has been chosen "select all"
		this->replaceSelected(targetText, flags.backwards);
		nReplacements++;
		this->selectAll(false);
	}
	return nReplacements;
}

void CustomQsciScintilla::replaceSelected(const QString& targetText, bool backwards) 
{
	beginUndoAction();
	removeSelectedText();
	int r, c;
	getCursorPosition(&r, &c);
	insert(targetText);
	if ( !backwards ) {
		setCursorPosition(r, c + targetText.length());
	}
	endUndoAction();
}
