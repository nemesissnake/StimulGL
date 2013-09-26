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

//!  The FirebirdClient class. 
/*!
  The Firebird Client provides an interface for working with Firebird databases (see http://www.firebirdsql.org/).
*/
class FirebirdClient : public QObject, protected QScriptable
{
	Q_OBJECT
	//Q_CLASSINFO("ScriptAPIClassName", "FirebirdClient");//Can't use defines here!, moc doesn't handle defines, not needed here

public:
	FirebirdClient(QObject *parent = 0);
	~FirebirdClient();
	FirebirdClient(const FirebirdClient& other ){Q_UNUSED(other);}//TODO fill in copy constructor, should be used for the Q_DECLARE_METATYPE macro

	static QScriptValue ctor__extensionname(QScriptContext* context, QScriptEngine* engine);

public slots:
	bool makeThisAvailableInScript(QString strObjectScriptName = "", QObject *engine = NULL);//To make the objects (e.g. defined in a *.exml file) available in the script
	
	//! \brief CreateDatabase slot.
	/*!  This function creates a new Firebird database and automatically initializes it.
	 * @param filePath the directory path to the new database file (<Path>/<DBName>.FDB).
	 * @param userName the username of the database creator that gets the administrator rights.
	 * @param password the password for the above administrator user.
	 * @return a boolean value determing whether the database could be created.
	 */
	bool CreateDatabase(const QString& filePath, const QString& userName, const QString& password);     
	//! \brief OpenDatabase slot.
	/*!  This function opens an existing Firebird database and automatically initializes it.
	 * @param filePath the directory path to the database file (<Path>/<DBName>.FDB) to open.
	 * @param userName a username of the database with sufficient rights to open the Firebird database.
	 * @param password the password for the above user.
	 * @return a boolean value determing whether the database could be opened.
	 */
	bool OpenDatabase(const QString& filePath, const QString& userName, const QString& password);
	//! \brief InitializeDatabase slot.
	/*!  This function initializes the last opened or created Firebird database.
	 *   This step is not necessary because the FirebirdClient::CreateDatabase and FirebirdClient::OpenDatabase automatically call this function when succesfully executed.
	 * @return a boolean value determing whether the database could be initialized.
	 */
	bool InitializeDatabase();
	//! \brief ExecuteDatabaseQuery slot.
	/*!  This function executes a custom SQL Query.
	 * @param sQuery the SQL Query to execute.
	 * @return a boolean value determing whether the SQL Query could be executed.
	 */
	bool ExecuteDatabaseQuery(const QString& sQuery);
	//! \brief ShowDatabaseQuery slot.
	/*! This function executes a custom SQL Select Query and shows the resulting table in a popup window with a grid layout.
	 * @param sQuery the SQL Select Query to execute.
	 * @return a boolean value determing whether the SQL Query could be executed.
	 */
	bool ShowDatabaseQuery(const QString& sQuery);
	//! \brief ExportDatabasetoExcel slot.
	/*!  This function exports the result (table) of an SQL Select Query to an Sheet of an Excel document.
	 * @param sPath the directory path to the (new or existing) Excel file (<Path>/<ExcelName>.XLS)..
	 * @param sQuery the SQL Select Query to execute.
	 * @param sSheetName the name of the Excel documents sheet to export the result to.
	 * @return a boolean value determing whether the SQL Select Query could be exported to an Sheet of an Excel document.
	 */
	bool ExportDatabasetoExcel(const QString& sPath, const QString& sQuery, const QString& sSheetName = "");
	//! \brief CloseDatabase slot.
	/*!  This function closes the Firebird database.	
	 * @return a boolean value determing whether the database could be closed.
	 */
	bool CloseDatabase();

private:
	bool RetrieveSQLQuery(const QString &sQuery, QSqlQuery &query);
	QStringList getListOfPids(const QString &SProcessName = "", const QStringList &lExcludeList = QStringList());
	bool KillPids(const QStringList &lPidList);

	QScriptEngine* currentScriptEngine;
	Model::DatabaseModel dbModel;
	Model::FireBirdDatabase *fbDatabase;

};

#endif // FirebirdClient_H
