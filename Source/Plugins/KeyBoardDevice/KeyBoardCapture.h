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
#include <QtScript>
#include <QScriptable>
#include <QMap>
#include "keyboardCaptureThread.h"
#include "./../../StimulGL/mainappinfo.h"

class keyboardCaptureThread;
class QTScriptEngine;

//!  The KeyBoard Device class. 
/*!
  The KeyBoard Device can be used to capture the KeyBoard.
*/
class KeyBoardCapture : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "ExperimentManager");//Can't use defines here!, moc doesn't handle defines, not needed here

signals:
	//void TestSignal(QPixmap *);

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
	KeyBoardCapture(QObject *parent = NULL);
	KeyBoardCapture(const KeyBoardCapture& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	void setScriptEngine(QScriptEngine *scriptEngine) {parentScriptEngine = scriptEngine;};
	~KeyBoardCapture();

public slots:
	bool StartCaptureThread(const short method, bool keyForwarding = true);
	void StopCaptureThread();
	//QString getVirtualKeyString(const quint32 &keyCode);
	//bool installCustomScriptHandlerFunction(QString FuncName);

private:
	void setupKeyCodeTable();
	QMap<int,QString> mKeyCodes;//QMap<quint32,QString> mKeyCodes;
	keyboardCaptureThread *captureThread;

	static QScriptEngine *parentScriptEngine;
};

#endif // KEYBOARDCAPTURE_H
