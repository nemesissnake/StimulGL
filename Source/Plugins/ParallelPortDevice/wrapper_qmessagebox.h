#ifndef WRAPPER_QMESSAGEBOX_H
#define WRAPPER_QMESSAGEBOX_H

#include <QObject>
#include <QMessageBox>
#include <QScriptable>
#include <QString>
#include <QScriptEngine>

// wrapper around QMessageBox:
class Wrapper_QMessageBox: public QObject, protected QScriptable//QMessageBox
{
	Q_OBJECT
	//Q_PROPERTY(QString text WRITE setText READ text)

public:

	Wrapper_QMessageBox(QWidget *parent =0);// : QMessageBox(parent);
	~Wrapper_QMessageBox();

public slots:
	QScriptValue qscript_call(QWidget *parent = 0);

private:

};

#endif // WRAPPER_QMESSAGEBOX_H
