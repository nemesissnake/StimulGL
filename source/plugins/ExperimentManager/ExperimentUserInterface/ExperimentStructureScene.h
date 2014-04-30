//ExperimentManagerplugin
//Copyright (C) 2014  Sven Gijsen
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

#ifndef EXPERIMENTSTRUCTURESCENE_H
#define EXPERIMENTSTRUCTURESCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>

enum ExperimentStructureItemType 
{ 
	TypeBlockItem			= QGraphicsItem::UserType + 1,
	TypeAutoConnectionItem	= QGraphicsItem::UserType + 2,
	TypeLoopConnectionItem	= QGraphicsItem::UserType + 3 
};

class ExperimentStructureScene : public QGraphicsScene
{
	protected:
		bool event(QEvent *event);
		virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *contextMenuEvent);

	public:
		ExperimentStructureScene(QObject *parent = NULL);	
};

#endif // EXPERIMENTSTRUCTURESCENE_H
