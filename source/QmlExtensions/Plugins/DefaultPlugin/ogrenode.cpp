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

#include <RenderSystems/GL/OgreGLTexture.h>
#include <RenderSystems/GL/OgreGLFrameBufferObject.h>
#include <RenderSystems/GL/OgreGLFBORenderTexture.h>

#include "ogrenode.h"
#include "cameranodeobject.h"
#include <Ogre.h>
#include "defines.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

static QString appPath()
{
    QString path = QCoreApplication::applicationDirPath();
    QDir dir(path);
#ifdef Q_WS_MAC
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
#elif defined(Q_WS_WIN)
    dir.cdUp();
#endif
    return dir.absolutePath();
}

OgreNode::OgreNode() : QSGGeometryNode()
    , m_geometry(QSGGeometry::defaultAttributes_TexturedPoint2D(), 4)
    , m_texture(0)
    , m_samples(0)
    , m_quickWindow(0)
    , m_ogreContext(0)
    , m_qtContext(0)
    , m_AAEnabled(false)
    , m_renderTexture(0)
    , m_ogreFBO(0)
    , m_initialized(false)
    , m_dirtyFBO(false)
{
	m_camera = NULL;
	m_root = NULL;
	m_ogreContext = NULL;

    setMaterial(&m_material);
    setOpaqueMaterial(&m_materialO);
    setGeometry(&m_geometry);
    setFlag(UsePreprocess);
}

OgreNode::~OgreNode()
{
    if (m_renderTexture) 
	{
        m_renderTexture->removeAllViewports();
    }
	m_root = Ogre::Root::getSingleton().getSingletonPtr();
    if (m_root) 
	{
        m_root->detachRenderTarget(m_renderTexture);
        if (m_sceneManager) 
		{
			m_sceneManager->destroyAllCameras();
			m_sceneManager->clearScene();
			m_root->destroySceneManager(m_sceneManager);
			if(m_window)
			{
				m_root->detachRenderTarget(m_window);
				m_root->destroyRenderTarget(m_window);
			
			}
			m_root->destroyRenderTarget(m_renderTexture);
			
        }
    }
	if(m_ogreContext)
	{
		delete m_ogreContext;
		m_ogreContext = NULL;
	}
}

void OgreNode::saveOgreState()
{
    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    ctx->functions()->glBindBuffer(GL_ARRAY_BUFFER, 0);
    ctx->functions()->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    ctx->functions()->glBindRenderbuffer(GL_RENDERBUFFER, 0);
    ctx->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);

    ctx->doneCurrent();
    m_qtContext->makeCurrent(m_quickWindow);

    glPushAttrib(GL_ALL_ATTRIB_BITS);
}

void OgreNode::restoreOgreState()
{
    glPopAttrib();

    m_qtContext = QOpenGLContext::currentContext();
    m_qtContext->functions()->glUseProgram(0);
    m_qtContext->doneCurrent();

    m_ogreContext->makeCurrent(m_quickWindow);
    m_ogreContext->functions()->glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_ogreFBO);

		//if (m_ogreFBO == 0)
		//	m_ogreFBO = QOpenGLContext::currentContext()->defaultFramebufferObject();
		//glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_ogreFBO);

}

GLuint OgreNode::getOgreFBO()
{
    if (!m_renderTexture)
        return 0;

    Ogre::GLFrameBufferObject *ogreFbo = 0;
    m_renderTexture->getCustomAttribute("FBO", &ogreFbo);
    Ogre::GLFBOManager *manager = ogreFbo->getManager();
    manager->bind(m_renderTexture);

    GLint id;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &id);

    return id;
}

void OgreNode::preprocess()
{
    restoreOgreState();
    m_renderTexture->update(true);
    saveOgreState();
}

void OgreNode::setQuickWindow(QQuickWindow *window)
{
    m_quickWindow = window;

    // create a new shared OpenGL context to be used exclusively by Ogre
    m_ogreContext = new QOpenGLContext();
    m_ogreContext->setFormat(m_quickWindow->requestedFormat());
    m_ogreContext->setShareContext(QOpenGLContext::currentContext());
    m_ogreContext->create();
}

void OgreNode::update()
{
    restoreOgreState();

    if (!m_initialized)
        init();

    if (m_dirtyFBO) {
        updateFBO();
        m_ogreFBO = getOgreFBO();
        m_dirtyFBO = false;
    }

    saveOgreState();
}

void OgreNode::updateFBO()
{
    if (m_renderTexture)
        Ogre::TextureManager::getSingleton().remove("RttTex");

    rtt_texture = Ogre::TextureManager::getSingleton().createManual("RttTex",
                                                                    Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                                                                    Ogre::TEX_TYPE_2D,
                                                                    m_size.width(),
                                                                    m_size.height(),
                                                                    0,
                                                                    Ogre::PF_R8G8B8A8,
                                                                    Ogre::TU_RENDERTARGET, 0, false,
                                                                    m_AAEnabled ? m_samples : 0);

    m_renderTexture = rtt_texture->getBuffer()->getRenderTarget();

    m_renderTexture->addViewport(m_camera);
    m_renderTexture->getViewport(0)->setClearEveryFrame(true);
    m_renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
    m_renderTexture->getViewport(0)->setOverlaysEnabled(false);

    Ogre::Real aspectRatio = Ogre::Real(m_size.width()) / Ogre::Real(m_size.height());
    m_camera->setAspectRatio(aspectRatio);

    QSGGeometry::updateTexturedRectGeometry(&m_geometry,
                                            QRectF(0, 0, m_size.width(), m_size.height()),
                                            QRectF(0, 0, 1, 1));

    Ogre::GLTexture *nativeTexture = static_cast<Ogre::GLTexture *>(rtt_texture.get());

    delete m_texture;
    m_texture = m_quickWindow->createTextureFromId(nativeTexture->getGLID(), m_size);

    m_material.setTexture(m_texture);
    m_materialO.setTexture(m_texture);
}

void OgreNode::setSize(const QSize &size)
{
    if (size == m_size)
        return;

    m_size = size;
    m_dirtyFBO = true;
    markDirty(DirtyGeometry);
}

void OgreNode::setAAEnabled(bool enable)
{
    if (m_AAEnabled == enable)
        return;

    m_AAEnabled = enable;
    m_dirtyFBO = true;
    markDirty(DirtyMaterial);
}

void OgreNode::init()
{
    const QOpenGLContext *ctx = QOpenGLContext::currentContext();
    QSurfaceFormat format = ctx->format();
    m_samples = format.samples();

    m_root = new Ogre::Root;
    QString glPlugin;
	//QString glPlugin = QLatin1String(OGRE_PLUGIN_DIR);
    //glPlugin.remove("\"");
	glPlugin = MainAppInfo::qmlExtensionsPluginDirPath() + "/" + OGRE3DITEM_PLUGINFOLDER_NAME;//"E:/Projects/StimulGL/Install/qml/plugins/Win32";
//#ifdef DEBUG_PLUGIN
#ifdef DEBUG
	glPlugin += QLatin1String("/RenderSystem_GL_d");
#else
    glPlugin += QLatin1String("/RenderSystem_GL");
#endif
    m_root->loadPlugin(glPlugin.toLatin1().constData());

    Ogre::RenderSystem *renderSystem = m_root->getRenderSystemByName("OpenGL Rendering Subsystem");
    m_root->setRenderSystem(renderSystem);
    m_root->initialise(false);

    Ogre::NameValuePairList params;

    params["externalGLControl"] = "true";
    params["currentGLContext"] = "true";

    //Finally create our window.
    m_window = m_root->createRenderWindow("OgreWindow", 1, 1, false, &params);
    m_window->setVisible(false);
    m_window->update(false);

    // Load resources
    Ogre::ResourceGroupManager::getSingleton().addResourceLocation(QString("E:/Projects/Experiments/StimulGL/Sven/QMLOgre3D/data").toLatin1().data(), "FileSystem");//.zip").toLatin1().data(), "Zip");
		//appPath() + "/resources/data.zip").toLatin1().data(), "Zip");
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Setup scene
    m_sceneManager = m_root->createSceneManager(Ogre::ST_GENERIC, "mySceneManager");
    m_camera = m_sceneManager->createCamera("myCamera");
    m_camera->setNearClipDistance(1);
    m_camera->setFarClipDistance(99999);
    m_camera->setAspectRatio(Ogre::Real(m_size.width()) / Ogre::Real(m_size.height()));

    // Setup content...

    // Set a sky dome
    m_sceneManager->setSkyBox(true, "SpaceSkyBox", 10000);

    // setup some basic lighting for our scene
    m_sceneManager->setAmbientLight(Ogre::ColourValue(0.3, 0.3, 0.3));
    m_sceneManager->createLight("myLight")->setPosition(20, 80, 50);

    // create an ogre head entity and place it at the origin
	Ogre::Entity* ogreHead;
	ogreHead = m_sceneManager->createEntity("Head", "test.mesh");
    m_sceneManager->getRootSceneNode()->attachObject(ogreHead);
	
	//QList<Ogre::Entity*> lOgreEntityList;
	//QList<Ogre::SceneNode*> lOgreSceneNodeList;
	//QString tmpString1;
	//QString tmpString2;
	//for(int i=1;i<=18;i++)
	//{
	//	tmpString2 = "(" + QString::number(i) + ")";//"ogrehead.mesh";
	//	tmpString1 = "Head" + tmpString2;
	//	tmpString2 = "n " + tmpString2 + ".mesh";
	//	//ogreHead = m_sceneManager->createEntity("Head1", "ogrehead.mesh");
	//	lOgreEntityList.append(m_sceneManager->createEntity((Ogre::String)tmpString1.toLocal8Bit().constData(), (Ogre::String)tmpString2.toLocal8Bit().constData()));//"ogrehead.mesh"));//(Ogre::String)tmpString2.toLocal8Bit().constData()));//(Ogre::String)tmpString1.toLocal8Bit().constData()
	//	tmpString1 = tmpString1 + "Node";
	//	lOgreSceneNodeList.append(m_sceneManager->getRootSceneNode()->createChildSceneNode( tmpString1.toLocal8Bit().constData(), Ogre::Vector3( 10 * i, 0, 0 ) ));
	//	lOgreSceneNodeList.last()->attachObject(lOgreEntityList.last());
	//}

    // Setup the camera
    m_cameraObject = new CameraNodeObject(m_camera);
    m_cameraObject->camera()->setAutoTracking(true, m_sceneManager->getRootSceneNode());

    m_initialized = true;
}
