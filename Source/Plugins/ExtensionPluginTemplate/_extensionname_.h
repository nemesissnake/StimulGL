//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef _extensionname__H
#define _extensionname__H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include "mainappinfo.h"

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
