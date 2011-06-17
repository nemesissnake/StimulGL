/****************************************************************************
**
**
****************************************************************************/

#include <QtGui>
#include "deviceplugin.h"

DevicePluginObject::DevicePluginObject()
{
    //createGUI();
//    setLayout(layout);
    //setWindowTitle("Device Plugin Example");

    //if (!loadPlugin()) {
    //    QMessageBox::information(this, "Error", "Could not load the device plugin");
    //    lineEdit->setEnabled(false);
    //    button->setEnabled(false);
    //}
}

bool DevicePluginObject::ConfigureScriptEngine(QScriptEngine &eng)
{
	return DInterface->ConfigureScriptEngine(eng);
}

//void DevicePluginObject::test()
//{
	//QObject::connect(&a, SIGNAL(valueChanged(int)), &b, SLOT(setValue(int)));
	//QObject::connectSIGNAL(DInterface->GetGUI("");

    //QString text = DInterface->test(lineEdit->text());
    //QString text = "aa";
    //label->setText(text);
//}

bool DevicePluginObject::ShowGUI()
{
    //DInterface->ShowGUI("default");
    return true;
}

//void DevicePluginObject::createGUI()
//{
//    lineEdit = new QLineEdit;
//    label = new QLabel;
//    label->setFrameStyle(QFrame::Box | QFrame::Plain);
//    button = new QPushButton(tr("Send Message"));
//
//    connect(lineEdit, SIGNAL(editingFinished()),
//            this, SLOT(test()));
//    connect(button, SIGNAL(clicked()),
//            this, SLOT(test()));
//
//    layout = new QGridLayout;
//    layout->addWidget(new QLabel(tr("Message:")), 0, 0);
//    layout->addWidget(lineEdit, 0, 1);
//    layout->addWidget(new QLabel(tr("Answer:")), 1, 0);
//    layout->addWidget(label, 1, 1);
//    layout->addWidget(button, 2, 1, Qt::AlignRight);
//    layout->setSizeConstraint(QLayout::SetFixedSize);
//}

//bool DevicePluginObject::loadPlugin()
//{
//    QDir pluginsDir(qApp->applationDrPth());
//#if defined(Q_OS_WIN)
//    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
//        pluginsDir.cdUp();
//#elif defined(Q_OS_MAC)
//    if (pluginsDir.dirName() == "MacOS") {
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//    }
//#endif
//    pluginsDir.cd("plugins");
//    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
//        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
//        QObject *plugin = pluginLoader.instance();
//        if (plugin) {
//            DInterface = qobject_cast<DeviceInterface *>(plugin);
//            if (DeviceInterface)
//                return true;
//        }
//    }
//
//    return false;
//}
