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


#include "optionpage.h"

OptionPage::OptionPage(QWidget *parent, GlobalApplicationInformation *g_AppInfo) : QDialog(parent), glob_AppInfo(g_AppInfo)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(applySettings()));	
	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
	readSettings();	
}
OptionPage::~OptionPage()
{

}

void OptionPage::applySettings()
{
	glob_AppInfo->setRegistryInformation(REGISTRY_DONOTLOADSCRIPTEXTENSION,(bool)(ui.chkDoNotLoadQTBindings->checkState() && Qt::Checked));
	glob_AppInfo->setRegistryInformation(REGISTRY_OPENINEXTERNALDEBUGGER,(bool)(ui.chk_OpenDebOnError->checkState() && Qt::Checked));
	glob_AppInfo->setRegistryInformation(REGISTRY_ALLOWMULTIPLEINHERITANCE,(bool)(ui.chkAllowMultipleInstances->checkState() && Qt::Checked));

	if (ui.rdb_3DRenderer->isChecked())	
	{		
		glob_AppInfo->setRegistryInformation(REGISTRY_RENDERTYPE, SvgView::Native);//SvgView::Native);	
		glob_AppInfo->setRegistryInformation(REGISTRY_HQANTIALIAS, false);
	}
	else if (ui.rdb_3DRenderer_2->isChecked()) 
	{
		glob_AppInfo->setRegistryInformation(REGISTRY_RENDERTYPE, SvgView::OpenGL);//SvgView::OpenGL);	
		glob_AppInfo->setRegistryInformation(REGISTRY_HQANTIALIAS, (bool)(ui.chk_HighAntiAFilter->checkState() && Qt::Checked));
	}
	else if (ui.rdb_3DRenderer_3->isChecked()) 
	{
		glob_AppInfo->setRegistryInformation(REGISTRY_RENDERTYPE, SvgView::Image);//SvgView::Image);	
		glob_AppInfo->setRegistryInformation(REGISTRY_HQANTIALIAS, false);
	}
}

void OptionPage::readSettings()
{
	bool bTemp;
	if(glob_AppInfo->checkRegistryInformation(REGISTRY_DONOTLOADSCRIPTEXTENSION))
	{
		bTemp = glob_AppInfo->getRegistryInformation(REGISTRY_DONOTLOADSCRIPTEXTENSION).toBool();
		ui.chkDoNotLoadQTBindings->setCheckState((Qt::CheckState)(bTemp*Qt::Checked));
	}
	if(glob_AppInfo->checkRegistryInformation(REGISTRY_OPENINEXTERNALDEBUGGER))
	{
		bTemp = glob_AppInfo->getRegistryInformation(REGISTRY_OPENINEXTERNALDEBUGGER).toBool();
		ui.chk_OpenDebOnError->setCheckState((Qt::CheckState)(bTemp*Qt::Checked));
	}
	if(glob_AppInfo->checkRegistryInformation(REGISTRY_RENDERTYPE))
	{
		SvgView::RendererType rValue = (SvgView::RendererType)glob_AppInfo->getRegistryInformation(REGISTRY_RENDERTYPE).toInt();
		switch (rValue)
		{
		case SvgView::Native://Native
			{
				ui.chk_HighAntiAFilter->setChecked(false);
				ui.rdb_3DRenderer->setChecked(true);
				break;
			}
		case SvgView::OpenGL://OpenGL
			{
				if(glob_AppInfo->checkRegistryInformation(REGISTRY_HQANTIALIAS))
				{
					bTemp = glob_AppInfo->getRegistryInformation(REGISTRY_HQANTIALIAS).toBool();
					ui.chk_HighAntiAFilter->setCheckState((Qt::CheckState)(bTemp*Qt::Checked));
				}
				else
				{
					ui.chk_HighAntiAFilter->setChecked(false);
				}
				ui.rdb_3DRenderer_2->setChecked(true);
				break;
			}
		case SvgView::Image://Image
			{
				ui.chk_HighAntiAFilter->setChecked(false);
				ui.rdb_3DRenderer_3->setChecked(true);
				break;
			}
		}
	}
	if(glob_AppInfo->checkRegistryInformation(REGISTRY_ALLOWMULTIPLEINHERITANCE))
	{
		bTemp = glob_AppInfo->getRegistryInformation(REGISTRY_ALLOWMULTIPLEINHERITANCE).toBool();
		ui.chkAllowMultipleInstances->setCheckState((Qt::CheckState)(bTemp*Qt::Checked));
	}
}

void OptionPage::on_rdb_3DRenderer_toggled(bool)//native
{
	if (ui.rdb_3DRenderer->isChecked()) setUI3DRendererControls(1);
}

void OptionPage::on_rdb_3DRenderer_2_toggled(bool)//opengl
{
	if (ui.rdb_3DRenderer_2->isChecked())  setUI3DRendererControls(2);
}

void OptionPage::on_rdb_3DRenderer_3_toggled(bool)//image
{
	if (ui.rdb_3DRenderer_3->isChecked())  setUI3DRendererControls(3);
}

void OptionPage::setUI3DRendererControls(int type)
{
	#ifndef QT_NO_OPENGL
		ui.chk_HighAntiAFilter->setEnabled(false);
	#endif
		//if (type == 1)
			//SVGPreviewer->setRenderer(SvgView::Native);
			//type = type;
	#ifndef QT_NO_OPENGL
		//else if (type == 2) {
			if (type == 2) {
			ui.chk_HighAntiAFilter->setEnabled(true);
			//SVGPreviewer->setRenderer(SvgView::OpenGL);
		}
	#endif
		//else if (type == 3) {
			//SVGPreviewer->setRenderer(SvgView::Image);
		//}
}
