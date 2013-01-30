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

#ifndef CONTAINERDLG_H
#define CONTAINERDLG_H

#include <QDialog>
#include <QCloseEvent>

class ContainerDlg : public QDialog
{
	Q_OBJECT

public:
	ContainerDlg(QWidget *parent = NULL);
	ContainerDlg::~ContainerDlg();
	
private slots:
	void reject();

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // CONTAINERDLG_H
