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
#include "mainappinfo.h"

ExperimentManager_Dialog::ExperimentManager_Dialog(QWidget *parent)	: QDialog(parent)
{
	currentExperimentSubObjectState = Experiment_SubObject_Constructing;
	QmlWidgetObject = NULL;
	ExperimentManagerObj = NULL;
	tmpContainerDlg = NULL;
	tmpLayout = NULL;
	
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
	connect(ui.btnExampleButton_2, SIGNAL(clicked()), this, SLOT(exampleButton_2_Pressed()));
	connect(ui.btnExampleButton_3, SIGNAL(clicked()), this, SLOT(exampleButton_3_Pressed()));
	connect(ui.btnExampleButton_4, SIGNAL(clicked()), this, SLOT(exampleButton_4_Pressed()));
	connect(ui.btnExampleButton_5, SIGNAL(clicked()), this, SLOT(exampleButton_5_Pressed()));
	currentExperimentSubObjectState = Experiment_SubObject_Initialized;
	currentExperimentState = ExperimentManager::ExperimentManager_NoState;
}

ExperimentManager_Dialog::~ExperimentManager_Dialog()
{
	currentExperimentSubObjectState = Experiment_SubObject_Destructing;
	cleanUp();
}

void ExperimentManager_Dialog::closeEvent(QCloseEvent *event)
{
	cleanUp();
	event->accept();//or event->ignore();
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
	if(ExperimentManagerObj)
	{
		if ((currentExperimentState == ExperimentManager::ExperimentManager_IsStarting) || (currentExperimentState == ExperimentManager::ExperimentManager_Started))
		{
			ExperimentManagerObj->abortExperiment();
			int nRetries = 10;
			while(currentExperimentState != ExperimentManager::ExperimentManager_Stopped)
			{
				qApp->processEvents(QEventLoop::ExcludeSocketNotifiers,50);
				nRetries--;
				if (nRetries == 0)
				{
					break;//Could not stop the experiment....
				}
			}
			nRetries = nRetries;
		}		
	}
	connectSignalSlots(true);
	if(ExperimentManagerObj)
	{
		delete ExperimentManagerObj;
		ExperimentManagerObj = NULL;
	}
	if (QmlWidgetObject)
	{
		QmlWidgetObject->stopObject();
		delete QmlWidgetObject;
		QmlWidgetObject = NULL;
	}
	if (tmpContainerDlg)
	{
		delete tmpContainerDlg;
		tmpContainerDlg = NULL;
	}
	if (tmpLayout)
	{
		delete tmpLayout;
		tmpLayout = NULL;
	}
	return;
}

void ExperimentManager_Dialog::connectSignalSlots(bool bDisconnect)
{
	//void UserWantsToClose(void);
	//void ObjectShouldStop(void);
	//void ObjectStateHasChanged(ExperimentSubObjectState);
	//void NewInitBlockTrial();
	//void ExternalTriggerIncremented(int);
	//void ExperimentStructureChanged(int,int,int);//Block,Trial,InternalTrigger

	if (bDisconnect)
	{
		if(QmlWidgetObject)
		{
			disconnect(QmlWidgetObject, SIGNAL(UserWantsToClose(void)), QmlWidgetObject, SLOT(abortExperimentObject(void)));
			//disconnect(QmlWidgetObject, SIGNAL(ObjectShouldStop(void)), QmlWidgetObject, SLOT(stopExperiment(void)));
			disconnect(QmlWidgetObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
		}
		if(ExperimentManagerObj)
		{
			disconnect(ExperimentManagerObj, SIGNAL(ExperimentStateHasChanged(int, QString)), this, SLOT(ExperimentStateHasChanged(int, QString)));
			disconnect(ExperimentManagerObj, SIGNAL(WriteToLogOutput(QString)), this, SLOT(LogMessage(QString)));
		}
	}
	else
	{
		if(QmlWidgetObject)
		{
			connect(QmlWidgetObject, SIGNAL(UserWantsToClose(void)), QmlWidgetObject, SLOT(abortExperimentObject(void)));
			//connect(QmlWidgetObject, SIGNAL(ObjectShouldStop(void)), QmlWidgetObject, SLOT(stopExperiment(void)));
			connect(QmlWidgetObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
		}
		if(ExperimentManagerObj)
		{
			connect(ExperimentManagerObj, SIGNAL(ExperimentStateHasChanged(int, QString)), this, SLOT(ExperimentStateHasChanged(int, QString)));
			connect(ExperimentManagerObj, SIGNAL(WriteToLogOutput(QString)), this, SLOT(LogMessage(QString)));
		}
	}	
}

void ExperimentManager_Dialog::LogMessage(QString sMessage)
{

}

void ExperimentManager_Dialog::ExperimentStateHasChanged(int nExperimentMainState, QString timeTextStamp)
{
	currentExperimentState = (ExperimentManager::ExperimentState)nExperimentMainState;
	if (currentExperimentState == ExperimentManager::ExperimentManager_Constructed)
	{

	} 
	else if (currentExperimentState == ExperimentManager::ExperimentManager_Initialized)
	{

	}
}

void ExperimentManager_Dialog::changeExperimentSubObjectState(ExperimentSubObjectState nState)
{
	if(nState == Experiment_SubObject_Abort)
	{
		if (currentExperimentSubObjectState != Experiment_SubObject_IsAborting)//Multiple Abort events could occur, catch only first one
		{
			if (QmlWidgetObject)
				QmlWidgetObject->deleteLater();
			currentExperimentSubObjectState = Experiment_SubObject_IsAborting;
		}
	}
	else if(nState == Experiment_SubObject_Stop)
	{
		if (currentExperimentSubObjectState != Experiment_SubObject_IsStopping)//Multiple Stop events could occur, catch only first one
		{
			if (QmlWidgetObject)
				QmlWidgetObject->deleteLater();
			currentExperimentSubObjectState = Experiment_SubObject_IsStopping;
		}
	}
	else if(nState == Experiment_SubObject_Stopped)
	{
		currentExperimentSubObjectState = nState;
		if (tmpContainerDlg)
		{
			tmpContainerDlg->deleteLater();
			//delete tmpContainerDlg;//Doesn't work!
			//tmpContainerDlg = NULL;
		}
		QmlWidgetObject = NULL;
	}
	else//All other cases
	{
		currentExperimentSubObjectState = nState;
	}
}

void ExperimentManager_Dialog::exampleButton_Pressed()
{
	if (((currentExperimentSubObjectState == Experiment_SubObject_Initialized) || (currentExperimentSubObjectState == Experiment_SubObject_Stopped)) == false)
		return;
	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open a Experiment file"), MainAppInfo::appExampleDirPath(), tr("Experiment Files (*.exml);;Any file (*)"));//"E:\\Projects\\StimulGL\\Install\\examples\\GettingStartedGuide\\ExperimentManager\\experiments\\PolarAngle.exml";
	if (fileName.isEmpty())
		return;
	QFile file(fileName);
	if (!file.exists())
		return;
	QFileInfo fi(file);
	QDir::setCurrent(fi.canonicalPath());
	ExperimentManagerObj = new ExperimentManager(this);
	connectSignalSlots(false);
	ExperimentManagerObj->setExperimentFileName(file.fileName());
		if(!ExperimentManagerObj->runExperiment())
	{
		return;
	}
}

void ExperimentManager_Dialog::exampleButton_2_Pressed()
{
	if (((currentExperimentSubObjectState == Experiment_SubObject_Initialized) || (currentExperimentSubObjectState == Experiment_SubObject_Stopped)) == false)
		return;

	QString fileName = "E:\\Projects\\StimulGL\\Install\\examples\\GettingStartedGuide\\ExperimentManager\\experiments\\Block_2.qml";//QFileDialog::getOpenFileName(NULL, tr("Open a QtQuick File"), MainAppInfo::appExampleDirPath(), tr("QtQuick Files (*.qml);;Any file (*)"));
	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	if (!file.exists())
		return;

	tmpContainerDlg = new ContainerDlg(this);
	tmpContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
	tmpContainerDlg->setAttribute(Qt::WA_PaintOnScreen);
	tmpLayout = new QVBoxLayout;
	tmpLayout->setAlignment(Qt::AlignCenter);
	tmpLayout->setMargin(0);
	tmpContainerDlg->setLayout(tmpLayout);
	tmpContainerDlg->setWindowModality(Qt::WindowModality::WindowModal);
	
	QmlWidgetObject = new qmlWidget(this);
	tmpLayout->addWidget(QmlWidgetObject);

	if(QmlWidgetObject->executeQMLDocument(fileName,tmpContainerDlg))//tmpLayout))
	//if(QmlWidgetObject->executeQMLDocument(fileName))//tmpLayout))
	{
		connectSignalSlots(false);
		tmpContainerDlg->showFullScreen();
	}
}

void ExperimentManager_Dialog::exampleButton_3_Pressed()
{
	
}

void ExperimentManager_Dialog::exampleButton_4_Pressed()
{
	
}

void ExperimentManager_Dialog::exampleButton_5_Pressed()
{

}


