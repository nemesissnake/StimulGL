#ifndef HIDCAPTURETHREAD_H
#define HIDCAPTURETHREAD_H

#define PACKET_SIZE 4

#include <QThread>
#include <QDateTime>
#include <QFile>
#include <QMutex>
#include <QList>
#include "Global.h"

class HIDCaptureThread : public QThread
{
	Q_OBJECT

protected:
	void run();

public:

	enum DetectionMethod
	{
		MaskedValueChanged				= 0,
		MaskedValueChangedHigh			= 1,
		MaskedValueChangedLow			= 2
	};
	Q_DECLARE_FLAGS(DetectionMethods, DetectionMethod)

	struct strcJoystickPosition {
		unsigned char Xpos;
		unsigned char Ypos;
	};  

	HIDCaptureThread(unsigned short productID,unsigned short vendorID,QObject *parent);
	~HIDCaptureThread();

public slots:
	void WriteCapturedDataToFile(bool bWriteToFile = true, QString qsFileName = "", bool bWriteHeaderInfo = true, bool bWriteFilteredData = true);
	void ConfigureHIDTriggers(bool bActivateJoystickTrigger, bool bActivateButtonTriggers, unsigned char cButtonMask, DetectionMethod ButtonDetectDetectionMethod);
	void ConfigureHIDFiltering(bool bActivateJoystickStabilisation, int nJoystickStabilisationThreshold, bool bActivateJoystickHistory, int nJoystickHistorySize);
	void stop();

private:
	bool OutputCapturedData();
	void CloseOutputFile();
	void EmitFilteredData();

	bool m_bIsRunning;
	bool m_bAbortRunning;
	bool m_bWriteToFile;
	bool m_bWriteHeaderInfo;
	bool m_bWriteFilteredData;
	bool m_bActivateJoystickTrigger;
	bool m_bActivateButtonTriggers;
	QString m_qsFileName;
	unsigned short m_ProductID;
	unsigned short m_VendorID;
	unsigned char m_HIDData[PACKET_SIZE];
	int m_HIDFilteredData[PACKET_SIZE];
	unsigned char m_cButtonMask;
	DetectionMethod m_ButtonDetectDetectionMethod;
	QFile *m_pFile;
	QMutex FileMutex;
	QList<strcJoystickPosition> m_JostickHistory;//x,y
	unsigned char cOldButtonByteVal;
	unsigned char cNewButtonByteVal;
	bool m_bActivateJoystickStabilisation;
	int m_nJoystickStabilisationThreshold;
	bool m_bActivateJoystickHistory;
	int m_nJoystickHistorySize;

signals:
	//void receiveThreadTriggered(QString sData);
	void receiveThreadJoystickChanged(int XValue, int YValue);
	void receiveThreadButtonTriggered(unsigned char ButtonByteValue,unsigned char ChangedByteValue);
	void receiveThreadStarted(QString timeTextStamp);
	void receiveThreadStopped(QString timeTextStamp);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(HIDCaptureThread::DetectionMethods)

#endif // HIDCAPTURETHREAD_H
