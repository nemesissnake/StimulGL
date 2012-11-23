//DefaultQMLPlugin
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

#include "PhononPlayer.h"
#include <QFileDialog>
#include <qapplication.h>

//int	PhononPlayer::instances=NULL;

/*! \brief The PhononPlayer constructor.
*
*   You do not have to specify the parent object. 
*/
PhononPlayer::PhononPlayer(QObject *parent) : QObject(parent)
{
	//if (++instances == 1) 
	//{
		//if(mediaObject == NULL)
		mediaObject = new Phonon::MediaObject();//this
		//if(audioOutput == NULL)
		audioOutput = new Phonon::AudioOutput(Phonon::NoCategory);//Phonon::MusicCategory);//,this
		setOutputDevice(0);
		//QString aa = audioOutput->outputDevice().name();
		//if (metaInformationResolver == NULL)
		metaInformationResolver = new Phonon::MediaObject(this);
		phononPath = Phonon::createPath(mediaObject, audioOutput);
		//connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
		connect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
		connect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
		connect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)), this, SLOT(sourceChanged(Phonon::MediaSource)));
		connect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
	//}
}

/*! \brief The PhononPlayer destructor.
*
*   You do not have to call the destructor. 
*	The StimulGL script engine automatically performs the garbage collection after you set the object to NULL and the script ends
*/
PhononPlayer::~PhononPlayer()
{
	//if (--instances == 0) 
	//{
		//disconnect(this, SLOT(tick(qint64)));
		disconnect(this, SLOT(stateChanged(Phonon::State,Phonon::State)));
		disconnect(this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
		disconnect(this, SLOT(sourceChanged(Phonon::MediaSource)));
		disconnect(this, SLOT(aboutToFinish()));
		if(mediaObject)
		{
			mediaObject->stop();
			mediaObject->clearQueue();//Removes all playing and enqueued media sources.
			delete mediaObject;
			mediaObject = NULL;
		}
		if(audioOutput)
		{
			delete audioOutput;
			audioOutput = NULL;
		}
		if (metaInformationResolver)
		{
			delete metaInformationResolver;
			metaInformationResolver = NULL;
		}
	//}
}

QStringList PhononPlayer::getOutputDevices()
{
/*! \brief Returns a list containing the available Output Devices.
 *
 *  A QStringList is returned after calling this function that
 *  contains the names of the available Output Devices found.
 *  See also PhononPlayer::setOutputDevice.
 */
	QStringList lstOutputDevices;
	//QList<Phonon::EffectDescription> effectDescriptions = Phonon::BackendCapabilities::availableAudioEffects();
	QList<Phonon::AudioOutputDevice> audioOutputDevices = Phonon::BackendCapabilities::availableAudioOutputDevices();
	//foreach (Phonon::EffectDescription effectDescription, effectDescriptions) 
	//{
	//	Phonon::Effect *effect = new Phonon::Effect(effectDescription);
	//	// ... Do something with the effect, like insert it into a media graph
	//}
	foreach (Phonon::AudioOutputDevice audioOutputDevice, audioOutputDevices) 
	{
		Phonon::AudioOutputDevice *device = new Phonon::AudioOutputDevice(audioOutputDevice);
		lstOutputDevices.append(device->name());
	}
	return lstOutputDevices;
}

bool PhononPlayer::setOutputDevice(const int nIndex)
{
/*! \brief Sets the Output Devices that should be used.
 *
 *  Sets the output device index to the integer parameter(nIndex).
 *  See also PhononPlayer::getOutputDevices.
 */
	QList<Phonon::AudioOutputDevice> audioOutputDevices = Phonon::BackendCapabilities::availableAudioOutputDevices();
	if(audioOutputDevices.count() > nIndex)
	{
		return audioOutput->setOutputDevice(audioOutputDevices[nIndex]);
	}
	return false;
}

int PhononPlayer::addFile(const QString strFile)
{
/*! \brief Adds a Media object to a internal playlist.
 *
 *  The Media object at the path, represented by the string parameter(strfile),
 *  is added to the internal playlist.
 *  The index of the newly added media object in the playlist is returned.
 */
	if(strFile.isEmpty())
		return -1;
	int index = mediaSources.size();
	Phonon::MediaSource source(strFile);
	mediaSources.append(strFile); 
	if (!mediaSources.isEmpty())
		metaInformationResolver->setCurrentSource(mediaSources.at(index));
	return index;
}

bool PhononPlayer::addFiles(QStringList strFiles)
{
	if(strFiles.isEmpty())
	{
		strFiles = QFileDialog::getOpenFileNames(NULL ,tr("Select Files"));//, QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
	}
	if (strFiles.isEmpty())
		return false;
	int index = mediaSources.size();
	foreach (QString string, strFiles) 
	{
		Phonon::MediaSource source(string);
		mediaSources.append(source);
	} 
	if (!mediaSources.isEmpty())
		metaInformationResolver->setCurrentSource(mediaSources.at(index));// --> void PhononPlayer::metaStateChanged(Phonon::State newState, Phonon::State oldState)
	return true;
}

bool PhononPlayer::play(const int index)
{
/*! \brief Plays a Media object.
 *
 *  The Media object at the integer parameter (index) is played after calling this function. 
 */

	//QFile file("E:\\Projects\\StimulGL\\Install\\examples\\Sounds\\level2.wav");//level2.wav");
	//if(file.open(QIODevice::ReadOnly))
	//{
	//	QDataStream in1(&file);
	//	// Read characters from file and put into a QByteArray.
	//	QByteArray byteArray;
	//
	//	byteArray = file.read(file.size()-100);//readAll();
	//
	//	//while(!in1.atEnd()) 
	//	//{
	//	//	char *c = new char[1];
	//	//	in1.readRawData(c,1);
	//	//	byteArray.push_back(*c);
	//	//}

	//	file.close();

	//	//QByteArray *bArray;//yourData, dataSize);
	//	//bArray = new QByteArray();
	//	buffer.setData(byteArray);
	//	mediaSources.append(Phonon::MediaSource(&buffer));//This constructor can be very handy in the combination of QByteArray and QBuffer.
	//	mediaObject->setCurrentSource(mediaSources[mediaSources.size()-1]);
	//	mediaObject->play();
	//	
	//	//delete bArray;
	//	return true;
	//}
	//return false;
	

	//if(mediaObject->state() == Phonon::PlayingState)
	//{
	//	do 
	//	{
	//		mediaObject->stop();
	//	}
	//	while (mediaObject->state() == Phonon::PlayingState);
	//}
	mediaObject->clearQueue();//Clears the queue of media sources, this makes reloading!?
	if (mediaSources.size() == 0)
		return false;
	if (index == -1)
	{
		if (mediaSources.count() > 0)
		{
			mediaObject->setCurrentSource(mediaSources[0]);// --> void PhononPlayer::sourceChanged(const Phonon::MediaSource &source)
			mediaObject->play();
			return true;
		}
	} 
	else
	{
		if (mediaSources.count() > index)
		{
			mediaObject->setCurrentSource(mediaSources[index]);// --> void PhononPlayer::sourceChanged(const Phonon::MediaSource &source)
			mediaObject->play();
			return true;
		}
	}
	return false;
}

bool PhononPlayer::pause()
{
/*! \brief Pauses the current playing Media object.
 *
 *  The Media object that is currently playing is paused. 
 */
	mediaObject->pause();
	return true;
}

bool PhononPlayer::stop()
{
/*! \brief Stops the current playing Media object.
 *
 *  The Media object that is currently playing is stopped. 
 */
	mediaObject->stop();
	return true;
}

void PhononPlayer::stateChanged(Phonon::State newState, Phonon::State oldState)
{
	switch (newState) 
	{
	case Phonon::ErrorState:
	//An unrecoverable error occurred. The Object is unusable in this state.
		if (mediaObject->errorType() == Phonon::FatalError) 
		{
			QMessageBox::warning(NULL, tr("Fatal Error"), mediaObject->errorString());
		} 
		else 
		{
			QMessageBox::warning(NULL, tr("Error"), mediaObject->errorString());
		}
		break;
	//case Phonon::LoadingState:
	//	//After construction it might take a while before the Player is
	//	//ready to play(). Normally this doesn't happen for local
	//	//files, but can happen for remote files where the asynchronous
	//	//mime type detection and pre buffering can take a while.
	//	//newState = newState;
	//	break;	
	//case Phonon::PlayingState:
	////The Player is playing a media file.
	//		break;
	//case Phonon::StoppedState:
	////The Player has a valid media file loaded and is ready for playing.    
 //       	break;
	//case Phonon::PausedState:
	////The Player is currently paused.
	//		break;
	//case Phonon::BufferingState:
	////The Player is waiting for data to be able to continue playing.
	//	//newState = newState;
	//	break;
	default:
		break;
	}
}

void PhononPlayer::metaStateChanged(Phonon::State newState, Phonon::State oldState)
{
	if (newState == Phonon::ErrorState) 
	{
		QMessageBox::warning(NULL, tr("Error opening files"), metaInformationResolver->errorString());
		while ((!mediaSources.isEmpty()) && (!(mediaSources.takeLast() == metaInformationResolver->currentSource()))) {}  /* loop */;
		return;
	}

	if ((newState != Phonon::StoppedState) && (newState != Phonon::PausedState))
		return;

	if (metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
		return;

	QMap<QString, QString> metaData = metaInformationResolver->metaData();

	QString title = metaData.value("TITLE");
	if (title == "")
		title = metaInformationResolver->currentSource().fileName();

	//QTableWidgetItem *titleItem = new QTableWidgetItem(title);
	//titleItem->setFlags(titleItem->flags() ^ Qt::ItemIsEditable);
	//QTableWidgetItem *artistItem = new QTableWidgetItem(metaData.value("ARTIST"));
	//artistItem->setFlags(artistItem->flags() ^ Qt::ItemIsEditable);
	//QTableWidgetItem *albumItem = new QTableWidgetItem(metaData.value("ALBUM"));
	//albumItem->setFlags(albumItem->flags() ^ Qt::ItemIsEditable);
	//QTableWidgetItem *yearItem = new QTableWidgetItem(metaData.value("DATE"));
	//yearItem->setFlags(yearItem->flags() ^ Qt::ItemIsEditable);

	//int currentRow = musicTable->rowCount();
	//musicTable->insertRow(currentRow);
	//musicTable->setItem(currentRow, 0, titleItem);
	//musicTable->setItem(currentRow, 1, artistItem);
	//musicTable->setItem(currentRow, 2, albumItem);
	//musicTable->setItem(currentRow, 3, yearItem);

	//if (musicTable->selectedItems().isEmpty()) 
	//{
	//	musicTable->selectRow(0);
	//	mediaObject->setCusdrgrrentSource(metaInformationResolver->currentSource());
	//}

	Phonon::MediaSource source = metaInformationResolver->currentSource();
	int index = mediaSources.indexOf(metaInformationResolver->currentSource()) + 1;
	if (mediaSources.size() > index) 
	{
		metaInformationResolver->setCurrentSource(mediaSources.at(index));
	}
	//else 
	//{
	//	musicTable->resizeColumnsToContents();
	//	if (musicTable->columnWidth(0) > 300)
	//		musicTable->setColumnWidth(0, 300);
	//}
}

void PhononPlayer::aboutToFinish()
{
	////When playback is finishing, i.e., when a media source has been played to the end and the queue is empty, several signals are emitted. 
	////First, the media object will emit aboutToFinish() - shortly before the playback has finished - and then finished(). 
	////The stateChanged() signal will also be emitted with PausedState, which is the state the media object takes when the playback is finished. 
	////If you wish to enter another state, you can connect a slot to finished() and set a new state there.
	//int index = mediaSources.indexOf(mediaObject->currentSource()) + 1;
	//if (mediaSources.size() > index) {
	//	mediaObject->enqueue(mediaSources.at(index));
	//	//Appends source to the queue.	You can use this function to provide the next source after the aboutToFinish() signal has been emitted.
	//}
}

void PhononPlayer::sourceChanged(const Phonon::MediaSource &source)
{
	int a = mediaSources.indexOf(source);	
}
