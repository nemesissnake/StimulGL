#include "stimulglgraphscene.h"

StimulGLGraphScene::StimulGLGraphScene(QObject *parent)	: QGraphicsScene(parent)
{

}

StimulGLGraphScene::~StimulGLGraphScene()
{

}

bool StimulGLGraphScene::Init()
{
	m_GraphRectItem = this->addRect(QRectF(0, 0, 100, 100));
	const QPixmap pixmap("qt-logo.png");
	//this->addPixmap(pixmap);
	//pixmap
	QString qstrTemp("Example text");
	QFont qfFont("Times", 30, 5, true);//type, size, weight, italic?
	this->addText(qstrTemp,qfFont);
	m_GraphItem = this->itemAt(50, 50);
	// item == rect

	//QRegion exposed;
	//pixmap.scroll(10, 10, pixmap.rect(), &exposed);
	//QPixmap pixmap;
	//QByteArray bytes;
	//QBuffer buffer(&bytes);
	//buffer.open(QIODevice::WriteOnly);
	//pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format

	return true;
}

void StimulGLGraphScene::keyPressEvent(QKeyEvent * keyEvent)
{
	switch (keyEvent->key()) 
	{
		case Qt::Key_Escape:
			if((keyEvent->modifiers() & Qt::ControlModifier) && (keyEvent->modifiers() & Qt::AltModifier))
			{
				emit UserWantsToClose();
			}
			break;
	}
	// Otherwise pass to the graphics view
//	QGraphicsScene::keyPressEvent(keyEvent);
}
