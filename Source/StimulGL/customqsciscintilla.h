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
