//_extensionname_
//Copyright (C) 2012  _authorname_
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


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef _extensionname__H
#define _extensionname__H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include "./../../StimulGL/mainappinfo.h"

class _extensionname_ : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	void ExampleSignalTriggered(short);

public:
	_extensionname_(QObject *parent = 0);
	~_extensionname_();
	_extensionname_(const _extensionname_& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	void setExampleProperty( short sExampleProperty );
	short getExampleProperty() const;

private:
	short m_ExampleProperty;

};

#endif // _extensionname__H
