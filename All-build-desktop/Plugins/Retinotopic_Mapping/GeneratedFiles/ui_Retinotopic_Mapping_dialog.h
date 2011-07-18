/********************************************************************************
** Form generated from reading UI file 'Retinotopic_Mapping_dialog.ui'
**
** Created: Fri 17. Jun 14:54:26 2011
**      by: Qt User Interface Compiler version 4.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RETINOTOPIC_MAPPING_DIALOG_H
#define UI_RETINOTOPIC_MAPPING_DIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QDialog>
#include <QtGui/QDockWidget>
#include <QtGui/QFormLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QSpinBox>
#include <QtGui/QTabWidget>
#include <QtGui/QTableWidget>
#include <QtGui/QToolButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Retinotopic_Mapping_DialogClass
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QTabWidget *mainTabWidget;
    QWidget *experimentTab;
    QVBoxLayout *verticalLayout;
    QFormLayout *formLayout_2;
    QLabel *experimentsFolderLbl;
    QHBoxLayout *horizontalLayout_2;
    QLineEdit *experimentsFolderEd;
    QToolButton *BrowseExpFolderBtn;
    QLabel *experimentsLbl;
    QComboBox *ExperimentCmb;
    QLabel *ExperimentTypeLbl;
    QComboBox *ExpTypeCmb;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_4;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QFormLayout *formLayout;
    QLabel *SubjectNrLbl;
    QHBoxLayout *horizontalLayout_3;
    QSpinBox *subjectNrSpBx;
    QSpacerItem *horizontalSpacer_4;
    QLabel *label_2;
    QLineEdit *subjectNameEd;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *horizontalSpacer_3;
    QWidget *MRITab;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_5;
    QLabel *TRLbl;
    QSpinBox *TRSpBx;
    QLabel *TRUnitLbl;
    QSpacerItem *horizontalSpacer1Tab2;
    QCheckBox *SimTriggerToggle;
    QHBoxLayout *horizontalLayout_10;
    QCheckBox *USBTriggerToggle;
    QSpacerItem *horizontalSpacer_6;
    QLabel *USBCharLbl;
    QLineEdit *USBCharEdit;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *COMTriggerOp;
    QSpacerItem *horizontalSpacer_7;
    QLabel *COMAddLbl;
    QComboBox *COMSelectCmb;
    QSpacerItem *verticalSpacer_5;
    QDockWidget *expContentDkWdg;
    QWidget *dockWidgetContents;
    QVBoxLayout *verticalLayout_2;
    QTableWidget *ExpContentTableWdg;
    QHBoxLayout *horizontalLayout_6;
    QLabel *SumOfTRsLbl;
    QLineEdit *NrOfTRsEd;
    QLabel *NrOfTRsUnitLbl;
    QHBoxLayout *horizontalLayout_7;
    QToolButton *AddBTBtn;
    QToolButton *RemoveBTBtn;
    QSpacerItem *horizontalSpacer_5;
    QToolButton *SaveEXPFileBtn;
    QWidget *layoutWidget_2;
    QHBoxLayout *horizontalLayout;
    QCheckBox *FullScreenToggle;
    QCheckBox *DebugInfoToggle;
    QSpacerItem *horizontalSpacer;
    QPushButton *GoBtn;
    QDockWidget *blockTrialContentDkWdg;
    QWidget *dockWidgetContents_2;
    QHBoxLayout *horizontalLayout_9;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *blockTrialContentTbWdg;
    QHBoxLayout *horizontalLayout_8;
    QToolButton *AddTrialBtn;
    QToolButton *RemoveTrialBtn;
    QSpacerItem *horizontalSpacer100;
    QToolButton *SaveBlockTrialFileBtn;
    QGroupBox *BlockTrialInfoGrpBx;
    QFormLayout *formLayout_3;
    QLabel *conditionTypeLbl;
    QLineEdit *conditionTypeEd;
    QLabel *nrOfParamsLbl;
    QLabel *nameOfParametersLbl;
    QLabel *typeOfParamsLbl;
    QLineEdit *typeOfParamsEd;
    QLabel *InfoTextLbl;
    QPlainTextEdit *InfoTxEd;
    QSpinBox *nrOfParamsSpBx;
    QComboBox *nameOfParamsCmBx;

    void setupUi(QDialog *Retinotopic_Mapping_DialogClass)
    {
        if (Retinotopic_Mapping_DialogClass->objectName().isEmpty())
            Retinotopic_Mapping_DialogClass->setObjectName(QString::fromUtf8("Retinotopic_Mapping_DialogClass"));
        Retinotopic_Mapping_DialogClass->resize(629, 690);
        layoutWidget = new QWidget(Retinotopic_Mapping_DialogClass);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(400, 650, 221, 33));
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

        mainTabWidget = new QTabWidget(Retinotopic_Mapping_DialogClass);
        mainTabWidget->setObjectName(QString::fromUtf8("mainTabWidget"));
        mainTabWidget->setGeometry(QRect(0, 10, 357, 220));
        mainTabWidget->setMinimumSize(QSize(0, 220));
        experimentTab = new QWidget();
        experimentTab->setObjectName(QString::fromUtf8("experimentTab"));
        verticalLayout = new QVBoxLayout(experimentTab);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        formLayout_2 = new QFormLayout();
        formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
        formLayout_2->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
        experimentsFolderLbl = new QLabel(experimentTab);
        experimentsFolderLbl->setObjectName(QString::fromUtf8("experimentsFolderLbl"));

        formLayout_2->setWidget(0, QFormLayout::LabelRole, experimentsFolderLbl);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        experimentsFolderEd = new QLineEdit(experimentTab);
        experimentsFolderEd->setObjectName(QString::fromUtf8("experimentsFolderEd"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(experimentsFolderEd->sizePolicy().hasHeightForWidth());
        experimentsFolderEd->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(experimentsFolderEd);

        BrowseExpFolderBtn = new QToolButton(experimentTab);
        BrowseExpFolderBtn->setObjectName(QString::fromUtf8("BrowseExpFolderBtn"));

        horizontalLayout_2->addWidget(BrowseExpFolderBtn);


        formLayout_2->setLayout(0, QFormLayout::FieldRole, horizontalLayout_2);

        experimentsLbl = new QLabel(experimentTab);
        experimentsLbl->setObjectName(QString::fromUtf8("experimentsLbl"));

        formLayout_2->setWidget(2, QFormLayout::LabelRole, experimentsLbl);

        ExperimentCmb = new QComboBox(experimentTab);
        ExperimentCmb->setObjectName(QString::fromUtf8("ExperimentCmb"));
        sizePolicy.setHeightForWidth(ExperimentCmb->sizePolicy().hasHeightForWidth());
        ExperimentCmb->setSizePolicy(sizePolicy);
        ExperimentCmb->setMinimumSize(QSize(120, 0));

        formLayout_2->setWidget(2, QFormLayout::FieldRole, ExperimentCmb);

        ExperimentTypeLbl = new QLabel(experimentTab);
        ExperimentTypeLbl->setObjectName(QString::fromUtf8("ExperimentTypeLbl"));

        formLayout_2->setWidget(4, QFormLayout::LabelRole, ExperimentTypeLbl);

        ExpTypeCmb = new QComboBox(experimentTab);
        ExpTypeCmb->setObjectName(QString::fromUtf8("ExpTypeCmb"));
        ExpTypeCmb->setInsertPolicy(QComboBox::InsertAfterCurrent);

        formLayout_2->setWidget(4, QFormLayout::FieldRole, ExpTypeCmb);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout_2->setItem(1, QFormLayout::LabelRole, verticalSpacer);

        verticalSpacer_2 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout_2->setItem(3, QFormLayout::FieldRole, verticalSpacer_2);


        verticalLayout->addLayout(formLayout_2);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_4);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        formLayout = new QFormLayout();
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        SubjectNrLbl = new QLabel(experimentTab);
        SubjectNrLbl->setObjectName(QString::fromUtf8("SubjectNrLbl"));

        formLayout->setWidget(0, QFormLayout::LabelRole, SubjectNrLbl);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        subjectNrSpBx = new QSpinBox(experimentTab);
        subjectNrSpBx->setObjectName(QString::fromUtf8("subjectNrSpBx"));
        subjectNrSpBx->setMinimumSize(QSize(60, 0));
        subjectNrSpBx->setMinimum(1);
        subjectNrSpBx->setMaximum(9999);

        horizontalLayout_3->addWidget(subjectNrSpBx);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_4);


        formLayout->setLayout(0, QFormLayout::FieldRole, horizontalLayout_3);

        label_2 = new QLabel(experimentTab);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        subjectNameEd = new QLineEdit(experimentTab);
        subjectNameEd->setObjectName(QString::fromUtf8("subjectNameEd"));

        formLayout->setWidget(2, QFormLayout::FieldRole, subjectNameEd);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);

        formLayout->setItem(1, QFormLayout::FieldRole, verticalSpacer_3);


        horizontalLayout_4->addLayout(formLayout);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);

        mainTabWidget->addTab(experimentTab, QString());
        MRITab = new QWidget();
        MRITab->setObjectName(QString::fromUtf8("MRITab"));
        verticalLayout_5 = new QVBoxLayout(MRITab);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        TRLbl = new QLabel(MRITab);
        TRLbl->setObjectName(QString::fromUtf8("TRLbl"));

        horizontalLayout_5->addWidget(TRLbl);

        TRSpBx = new QSpinBox(MRITab);
        TRSpBx->setObjectName(QString::fromUtf8("TRSpBx"));
        TRSpBx->setMinimumSize(QSize(60, 0));
        TRSpBx->setMaximum(99999);
        TRSpBx->setValue(2000);

        horizontalLayout_5->addWidget(TRSpBx);

        TRUnitLbl = new QLabel(MRITab);
        TRUnitLbl->setObjectName(QString::fromUtf8("TRUnitLbl"));

        horizontalLayout_5->addWidget(TRUnitLbl);

        horizontalSpacer1Tab2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer1Tab2);

        SimTriggerToggle = new QCheckBox(MRITab);
        SimTriggerToggle->setObjectName(QString::fromUtf8("SimTriggerToggle"));
        SimTriggerToggle->setEnabled(false);
        SimTriggerToggle->setChecked(true);

        horizontalLayout_5->addWidget(SimTriggerToggle);


        verticalLayout_5->addLayout(horizontalLayout_5);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        USBTriggerToggle = new QCheckBox(MRITab);
        USBTriggerToggle->setObjectName(QString::fromUtf8("USBTriggerToggle"));
        USBTriggerToggle->setEnabled(false);

        horizontalLayout_10->addWidget(USBTriggerToggle);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_10->addItem(horizontalSpacer_6);

        USBCharLbl = new QLabel(MRITab);
        USBCharLbl->setObjectName(QString::fromUtf8("USBCharLbl"));

        horizontalLayout_10->addWidget(USBCharLbl);

        USBCharEdit = new QLineEdit(MRITab);
        USBCharEdit->setObjectName(QString::fromUtf8("USBCharEdit"));
        USBCharEdit->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(USBCharEdit->sizePolicy().hasHeightForWidth());
        USBCharEdit->setSizePolicy(sizePolicy1);
        USBCharEdit->setMaxLength(1);

        horizontalLayout_10->addWidget(USBCharEdit);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        COMTriggerOp = new QCheckBox(MRITab);
        COMTriggerOp->setObjectName(QString::fromUtf8("COMTriggerOp"));
        COMTriggerOp->setEnabled(false);

        horizontalLayout_11->addWidget(COMTriggerOp);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_7);

        COMAddLbl = new QLabel(MRITab);
        COMAddLbl->setObjectName(QString::fromUtf8("COMAddLbl"));

        horizontalLayout_11->addWidget(COMAddLbl);

        COMSelectCmb = new QComboBox(MRITab);
        COMSelectCmb->setObjectName(QString::fromUtf8("COMSelectCmb"));
        COMSelectCmb->setEnabled(false);

        horizontalLayout_11->addWidget(COMSelectCmb);


        verticalLayout_5->addLayout(horizontalLayout_11);

        verticalSpacer_5 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_5->addItem(verticalSpacer_5);

        mainTabWidget->addTab(MRITab, QString());
        expContentDkWdg = new QDockWidget(Retinotopic_Mapping_DialogClass);
        expContentDkWdg->setObjectName(QString::fromUtf8("expContentDkWdg"));
        expContentDkWdg->setGeometry(QRect(380, 10, 244, 271));
        expContentDkWdg->setMinimumSize(QSize(230, 191));
        expContentDkWdg->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
        verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(3, 3, -1, -1);
        ExpContentTableWdg = new QTableWidget(dockWidgetContents);
        if (ExpContentTableWdg->columnCount() < 2)
            ExpContentTableWdg->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        ExpContentTableWdg->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        ExpContentTableWdg->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        ExpContentTableWdg->setObjectName(QString::fromUtf8("ExpContentTableWdg"));

        verticalLayout_2->addWidget(ExpContentTableWdg);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        SumOfTRsLbl = new QLabel(dockWidgetContents);
        SumOfTRsLbl->setObjectName(QString::fromUtf8("SumOfTRsLbl"));

        horizontalLayout_6->addWidget(SumOfTRsLbl);

        NrOfTRsEd = new QLineEdit(dockWidgetContents);
        NrOfTRsEd->setObjectName(QString::fromUtf8("NrOfTRsEd"));
        NrOfTRsEd->setReadOnly(true);

        horizontalLayout_6->addWidget(NrOfTRsEd);

        NrOfTRsUnitLbl = new QLabel(dockWidgetContents);
        NrOfTRsUnitLbl->setObjectName(QString::fromUtf8("NrOfTRsUnitLbl"));

        horizontalLayout_6->addWidget(NrOfTRsUnitLbl);


        verticalLayout_2->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        AddBTBtn = new QToolButton(dockWidgetContents);
        AddBTBtn->setObjectName(QString::fromUtf8("AddBTBtn"));

        horizontalLayout_7->addWidget(AddBTBtn);

        RemoveBTBtn = new QToolButton(dockWidgetContents);
        RemoveBTBtn->setObjectName(QString::fromUtf8("RemoveBTBtn"));

        horizontalLayout_7->addWidget(RemoveBTBtn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_7->addItem(horizontalSpacer_5);

        SaveEXPFileBtn = new QToolButton(dockWidgetContents);
        SaveEXPFileBtn->setObjectName(QString::fromUtf8("SaveEXPFileBtn"));

        horizontalLayout_7->addWidget(SaveEXPFileBtn);


        verticalLayout_2->addLayout(horizontalLayout_7);

        expContentDkWdg->setWidget(dockWidgetContents);
        layoutWidget_2 = new QWidget(Retinotopic_Mapping_DialogClass);
        layoutWidget_2->setObjectName(QString::fromUtf8("layoutWidget_2"));
        layoutWidget_2->setGeometry(QRect(10, 600, 601, 27));
        horizontalLayout = new QHBoxLayout(layoutWidget_2);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        FullScreenToggle = new QCheckBox(layoutWidget_2);
        FullScreenToggle->setObjectName(QString::fromUtf8("FullScreenToggle"));

        horizontalLayout->addWidget(FullScreenToggle);

        DebugInfoToggle = new QCheckBox(layoutWidget_2);
        DebugInfoToggle->setObjectName(QString::fromUtf8("DebugInfoToggle"));

        horizontalLayout->addWidget(DebugInfoToggle);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        GoBtn = new QPushButton(layoutWidget_2);
        GoBtn->setObjectName(QString::fromUtf8("GoBtn"));
        GoBtn->setAutoDefault(true);
        GoBtn->setDefault(true);

        horizontalLayout->addWidget(GoBtn);

        blockTrialContentDkWdg = new QDockWidget(Retinotopic_Mapping_DialogClass);
        blockTrialContentDkWdg->setObjectName(QString::fromUtf8("blockTrialContentDkWdg"));
        blockTrialContentDkWdg->setGeometry(QRect(0, 290, 625, 309));
        blockTrialContentDkWdg->setMinimumSize(QSize(625, 296));
        blockTrialContentDkWdg->setAllowedAreas(Qt::BottomDockWidgetArea);
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName(QString::fromUtf8("dockWidgetContents_2"));
        horizontalLayout_9 = new QHBoxLayout(dockWidgetContents_2);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        blockTrialContentTbWdg = new QTableWidget(dockWidgetContents_2);
        blockTrialContentTbWdg->setObjectName(QString::fromUtf8("blockTrialContentTbWdg"));
        blockTrialContentTbWdg->setMinimumSize(QSize(300, 0));

        verticalLayout_3->addWidget(blockTrialContentTbWdg);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        AddTrialBtn = new QToolButton(dockWidgetContents_2);
        AddTrialBtn->setObjectName(QString::fromUtf8("AddTrialBtn"));

        horizontalLayout_8->addWidget(AddTrialBtn);

        RemoveTrialBtn = new QToolButton(dockWidgetContents_2);
        RemoveTrialBtn->setObjectName(QString::fromUtf8("RemoveTrialBtn"));

        horizontalLayout_8->addWidget(RemoveTrialBtn);

        horizontalSpacer100 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer100);

        SaveBlockTrialFileBtn = new QToolButton(dockWidgetContents_2);
        SaveBlockTrialFileBtn->setObjectName(QString::fromUtf8("SaveBlockTrialFileBtn"));

        horizontalLayout_8->addWidget(SaveBlockTrialFileBtn);


        verticalLayout_3->addLayout(horizontalLayout_8);


        horizontalLayout_9->addLayout(verticalLayout_3);

        BlockTrialInfoGrpBx = new QGroupBox(dockWidgetContents_2);
        BlockTrialInfoGrpBx->setObjectName(QString::fromUtf8("BlockTrialInfoGrpBx"));
        formLayout_3 = new QFormLayout(BlockTrialInfoGrpBx);
        formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
        conditionTypeLbl = new QLabel(BlockTrialInfoGrpBx);
        conditionTypeLbl->setObjectName(QString::fromUtf8("conditionTypeLbl"));

        formLayout_3->setWidget(0, QFormLayout::LabelRole, conditionTypeLbl);

        conditionTypeEd = new QLineEdit(BlockTrialInfoGrpBx);
        conditionTypeEd->setObjectName(QString::fromUtf8("conditionTypeEd"));

        formLayout_3->setWidget(0, QFormLayout::FieldRole, conditionTypeEd);

        nrOfParamsLbl = new QLabel(BlockTrialInfoGrpBx);
        nrOfParamsLbl->setObjectName(QString::fromUtf8("nrOfParamsLbl"));

        formLayout_3->setWidget(1, QFormLayout::LabelRole, nrOfParamsLbl);

        nameOfParametersLbl = new QLabel(BlockTrialInfoGrpBx);
        nameOfParametersLbl->setObjectName(QString::fromUtf8("nameOfParametersLbl"));

        formLayout_3->setWidget(2, QFormLayout::LabelRole, nameOfParametersLbl);

        typeOfParamsLbl = new QLabel(BlockTrialInfoGrpBx);
        typeOfParamsLbl->setObjectName(QString::fromUtf8("typeOfParamsLbl"));

        formLayout_3->setWidget(3, QFormLayout::LabelRole, typeOfParamsLbl);

        typeOfParamsEd = new QLineEdit(BlockTrialInfoGrpBx);
        typeOfParamsEd->setObjectName(QString::fromUtf8("typeOfParamsEd"));

        formLayout_3->setWidget(3, QFormLayout::FieldRole, typeOfParamsEd);

        InfoTextLbl = new QLabel(BlockTrialInfoGrpBx);
        InfoTextLbl->setObjectName(QString::fromUtf8("InfoTextLbl"));

        formLayout_3->setWidget(4, QFormLayout::LabelRole, InfoTextLbl);

        InfoTxEd = new QPlainTextEdit(BlockTrialInfoGrpBx);
        InfoTxEd->setObjectName(QString::fromUtf8("InfoTxEd"));

        formLayout_3->setWidget(4, QFormLayout::FieldRole, InfoTxEd);

        nrOfParamsSpBx = new QSpinBox(BlockTrialInfoGrpBx);
        nrOfParamsSpBx->setObjectName(QString::fromUtf8("nrOfParamsSpBx"));

        formLayout_3->setWidget(1, QFormLayout::FieldRole, nrOfParamsSpBx);

        nameOfParamsCmBx = new QComboBox(BlockTrialInfoGrpBx);
        nameOfParamsCmBx->setObjectName(QString::fromUtf8("nameOfParamsCmBx"));

        formLayout_3->setWidget(2, QFormLayout::FieldRole, nameOfParamsCmBx);


        horizontalLayout_9->addWidget(BlockTrialInfoGrpBx);

        blockTrialContentDkWdg->setWidget(dockWidgetContents_2);

        retranslateUi(Retinotopic_Mapping_DialogClass);
        QObject::connect(USBTriggerToggle, SIGNAL(toggled(bool)), SimTriggerToggle, SLOT(toggle()));
        QObject::connect(COMTriggerOp, SIGNAL(toggled(bool)), SimTriggerToggle, SLOT(toggle()));
        QObject::connect(COMTriggerOp, SIGNAL(toggled(bool)), USBTriggerToggle, SLOT(setDisabled(bool)));
        QObject::connect(USBTriggerToggle, SIGNAL(toggled(bool)), COMTriggerOp, SLOT(setDisabled(bool)));
        QObject::connect(USBTriggerToggle, SIGNAL(toggled(bool)), COMSelectCmb, SLOT(setDisabled(bool)));
        QObject::connect(USBTriggerToggle, SIGNAL(toggled(bool)), COMAddLbl, SLOT(setDisabled(bool)));
        QObject::connect(COMTriggerOp, SIGNAL(toggled(bool)), USBCharLbl, SLOT(setDisabled(bool)));

        mainTabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Retinotopic_Mapping_DialogClass);
    } // setupUi

    void retranslateUi(QDialog *Retinotopic_Mapping_DialogClass)
    {
        Retinotopic_Mapping_DialogClass->setWindowTitle(QApplication::translate("Retinotopic_Mapping_DialogClass", "Retinotopic_Mapping_Dialog", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Cancel", 0, QApplication::UnicodeUTF8));
        experimentsFolderLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Experiments folder:", 0, QApplication::UnicodeUTF8));
        BrowseExpFolderBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "...", 0, QApplication::UnicodeUTF8));
        experimentsLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Experiments:", 0, QApplication::UnicodeUTF8));
        ExperimentTypeLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Experiment Type:", 0, QApplication::UnicodeUTF8));
        ExpTypeCmb->clear();
        ExpTypeCmb->insertItems(0, QStringList()
         << QApplication::translate("Retinotopic_Mapping_DialogClass", "Eccentricity", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Retinotopic_Mapping_DialogClass", "Polarangle", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Retinotopic_Mapping_DialogClass", "Horizontal meridians", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Retinotopic_Mapping_DialogClass", "Vertical meridians", 0, QApplication::UnicodeUTF8)
        );
        SubjectNrLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Subject No.:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Subject Name", 0, QApplication::UnicodeUTF8));
        mainTabWidget->setTabText(mainTabWidget->indexOf(experimentTab), QApplication::translate("Retinotopic_Mapping_DialogClass", "Experiment", 0, QApplication::UnicodeUTF8));
        TRLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "TR:", 0, QApplication::UnicodeUTF8));
        TRUnitLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "ms", 0, QApplication::UnicodeUTF8));
        SimTriggerToggle->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Simulate Trigger", 0, QApplication::UnicodeUTF8));
        USBTriggerToggle->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "USB Trigger", 0, QApplication::UnicodeUTF8));
        USBCharLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Character:", 0, QApplication::UnicodeUTF8));
        USBCharEdit->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "5", 0, QApplication::UnicodeUTF8));
        COMTriggerOp->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "COM Trigger", 0, QApplication::UnicodeUTF8));
        COMAddLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Register:", 0, QApplication::UnicodeUTF8));
        COMSelectCmb->clear();
        COMSelectCmb->insertItems(0, QStringList()
         << QApplication::translate("Retinotopic_Mapping_DialogClass", "0x378", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("Retinotopic_Mapping_DialogClass", "0x278", 0, QApplication::UnicodeUTF8)
        );
        mainTabWidget->setTabText(mainTabWidget->indexOf(MRITab), QApplication::translate("Retinotopic_Mapping_DialogClass", "MRI", 0, QApplication::UnicodeUTF8));
        expContentDkWdg->setWindowTitle(QApplication::translate("Retinotopic_Mapping_DialogClass", " Experiment Content", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem = ExpContentTableWdg->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Block/Trial", 0, QApplication::UnicodeUTF8));
        QTableWidgetItem *___qtablewidgetitem1 = ExpContentTableWdg->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Duration [TR]", 0, QApplication::UnicodeUTF8));
        SumOfTRsLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Experiment duration:", 0, QApplication::UnicodeUTF8));
        NrOfTRsUnitLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "TRs  ", 0, QApplication::UnicodeUTF8));
        AddBTBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "+", 0, QApplication::UnicodeUTF8));
        RemoveBTBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "-", 0, QApplication::UnicodeUTF8));
        SaveEXPFileBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Save", 0, QApplication::UnicodeUTF8));
        FullScreenToggle->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Full Screen", 0, QApplication::UnicodeUTF8));
        DebugInfoToggle->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Info (test mode)", 0, QApplication::UnicodeUTF8));
        GoBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "GO", 0, QApplication::UnicodeUTF8));
        AddTrialBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "+", 0, QApplication::UnicodeUTF8));
        RemoveTrialBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "-", 0, QApplication::UnicodeUTF8));
        SaveBlockTrialFileBtn->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Save", 0, QApplication::UnicodeUTF8));
        BlockTrialInfoGrpBx->setTitle(QApplication::translate("Retinotopic_Mapping_DialogClass", "Info", 0, QApplication::UnicodeUTF8));
        conditionTypeLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "ConditionType:", 0, QApplication::UnicodeUTF8));
        nrOfParamsLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "No. of parameters:", 0, QApplication::UnicodeUTF8));
        nameOfParametersLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Names of parameters:", 0, QApplication::UnicodeUTF8));
        typeOfParamsLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Type of parameters:", 0, QApplication::UnicodeUTF8));
        InfoTextLbl->setText(QApplication::translate("Retinotopic_Mapping_DialogClass", "Info:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Retinotopic_Mapping_DialogClass: public Ui_Retinotopic_Mapping_DialogClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RETINOTOPIC_MAPPING_DIALOG_H
