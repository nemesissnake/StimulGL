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


//This file implements the UI functionality

#ifndef ExperimentManager_DIALOG_H
#define ExperimentManager_DIALOG_H

#include <QDialog>
#include "ui_ExperimentManager_dialog.h"
#include <QtGui>
//#include <QDeclarativeEngine>
//#include <QDeclarativeComponent>
//#include <QDeclarativeItem>
//#include <QDeclarativeProperty>
//#include <QDeclarativeContext>
//#include <Windows.h>
//#include "ContainerDlg.h"
////#include "qmlapplicationviewer.h"
////#include "stimulglgraphscene.h"
//
////class ApplicationData;
////class StimulGLGraphScene;
//class MyClass;
//class ImageViewer;
//class ApplicationData;
class ExperimentManager_Dialog : public QDialog, private Ui::ExperimentManager_DialogClass
{
	Q_OBJECT

public:
	ExperimentManager_Dialog(QWidget *parent = 0);
	~ExperimentManager_Dialog();

private:
	Ui::ExperimentManager_DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void exampleButton_2_Pressed();
	void exampleButton_3_Pressed();
	void exampleButton_4_Pressed();
	void exampleButton_5_Pressed();
	void on_okButton_clicked();
	//void on_GraphScene_Closed();

private:
	//StimulGLGraphScene *StimGLScene;
	//QGraphicsView *GraphView;
	//QmlApplicationViewer *qmlViewer;
	//QString qmlMainFilePath;
	//QObject *rootObject;
	//MyClass *myClass;
	//ImageViewer *imageViewerObject;
	//ApplicationData *appData;
	//QVBoxLayout *mainLayout;
	//ContainerDlg *stimContainerDlg;
};

//class ApplicationData : public QObject
//{
//	Q_OBJECT
//	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor	NOTIFY backgroundColorChanged)
//
//	signals:
//		void onDataChanged();
//		void backgroundColorChanged();
//
//	public:
//		Q_INVOKABLE QDateTime getCurrentDateTime() const
//		{
//			return QDateTime::currentDateTime();
//		}
//
//		void setBackgroundColor(const QColor &c) {
//			if (c != m_color) {
//				m_color = c;
//				emit backgroundColorChanged();
//			}
//		}
//
//		QColor backgroundColor() const {
//			return m_color;
//		}
//
//	private:
//	//	bool eventFilter(QObject * o,QEvent * e);
//	//	bool eventFilter(QObject *target, QEvent *event);
//
//	private:
//		QColor m_color;
//};
//
//class ImageViewer : public QDeclarativeItem//See also qmlRegisterType<ImageViewer>("MyLibrary", 1, 0, "ImageViewer");
//{
//	Q_OBJECT
//	Q_ENUMS(Status)
//	Q_PROPERTY(Status status READ status NOTIFY statusChanged)
//	Q_PROPERTY(QUrl image READ image WRITE setImage NOTIFY imageChanged)
//
//	signals:
//		void imageChanged();
//		void loadingError(const QString &errorMsg);
//		void statusChanged();
//
//	public:
//		enum Status {
//			Ready,
//			Loading,
//			Error
//		};
//
//		Status status() const {
//			return Ready;
//		};
//
//		ImageViewer() {
//			bool bCouldConnect = QObject::connect(this, SIGNAL(imageChanged()),this, SLOT(changed()));
//		};
//
//		void setImage(const QUrl &url) 
//		{
//			mQUrl = url;
//			sQUrl = url.toString();
//			emit imageChanged();
//			emit statusChanged();
//		};
//
//		QUrl image() const 
//		{
//			return mQUrl;
//		};
//
//		Q_INVOKABLE void changed() {
//			qDebug() << "changed() slot Called";
//		}
//
//	private:
//		QUrl mQUrl;
//		QString sQUrl;
//};
//
//class MyClass : public QObject
//{
//	Q_OBJECT
//	public:
//		Q_INVOKABLE void cppMethod(const QString &msg) {
//			qDebug() << "Called the C++ method with" << msg;
//		}
//		Q_INVOKABLE void cppMethod2(const QString &msg) {
//			qDebug() << "Called the C++ method with" << msg;
//		}
//
//	public slots:
//		void cppSlot(int number) {
//			qDebug() << "Called the C++ slot with" << number;
//		}
//		void cppSignaledSlot(const QString &msg) {
//			qDebug() << "Called the Signaled C++ slot with" << msg;
//		}
//};

#endif // ExperimentManager_DIALOG_H
