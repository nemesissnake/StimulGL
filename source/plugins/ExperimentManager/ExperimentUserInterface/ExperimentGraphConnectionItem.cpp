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

#include "ExperimentGraphConnectionItem.h"
#include <QtCore/qmath.h>

ExperimentGraphConnectionItem::ExperimentGraphConnectionItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
	bIsCurrentlyHovered = false;
	pPen.setWidth(5.0);
	pSelectedColor.setRgb(0,255,0);
	pUnselectedColor.setRgb(31,6,130);
	pBrush.setColor(pUnselectedColor);
	pBrush.setStyle(Qt::SolidPattern);
	pPen.setColor(pUnselectedColor);
	pEndPoint.setX(0.0);
	pEndPoint.setY(1.0);
	pArrowLine.setP1(QPointF(0.0,0.0));
	pArrowLine.setP2(pEndPoint);
	fAngle = pArrowLine.angle()+90.0;
	fLength = pArrowLine.length();
	fPerpendicularLenght = 0.0;
	// Describe a closed arrow pointing downwards; lenght 1.0
	polyArrow.append(QPointF( 0.0, 0.0));
	polyArrow.append(QPointF( 0.0, 1.0));
	polyArrow.append(QPointF(-0.1, 0.8));
	polyArrow.append(QPointF( 0.1, 0.8));
	polyArrow.append(QPointF( 0.0, 1.0));
	rBoundingBox = polyArrow.boundingRect();
	pShape.addPolygon(polyArrow);
		
	//this->setToolTip("ExperimentGraphConnectionItem");
	this->setAcceptHoverEvents(true);
}

void ExperimentGraphConnectionItem::setEndPoint(const QPointF &pEnd, const float &fPerpLenght)
{
	pEndPoint = pEnd;
	//pArrowLine.setP1(QPointF(0.0,0.0));
	pArrowLine.setP2(pEndPoint);
	fAngle = pArrowLine.angle()+90.0;
	fLength = pArrowLine.length();
	fPerpendicularLenght = fPerpLenght;
	update();
}

QRectF ExperimentGraphConnectionItem::boundingRect() const
{
	return rBoundingBox;
}

QPainterPath ExperimentGraphConnectionItem::shape() const
{
	return pShape;
}

void ExperimentGraphConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{	
	painter->setPen(pPen);
	painter->setBrush(pBrush);
	//painter->save();
	//painter->rotate(fAngle);
	QMatrix matrix;
	matrix.rotate(fAngle);
	//matrix.scale(fLength,fLength);	
	polyArrow.clear();
	pShape = QPainterPath();
	float fArrLenght = qMin(fLength * 0.4, EXPGRAPHCONNITEM_ARROW_MIN_SIZE);

	if(fPerpendicularLenght != 0.0)
	{	
		polyArrow.append(QPointF(fPerpendicularLenght, 0.0));
		pShape.addRect(0.0, 0.0, fPerpendicularLenght, 0.0);
	}
	polyArrow.append(QPointF( 0.0, 0.0));
	polyArrow.append(QPointF( 0.0, fLength));
	polyArrow.append(QPointF( fArrLenght/-2, fLength - fArrLenght));
	polyArrow.append(QPointF( fArrLenght/2, fLength - fArrLenght));
	polyArrow.append(QPointF( 0.0, fLength));
	pShape.addRect(fArrLenght/-2,0.0,fArrLenght,fLength);
	
	if(fPerpendicularLenght != 0.0)
	{
		polyArrow.append(QPointF( fPerpendicularLenght, fLength));
		polyArrow.append(QPointF( 0.0, fLength));
		polyArrow.append(QPointF( 0.0, 0.0));
		pShape.addRect(fArrLenght/-2,0.0,fArrLenght,fLength);
	}
	QPolygonF polyTriangleHeadAdjusted = matrix.map(polyArrow);
	//QPainterPath pPath;
	//pShape = QPainterPath();
	//pShape.addPolygon(polyTriangleHeadAdjusted);
	rBoundingBox = polyTriangleHeadAdjusted.boundingRect();	//polyTriangleHeadAdjusted	
	
	painter->drawPolygon(polyTriangleHeadAdjusted);
	
	//pShape.addPolygon(polyTriangleHeadAdjusted);
	//painter->setBrush(Qt::NoBrush); painter->setPen(QColor(255,0,0)); 
	pShape = matrix.map(pShape);
	if(bIsCurrentlyHovered)
	{
		painter->setBrush(Qt::NoBrush); painter->setPen(QColor(255,0,0)); painter->drawRect(rBoundingBox); 
		painter->setPen(Qt::DotLine); painter->setPen(QColor(0,0,255)); painter->drawPath(pShape);
	}

	//painter->drawPath(pShape);
	//painter->rotate(-fAngle);
}

void ExperimentGraphConnectionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	bIsCurrentlyHovered = true;
	pBrush.setColor(pSelectedColor);
	pPen.setColor(pSelectedColor);
	update();
}

//void ExperimentGraphConnectionItem::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )

void ExperimentGraphConnectionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	bIsCurrentlyHovered = false;
	pBrush.setColor(pUnselectedColor);
	pPen.setColor(pUnselectedColor);
	update();
}