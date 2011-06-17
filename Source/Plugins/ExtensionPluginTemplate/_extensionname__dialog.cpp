#include "_extensionname__dialog.h"

_extensionname__Dialog::_extensionname__Dialog(QWidget *parent)	: QDialog(parent)
{
	//Gets constructed only once during the load of the plugin
	ui.setupUi(this);
	connect(ui.btnExampleButton, SIGNAL(clicked()), this, SLOT(exampleButton_Pressed()));
}

_extensionname__Dialog::~_extensionname__Dialog()
{

}

void _extensionname__Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void _extensionname__Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void _extensionname__Dialog::cleanUp()
{
	return;
}

void _extensionname__Dialog::exampleButton_Pressed()
{
	QMessageBox msgBox;
	msgBox.setText("Example button pressed.");
	msgBox.exec();
}
