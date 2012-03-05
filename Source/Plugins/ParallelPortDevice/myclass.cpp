#include "myclass.h"

MyClass::MyClass(QObject* parent) : QObject(parent)
{
} // constructor

MyClass::~MyClass()
{
} // destructor

QString MyClass::text() const
{
	return m_text;
} // text

void MyClass::setText(const QString& text)
{
	m_text = text;
} // setText

void MyClass::append(const QString& text)
{
	m_text.append(text);
} // append
