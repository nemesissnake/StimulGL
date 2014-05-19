#include "Qml2Interface.h"

Qml2Interface::Qml2Interface(QObject *parent) : QObject(parent), m_message("")
{

}

Qml2Interface::~Qml2Interface()
{

}

void Qml2Interface::setmessage(const QString &a) 
{
	if (a != m_message) 
	{
		m_message = a;
		emit messageChanged();
	}
}

QString Qml2Interface::message() const 
{
	return m_message;
}

QString Qml2Interface::RootDirectory() const 
{
	return MainAppInfo::appDirPath();
}

QString Qml2Interface::doSomething()
{ 
	setmessage("something"); 
	return m_message; 
}
