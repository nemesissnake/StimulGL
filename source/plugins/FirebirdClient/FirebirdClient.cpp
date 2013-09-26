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


#include "FirebirdClient.h"
#include <QSQLRecord>
#include <QTableView>
#include <QAxObject>
#include <QFileDialog>
#include "Psapi.h"

QScriptValue FirebirdClient::ctor__extensionname(QScriptContext* context, QScriptEngine* engine)
{
	Q_UNUSED(context);
	//this function gets called first whenever a new object is constructed trough the script

	//	if (context->isCalledAsConstructor()) {
	//		// initialize the new object
	//		//context->thisObject().setProperty("bar", ...);
	//		// ...
	//		// return a non-object value to indicate that the
	//		// thisObject() should be the result of the "new Foo()" expression
	//		//return engine->undefinedValue();

	return engine->newQObject(new FirebirdClient(), QScriptEngine::ScriptOwnership);//Now call the below real Object constructor
} 

FirebirdClient::FirebirdClient(QObject *parent) : QObject(parent)
{
	currentScriptEngine = NULL;
	fbDatabase = NULL;
}

FirebirdClient::~FirebirdClient()
{
	if(fbDatabase)
	{
		if(fbDatabase->IsOpen())
			fbDatabase->Close();
		delete fbDatabase;
		fbDatabase = NULL;
	}
}

bool FirebirdClient::makeThisAvailableInScript(QString strObjectScriptName, QObject *engine)
{
	if (engine)
	{
		currentScriptEngine = reinterpret_cast<QScriptEngine *>(engine);
		//QObject *someObject = this;//new MyObject;
		QScriptValue objectValue = currentScriptEngine->newQObject(this);
		currentScriptEngine->globalObject().setProperty(strObjectScriptName, objectValue);
		return true;
	}
	return false;
}

bool FirebirdClient::InitializeDatabase()
{
	if(fbDatabase == NULL)
		return false;
	return fbDatabase->Initialize();
}

bool FirebirdClient::CreateDatabase(const QString& filePath, const QString& userName, const QString& password)
{
	if(fbDatabase == NULL)
		fbDatabase = new Model::FireBirdDatabase();
	else if(fbDatabase->IsOpen())
		fbDatabase->Close();
	return fbDatabase->Create(filePath,userName,password);
}

bool FirebirdClient::OpenDatabase(const QString& filePath, const QString& userName, const QString& password)
{
	if(fbDatabase == NULL)
		fbDatabase = new Model::FireBirdDatabase();
	else if(fbDatabase->IsOpen())
		fbDatabase->Close();
	return fbDatabase->Open(filePath,userName,password);
}

bool FirebirdClient::CloseDatabase()
{
	if(fbDatabase == NULL)
		return true;
	if(fbDatabase->IsOpen())
		return fbDatabase->Close();
	return true;
}

bool FirebirdClient::ExportDatabasetoExcel(const QString& sPath, const QString& sQuery, const QString& sSheetName)
{
	QSqlQuery query;
	if(RetrieveSQLQuery(sQuery,query))
	{
		QString sFinalPath = "";
		QFile fileSource(sPath);
		QFileInfo fileDestInfo(sPath);
		bool bNewFile = true;

		if (!fileSource.exists())
		{
			if(sPath == "")
			{
				sFinalPath = QFileDialog::getSaveFileName(NULL, tr("Select an Excel File or Create a new one"), QDir::currentPath(), tr("Excel File (*.xls)"));
				fileSource.setFileName(sFinalPath);
			}
			else
			{
				if (!fileDestInfo.absoluteDir().exists())
				{
					if (!QDir().mkdir(fileDestInfo.absolutePath()))
					{
						qDebug() << __FUNCTION__ << "Could not create the directory: " << fileDestInfo.absolutePath();
						return false;
					}
				}
			}
			bNewFile = true;
		}
		else
		{
			bNewFile = false;
		}
		sFinalPath = sPath;
		QStringList lExcelPIDS = getListOfPids("EXCEL.EXE");
		
		QAxObject* excel = new QAxObject( "Excel.Application");
		QAxObject* workbooks = excel->querySubObject( "Workbooks" );
		QAxObject* workbook = NULL;
		QAxObject* sheets = NULL;
		QAxObject* sheet = NULL;
		QAxObject* range = NULL;
		QAxObject* font = NULL;
		QVariantList params;
		if(bNewFile)
			workbook = workbooks->querySubObject( "Add()");
		else
			workbook = workbooks->querySubObject( "Open(const QString&)", sFinalPath );			
		sheets = workbook->querySubObject( "Worksheets" );

		excel->setProperty("Visible", false);
		excel->setProperty("ScreenUpdating", false);
		int nSheetCount = sheets->dynamicCall("Count()").toInt();//worksheets count
		//count = sheets->property("Count").toInt();
		
		QString sFinalSheetName;
		if(sSheetName == "")
		{
			sFinalSheetName = QDateTime::currentDateTime().toString(MainAppInfo::stdDateTimeFormat());
		}
		else
		{
			sFinalSheetName = sSheetName;
		}

		for (int i=1; i<=nSheetCount; i++) //cycle through sheets
		{			
			sheet = sheets->querySubObject( "Item(int)", i);//sheet pointer
			//params.clear();
			//params.append("D8");
			//params.append("B2");
			//range = sheet->querySubObject( "Range(QVariant)", params);//, QVariant
			if((sheet->property("Name").toString() == sFinalSheetName) || (i==nSheetCount))
			{
				if(sheet->property("Name").toString() != sFinalSheetName)
				{
					params.clear();
					params.append(sFinalSheetName);
					if(bNewFile)
					{
						sheet->dynamicCall("Name",params);
					}
					else
					{
						sheets->dynamicCall("Add()",0);
						nSheetCount++;
						//for (int j=1; j<=nSheetCount; j++) //cycle through sheets
						//{
							sheet = sheets->querySubObject( "Item(int)", 1);
							QString aa = sheet->property("Name").toString();
							//{
							//	sheet = sheets->querySubObject( "Item(int)", j);//sheet pointer
							//	break;
							//}
						//}						
						sheet->dynamicCall("Name",params);
					}					
				}

				int nRow=0;
				while (query.next())//Rows
				{
					for (int nColumn=0; nColumn<=query.record().count(); nColumn++)//Columns
					{
						if(nRow == 0)
						{
							range = sheet->querySubObject( "Cells(integer,integer)", 1, nColumn+1);//row,column defined
							params.clear();
							params.append(query.record().fieldName(nColumn));
							range->dynamicCall("Value",params);		
							font = range->querySubObject("Font");
							font->setProperty("Italic", true);
							font->setProperty("Bold", true);							
						}
						range = sheet->querySubObject( "Cells(integer,integer)", nRow + 2,nColumn + 1);
						params.clear();
						QString tmpString = query.value(nColumn).toString();
						params.append(query.value(nColumn).toString());
						range->dynamicCall("Value",params);	
					}
					nRow++;
				}
				break;
			}
			else if(bNewFile)
			{
				sheet->dynamicCall("Delete");
				nSheetCount--;
				i--;
			}
			//int fieldNo = query->record().indexOf("id");
			//if(fieldNo >= 0)
			//{
				//int jobID = query.value(0).toUInt();
			
			//params.clear();
			//params.append(QString("helleo123"));
			//range->dynamicCall("Value",params);

			//objExcelRG = objExcelWS.Range("A1", "B5")
			//objExcelRG.Select()
			//objExcelRG.Cells.Font.Italic = True
			//objExcelRG.Font.ColorIndex = 4
		/*
			int rowCount = rows->dynamicCall( "Count()" ).toInt(); //unfortunately, always returns 255, so you have to check somehow validity of cell values
			QAxObject* columns = sheet->querySubObject( "Columns" );
			int columnCount = columns->property("Count").toInt();
			for (int row=1; row <= rowCount; row++)
			{
				QVariantList dataRow;
				bool isEmpty = true; //when all the cells of row are empty, it means that file is at end (of course, it maybe not right for different excel files. it's just criteria to calculate somehow row count for my file)
				for (int column=1; column <= columnCount; column++)
				{
					//Do something useful here
				}
				if (isEmpty) //criteria to get out of cycle
					break;
				data.append(dataRow);
			}
			*/
		}

		sFinalPath = sFinalPath.replace("/","\\");
		//workbook->dynamicCall(QString("Save(" + sFinalPath + ")").toLatin1());//SaveAs
		params.clear();
		params.append(sFinalPath);
		excel->setProperty("DisplayAlerts", false);
		workbook->dynamicCall("SaveAs(QVariant)", params);
		//workbook->dynamicCall("SaveCopyAs()", params);//QVariant
		//excel process still running, visible , see around and above
		fileSource.setFileName(sFinalPath);
		if(!fileSource.exists())
		{
			//params.clear();
			//QString sTemp = QDir::currentPath();//+ "\\untitled.xls";
			//sTemp = sTemp.replace("/","\\");
			//params.append(sTemp);
			//params.clear();
			//workbook->dynamicCall("SaveAs()", params);
			//workbook->dynamicCall("GetSaveAsFilename()", params);
			qDebug() << __FUNCTION__ << "Could not save the Excel file: " << sFinalPath;
		}
		excel->setProperty("DisplayAlerts", true);		
		workbook->dynamicCall("Close (Boolean)", true);
		excel->dynamicCall("Quit (void)");
		if(font)
			delete font;
		if(range)
			delete range;
		if(sheet)
			delete sheet;
		if(sheets)
			delete sheets;
		if(workbook)
			delete workbook;
		if(workbooks)
			delete workbooks;
		if(excel)
			delete excel;
		//Create Excel API Documentation!
		//QFile file1("c://sheet2.html");
		//file1.open(QIODevice::WriteOnly | QIODevice::Text);
		//QTextStream out(&file1);
		//out << excel->generateDocumentation();
		//file1.close();
		lExcelPIDS = getListOfPids("EXCEL.EXE",lExcelPIDS);
		KillPids(lExcelPIDS);
		return true;
	}
	return false;
}

bool FirebirdClient::KillPids(const QStringList &lPidList)
{
	bool bReturnValue = false;
	for (int i=0;i<lPidList.count();i++)
	{
		QProcess process;
		QString sCommand("Taskkill /PID " + lPidList.at(i) + " /F");
		process.start(sCommand);
		//process.waitForStarted(1000);
		process.waitForFinished(1000);
		bReturnValue = true;
	}
	return bReturnValue;
}

QStringList FirebirdClient::getListOfPids(const QString &SProcessName, const QStringList &lExcludeList)
{
		QStringList listOfPids;
		QByteArray tmpByteArray = SProcessName.toLocal8Bit();
		const char* processName = tmpByteArray.data();

#if defined(Q_OS_WIN)
		// Get the list of process identifiers.
		DWORD aProcesses[1024], cbNeeded, cProcesses;
		unsigned int i;
		QString tmpString;
		if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
		{
			return listOfPids;
		}
		// Calculate how many process identifiers were returned.
		cProcesses = cbNeeded / sizeof(DWORD);
		// Search for a matching name for each process
		for (i = 0; i < cProcesses; i++)
		{
			if (aProcesses[i] != 0)
			{
				char szProcessName[MAX_PATH] = {0};
				DWORD processID = aProcesses[i];
				// Get a handle to the process.
				HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,	FALSE, processID);
				// Get the process name
				if (NULL != hProcess)
				{
					HMODULE hMod;
					DWORD cbNeeded;
					if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
					{
						GetModuleBaseNameA(hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(char));
					}
					// Release the handle to the process.
					CloseHandle(hProcess);
					bool bAvailable = false;
					if(*szProcessName != 0 && SProcessName == "") //All processes!
						bAvailable = true;
					else if (*szProcessName != 0 && strcmp(processName, szProcessName) == 0)
						bAvailable = true;
					if(bAvailable)
					{
						tmpString = QString::number(processID);
						if(lExcludeList.contains(tmpString)==false)
						{
							listOfPids.append(QString::number(processID));
						}						
					}
				}
			}
		}
		//int nCount = 
			listOfPids.count();
		return listOfPids;

#else

		// Run pgrep, which looks through the currently running processes and lists the process IDs
		// which match the selection criteria to stdout.
		QProcess process;
		process.start("pgrep",  QStringList() << processName);
		process.waitForReadyRead();

		QByteArray bytes = process.readAllStandardOutput();

		process.terminate();
		process.waitForFinished();
		process.kill();

		// Output is something like "2472\n2323" for multiple instances
		if (bytes.isEmpty())
			return 0;

		// Remove trailing CR
		if (bytes.endsWith("\n"))
			bytes.resize(bytes.size() - 1);

		listOfPids = QString(bytes).split("\n");
		return listOfPids.count();

#endif
	//}


	//return;
	//QProcess process;
	//process.setReadChannel(QProcess::StandardOutput);
	//process.setReadChannelMode(QProcess::MergedChannels);
	////    process.start("cmd.exe /C echo test");
	//process.start("wmic.exe /OUTPUT:STDOUT PROCESS get Caption, Processid");
	//process.waitForStarted(1000);
	////process.waitForFinished(1000);
	//process.waitForFinished(-1);
	//QByteArray list = process.readAll();

	//qDebug() << "Read" << list.length() << "bytes";
	////qDebug() << list;
}

bool FirebirdClient::ShowDatabaseQuery(const QString& sQuery)
{
	QSqlQuery query;
	if(RetrieveSQLQuery(sQuery,query))
	{
		QSqlQueryModel *model = new QSqlQueryModel;
		model->setQuery(query);//"SELECT name, salary FROM employee");
		//model->setHeaderData(0, Qt::Horizontal, tr("Name"));
		//model->setHeaderData(1, Qt::Horizontal, tr("Salary"));
		//int nId = model->record(2).value("id").toInt();
		QTableView *view = new QTableView;
		view->setModel(model);
		view->show();
		return true;
	}
	return false;
}

bool FirebirdClient::ExecuteDatabaseQuery(const QString& sQuery)
{
	QSqlQuery query;
	if(RetrieveSQLQuery(sQuery,query))
	{
		/*
		QStringList sList;
		//int fieldNo = query->record().indexOf("id");
		//if(fieldNo >= 0)
		{
			//int jobID = query.value(0).toUInt();
			while (query.next()) 
			{
				//sList.append(query->value(fieldNo).toString());
				sList.append(query.value(0).toString());
			}
			//QString sResult = query.value(0).toString();
		}
		*/
		return true;
	}
	return false;
}

bool FirebirdClient::RetrieveSQLQuery(const QString &sQuery, QSqlQuery &query)
{
	if(fbDatabase == NULL)
		return false;
	if(fbDatabase->IsOpen())
	{
		//QByteArray description("Test description");
		//int salary = 1200;
		//QSqlQuery query(fireBirdDatabase_.CreateQuery());
		//bool result = query.prepare("EXECUTE PROCEDURE SP_INSERT_JOB (?, ?, ?)");
		//query.addBindValue(jobName);
		//query.addBindValue(salary);
		//query.addBindValue(description);  
		//if (!query.exec())
		//{
		//	//for(int i=0;i<query.la();i++)
		//	//{

		//	//}
		//	QString err = query.lastError().text();            
		//	throw std::runtime_error("Error executing Query.");
		//}    
		//query.next();
		//int jobID = query.value(0).toUInt();

		query = QSqlQuery(fbDatabase->CreateQuery());
		if(query.prepare(sQuery))
		{
			if (!query.exec())
			{
				QString sError = query.lastError().text();            
				qDebug() << __FUNCTION__ << "Error executing Query (" << sError << ")";
			}    
			return true;
		}
		//delete query;
		//query = NULL;
	}
	return false;
}