/********************************************************************************
** Form generated from reading UI file '_extensionname__dialog.ui'
**
** Created: Fri 17. Jun 14:54:46 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI__EXTENSIONNAME__DIALOG_H
#define UI__EXTENSIONNAME__DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTabWidget>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui__extensionname__DialogClass
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QTabWidget *tabWidget;
    QWidget *tab1;
    QPushButton *btnExampleButton;
    QWidget *tab2;
    QWidget *tab;

    void setupUi(QDialog *_extensionname__DialogClass)
    {
        if (_extensionname__DialogClass->objectName().isEmpty())
            _extensionname__DialogClass->setObjectName(QString::fromUtf8("_extensionname__DialogClass"));
        _extensionname__DialogClass->resize(565, 495);
        layoutWidget = new QWidget(_extensionname__DialogClass);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(340, 460, 221, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
#ifndef Q_OS_MAC
        hboxLayout->setSpacing(6);
#endif
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        hboxLayout->setObjectName(QString::fromUtf8("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        hboxLayout->addWidget(cancelButton);

        tabWidget = new QTabWidget(_extensionname__DialogClass);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 565, 451));
        tab1 = new QWidget();
        tab1->setObjectName(QString::fromUtf8("tab1"));
        btnExampleButton = new QPushButton(tab1);
        btnExampleButton->setObjectName(QString::fromUtf8("btnExampleButton"));
        btnExampleButton->setGeometry(QRect(50, 50, 131, 51));
        tabWidget->addTab(tab1, QString());
        tab2 = new QWidget();
        tab2->setObjectName(QString::fromUtf8("tab2"));
        tabWidget->addTab(tab2, QString());
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        tabWidget->addTab(tab, QString());

        retranslateUi(_extensionname__DialogClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(_extensionname__DialogClass);
    } // setupUi

    void retranslateUi(QDialog *_extensionname__DialogClass)
    {
        _extensionname__DialogClass->setWindowTitle(QApplication::translate("_extensionname__DialogClass", "_extensionname__Dialog", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("_extensionname__DialogClass", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("_extensionname__DialogClass", "Cancel", 0, QApplication::UnicodeUTF8));
        btnExampleButton->setText(QApplication::translate("_extensionname__DialogClass", "Example Button", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab1), QApplication::translate("_extensionname__DialogClass", "Tab1", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab2), QApplication::translate("_extensionname__DialogClass", "Tab2", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("_extensionname__DialogClass", "Tab3", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class _extensionname__DialogClass: public Ui__extensionname__DialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI__EXTENSIONNAME__DIALOG_H
