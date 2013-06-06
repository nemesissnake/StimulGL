//DefaultQMLPlugin
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

#include "ogreitem.h"
#include "ogrenode.h"
#include "cameranodeobject.h"

#include <QtCore/QPropertyAnimation>

OgreItem::OgreItem(QQuickItem *parent) : QQuickItem(parent), m_timerID(0)
{
	m_camera = NULL;
    setFlag(ItemHasContents);
    setSmooth(false);
    startTimer(16);
}

OgreItem::~OgreItem()
{
}

QSGNode *OgreItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return 0;
    }
    OgreNode *node = static_cast<OgreNode *>(oldNode);
    if (!node)
    {
        node = new OgreNode();
        node->setQuickWindow(window());
    }
    node->setSize(QSize(width(), height()));
    node->setAAEnabled(smooth());
    node->update();
    m_camera = static_cast<QObject *>(node->camera());
    return node;
}

void OgreItem::timerEvent(QTimerEvent *)
{
    update();
}
