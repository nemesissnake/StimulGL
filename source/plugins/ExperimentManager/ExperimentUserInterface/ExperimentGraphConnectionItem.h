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

#ifndef EXPERIMENTGRAPHCONNECTIONITEM_H
#define EXPERIMENTGRAPHCONNECTIONITEM_H

#include <QGraphicsRectItem>
#include <QPainter>

#define EXPGRAPHCONNITEM_ARROW_MIN_SIZE		20.0

class ExperimentGraphConnectionItem : public QGraphicsItem
{
public:
	ExperimentGraphConnectionItem(QGraphicsItem *parent = NULL);

	QRectF boundingRect() const;
	QPainterPath shape() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void setEndPoint(const QPointF &pEnd, const float &fPerpLenght = 0.0);

protected:
	void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		
private:
	QColor pSelectedColor;
	QColor pUnselectedColor;
	QPen pPen;
	QBrush pBrush;
	QRectF rBoundingBox;
	QPainterPath pShape;
	QPointF pEndPoint;
	QLineF pArrowLine;
	QPolygonF polyArrow;
	float fLength;
	float fAngle;
	float fPerpendicularLenght;

	bool bIsCurrentlyHovered;
};

#endif // EXPERIMENTGRAPHCONNECTIONITEM_H
