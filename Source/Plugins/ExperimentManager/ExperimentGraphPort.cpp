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

#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPen>
#include "ExperimentGraphPort.h"
#include "ExperimentGraphConnection.h"

ExperimentGraphPort::ExperimentGraphPort(QGraphicsItem *parent, QGraphicsScene *scene):
	QGraphicsPathItem(parent, scene)
{
	label = new QGraphicsTextItem(this);

	radius_ = 5;
	margin = 2;

	QPainterPath p;
	p.addEllipse(-radius_, -radius_, 2*radius_, 2*radius_);
	setPath(p);

	setPen(QPen(Qt::darkRed));
	setBrush(Qt::red);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;
}

ExperimentGraphPort::~ExperimentGraphPort()
{
	foreach(ExperimentGraphConnection *conn, m_connections)
		delete conn;
}

void ExperimentGraphPort::setNEBlock(ExperimentGraphBlock *b)
{
	m_block = b;
}

void ExperimentGraphPort::setName(const QString &n)
{
	name = n;
	label->setPlainText(n);
}

void ExperimentGraphPort::setIsOutput(bool o)
{
	isOutput_ = o;

	QFontMetrics fm(scene()->font());
	QRect r = fm.boundingRect(name);

	if (isOutput_)
		label->setPos(-radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
	else
		label->setPos(radius_ + margin, -label->boundingRect().height()/2);
}

int ExperimentGraphPort::radius()
{
	return radius_;
}

bool ExperimentGraphPort::isOutput()
{
	return isOutput_;
}

QVector<ExperimentGraphConnection*>& ExperimentGraphPort::connections()
{
	return m_connections;
}

void ExperimentGraphPort::setPortFlags(int f)
{
	m_portFlags = f;

	if (m_portFlags & TypePort)
	{
		QFont font(scene()->font());
		font.setItalic(true);
		label->setFont(font);
		setPath(QPainterPath());
	} else if (m_portFlags & NamePort)
	{
		QFont font(scene()->font());
		font.setBold(true);
		label->setFont(font);
		setPath(QPainterPath());
	}
}

ExperimentGraphBlock* ExperimentGraphPort::block() const
{
	return m_block;
}

quint64 ExperimentGraphPort::ptr()
{
	return m_ptr;
}

void ExperimentGraphPort::setPtr(quint64 p)
{
	m_ptr = p;
}

bool ExperimentGraphPort::isConnected(ExperimentGraphPort *other)
{
	foreach(ExperimentGraphConnection *conn, m_connections)//For each connection from this port
	{
		if (conn->port1() == other || conn->port2() == other)
			return true;
	}
	return false;
}

QVariant ExperimentGraphPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		foreach(ExperimentGraphConnection *conn, m_connections)
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}
	return value;
}
