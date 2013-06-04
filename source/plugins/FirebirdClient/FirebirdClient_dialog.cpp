//FirebirdClient
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


#include "FirebirdClient_dialog.h"

FirebirdClient_Dialog::FirebirdClient_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

FirebirdClient_Dialog::~FirebirdClient_Dialog()
{

}

void FirebirdClient_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void FirebirdClient_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void FirebirdClient_Dialog::cleanUp()
{
	return;
}

void FirebirdClient_Dialog::exampleButton_Pressed()
{
	QMessageBox msgBox;
	msgBox.setText("Example button pressed.");
	msgBox.exec();
}
