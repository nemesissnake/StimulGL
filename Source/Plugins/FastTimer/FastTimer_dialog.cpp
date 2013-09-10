//FastTimer
//Copyright (C) 2013  Michael Luehrs
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


#include "FastTimer_dialog.h"

FastTimer_Dialog::FastTimer_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

FastTimer_Dialog::~FastTimer_Dialog()
{

}

void FastTimer_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void FastTimer_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void FastTimer_Dialog::cleanUp()
{
	return;
}

void FastTimer_Dialog::exampleButton_Pressed()
{
	QMessageBox msgBox;
	msgBox.setText("Example button pressed.");
	msgBox.exec();
}
