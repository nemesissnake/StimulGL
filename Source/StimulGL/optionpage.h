#ifndef OPTIONPAGE_H
#define OPTIONPAGE_H

#include <QDialog>
#include <QSettings>
#include "ui_optionpage.h"

class OptionPage : public QDialog
{
	Q_OBJECT

public:
	OptionPage(QSettings &settings, QWidget *parent = 0);
	~OptionPage();

private:
	Ui::OptionPageClass ui;
	QSettings *mainSettings;

private slots:
	void on_rdb_3DRenderer_3_toggled(bool);
	void on_rdb_3DRenderer_2_toggled(bool);
	void on_rdb_3DRenderer_toggled(bool);
	void setUI3DRendererControls(int type);
	void applySettings();
	void readSettings();
};

#endif // OPTIONPAGE_H
