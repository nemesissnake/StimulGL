//_extensionname_
//Copyright (C) 2012  _authorname_
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


//This file implements the UI functionality

#ifndef _extensionname__DIALOG_H
#define _extensionname__DIALOG_H

#include <QDialog>
#include "ui__extensionname__dialog.h"
#include <QtGui>
#include <Windows.h>
#include <QDialog>

class _extensionname__Dialog : public QDialog, private Ui::_extensionname__DialogClass
{
	Q_OBJECT

public:
	_extensionname__Dialog(QWidget *parent = 0);
	~_extensionname__Dialog();

private:
	Ui::_extensionname__DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void on_okButton_clicked();
};

#endif // _extensionname__DIALOG_H
