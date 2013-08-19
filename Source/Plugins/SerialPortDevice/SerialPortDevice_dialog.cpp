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

#include "SerialPortDevice_dialog.h"
#include "Console.h"
#include "SettingsDialog.h"
#include <QMessageBox>
//#include <QtSerialPort/QSerialPort>

SerialPortDevice_Dialog::SerialPortDevice_Dialog(QWidget *parent) : QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);

	console = new Console;
	console->setEnabled(false);
	ui.lneDataToSend->setEnabled(false);
	mainConsoleLayout.addWidget(console);
	ui.gbConsole->setLayout(&mainConsoleLayout);
	serial = new SerialPortDevice(this);
	settings = new SettingsDialog(this);

	ui.btnConnect->setEnabled(true);
	ui.btnDisconnect->setEnabled(false);
	//ui->actionQuit->setEnabled(true);
	ui.btnConfigure->setEnabled(true);

	initActionsConnections();
	bool bResult = false;
	//bResult = connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this, SLOT(handleError(QSerialPort::SerialPortError)));
	//connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
	bResult = connect(serial, SIGNAL(SerialDataReceived(QString)), this, SLOT(readData(QString)));		
	bResult = connect(console, SIGNAL(getData(QString)), this, SLOT(writeDataString(QString)));
	bResult = connect(ui.lneDataToSend, SIGNAL(returnPressed()), this, SLOT(SendToConsole()));
	//#ifdef Q_OS_WIN32 //Are we on Windows?
//	ui.portBox->addItems(QStringList()<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<"COM9");
//#else
//	ui.portBox->addItems(QStringList()<<"/dev/ttyS0"<<"/dev/ttyS1"<<"/dev/ttyUSB0"<<"/dev/ttyUSB1");
//#endif
//	//make sure user can input their own port name!
//	ui.portBox->setEditable(true);
//
//	ui.baudRateBox->addItem("1200", BAUD1200);	
//	ui.baudRateBox->addItem("2400", BAUD2400);
//	ui.baudRateBox->addItem("4800", BAUD4800);
//	ui.baudRateBox->addItem("9600", BAUD9600);
//	ui.baudRateBox->addItem("19200", BAUD19200);
//	ui.baudRateBox->addItem("38400", BAUD38400);
//	ui.baudRateBox->addItem("57600", BAUD57600);
//	ui.baudRateBox->addItem("115200", BAUD115200);
//	ui.baudRateBox->setCurrentIndex(3);
//
//	ui.parityBox->addItem("NONE", PAR_NONE);
//	ui.parityBox->addItem("ODD", PAR_ODD);
//	ui.parityBox->addItem("EVEN", PAR_EVEN);
//
//	ui.dataBitsBox->addItem("5", DATA_5);
//	ui.dataBitsBox->addItem("6", DATA_6);
//	ui.dataBitsBox->addItem("7", DATA_7);
//	ui.dataBitsBox->addItem("8", DATA_8);
//	ui.dataBitsBox->setCurrentIndex(3);
//
//	ui.stopBitsBox->addItem("1", STOP_1);
//	ui.stopBitsBox->addItem("2", STOP_2);
//
//	ui.queryModeBox->addItem("Polling", QextSerialPort::Polling);
//	ui.queryModeBox->addItem("EventDriven", QextSerialPort::EventDriven);
//
//	ui.rdb_PortIsOpen->setChecked(false);
//
//	serialTimer = new QTimer(this);
//	serialTimer->setInterval(40);
//
//	serialSettings.BaudRate = BAUD9600;
//	serialSettings.DataBits = DATA_8;
//	serialSettings.FlowControl = FLOW_OFF;
//	serialSettings.Parity = PAR_NONE;
//	serialSettings.StopBits = STOP_1;
//	serialSettings.Timeout_Millisec = 10;
//
//	//serialPort = new QextSerialPort());//ui.portBox->currentText(), serialSettings, QextSerialPort::Polling);
//	serialPort.setPortName(ui.portBox->currentText());
//	serialPort.setQueryMode(QextSerialPort::Polling);
//	serialPort.setBaudRate(serialSettings.BaudRate);
//	serialPort.setDataBits(serialSettings.DataBits);
//	serialPort.setParity(serialSettings.Parity);
//	serialPort.setStopBits(serialSettings.StopBits);
//	serialPort.setFlowControl(serialSettings.FlowControl);
//	serialPort.setTimeout(serialSettings.Timeout_Millisec);
//
//	connect(ui.btnAvailablePorts, SIGNAL(clicked()), this, SLOT(pushButton_btnAvailablePorts_Pressed()));
//	connect(ui.baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
//	connect(ui.parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onParityChanged(int)));
//	connect(ui.dataBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
//	connect(ui.stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));
//	connect(ui.queryModeBox, SIGNAL(currentIndexChanged(int)), SLOT(onQueryModeChanged(int)));
//	connect(ui.timeoutBox, SIGNAL(valueChanged(int)), SLOT(onTimeoutChanged(int)));
//	connect(ui.portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
//	connect(ui.openCloseButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
//	connect(ui.sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
//	connect(serialTimer, SIGNAL(timeout()), SLOT(onReadyRead()));
//	connect(&serialPort, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

void SerialPortDevice_Dialog::SendToConsole()
{
	QString sTextToSend = ui.lneDataToSend->text();
	if(console->parseTextString(sTextToSend))
		ui.lneDataToSend->clear();
}

SerialPortDevice_Dialog::~SerialPortDevice_Dialog()
{
	delete settings;
	//delete ui;

	//CleanupSerialDevice();
	//disconnect(ui.btnAvailablePorts, SIGNAL(clicked()), this, SLOT(pushButton_btnAvailablePorts_Pressed()));
	//disconnect(ui.baudRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onBaudRateChanged(int)));
	//disconnect(ui.parityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onParityChanged(int)));
	//disconnect(ui.dataBitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataBitsChanged(int)));
	//disconnect(ui.stopBitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStopBitsChanged(int)));
	//disconnect(ui.queryModeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onQueryModeChanged(int)));
	//disconnect(ui.timeoutBox, SIGNAL(valueChanged(int)), this, SLOT(onTimeoutChanged(int)));
	//disconnect(ui.portBox, SIGNAL(editTextChanged(QString)), this, SLOT(onPortNameChanged(QString)));
	//disconnect(ui.openCloseButton, SIGNAL(clicked()), this, SLOT(onOpenCloseButtonClicked()));
	//disconnect(ui.sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));
	//disconnect(serialTimer, SIGNAL(timeout()), this, SLOT(onReadyRead()));
	//disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

//void SerialPortDevice_Dialog::CleanupSerialDevice()
//{
	//if (serialTimer)
	//{
	//	serialTimer->stop();
	//	delete serialTimer;
	//	serialTimer = NULL;
	//}
	////if (serialPort)
	////{
	//	if (serialPort.isOpen())
	//		onOpenCloseButtonClicked();
	//	//serialPort.deleteLater();
	//	//serialPort = NULL;
	////}
//}

void SerialPortDevice_Dialog::on_okButton_clicked()
{
	//if (serialPort.isOpen())
	//	onOpenCloseButtonClicked();
	accept();
}

void SerialPortDevice_Dialog::on_cancelButton_clicked()
{
	//if (serialPort.isOpen())
	//	onOpenCloseButtonClicked();
	reject();
}

void SerialPortDevice_Dialog::closeEvent(QCloseEvent * e)
{
	Q_UNUSED(e);
	closeSerialPort();
	on_cancelButton_clicked();
}

//void SerialPortDevice_Dialog::pushButton_btnAvailablePorts_Pressed()
//{
	//QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

	//ui.txtSystemPorts->clear();
	//if (ports.count()>0)
	//{
	//	ui.txtSystemPorts->append("List of ports:");
	//	ui.txtSystemPorts->append("");
	//	foreach (QextPortInfo info, ports) 
	//	{
	//		ui.txtSystemPorts->append("port name:" + info.portName);
	//		ui.txtSystemPorts->append("friendly name:" + info.friendName);
	//		ui.txtSystemPorts->append("physical name:" + info.physName);
	//		ui.txtSystemPorts->append("enumerator name:" + info.enumName);
	//		ui.txtSystemPorts->append("vendor ID:" + info.vendorID);
	//		ui.txtSystemPorts->append("product ID:" + info.productID);
	//		ui.txtSystemPorts->append("===================================");
	//	}
	//} 
	//else
	//{
	//	ui.txtSystemPorts->append("No available ports found:");
	//}
//}

//void SerialPortDevice_Dialog::onPortNameChanged(const QString &sName)//name
//{
//	Q_UNUSED(sName);
	//if (serialPort.isOpen()) 
	//{
	//	serialPort.close();
	//	ui.rdb_PortIsOpen->setChecked(false);
	//}
//}

//void SerialPortDevice_Dialog::onBaudRateChanged(int idx)
//{
	//serialPort.setBaudRate((BaudRateType)ui.baudRateBox->itemData(idx).toInt());
//}

//void SerialPortDevice_Dialog::onParityChanged(int idx)
//{
	//serialPort.setParity((ParityType)ui.parityBox->itemData(idx).toInt());
//}

//void SerialPortDevice_Dialog::onDataBitsChanged(int idx)
//{
	//serialPort.setDataBits((DataBitsType)ui.dataBitsBox->itemData(idx).toInt());
//}

//void SerialPortDevice_Dialog::onStopBitsChanged(int idx)
//{
	//serialPort.setStopBits((StopBitsType)ui.stopBitsBox->itemData(idx).toInt());
//}

//void SerialPortDevice_Dialog::onQueryModeChanged(int idx)
//{
	//serialPort.setQueryMode((QextSerialPort::QueryMode)ui.queryModeBox->itemData(idx).toInt());
//}

//void SerialPortDevice_Dialog::onTimeoutChanged(int val)
//{
	//serialPort.setTimeout(val);
//}

//void SerialPortDevice_Dialog::onOpenCloseButtonClicked()
//{
	//if (!serialPort.isOpen()) 
	//{
	//	serialPort.setPortName(ui.portBox->currentText());
	//	serialPort.open(QIODevice::ReadWrite);
	//}
	//else 
	//{
	//	serialPort.close();
	//}
	//If using polling mode, we need a QTimer
	//if (serialPort.isOpen() && serialPort.queryMode() == QextSerialPort::Polling)
	//	serialTimer->start();
	//else
	//	serialTimer->stop();
	//update the status
	//ui.rdb_PortIsOpen->setChecked(serialPort.isOpen());
//}

//void SerialPortDevice_Dialog::onSendButtonClicked()
//{
	//if (serialPort.isOpen() && !ui.sendEdit->toPlainText().isEmpty())
	//{
	//	QString sToSend = ui.sendEdit->toPlainText();
	//	if (ui.rdbSendFormat->isChecked())//ASCII?
	//	{
	//		serialPort.write(sToSend.toLatin1());
	//	}
	//	else//Decimal
	//	{
	//		if (sToSend.isEmpty() == false)
	//		{
	//			bool bNoError = false;
	//			int nDecVal = sToSend.toInt(&bNoError);//(int)sToSend;//(int) str[0] - '0';//QString::fromAscii(sToSend.toLatin1()).toInt();//sToSend.toInt(&bNoError);
	//			if(bNoError)
	//			{
	//				nDecVal = nDecVal - '0';
	//				QString sConverted = QString::number(nDecVal);
	//				serialPort.write(sConverted.toLatin1());
	//			}
	//			else
	//			{
	//				QMessageBox msgBox(QMessageBox::Warning,"Exception","Could not send the requested data!",QMessageBox::Ok,this);
	//				msgBox.exec();
	//			}
	//		}			
	//	}
	//}
//}

//void SerialPortDevice_Dialog::onReadyRead()
//{
	//if (serialPort.bytesAvailable()) {
	//	ui.recvEdit->moveCursor(QTextCursor::End);
	//	QString sReceived = QString::fromLatin1(serialPort.readAll());

	//	if (ui.rdbRecFormat->isChecked())//ASCII?
	//	{
	//		ui.recvEdit->insertPlainText(sReceived);
	//	} 
	//	else//Decimal
	//	{
	//		if (sReceived.isEmpty() == false)
	//		{
	//			QString sConverted = "";
	//			int nDecVal;
	//			for (int i=0;i<sReceived.count();i++)
	//			{
	//				nDecVal = sReceived[i].toLatin1();
	//				sConverted = sConverted + QString::number(nDecVal);
	//			}
	//			ui.recvEdit->insertPlainText(sConverted);
	//		}
	//	}		
	//}
//}

void SerialPortDevice_Dialog::openSerialPort()
{
	SettingsDialog::Settings p = settings->settings();
	serial->setPortName(p.name);
	if (serial->open(QIODevice::ReadWrite)) 
	{
		if (serial->setBaudRate(p.baudRate)
			&& serial->setDataBits(p.dataBits)
			&& serial->setParity(p.parity)
			&& serial->setStopBits(p.stopBits)
			&& serial->setFlowControl(p.flowControl)) 
		{

				console->setEnabled(true);
				ui.lneDataToSend->setEnabled(true);
				console->setLocalEchoEnabled(p.localEchoEnabled);
				ui.btnConnect->setEnabled(false);
				ui.btnDisconnect->setEnabled(true);
				ui.btnConfigure->setEnabled(false);
				//ui->statusBar->showMessage(tr("Connected to %1 : %2, %3, %4, %5, %6")
				//	.arg(p.name).arg(p.stringBaudRate).arg(p.stringDataBits)
				//	.arg(p.stringParity).arg(p.stringStopBits).arg(p.stringFlowControl));

		} 
		//else 
		//{
		//	serial->close();
			//QMessageBox::critical(this, tr("Error"), serial->errorString());
			//ui->statusBar->showMessage(tr("Open error"));
		//}
	} 
	//else 
	//{
	//	QMessageBox::critical(this, tr("Error"), serial->errorString());
	//	ui->statusBar->showMessage(tr("Configure error"));
	//}
}

void SerialPortDevice_Dialog::closeSerialPort()
{
	serial->close();
	console->setEnabled(false);
	ui.lneDataToSend->setEnabled(false);
	ui.btnConnect->setEnabled(true);
	ui.btnDisconnect->setEnabled(false);
	ui.btnConfigure->setEnabled(true);
	//ui->statusBar->showMessage(tr("Disconnected"));
}

void SerialPortDevice_Dialog::writeDataString(const QString &sData)
{
	serial->writeData(sData);
}

void SerialPortDevice_Dialog::readData(const QString &sData)
{
	console->putData(sData);
}

//void SerialPortDevice_Dialog::handleError(QSerialPort::SerialPortError error)
//{
//	if (error == QSerialPort::ResourceError) {
//		QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
//		closeSerialPort();
//	}
//}

void SerialPortDevice_Dialog::initActionsConnections()
{
	connect(ui.btnConnect, SIGNAL(clicked()), this, SLOT(openSerialPort()));
	connect(ui.btnDisconnect, SIGNAL(clicked()), this, SLOT(closeSerialPort()));
	//connect(ui->actionQuit, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnConfigure, SIGNAL(clicked()), settings, SLOT(show()));
	connect(ui.btnClear, SIGNAL(clicked()), console, SLOT(clear()));
}