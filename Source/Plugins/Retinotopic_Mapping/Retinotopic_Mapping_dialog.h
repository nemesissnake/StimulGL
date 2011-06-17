//This file implements the UI functionality

#ifndef Retinotopic_Mapping_DIALOG_H
#define Retinotopic_Mapping_DIALOG_H

#include "ui_Retinotopic_Mapping_dialog.h"
#include "Retinotopic_Mapping.h"
#include <QtGui>
#include <QDialog>

class Retinotopic_Mapping;

class Retinotopic_Mapping_Dialog : public QDialog, private Ui::Retinotopic_Mapping_DialogClass
{
	Q_OBJECT

public:
	Retinotopic_Mapping_Dialog(QWidget *parent = 0);
	~Retinotopic_Mapping_Dialog();

	Retinotopic_Mapping *getRetinotopic_MappingObject();

public slots:
	void onRunStimuli();
	void onSaveEXP();
	void onSaveBlockTrial();
	void onToggleFullScreen();
	void onToggleDebugInfo();
	void onToggleUSBTrigger();
	void onToggleSimulateTrigger();
	void onBrowseExpFolder();
	void onExperimentTypeSelected(int index);
	void onCOMSelected(int);

protected:
	void loadSelectedExperiment();

private:
	Ui::Retinotopic_Mapping_DialogClass ui;
	Retinotopic_Mapping *pRetinotopic_Mapping_MainObj;
	QString m_ExpFileName;
	QString expFolder;
	QString selectedBlockTrialFileName;

	void cleanUp();
	
private slots:
	void on_cancelButton_clicked();
	void on_okButton_clicked();
	void onExperimentSelected(int index);
	void onBlockTrialFileSelected(int row, int col, int prev_row, int prev_col);

};

#endif // Retinotopic_Mapping_DIALOG_H
