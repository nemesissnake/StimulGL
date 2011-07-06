#ifndef HIDCALIBRATEDIALOG_H
#define HIDCALIBRATEDIALOG_H

#include <QDialog>
#include "ui_hidcalibratedialog.h"
#include "renderarea.h"

class HIDCalibrateDialog : public QDialog
{
	Q_OBJECT

public:
	HIDCalibrateDialog(QWidget *parent = 0);
	~HIDCalibrateDialog();

private:
	Ui::HIDCalibrateDialog ui;
	RenderArea *renderArea;

private slots:
	void OkButtonClicked();
};

#endif // HIDCALIBRATEDIALOG_H
