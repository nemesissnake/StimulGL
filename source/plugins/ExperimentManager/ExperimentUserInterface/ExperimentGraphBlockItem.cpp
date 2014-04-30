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

#include "ExperimentGraphBlockItem.h"
#include "ExperimentStructureScene.h"

ExperimentGraphBlockItem::ExperimentGraphBlockItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
	bType = GRAPHBLOCK_TYPE_BLOCK;
	pPen.setWidth(2.0);
	cCaption.setRgb(63,72,204);
	cUnselectedFill.setRgb(195,195,195);
	cSelectedFill.setRgb(239,228,176);
	pPen.setColor(cCaption);
	pBrush.setColor(cUnselectedFill);
	pBrush.setStyle(Qt::SolidPattern);

	nBlockWidth = EXPGRAPHBLOCKITEM_BLOCK_WIDTH;
	nHalfBlockWidth = nBlockWidth/2;
	nBlockHeight = EXPGRAPHBLOCKITEM_BLOCK_HEIGHT;
	nHalfBlockHeight = nBlockHeight/2;
	nBlockDistance = EXPGRAPHBLOCKITEM_BLOCK_DISTANCE;
	nHalfBlockDistance = nBlockDistance/2;
	rBoundingBox.setCoords(-nHalfBlockWidth,-nHalfBlockHeight-nHalfBlockDistance,nHalfBlockWidth,nHalfBlockHeight+nHalfBlockDistance); 

	bIsCurrentlyHovered = false;
	//this->setToolTip("ExperimentGraphBlockItem");
	setAcceptHoverEvents(true);
}

void ExperimentGraphBlockItem::setType(const ExperimentGraphBlockItemTypeEnum &eBlockType)
{
	bType = eBlockType;
	update();
}

void ExperimentGraphBlockItem::setCaption(const QString &sName)
{
	sCurrentText = sName;
	update();
}

int ExperimentGraphBlockItem::type() const
{
	return ExperimentStructureItemType::TypeBlockItem;
}

QPainterPath ExperimentGraphBlockItem::shape() const
{
	return pShape;
}

QRectF ExperimentGraphBlockItem::boundingRect() const
{
	return rBoundingBox;
}

void ExperimentGraphBlockItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	painter->setPen(pPen);
	painter->setBrush(pBrush);

	pShape = QPainterPath();
	if((bType == GRAPHBLOCK_TYPE_BLOCK) || (bType == GRAPHBLOCK_TYPE_END))
	{
		pShape.moveTo(0, -nHalfBlockHeight-nHalfBlockDistance);
		pShape.lineTo(0, -nHalfBlockHeight);
	}
	QRectF rectInnerBlock(-nHalfBlockWidth, -nHalfBlockHeight, nBlockWidth, nBlockHeight);	
	pShape.addRoundedRect(rectInnerBlock,50,50);	
	if((bType == GRAPHBLOCK_TYPE_BLOCK) || (bType == GRAPHBLOCK_TYPE_START))
	{
		pShape.moveTo(0, nHalfBlockHeight);
		pShape.lineTo(0, nHalfBlockHeight+nHalfBlockDistance);
	}
	painter->drawPath(pShape);

	QFont fCurrent("Times");
	if(bIsCurrentlyHovered)
		fCurrent.setBold(true);
	else
		fCurrent.setBold(false);
	fCurrent.setPointSizeF(rectInnerBlock.height()/4);
	painter->setFont(fCurrent);

	if(bType == GRAPHBLOCK_TYPE_START)
	{
		rBoundingBox.setCoords(-nHalfBlockWidth,-nHalfBlockHeight,nHalfBlockWidth,nHalfBlockHeight+nHalfBlockDistance);
		painter->drawText(rectInnerBlock,Qt::AlignCenter, "START");
	}
	else if(bType == GRAPHBLOCK_TYPE_BLOCK)
	{
		rBoundingBox.setCoords(-nHalfBlockWidth,-nHalfBlockHeight-nHalfBlockDistance,nHalfBlockWidth,nHalfBlockHeight+nHalfBlockDistance);
		painter->drawText(rectInnerBlock,Qt::AlignLeft | Qt::AlignVCenter, " " + sCurrentText);
	}
	else if(bType == GRAPHBLOCK_TYPE_END)
	{
		rBoundingBox.setCoords(-nHalfBlockWidth,-nHalfBlockHeight-nHalfBlockDistance,nHalfBlockWidth,nHalfBlockHeight);
		painter->drawText(rectInnerBlock,Qt::AlignCenter, "END");
	}
	//if(bIsCurrentlyHovered)
	//{
	//	painter->setBrush(Qt::NoBrush); painter->setPen(QColor(255,0,0)); painter->drawRect(rBoundingBox); 
	//}
}

void ExperimentGraphBlockItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
	bIsCurrentlyHovered = true;
	pBrush.setColor(cSelectedFill);
	update();
}

//void QGraphicsItem::hoverMoveEvent ( QGraphicsSceneHoverEvent * event )

void ExperimentGraphBlockItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	bIsCurrentlyHovered = false;
	pBrush.setColor(cUnselectedFill);
	update();
}