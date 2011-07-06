#include "hidcalibratedialog.h"

HIDCalibrateDialog::HIDCalibrateDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(tr("HID Calibration"));
	renderArea = new RenderArea;
	//QGridLayout *mainLayout = new QGridLayout;
	//mainLayout->setColumnStretch(0, 1);
	ui.gridLayout->addWidget(renderArea, 0, 0, 1, 4);
	//setLayout(ui.gridLayout);
	//ui.labe/*.label*/
	connect(ui.okButton, SIGNAL(clicked()), this, SLOT(OkButtonClicked()));
}

HIDCalibrateDialog::~HIDCalibrateDialog()
{

}

void HIDCalibrateDialog::OkButtonClicked()
{

}