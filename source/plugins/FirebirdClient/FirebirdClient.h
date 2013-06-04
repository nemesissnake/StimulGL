//FirebirdClient
//Copyright (C) 2013  Sven Gijsen
//
//This file is part of StimulGL.
//StimulGL is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.  If not, see <http://www.gnu.org/licenses/>.
//


//This file defines the script binding interface, all below function are scriptable except for the destructor

#ifndef FirebirdClient_H
#define FirebirdClient_H
#include <QObject>
#include <QString>
#include <QtScript>
#include <QScriptable>
//#include "FireBirdDatabase.h"
#include "DatabaseModel.h"


class FirebirdClient : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "FirebirdClient");//Can't use defines here!, moc doesn't handle defines, not needed here
	//Q_PROPERTY( short ExampleProperty WRITE setExampleProperty READ getExampleProperty )

signals:
	//void ExampleSignalTriggered(short);

public:
	FirebirdClient(QObject *parent = 0);
	~FirebirdClient();
	FirebirdClient(const FirebirdClient& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script

	bool InitializeDatabase();
	bool CreateDatabase(const QString& filePath, const QString& userName, const QString& password);      
	bool OpenDatabase(const QString& filePath, const QString& userName, const QString& password);
	bool CloseDatabase();
	bool ExecuteDatabaseQuery(const QString& sQuery);
	bool ShowDatabaseQuery(const QString& sQuery);
	bool ExportDatabasetoExcel(const QString& sPath, const QString& sQuery, const QString& sSheetName = "");

private:
	bool RetrieveSQLQuery(const QString &sQuery, QSqlQuery &query);
	QStringList getListOfPids(const QString &SProcessName = "", const QStringList &lExcludeList = QStringList());
	bool KillPids(const QStringList &lPidList);

	QScriptEngine* currentScriptEngine;
	Model::DatabaseModel dbModel;
	Model::FireBirdDatabase *fbDatabase;

};

#endif // FirebirdClient_H
