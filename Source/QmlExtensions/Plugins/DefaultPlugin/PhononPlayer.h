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

#ifndef PHONONPLAYER_H
#define PHONONPLAYER_H

#include <phonon>
#include <QMessageBox>
//#include <QBuffer>

class PhononPlayer : public QObject
{
	Q_OBJECT
		//Q_PROPERTY(int hour READ hour NOTIFY phononChanged)
		//Q_PROPERTY(int minute READ minute NOTIFY phononChanged)

public:
	PhononPlayer(QObject *parent=0);
	~PhononPlayer();
	bool addFiles(QStringList strFiles = QStringList());

public slots:
	bool play(const int index = -1); // or use Q_INVOKABLE bool play(); to make it available in QML
	bool pause();
	bool stop();
	int addFile(const QString strFile = QString());
	
	QStringList getOutputDevices();
	bool setOutputDevice(const int nIndex);

private slots:
	void stateChanged(Phonon::State newState, Phonon::State oldState);
	void sourceChanged(const Phonon::MediaSource &source);
	void metaStateChanged(Phonon::State newState, Phonon::State oldState);
	void aboutToFinish();

	//signals:
	//	void phononChanged();

private:
	Phonon::MediaObject *mediaObject;
	Phonon::AudioOutput *audioOutput;
	Phonon::MediaObject *metaInformationResolver;
	QList<Phonon::MediaSource> mediaSources;
	Phonon::Path phononPath;

	//QBuffer buffer;
	//static int instances;
};

#endif // PHONONPLAYER_H