#ifndef PPORT_DIALOG_H
#define PPORT_DIALOG_H

#include <QDialog>
#include "ui_pport_dialog.h"
#include <QtGui>
#include <Windows.h>
#include <QDialog>
#include "InpOut32.h"
#include "ppCaptureThread.h"
#include "ppGenerateThread.h"

class PPort_Dialog : public QDialog, private Ui::PPort_DialogClass
{
	Q_OBJECT

public:
	PPort_Dialog(QWidget *parent = 0);
	~PPort_Dialog();

private:
	Ui::PPort_DialogClass ui;
	InpOut32 UI_InpOutObject;
	int testvalue;
	ppCaptureThread *captureThread;
	ppGenerateThread *generateThread;
	short nATBaseAddress;
	short nCBaseAddress;
	short nGBaseAddress;
	QValidator* intValidator;
	bool captureThreadRunning;
	bool generateThreadRunning;

	void ReadPortValues();
	void WritePortValues();
	void GetCurrentPortMode(QString message, short PortAddr);
	short getATBaseAddress();
	short getCBaseAddress();
	short getCBitMask();
	ppCaptureThread::DetectionMethod getCDetectionMethod();
	int getCPostLHTiming();
	int getCPostHLTiming();

	short getGBaseAddress();
	short getGBitActiveValue();
	short getGBitInActiveValue();
	short getGBitMaskValue();
	int getGActivePulseTime();
	int getGRepetitionTime();
	ppGenerateThread::GenerationMethod getGGenerationMethod();

	void cleanUp();

	private slots:
		void on_btnStartGenerateThread_clicked();
		void on_btnStopGenerateThread_clicked();
		void on_chkGenerateMaskB7_stateChanged(int);
		void on_chkGenerateMaskB6_stateChanged(int);
		void on_chkGenerateMaskB5_stateChanged(int);
		void on_chkGenerateMaskB4_stateChanged(int);
		void on_chkGenerateMaskB3_stateChanged(int);
		void on_chkGenerateMaskB2_stateChanged(int);
		void on_chkGenerateMaskB1_stateChanged(int);
		void on_chkGenerateMaskB0_stateChanged(int);
		void on_rbGenMethod_Periodical_toggled(bool);
		void on_rbGenMethod_Pulse_toggled(bool);
		void on_rbGenMethod_Value_toggled(bool);
		void on_rbDecMethodLow_MVC2_toggled(bool);
		void on_rbDecMethodHigh_MVC1_toggled(bool);
		void on_rbDecMethod_MVC0_toggled(bool);

		void on_btnStopCaptureThread_clicked();
		void on_btnStartCaptureThread_clicked();
		void on_cancelButton_clicked();
		void on_btnWritePort_clicked();
		void on_btnReadPort_clicked();
		void on_btnGetMode_clicked();
		void on_btnSetOutputMode_clicked();
		void on_btnSetInputMode_clicked();
		void on_okButton_clicked();
		void on_btnReadBIOS_clicked();

		void UpdateCaptureThreadUI(short Value);
		void CaptureThreadStarted(QString dateTimeStampText);
		void CaptureThreadStopped(QString dateTimeStampText);

		void UpdateGenerateThreadUI(short Value);
		void GenerateThreadStarted(QString dateTimeStampText);
		void GenerateThreadStopped(QString dateTimeStampText);
};

#endif // PPORT_DIALOG_H
