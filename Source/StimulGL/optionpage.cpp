#include "optionpage.h"

OptionPage::OptionPage(QSettings &settings,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	mainSettings = &settings;
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
	mainSettings->beginGroup("General");
	//mainSettings.setValue("name",control);
	mainSettings->setValue("DoNotLoadScriptExtension",ui.chkDoNotLoadQTBindings->checkState());
	mainSettings->endGroup();

	mainSettings->beginGroup("Debugging");
	mainSettings->setValue("OpenExtDebug",ui.chk_OpenDebOnError->checkState());
	//mainSettings->setValue("OpenExtDebug",Qt::Checked);
	mainSettings->endGroup();

	mainSettings->beginGroup("Rendering");
	if (ui.rdb_3DRenderer->isChecked())	
	{		
		mainSettings->setValue("RenderType",1);//SvgView::Native);	
		mainSettings->setValue("HQAntiAlias",Qt::Unchecked);
	}
	else if (ui.rdb_3DRenderer_2->isChecked()) 
	{
		mainSettings->setValue("RenderType",2);//SvgView::OpenGL);	
		mainSettings->setValue("HQAntiAlias",ui.chk_HighAntiAFilter->checkState());
	}
	else if (ui.rdb_3DRenderer_3->isChecked()) 
	{
		mainSettings->setValue("RenderType",3);//SvgView::Image);	
		mainSettings->setValue("HQAntiAlias",Qt::Unchecked);
	}
	mainSettings->endGroup();
}

void OptionPage::readSettings()
{
	mainSettings->beginGroup("General");
	//
	if (mainSettings->contains("DoNotLoadScriptExtension")) {ui.chkDoNotLoadQTBindings->setCheckState(Qt::CheckState(mainSettings->value("DoNotLoadScriptExtension").toInt()));}
	else {}//key doesn't exist, default value!
	mainSettings->endGroup();

	mainSettings->beginGroup("Debugging");
	if (mainSettings->contains("OpenExtDebug"))	{ui.chk_OpenDebOnError->setCheckState(Qt::CheckState(mainSettings->value("OpenExtDebug").toInt()));}
	else {}//key doesn't exist, default value!
	mainSettings->endGroup();

	mainSettings->beginGroup("Rendering");
	if (mainSettings->contains("RenderType"))	
	{
		int type;
		type = mainSettings->value("RenderType").toInt();
		switch (type)
		{
		case 1://Native
			{
				ui.chk_HighAntiAFilter->setChecked(false);
				ui.rdb_3DRenderer->setChecked(true);
				break;
			}
		case 2://OpenGL
			{
				if (mainSettings->contains("HQAntiAlias"))
				{
					ui.chk_HighAntiAFilter->setCheckState(Qt::CheckState(mainSettings->value("HQAntiAlias").toInt()));
				}
				else
				{
					ui.chk_HighAntiAFilter->setChecked(false);
				}
				ui.rdb_3DRenderer_2->setChecked(true);
				break;
			}
		case 3://Image
			{
				ui.chk_HighAntiAFilter->setChecked(false);
				ui.rdb_3DRenderer_3->setChecked(true);
				break;
			}
		}
	}
	else {}//key doesn't exist, default value!
	mainSettings->endGroup();
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
