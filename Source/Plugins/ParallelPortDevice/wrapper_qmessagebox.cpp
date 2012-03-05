#include "wrapper_qmessagebox.h"

//Wrapper_QMessageBox::Wrapper_QMessageBox(QObject *parent) : QObject(parent)
//{
//
//}
Wrapper_QMessageBox::Wrapper_QMessageBox(QWidget *parent) : QObject(parent)//QMessageBox
{

}

Wrapper_QMessageBox::~Wrapper_QMessageBox()
{

}

QScriptValue Wrapper_QMessageBox::qscript_call(QWidget *parent)
{
	QObject * const iface = new Wrapper_QMessageBox(parent);//QMessageBox
	return engine()->newQObject(iface, QScriptEngine::AutoOwnership); 
}
