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
#include <QStyle>
#include <QStyleOptionGraphicsItem>

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
	//fAngle = pArrowLine.angle()+90.0;
	//fLength = pArrowLine.length();
	//fPerpendicularLenght = 0.0;
	// Describe a closed arrow pointing downwards; lenght 1.0
	polyArrow.append(QPointF( 0.0, 0.0));
	polyArrow.append(QPointF( 0.0, 1.0));
	polyArrow.append(QPointF(-0.1, 0.8));
	polyArrow.append(QPointF( 0.1, 0.8));
	polyArrow.append(QPointF( 0.0, 1.0));
	rBoundingBox = polyArrow.boundingRect();
	pShape.addPolygon(polyArrow);
		
	//this->setToolTip("ExperimentGraphConnectionItem");
	setAcceptHoverEvents(true);

	setFlag(QGraphicsItem::ItemIsSelectable);
	setFlag(QGraphicsItem::ItemIsMovable);
}

void ExperimentGraphConnectionItem::setEndPoint(const QPointF &pEnd, const float &fPerpLenght)
{
	pEndPoint = pEnd;
	//pArrowLine.setP1(QPointF(0.0,0.0));
	pArrowLine.setP2(pEndPoint);
	//fAngle = pArrowLine.angle()+90.0;
	//fLength = pArrowLine.length();
	//fPerpendicularLenght = fPerpLenght;
	rBoundingBox = polyArrow.boundingRect();
	update(rBoundingBox);
}

QRectF ExperimentGraphConnectionItem::boundingRect() const
{
	return rBoundingBox;
}

QPainterPath ExperimentGraphConnectionItem::shape() const
{
	QPainterPath pLocalShape;
	////pLocalShape.addPolygon(polyTriangleHeadAdjusted);
	pLocalShape.addRect(rBoundingBox);
	return pLocalShape;
//	return pShape;
}

void ExperimentGraphConnectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{	
	painter->setPen(pPen);
	painter->setBrush(pBrush);
	//painter->save();
	//painter->rotate(pArrowLine.angle()+90.0);
	QMatrix matrix;
	matrix.rotate(pArrowLine.angle()+90.0);
	//matrix.scale(pArrowLine.length(),pArrowLine.length());	
	polyArrow.clear();
	pShape = QPainterPath();
	pShape.setFillRule(Qt::WindingFill);
	float fArrLenght = qMin(pArrowLine.length() * 0.4, EXPGRAPHCONNITEM_ARROW_MIN_SIZE);
	float fHalfArrLenght = fArrLenght / 2;
	/*
	if(fPerpendicularLenght != 0.0)
	{	
		polyArrow.append(QPointF(fPerpendicularLenght, 0.0));
		//pShape.addRect(0.0, fArrLenght/-2, fPerpendicularLenght, fArrLenght);
	}
	*/

	polyArrow.append(QPointF( fHalfArrLenght, 0.0));
	polyArrow.append(QPointF( fHalfArrLenght, pArrowLine.length()));
	polyArrow.append(QPointF( 0.0, pArrowLine.length() - fArrLenght));
	polyArrow.append(QPointF( fArrLenght, pArrowLine.length() - fArrLenght));
	polyArrow.append(QPointF( fHalfArrLenght, pArrowLine.length()));
	pShape.addRect(0.0,0.0,fArrLenght,pArrowLine.length());
	
	/*
	if(fPerpendicularLenght != 0.0)
	{
		polyArrow.append(QPointF( fPerpendicularLenght, pArrowLine.length()));
		polyArrow.append(QPointF( 0.0, pArrowLine.length()));
		polyArrow.append(QPointF( 0.0, 0.0));
		//pShape.addRect(0.0, pArrowLine.length() + fArrLenght/-2, fPerpendicularLenght, fArrLenght);
	}
	*/
	polyTriangleHeadAdjusted = matrix.map(polyArrow);
	
	//rBoundingBox = polyTriangleHeadAdjusted.boundingRect();	//polyTriangleHeadAdjusted	
	//rBoundingBox.setTop(rBoundingBox.top()-fArrLenght/2);
	//rBoundingBox.setBottom(rBoundingBox.bottom()+fArrLenght/2);
	//rBoundingBox.setCoords(fArrLenght/-2,0.0,fArrLenght/2,pArrowLine.length());
	rBoundingBox = polyTriangleHeadAdjusted.boundingRect();
	rBoundingBox = rBoundingBox.normalized();
	rBoundingBox.moveTopLeft(0.0);
	rBoundingBox.setY(0.0);

	//here....
	//pShape = QPainterPath();
	//pShape.addRect(rBoundingBox);//polyTriangleHeadAdjusted);

	painter->drawPolygon(polyTriangleHeadAdjusted);
	
	//pShape.addPolygon(polyTriangleHeadAdjusted);
	//painter->setBrush(Qt::NoBrush); painter->setPen(QColor(255,0,0)); 
	pShape = matrix.map(pShape);
	//if(bIsCurrentlyHovered)
	if (option->state & QStyle::State_MouseOver )
	{
		painter->setBrush(Qt::NoBrush); painter->setPen(QColor(255,0,0)); painter->drawRect(rBoundingBox); 
		//painter->setPen(Qt::DotLine); painter->setPen(QColor(0,0,255)); painter->drawPath(pShape);
	}

	//painter->drawPath(pShape);
	//painter->rotate(-1*(pArrowLine.angle()+90.0));
}

void ExperimentGraphConnectionItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	bIsCurrentlyHovered = true;
	pBrush.setColor(pSelectedColor);
	pPen.setColor(pSelectedColor);
	update(rBoundingBox);
	QGraphicsItem::hoverEnterEvent(event);
}

//void ExperimentGraphConnectionItem::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )

void ExperimentGraphConnectionItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	bIsCurrentlyHovered = false;
	pBrush.setColor(pUnselectedColor);
	pPen.setColor(pUnselectedColor);
	update(rBoundingBox);
	QGraphicsItem::hoverLeaveEvent(event);
}