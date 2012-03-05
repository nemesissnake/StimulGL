//ExperimentManagerplugin
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


#include "ExperimentManager_dialog.h"

ExperimentManager_Dialog::ExperimentManager_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

ExperimentManager_Dialog::~ExperimentManager_Dialog()
{

}

void ExperimentManager_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void ExperimentManager_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void ExperimentManager_Dialog::cleanUp()
{
	return;
}

void ExperimentManager_Dialog::exampleButton_Pressed()
{
	QMessageBox msgBox;
	msgBox.setText("This Gui has not been implemented jet.");
	msgBox.exec();

	//StimGLScene = new StimulGLGraphScene(this);
	//if(StimGLScene->Init())
	//{
	//	GraphView = new QGraphicsView(StimGLScene);
	//	GraphView->setWindowModality(Qt::WindowModal);
	//	//connect(StimGLScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
	//	GraphView->showFullScreen();
	//}
}

//void ExperimentManager_Dialog::on_GraphScene_Closed()
//{
//	//disconnect(StimGLScene, SIGNAL(UserWantsToClose(void)), this, SLOT(on_GraphScene_Closed(void)));
//	GraphView->close();
//	delete GraphView;
//	delete StimGLScene;
//	GraphView = NULL;
//	StimGLScene = NULL;
//}
