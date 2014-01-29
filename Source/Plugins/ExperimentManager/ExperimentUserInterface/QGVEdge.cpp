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

#include "QGVEdge.h"
#include "QGVScene.h"
#include <QDebug>
#include <QPainter>

QGVEdge::QGVEdge(Agedge_t *edge, QGVScene *scene) :  _edge(edge), _scene(scene)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVEdge::~QGVEdge()
{
    _scene->removeItem(this);
}

QString QGVEdge::label() const
{
    return getAttribute("xlabel");
}

QRectF QGVEdge::boundingRect() const
{
    return _path.boundingRect() | _head_arrow.boundingRect() | _tail_arrow.boundingRect() | _label_rect;
}

QPainterPath QGVEdge::shape() const
{
    QPainterPathStroker ps;
    ps.setCapStyle(_pen.capStyle());
    ps.setWidth(_pen.widthF() + 10);
    ps.setJoinStyle(_pen.joinStyle());
    ps.setMiterLimit(_pen.miterLimit());
    return ps.createStroke(_path);
}

void QGVEdge::setLabel(const QString &label)
{
    setAttribute("xlabel", label);
}

void QGVEdge::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

    painter->save();
	if(this->getAttribute("penwidth").isEmpty() == false)
		_pen.setWidth(this->getAttribute("penwidth").toInt());
	if(isSelected())
    {
        QPen tpen(_pen);
        tpen.setColor(_pen.color().darker(120));
        tpen.setStyle(Qt::DotLine);
        painter->setPen(tpen);
    }
    else
	{
		painter->setPen(_pen);
	}
    painter->drawPath(_path);
    
/*

    QRectF rectControl = _path.controlPointRect();
	QRectF rectHead = _head_arrow.boundingRect();
	QRectF rectTail = _path.boundingRect();
    //if(pp.width() < pp.height())
    //{
    //   painter->save();
    //    painter->translate(_label_rect.topLeft());
    //    painter->rotate(90);
    //    painter->drawText(QRectF(QPointF(0, -_label_rect.width()), _label_rect.size()), Qt::AlignCenter, _label);
    //    painter->restore();
    //}
    
	painter->save();
	//painter->drawRect(rectHead);
	//painter->drawRect(rectControl);
	if(rectHead.x() < _label_rect.x()) //Left 
	{

	}
	else //Right
	{
		if((_label_rect.bottom()-_label_rect.height()) > rectControl.top())
			painter->translate(0,-_label_rect.height());
		else
			painter->translate(0,-1*(_label_rect.bottom() - rectControl.bottom()));
	}
	//painter->drawText(_label_rect, Qt::AlignCenter, _label);
	//painter->drawRect(_label_rect);
	
	QLineF tmpLine(rectTail.center(), rectHead.center());
	tmpLine = tmpLine.unitVector();
	
	if(rectHead.x() < _label_rect.x()) //Left 
	{
		if(rectHead.top() > _label_rect.top()) //Top-Left
		{
			//painter->save();
			//painter->translate((_label_rect.width()/-2), (_label_rect.height()/2));
			//painter->setPen(QColor("brown"));
			//painter->drawRect(_label_rect);
			//painter->restore();
			//painter->setPen(QColor("yellow"));
			painter->translate((_label_rect.width()/-2)*(tmpLine.dx()/-1.5), (_label_rect.height()/2)*(tmpLine.dy()/2));
			//painter->drawRect(_label_rect);
		}
		else //Bottom-Left
		{
			//painter->save();
			//painter->translate((_label_rect.width()/2), (_label_rect.height()/2));
			//painter->setPen(QColor("brown"));
			//painter->drawRect(_label_rect);
			//painter->restore();
			//painter->setPen(QColor("yellow"));
			painter->translate((_label_rect.width()/2)*(tmpLine.dx()/-1.5), (_label_rect.height()/2)*(tmpLine.dy()/-2));
			//painter->drawRect(_label_rect);
		}
	}
	else //Right
	{
		if(rectHead.top() > _label_rect.top()) //Top-Right
		{
			//painter->save();
			//painter->translate((_label_rect.width()/-2), (_label_rect.height()/-2));
			//painter->setPen(QColor("brown"));
			//painter->drawRect(_label_rect);
			//painter->restore();
			//painter->setPen(QColor("yellow"));
			painter->translate((_label_rect.width()/-2)*(tmpLine.dx()/1.5), (_label_rect.height()/-2)*(tmpLine.dy()/2));
			//painter->drawRect(_label_rect);
		}
		else //Bottom-Right
		{
			//painter->save();
			//painter->translate((_label_rect.width()/2), (_label_rect.height()/-2));
			//painter->setPen(QColor("brown"));
			//painter->drawRect(_label_rect);
			//painter->restore();
			//painter->setPen(QColor("yellow"));
			painter->translate((_label_rect.width()/2)*(tmpLine.dx()/1.5), (_label_rect.height()/2)*(tmpLine.dy()/2));
			//painter->drawRect(_label_rect);
		}		
	}

*/

	QFont serifFont("Courier", 15, QFont::Bold);//"Courier" is fixed and should not be changed!
	if(getAttribute("labelfontsize").isEmpty() == false)
	{
		serifFont.setPointSize(getAttribute("labelfontsize").toInt());
	}	
	painter->setFont(serifFont);
	QFontMetrics fm(serifFont);
	_label_rect.setSize(fm.size(Qt::TextSingleLine,_label));
	painter->drawText(_label_rect, Qt::AlignLeft, _label);
	//painter->drawText(Qt::AlignCenter, _label);
	painter->restore();
	painter->setBrush(QBrush(_pen.color(), Qt::SolidPattern));
    painter->drawPolygon(_head_arrow);
    painter->drawPolygon(_tail_arrow);
    painter->restore();
}

void QGVEdge::setAttribute(const QString &name, const QString &value)
{
    agsafeset(_edge, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

QString QGVEdge::getAttribute(const QString &name) const
{
    char* value = agget(_edge, name.toLocal8Bit().data());
    if(value)
        return value;
    return QString();
}

void QGVEdge::updateLayout()
{
    prepareGeometryChange();

    qreal gheight = QGVCore::graphHeight(_scene->_graph);

    const splines* spl = ED_spl(_edge);
    _path = QGVCore::toPath(spl, gheight);


    //Edge arrows
    if((spl->list != 0) && (spl->list->size%3 == 1))
    {
        if(spl->list->sflag)
        {
            _tail_arrow = toArrow(QLineF(QGVCore::toPoint(spl->list->list[0], gheight), QGVCore::toPoint(spl->list->sp, gheight)));
        }

        if(spl->list->eflag)
        {
            _head_arrow = toArrow(QLineF(QGVCore::toPoint(spl->list->list[spl->list->size-1], gheight), QGVCore::toPoint(spl->list->ep, gheight)));
        }
    }

    _pen.setWidth(1);
    _pen.setColor(QGVCore::toColor(getAttribute("color")));
    _pen.setStyle(QGVCore::toPenStyle(getAttribute("style")));

    //Edge label
   	textlabel_t *xlabel = ED_xlabel(_edge);
    if(xlabel)
    {
        _label = xlabel->text;
        _label_rect.setSize(QSize(xlabel->dimen.x, xlabel->dimen.y));
        _label_rect.moveCenter(QGVCore::toPoint(xlabel->pos, QGVCore::graphHeight(_scene->_graph)));
    }

    setToolTip(getAttribute("tooltip"));
}

QPolygonF QGVEdge::toArrow(const QLineF &line) const
{
    QLineF n = line.normalVector();
    QPointF o(n.dx() / 3.0, n.dy() / 3.0);

    //Only support normal arrow type
    QPolygonF polygon;
    polygon.append(line.p1() + o);
    polygon.append(line.p2());
    polygon.append(line.p1() - o);

    return polygon;
}
