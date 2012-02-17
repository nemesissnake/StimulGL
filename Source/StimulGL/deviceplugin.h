//StimulGL
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


/****************************************************************************
**
**
****************************************************************************/

#ifndef DEVICEPLUGIN_H
#define DEVICEPLUGIN_H

#include <QWidget>
#include <QtScript>
#include "plugininterface.h"

QT_BEGIN_NAMESPACE
class QString;
//class QLineEdit;
//class QLabel;
//class QPushButton;
//class QGridLayout;
QT_END_NAMESPACE

//! [0]
class DevicePluginObject : public QWidget
{
    Q_OBJECT

public:
    DevicePluginObject();
    bool ShowGUI();
    //bool GetGUI();
	//QScriptValue ExecuteScript(const QString &scriptcode);
	bool ConfigureScriptEngine(QScriptEngine &eng);

//private slots:
//    //void test();
//
//signals:
//	void DoSignal();

private:
    //void createGUI();
    //bool loadPlugin();

    DeviceInterface *DInterface;
    //QLineEdit *lineEdit;
    //QLabel *label;
    //QPushButton *button;
    //QGridLayout *layout;
};

#endif // DEVICEPLUGIN_H
