//StimulGL
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

#include "GlobalApplicationInformation.h"
#include "maindefines.h"
#include <QtWebKitWidgets/QWebView>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>
#include <QResource>
#include <QBuffer>
#include <QSharedMemory>

GlobalApplicationInformation::GlobalApplicationInformation(QObject *parent)
{
	Q_UNUSED(parent);
	webView = NULL;
	Initialize();
	composeJavaScriptConfigurationFile();
	AppRegistrySettings = new QSettings(getCompanyName(),getTitle());
	initAndParseRegistrySettings();
	copyMainAppInformationStructureToSharedMemory(mainAppInformation);
};

GlobalApplicationInformation::~GlobalApplicationInformation()
{
	if(AppRegistrySettings)
	{
		delete AppRegistrySettings;
		AppRegistrySettings = NULL;
	}
	if(webView)
	{
		delete webView;
		webView = NULL;
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalApplicationInformation::Initialize()
{
	mainAppInformation.sCompanyName = "";
	mainAppInformation.sFileVersion = "";
	mainAppInformation.sInternalName = "";

	mainAppInformation.bDoNotLoadScriptBindings = false;
	mainAppInformation.bOpenExtDebug = false;
	mainAppInformation.rRendererType = SvgView::OpenGL;
	mainAppInformation.bHQAntiAlias = false;
	mainAppInformation.bAllowMultipleInheritance = false;
}

QDataStream &operator<<(QDataStream &out, const MainAppInformationStructure &mainAppInformationStructure)
{
	out << mainAppInformationStructure.sCompanyName;
	out	<< mainAppInformationStructure.sInternalName;
	out	<< mainAppInformationStructure.sFileVersion;
	out << mainAppInformationStructure.bDoNotLoadScriptBindings;
	out << mainAppInformationStructure.bOpenExtDebug; 
	out << (int)mainAppInformationStructure.rRendererType;
	out << mainAppInformationStructure.bHQAntiAlias; 
	out << mainAppInformationStructure.bAllowMultipleInheritance;
	return out;
}

QDataStream &operator>>(QDataStream &in, MainAppInformationStructure &mainAppInformationStructure)
{
	//in >> title >> artist >> year;
	int nTemp;
	in >> mainAppInformationStructure.sCompanyName;
	in >> mainAppInformationStructure.sInternalName;
	in >> mainAppInformationStructure.sFileVersion;
	in >> mainAppInformationStructure.bDoNotLoadScriptBindings;
	in >> mainAppInformationStructure.bOpenExtDebug; 
	in >> nTemp;
	mainAppInformationStructure.rRendererType = (SvgView::RendererType)nTemp;
	in >> mainAppInformationStructure.bHQAntiAlias; 
	in >> mainAppInformationStructure.bAllowMultipleInheritance;
	return in;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GlobalApplicationInformation::setCompanyName(const QString &sName)
{
	mainAppInformation.sCompanyName = sName;
	QCoreApplication::setOrganizationDomain(mainAppInformation.sCompanyName);
	QCoreApplication::setOrganizationName(mainAppInformation.sCompanyName);
}

QString GlobalApplicationInformation::getCompanyName()
{
	return mainAppInformation.sCompanyName;
}

void GlobalApplicationInformation::setFileVersionString(const QString &sVersion)	
{
	mainAppInformation.sFileVersion = sVersion;
	QCoreApplication::setApplicationVersion(mainAppInformation.sFileVersion);
}

QString GlobalApplicationInformation::getFileVersionString()
{
	return mainAppInformation.sFileVersion;
}

void GlobalApplicationInformation::setInternalName(const QString &sIntName)		
{
	mainAppInformation.sInternalName = sIntName;
	QCoreApplication::setApplicationName(mainAppInformation.sInternalName);
}

QString GlobalApplicationInformation::getInternalName()
{
	return mainAppInformation.sInternalName;
}

QString GlobalApplicationInformation::getTitle()
{
	return mainAppInformation.sInternalName +  "(v" + mainAppInformation.sFileVersion + ")";
}

bool GlobalApplicationInformation::shouldLoadScriptBindings()
{
	return !(mainAppInformation.bDoNotLoadScriptBindings);
}

////////////////////////////////////////////////////////////////

void GlobalApplicationInformation::initAndParseRegistrySettings()
{
	if (AppRegistrySettings->contains(REGISTRY_DONOTLOADSCRIPTEXTENSION)) 
	{
		mainAppInformation.bDoNotLoadScriptBindings = AppRegistrySettings->value(REGISTRY_DONOTLOADSCRIPTEXTENSION).toBool();
	}
	else //key doesn't exist, default value here!
	{
		mainAppInformation.bDoNotLoadScriptBindings = false;
		AppRegistrySettings->setValue(REGISTRY_DONOTLOADSCRIPTEXTENSION, mainAppInformation.bDoNotLoadScriptBindings);
	}

	if (AppRegistrySettings->contains(REGISTRY_OPENINEXTERNALDEBUGGER)) 
	{
		mainAppInformation.bOpenExtDebug = AppRegistrySettings->value(REGISTRY_OPENINEXTERNALDEBUGGER).toBool();
	}
	else //key doesn't exist, default value here!
	{
		mainAppInformation.bOpenExtDebug = false;
		AppRegistrySettings->setValue(REGISTRY_OPENINEXTERNALDEBUGGER, mainAppInformation.bOpenExtDebug);
	}

	if (AppRegistrySettings->contains(REGISTRY_RENDERTYPE)) 
	{
		mainAppInformation.rRendererType = (SvgView::RendererType)AppRegistrySettings->value(REGISTRY_RENDERTYPE).toInt();
	}
	else //key doesn't exist, default value here!
	{
		mainAppInformation.rRendererType = SvgView::OpenGL;
		AppRegistrySettings->setValue(REGISTRY_RENDERTYPE, (int)mainAppInformation.rRendererType);
	}

	if (AppRegistrySettings->contains(REGISTRY_HQANTIALIAS)) 
	{
		mainAppInformation.bHQAntiAlias = AppRegistrySettings->value(REGISTRY_HQANTIALIAS).toBool();		
	}
	else //key doesn't exist, default value here!
	{
		mainAppInformation.bHQAntiAlias = false;
		AppRegistrySettings->setValue(REGISTRY_HQANTIALIAS, mainAppInformation.bHQAntiAlias);
	}

	if (AppRegistrySettings->contains(REGISTRY_ALLOWMULTIPLEINHERITANCE)) 
	{
		mainAppInformation.bAllowMultipleInheritance = AppRegistrySettings->value(REGISTRY_ALLOWMULTIPLEINHERITANCE).toBool();		
	}
	else //key doesn't exist, default value here!
	{
		mainAppInformation.bAllowMultipleInheritance = false;
		AppRegistrySettings->setValue(REGISTRY_ALLOWMULTIPLEINHERITANCE, mainAppInformation.bAllowMultipleInheritance);
	}
	AppRegistrySettings->sync();
}

bool GlobalApplicationInformation::setRegistryInformation(const QString &sName, const QVariant &vValue)
{
	if (sName == REGISTRY_DONOTLOADSCRIPTEXTENSION)
	{
		AppRegistrySettings->setValue(REGISTRY_DONOTLOADSCRIPTEXTENSION, vValue.toBool());
	}
	else if (sName == REGISTRY_OPENINEXTERNALDEBUGGER)
	{
		AppRegistrySettings->setValue(REGISTRY_OPENINEXTERNALDEBUGGER, vValue.toBool());
	}
	else if (sName == REGISTRY_RENDERTYPE)
	{
		AppRegistrySettings->setValue(REGISTRY_RENDERTYPE,vValue.toInt());//SvgView::Native, SvgView::OpenGL, SvgView::Image
	}
	else if (sName == REGISTRY_HQANTIALIAS)
	{
		AppRegistrySettings->setValue(REGISTRY_HQANTIALIAS, vValue.toBool());
	}
	else if (sName == REGISTRY_ALLOWMULTIPLEINHERITANCE)
	{
		AppRegistrySettings->setValue(REGISTRY_ALLOWMULTIPLEINHERITANCE, vValue.toBool());
	}
	else if (sName == REGISTRY_RECENTFILELIST)
	{
		AppRegistrySettings->setValue(REGISTRY_RECENTFILELIST, vValue.toStringList());
	}
	else if (sName == REGISTRY_MAINWINDOWPOS)
	{
		AppRegistrySettings->setValue(REGISTRY_MAINWINDOWPOS, vValue.toPoint());
	}
	else if (sName == REGISTRY_MAINWINDOWSIZE)
	{
		AppRegistrySettings->setValue(REGISTRY_MAINWINDOWSIZE, vValue.toSize());
	}
	else if (sName == REGISTRY_DEBUGWINDOWWIDTH)
	{
		AppRegistrySettings->setValue(REGISTRY_DEBUGWINDOWWIDTH, vValue.toInt());
	}
	else
	{
		return false;
	}
	return true;
}

bool GlobalApplicationInformation::checkRegistryInformation(const QString &sName)
{
	return AppRegistrySettings->contains(sName);
}

QVariant GlobalApplicationInformation::getRegistryInformation(const QString &sName)
{
	if(AppRegistrySettings->contains(sName) == false)
		return NULL;
	return AppRegistrySettings->value(sName);	
}

/////////////////////////////////////////////////////////////////////////////////////////

void GlobalApplicationInformation::parseJavaScriptConfigurationFile()
{
	setCompanyName(invokeJavaScriptConfigurationFile("StimulGLInfo.GetMainAppCompanyName()").toString());
	setInternalName(invokeJavaScriptConfigurationFile("StimulGLInfo.GetMainAppInternalName()").toString());
	setFileVersionString(invokeJavaScriptConfigurationFile("StimulGLInfo.GetCurrentRelease()").toString());
}

void GlobalApplicationInformation::composeJavaScriptConfigurationFile()
{
	bool bConnected = false;
	QWebView *webViewCast = qobject_cast<QWebView *>(webView);
	if(webViewCast == NULL)
	{
		webView = new QWebView();
		webViewCast = qobject_cast<QWebView *>(webView);
		webViewCast->setObjectName(QString::fromUtf8("webView"));
		bConnected = connect(webViewCast, SIGNAL(loadFinished(bool)), SLOT(parseJavaScriptConfigurationFile()),Qt::DirectConnection);
	}
	QString sPath = (":/resources/StimulGL.js");
	QResource res(sPath);
	QByteArray bResourceData;
	bResourceData=reinterpret_cast< const char* >( res.data() ), res.size();
	webViewCast->page()->mainFrame()->evaluateJavaScript(bResourceData);
	sPath = (":/resources/versioning.html");
	res.setFileName(sPath);
	bResourceData = reinterpret_cast< const char* >( res.data() ), res.size();
	webViewCast->setContent(bResourceData);//Invokes the loadFinished() signal after loading
}

QVariant GlobalApplicationInformation::invokeJavaScriptConfigurationFile(const QString &sCode)
{
	if(webView)
		return qobject_cast<QWebView *>(webView)->page()->mainFrame()->evaluateJavaScript(sCode);
	return NULL;
}

void GlobalApplicationInformation::showJavaScriptConfigurationFile()
{
	if(webView)
	{
		qobject_cast<QWebView *>(webView)->show();
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

MainAppInformationStructure GlobalApplicationInformation::getStaticMainAppInformationStructureFromSharedMemory()
{
	MainAppInformationStructure tmpAppInfoStruct;
	QSharedMemory sharedMemory;
	sharedMemory.setKey(MAIN_PROGRAM_SHARED_MEM_KEY);//Sets a new key for this shared memory object. If key and the current key are the same, the function returns without doing anything.
	if (sharedMemory.attach(QSharedMemory::ReadOnly))//ReadWrite))//Attempts to attach the process to the shared memory segment identified by the key that was passed to the constructor.
	{
		QBuffer buffer;
		QDataStream inStream(&buffer);
		sharedMemory.lock();
		buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
		buffer.open(QBuffer::ReadOnly);
		inStream >> tmpAppInfoStruct;
		sharedMemory.unlock();
		return tmpAppInfoStruct;
	}
	return tmpAppInfoStruct;
}

//bool GlobalApplicationInformation::fetchMainAppInformationStructure()
//{
//	QSharedMemory sharedMemory;
//	sharedMemory.setKey(MAIN_PROGRAM_SHARED_MEM_KEY);//Sets a new key for this shared memory object. If key and the current key are the same, the function returns without doing anything.
//	if (sharedMemory.attach(QSharedMemory::ReadOnly))//ReadWrite))//Attempts to attach the process to the shared memory segment identified by the key that was passed to the constructor.
//	{
//		QBuffer buffer;
//		QDataStream inStream(&buffer);
//		sharedMemory.lock();
//		buffer.setData((char*)sharedMemory.constData(), sharedMemory.size());
//		buffer.open(QBuffer::ReadOnly);
//		inStream >> mainAppInformation;
//		sharedMemory.unlock();
//		return true;
//	}
//	return false;
//}

bool GlobalApplicationInformation::copyMainAppInformationStructureToSharedMemory(const MainAppInformationStructure &mainAppInformationStructure)
{
	QSharedMemory sharedMemory;
	sharedMemory.setKey(MAIN_PROGRAM_SHARED_MEM_KEY);//Sets a new key for this shared memory object. If key and the current key are the same, the function returns without doing anything.
	if (sharedMemory.attach(QSharedMemory::ReadWrite))//Attempts to attach the process to the shared memory segment identified by the key that was passed to the constructor.
	{
		QBuffer buffer;
		buffer.open(QBuffer::ReadWrite);
		QDataStream outStream(&buffer);
		outStream << mainAppInformationStructure;
		int nSize = buffer.size();

		sharedMemory.lock();
		char *cDestination = (char*)sharedMemory.data();
		const char *cSource = buffer.data().data();
		memcpy(cDestination, cSource, qMin(sharedMemory.size(), nSize));
		sharedMemory.unlock();
		return true;
	}
	return false;
}
