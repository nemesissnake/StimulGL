//ExperimentManagerplugin
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

#include "ObjectParameterDialog.h"
#include "ui_ObjectParameterDialog.h"

ObjectParameterDialog::ObjectParameterDialog(QWidget *parent) : QDialog(parent)
{
	setAttribute(Qt::WA_DeleteOnClose);

	ui = new Ui::ObjectParameterDialog();
	ui->setupUi(this);
	setWindowTitle("Add Block Parameter(s)");

	connect(ui->pbApply, SIGNAL(clicked()), this, SLOT(applyChanges()));
}

ObjectParameterDialog::~ObjectParameterDialog()
{
	delete ui;
}

void ObjectParameterDialog::applyChanges()
{
	//itemEditing->setDefinitions(m_definitions);
	close();
}

