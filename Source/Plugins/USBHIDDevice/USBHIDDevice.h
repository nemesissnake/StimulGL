//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef USBHIDDevice_H
#define USBHIDDevice_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "hidapi.h"
#include "hidcalibratedialog.h"
#include "hidcapturethread.h"
#include "Global.h"

class HIDCalibrateDialog;
class USBHIDDevice : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	void CaptureThreadStarted(QString);//DateTimeStamp
	void CaptureThreadStopped(QString);//DateTimeStamp
	void CalibrateThreadStarted(QString);//DateTimeStamp
	void CalibrateThreadStopped(QString);//DateTimeStamp
	void CaptureThreadJoystickChanged(int, int);//Xpos,Ypos
	void CaptureThreadButtonTriggered(unsigned char, unsigned char);//NewValue, WhichBitsChanged

public:
	USBHIDDevice(unsigned short vendor_id, unsigned short product_id, QObject *parent = 0);
	~USBHIDDevice();

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

	void ResetCalibrationData();
	void SetCalibrationData(const strcCalibrationData &CalData);

public slots:
	void setExampleProperty( short sExampleProperty );
	short getExampleProperty() const;

	bool Calibrate();
	bool StartCaptureThread(bool bIsCalibrationThread = false);
	void StopCaptureThread(bool bWasCalibrationThread = false);
	void SetVendorID(unsigned short vendor_id);
	void SetProductID(unsigned short product_id);

	void WriteCapturedDataToFile(bool bWriteToFile, QString qsFileName, bool bWriteHeaderInfo, bool bWriteFilteredData);
	void ConfigureHIDTriggers(bool bActivateJoystickTrigger, bool bActivateButtonTriggers, unsigned char cButtonMask, short ButtonDetectDetectionMethod);
	void ConfigureHIDFiltering(bool bActivateJoystickStabilisation, int nJoystickStabilisationThreshold, bool bActivateJoystickHistory, int nJoystickHistorySize);
	void EmulateHIDMouse(bool bEnable, bool bFullScreenMode, bool bEnableLeftMouse = false, bool bEnableRightMouse = false);

private slots:
	void UpdateHIDMouseEmuPosition(int XPos, int YPos);
	void UpdateHIDMouseEmuButtons(unsigned char ButtonByteValue,unsigned char ChangedByteValue);
	
private:
	short m_ExampleProperty;
	HIDCalibrateDialog *HIDCalDiag;
	HIDCaptureThread *HIDCapThread;
	bool m_bActivateJoystickTrigger;
	bool m_bActivateButtonTriggers;
	unsigned char m_cButtonMask;
	short m_sButtonDetectDetectionMethod;
	bool m_bActivateJoystickStabilisation;
	int m_nJoystickStabilisationThreshold;
	bool m_bActivateJoystickHistory;
	bool m_bActivateMousePosEmu;
	bool m_bActivateMouseLButtonEmu;
	bool m_bActivateMouseRButtonEmu;
	//QMouseEvent* mouseEvent;
	//QWidget *widgetUnderCursor;
	QPoint mouseRelPos;
	int m_nJoystickHistorySize;
	bool m_bMouseFullScreenMode;
	QRect rScreenResolution;
	QMutex MouseEmuMutex;

	unsigned short m_VendorId;
	unsigned short m_ProductId;
	bool m_bWriteToFile;
	bool m_bWriteHeaderInfo;
	bool m_bWriteCalibratedData;
	QString m_qsFileName;
	strcCalibrationData m_CalData;  
	int m_nXCalcHIDPos;
	int m_nYCalcHIDPos;
	float m_fScreenWidthStep;
	float m_fScreenHeightStep;
};

#endif // USBHIDDevice_H
