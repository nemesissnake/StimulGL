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


#include "ExperimentManager_dialog.h"
#include "globalapplicationinformation.h"

ExperimentManager_Dialog::ExperimentManager_Dialog(QWidget *parent)	: QDialog(parent)
{
	currentExperimentSubObjectState = Experiment_SubObject_Constructing;
	QmlWidgetObject = NULL;
	Qml2ViewerObject = NULL;
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
	docContentStructToRun.strDocContent = "";
	docContentStructToRun.strDocExtension = "";
	docContentStructToRun.strDocHomeDir = "";
	docContentStructToRun.bIsFile = true;
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
	if(Qml2ViewerObject)
	{
		Qml2ViewerObject->stopObject();
		delete Qml2ViewerObject;
		Qml2ViewerObject = NULL;
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

bool ExperimentManager_Dialog::setContentToExecute(DocContentInfoStructure docContentStruct)
{
	docContentStructToRun = docContentStruct;
	return true;
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
			disconnect(QmlWidgetObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
		}
		if(Qml2ViewerObject)
		{
			disconnect(Qml2ViewerObject, SIGNAL(UserWantsToClose(void)), Qml2ViewerObject, SLOT(abortExperimentObject(void)));
			disconnect(Qml2ViewerObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
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
			connect(QmlWidgetObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
		}		
		if(Qml2ViewerObject)
		{
			connect(Qml2ViewerObject, SIGNAL(UserWantsToClose(void)), Qml2ViewerObject, SLOT(abortExperimentObject(void)));
			connect(Qml2ViewerObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
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
	QMetaObject::invokeMethod(MainAppInfo::getMainWindow(), MainAppInfo::getMainWindowLogSlotName().toLatin1(), Qt::DirectConnection, Q_ARG(QString, sMessage));
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
			if (Qml2ViewerObject)
				Qml2ViewerObject->deleteLater();
			currentExperimentSubObjectState = Experiment_SubObject_IsAborting;
		}
	}
	else if(nState == Experiment_SubObject_Stop)
	{
		if (currentExperimentSubObjectState != Experiment_SubObject_IsStopping)//Multiple Stop events could occur, catch only first one
		{
			if (QmlWidgetObject)
				QmlWidgetObject->deleteLater();
			if (Qml2ViewerObject)
				Qml2ViewerObject->deleteLater();
			if (tmpContainerDlg)
				tmpContainerDlg->deleteLater();
			currentExperimentSubObjectState = Experiment_SubObject_IsStopping;
		}
	}
	else if(nState == Experiment_SubObject_Destructing)
	{
		currentExperimentSubObjectState = Experiment_SubObject_Destructing;
		QmlWidgetObject = NULL;
		Qml2ViewerObject = NULL;
		tmpContainerDlg = NULL;
		delete tmpLayout;
		tmpLayout = NULL;
		currentExperimentSubObjectState = Experiment_SubObject_Stopped;		
	}
	else//All other cases
	{
		currentExperimentSubObjectState = nState;
	}
}

bool ExperimentManager_Dialog::executeDocument()
{
	if (((currentExperimentSubObjectState == Experiment_SubObject_Initialized) || (currentExperimentSubObjectState == Experiment_SubObject_Stopped)) == false)
		return false;

	QString fileSource = "";
	if(docContentStructToRun.strDocExtension == PLUGIN_EXMLDOC_EXTENSION)
	{
		if(docContentStructToRun.bIsFile)
		{
			if(docContentStructToRun.strDocContent.isEmpty())
			{
				fileSource = QFileDialog::getOpenFileName(NULL, tr("Open a Experiment file"), MainAppInfo::appExampleDirPath(), tr("Experiment Files (*.exml);;Any file (*)"));
			}
			if (fileSource.isEmpty())
				return false;
			QFile file(fileSource);
			if (!file.exists())
				return false;
			QFileInfo fi(file);
			QDir::setCurrent(fi.canonicalPath());
		}
		else
		{
			fileSource = docContentStructToRun.strDocContent;
			//QString dd = MainAppInfo::appDirPath();
			QDir::setCurrent(docContentStructToRun.strDocHomeDir);
		}
		ExperimentManagerObj = new ExperimentManager(this);
		connectSignalSlots(false);
		if(ExperimentManagerObj->loadExperiment(fileSource,false,docContentStructToRun.bIsFile))
		{
			return ExperimentManagerObj->runExperiment();
		}
		else
		{
			LogMessage("Error: Could not execute the document!");
		}
	}
	else if (docContentStructToRun.strDocExtension == PLUGIN_QMLDOC_EXTENSION)
	{
		if(docContentStructToRun.bIsFile)
		{
			if(docContentStructToRun.strDocContent.isEmpty())
			{
				fileSource = QFileDialog::getOpenFileName(NULL, tr("Open a QtQuick File"), MainAppInfo::appExampleDirPath(), tr("QtQuick Files (*.qml);;Any file (*)"));
			}
			if (fileSource.isEmpty())
				return false;
			QFile file(fileSource);
			if (!file.exists())
				return false;
			QFileInfo fi(file);
			QDir::setCurrent(fi.canonicalPath());
		}
		else
		{
			fileSource = docContentStructToRun.strDocContent;
			//QString dd = MainAppInfo::appDirPath();
			QDir::setCurrent(docContentStructToRun.strDocHomeDir);
		}
		tmpContainerDlg = new ContainerDlg(this);
		tmpContainerDlg->setAttribute(Qt::WA_DeleteOnClose);
		tmpContainerDlg->setAttribute(Qt::WA_PaintOnScreen);
		tmpLayout = new QVBoxLayout;
		tmpLayout->setAlignment(Qt::AlignHCenter);
		tmpLayout->setMargin(0);
		//tmpLayout->setStretch(0,1680);
		tmpContainerDlg->setLayout(tmpLayout);
		tmpContainerDlg->setWindowModality(Qt::WindowModality::WindowModal);

		QmlWidgetObject = new qmlWidget(this);
		//tmpLayout->setStretch(0,)
		tmpLayout->addWidget(QmlWidgetObject);

		if(QmlWidgetObject->executeQMLDocument(fileSource,tmpContainerDlg,docContentStructToRun.bIsFile))
		{
			connectSignalSlots(false);
			tmpContainerDlg->showFullScreen();
			return true;
		}
	}
	return false;
}

void ExperimentManager_Dialog::exampleButton_Pressed()
{
	docContentStructToRun.strDocContent = "";
	docContentStructToRun.bIsFile = true;
	docContentStructToRun.strDocExtension = PLUGIN_EXMLDOC_EXTENSION;
	executeDocument();
}

void ExperimentManager_Dialog::exampleButton_2_Pressed()
{
	docContentStructToRun.strDocContent = "";
	docContentStructToRun.bIsFile = true;
	docContentStructToRun.strDocExtension = PLUGIN_QMLDOC_EXTENSION;
	executeDocument();
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


