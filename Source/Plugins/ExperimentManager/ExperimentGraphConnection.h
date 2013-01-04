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

#ifndef EXPERIMENTGRAPHCONNECTION_H
#define EXPERIMENTGRAPHCONNECTION_H

#include <QGraphicsPathItem>

class ExperimentGraphPort;

class ExperimentGraphConnection : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 2 };

	ExperimentGraphConnection(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~ExperimentGraphConnection();

	void setPos1(const QPointF &p);
	void setPos2(const QPointF &p);
	void setPort1(ExperimentGraphPort *p);
	void setPort2(ExperimentGraphPort *p);
	void updatePosFromPorts();
	void updatePath();
	ExperimentGraphPort* port1() const;
	ExperimentGraphPort* port2() const;

	void save(QDataStream&);
	void load(QDataStream&, const QMap<quint64, ExperimentGraphPort*> &portMap);

	int type() const { return Type; }

private:
	QPointF pos1;
	QPointF pos2;
	ExperimentGraphPort *m_port1;
	ExperimentGraphPort *m_port2;
};

#endif // EXPERIMENTGRAPHCONNECTION_H
