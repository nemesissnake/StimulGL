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


#include "QmlErrorHandler.h"
#include <QDebug>

QmlErrorHandler::QmlErrorHandler(QDeclarativeView &view, QObject *parent) : QObject(parent), mView(view), mErrorOccured(false)
{
	connect(&view, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(handleQmlStatusChange(QDeclarativeView::Status)));
	connect((QObject*)view.engine(), SIGNAL(warnings(QList<QDeclarativeError>)), this, SLOT(handleQmlErrors(QList<QDeclarativeError>)));
}

QmlErrorHandler::~QmlErrorHandler()
{
	disconnect(this, SLOT(handleQmlStatusChange(QDeclarativeView::Status)));
	disconnect(this, SLOT(handleQmlErrors(QList<QDeclarativeError>)));
}

void QmlErrorHandler::handleQmlStatusChange(QDeclarativeView::Status status)
{
	if (status == QDeclarativeView::Error) 
	{
		handleQmlErrors(mView.errors());
	}
}

void QmlErrorHandler::handleQmlErrors(const QList<QDeclarativeError>& qmlErrors)
{
	QStringList errors;
	foreach (const QDeclarativeError& error, qmlErrors) 
	{
		// Special case for bug in QtComponents 1.1
		// https://bugreports.qt-project.org/browse/QTCOMPONENTS-1217
		if (error.url().toString().endsWith("PageStackWindow.qml") && error.line() == 70)
			continue;

		errors.append(error.toString());
	}

	if (errors.isEmpty())
		return;

	mErrorOccured = true;

	//QMessageBox msgBox;
	//msgBox.setText(errors.join("\n"));
	//msgBox.exec();
	qWarning() << errors.join("\n");
}

bool QmlErrorHandler::errorOccured() const
{
	return mErrorOccured;
}