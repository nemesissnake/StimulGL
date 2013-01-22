//SerialPortDeviceplugin
//Copyright (C) 2012  Sven Gijsen
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

SerialPortDevice_Dialog::SerialPortDevice_Dialog(QWidget *parent) : QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);

#ifdef Q_WS_WIN
	ui.portBox->addItems(QStringList()<<"COM1"<<"COM2"<<"COM3"<<"COM4"<<"COM5"<<"COM6"<<"COM7"<<"COM8"<<"COM9");
#else
	ui.portBox->addItems(QStringList()<<"/dev/ttyS0"<<"/dev/ttyS1"<<"/dev/ttyUSB0"<<"/dev/ttyUSB1");
#endif
	//make sure user can input their own port name!
	ui.portBox->setEditable(true);

	ui.baudRateBox->addItem("1200", BAUD1200);	
	ui.baudRateBox->addItem("2400", BAUD2400);
	ui.baudRateBox->addItem("4800", BAUD4800);
	ui.baudRateBox->addItem("9600", BAUD9600);
	ui.baudRateBox->addItem("19200", BAUD19200);
	ui.baudRateBox->addItem("38400", BAUD38400);
	ui.baudRateBox->addItem("57600", BAUD57600);
	ui.baudRateBox->addItem("115200", BAUD115200);
	ui.baudRateBox->setCurrentIndex(3);

	ui.parityBox->addItem("NONE", PAR_NONE);
	ui.parityBox->addItem("ODD", PAR_ODD);
	ui.parityBox->addItem("EVEN", PAR_EVEN);

	ui.dataBitsBox->addItem("5", DATA_5);
	ui.dataBitsBox->addItem("6", DATA_6);
	ui.dataBitsBox->addItem("7", DATA_7);
	ui.dataBitsBox->addItem("8", DATA_8);
	ui.dataBitsBox->setCurrentIndex(3);

	ui.stopBitsBox->addItem("1", STOP_1);
	ui.stopBitsBox->addItem("2", STOP_2);

	ui.queryModeBox->addItem("Polling", QextSerialPort::Polling);
	ui.queryModeBox->addItem("EventDriven", QextSerialPort::EventDriven);

	ui.rdb_PortIsOpen->setChecked(false);

	serialTimer = new QTimer(this);
	serialTimer->setInterval(40);

	serialSettings.BaudRate = BAUD9600;
	serialSettings.DataBits = DATA_8;
	serialSettings.FlowControl = FLOW_OFF;
	serialSettings.Parity = PAR_NONE;
	serialSettings.StopBits = STOP_1;
	serialSettings.Timeout_Millisec = 10;

	serialPort = new QextSerialPort(ui.portBox->currentText(), serialSettings, QextSerialPort::Polling);

	connect(ui.btnAvailablePorts, SIGNAL(clicked()), this, SLOT(pushButton_btnAvailablePorts_Pressed()));
	connect(ui.baudRateBox, SIGNAL(currentIndexChanged(int)), SLOT(onBaudRateChanged(int)));
	connect(ui.parityBox, SIGNAL(currentIndexChanged(int)), SLOT(onParityChanged(int)));
	connect(ui.dataBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onDataBitsChanged(int)));
	connect(ui.stopBitsBox, SIGNAL(currentIndexChanged(int)), SLOT(onStopBitsChanged(int)));
	connect(ui.queryModeBox, SIGNAL(currentIndexChanged(int)), SLOT(onQueryModeChanged(int)));
	connect(ui.timeoutBox, SIGNAL(valueChanged(int)), SLOT(onTimeoutChanged(int)));
	connect(ui.portBox, SIGNAL(editTextChanged(QString)), SLOT(onPortNameChanged(QString)));
	connect(ui.openCloseButton, SIGNAL(clicked()), SLOT(onOpenCloseButtonClicked()));
	connect(ui.sendButton, SIGNAL(clicked()), SLOT(onSendButtonClicked()));
	connect(serialTimer, SIGNAL(timeout()), SLOT(onReadyRead()));
	connect(serialPort, SIGNAL(readyRead()), SLOT(onReadyRead()));
}

SerialPortDevice_Dialog::~SerialPortDevice_Dialog()
{
	CleanupSerialDevice();
	disconnect(ui.btnAvailablePorts, SIGNAL(clicked()), this, SLOT(pushButton_btnAvailablePorts_Pressed()));
	disconnect(ui.baudRateBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onBaudRateChanged(int)));
	disconnect(ui.parityBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onParityChanged(int)));
	disconnect(ui.dataBitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onDataBitsChanged(int)));
	disconnect(ui.stopBitsBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onStopBitsChanged(int)));
	disconnect(ui.queryModeBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onQueryModeChanged(int)));
	disconnect(ui.timeoutBox, SIGNAL(valueChanged(int)), this, SLOT(onTimeoutChanged(int)));
	disconnect(ui.portBox, SIGNAL(editTextChanged(QString)), this, SLOT(onPortNameChanged(QString)));
	disconnect(ui.openCloseButton, SIGNAL(clicked()), this, SLOT(onOpenCloseButtonClicked()));
	disconnect(ui.sendButton, SIGNAL(clicked()), this, SLOT(onSendButtonClicked()));
	//disconnect(serialTimer, SIGNAL(timeout()), this, SLOT(onReadyRead()));
	//disconnect(serialPort, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
}

void SerialPortDevice_Dialog::CleanupSerialDevice()
{
	if (serialTimer)
	{
		serialTimer->stop();
		delete serialTimer;
		serialTimer = NULL;
	}
	if (serialPort)
	{
		if (serialPort->isOpen())
			onOpenCloseButtonClicked();
		delete serialPort;
		serialPort = NULL;
	}
}

void SerialPortDevice_Dialog::on_okButton_clicked()
{
	if (serialPort->isOpen())
		onOpenCloseButtonClicked();
	accept();
}

void SerialPortDevice_Dialog::on_cancelButton_clicked()
{
	if (serialPort->isOpen())
		onOpenCloseButtonClicked();
	reject();
}

void SerialPortDevice_Dialog::closeEvent(QCloseEvent * e)
{
	on_cancelButton_clicked();
}

void SerialPortDevice_Dialog::pushButton_btnAvailablePorts_Pressed()
{
	QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();

	ui.txtSystemPorts->clear();
	if (ports.count()>0)
	{
		ui.txtSystemPorts->append("List of ports:");
		ui.txtSystemPorts->append("");
		foreach (QextPortInfo info, ports) 
		{
			ui.txtSystemPorts->append("port name:" + info.portName);
			ui.txtSystemPorts->append("friendly name:" + info.friendName);
			ui.txtSystemPorts->append("physical name:" + info.physName);
			ui.txtSystemPorts->append("enumerator name:" + info.enumName);
			ui.txtSystemPorts->append("vendor ID:" + info.vendorID);
			ui.txtSystemPorts->append("product ID:" + info.productID);
			ui.txtSystemPorts->append("===================================");
		}
	} 
	else
	{
		ui.txtSystemPorts->append("No available ports found:");
	}
}


void SerialPortDevice_Dialog::onPortNameChanged(const QString &sName)//name
{
	if (serialPort->isOpen()) 
	{
		serialPort->close();
		ui.rdb_PortIsOpen->setChecked(false);
	}
}

void SerialPortDevice_Dialog::onBaudRateChanged(int idx)
{
	serialPort->setBaudRate((BaudRateType)ui.baudRateBox->itemData(idx).toInt());
}

void SerialPortDevice_Dialog::onParityChanged(int idx)
{
	serialPort->setParity((ParityType)ui.parityBox->itemData(idx).toInt());
}

void SerialPortDevice_Dialog::onDataBitsChanged(int idx)
{
	serialPort->setDataBits((DataBitsType)ui.dataBitsBox->itemData(idx).toInt());
}

void SerialPortDevice_Dialog::onStopBitsChanged(int idx)
{
	serialPort->setStopBits((StopBitsType)ui.stopBitsBox->itemData(idx).toInt());
}

void SerialPortDevice_Dialog::onQueryModeChanged(int idx)
{
	serialPort->setQueryMode((QextSerialPort::QueryMode)ui.queryModeBox->itemData(idx).toInt());
}

void SerialPortDevice_Dialog::onTimeoutChanged(int val)
{
	serialPort->setTimeout(val);
}

void SerialPortDevice_Dialog::onOpenCloseButtonClicked()
{
	if (!serialPort->isOpen()) 
	{
		serialPort->setPortName(ui.portBox->currentText());
		serialPort->open(QIODevice::ReadWrite);
	}
	else 
	{
		serialPort->close();
	}
	//If using polling mode, we need a QTimer
	if (serialPort->isOpen() && serialPort->queryMode() == QextSerialPort::Polling)
		serialTimer->start();
	else
		serialTimer->stop();
	//update the status
	ui.rdb_PortIsOpen->setChecked(serialPort->isOpen());
}

void SerialPortDevice_Dialog::onSendButtonClicked()
{
	if (serialPort->isOpen() && !ui.sendEdit->toPlainText().isEmpty())
	{
		QString sToSend = ui.sendEdit->toPlainText();
		if (ui.rdbSendFormat->isChecked())//ASCII?
		{
			serialPort->write(sToSend.toLatin1());
		}
		else//Decimal
		{
			if (sToSend.isEmpty() == false)
			{
				bool bNoError = false;
				int nDecVal = sToSend.toInt(&bNoError);//(int)sToSend;//(int) str[0] - '0';//QString::fromAscii(sToSend.toLatin1()).toInt();//sToSend.toInt(&bNoError);
				if(bNoError)
				{
					nDecVal = nDecVal - '0';
					QString sConverted = QString::number(nDecVal);
					serialPort->write(sConverted.toLatin1());
				}
				else
				{
					QMessageBox msgBox(QMessageBox::Warning,"Exception","Could not send the requested data!",QMessageBox::Ok,this);
					msgBox.exec();
				}
			}			
		}
	}
}

void SerialPortDevice_Dialog::onReadyRead()
{
	if (serialPort->bytesAvailable()) {
		ui.recvEdit->moveCursor(QTextCursor::End);
		QString sReceived = QString::fromLatin1(serialPort->readAll());

		if (ui.rdbRecFormat->isChecked())//ASCII?
		{
			ui.recvEdit->insertPlainText(sReceived);
		} 
		else//Decimal
		{
			if (sReceived.isEmpty() == false)
			{
				QString sConverted = "";
				int nDecVal;
				for (int i=0;i<sReceived.count();i++)
				{
					nDecVal = sReceived[i].toLatin1();
					sConverted = sConverted + QString::number(nDecVal);
				}
				ui.recvEdit->insertPlainText(sConverted);
			}
		}		
	}
}

