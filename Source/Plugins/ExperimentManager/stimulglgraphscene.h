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


#ifndef STIMULGLGRAPHSCENE_H
#define STIMULGLGRAPHSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QKeyEvent>

class StimulGLGraphScene : public QGraphicsScene
{
	Q_OBJECT

signals:
	void UserWantsToClose();

public:
	StimulGLGraphScene(QObject *parent);
	~StimulGLGraphScene();

	bool Init();

protected:
	void keyPressEvent(QKeyEvent * keyEvent);

private:
	QGraphicsRectItem *m_GraphRectItem;
	QGraphicsItem *m_GraphItem;	
};

#endif // STIMULGLGRAPHSCENE_H
