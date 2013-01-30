//ExperimentManagerplugin
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

#include "ContainerDlg.h"

ContainerDlg::ContainerDlg(QWidget *parent) : QDialog(parent)
{
	//Setting a black background
	QPalette pal = palette();
	pal.setColor(QPalette::Window, QColor(0, 0, 0));
	setPalette(pal);
}

ContainerDlg::~ContainerDlg()
{

}

void ContainerDlg::closeEvent(QCloseEvent *e)
{
	showNormal();
	e->accept();
}

void ContainerDlg::reject() // called when "Esc" pressed
{
	showNormal(); // to get back dock on Mac, we need to call this here (restore from fullscreen mode)
	QDialog::reject();
}
