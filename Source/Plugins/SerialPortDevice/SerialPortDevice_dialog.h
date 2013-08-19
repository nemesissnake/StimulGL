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
#include <QtCore/QtGlobal>
#include "ui_SerialPortDevice_dialog.h"
#include "SerialPortDevice.h"
#include <QtWidgets>

class Console;
class SettingsDialog;

class SerialPortDevice_Dialog : public QDialog, private Ui::SerialPortDevice_DialogClass
{
	Q_OBJECT

public:
	SerialPortDevice_Dialog(QWidget *parent = 0);
	~SerialPortDevice_Dialog();

private slots:
	void openSerialPort();
	void closeSerialPort();
	void writeDataString(const QString &sData);
	void readData(const QString &sData);
	//void handleError(QSerialPort::SerialPortError error);

	void on_cancelButton_clicked();
	void on_okButton_clicked();
	void closeEvent(QCloseEvent * e);
	void SendToConsole();

private:
	void initActionsConnections();
	Ui::SerialPortDevice_DialogClass ui;
	Console *console;
	SettingsDialog *settings;
	SerialPortDevice *serial;
	QVBoxLayout mainConsoleLayout;
};

#endif // SerialPortDevice_DIALOG_H
