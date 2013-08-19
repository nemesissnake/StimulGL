#ifndef CONSOLE_H
#define CONSOLE_H

#include <QPlainTextEdit>

class Console : public QPlainTextEdit
{
    Q_OBJECT

signals:
    void getData(const QString &sData);//const QByteArray &data);

public:
    explicit Console(QWidget *parent = 0);

    void putData(const QString &sData);//const QByteArray &data);
    void setLocalEchoEnabled(bool set);

public slots:
	bool parseTextString(const QString &sTextToSend);

protected:
    virtual void keyPressEvent(QKeyEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);
    virtual void mouseDoubleClickEvent(QMouseEvent *e);
    virtual void contextMenuEvent(QContextMenuEvent *e);

private:
    bool localEchoEnabled;
	QString sTextToSend;

};

#endif // CONSOLE_H
