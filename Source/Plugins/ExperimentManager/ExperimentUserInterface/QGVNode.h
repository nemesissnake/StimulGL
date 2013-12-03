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

#ifndef QGVNODE_H
#define QGVNODE_H

#include "QGVCore.h"
#include <QGraphicsItem>
#include <QPen>

class QGVEdge;
class QGVScene;

/**
 * @brief Node item
 *
 */
class QGVNode : public QGraphicsItem
{
public:
    ~QGVNode();

    QString label() const;
    void setLabel(const QString &label);

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &label, const QString &value);
    QString getAttribute(const QString &name) const;

    void setIcon(const QImage &icon);

    enum { Type = UserType + 2 };
    int type() const
    {
        return Type;
    }

private:
    friend class QGVScene;
    friend class QGVSubGraph;
    
    QGVNode(Agnode_t* node, QGVScene *scene);
	void updateLayout();
    QPainterPath makeShape(Agnode_t* node) const;
    QPolygonF makeShapeHelper(Agnode_t* node) const;

    QPainterPath _path;
    QPen _pen;
    QBrush _brush;
    QImage _icon;

    QGVScene *_scene;
    Agnode_t* _node;
};


#endif // QGVNODE_H
