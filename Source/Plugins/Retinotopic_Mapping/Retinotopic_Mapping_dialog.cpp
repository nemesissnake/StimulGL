#include "Retinotopic_Mapping_dialog.h"
#include "ui_Retinotopic_Mapping_dialog.h"
//#include "renderWdg.h"

Retinotopic_Mapping_Dialog::Retinotopic_Mapping_Dialog(QWidget *parent)	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle("Retinotopic Mapping");
	pRetinotopic_Mapping_MainObj = NULL;
	pRetinotopic_Mapping_MainObj = new Retinotopic_Mapping(this,&ui);
	setFocusPolicy(Qt::StrongFocus);

	connect(ui.GoBtn, SIGNAL(clicked()), this, SLOT(onRunStimuli()));
	connect(ui.SaveEXPFileBtn, SIGNAL(clicked()), this, SLOT(onSaveEXP()));
	connect(ui.SaveBlockTrialFileBtn, SIGNAL(clicked()), this, SLOT(onSaveBlockTrial()));
	connect(ui.FullScreenToggle, SIGNAL(clicked()), this, SLOT(onToggleFullScreen()));
	connect(ui.DebugInfoToggle, SIGNAL(clicked()), this, SLOT(onToggleDebugInfo()));
	connect(ui.USBTriggerToggle, SIGNAL(clicked()), this, SLOT (onToggleUSBTrigger()));
	connect(ui.SimTriggerToggle, SIGNAL(clicked()), this, SLOT(onToggleSimulateTrigger()));
	connect(ui.BrowseExpFolderBtn, SIGNAL(clicked()), this, SLOT(onBrowseExpFolder()));
	connect(ui.ExperimentCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(onExperimentSelected(int)));
	connect(ui.ExpContentTableWdg, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(onBlockTrialFileSelected(int, int, int, int)));
	connect(ui.ExpTypeCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(onExperimentTypeSelected(int)));
	connect(ui.COMSelectCmb, SIGNAL(currentIndexChanged(int)), this, SLOT(onCOMSelected(int)));
}

Retinotopic_Mapping_Dialog::~Retinotopic_Mapping_Dialog()
{

}

Retinotopic_Mapping *Retinotopic_Mapping_Dialog::getRetinotopic_MappingObject()
{
	return pRetinotopic_Mapping_MainObj;
}

void Retinotopic_Mapping_Dialog::on_okButton_clicked()
{
	cleanUp();
	accept();
}

void Retinotopic_Mapping_Dialog::on_cancelButton_clicked()
{
	cleanUp();
	reject();
}

void Retinotopic_Mapping_Dialog::cleanUp()
{
	return;
}

void Retinotopic_Mapping_Dialog::onToggleFullScreen()
{
	pRetinotopic_Mapping_MainObj->setFullScreenMode(ui.FullScreenToggle->isChecked());
}

void Retinotopic_Mapping_Dialog::onToggleDebugInfo()
{
	pRetinotopic_Mapping_MainObj->setDebugMode(ui.DebugInfoToggle->isChecked());
}

void Retinotopic_Mapping_Dialog::onToggleUSBTrigger()
{
	//if(ui.USBTriggerToggle->isChecked())
	//{
	//}
}

void Retinotopic_Mapping_Dialog::onToggleSimulateTrigger()
{
	//if(ui.SimTriggerToggle->isChecked())
	//{
	//}
}

void Retinotopic_Mapping_Dialog::onCOMSelected(int)
{
	//portNumber = ui.COMSelectCmb->currentText().toInt();
}

void Retinotopic_Mapping_Dialog::onExperimentTypeSelected(int index)
{
	pRetinotopic_Mapping_MainObj->setExperimentType(index);
}

void Retinotopic_Mapping_Dialog::onBrowseExpFolder()
{
	expFolder = QFileDialog::getExistingDirectory(this, "Select Directory", expFolder);
	if(expFolder.isEmpty())
		return;

	QDir::setCurrent(expFolder);
	ui.experimentsFolderEd->setText(expFolder);

	QDir expDir(expFolder);
	QStringList filterList; filterList << "*.exp";
	QStringList expFiles = expDir.entryList(filterList, QDir::Files | QDir::NoDotAndDotDot);

	ui.ExperimentCmb->clear();
	ui.ExperimentCmb->addItems(expFiles);

	loadSelectedExperiment();
}

void Retinotopic_Mapping_Dialog::onSaveEXP()
{
	pRetinotopic_Mapping_MainObj->saveExperiment();
}

void Retinotopic_Mapping_Dialog::onRunStimuli()
{
	pRetinotopic_Mapping_MainObj->runExperiment();
}

//void Retinotopic_Mapping_Dialog::onRenderWdgClosed()//Doesn't get called!!
//{
	//delete stimTimer;
	//stimTimer = NULL;

	//QThread::currentThread()->setPriority(QThread::NormalPriority);
//}

void Retinotopic_Mapping_Dialog::onExperimentSelected(int index)
{
	pRetinotopic_Mapping_MainObj->ClearBlockTrials();
	if(index >= 0)
		loadSelectedExperiment();
}

void Retinotopic_Mapping_Dialog::loadSelectedExperiment()
{
	ui.ExpContentTableWdg->setRowCount(0);

	if (!pRetinotopic_Mapping_MainObj->loadExperiment())
		return;

	ui.ExpContentTableWdg->resizeColumnsToContents();
	ui.ExpContentTableWdg->resizeRowsToContents();

	pRetinotopic_Mapping_MainObj->updateTotalNrOfTRs();
}

void Retinotopic_Mapping_Dialog::onBlockTrialFileSelected(int row, int col, int prev_row, int prev_col)
{
	if(col == 1)
	{
		pRetinotopic_Mapping_MainObj->updateExperimentFromTable();
		return;
	}

	QTableWidgetItem *item = ui.ExpContentTableWdg->currentItem();
	selectedBlockTrialFileName = item->text();

	pRetinotopic_Mapping_MainObj->loadBlockTrialFile(selectedBlockTrialFileName);
}

void Retinotopic_Mapping_Dialog::onSaveBlockTrial()
{
	QString filename = combinePathAndFileName(expFolder, selectedBlockTrialFileName);

	QFile fileB(filename);
	if(!fileB.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, " Retinotopic Mapping", "Can not open file", QMessageBox::Ok);
		return;
	}

	QTextStream ar(&fileB);

	int i;

	QString conditionType = ui.conditionTypeEd->text();
	ar << "ConditionType:    " << conditionType << "\n";

	int nrOfParams = ui.nrOfParamsSpBx->value();
	ar << "NrOfParameters:   " << QString::number(nrOfParams) << "\n";

	ar << "NameOfParameters: ";
	for(i=0; i<nrOfParams; i++)
	{
		QString paramName = ui.nameOfParamsCmBx->itemText(i);
		ar << "\"" << paramName << "\"";
		if(i < (nrOfParams - 1))
			ar << " ";
	}
	ar << "\n";

	QString paramTypes = ui.typeOfParamsEd->text();
	ar << "TypeOfParameters: " << paramTypes << "\n";

	QString infoText = ui.InfoTxEd->toPlainText();
	ar << "Info:             \"" << infoText << "\"\n\n";

	int nrOfEntries = ui.blockTrialContentTbWdg->rowCount();
	ar << "NrOfEntries: " << nrOfEntries << "\n";

	ar.setFieldWidth(6);
	ar.setFieldAlignment(QTextStream::AlignLeft);
	int r, c;
	for(r=0; r<nrOfEntries; r++)
	{
		for(c=0; c<nrOfParams; c++)
		{
			QString paramStr = ui.blockTrialContentTbWdg->item(r, c)->text();

			ar << paramStr;

			if(c < (nrOfParams - 1))
				ar << " ";
		}
		ar << "\n";
	}

	fileB.close();
}
