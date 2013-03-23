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

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPixmapItem>
#include <QList>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class DiagramArrow;

class DiagramItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType + 15 };
    enum DiagramType { Step, Conditional, StartEnd, Io };
    
    DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = 0);
    void removeArrow(DiagramArrow *arrow);
    void removeArrows();
    DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(DiagramArrow *arrow);
    QPixmap image() const;
    int type() const { return Type;}

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    DiagramType myDiagramType;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QList<DiagramArrow *> arrows;
};

#endif // DIAGRAMITEM_H
