//Construct a new Plugin object
var FirebirdClientobject = new FirebirdClient();
var scriptRootPath = StimulGL.getSelectedScriptFileLocation();
var databaseName = "Test.FDB";
var databasePath = scriptRootPath + "/" + databaseName;
var excelFileName = "Test.xls";
var excelExportPath = scriptRootPath + "/" + excelFileName;
var userName = "user";
var userPassword = "123";
var bResult = true;
var QueryCreateTable = 
	"create table people (" + 
	"id int not null," + 
	"nickname varchar(12) not null," + 
	"country char(4)," + 
	"constraint pk_people primary key (id)," + 
	"constraint uk_nickname unique (nickname) using index ix_nick" + 
	")";
var QueryInsertRecord =	
"insert into people (id, nickname, country) " +
"values ('1', 'Nick_Name', 'NLE') " +
"returning nickname";
var QuerySelectRecords = 
"select * from people"
var QueryCreateProcedure = 
"CREATE PROCEDURE ADD_INTS (" +
" A      INTEGER," +
" B      INTEGER)" +
" RETURNS (" +
" RESULT INTEGER)" +
" AS " +
" begin" +
" RESULT = A + B;" +
" end";
var QueryCallProcedure = "EXECUTE PROCEDURE ADD_INTS(53, 4)";
//""
//"EXECUTE PROCEDURE ADD_INTS" +
//" 3, 4" +
//" RETURNING_VALUES RESULT;";


////Using JSON Arrays
//var employees = [
//{ "firstName":"John" , "lastName":"Doe" }, 
//{ "firstName":"Anna" , "lastName":"Smith" }, 
//{ "firstName":"Peter" , "lastName": "Jones" }
//];
//Log(employees[1].lastName);
//employees[0].firstName = "Jonatan";
//Log(employees[0].firstName);

Log("\n");
//Test whether we can invoke some default implemented Slots

var fDatabase = new QFile(databasePath);
if(fDatabase.exists() == false)
{
	bResult = FirebirdClientobject.CreateDatabase(databasePath,userName,userPassword);
	Log("CreateDatabase() returned: " + bResult);
}
else
{
	bResult = true;
}
if(bResult)
{
	bResult = bResult && FirebirdClientobject.OpenDatabase(databasePath,userName,userPassword)
	Log("OpenDatabase() returned: " + bResult);
	if(bResult)
	{
		//bResult = bResult && FirebirdClientobject.InitializeDatabase()
		//Log("InitializeDatabase() returned: " + bResult);	
		//bResult = bResult && FirebirdClientobject.ExecuteDatabaseQuery(QueryCreateTable)
		//Log("ExecuteDatabaseQuery(QueryCreateTable) returned: " + bResult);
		//bResult = bResult && FirebirdClientobject.ExecuteDatabaseQuery(QueryInsertRecord)
		//Log("ExecuteDatabaseQuery(QueryInsertRecord) returned: " + bResult);	
		//bResult = bResult && FirebirdClientobject.ExecuteDatabaseQuery(QuerySelectRecords)
		//Log("ExecuteDatabaseQuery(QuerySelectRecords) returned: " + bResult);	
		//bResult = bResult && FirebirdClientobject.ExecuteDatabaseQuery(QueryCreateProcedure)
		//Log("ExecuteDatabaseQuery(QueryCreateProcedure) returned: " + bResult);		
		//bResult = bResult && FirebirdClientobject.ExecuteDatabaseQuery(QueryCallProcedure)
		//Log("ExecuteDatabaseQuery(QueryCallProcedure) returned: " + bResult);
		bResult = bResult && FirebirdClientobject.ShowDatabaseQuery(QuerySelectRecords)
		Log("ShowDatabaseQuery(QuerySelectRecords) returned: " + bResult);
		//bResult = bResult && FirebirdClientobject.ExportDatabasetoExcel(excelExportPath,QuerySelectRecords);//, "20130604141747883")
		//Log("ExportDatabasetoExcel(QuerySelectRecords) returned: " + bResult);
		
		
		bResult = bResult && FirebirdClientobject.CloseDatabase()
		Log("CloseDatabase() returned: " + bResult);	
	}	
}


Log("\n");
//Set the constructed object to null
FirebirdClientobject = null;