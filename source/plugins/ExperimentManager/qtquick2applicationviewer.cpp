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

#include "qtquick2applicationviewer.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtQml/QQmlEngine>

class QtQuick2ApplicationViewerPrivate
{
    QString mainQmlFile;
    friend class QtQuick2ApplicationViewer;
    static QString adjustPath(const QString &path);
};

QString QtQuick2ApplicationViewerPrivate::adjustPath(const QString &path)
{
#if defined(Q_OS_MAC)
    if (!QDir::isAbsolutePath(path))
        return QString::fromLatin1("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#elif defined(Q_OS_UNIX)
    const QString pathInInstallDir =
            QString::fromLatin1("%1/../%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
    return path;
}

QtQuick2ApplicationViewer::QtQuick2ApplicationViewer(QObject *parent) : QQuickView(), d(new QtQuick2ApplicationViewerPrivate())
{
	//configureEventFilter(parent);
    //bool bResult = connect(engine(), &QQmlEngine::quit, this, &QWindow::close);
	bool bResult = connect(engine(), &QQmlEngine::quit, this, &QtQuick2ApplicationViewer::qtQuick2EngineQuit);
    setResizeMode(QQuickView::SizeRootObjectToView);
}

void QtQuick2ApplicationViewer::qtQuick2EngineQuit()
{
	QWindow::close();
	emit QtQuickEngineQuit();
}

QtQuick2ApplicationViewer::~QtQuick2ApplicationViewer()
{
    delete d;
}

void QtQuick2ApplicationViewer::setMainQmlFile(const QString &file)
{
    d->mainQmlFile = QtQuick2ApplicationViewerPrivate::adjustPath(file);
    setSource(QUrl::fromLocalFile(d->mainQmlFile));
}

void QtQuick2ApplicationViewer::addImportPath(const QString &path)
{
    engine()->addImportPath(QtQuick2ApplicationViewerPrivate::adjustPath(path));
}

void QtQuick2ApplicationViewer::showExpanded()
{
#if defined(Q_WS_SIMULATOR)
    showFullScreen();
#else
    show();
#endif
}

//void QtQuick2ApplicationViewer::closeEvent(QCloseEvent *event)
//{
//	int ee =4;
//};

//void QtQuick2ApplicationViewer::configureEventFilter(QObject* parentObject)
//{
	//if (parentObject)
	//{
	//	window_->removeEventFilter(this);
		//disconnect(window_, 0, this, 0);
	//}
	// else do nothing

	//if (window_ = window)
	//if(parentObject)
	//{
		//Q_ASSERT(windowHandle());
		//window->setParent(windowHandle());
		//window->setOpacity(.0);
		//if (window->size().isEmpty())
		//{
		//qobject_cast<QQuickWindow*>(this)->installEventFilter(parentObject);
		//}
		//else
		//{
		//	resize(window->size());
		//}
		//window->setPosition(width(), height());
		//connect(window, SIGNAL(afterRendering()), SLOT(afterRendering()),Qt::DirectConnection);
		//connect(window, SIGNAL(beforeRendering()), SLOT(beforeRendering()),Qt::DirectConnection);
	//}
	// else do nothing
//}

//bool QtQuick2ApplicationViewer::event(QEvent* event)
//{
//	if (window_)
//	{
//		switch (event->type())
//		{
//		case QEvent::FocusIn:
//		case QEvent::FocusOut:
//		case QEvent::FocusAboutToChange:
//		case QEvent::KeyPress:
//
//
//				//		if (event->type() == QEvent::KeyPress) 
//				//		{
//				//			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
//				//
//				//			switch (keyEvent->key())
//				//			{
//				//			case Qt::Key_A://Key_Escape:	//To abort the experiment
//				//				if((keyEvent->modifiers() & Qt::ControlModifier))// && (keyEvent->modifiers() & Qt::AltModifier))
//				//				{
//				//					emit UserWantsToClose();
//				//				}
//				//				break;
//				//			//case Qt::Key_Tab:
//				//			//	setExperimentObjectReadyToUnlock();
//				//			//	break;
//				//			case Qt::Key_Alt:	//To start the experiment
//				//				setExperimentObjectReadyToUnlock();
//				//				//if(target == alternativeContainerDlg)//here we don't have a trigger object because of the lack of the ExperimentManager, so we trigger it automatically to start
//				//				//	tStimTimer.singleShot(100, this, SLOT(incrementExternalTrigger()));//incrementExternalTrigger();
//				//				break;
//				//			}
//				//		}
//
//		case QEvent::KeyRelease:
//		case QEvent::Close:
//			//emit ViewClosed();
//			//return true;
//		case QEvent::Wheel:
//			//QCoreApplication::sendEvent(window_, event);
//			//return true;
//		case QEvent::MouseButtonPress:
//		case QEvent::MouseButtonRelease:
//		case QEvent::MouseButtonDblClick:
//		case QEvent::MouseMove:
//			//{
//			//	QPointF localPos(static_cast<QMouseEvent*>(event)->localPos());
//			//	localPos.setY(height() - localPos.y());
//			//	QPointF windowPos(static_cast<QMouseEvent*>(event)->windowPos());
//			//	windowPos.setY(height() - windowPos.y());
//			//	QMouseEvent mouseEvent(event->type(), localPos, windowPos,
//			//		static_cast<QMouseEvent*>(event)->button(),
//			//		static_cast<QMouseEvent*>(event)->buttons(),
//			//		static_cast<QMouseEvent*>(event)->modifiers());
//			//	QCoreApplication::sendEvent(window_, &mouseEvent);
//			//	return true;
//			//}
//		case QEvent::Resize:
//			//if (window_)
//			//{
//			//	QSize size(static_cast<QResizeEvent*>(event)->size());
//			//	window_->setGeometry(size.width(), size.height(),
//			//		size.width(), size.height());
//			//}
//			//// else do nothing
//			//break;
//		case QEvent::Show:
//		//	if (window_)
//		//	{
//		//		window_->show();
//		//	}
//		//	// else do nothing
//		default:
//			break;
//		}
//	}
//	//QEvent::Type tmpEvent = event->type();
//	return false;//QWidget::event(event);
//}

//bool QtQuick2ApplicationViewer::eventFilter(QObject*, QEvent* event)
//{
//	//Q_ASSERT(window_);
//	switch (event->type())
//	{
//	case QEvent::Resize:
//		//{
//		//	QSize size(static_cast<QResizeEvent*>(event)->size());
//		//	resize(size);
//		//	window_->setPosition(size.width(), size.height());
//		//	window_->removeEventFilter(this);
//		//	break;
//		//}
//	default:
//		break;
//	}
//	return false;
//}
