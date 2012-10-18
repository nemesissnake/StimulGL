#ifndef CONTAINERDLG_H
#define CONTAINERDLG_H

#include <QDialog>

class ContainerDlg : public QDialog
{
	Q_OBJECT

public:
	ContainerDlg(QWidget *parent = NULL);
	ContainerDlg::~ContainerDlg();
	
	private slots:
		void reject();

protected:
	void closeEvent(QCloseEvent *e);
};

#endif // CONTAINERDLG_H
