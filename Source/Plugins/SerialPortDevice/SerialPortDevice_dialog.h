//SerialPortDeviceplugin
//Copyright (C) 2013  Sven Gijsen
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


//This file implements the UI functionality

#ifndef SerialPortDevice_DIALOG_H
#define SerialPortDevice_DIALOG_H

#include <QDialog>
#include "ui_SerialPortDevice_dialog.h"
//#include <QtGui>
#include <QtWidgets>
//#include <Windows.h>
//#include <QDialog>
#include <QThread>
#include <QTimer>
#include "qextserialenumerator.h"
#include "qextserialport.h"



class SerialPortDevice_Dialog : public QDialog, private Ui::SerialPortDevice_DialogClass
{
	Q_OBJECT

public:
	SerialPortDevice_Dialog(QWidget *parent = 0);
	~SerialPortDevice_Dialog();

private:
	Ui::SerialPortDevice_DialogClass ui;
	QTimer *serialTimer;
	QextSerialPort serialPort;
	PortSettings serialSettings;

private:
	void Initialize();
	void CleanupSerialDevice();

private Q_SLOTS:
	void onPortNameChanged(const QString &sName);
	void onBaudRateChanged(int idx);
	void onParityChanged(int idx);
	void onDataBitsChanged(int idx);
	void onStopBitsChanged(int idx);
	void onQueryModeChanged(int idx);
	void onTimeoutChanged(int val);
	void onOpenCloseButtonClicked();
	void onSendButtonClicked();
	void onReadyRead();
	void pushButton_btnAvailablePorts_Pressed();
	void on_cancelButton_clicked();
	void on_okButton_clicked();
	void closeEvent(QCloseEvent * e);
};

#endif // SerialPortDevice_DIALOG_H
