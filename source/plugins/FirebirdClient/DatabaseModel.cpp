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


//#include "stdafx.h"
#include "DatabaseModel.h"

#include <QSqlRecord>
#include <QStringList>

using namespace Model;

QString DatabaseModel::userName_ = "Serg";
QString DatabaseModel::password_ = "12345";
QString DatabaseModel::resourceString_ = ":/FireBirdAndQT/Resources/Model/New.FDB";

DatabaseModel::DatabaseModel()
{

}

DatabaseModel::~DatabaseModel()
{
     Close();
}

void DatabaseModel::Open()
{
    if (fireBirdDatabase_.IsOpen())
    {
        fireBirdDatabase_.Close();
    }    
    QString databasePath = GetFullPath("Created.FDB");
    //bool isOpen = 
		fireBirdDatabase_.Open(databasePath, userName_, password_);
}

bool DatabaseModel::Create()
{
    Close();
    QString testDBPath = GetFullPath("Created.FDB");
    return fireBirdDatabase_.Create(testDBPath, userName_, password_);
}

void DatabaseModel::Close()
{   
    if (fireBirdDatabase_.IsOpen())
    {
        fireBirdDatabase_.Close();
    }    
}

QString DatabaseModel::GetFullPath(const QString& path)
{
    QString curPath  = QDir::currentPath();
    QDir directory(curPath);
    directory.cdUp();
    bool result = directory.setCurrent("debug");
    directory.cd("debug");
    result = directory.setCurrent("DB");
    directory.cd("DB");
    QString databasePath = directory.filePath(path);    
    // return back to curPath;
    result = directory.setCurrent(curPath);
    return databasePath;
}

void DatabaseModel::AddJob(const QString& jobName )
{
    QByteArray description("Test description");
    int salary = 1200;
    QSqlQuery query(fireBirdDatabase_.CreateQuery());
    //bool result = 
		query.prepare("EXECUTE PROCEDURE SP_INSERT_JOB (?, ?, ?)");
    query.addBindValue(jobName);
    query.addBindValue(salary);
    query.addBindValue(description);  
    if (!query.exec())
    {
		//for(int i=0;i<query.la();i++)
		//{

		//}
        QString err = query.lastError().text();            
        throw std::runtime_error("Error executing Query.");
    }    
    query.next();
    //int jobID = 
		query.value(0).toUInt();
}

void DatabaseModel::SelectJobs(QStringList& jobs )
{
	QSqlQuery query = QSqlQuery(fireBirdDatabase_.CreateQuery());
    QString preparedString = "CREATE TABLE TBL_JOBS ( id int not null primary key )";//, JOB_NAME CSTRING(100) )";
    query.prepare(preparedString);
    if (!query.exec())    {
        QString err = query.lastError().text();       
        throw std::runtime_error("Error executing Query.");
    }  


/*    QSqlQuery query = QSqlQuery(fireBirdDatabase_.CreateQuery());
    QString preparedString = "SELECT JOB_NAME FROM TBL_JOBS";
    query.prepare(preparedString);
    if (!query.exec())    {
        QString err = query.lastError().text();       
        throw std::runtime_error("Error executing Query.");
    } */   
    while (query.next())
    { 
        QSqlRecord record = query.record();
        jobs.append(record.value(0).toString());
        
    }
}