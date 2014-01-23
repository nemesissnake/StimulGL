//PiezoStimDevice
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


//This file implements the UI functionality

#ifndef PiezoStimDevice_DIALOG_H
#define PiezoStimDevice_DIALOG_H

#include <QDialog>
#include "ui_PiezoStimDevice_dialog.h"
#include <QtWidgets>
#include <Windows.h>
#include <QDialog>

class PiezoStimDevice_Dialog : public QDialog, private Ui::PiezoStimDevice_DialogClass
{
	Q_OBJECT

public:
	PiezoStimDevice_Dialog(QWidget *parent = 0);
	~PiezoStimDevice_Dialog();

private:
	Ui::PiezoStimDevice_DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void on_okButton_clicked();
};

#endif // PiezoStimDevice_DIALOG_H
