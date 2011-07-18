#include "USBHIDDevice.h"
#include <windows.h>
#include <QTextOStream>

QScriptValue USBHIDDevice::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new USBHIDDevice(0x181b, 0x4002), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

USBHIDDevice::USBHIDDevice(unsigned short vendor_id, unsigned short product_id, QObject *parent) : QObject(parent)
{
	m_bActivateJoystickTrigger = false;
	m_bActivateButtonTriggers = false;
	m_bActivateMousePosEmu = false;
	m_bMouseFullScreenMode = false;
	m_bActivateMouseLButtonEmu = false;
	m_bActivateMouseRButtonEmu = false;
	m_cButtonMask = 255;
	m_sButtonDetectDetectionMethod = HIDCaptureThread::DetectionMethod::MaskedValueChanged;
	m_VendorId = vendor_id;
	m_ProductId = product_id;
	m_bWriteToFile = false;
	m_bWriteHeaderInfo = true;
	m_bWriteCalibratedData = true;
	m_qsFileName = "";
	m_bActivateJoystickStabilisation = false;
	m_nJoystickStabilisationThreshold = 0;
	m_bActivateJoystickHistory = false;
	m_nJoystickHistorySize = 0;
	HIDCalDiag = NULL;
	HIDCapThread = NULL;
	//mouseEvent = NULL;
	ResetCalibrationData();
}

USBHIDDevice::~USBHIDDevice()
{
	StopCaptureThread();
	if(HIDCalDiag)
	{
		delete HIDCalDiag;
		HIDCalDiag = NULL;
	}
}

void USBHIDDevice::SetVendorID(unsigned short vendor_id)
{
	m_VendorId = vendor_id;
}

void USBHIDDevice::SetProductID(unsigned short product_id)
{
	m_ProductId = product_id;
}

void USBHIDDevice::ResetCalibrationData()
{
	m_CalData.Xmin = 255 * RESSCALEFACTOR;
	m_CalData.Xmax = 0;
	m_CalData.Ymin = 255 * RESSCALEFACTOR;
	m_CalData.Ymax = 0;
	m_CalData.XAbsOffset = 0; 
	m_CalData.YAbsOffset = 0;
	m_CalData.XRelScaleFactor = 0.0f;
	m_CalData.YRelScaleFactor = 0.0f;
}

void USBHIDDevice::SetCalibrationData(const strcCalibrationData &CalData)
{
	m_CalData = CalData;
}

void USBHIDDevice::setExampleProperty( short sExampleProperty )
{
	m_ExampleProperty = sExampleProperty;
	//emit ExampleSignalTriggered(m_ExampleProperty);//Signal that the value has been changed
}

short USBHIDDevice::getExampleProperty() const
{
	return m_ExampleProperty;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool USBHIDDevice::Calibrate()
{
	int nResult;
	bool bRetVal = false;

	if (HIDCalDiag == NULL)
		HIDCalDiag = new HIDCalibrateDialog(this);
	nResult = HIDCalDiag->exec();   //->show();
	switch (nResult)
	{
	case QDialog::Accepted://OK Button Pressed
		nResult = 9;
		bRetVal = true;
		break;
	case QDialog::Rejected://Cancel or Close pressed
		nResult = 10;
		break;
	}	
	delete HIDCalDiag;
	HIDCalDiag = NULL;
	return bRetVal;
}

void USBHIDDevice::ConfigureHIDTriggers(bool bActivateJoystickTrigger, bool bActivateButtonTriggers, unsigned char cButtonMask, short sButtonDetectDetectionMethod)
{
	m_bActivateJoystickTrigger = bActivateJoystickTrigger;
	m_bActivateButtonTriggers = bActivateButtonTriggers;
	m_cButtonMask = cButtonMask;
	m_sButtonDetectDetectionMethod = sButtonDetectDetectionMethod;
}

void USBHIDDevice::ConfigureHIDFiltering(bool bActivateJoystickStabilisation, int nJoystickStabilisationThreshold, bool bActivateJoystickHistory, int nJoystickHistorySize)
{
	m_bActivateJoystickStabilisation = bActivateJoystickStabilisation;
	m_nJoystickStabilisationThreshold = nJoystickStabilisationThreshold;
	m_bActivateJoystickHistory = bActivateJoystickHistory;
	m_nJoystickHistorySize = nJoystickHistorySize;
}

void USBHIDDevice::EmulateHIDMouse(bool bEnable, bool bFullScreenMode, bool bEnableLeftMouse, bool bEnableRightMouse)
{
	//Get The screen resolution
	QDesktopWidget* desktopWidget = QApplication::desktop();
	//get client rect.
	rScreenResolution = desktopWidget->screenGeometry();//availableGeometry();
	//get application rect.
	//QRect applicationRect = desktopWidget->screenGeometry();

	if(!m_bActivateMousePosEmu)
	{
		if(bEnable)//When setting from false to true
		{
			connect(this, SIGNAL(CaptureThreadJoystickChanged(int, int)), this, SLOT(UpdateHIDMouseEmuPosition(int, int))); 
			if (bEnableLeftMouse || bEnableRightMouse)
				connect(this, SIGNAL(CaptureThreadButtonTriggered(unsigned char, unsigned char)), this, SLOT(UpdateHIDMouseEmuButtons(unsigned char, unsigned char)));					
			else
				disconnect(this, SIGNAL(CaptureThreadButtonTriggered(unsigned char, unsigned char)), this, SLOT(UpdateHIDMouseEmuButtons(unsigned char, unsigned char)));	
		}
	}
	else if(m_bActivateMousePosEmu)
	{
		if(!bEnable)//When setting from true to false
		{
			disconnect(this, SIGNAL(CaptureThreadJoystickChanged(int, int)), this, SLOT(UpdateHIDMouseEmuPosition(int, int)));
			disconnect(this, SIGNAL(CaptureThreadButtonTriggered(unsigned char, unsigned char)), this, SLOT(UpdateHIDMouseEmuButtons(unsigned char, unsigned char)));	
		}
		
	}
	m_bActivateMousePosEmu = bEnable;
	m_bMouseFullScreenMode = bFullScreenMode;
	m_bActivateMouseLButtonEmu = bEnableLeftMouse;
	m_bActivateMouseRButtonEmu = bEnableRightMouse;
}

void USBHIDDevice::UpdateHIDMouseEmuPosition(int XPos, int YPos)
{
	MouseEmuMutex.lock();
	m_fScreenWidthStep = (rScreenResolution.width()/(RESSCALEFACTOR));
	m_fScreenHeightStep = (rScreenResolution.height()/(RESSCALEFACTOR));

	if(m_bActivateMousePosEmu)
	{
		m_nXCalcHIDPos = m_fScreenWidthStep * ((XPos-m_CalData.Xmin)*m_CalData.XRelScaleFactor);
		m_nYCalcHIDPos = m_fScreenHeightStep * ((YPos-m_CalData.Ymin)*m_CalData.YRelScaleFactor);
		if (m_bMouseFullScreenMode)
		{
			m_nXCalcHIDPos = (m_nXCalcHIDPos * SQRRTTWO) - (0.5*(SQRRTTWO-1)*rScreenResolution.width());
			m_nYCalcHIDPos = (m_nYCalcHIDPos * SQRRTTWO) - (0.5*(SQRRTTWO-1)*rScreenResolution.height());
			if (m_nXCalcHIDPos >= rScreenResolution.width())
			{
				m_nXCalcHIDPos = rScreenResolution.width()-1;
			}
			else if (m_nXCalcHIDPos < 0)
			{
				m_nXCalcHIDPos = 0;
			}
			if (m_nYCalcHIDPos >= rScreenResolution.height())
			{
				m_nYCalcHIDPos = rScreenResolution.height()-1;
			}
			else if (m_nYCalcHIDPos < 0)
			{
				m_nYCalcHIDPos = 0;
			}
		}
		QCursor::setPos(m_nXCalcHIDPos,m_nYCalcHIDPos);
	}
	MouseEmuMutex.unlock();
}

void USBHIDDevice::UpdateHIDMouseEmuButtons(unsigned char ButtonByteValue,unsigned char ChangedByteValue)
{
	MouseEmuMutex.lock();
	//if(mouseEvent)
	//{
	//	delete mouseEvent;
	//	mouseEvent = NULL;
	//}
	QMouseEvent* mouseEvent;
	QWidget *widgetUnderCursor;
	if(m_bActivateMouseLButtonEmu)
	{
		if ((ChangedByteValue & 0x01) == 0x01)//Did the Left Mouse Button Change?
		{
			widgetUnderCursor = qApp->widgetAt(QCursor::pos());
			mouseRelPos = widgetUnderCursor->mapFromGlobal(QCursor::pos());

			if((ButtonByteValue & 0x01) == 0x01)//Did it Press?
			{
				mouseEvent = new QMouseEvent( QEvent::MouseButtonPress, mouseRelPos, Qt::MouseButton::LeftButton, Qt::MouseButton::LeftButton, Qt::NoModifier);
			}
			else//Was it a Release?
			{
				mouseEvent = new QMouseEvent( QEvent::MouseButtonRelease, mouseRelPos, Qt::MouseButton::LeftButton, Qt::MouseButton::LeftButton, Qt::NoModifier);
			}
			QApplication::postEvent( widgetUnderCursor, mouseEvent );	
			delete mouseEvent;
		}
	}
	if(m_bActivateMouseRButtonEmu)
	{
		if ((ChangedByteValue & 0x02) == 0x02)//Did the Right Mouse Button Change?
		{
			widgetUnderCursor = qApp->widgetAt(QCursor::pos());
			mouseRelPos = widgetUnderCursor->mapFromGlobal(QCursor::pos());

			if((ButtonByteValue & 0x02) == 0x02)//Did it Press?
			{
				mouseEvent = new QMouseEvent( QEvent::MouseButtonPress, mouseRelPos, Qt::MouseButton::RightButton, Qt::MouseButton::RightButton, Qt::NoModifier);
			}
			else//Was it a Release?
			{
				mouseEvent = new QMouseEvent( QEvent::MouseButtonRelease, mouseRelPos, Qt::MouseButton::RightButton, Qt::MouseButton::RightButton, Qt::NoModifier);
			}
			QApplication::postEvent( widgetUnderCursor, mouseEvent );	
			delete mouseEvent;
		}
	}
	widgetUnderCursor = NULL;
	MouseEmuMutex.unlock();
}

void USBHIDDevice::WriteCapturedDataToFile(bool bWriteToFile, QString qsFileName, bool bWriteHeaderInfo, bool bWriteCalibratedData)
{
	m_bWriteToFile = bWriteToFile;
	m_bWriteHeaderInfo = bWriteHeaderInfo;
	m_bWriteCalibratedData = bWriteCalibratedData;
	if (qsFileName!="")
		m_qsFileName = qsFileName;
}

bool USBHIDDevice::StartCaptureThread(bool bIsCalibrationThread)
{
	HIDCaptureThread::DetectionMethod CapDecMethod;
	if (m_sButtonDetectDetectionMethod == 0) 
	{CapDecMethod = HIDCaptureThread::DetectionMethod::MaskedValueChanged;}
	else if (m_sButtonDetectDetectionMethod == 1) 
	{CapDecMethod = HIDCaptureThread::DetectionMethod::MaskedValueChangedHigh;}
	else 
	{CapDecMethod = HIDCaptureThread::DetectionMethod::MaskedValueChangedLow;}
	if (HIDCapThread)
	{
		delete HIDCapThread;
	}
	HIDCapThread = new HIDCaptureThread(m_ProductId,m_VendorId,this);
	HIDCapThread->WriteCapturedDataToFile(m_bWriteToFile,m_qsFileName,m_bWriteHeaderInfo,m_bWriteCalibratedData);
	HIDCapThread->ConfigureHIDTriggers(m_bActivateJoystickTrigger,m_bActivateButtonTriggers,m_cButtonMask,CapDecMethod);
	HIDCapThread->ConfigureHIDFiltering(m_bActivateJoystickStabilisation,m_nJoystickStabilisationThreshold,m_bActivateJoystickHistory,m_nJoystickHistorySize);

	connect(HIDCapThread, SIGNAL(receiveThreadJoystickChanged(int, int)), this, SIGNAL(CaptureThreadJoystickChanged(int, int)));
	if(bIsCalibrationThread)
	{
		connect(HIDCapThread, SIGNAL(receiveThreadStarted(QString)), this, SIGNAL(CalibrateThreadStarted(QString)));
		connect(HIDCapThread, SIGNAL(receiveThreadStopped(QString)), this, SIGNAL(CalibrateThreadStopped(QString)));
	}
	else
	{
		connect(HIDCapThread, SIGNAL(receiveThreadStarted(QString)), this, SIGNAL(CaptureThreadStarted(QString)));
		connect(HIDCapThread, SIGNAL(receiveThreadStopped(QString)), this, SIGNAL(CaptureThreadStopped(QString)));
	}
	connect(HIDCapThread, SIGNAL(receiveThreadButtonTriggered(unsigned char,unsigned char)), this, SIGNAL(CaptureThreadButtonTriggered(unsigned char,unsigned char)));

	HIDCapThread->start();
	return true;
}

void USBHIDDevice::StopCaptureThread(bool bWasCalibrationThread)
{
	if(HIDCapThread)
	{
		HIDCapThread->stop();
		disconnect(HIDCapThread, SIGNAL(receiveThreadJoystickChanged(int, int)), this, SIGNAL(CaptureThreadJoystickChanged(int, int)));
		if(bWasCalibrationThread)
		{
			disconnect(HIDCapThread, SIGNAL(receiveThreadStarted(QString)), this, SIGNAL(CalibrateThreadStarted(QString)));
			disconnect(HIDCapThread, SIGNAL(receiveThreadStopped(QString)), this, SIGNAL(CalibrateThreadStopped(QString)));
		}
		else
		{
			disconnect(HIDCapThread, SIGNAL(receiveThreadStarted(QString)), this, SIGNAL(CaptureThreadStarted(QString)));
			disconnect(HIDCapThread, SIGNAL(receiveThreadStopped(QString)), this, SIGNAL(CaptureThreadStopped(QString)));
		}
		disconnect(HIDCapThread, SIGNAL(receiveThreadButtonTriggered(unsigned char,unsigned char)), this, SIGNAL(CaptureThreadButtonTriggered(unsigned char,unsigned char)));
		delete HIDCapThread;
		HIDCapThread = NULL;
	}
}


//// Register for device connect notification
//DEV_BROADCAST_DEVICEINTERFACE devInt;
//ZeroMemory( &devInt, sizeof(devInt) );
//devInt.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
//devInt.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
//devInt.dbcc_classguid = GUID_DEVINTERFACE_VOLUME;
//
//m_hDeviceNotify =
//RegisterDeviceNotification( winId(), &devInt, DEVICE_NOTIFY_WINDOW_HANDLE );   
//if(m_hDeviceNotify == NULL)
//{
//	qDebug() << "Failed to register device notification";
//} // end if
