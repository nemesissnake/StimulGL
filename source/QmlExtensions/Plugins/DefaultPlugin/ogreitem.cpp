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
#include "cameranodeobject.h"

#include <QtCore/QPropertyAnimation>
#include <QTimer>

OgreItem::OgreItem(QQuickItem *parent) : QQuickItem(parent), m_timerID(0)
{
	m_node = NULL;
	m_camera = NULL;
    setFlag(ItemHasContents);
    setSmooth(false);
    startTimer(16);
	bFirstUpdatePaintNode = true;
}

OgreItem::~OgreItem()
{
	lResources.clear();
}

bool OgreItem::addResourceLocation(const QString &sLocation,const QString &sType)
{
	sTypeOgreResourcesStructure tmpResource;
	tmpResource.sLocation = sLocation;
	tmpResource.sType = sType;
	lResources.append(tmpResource);
	return true;
}

bool OgreItem::createEntity(const QString &sEntityName, const QString &sEntityMesh)
{
	sEntityStructure tmpEntity;
	tmpEntity.pEntity = NULL;
	tmpEntity.sName = sEntityName;
	tmpEntity.sMesh = sEntityMesh;
	lEntities.append(tmpEntity);
	return true;
}

bool OgreItem::createSceneNode(const QString &sNodeName, const QString &sEntityName, const float &xPos, const float &yPos, const float &zPos)
{
	sSceneNodeStructure tmpSceneNode;
	tmpSceneNode.pSceneNode = NULL;
	tmpSceneNode.sName = sNodeName;
	tmpSceneNode.sEntityName = sEntityName;
	tmpSceneNode.xPos = xPos;
	tmpSceneNode.yPos = yPos;
	tmpSceneNode.zPos = zPos;
	lSceneNodes.append(tmpSceneNode);
	return true;
}

QVector3D OgreItem::getObjectBoundingBoxCenter(const QString &sSceneNodeName, const QString &sObjectName)
{
	Ogre::Vector3 tmpVec = Ogre::Root::getSingleton().getSceneManager("mySceneManager")->getSceneNode(sSceneNodeName.toLocal8Bit().constData())->getAttachedObject(sObjectName.toLocal8Bit().constData())->getWorldBoundingBox().getCenter();//getCorner(Ogre::AxisAlignedBox::FAR_LEFT_BOTTOM);//Ogre::CornerEnum::FAR_LEFT_BOTTOM
	return QVector3D(tmpVec.x,tmpVec.y,tmpVec.z);
}

QVector3D OgreItem::getObjectBoundingBoxSize(const QString &sSceneNodeName, const QString &sObjectName)
{
	Ogre::Vector3 tmpVec = Ogre::Root::getSingleton().getSceneManager("mySceneManager")->getSceneNode(sSceneNodeName.toLocal8Bit().constData())->getAttachedObject(sObjectName.toLocal8Bit().constData())->getBoundingBox().getSize();
	return QVector3D(tmpVec.x,tmpVec.y,tmpVec.z);
}

QSGNode *OgreItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return 0;
    }
    m_node = static_cast<OgreNode *>(oldNode);
	bool bDoEmit = false;
    if (!m_node)
    {
        m_node = new OgreNode();
		if(bFirstUpdatePaintNode)
		{
			bFirstUpdatePaintNode = false;
			if(lResources.isEmpty() == false)
			{
				m_node->setResourceLocations(lResources);
				lResources.clear();
				m_node->setEntities(lEntities);
				lEntities.clear();
				m_node->setSceneNodes(lSceneNodes);
				lSceneNodes.clear();				
			}
		}
		m_node->setQuickWindow(window());
		bDoEmit = true;
    }
    m_node->setSize(QSize(width(), height()));
    m_node->setAAEnabled(smooth());
    m_node->update();
    m_camera = static_cast<QObject *>(m_node->camera());
	if(bDoEmit)
		QTimer::singleShot(1, this, SIGNAL(ogreNodeInitialized()));//Thread safety
    return m_node;
}

void OgreItem::timerEvent(QTimerEvent *)
{
    update();
}
