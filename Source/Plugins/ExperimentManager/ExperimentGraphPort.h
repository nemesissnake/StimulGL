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

#ifndef EXPERIMENTGRAPHPORT_H
#define EXPERIMENTGRAPHPORT_H

#include <QGraphicsPathItem>

class ExperimentGraphBlock;
class ExperimentGraphConnection;

class ExperimentGraphPort : public QGraphicsPathItem
{
public:
	enum { Type = QGraphicsItem::UserType + 1 };
	enum { NamePort = 1, TypePort = 2 };

	ExperimentGraphPort(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	~ExperimentGraphPort();

	void setNEBlock(ExperimentGraphBlock*);
	void setName(const QString &n);
	void setIsOutput(bool o);
	int radius();
	bool isOutput();
	QVector<ExperimentGraphConnection*>& connections();
	void setPortFlags(int);

	const QString& portName() const { return name; }
	int portFlags() const { return m_portFlags; }

	int type() const { return Type; }

	ExperimentGraphBlock* block() const;

	quint64 ptr();
	void setPtr(quint64);

	bool isConnected(ExperimentGraphPort*);

protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
	ExperimentGraphBlock *m_block;
	QString name;
	bool isOutput_;
	QGraphicsTextItem *label;
	int radius_;
	int margin;
	QVector<ExperimentGraphConnection*> m_connections;
	int m_portFlags;
	quint64 m_ptr;
};

#endif // EXPERIMENTGRAPHPORT_H
