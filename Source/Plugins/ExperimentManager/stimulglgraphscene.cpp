//ExperimentManagerplugin
//Copyright (C) 2012  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


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
