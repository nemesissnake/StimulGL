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

#include "QGVNode.h"
#include "QGVScene.h"
#include <QDebug>
#include <QPainter>

QGVNode::QGVNode(Agnode_t* node, QGVScene *scene): _node(node), _scene(scene)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
}

QGVNode::~QGVNode()
{
    _scene->removeItem(this);
}

QString QGVNode::label() const
{
    return getAttribute("label");
}

void QGVNode::setLabel(const QString &label)
{
    setAttribute("label", label);
}

QRectF QGVNode::boundingRect() const
{
    return _path.boundingRect();
}

void QGVNode::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(option);

    painter->save();
    painter->setPen(_pen);
    if(isSelected())
    {
        QBrush tbrush(_brush);
        tbrush.setColor(tbrush.color().darker(120));
        painter->setBrush(tbrush);
    }
    else
        painter->setBrush(_brush);

    painter->drawPath(_path);

    painter->setPen(QGVCore::toColor(getAttribute("labelfontcolor")));

    const QRectF rect = boundingRect().adjusted(2,2,-2,-2); //Margin
    if(_icon.isNull())
    {
		//QString tmpString = QGVNode::label();
		QFont serifFont("Courier", 15, QFont::Bold);//"Courier" is fixed and should not be changed!
		if(getAttribute("labelfontsize").isEmpty() == false)
		{
			serifFont.setPointSize(getAttribute("labelfontsize").toInt());
		}	
		painter->setFont(serifFont);

		QFontMetrics fm(serifFont);
		int charPixelsWidth = fm.width("s");//Courier is in mono-space, so let's see how much space a single character takes...
		int nMaxChars = rect.width()/charPixelsWidth;
		QString tmpLabelText = QGVNode::label();
		if(tmpLabelText.size() > nMaxChars)
		{
			if(nMaxChars > 2)
				tmpLabelText = tmpLabelText.left(nMaxChars-2) + "..";
		}
        painter->drawText(rect, Qt::AlignHCenter, tmpLabelText);
    }
    else
    {
        painter->drawText(rect.adjusted(0,0,0, -rect.height()*2/3), Qt::AlignCenter , QGVNode::label());
        const QRectF img_rect = rect.adjusted(0, rect.height()/3,0, 0);
        QImage img = _icon.scaled(img_rect.size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter->drawImage(img_rect.topLeft() + QPointF((img_rect.width() - img.rect().width())/2, 0), img);
    }
    painter->restore();
}

void QGVNode::setAttribute(const QString &name, const QString &value)
{
    agsafeset(_node, name.toLocal8Bit().data(), value.toLocal8Bit().data(), "");
}

QString QGVNode::getAttribute(const QString &name) const
{
    char* value = agget(_node, name.toLocal8Bit().data());
    if(value)
        return value;
    return QString();
}

void QGVNode::setIcon(const QImage &icon)
{
    _icon = icon;
}

void QGVNode::updateLayout()
{
    prepareGeometryChange();
    qreal width = ND_width(_node)*DotDefaultDPI;
    qreal height = ND_height(_node)*DotDefaultDPI;

    //Node Position (center)
    qreal gheight = QGVCore::graphHeight(_scene->_graph);

    setPos(QGVCore::centerToOrigin(QGVCore::toPoint(ND_coord(_node), gheight), width, height));

    //Node on top
    setZValue(1);

    //Node path
    _path = QGVCore::toPath(ND_shape(_node)->name, (polygon_t*)ND_shape_info(_node), width, height);
    _pen.setWidth(1);

    _brush.setStyle(QGVCore::toBrushStyle(getAttribute("style")));
    _brush.setColor(QGVCore::toColor(getAttribute("fillcolor")));
    _pen.setColor(QGVCore::toColor(getAttribute("color")));

    setToolTip(getAttribute("tooltip"));
}
