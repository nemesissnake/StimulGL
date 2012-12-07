#ifndef MAINAPPEXCHANGE_H
#define MAINAPPEXCHANGE_H

#include <QApplication>
#include <QSharedMemory>
#include <QLocalServer>

class MainAppExchange : public QApplication
{	
	Q_OBJECT

signals:
	void messageAvailable(QString message);

public:
	MainAppExchange(int &argc, char *argv[], const QString uniqueKey);

	bool isRunning();
	bool sendMessage(const QString &message);

public slots:
	void receiveMessage();

private:
	bool _isRunning;
	QString _uniqueKey;
	QSharedMemory sharedMemory;
	QLocalServer *localServer;
	static const int timeout = 1000;
};

#endif // MAINAPPEXCHANGE_H
