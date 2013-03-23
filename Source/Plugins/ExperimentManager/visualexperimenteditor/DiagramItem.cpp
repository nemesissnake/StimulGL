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

#include "DiagramItem.h"
#include "DiagramArrow.h"

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <QPainter>

DiagramItem::DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent) : QGraphicsPolygonItem(parent)
{
    myDiagramType = diagramType;
    myContextMenu = contextMenu;
    QPainterPath path;
    switch (myDiagramType) {
        case StartEnd:
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            myPolygon = path.toFillPolygon();
            break;
        case Conditional:
            myPolygon << QPointF(-100, 0) << QPointF(0, 100)
                      << QPointF(100, 0) << QPointF(0, -100)
                      << QPointF(-100, 0);
            break;
        case Step:
            myPolygon << QPointF(-100, -100) << QPointF(100, -100)
                      << QPointF(100, 100) << QPointF(-100, 100)
                      << QPointF(-100, -100);
            break;
        default:
            myPolygon << QPointF(-120, -80) << QPointF(-70, 80)
                      << QPointF(120, 80) << QPointF(70, -80)
                      << QPointF(-120, -80);
            break;
    }
    setPolygon(myPolygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

void DiagramItem::removeArrow(DiagramArrow *arrow)
{
    int index = arrows.indexOf(arrow);

    if (index != -1)
        arrows.removeAt(index);
}

void DiagramItem::removeArrows()
{
    foreach (DiagramArrow *arrow, arrows) {
        arrow->startItem()->removeArrow(arrow);
        arrow->endItem()->removeArrow(arrow);
        scene()->removeItem(arrow);
        delete arrow;
    }
}

void DiagramItem::addArrow(DiagramArrow *arrow)
{
    arrows.append(arrow);
}

QPixmap DiagramItem::image() const
{
    QPixmap pixmap(250, 250);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setPen(QPen(Qt::black, 8));
    painter.translate(125, 125);
    painter.drawPolyline(myPolygon);

    return pixmap;
}

void DiagramItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    scene()->clearSelection();
    setSelected(true);
    myContextMenu->exec(event->screenPos());
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (DiagramArrow *arrow, arrows) {
            arrow->updatePosition();
        }
    }

    return value;
}
