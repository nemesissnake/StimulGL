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

#include "DefaultPlugin.h"
#include "defines.h"
#include "TimeModel.h"
#include "PhononPlayer.h"

void QDefaultQmlPlugin::registerTypes(const char *uri)
{
	//see --> http://doc.qt.nokia.com/4.7-snapshot/qml-extending.html#qml-type-versioning !!

	Q_ASSERT(uri == QLatin1String(DEFAULT_STIMULGL_PLUGIN_QML_NAME));
	int nTypeID=-1;
	nTypeID = qmlRegisterType<TimeModel,TIMEMODEL_VERSION_STRING_REVISION>(uri, QML_VERSION_STRING_MAJOR, QML_VERSION_STRING_MINOR, TIMEMODEL_TYPE_QML_NAME);
	nTypeID = qmlRegisterType<PhononPlayer,PHONONPLAYER_VERSION_STRING_REVISION>(uri, QML_VERSION_STRING_MAJOR, QML_VERSION_STRING_MINOR, PHONONPLAYER_TYPE_QML_NAME);
}

Q_EXPORT_PLUGIN2(qmlstimulgldefaultplugin, QDefaultQmlPlugin);//qmlqtimeexampleplugin