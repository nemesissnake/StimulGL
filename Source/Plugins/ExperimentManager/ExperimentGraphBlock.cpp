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

#include <QPen>
#include <QGraphicsScene>
#include <QFontMetrics>
#include <QPainter>
#include "ExperimentGraphBlock.h"
#include "ExperimentGraphPort.h"

ExperimentGraphBlock::ExperimentGraphBlock(QGraphicsItem *parent, QGraphicsScene *scene) : QGraphicsPathItem(parent, scene)
{
	QPainterPath p;
	p.addRoundedRect(-50, -15, 100, 30, 5, 5);
	setPath(p);
	setPen(QPen(Qt::darkGreen));
	setBrush(Qt::green);
	setFlag(QGraphicsItem::ItemIsMovable);
	setFlag(QGraphicsItem::ItemIsSelectable);
	horzMargin = 20;
	vertMargin = 5;
	width = horzMargin;
	height = vertMargin;
	sName = "";
	nID = -1;
}

ExperimentGraphPort* ExperimentGraphBlock::addPort(const QString &name, bool isOutput, int flags, int ptr)
{
	ExperimentGraphPort *port = new ExperimentGraphPort(this);
	port->setName(name);
	port->setIsOutput(isOutput);
	port->setNEBlock(this);
	port->setPortFlags(flags);
	port->setPtr(ptr);

	QFontMetrics fm(scene()->font());
	int w = fm.width(name);
	int h = fm.height();
	// port->setPos(0, height + h/2);
	if (w > width - horzMargin)
		width = w + horzMargin;
	height += h;

	QPainterPath p;
	p.addRoundedRect(-width/2, -height/2, width, height, 5, 5);
	setPath(p);

	int y = -height / 2 + vertMargin + port->radius();
	foreach(QGraphicsItem *port_, children()) {
		if (port_->type() != ExperimentGraphPort::Type)
			continue;

		ExperimentGraphPort *port = (ExperimentGraphPort*) port_;
		if (port->isOutput())
			port->setPos(width/2 + port->radius(), y);
		else
			port->setPos(-width/2 - port->radius(), y);
		y += h;
	}

	return port;
}

void ExperimentGraphBlock::addInputPort(const QString &name)
{
	addPort(name, false);
}

void ExperimentGraphBlock::addOutputPort(const QString &name)
{
	addPort(name, true);
}

void ExperimentGraphBlock::addInputPorts(const QStringList &names)
{
	foreach(QString n, names)
		addInputPort(n);
}

void ExperimentGraphBlock::addOutputPorts(const QStringList &names)
{
	foreach(QString n, names)
		addOutputPort(n);
}

void ExperimentGraphBlock::save(QDataStream &ds)
{
	ds << pos();

	int count(0);

	foreach(QGraphicsItem *port_, children())
	{
		if (port_->type() != ExperimentGraphPort::Type)
			continue;

		count++;
	}

	ds << count;

	foreach(QGraphicsItem *port_, children())
	{
		if (port_->type() != ExperimentGraphPort::Type)
			continue;

		ExperimentGraphPort *port = (ExperimentGraphPort*) port_;
		ds << (quint64) port;
		ds << port->portName();
		ds << port->isOutput();
		ds << port->portFlags();
	}
}

void ExperimentGraphBlock::load(QDataStream &ds, QMap<quint64, ExperimentGraphPort*> &portMap)
{
	QPointF p;
	ds >> p;
	setPos(p);
	int count;
	ds >> count;
	for (int i = 0; i < count; i++)
	{
		QString name;
		bool output;
		int flags;
		quint64 ptr;

		ds >> ptr;
		ds >> name;
		ds >> output;
		ds >> flags;
		portMap[ptr] = addPort(name, output, flags, ptr);
	}
}

#include <QStyleOptionGraphicsItem>

void ExperimentGraphBlock::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	Q_UNUSED(option)
	Q_UNUSED(widget)

	QColor cBack = QColor(255,255,0,10);
	if (isSelected()) {
		painter->setPen(QPen(cBack));//(Qt::darkYellow));
		painter->setBrush(Qt::yellow);
	} else {
		painter->setPen(QPen(Qt::darkGreen));
		painter->setBrush(Qt::green);
	}

	painter->drawPath(path());
}

ExperimentGraphBlock* ExperimentGraphBlock::clone()
{
	ExperimentGraphBlock *b = new ExperimentGraphBlock(0, scene());

	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == ExperimentGraphPort::Type)
		{
			ExperimentGraphPort *port = (ExperimentGraphPort*) port_;
			b->addPort(port->portName(), port->isOutput(), port->portFlags(), port->ptr());
		}
	}

	return b;
}

QVector<ExperimentGraphPort*> ExperimentGraphBlock::ports()
{
	QVector<ExperimentGraphPort*> res;
	foreach(QGraphicsItem *port_, childItems())
	{
		if (port_->type() == ExperimentGraphPort::Type)
			res.append((ExperimentGraphPort*) port_);
	}
	return res;
}

QVariant ExperimentGraphBlock::itemChange(GraphicsItemChange change, const QVariant &value)
{
	return value;
}

