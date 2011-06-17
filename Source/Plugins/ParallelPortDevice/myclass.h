#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QString>
#include <QScriptable>

class MyClass : public QObject, protected QScriptable
{
	Q_OBJECT
	Q_PROPERTY(QString text WRITE setText READ text)

public:
	MyClass(QObject *parent = 0);
	~MyClass();

public slots:
		QString text() const;
		void setText(const QString& text);
		void append(const QString& text);

private:
	QString m_text;
};

#endif // MYCLASS_H
