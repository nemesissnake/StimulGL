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

#include <QBrush>
#include <QPen>
#include <QGraphicsScene>
#include "ExperimentGraphConnection.h"
#include "ExperimentGraphPort.h"

ExperimentGraphConnection::ExperimentGraphConnection(QGraphicsItem *parent) : QGraphicsPathItem(parent)//, scene)
{
	setPen(QPen(Qt::black, 2));
	setBrush(Qt::NoBrush);
	setZValue(-1);
	m_port1 = 0;
	m_port2 = 0;
}

ExperimentGraphConnection::~ExperimentGraphConnection()
{
	if (m_port1)
		m_port1->connections().remove(m_port1->connections().indexOf(this));
	if (m_port2)
		m_port2->connections().remove(m_port2->connections().indexOf(this));
}

void ExperimentGraphConnection::setPos1(const QPointF &p)
{
	pos1 = p;
}

void ExperimentGraphConnection::setPos2(const QPointF &p)
{
	pos2 = p;
}

void ExperimentGraphConnection::setPort1(ExperimentGraphPort *p)
{
	m_port1 = p;
	m_port1->connections().append(this);
}

void ExperimentGraphConnection::setPort2(ExperimentGraphPort *p)
{
	m_port2 = p;
	m_port2->connections().append(this);
}

void ExperimentGraphConnection::updatePosFromPorts()
{
	pos1 = m_port1->scenePos();
	pos2 = m_port2->scenePos();
}

void ExperimentGraphConnection::updatePath()
{
	QPainterPath p;
	//QPointF pos1(m_port1->scenePos());
	//QPointF pos2(m_port2->scenePos());
	QString name = m_port1->portName();
	
	//qreal dx = pos2.x() - pos1.x();
	//qreal dy = pos2.y() - pos1.y();
	//qreal bezX1 = 0.25;
	//qreal bezX2 = 1.0 - bezX1;
	//qreal bezY1 = 0.1;
	//qreal bezY2 = 1.0 - bezY1;
	//QPointF ctr1(pos1.x() + dx * bezX1, pos1.y() + dy * bezY1);
	//QPointF ctr2(pos1.x() + dx * bezX2, pos1.y() + dy * bezY2);

	double diffX = pos2.x() - pos1.x();
	//double diffY = pos2.y() - pos1.y();
	
	//double half_diffy = diffY/2;
	QPointF ctr1;
	QPointF ctr2;
	QPointF ctr3;
	QPointF ctr4;
	QPointF intermediatePos;

	if(diffX>0.0)//Right
	{
		ctr1 = QPointF(pos1.x() + qMin(diffX/2,100.0), pos1.y());
		ctr2 = QPointF(pos2.x() - qMin(diffX/2,100.0), pos2.y());
		p.moveTo(pos1);
		p.cubicTo(ctr1,ctr2,pos2);
		//p.quadTo(ctr1,intermediatePos);
		//p.quadTo(ctr2,pos2);
	}
	else//Left
	{
		//double minY = qMin(pos1.y(),pos2.y());
		ctr1 = pos1 + QPointF(40.0,0);
		ctr2 = ctr1 + QPointF(0,20.0);
		QPointF intermediatePos = QPointF(pos2.x() - (diffX / 2),ctr2.y());
		ctr3 = QPointF(pos2.x()-40.0,intermediatePos.y());
		ctr4 = pos2 - QPointF(40.0,0);
		//if(diffX<0.0)//Left
		//{
		//	ctr1 = ctr1 - QPointF(diffX/4,qMin(diffX/4,50.0));
		//	ctr2 = ctr2 + QPointF(diffX/4,qMin(diffX/4,50.0));
		//}
		p.moveTo(pos1);	
		//p.lineTo(ctr1);
		p.moveTo(ctr2);
		//p.lineTo(intermediatePos);

		//p.lineTo(ctr3);
		p.moveTo(pos2);
		//p.lineTo(ctr4);

		p.moveTo(pos1);
		p.cubicTo(ctr1,ctr2,intermediatePos);
		p.cubicTo(ctr3,ctr4,pos2);
		//p.quadTo(ctr1,intermediatePos);
		//p.quadTo(ctr2,pos2);
	}
	setPath(p);
}

ExperimentGraphPort* ExperimentGraphConnection::port1() const
{
	return m_port1;
}

ExperimentGraphPort* ExperimentGraphConnection::port2() const
{
	return m_port2;
}

void ExperimentGraphConnection::save(QDataStream &ds)
{
	ds << (quint64) m_port1;
	ds << (quint64) m_port2;
}

void ExperimentGraphConnection::load(QDataStream &ds, const QMap<quint64, ExperimentGraphPort*> &portMap)
{
	quint64 ptr1;
	quint64 ptr2;
	ds >> ptr1;
	ds >> ptr2;

	setPort1(portMap[ptr1]);
	setPort2(portMap[ptr2]);
	updatePosFromPorts();
	updatePath();
}
