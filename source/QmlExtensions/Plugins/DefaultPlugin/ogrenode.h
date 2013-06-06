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

#ifndef OGRENODE_H
#define OGRENODE_H

#include "Ogre.h"
#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGTextureMaterial>
#include <QtQuick/QSGOpaqueTextureMaterial>
#include <QtQuick/QQuickWindow>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define OGRE_PLUGIN_DIR STRINGIFY(OGRE_PLUGIN_VAR)

namespace Ogre {
class Root;
class Camera;
class SceneManager;
class RenderTexture;
class Viewport;
class RenderTarget;
}

class CameraNodeObject;

class OgreNode : public QSGGeometryNode
{
public:
    OgreNode();
    ~OgreNode();

    void setSize(const QSize &size);
    QSize size() const { return m_size; }
    void setQuickWindow(QQuickWindow *window);
	void setAAEnabled(bool enable);
    CameraNodeObject *camera() const { return m_cameraObject; }
    void update();
    void updateFBO();
    void init();
    GLuint getOgreFBO();
    void saveOgreState();
    void restoreOgreState();
    void preprocess();

private:
    QSGTextureMaterial m_material;
    QSGOpaqueTextureMaterial m_materialO;
    QSGGeometry m_geometry;
    QSGTexture *m_texture;
    QQuickWindow *m_quickWindow;
    /** Pointer to QOpenGLContext to be used by Ogre. */
    QOpenGLContext* m_ogreContext;
    /** Pointer to QOpenGLContext to be restored after Ogre context. */
    QOpenGLContext* m_qtContext;

    int m_samples;
    bool m_AAEnabled;
    QSize m_size;

    Ogre::Root *m_root;
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneManager;
    Ogre::RenderTexture *m_renderTexture;
    Ogre::Viewport *m_viewport;
    Ogre::TexturePtr rtt_texture;
    Ogre::RenderWindow *m_window;

    GLuint m_ogreFBO;

    CameraNodeObject *m_cameraObject;

    bool m_initialized;
    bool m_dirtyFBO;
};

#endif // OGRENODE_H
