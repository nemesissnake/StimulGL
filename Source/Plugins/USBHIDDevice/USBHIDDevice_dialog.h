//This file implements the UI functionality

#ifndef USBHIDDevice_DIALOG_H
#define USBHIDDevice_DIALOG_H

#include <QDialog>
#include "ui_USBHIDDevice_dialog.h"
#include <QtGui>
#include <Windows.h>
#include <QDialog>
#include <QThread>
#include "USBHIDDevice.h"

class USBHIDDevice_Dialog : public QDialog, private Ui::USBHIDDevice_DialogClass
{
	Q_OBJECT

public:
	USBHIDDevice_Dialog(QWidget *parent = 0);
	~USBHIDDevice_Dialog();

private:
	USBHIDDevice *UI_USBHIDDeviceObject;
	HIDDevDialogState m_CurrentState;
	bool bCalibrationActive;
	bool bDoReject;
	bool bDoAccept;
	QHBoxLayout *layout;
	RenderArea *renderArea;
	strcCalibrationData m_CalibrationConfiguration;
	Ui::USBHIDDevice_DialogClass ui;
	void ConnectSignalSlots();
	void DisconnectSignalSlots();
	void Initialize();
	void ResetCurrentCalibration();
	void DestroyHIDDevice();
	void UpdateUICalibration();
	
private slots:
	void on_cancelButton_clicked();
	void on_okButton_clicked();
	void closeEvent(QCloseEvent * e);
	void keyPressEvent(QKeyEvent *event);
	void checkBox_EnableFilter_Pressed();
	void checkBox_ToLog_Pressed();
	void checkBox_MouseEmulation_Pressed();
	void checkBox_FullScreen_Pressed();
	void checkBox_LeftMouse_Pressed();
	void checkBox_RightMouse_Pressed();
	void checkBox_EnableStab_Pressed();
	void checkBox_ShowGraph_Pressed();
	void checkBox_ShowRef_Pressed();
	void pushButton_Capture_Pressed();
	void pushButton_Stop_Pressed();
	void pushButton_ResetCal_Pressed();
	void pushButton_Calibrate_Pressed();
	void pushButton_Browse_Pressed();

	void ChangeCurrentState(HIDDevDialogState currState);
	void UpdateCalibration(int nXPos, int nYPos);
	void StartCapture();
	void StopCapture();

	void CaptureHasStarted(QString sMessage);
	void CaptureHasStopped(QString sMessage);
	void HIDButtonPressed(unsigned char cButtonValue,unsigned char cChangedByteValue);
	void HIDJoystickChanged(int nXpos, int nYpos);
};

#endif // USBHIDDevice_DIALOG_H
