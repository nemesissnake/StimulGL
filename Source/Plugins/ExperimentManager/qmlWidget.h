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

#include "QmlErrorHandler.h"
#include <QAbstractItemModel>
#include <QStringListModel>
#include "ImageListModel.h"

#define QML_WIDGET_MAINFILEPATH							"qmlmainfilepath"
#define QML_WIDGET_MAX_EVENT_TIME						"qmlmineventtime"

//!  The qmlWidget class. 
/*!
  The qmlWidget class can be used in combination with the ExperimentManager for presenting visual or 
  other kind of stimuli using QML/QtQuick.
*/
class qmlWidget : public GLWidgetWrapper
{
	Q_OBJECT

public:
	qmlWidget(QWidget *parent = NULL);
	~qmlWidget();
	qmlWidget(const qmlWidget& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	static QScriptValue ctor_QmlWidget(QScriptContext* context, QScriptEngine* engine);
	
public:	
	bool executeQMLDocument(const QString &strSource, QDialog *ContainerDlg = NULL, bool bIsFile = true);// QVBoxLayout *layout = NULL);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	bool initObject();
	bool startObject();
	bool stopObject();
	bool setObjectConfiguration(QObject *pExpConfStruct = NULL);//ExperimentConfiguration *pExpConfStruct = NULL);
	bool setObjectID(int nObjID);
	bool initObjectBlockTrial();
	bool paintObject(int paintFlags = 0, QObject *paintEventObject = NULL);
	
	QVariant invokeQmlMethod(QString strRootObjectName, QString strMethodName, QVariant inputValue1 = QVariant(), QVariant inputValue2 = QVariant(), QVariant inputValue3 = QVariant(), QVariant inputValue4 = QVariant(), QVariant inputValue5 = QVariant(), QVariant inputValue6 = QVariant(), QVariant inputValue7 = QVariant(), QVariant inputValue8 = QVariant(), QVariant inputValue9 = QVariant());	
	QString addPixmapToImageBuffer(const QPixmap &pixmap);
	bool getPixmapFromImageBuffer(QPixmap *pixmap, const QString &ID);
	bool updatePixmapFromImageBuffer(QPixmap *pixmap, const QString &ID);
	bool removePixmapFromImageBuffer(const QString &ID);

private slots:
	void callAnimate();
	void onStatusChanged(QDeclarativeView::Status status);

private:
	void initialize();
	void parseExperimentObjectBlockParameters(bool bInit = false, bool bSetOnlyToDefault = false);
	void qmlEventRoutine(bool dShowWidget = true, QString strContent = "");

	QFile tmpFile;
	QmlErrorHandler *qmlErrorHandler;
	QWidget *parentWidget;
	ImageListModel *imgLstModel;
	QDeclarativeView *qmlViewer;
	QPalette GlWidgetPallette;
	QString qmlMainFilePath;
	QObject *rootObject;
	ExperimentSnapshotStructure expSnapshot;
	QPainter *stimuliPainter;
	ExperimentConfiguration *currExpConfStruct;
	QTimer tWidgetUpdateLoopTimer;
	int nWidgetMaxEvenTime;
	QGLWidget *glWidget;						//The QGLWidget that acts as the viewport for the QML
	int nQMLWidgetID;							//This variable stores the ObjectID used to identify the object
	QRectF rectScreenRes;						//The screen resolution
	QColor colorBackground;						//The color of the background
	float stimWidthPixelAmount;					//The amount of visible stimuli pixels(height)
	float stimHeigthPixelAmount;				//The amount of visible stimuli pixels(width)
	int lastTriggerNumber;						//To keep track of the last trigger number
	bool bResolutionChanged;
};
Q_DECLARE_METATYPE(qmlWidget)
Q_DECLARE_METATYPE(qmlWidget*)

#endif // QMLWIDGET_H
