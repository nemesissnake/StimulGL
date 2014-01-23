//FirebirdClient
//Copyright (C) 2014  Sven Gijsen
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


#ifndef _FIREBIRD_DATABASE_H_
#define _FIREBIRD_DATABASE_H_

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlDriver>
#include <QPluginLoader>
#include <QSqlDriverPlugin>
#include <QDir>
#include <QLibrary>
#include <QSqlResult>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QtSql/qsql.h>
#include <QDebug>
#include "./../../StimulGL/mainappinfo.h"

namespace Model
{
    class FireBirdDatabase : public QObject
    {
        Q_OBJECT

    public:
        FireBirdDatabase();
        ~FireBirdDatabase();

        bool Initialize();        
        bool Create(const QString& filePath, const QString& userName, const QString& password);
        bool Open(const QString& filePath, const QString& userName, const QString& password);
        bool Close();
        int LastError();

        bool IsOpen();
        
        QSqlQuery CreateQuery();

    private:
        QPluginLoader pluginLoader_;
        QString databasePath_;
        QString connectionName_;
        QSqlDriver* driver_;
        bool isInitialized_;    
        int instances_;
        int lastError_;

    private:
        static QString qtIBasePluginName_;
        static QString qtIBaseDriverName_;
        static QString connectionString_;

        Q_DISABLE_COPY(FireBirdDatabase);
    };
}
#endif //_FIREBIRD_DATABASE_H_