#ifndef QML2INTERFACE_H
#define QML2INTERFACE_H

#include <QObject>
#include <QQmlEngine>

// First, define your QObject which provides the functionality.
class Qml2Interface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(QString message READ message WRITE setmessage NOTIFY messageChanged)

signals:
	void messageChanged();

public:
	Qml2Interface(QObject *parent);
	~Qml2Interface();

	void setmessage(const QString &a); 
	QString message() const;

	Q_INVOKABLE QString doSomething();

private:
	QString m_message;
};

// Second, define the singleton type provider function (callback).
static QObject *interface_singletontype_provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)
	Qml2Interface *example = new Qml2Interface(NULL);
	return example;
}



#endif //QML2INTERFACE_H