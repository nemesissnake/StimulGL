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

#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include "diagramitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE

class DiagramArrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    DiagramArrow(DiagramItem *startItem, DiagramItem *endItem,
      QGraphicsItem *parent = 0);

    int type() const { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color) { myColor = color; }
    DiagramItem *startItem() const { return myStartItem; }
    DiagramItem *endItem() const { return myEndItem; }

    void updatePosition();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);

private:
    DiagramItem *myStartItem;
    DiagramItem *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};

#endif // ARROW_H
