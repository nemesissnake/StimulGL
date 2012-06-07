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


#ifndef QMLWIDGET_H
#define QMLWIDGET_H

#include "GLWidgetWrapper.h"
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QDeclarativeItem>
#include <QDeclarativeProperty>
#include <QDeclarativeContext>
#include <QDeclarativeView>

#define QML_WIDGET_MAINFILEPATH							"qmlmainfilepath"
#define QML_WIDGET_MAX_EVENT_TIME						"qmlmineventtime"

class qmlWidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	qmlWidget(QWidget *parent = NULL);
	~qmlWidget();
	qmlWidget(const qmlWidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor_QmlWidget(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool initObject();
	bool startObject();
	bool stopObject();
	bool setObjectConfiguration(QObject *pExpConfStruct = NULL);//ExperimentConfiguration *pExpConfStruct = NULL);
	bool setObjectID(int nObjID);
	bool initObjectBlockTrial();
	bool paintObject(int paintFlags = 0, QObject *paintEventObject = NULL);
	//bool invokeQmlMethod(QString strRootObjectName = "", QString strMethodName = "", QVariant &returnedValue = QVariant(), QVariant inputValue1 = "", QVariant inputValue2 = "", QVariant inputValue3 = NULL, QVariant inputValue4 = NULL, QVariant inputValue5 = NULL, QVariant inputValue6 = NULL, QVariant inputValue7 = NULL, QVariant inputValue8 = NULL, QVariant inputValue9 = NULL);
	QVariant invokeQmlMethod(QString strRootObjectName, QString strMethodName, QVariant inputValue1 = QVariant(), QVariant inputValue2 = QVariant(), QVariant inputValue3 = QVariant(), QVariant inputValue4 = QVariant(), QVariant inputValue5 = QVariant(), QVariant inputValue6 = QVariant(), QVariant inputValue7 = QVariant(), QVariant inputValue8 = QVariant(), QVariant inputValue9 = QVariant());

private slots:
	void callAnimate();

private:
	void initialize();
	void parseExperimentObjectBlockParameters(bool bInit = false);
	void qmlEventRoutine(bool dShowWidget = true);

	QDeclarativeView *qmlViewer;
	QPalette GlWidgetPallette;
	QString qmlMainFilePath;
	QObject *rootObject;
	QScriptEngine* currentScriptEngine;
	ExperimentSnapshotStructure expSnapshot;
	QPainter *stimuliPainter;
	ExperimentConfiguration *currExpConfStruct;
	QTimer tWidgetUpdateLoopTimer;
	int nWidgetMaxEvenTime;
	QGLWidget *glWidget;						//The QGLWidget that acts as the viewport for the QML
	int nQMLWidgetID;							//This variable stores the ObjectID used to identify the object
	QRectF rectScreenRes;						//The screen resolution
	//bool firstBlockTrialPaintFrame;				//To determine whether it's the first frame to paint from a new Block trial
	QColor colorBackground;						//The color of the background
	float stimWidthPixelAmount;					//The amount of visible stimuli pixels(height)
	float stimHeigthPixelAmount;				//The amount of visible stimuli pixels(width)
	int lastTriggerNumber;						//To keep track of the last trigger number
	bool bResolutionChanged;
};
Q_DECLARE_METATYPE(qmlWidget)
Q_DECLARE_METATYPE(qmlWidget*)

#endif // QMLWIDGET_H
