/********************************************************************************
** Form generated from reading UI file 'scifinddialog.ui'
**
** Created: Fri 17. Jun 14:54:59 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCIFINDDIALOG_H
#define UI_SCIFINDDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_sciFindDialogClass
{
public:
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label;
    QHBoxLayout *horizontalLayout;
    QComboBox *findCmb;
    QTextEdit *mlEd;
    QVBoxLayout *verticalLayout_2;
    QPushButton *findBtn;
    QPushButton *cancelBtn;
    QSpacerItem *verticalSpacer;
    QCheckBox *replaceChk;
    QComboBox *replaceCmb;
    QCheckBox *matchCaseChk;
    QCheckBox *backwardChk;
    QCheckBox *wholeWordsChk;
    QCheckBox *regexpChk;
    QCheckBox *multiLineChk;
    QSpacerItem *lowerSpacer;

    void setupUi(QWidget *sciFindDialogClass)
    {
        if (sciFindDialogClass->objectName().isEmpty())
            sciFindDialogClass->setObjectName(QString::fromUtf8("sciFindDialogClass"));
        sciFindDialogClass->resize(667, 355);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(sciFindDialogClass->sizePolicy().hasHeightForWidth());
        sciFindDialogClass->setSizePolicy(sizePolicy);
        sciFindDialogClass->setMaximumSize(QSize(16777215, 400));
        verticalLayout = new QVBoxLayout(sciFindDialogClass);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(sciFindDialogClass);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetMinimumSize);
        findCmb = new QComboBox(sciFindDialogClass);
        findCmb->setObjectName(QString::fromUtf8("findCmb"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(findCmb->sizePolicy().hasHeightForWidth());
        findCmb->setSizePolicy(sizePolicy1);
        findCmb->setMinimumSize(QSize(200, 0));
        findCmb->setEditable(true);
        findCmb->setInsertPolicy(QComboBox::InsertAtBottom);

        horizontalLayout->addWidget(findCmb);

        mlEd = new QTextEdit(sciFindDialogClass);
        mlEd->setObjectName(QString::fromUtf8("mlEd"));
        mlEd->setEnabled(true);
        mlEd->setBaseSize(QSize(0, 600));
        mlEd->setTabChangesFocus(true);
        mlEd->setAcceptRichText(false);

        horizontalLayout->addWidget(mlEd);


        gridLayout->addLayout(horizontalLayout, 0, 1, 1, 1);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        findBtn = new QPushButton(sciFindDialogClass);
        findBtn->setObjectName(QString::fromUtf8("findBtn"));
        findBtn->setMinimumSize(QSize(120, 0));
        findBtn->setDefault(true);

        verticalLayout_2->addWidget(findBtn);

        cancelBtn = new QPushButton(sciFindDialogClass);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));

        verticalLayout_2->addWidget(cancelBtn);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        gridLayout->addLayout(verticalLayout_2, 0, 2, 7, 1);

        replaceChk = new QCheckBox(sciFindDialogClass);
        replaceChk->setObjectName(QString::fromUtf8("replaceChk"));
        replaceChk->setChecked(false);

        gridLayout->addWidget(replaceChk, 1, 0, 1, 1);

        replaceCmb = new QComboBox(sciFindDialogClass);
        replaceCmb->setObjectName(QString::fromUtf8("replaceCmb"));
        replaceCmb->setEnabled(false);
        sizePolicy1.setHeightForWidth(replaceCmb->sizePolicy().hasHeightForWidth());
        replaceCmb->setSizePolicy(sizePolicy1);
        replaceCmb->setEditable(true);

        gridLayout->addWidget(replaceCmb, 1, 1, 1, 1);

        matchCaseChk = new QCheckBox(sciFindDialogClass);
        matchCaseChk->setObjectName(QString::fromUtf8("matchCaseChk"));

        gridLayout->addWidget(matchCaseChk, 2, 1, 1, 1);

        backwardChk = new QCheckBox(sciFindDialogClass);
        backwardChk->setObjectName(QString::fromUtf8("backwardChk"));

        gridLayout->addWidget(backwardChk, 3, 1, 1, 1);

        wholeWordsChk = new QCheckBox(sciFindDialogClass);
        wholeWordsChk->setObjectName(QString::fromUtf8("wholeWordsChk"));

        gridLayout->addWidget(wholeWordsChk, 4, 1, 1, 1);

        regexpChk = new QCheckBox(sciFindDialogClass);
        regexpChk->setObjectName(QString::fromUtf8("regexpChk"));

        gridLayout->addWidget(regexpChk, 5, 1, 1, 1);

        multiLineChk = new QCheckBox(sciFindDialogClass);
        multiLineChk->setObjectName(QString::fromUtf8("multiLineChk"));
        multiLineChk->setEnabled(false);

        gridLayout->addWidget(multiLineChk, 6, 1, 1, 1);


        verticalLayout->addLayout(gridLayout);

        lowerSpacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(lowerSpacer);

#ifndef QT_NO_SHORTCUT
        label->setBuddy(findCmb);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(findCmb, replaceChk);
        QWidget::setTabOrder(replaceChk, replaceCmb);
        QWidget::setTabOrder(replaceCmb, matchCaseChk);
        QWidget::setTabOrder(matchCaseChk, backwardChk);
        QWidget::setTabOrder(backwardChk, wholeWordsChk);
        QWidget::setTabOrder(wholeWordsChk, regexpChk);
        QWidget::setTabOrder(regexpChk, multiLineChk);
        QWidget::setTabOrder(multiLineChk, findBtn);

        retranslateUi(sciFindDialogClass);
        QObject::connect(replaceChk, SIGNAL(toggled(bool)), replaceCmb, SLOT(setEnabled(bool)));
        QObject::connect(regexpChk, SIGNAL(toggled(bool)), multiLineChk, SLOT(setEnabled(bool)));
        QObject::connect(multiLineChk, SIGNAL(toggled(bool)), findCmb, SLOT(setHidden(bool)));
        QObject::connect(multiLineChk, SIGNAL(toggled(bool)), mlEd, SLOT(setVisible(bool)));

        QMetaObject::connectSlotsByName(sciFindDialogClass);
    } // setupUi

    void retranslateUi(QWidget *sciFindDialogClass)
    {
        sciFindDialogClass->setWindowTitle(QApplication::translate("sciFindDialogClass", "sciFindDialog", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("sciFindDialogClass", "&Find", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        mlEd->setToolTip(QString());
#endif // QT_NO_TOOLTIP
        findBtn->setText(QApplication::translate("sciFindDialogClass", "Find", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("sciFindDialogClass", "Cancel", 0, QApplication::UnicodeUTF8));
        replaceChk->setText(QApplication::translate("sciFindDialogClass", "&Replace", 0, QApplication::UnicodeUTF8));
        matchCaseChk->setText(QApplication::translate("sciFindDialogClass", "Match &case", 0, QApplication::UnicodeUTF8));
        backwardChk->setText(QApplication::translate("sciFindDialogClass", "&Backward", 0, QApplication::UnicodeUTF8));
        wholeWordsChk->setText(QApplication::translate("sciFindDialogClass", "Whole &words only", 0, QApplication::UnicodeUTF8));
        regexpChk->setText(QApplication::translate("sciFindDialogClass", "Regular &expression", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        multiLineChk->setToolTip(QApplication::translate("sciFindDialogClass", "<p>If <b>checked</b>, all the text is treated like a single line. Symbols '^' and '$' are at the beginning and at the end of the document. In this mode you can search for multiline regexps using '\\r' and '\\n' symbols for line ends.</p>\n"
"\n"
"<p>If <b>not checked</b>, the regexp is applied to each line separately and each line has its own symbols '^' and '$' (at the beginning and at the end of the line).</p>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        multiLineChk->setText(QApplication::translate("sciFindDialogClass", "&Multi-line", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class sciFindDialogClass: public Ui_sciFindDialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCIFINDDIALOG_H
