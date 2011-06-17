/****************************************************************************
**
**
****************************************************************************/

#ifndef DEVICEPLUGIN_H
#define DEVICEPLUGIN_H

#include <QWidget>
#include <QtScript>
#include "plugininterface.h"

QT_BEGIN_NAMESPACE
class QString;
//class QLineEdit;
//class QLabel;
//class QPushButton;
//class QGridLayout;
QT_END_NAMESPACE

//! [0]
class DevicePluginObject : public QWidget
{
    Q_OBJECT

public:
    DevicePluginObject();
    bool ShowGUI();
    //bool GetGUI();
	//QScriptValue ExecuteScript(const QString &scriptcode);
	bool ConfigureScriptEngine(QScriptEngine &eng);

//private slots:
//    //void test();
//
//signals:
//	void DoSignal();

private:
    //void createGUI();
    //bool loadPlugin();

    DeviceInterface *DInterface;
    //QLineEdit *lineEdit;
    //QLabel *label;
    //QPushButton *button;
    //QGridLayout *layout;
};

#endif // DEVICEPLUGIN_H
