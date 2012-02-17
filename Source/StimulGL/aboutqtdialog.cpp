//StimulGL
//Copyright (C) 2012  Sven Gijsen
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


/****************************************************************************
**
**
****************************************************************************/
#include "aboutqtdialog.h"

#include <QPluginLoader>
#include <QStringList>
#include <QDir>

#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include "maindefines.h"

aboutQTDialog::aboutQTDialog(const QString &title,const QStringList &pluginFileNames,const QString &path, const QStringList &fileNames, QWidget *parent) : 
	QDialog(parent),
	label(new QLabel),
	treeWidget(new QTreeWidget),
	okButton(new QPushButton(tr("OK")))
{
	labWebsite = new QLabel;
	treeWidget->setAlternatingRowColors(false);
	treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
	treeWidget->setColumnCount(1);
	treeWidget->header()->hide();

	okButton->setDefault(true);

	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(2, 1);
	mainLayout->addWidget(labWebsite, 0, 0, 1, 3);
	mainLayout->addWidget(label, 1, 0, 1, 3);
	mainLayout->addWidget(treeWidget, 2, 0, 1, 3);
	mainLayout->addWidget(okButton, 3, 1);
	setLayout(mainLayout);

	interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirOpenIcon),
		QIcon::Normal, QIcon::On);
	interfaceIcon.addPixmap(style()->standardPixmap(QStyle::SP_DirClosedIcon),
		QIcon::Normal, QIcon::Off);
	featureIcon.addPixmap(style()->standardPixmap(QStyle::SP_FileIcon));

	setWindowTitle("About " + title);
	//findPlugins(title,path, fileNames);

	foreach (QObject *plugin, QPluginLoader::staticInstances())
		populateTreeWidget(plugin, tr("%1 (Static Plugin)")
		.arg(plugin->metaObject()->className()));


	label->setText(title + tr(" found the following plugins\n"
		"(Dynamic plugins looked for in %1):")
		.arg(QDir::toNativeSeparators(path)));
	QString tmpStr = ("\n");
	tmpStr.append(MAIN_PROGRAM_INTERNAL_NAME ", " MAIN_PROGRAM_LEGAL_COPYRIGHT ".\n");
	tmpStr.append(MAIN_PROGRAM_COMPANY_NAME ", " MAIN_PROGRAM_AUTHOR_NAME ".\n\n");
	tmpStr.append("StimulGL is free software: you can redistribute it and/or modify\n");
	tmpStr.append("it under the terms of the GNU General Public License as published by\n");
	tmpStr.append("the Free Software Foundation, either version 3 of the License, or\n");
	tmpStr.append("(at your option) any later version.\n");
	tmpStr.append("\n");
	tmpStr.append("This program is distributed in the hope that it will be useful,\n");
	tmpStr.append("but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
	tmpStr.append("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
	tmpStr.append("GNU General Public License for more details.\n");
	tmpStr.append("\n");
	tmpStr.append("You should have received a copy of the GNU General Public License\n");
	tmpStr.append("along with this program.  If not, see <http://www.gnu.org/licenses/>.\n\n");
	tmpStr.append("See https://github.com/svengijsen/StimulGL\n");
	labWebsite->setText(tmpStr);
	//labWebsite->setOpenExternalLinks(true);//openExternalLinks
	//labWebsite->setTextFormat(Qt::RichText);
	//
	foreach (QString pluginName,pluginFileNames)
		populateTreeWidget(0, tr("%1 (Device Plugin)").arg(pluginName));

}

//! [0]
void aboutQTDialog::findPlugins(const QString &title,const QString &path,
							   const QStringList &fileNames)
{
	const QDir dir(path);

	foreach (QObject *plugin, QPluginLoader::staticInstances())
		populateTreeWidget(plugin, tr("%1 (Static Plugin)")
		.arg(plugin->metaObject()->className()));

	foreach (QString fileName, fileNames) {
		QPluginLoader loader(dir.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		if (plugin)
			populateTreeWidget(plugin, fileName);
	}
}

void aboutQTDialog::populateTreeWidget(QObject *plugin, const QString &text)
{
	QTreeWidgetItem *pluginItem = new QTreeWidgetItem(treeWidget);
	pluginItem->setText(0, text);
	treeWidget->setItemExpanded(pluginItem, true);

	QFont boldFont = pluginItem->font(0);
	boldFont.setBold(true);
	pluginItem->setFont(0, boldFont);

	if (plugin) {
//	DeviceInterface *iDevice = qobject_cast<DeviceInterface *>(plugin);
//	if (iDevice)
	   // addItems(pluginItem, "DeviceInterface", text);

//	//ShapeInterface *iShape = qobject_cast<ShapeInterface *>(plug/in);
//	//if (iShape)
//	//    addItems(pluginItem, "ShapeInterface", iShape->shapes());
//
//	//FilterInterface *iFilter =
//	//        qobject_cast<FilterInterface *>(plugin);
//	//if (iFilter)
//	//    addItems(pluginItem, "FilterInterface", iFilter->filters());
	}
}

void aboutQTDialog::addItems(QTreeWidgetItem *pluginItem,
							const char *interfaceName,
							const QStringList &features)
{
	QTreeWidgetItem *interfaceItem = new QTreeWidgetItem(pluginItem);
	interfaceItem->setText(0, interfaceName);
	interfaceItem->setIcon(0, interfaceIcon);

	foreach (QString feature, features) {
		if (feature.endsWith("..."))
			feature.chop(3);
		QTreeWidgetItem *featureItem = new QTreeWidgetItem(interfaceItem);
		featureItem->setText(0, feature);
		featureItem->setIcon(0, featureIcon);
	}
}
