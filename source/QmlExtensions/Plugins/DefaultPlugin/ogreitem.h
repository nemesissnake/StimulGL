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

#ifndef OGREITEM_H
#define OGREITEM_H

#include <QtQuick/QQuickItem>
#include <QtCore/QPropertyAnimation>
#include "ogrenode.h"

class CameraNodeObject;

class OgreItem : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QObject *camera READ camera)

signals:
	void ogreNodeInitialized();
	void ogreNodePreInitialize();

public:
    OgreItem(QQuickItem *parent = 0);
	~OgreItem();
    QObject *camera() const { return m_camera; }

public slots:
	bool addResourceLocation(const QString &sLocation,const QString &sType);
	bool createEntity(const QString &sEntityName, const QString &sEntityMesh);
	bool createSceneNode(const QString &sNodeName, const QString &sEntityName, const float &xPos, const float &yPos, const float &zPos);
	bool createLightSource(const QString &sLightName, const float &xPos, const float &yPos, const float &zPos);
	bool setAmbientColor(const int &nRed, const int &nGreen, const int &nBlue, const int &nAlpha = 255);
	QVector3D getObjectBoundingBoxCenter(const QString &sSceneNodeName, const QString &sObjectName);
	QVector3D getObjectBoundingBoxSize(const QString &sSceneNodeName, const QString &sObjectName);
	void setObjectVisibility(const QString &sSceneNodeName, const QString &sObjectName, const bool &bVisible);
	//bool setObjectAttachement(const QString &sSceneNodeName, const QString &sObjectName, const bool &bAttachment);
	//void startRenderLoop();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);
    void timerEvent(QTimerEvent *);

private:
	bool bIsPreInitializing;
	bool bFirstUpdatePaintNode;
    int m_timerID;
    QObject *m_camera;
	OgreNode *m_node;
	QList<sTypeOgreResourcesStructure> lResources;
	QList<sEntityStructure> lEntities;
	QList<sSceneNodeStructure> lSceneNodes;
	QList<sLightSourceStructure> lLightSources;
	QColor *cAmbientSceneColor;		
};

#endif // OGREITEM_H
