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


#ifndef QML2VIEWER_H
#define QML2VIEWER_H

//#include "Quick2ContainerWidget.h"
#include "qtquick2applicationviewer.h"
#include "ExperimentEngine.h"
#include "ImageListModel.h"

#define QML2VIEWER_MAINFILEPATH							"qmlmainfilepath"
#define QML2VIEWER_MAX_EVENT_TIME						"qmlmineventtime"

//!  The QML2Viewer class. 
/*!
  The QML2Viewer class can be used in combination with the ExperimentManager for presenting visual or 
  other kind of stimuli using QML/QtQuick.
*/
class QML2Viewer : public ExperimentEngine
{
	Q_OBJECT

signals:
	//! The UserWantsToClose Signal.
	/*!
		You can use this Signal to detect whenever the user tries to abort the experiment.
		No Parameter.
	*/
	void NewSourceLoaded(QString);

public:
	QML2Viewer(QObject *parent = NULL);
	~QML2Viewer();
	QML2Viewer(const QML2Viewer& other){};//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	static QScriptValue ctor_QML2Viewer(QScriptContext* context, QScriptEngine* engine);
	
public:	
	bool executeQML2Document(const QString &strSource, bool bIsFile = true);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool initObject();
	bool startObject();
	bool stopObject();
	bool setObjectID(int nObjID);
	bool initObjectBlockTrial();
	QVariant invokeQml2Method(QString strRootObjectName, QString strMethodName, QVariant inputValue1 = QVariant(), QVariant inputValue2 = QVariant(), QVariant inputValue3 = QVariant(), QVariant inputValue4 = QVariant(), QVariant inputValue5 = QVariant(), QVariant inputValue6 = QVariant(), QVariant inputValue7 = QVariant(), QVariant inputValue8 = QVariant(), QVariant inputValue9 = QVariant());	
	QString addPixmapToImageBuffer(const QPixmap &pixmap);
	bool getPixmapFromImageBuffer(QPixmap *pixmap, const QString &ID);
	bool updatePixmapFromImageBuffer(QPixmap *pixmap, const QString &ID);
	bool removePixmapFromImageBuffer(const QString &ID);
	bool setExperimentManager(ExperimentManager *expManager);

protected:
	bool eventFilter(QObject *target, QEvent *event);

private slots:
	void onStatusChanged(QQuickView::Status status);
	void onQuick2ViewWindowClosed();
	void onStartTriggerRecieved();

private:
	void initialize();
	void parseExperimentObjectBlockParameters(bool bInit = false, bool bSetOnlyToDefault = false);
	void qml2EventRoutine(QString strContent = "");
	void deleteQML2ViewerWindow();

	ExperimentManager *experimentManager;
	cExperimentStructure *currentExperimentStructure;
	QFile tmpFile;
	//QmlErrorHandler *qmlErrorHandler;
	ImageListModel *imgLstModel;
	QtQuick2ApplicationViewer *quick2ViewerWindow;

	QString qmlMainFilePath;
	QObject *rootObject;
	int nQML2ViewerID;							//The ObjectID used to identify the object
	QRectF rectScreenRes;						//The screen resolution
	QColor colorBackground;						//The color of the background
	float stimWidthPixelAmount;					//The amount of visible stimuli pixels(height)
	float stimHeigthPixelAmount;				//The amount of visible stimuli pixels(width)
	bool bParameterChanged;
	bool bFirstQuickWindowAvtivation;
	bool bExperimentUnlocked;
};
Q_DECLARE_METATYPE(QML2Viewer)
Q_DECLARE_METATYPE(QML2Viewer*)

#endif // QML2VIEWER_H
