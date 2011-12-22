#ifndef PPGENERATETHREAD_H
#define PPGENERATETHREAD_H

#include <QThread>
#include <QDateTime>

class ParallelPort;
class ppGenerateThread : public QThread
{
	Q_OBJECT

protected:
	void run();

public:

	enum GenerationMethod
	{
		Value				= 0,
		Pulse    			= 1,
		Periodical			= 2
	};
	Q_DECLARE_FLAGS(GenerationMethods, GenerationMethod)

	ppGenerateThread(short baseAddress, GenerationMethod method, short outputMask, short activeValue, short inActiveValue = 0, int activePulseTime = 10, int repetitionTime = 2000, QObject *parent = 0);
	~ppGenerateThread();

	public slots:
		void stop();

private:
	bool isRunning;
	bool abortRunning;
	short nBaseAddress;
	GenerationMethod dMethod;
	short nOutputMask;
	short nActiveValue;
	short nInActiveValue;
	int nActivePulseTime;
	int nRepetitionTime;
	ParallelPort *portDev;

signals:
	void generateThreadTriggered(short Value);
	void generateThreadStarted(QString timeTextStamp);
	void generateThreadStopped(QString timeTextStamp);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ppGenerateThread::GenerationMethods)

#endif // PPGENERATETHREAD_H
