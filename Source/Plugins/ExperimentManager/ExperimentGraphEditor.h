//ExperimentManagerplugin
//Copyright (C) 2013  Sven Gijsen
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

#ifndef EXPERIMENTGRAPHEDITOR_H
#define EXPERIMENTGRAPHEDITOR_H

#include <QObject>
#include "ExperimentStructures.h"

class QGraphicsScene;
class ExperimentGraphConnection;
class QGraphicsItem;
class QPointF;
class ExperimentGraphBlock;

class ExperimentGraphEditor : public QObject
{
	Q_OBJECT
public:
	explicit ExperimentGraphEditor(QObject *parent = 0);

	void install(QGraphicsScene *s, QGraphicsView *v);
	bool eventFilter(QObject *, QEvent *);
	void save(QDataStream &ds);
	void load(QDataStream &ds);
	bool parseExperimentStructure(cExperimentStructure *ExpStruct);

private:
	QGraphicsItem *itemAt(const QPointF &pos);
	bool createConnection(QGraphicsItem *from, QGraphicsItem *to);

private:
	QGraphicsView *gView;
	QGraphicsScene *gScene;
	ExperimentGraphConnection *conn;
	bool bAllowSelfRecurrentConnection;
	// ExperimentGraphBlock *selBlock;
};

#endif // EXPERIMENTGRAPHEDITOR_H
