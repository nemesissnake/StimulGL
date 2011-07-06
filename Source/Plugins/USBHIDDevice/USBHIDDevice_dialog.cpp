#include "USBHIDDevice_dialog.h"

USBHIDDevice_Dialog::USBHIDDevice_Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

USBHIDDevice_Dialog::~USBHIDDevice_Dialog()
{

}

void USBHIDDevice_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void USBHIDDevice_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void USBHIDDevice_Dialog::cleanUp()
{
	return;
}

void USBHIDDevice_Dialog::exampleButton_Pressed()
{
	//QMessageBox msgBox;
	//msgBox.setText("Example button pressed.");
	//msgBox.exec();
	UI_USBHIDDeviceObject.doMeasurement();
}
