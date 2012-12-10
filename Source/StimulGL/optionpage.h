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


#ifndef OPTIONPAGE_H
#define OPTIONPAGE_H

#include <QDialog>
#include <QSettings>
#include "ui_optionpage.h"
#include "GlobalApplicationInformation.h"

class OptionPage : public QDialog
{
	Q_OBJECT

public:
	OptionPage(QWidget *parent, GlobalApplicationInformation *g_AppInfo);
	~OptionPage();

private:
	Ui::OptionPageClass ui;
	GlobalApplicationInformation *glob_AppInfo;

private slots:
	void on_rdb_3DRenderer_3_toggled(bool);
	void on_rdb_3DRenderer_2_toggled(bool);
	void on_rdb_3DRenderer_toggled(bool);
	void setUI3DRendererControls(int type);
	void applySettings();
	void readSettings();
};

#endif // OPTIONPAGE_H
