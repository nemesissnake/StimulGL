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

#ifndef QGVEDGE_H
#define QGVEDGE_H

#include "QGVCore.h"
#include <QGraphicsItem>
#include <QPen>

class QGVNode;
class QGVScene;

/**
 * @brief Edge item
 *
 */
class QGVEdge : public QGraphicsItem
{
public:
    ~QGVEdge();

    QString label() const;
    QRectF boundingRect() const;
    QPainterPath shape() const;

    void setLabel(const QString &label);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

    void setAttribute(const QString &name, const QString &value);
    QString getAttribute(const QString &name) const;

    void updateLayout();

    enum { Type = UserType + 3 };
    int type() const
    {
        return Type;
    }

private:
    QGVEdge(Agedge_t *edge, QGVScene *scene);

    QPolygonF toArrow(const QLineF &normal) const;

    friend class QGVScene;
    //friend class QGVSubGraph;

    QGVScene *_scene;
    Agedge_t* _edge;

    QPainterPath _path;
    QPen _pen;
    QPolygonF _head_arrow;
    QPolygonF _tail_arrow;

    QString _label;
    QRectF _label_rect;
};

#endif // QGVEDGE_H
