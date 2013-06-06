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

#ifndef CAMERANODEOBJECT_H
#define CAMERANODEOBJECT_H

#include <QObject>

namespace Ogre {
class SceneNode;
class Camera;
}

class CameraNodeObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qreal yaw READ yaw WRITE setYaw)
    Q_PROPERTY(qreal pitch READ pitch WRITE setPitch)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom)
public:
    explicit CameraNodeObject(Ogre::Camera *cam, QObject *parent = 0);

    Ogre::SceneNode *sceneNode() const
    { return m_node; }
    Ogre::Camera *camera() const
    { return m_camera; }

    qreal yaw() const
    { return m_yaw; }
    qreal pitch() const
    { return m_pitch; }
    qreal zoom() const
    { return m_zoom; }
    void setYaw(qreal y)
    { m_yaw = y; updateRotation(); }
    void setPitch(qreal p)
    { m_pitch = p; updateRotation(); }
    void setZoom(qreal z);

private:
    void updateRotation();

    Ogre::SceneNode *m_node;
    Ogre::Camera *m_camera;

    qreal m_yaw;
    qreal m_pitch;
    qreal m_zoom;
};

#endif // CAMERANODEOBJECT_H
