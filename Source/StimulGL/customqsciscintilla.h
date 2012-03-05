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


#ifndef CUSTOMQSCISCINTILLA_H
#define CUSTOMQSCISCINTILLA_H

#include <QObject>
#include <QtGui>
#include <QEvent>
#include <QKeyEvent>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qsciapis.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciprinter.h>
#include "scifinddialog.h"
#include "mainappinfo.h"

class CustomQsciScintilla : public QsciScintilla
{
	public:
		CustomQsciScintilla(MainAppInfo::DocType docType);
		~CustomQsciScintilla();

		void setCustomLexer();
		void setManagerObject(QObject *managerObject,QObject *childWindowObject);
		void toggleLineComment();
		void toggleBlockComment();
		void commentLine(int line, const QString& str1, const QString& comment); 
		void uncommentLine(int line, const QString& str1, const QString& comment);
		void replaceSelectedText(const QString& text); 
		void insertText(const QString& text); 
		bool print();
		void toggleMarker(int line);
		void toggleMarker();
		void nextMarker();
		void prevMarker();
		void removeAllMarkers();
		void gotoLine(int line);
		int lineCount() const;
		QList<int> markers() const;
		QString getLine(int line) const;
		QString wordUnderCursor();
		void find(const QString& str, const DocFindFlags& flags);
		void startFind(const QString& str, const DocFindFlags& flags); 
		void replace(const QString& str1, const QString& str2, const DocFindFlags& flags);
		void startReplace(const QString& str1, const QString& str2, const DocFindFlags& flags, bool& replaceAll, int& count);

	protected:
		void closeEvent(QCloseEvent *event);

	private:
		void prepareForFind(const QString& s, const DocFindFlags& flags);
		bool findsci(const QString& s, const DocFindFlags& flags);
		bool findML(const QString& s, const DocFindFlags& flags);
		long curPos() const;
		long lineColToPos(int line, int col) const;
		void posToLineCol(long pos, int& line, int& col) const;
		void stepOver(bool back);
		bool doReplace(const QString& str1, const QString& str2, const DocFindFlags& flags, bool& replaceAll);
		void replaceSelected(const QString& targetText, bool backwards);

		QObject *manager;
		QObject *childWindow;
		QsciLexer *Qjslexer;

		int searchStartingLine_;
		int searchStartingCol_;
		bool searchSteppedOver_;
		int searchStartingScroll_;
};

#endif
