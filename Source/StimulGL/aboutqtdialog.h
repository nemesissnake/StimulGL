#ifndef ABOUTQTDIALOG_H
#define ABOUTQTDIALOG_H

#include <QDialog>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QStringList;
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class aboutQTDialog : public QDialog
{
	Q_OBJECT

public:
	aboutQTDialog(const QString &title,const QStringList &pluginFileNames,const QString &path, const QStringList &fileNames, QWidget *parent = 0);

private:
	void findPlugins(const QString &title, const QString &path, const QStringList &fileNames);
	void populateTreeWidget(QObject *plugin, const QString &text);
	void addItems(QTreeWidgetItem *pluginItem, const char *interfaceName,const QStringList &features);

	QLabel *label;
	QTreeWidget *treeWidget;
	QPushButton *okButton;
	QIcon interfaceIcon;
	QIcon featureIcon;
	
};

#endif // ABOUTQTDIALOG_H
