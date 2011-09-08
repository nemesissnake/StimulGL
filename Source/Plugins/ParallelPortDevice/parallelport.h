#ifndef PARALLELPORT_H
#define PARALLELPORT_H
#include <QObject>
#include <QString>
#include <QScriptable>
#include "InpOut32.h"
#include "ppCaptureThread.h"
#include "ppGenerateThread.h"

class ParallelPort : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_PROPERTY( short BaseAddress WRITE setBaseAddress READ getBaseAddress )

signals:
	void GenerateThreadTriggered(short);
	void GenerateThreadStarted(QString);
	void GenerateThreadStopped(QString);
	void CaptureThreadTriggered(short);
	void CaptureThreadStarted(QString);
	void CaptureThreadStopped(QString);

public:
	ParallelPort(short BaseAddress = 888, QObject *parent = 0);
	ParallelPort(const ParallelPort& other ){}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro
	~ParallelPort();

public slots:
	void setBaseAddress( short BaseAddress );
	short getBaseAddress() const;

	short PortRead();
	void PortWrite(const short &Value);
	bool IsPortEpp();
	bool IsPortEcp();
	//ECP_Mode GetEcpMode();
	//PortType GetPortType();
	bool IsPortSpp();
	bool IsPortPS2();
	bool IsPortInput();
	bool IsPortOutput();
	bool ConfigurePortForInput();
	void ConfigurePortForOutput();
	QString GetPortDescription();

	bool BitRead(const short nBitNumber);
	void BitWrite(const short nBitNumber, bool Value);
	short BitReset(const short nBitNumber);
	short BitSet(const short nBitNumber);
	short BitToggle(const short nBitNumber);
	short StatusPortRead();
	short DataPortRead();
	void DataPortWrite(const short Value);
	short ControlPortRead();
	void ControlPortWrite(const short Value);
	int GetEppTimeoutBit();

	bool StartGenerateThread(const short baseAddress,const short method, const short outputMask, const short activeValue, const short inActiveValue, const int activePulseTime, const int repetitionTime);
	void StopGenerateThread();

	bool StartCaptureThread(const short baseAddress, const short mask, const short method, const int postLHDelay = 0, const int postHLDelay = 0);
	void StopCaptureThread();

	//bool SetEcpMode(ECP_Mode nEcpModeValue);

private:
	InpOut32 InpOut32Object;
	short nBaseAddress;

	ppCaptureThread *captureThread;
	ppGenerateThread *generateThread;
};

#endif // PARALLELPORT_H
