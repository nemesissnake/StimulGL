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

aboutQTDialog::aboutQTDialog(const QString &title,const QStringList &pluginFileNames,const QString &path, const QStringList &fileNames, QWidget *parent) : 
	QDialog(parent),
	label(new QLabel),
	treeWidget(new QTreeWidget),
	okButton(new QPushButton(tr("OK")))
{
	treeWidget->setAlternatingRowColors(false);
	treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
	treeWidget->setColumnCount(1);
	treeWidget->header()->hide();

	okButton->setDefault(true);

	connect(okButton, SIGNAL(clicked()), this, SLOT(close()));

	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->setColumnStretch(0, 1);
	mainLayout->setColumnStretch(2, 1);
	mainLayout->addWidget(label, 0, 0, 1, 3);
	mainLayout->addWidget(treeWidget, 1, 0, 1, 3);
	mainLayout->addWidget(okButton, 2, 1);
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
