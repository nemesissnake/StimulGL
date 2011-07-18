#ifndef HIDCALIBRATEDIALOG_H
#define HIDCALIBRATEDIALOG_H

#include <QDialog>
#include "ui_hidcalibratedialog.h"
#include "renderarea.h"
#include "USBHIDDevice.h"
#include "Global.h"

class USBHIDDevice;
class HIDCalibrateDialog : public QDialog
{
	Q_OBJECT

public:
	HIDCalibrateDialog(USBHIDDevice *pUSBHIDDEV, QWidget *parent = NULL);
	~HIDCalibrateDialog();

private:
	void ApplyCalibrationData();
	void StartCalibration();
	void StopCalibration();
	void ResetCurrentCalibration();
	bool Initialize();
	
	Ui::HIDCalibrateDialog ui;
	QHBoxLayout *layout;
	RenderArea *renderArea;
	USBHIDDevice *m_USBHIDDev;
	bool m_bCalibrationActive;
	bool m_bSuccesfullyInitialized;
	strcCalibrationData m_CalibrationConfiguration;

public slots:
	void UpdateCalibration(int nXPos, int nYPos);
	void CalibrationStarted(QString sMessage);
	void CalibrationStopped(QString sMessage);
	void HIDButtonPressed(unsigned char cButtonValue,unsigned char cChangedByteValue);

private slots:
	void OkButtonClicked();
	void StartButtonClicked();
	void StopButtonClicked();
	void CancelButtonClicked();
};

#endif // HIDCALIBRATEDIALOG_H
