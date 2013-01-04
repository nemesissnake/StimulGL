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

#ifndef EXPERIMENTGRAPHBLOCK_H
#define EXPERIMENTGRAPHBLOCK_H

#include <QGraphicsPathItem>

class ExperimentGraphPort;

class ExperimentGraphBlock : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 3 };

	ExperimentGraphBlock(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

	ExperimentGraphPort* addPort(const QString &name, bool isOutput, int flags = 0, int ptr = 0);
	void addInputPort(const QString &name);
	void addOutputPort(const QString &name);
	void addInputPorts(const QStringList &names);
	void addOutputPorts(const QStringList &names);
	void save(QDataStream&);
	void load(QDataStream&, QMap<quint64, ExperimentGraphPort*> &portMap);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	ExperimentGraphBlock* clone();
	QVector<ExperimentGraphPort*> ports();

	int type() const { return Type; }

	void setName(const QString &sValue) {sName = sValue;};
	QString getName() const {return sName;};
	void setID(const int &nValue) {nID = nValue;};
	int getID() const {return nID;};

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	int horzMargin;
	int vertMargin;
	int width;
	int height;

	QString sName;
	int nID;
};

#endif // EXPERIMENTGRAPHBLOCK_H
