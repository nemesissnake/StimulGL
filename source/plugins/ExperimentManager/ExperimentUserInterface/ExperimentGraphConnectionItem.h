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

#define EXPGRAPHCONNITEM_ARROW_MIN_SIZE		40.0

enum ExperimentGraphConnItemDrawOrder
{
	GRAPHCONN_DRAW_ORDER_UNNESTED_BOTHSIDES		= 0,
	GRAPHCONN_DRAW_ORDER_NUMBER_MASTERSIDE		= 1,
	GRAPHCONN_DRAW_ORDER_NUMBER_SLAVESIDE		= 2
};

enum ExperimentGraphConnectionTypeEnum
{
	GRAPHBLOCK_CONN_INPUT			= 0,
	GRAPHBLOCK_CONN_OUTPUT			= 1
};

class ExperimentGraphConnectionItem : public QGraphicsItem
{
public:
	ExperimentGraphConnectionItem(QGraphicsItem *parent = NULL);

	void setEndPoint(const QPointF &pEnd, const float &fPerpLenght = 0.0);

protected:

	virtual QRectF boundingRect() const;
	virtual QPainterPath shape() const;
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
		
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
	QPolygonF polyTriangleHeadAdjusted;
	//float fLength;
	//float fAngle;
	//float fPerpendicularLenght;

	bool bIsCurrentlyHovered;
};

#endif // EXPERIMENTGRAPHCONNECTIONITEM_H
