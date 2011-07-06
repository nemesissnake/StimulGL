//This file implements the UI functionality

#ifndef USBHIDDevice_DIALOG_H
#define USBHIDDevice_DIALOG_H

#include <QDialog>
#include "ui_USBHIDDevice_dialog.h"
#include <QtGui>
#include <Windows.h>
#include <QDialog>
#include "USBHIDDevice.h"

class USBHIDDevice_Dialog : public QDialog, private Ui::USBHIDDevice_DialogClass
{
	Q_OBJECT

public:
	USBHIDDevice_Dialog(QWidget *parent = 0);
	~USBHIDDevice_Dialog();

private:
	USBHIDDevice UI_USBHIDDeviceObject;

	Ui::USBHIDDevice_DialogClass ui;
	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void exampleButton_Pressed();
	void on_okButton_clicked();
};

#endif // USBHIDDevice_DIALOG_H
