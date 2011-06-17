#ifndef PLUGINCOLLECTION_H
#define PLUGINCOLLECTION_H

#include <QObject>
#include <QStringList>
#include <QRegExp>
#include "plugininterface.h"

class PluginCollection : public QObject
{
	Q_OBJECT

public:

	enum PluginType
	{
		DevicePlugin		    = 0,
		ExtensionPlugin			= 1
	};
	Q_DECLARE_FLAGS(PluginTypes, PluginType)

	PluginCollection(QObject *parent);
	~PluginCollection();

	int Add(QObject *plugin);
	bool IsEmpty();
	int Count();
	QString GetLoaderName(int index);
	int GetIndex(QString name);
	PluginInterface *GetInterface(int index);

private:
	bool Cleanup();

	int MaxPlugins;
	int NrofPlugins;
	QList<QObject *> PlgInterfaces;
	QList<int> PlgTypes;
	QStringList PlgNames;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(PluginCollection::PluginTypes)

#endif // PLUGINCOLLECTION_H
