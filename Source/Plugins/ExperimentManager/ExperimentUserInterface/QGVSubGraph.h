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

#ifndef QGVSUBGRAPH_H
#define QGVSUBGRAPH_H

#include <QGraphicsItem>
#include <QPen>
#include "QGVCore.h"

class QGVNode;
class QGVEdge;
class QGVScene;

/**
 * @brief SubGraph item
 *
 */
class QGVSubGraph : public QGraphicsItem
{
public:
    ~QGVSubGraph();

    QString name() const;

    QGVNode* addNode(const QString& label);
    QGVSubGraph* addSubGraph(const QString& name, bool cluster=true);

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);
    void setAttribute(const QString &name, const QString &value);
    QString getAttribute(const QString &name) const;
    void updateLayout();

    enum { Type = UserType + 4 };
    int type() const
    {
        return Type;
    }


private:
    friend class QGVScene;
    QGVSubGraph(Agraph_t* subGraph, QGVScene *scene);

    double _height, _width;
    QPen _pen;
    QBrush _brush;

    QString _label;
    QRectF _label_rect;

    QGVScene *_scene;
    Agraph_t *_sgraph;
    QList<QGVNode*> _nodes;
};

#endif // QGVSUBGRAPH_H
