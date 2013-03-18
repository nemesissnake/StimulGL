//DefaultQMLPlugin
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
		//QT5PhononmediaObject = new Phonon::MediaObject();//this
		//if(audioOutput == NULL)
		//QT5PhononaudioOutput = new Phonon::AudioOutput(Phonon::NoCategory);//Phonon::MusicCategory);//,this
		setOutputDevice(0);
		//QString aa = audioOutput->outputDevice().name();
		//if (metaInformationResolver == NULL)
		//QT5PhononmetaInformationResolver = new Phonon::MediaObject(this);
		//QT5PhononphononPath = Phonon::createPath(mediaObject, audioOutput);
		//connect(mediaObject, SIGNAL(tick(qint64)), this, SLOT(tick(qint64)));
		//QT5Phononconnect(mediaObject, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(stateChanged(Phonon::State,Phonon::State)));
		//QT5Phononconnect(metaInformationResolver, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
		//QT5Phononconnect(mediaObject, SIGNAL(currentSourceChanged(Phonon::MediaSource)), this, SLOT(sourceChanged(Phonon::MediaSource)));
		//QT5Phononconnect(mediaObject, SIGNAL(aboutToFinish()), this, SLOT(aboutToFinish()));
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
		//QT5Phonondisconnect(this, SLOT(stateChanged(Phonon::State,Phonon::State)));
		//QT5Phonondisconnect(this, SLOT(metaStateChanged(Phonon::State,Phonon::State)));
		//QT5Phonondisconnect(this, SLOT(sourceChanged(Phonon::MediaSource)));
		//QT5Phonondisconnect(this, SLOT(aboutToFinish()));
		//QT5Phononif(mediaObject)
		{
		//QT5Phonon	mediaObject->stop();
		//QT5Phonon	mediaObject->clearQueue();//Removes all playing and enqueued media sources.
		//QT5Phonon	delete mediaObject;
		//QT5Phonon	mediaObject = NULL;
		}
		//QT5Phononif(audioOutput)
		{
		//QT5Phonon	delete audioOutput;
		//QT5Phonon	audioOutput = NULL;
		}
		//QT5Phononif (metaInformationResolver)
		//QT5Phonon{
		//QT5Phonon	delete metaInformationResolver;
		//QT5Phonon	metaInformationResolver = NULL;
		//QT5Phonon}
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
	//QT5PhononQList<Phonon::AudioOutputDevice> audioOutputDevices = Phonon::BackendCapabilities::availableAudioOutputDevices();
	//foreach (Phonon::EffectDescription effectDescription, effectDescriptions) 
	//{
	//	Phonon::Effect *effect = new Phonon::Effect(effectDescription);
	//	// ... Do something with the effect, like insert it into a media graph
	//}
	//QT5Phononforeach (Phonon::AudioOutputDevice audioOutputDevice, audioOutputDevices) 
	//QT5Phonon{
	//QT5Phonon	Phonon::AudioOutputDevice *device = new Phonon::AudioOutputDevice(audioOutputDevice);
	//QT5Phonon	lstOutputDevices.append(device->name());
	//QT5Phonon}
	return lstOutputDevices;
}

bool PhononPlayer::setOutputDevice(const int nIndex)
{
/*! \brief Sets the Output Devices that should be used.
 *
 *  Sets the output device index to the integer parameter(nIndex).
 *  See also PhononPlayer::getOutputDevices.
 */
	//QT5PhononQList<Phonon::AudioOutputDevice> audioOutputDevices = Phonon::BackendCapabilities::availableAudioOutputDevices();
	//QT5Phononif(audioOutputDevices.count() > nIndex)
	{
	//QT5Phonon	return audioOutput->setOutputDevice(audioOutputDevices[nIndex]);
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
//QT5Phonon	int index = mediaSources.size();
	//QT5PhononPhonon::MediaSource source(strFile);
//QT5Phonon	mediaSources.append(strFile); 
//QT5Phonon	if (!mediaSources.isEmpty())
		//QT5PhononmetaInformationResolver->setCurrentSource(mediaSources.at(index));
	return 0;//QT5Phononindex;
}

bool PhononPlayer::addFiles(QStringList strFiles)
{
	if(strFiles.isEmpty())
	{
		strFiles = QFileDialog::getOpenFileNames(NULL ,tr("Select Files"));//, QDesktopServices::storageLocation(QDesktopServices::MusicLocation));
	}
	if (strFiles.isEmpty())
		return false;
//QT5Phonon	int index = mediaSources.size();
	foreach (QString string, strFiles) 
	{
//QT5Phonon		Phonon::MediaSource source(string);
	//QT5Phonon	mediaSources.append(source);
	} 
//QT5Phonon	if (!mediaSources.isEmpty())
		//QT5PhononmetaInformationResolver->setCurrentSource(mediaSources.at(index));// --> void PhononPlayer::metaStateChanged(Phonon::State newState, Phonon::State oldState)
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
//QT5Phonon	mediaObject->clearQueue();//Clears the queue of media sources, this makes reloading!?
//QT5Phonon	if (mediaSources.size() == 0)
		return false;
	if (index == -1)
	{
//QT5Phonon		if (mediaSources.count() > 0)
		{
	//QT5Phonon		mediaObject->setCurrentSource(mediaSources[0]);// --> void PhononPlayer::sourceChanged(const Phonon::MediaSource &source)
	//QT5Phonon		mediaObject->play();
			return true;
		}
	} 
	else
	{
	//QT5Phonon	if (mediaSources.count() > index)
		{
	//QT5Phonon		mediaObject->setCurrentSource(mediaSources[index]);// --> void PhononPlayer::sourceChanged(const Phonon::MediaSource &source)
	//QT5Phonon		mediaObject->play();
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
//QT5Phonon	mediaObject->pause();
	return true;
}

bool PhononPlayer::stop()
{
/*! \brief Stops the current playing Media object.
 *
 *  The Media object that is currently playing is stopped. 
 */
	//QT5PhononmediaObject->stop();
	return true;
}

//QT5Phononvoid PhononPlayer::stateChanged(Phonon::State newState, Phonon::State oldState)
//QT5Phonon{
//QT5Phonon	switch (newState) 
//QT5Phonon	{
//QT5Phonon	case Phonon::ErrorState:
	//An unrecoverable error occurred. The Object is unusable in this state.
//QT5Phonon		if (mediaObject->errorType() == Phonon::FatalError) 
//QT5Phonon		{
	//QT5Phonon		QMessageBox::warning(NULL, tr("Fatal Error"), mediaObject->errorString());
	//QT5Phonon	} 
//QT5Phonon		else 
	//QT5Phonon	{
//QT5Phonon			QMessageBox::warning(NULL, tr("Error"), mediaObject->errorString());
//QT5Phonon		}
//QT5Phonon		break;
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
//QT5Phonon	default:
//QT5Phonon		break;
//QT5Phonon	}
//QT5Phonon}

//QT5Phononvoid PhononPlayer::metaStateChanged(Phonon::State newState, Phonon::State oldState)
//QT5Phonon{
//QT5Phonon	if (newState == Phonon::ErrorState) 
//QT5Phonon	{
	//QT5Phonon	QMessageBox::warning(NULL, tr("Error opening files"), metaInformationResolver->errorString());
//QT5Phonon		while ((!mediaSources.isEmpty()) && (!(mediaSources.takeLast() == metaInformationResolver->currentSource()))) {}  /* loop */;
	//QT5Phonon	return;
//QT5Phonon	}

//QT5Phonon	if ((newState != Phonon::StoppedState) && (newState != Phonon::PausedState))
	//QT5Phonon	return;

//QT5Phonon	if (metaInformationResolver->currentSource().type() == Phonon::MediaSource::Invalid)
//QT5Phonon		return;

//QT5Phonon	QMap<QString, QString> metaData = metaInformationResolver->metaData();

//QT5Phonon	QString title = metaData.value("TITLE");
//QT5Phonon	if (title == "")
//QT5Phonon		title = metaInformationResolver->currentSource().fileName();

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

//QT5Phonon	Phonon::MediaSource source = metaInformationResolver->currentSource();
//QT5Phonon	int index = mediaSources.indexOf(metaInformationResolver->currentSource()) + 1;
//QT5Phonon	if (mediaSources.size() > index) 
//QT5Phonon	{
//QT5Phonon		metaInformationResolver->setCurrentSource(mediaSources.at(index));
//QT5Phonon	}
	//else 
	//{
	//	musicTable->resizeColumnsToContents();
	//	if (musicTable->columnWidth(0) > 300)
	//		musicTable->setColumnWidth(0, 300);
	//}
//QT5Phonon}

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

//QT5Phononvoid PhononPlayer::sourceChanged(const Phonon::MediaSource &source)
//QT5Phonon{
//QT5Phonon	int a = mediaSources.indexOf(source);	
//QT5Phonon}
