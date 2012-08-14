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
	currentExperimentState = Experiment_SubObject_Constructing;
	//Gets constructed only once during the load of the plugin
	QmlWidgetObject = NULL;
	tmpContainerDlg = NULL;
	tmpLayout = NULL;
	//audioOutput = NULL;
	
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
	connect(ui.btnExampleButton_2, SIGNAL(clicked()), this, SLOT(exampleButton_2_Pressed()));
	connect(ui.btnExampleButton_3, SIGNAL(clicked()), this, SLOT(exampleButton_3_Pressed()));
	connect(ui.btnExampleButton_4, SIGNAL(clicked()), this, SLOT(exampleButton_4_Pressed()));
	connect(ui.btnExampleButton_5, SIGNAL(clicked()), this, SLOT(exampleButton_5_Pressed()));
	currentExperimentState = Experiment_SubObject_Initialized;
}

ExperimentManager_Dialog::~ExperimentManager_Dialog()
{
	currentExperimentState = Experiment_SubObject_Destructing;
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
	connectSignalSlots(true);
	if (QmlWidgetObject)
	{
		QmlWidgetObject->stopObject();
		delete QmlWidgetObject;
		QmlWidgetObject = NULL;
	}
	if (tmpContainerDlg == NULL)
	{
		delete tmpContainerDlg;
		tmpContainerDlg = NULL;
	}
	if (tmpLayout == NULL)
	{
		delete tmpLayout;
		tmpLayout = NULL;
	}
	return;
}

void ExperimentManager_Dialog::connectSignalSlots(bool bDisconnect)
{
	if (QmlWidgetObject == NULL)
		return;
	if (bDisconnect)
	{
		disconnect(QmlWidgetObject, SIGNAL(UserWantsToClose(void)), QmlWidgetObject, SLOT(abortExperimentObject(void)));
		//disconnect(QmlWidgetObject, SIGNAL(ObjectShouldStop(void)), QmlWidgetObject, SLOT(stopExperiment(void)));
		disconnect(QmlWidgetObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
	}
	else
	{
		connect(QmlWidgetObject, SIGNAL(UserWantsToClose(void)), QmlWidgetObject, SLOT(abortExperimentObject(void)));
		//connect(QmlWidgetObject, SIGNAL(ObjectShouldStop(void)), QmlWidgetObject, SLOT(stopExperiment(void)));
		connect(QmlWidgetObject, SIGNAL(ObjectStateHasChanged(ExperimentSubObjectState)), this, SLOT(changeExperimentSubObjectState(ExperimentSubObjectState)));
	}
}

void ExperimentManager_Dialog::changeExperimentSubObjectState(ExperimentSubObjectState nState)
{
	if(nState == Experiment_SubObject_Abort)
	{
		if (currentExperimentState != Experiment_SubObject_IsAborting)//Multiple Abort events could occur, catch only first one
		{
			if (QmlWidgetObject)
				QmlWidgetObject->deleteLater();
			currentExperimentState = Experiment_SubObject_IsAborting;
		}
	}
	else if(nState == Experiment_SubObject_Stop)
	{
		if (currentExperimentState != Experiment_SubObject_IsStopping)//Multiple Stop events could occur, catch only first one
		{
			if (QmlWidgetObject)
				QmlWidgetObject->deleteLater();
			currentExperimentState = Experiment_SubObject_IsStopping;
		}
	}
	else if(nState == Experiment_SubObject_Stopped)
	{
		currentExperimentState = nState;
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
		currentExperimentState = nState;
	}
}

void ExperimentManager_Dialog::exampleButton_Pressed()
{
	if (((currentExperimentState == Experiment_SubObject_Initialized) || (currentExperimentState == Experiment_SubObject_Stopped)) == false)
		return;
	
	QString fileName = QFileDialog::getOpenFileName(NULL, tr("Open a QML Document"), MainAppInfo::appExampleDirPath(), tr("QML Document Files (*.qml);;Any file (*)"));
	if (fileName.isEmpty())
		return;

	QFile file(fileName);
	if (!file.exists())
		return;
	
	//if (tmpContainerDlg == NULL)//Always construct a new one
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
	{
		connectSignalSlots(false);
		tmpContainerDlg->showFullScreen();
	}
}

void ExperimentManager_Dialog::exampleButton_2_Pressed()
{
	//startPlaying();
}

//void ExperimentManager_Dialog::startPlaying()
//{
//	inputFile.setFileName("E:\\Projects\\StimulGL\\Install\\examples\\Sounds\\level2.wav");//morsecode.mp3");//level2.wav");//WindowsNotify.wav");
//	inputFile.open(QIODevice::ReadOnly);
//
//	QAudioFormat format;
//	// Set up the format, eg.
//	format.setFrequency(22050);////44100);//8000);
//	format.setChannels(1);//2);
//	format.setSampleSize(16);//8);
//	format.setCodec("audio/pcm");
//	format.setByteOrder(QAudioFormat::LittleEndian);
//	format.setSampleType(QAudioFormat::UnSignedInt);
//
//
//	QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
//	if (!info.isFormatSupported(format)) {
//		qWarning()<<"raw audio format not supported by backend, cannot play audio.";
//		return;
//	}
//
//	audioOutput = new QAudioOutput(format, this);
//	connect(audioOutput,SIGNAL(stateChanged(QAudio::State)),this,SLOT(finishedPlaying(QAudio::State)));
//	audioOutput->start(&inputFile);
//}

//void ExperimentManager_Dialog::finishedPlaying(QAudio::State state)
//{
//	if (state == QAudio::IdleState) {
//		audioOutput->stop();
//		inputFile.close();
//		delete audioOutput;
//	}
//}

//void ExperimentManager_Dialog::AudioStateChanged(QAudio::State newState)
//{
//	switch (newState) {
//	case QAudio::StoppedState:
//		if (audioOutput->error() != QAudio::NoError) {
//			// Perform error handling
//		} else {
//			// Normal stop
//		}
//		break;
//	}
//}

void ExperimentManager_Dialog::exampleButton_3_Pressed()
{
	//if(audioOutput)
	//{
	//	audioOutput->stop();
	//	if (inputFile.isOpen())
	//		inputFile.close();
	//	delete audioOutput;
	//	audioOutput = NULL;
	//}
}

void ExperimentManager_Dialog::exampleButton_4_Pressed()
{
	////Phonon::MediaObject *music = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(
	////music->play();

	//Phonon::MediaObject *mediaObject = new Phonon::MediaObject(this);
	//mediaObject->setCurrentSource(Phonon::MediaSource("E:\\Projects\\StimulGL\\Install\\examples\\Sounds\\level2.wav"));//morsecode.mp3"));//level2.wav"));//WindowsNotify.wav"));
	//Phonon::AudioOutput *audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	//Phonon::Path path = Phonon::createPath(mediaObject, audioOutput);

	//mediaObject->play();
	
}

void ExperimentManager_Dialog::exampleButton_5_Pressed()
{

}


