#ifndef WRAPPER_QMESSAGEBOX_H
#define WRAPPER_QMESSAGEBOX_H

#include <QObject>
#include <QMessageBox>
#include <QScriptable>
#include <QString>
#include <QScriptEngine>

// wrapper for QMessageBox:
class Wrapper_QMessageBox: public QObject, protected QScriptable
{
	Q_OBJECT

public:

	Wrapper_QMessageBox(QWidget *parent =0);
	~Wrapper_QMessageBox();

public slots:
	QScriptValue qscript_call(QWidget *parent = 0);

private:

};

#endif // WRAPPER_QMESSAGEBOX_H
