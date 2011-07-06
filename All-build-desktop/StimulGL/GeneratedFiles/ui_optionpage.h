/********************************************************************************
** Form generated from reading UI file 'optionpage.ui'
**
** Created: Fri 17. Jun 14:54:59 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTIONPAGE_H
#define UI_OPTIONPAGE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OptionPageClass
{
public:
    QTabWidget *tabWidget;
    QWidget *tab1;
    QPushButton *btnTest;
    QCheckBox *chkDoNotLoadQTBindings;
    QWidget *tab2;
    QCheckBox *chk_OpenDebOnError;
    QWidget *tab3;
    QLabel *label;
    QRadioButton *rdb_3DRenderer;
    QRadioButton *rdb_3DRenderer_2;
    QRadioButton *rdb_3DRenderer_3;
    QCheckBox *chk_HighAntiAFilter;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *OptionPageClass)
    {
        if (OptionPageClass->objectName().isEmpty())
            OptionPageClass->setObjectName(QString::fromUtf8("OptionPageClass"));
        OptionPageClass->resize(400, 300);
        tabWidget = new QTabWidget(OptionPageClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 400, 251));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        btnTest = new QPushButton(tab1);
        btnTest->setObjectName(QString::fromUtf8("btnTest"));
        btnTest->setGeometry(QRect(10, 200, 91, 23));
        chkDoNotLoadQTBindings = new QCheckBox(tab1);
        chkDoNotLoadQTBindings->setObjectName(QString::fromUtf8("chkDoNotLoadQTBindings"));
        chkDoNotLoadQTBindings->setEnabled(true);
        chkDoNotLoadQTBindings->setGeometry(QRect(10, 20, 241, 19));
        chkDoNotLoadQTBindings->setChecked(false);
        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        chk_OpenDebOnError = new QCheckBox(tab2);
        chk_OpenDebOnError->setObjectName(QString::fromUtf8("chk_OpenDebOnError"));
        chk_OpenDebOnError->setEnabled(false);
        chk_OpenDebOnError->setGeometry(QRect(10, 10, 241, 19));
        chk_OpenDebOnError->setChecked(true);
        tabWidget->addTab(tab2, QString());
        tab3 = new QWidget();
        tab3->setObjectName(QString::fromUtf8("tab3"));
        label = new QLabel(tab3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 71, 16));
        rdb_3DRenderer = new QRadioButton(tab3);
        rdb_3DRenderer->setObjectName(QString::fromUtf8("rdb_3DRenderer"));
        rdb_3DRenderer->setGeometry(QRect(120, 10, 82, 19));
        rdb_3DRenderer->setChecked(true);
        rdb_3DRenderer_2 = new QRadioButton(tab3);
        rdb_3DRenderer_2->setObjectName(QString::fromUtf8("rdb_3DRenderer_2"));
        rdb_3DRenderer_2->setGeometry(QRect(120, 30, 82, 19));
        rdb_3DRenderer_3 = new QRadioButton(tab3);
        rdb_3DRenderer_3->setObjectName(QString::fromUtf8("rdb_3DRenderer_3"));
        rdb_3DRenderer_3->setGeometry(QRect(120, 50, 81, 20));
        chk_HighAntiAFilter = new QCheckBox(tab3);
        chk_HighAntiAFilter->setObjectName(QString::fromUtf8("chk_HighAntiAFilter"));
        chk_HighAntiAFilter->setEnabled(false);
        chk_HighAntiAFilter->setGeometry(QRect(200, 30, 151, 19));
        tabWidget->addTab(tab3, QString());
        layoutWidget = new QWidget(OptionPageClass);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 260, 381, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        hboxLayout->addWidget(buttonBox);


        retranslateUi(OptionPageClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(OptionPageClass);
    } // setupUi

    void retranslateUi(QDialog *OptionPageClass)
    {
        OptionPageClass->setWindowTitle(QApplication::translate("OptionPageClass", "OptionPage", 0, QApplication::UnicodeUTF8));
        btnTest->setText(QApplication::translate("OptionPageClass", "Test", 0, QApplication::UnicodeUTF8));
        chkDoNotLoadQTBindings->setText(QApplication::translate("OptionPageClass", "Do not automatically load QT Script bindings", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("OptionPageClass", "General", 0, QApplication::UnicodeUTF8));
        chk_OpenDebOnError->setText(QApplication::translate("OptionPageClass", "Open External Debugger on Syntax Error", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("OptionPageClass", "Debugging", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("OptionPageClass", "3D Renderer:", 0, QApplication::UnicodeUTF8));
        rdb_3DRenderer->setText(QApplication::translate("OptionPageClass", "Native", 0, QApplication::UnicodeUTF8));
        rdb_3DRenderer_2->setText(QApplication::translate("OptionPageClass", "OpenGL", 0, QApplication::UnicodeUTF8));
        rdb_3DRenderer_3->setText(QApplication::translate("OptionPageClass", "Image", 0, QApplication::UnicodeUTF8));
        chk_HighAntiAFilter->setText(QApplication::translate("OptionPageClass", "High Quality Antialiasing", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab3), QApplication::translate("OptionPageClass", "Renderer", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OptionPageClass: public Ui_OptionPageClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTIONPAGE_H
