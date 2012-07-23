//KeyBoardplugin
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


#ifndef KEYBOARDCAPTURE_H
#define KEYBOARDCAPTURE_H
#include <QObject>
#include <QString>
#include <QScriptable>
#include <QMap>
#include "keyboardCaptureThread.h"
#include "./../../StimulGL/mainappinfo.h"

class keyboardCaptureThread;

//!  The KeyBoard Device class. 
/*!
  The KeyBoard Device can be used to capture the KeyBoard.
*/
class KeyBoardCapture : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_PROPERTY( short BaseAddress WRITE setBaseAddress READ getBaseAddress )

signals:
	//! The CaptureThreadKeyPressed Signal.
	/*!
		You can use this Signal for receiving a notification when the capture thread gets triggered by a KeyBoard key-press.
		The Key parameter holds the pressed Key.
	*/
	void CaptureThreadKeyPressed(quint32 Key);
	//! The CaptureThreadKeyReleased Signal.
	/*!
		You can use this Signal for receiving a notification when the capture thread gets triggered by a KeyBoard key-release.
		The Key parameter holds the released Key.
	*/
	void CaptureThreadKeyReleased(quint32 Key);
	//! The CaptureThreadStarted Signal.
	/*!
		You can use this Signal to keep track of when the capture thread starts.
		Parameter DateTimeStamp holds the date/time of when the capture thread started.
	*/
	void CaptureThreadStarted(QString);
	//! The CaptureThreadStopped Signal.
	/*!
		You can use this Signal to keep track of when the capture thread stops.
		Parameter DateTimeStamp holds the date/time of when the capture thread stopped.
	*/
	void CaptureThreadStopped(QString);

public:
	KeyBoardCapture(QObject *parent = 0);
	KeyBoardCapture(const KeyBoardCapture& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~KeyBoardCapture();

public slots:
	bool StartCaptureThread(const short method);
	void StopCaptureThread();
	QString getVirtualKeyString(const quint32 &keyCode);

private:
	void setupKeyCodeTable();
	QMap<int,QString> mKeyCodes;//QMap<quint32,QString> mKeyCodes;
	keyboardCaptureThread *captureThread;
};

#endif // KEYBOARDCAPTURE_H
