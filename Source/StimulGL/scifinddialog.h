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


#ifndef SCIFINDDIALOG_H
#define SCIFINDDIALOG_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QTextDocument>
#include "ui_scifinddialog.h"

typedef struct _DocFindFlags 
{
	_DocFindFlags(bool Replace = false, bool MatchCase = false, bool Backwards = false, 
		bool IsRegExp = false, bool WholeWords = false, bool MultiLine = false) 
	{
		replace = Replace;
		matchCase = MatchCase;
		backwards = Backwards;
		isRegExp = IsRegExp;
		wholeWords = WholeWords;
		multiLine = MultiLine;
	}
	bool replace;
	bool matchCase;
	bool backwards;
	bool isRegExp;
	bool wholeWords;
	bool multiLine;
} DocFindFlags;

class sciFindDialog : public QDialog
{
	Q_OBJECT

public:
	sciFindDialog(QWidget*, bool);
	virtual ~sciFindDialog();

	QString text() const;
	QString replaceTo() const;
	DocFindFlags flags() const;
	bool isReplaceMode() const { return uiFind.replaceChk->isChecked(); }
	void setText(const QString&);

	static QString lastText() { return lastString_; }
	static QString lastReplaceText() { return lastReplaceText_; }
	static bool lastMatchCase() { return matchCase_; }
	static bool lastRegExpMode() { return regExpMode_; }
	static bool lastWholeWords() { return wholeWords_; }

public slots:
	void setReplaceMode(bool);

protected slots:
	void multiLineChecked(bool);
	void regExpChecked(bool);

protected:
	virtual void keyPressEvent(QKeyEvent*);

private:

	Ui::sciFindDialogClass uiFind;

	static QString lastString_;
	static QString lastReplaceText_;
	static bool matchCase_;
	static bool backward_;
	static bool regExpMode_;
	static bool wholeWords_;
	static bool multiLine_;
	static QStringList strings_;
	static QStringList replaces_;
};

#endif // SCIFINDDIALOG_H
