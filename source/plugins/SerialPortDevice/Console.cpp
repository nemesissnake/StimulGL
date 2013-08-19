#include "Console.h"
#include <QScrollBar>
#include <QtCore/QDebug>

Console::Console(QWidget *parent) : QPlainTextEdit(parent), localEchoEnabled(false)
{
    document()->setMaximumBlockCount(100);
    QPalette p = palette();
    p.setColor(QPalette::Base, Qt::black);
    p.setColor(QPalette::Text, Qt::green);
    setPalette(p);
}

void Console::putData(const QString &sData)//const QByteArray &data)
{
    insertPlainText("< " + sData + "\n");//QString(data));
    QScrollBar *bar = verticalScrollBar();
    bar->setValue(bar->maximum());
}

void Console::setLocalEchoEnabled(bool set)
{
    localEchoEnabled = set;
}

void Console::keyPressEvent(QKeyEvent *e)
{
    //switch (e->key()) 
	//{
	//case Qt::Key_Delete:
	//		QPlainTextEdit::keyPressEvent(e);
	//		break;
	//	case Qt::Key_Backspace:
	//		QPlainTextEdit::keyPressEvent(e);
	//		break;
	//	case Qt::Key_Left:
	//		QPlainTextEdit::keyPressEvent(e);
	//		break;
	//	case Qt::Key_Right:
	//		QPlainTextEdit::keyPressEvent(e);
	//		break;		
	//	case Qt::Key_Up:
	//		break;
	//	case Qt::Key_Down:
	//		break;
	//	case Qt::Key_Return:
	//		sTextToSend = this->toPlainText();
	//		QPlainTextEdit::keyPressEvent(e);
	//		break;
	//	default:
	//		if (localEchoEnabled)
	//			QPlainTextEdit::keyPressEvent(e);
	//		emit getData(e->text());//.toLocal8Bit());
    //}
}

bool Console::parseTextString(const QString &sTextToSend)
{
	if (localEchoEnabled)
		this->insertPlainText("> " + sTextToSend + "\n");
	emit getData(sTextToSend);
	return true;
}

void Console::mousePressEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
    setFocus();
}

void Console::mouseDoubleClickEvent(QMouseEvent *e)
{
    Q_UNUSED(e)
}

void Console::contextMenuEvent(QContextMenuEvent *e)
{
    Q_UNUSED(e)
}
