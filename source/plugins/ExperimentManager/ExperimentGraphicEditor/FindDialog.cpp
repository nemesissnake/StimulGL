#include "FindDialog.h"
#include "ui_FindDialog.h"

FindDialog::FindDialog(QWidget *parent) : QWidget(parent), ui(new Ui::FindDialog)
{
    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    connect(ui->findButton, SIGNAL(clicked()), this, SLOT(findSlot()));
    connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::findSlot()
{
    QStringList filters;

    if (ui->cbTags->isChecked())
        filters.append("TAGS");
    if (ui->cbValues->isChecked())
        filters.append("VALUES");
    if (ui->cbAttributes->isChecked())
        filters.append("ATTRIBUTES");
    if (ui->cbCaseSensitive->isChecked())
        filters.append("CASE_SENSITIVE");

    emit findSignal(ui->LEFind->text(), filters);

    close();
}
