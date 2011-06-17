//This file implements the UI functionality

#ifndef _extensionname__DIALOG_H
#define _extensionname__DIALOG_H

#include <QDialog>
#include "ui__extensionname__dialog.h"
#include <QtGui>
#include <Windows.h>
#include <QDialog>

class _extensionname__Dialog : public QDialog, private Ui::_extensionname__DialogClass
{
	Q_OBJECT

public:
	_extensionname__Dialog(QWidget *parent = 0);
	~_extensionname__Dialog();

private:
	Ui::_extensionname__DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void on_okButton_clicked();
};

#endif // _extensionname__DIALOG_H
