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


#include "stdafx.h"
#include "FireBirdDatabase.h"
#include "ibase.h"
#include "iberror.h"

using namespace Model;

QString FireBirdDatabase::qtIBaseDriverName_ = "QIBASE";
QString FireBirdDatabase::connectionString_ = "server type=Embedded; auto_commit=True; auto_commit_level=4096; connection lifetime=1; DataBase=\"%1\"";

#ifdef _DEBUG
QString FireBirdDatabase::qtIBasePluginName_ = "qsqlibased.dll";
#else
QString FireBirdDatabase::qtIBasePluginName_ = "qsqlibase.dll";
#endif

FireBirdDatabase::FireBirdDatabase():isInitialized_(false), databasePath_(""), connectionName_(""), instances_(0), lastError_(0)
{   
}

FireBirdDatabase::~FireBirdDatabase()
{
    Close();
}

bool FireBirdDatabase::Initialize()
{
	lastError_ = 0;
	if (isInitialized_)
    {
        return true;
    }
    if(!pluginLoader_.isLoaded())
    {
		QString tmpString = MainAppInfo::appDirPath() + QDir::separator() + qtIBasePluginName_;//+ "sqldrivers" + QDir::separator() + qtIBasePluginName_;
		pluginLoader_.setFileName(tmpString);        
        if (!pluginLoader_.load())
        {            
            qDebug() << __FUNCTION__ << "Loading SQL Driver failed, searched in folder: " << tmpString;
            isInitialized_ = false;
            return false;
        }
    }
    QObject* object = pluginLoader_.instance();
	if (object == NULL)
    {            
        qDebug() << __FUNCTION__ << "Loading SQL Driver Instance failed.";
        pluginLoader_.unload();
        return false;
    }
    QSqlDriverPlugin* plugin = qobject_cast<QSqlDriverPlugin*>(object);
    if (plugin == NULL)
    {            
        qDebug() << __FUNCTION__ << "QSqlDriverPlugin == NULL";
        pluginLoader_.unload();
        return false;
    }
    driver_ = plugin->create("QIBASE");
    if (driver_ == NULL)
    {            
        qDebug() << __FUNCTION__ << "Loading QIBASE Driver Instance failed.";
        pluginLoader_.unload();
        return false;
    }
    isInitialized_ = true;
    return isInitialized_;
}

bool FireBirdDatabase::Create(const QString& filePath, const QString& userName, const QString& password)
{
    if (!isInitialized_)
    {
        Initialize();
    }
    if (QFile::exists(filePath))
    {
        return false;
    }
	databasePath_ = filePath;

    QString queryString;
    queryString += "CREATE DATABASE";
    queryString += " \'" + databasePath_ + "\'";
    queryString += " USER \'" + userName + "\'";
    queryString += " PASSWORD \'" + password + "\'";
    queryString += " DEFAULT CHARACTER SET UNICODE_FSS";
    ISC_STATUS_ARRAY status;
    isc_db_handle   databaseHandle = NULL;
    isc_tr_handle   transactionHandle = NULL;
    unsigned short g_nFbDialect = SQL_DIALECT_V6;
    if (isc_dsql_execute_immediate(status, &databaseHandle, &transactionHandle, 0, queryString.toStdString().c_str (), g_nFbDialect, NULL))
    {
        long SQLCODE=isc_sqlcode(status);
		qDebug() << __FUNCTION__ << "Could not create database(SQLCODE=" << SQLCODE << ")";
        return false;
    }
    isc_commit_transaction( status, &transactionHandle );
    if (databaseHandle != NULL)
    {
        ISC_STATUS_ARRAY status;
        isc_detach_database(status, &databaseHandle);
    }
    return true;
}

bool FireBirdDatabase::Open(const QString& filePath, const QString& userName, const QString& password)
{
    isInitialized_ = false;
    lastError_ = 0;
    if (!Initialize())
    {
        qDebug() << __FUNCTION__ << "Firebird database is not initialized.";
        return false;
    }
    instances_++;
    connectionName_ = "Connection_1";            
    QSqlDatabase database; 
    // "Adding database (DRIVER) ";
    database = QSqlDatabase::addDatabase(driver_, connectionName_);
    // "Check Valid database. ";
    if (!database.isValid())
    {
        QString lastError = database.lastError().text();
        qDebug() << __FUNCTION__ << "Database is not valid. LastError = " << lastError;
        return false;
    }
    // "Set database configurations.";
    database.setDatabaseName(filePath);
    database.setUserName(userName);
    database.setPassword(password);
    QString connectionString = QString(connectionString_).arg(filePath);
    database.setConnectOptions(connectionString);
	bool result = false;    
    result = database.open();
    if(!result)
    {
        QString lastError = database.lastError().text();   
		qDebug() << __FUNCTION__ << "Could not open database(" << lastError << ")";
        lastError_ = (uint)database.lastError().number();        
    }
    return result;
}

bool FireBirdDatabase::Close()
{    
    try
    {
        lastError_ = 0;
		{
			QSqlDatabase database = QSqlDatabase::database(connectionName_);
			if (database.isValid() && database.isOpen())
			{
				database.close();
			} 
		}
        QSqlDatabase::removeDatabase(connectionName_);
        driver_ = NULL;
    }
    catch (...)
    {
		qDebug() << __FUNCTION__ << "Error while closing database";
        return false;
    }
    return true;
}

bool FireBirdDatabase::IsOpen()
{
	QSqlDatabase database = QSqlDatabase::database(connectionName_);
    if (database.isValid() && database.isOpen())
    {
        return true;
    }
    return false;
}

QSqlQuery Model::FireBirdDatabase::CreateQuery()
{
    return QSqlQuery(QSqlDatabase::database(connectionName_));
}

int Model::FireBirdDatabase::LastError()
{
    return lastError_;
}