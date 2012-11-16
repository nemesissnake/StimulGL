var tmpString;
var InstallProcess = new QProcess();
var tmpByteArray = new QByteArray();
var tmpStringList =  new Array();
var tmpString = new String();
var sBinairySteps;
var bDoCleanup;
var bFileProccessed;
var nCounter;
var bSkipStep;
var fileDest;
var changeSet;
var fileSource = "PluginTemplate.cfg";
var preFix = "-*/*-";
var postFix = "-*/*-";

var sScriptPath = StimulGL.getSelectedScriptFileLocation();
var sBinairyPath;

Include("GenerateDoxyConfigFile.qs");//Make sure to call the cleanup when not needed anymore!

QByteArray.prototype.toString = function()
{
	ts = new QTextStream( this, QIODevice.ReadOnly );
	return ts.readAll();
}

function CleanUpScript()
{
	Log("CleanUpScript started...");
	ConnectDisconnectScriptFunctions(false);
	GDCF_Cleanup();
	tr=null;
	tmpByteArray=null;
	tmpStringList=null;
	tmpString=null;
	ExecuteBinairy=null;
	processReadyRead=null;
	QByteArray.prototype.toString=null;
	InstallProcess=null;
	ConnectDisconnectScriptFunctions=null;
	processFinished=null;
	CleanUpScript=null;
	getString = null;
	Log("CleanUpScript exited Successfully!");
	StimulGL.cleanupScript();
}

function ConnectDisconnectScriptFunctions(Connect)
{
	if(Connect)
	{
		Log("Connecting Signal/Slots...");
		try 
		{	
			InstallProcess.readyRead.connect(this, this.processReadyRead);	
			InstallProcess["finished(int,QProcess::ExitStatus)"].connect(this, this.processFinished);	 
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e);
		}		
	}
	else
	{
		Log("Disconnecting Signal/Slots...");
		try 
		{	
			InstallProcess.readyRead.disconnect(this, this.processReadyRead);
			InstallProcess.finished.disconnect(this, this.processFinished);	 
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
		}		
	}	
}

function tr(s) { return s; }

function getString(title,text,defaultValue)
{
	tmpString = QInputDialog.getText(this, tr(title), tr(text), QLineEdit.Normal, tr(defaultValue), Qt.WindowFlags(0));
	if (tmpString != null && tmpString.length != 0)
	{
		return tr(tmpString);
	}
	else
	{	
		return tr("");
	}
}

function processReadyRead()
{
	tmpByteArray = InstallProcess.readAll();
	Log(tmpByteArray.toString());	
}

function processFinished()
{
	Log("processFinished() called!");
	Log("** STEP "  + nCounter + " ** PASSED!");
	bFileProccessed = true;
	if(bDoCleanup)
	{
		CleanUpScript();
	}
}

function ExecuteBinairy(binPath, argList)
{
	InstallProcess.start(binPath,argList);//sQTDirWin32 + "bin/qhelpgenerator", tmpStringList);
	return InstallProcess.waitForStarted();
}

ConnectDisconnectScriptFunctions(true);
//Log(getString("This is an Title", "Some Text....","default x"));

bDoCleanup = false;
sBinairySteps = 1;//6;
for(nCounter=1;nCounter<=sBinairySteps;nCounter++)
{
	bFileProccessed = false;
	bSkipStep = false;
	
	Log("** STEP "  + nCounter + " **");
	if(nCounter==1)
	{
//		tmpStringList = new Array();//Reset list
//		tmpStringList[0] = "E:\\Projects\\StimulGL documentation\\Doxygen\\ExperimentManager.cfg";
//		sBinairyPath = "doxywizard";

		fileDest = "ExperimentManager.cfg";
		changeSet = GDCF_CreateArray(7,2); //See the doxygen template document for parameter descriptions!!
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"ExperimentManager script class\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"2.0.0.2\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The Experiment Manager script class definition.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/ExperimentManagerPlugin\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/Plugins/ExperimentManager/ExperimentManager.cpp \\\n" +
		                          "../../StimulGL/Source/Plugins/ExperimentManager/ExperimentManager.h \\\n" +
		                          "../../StimulGL/Source/Plugins/ExperimentManager/Global.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";			  
		
		GDCF_ReplaceInFiles(fileSource,fileDest,changeSet);
		//doxygen -w html header.html footer.html stylesheet.css <config_file>
		tmpStringList = new Array();//Reset list
		tmpStringList[0] =  "E:\\Projects\\StimulGL documentation\\Doxygen\\ExperimentManager.cfg";
		sBinairyPath = "doxygen";	
		Log("DOXY");
	}
	else if (nCounter==2)
	{
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = "E:\\Projects\\StimulGL documentation\\Doxygen\\ParallelPortDevice.cfg";
		sBinairyPath = "doxywizard";		
	}
	else if (nCounter==3)
	{
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = "E:\\Projects\\StimulGL documentation\\Doxygen\\USBHIDDevice.cfg";
		sBinairyPath = "doxywizard";		
	}
	else if(nCounter==4)
	{
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = "E:\\Projects\\StimulGL documentation\\Doxygen\\KeyBoard.cfg";
		sBinairyPath = "doxywizard";
	}
	else if (nCounter==5)
	{
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = "E:\\Projects\\StimulGL documentation\\Doxygen\\SerialPortDevice.cfg";
		sBinairyPath = "doxywizard";		
	}
	else if (nCounter==6)
	{
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = "E:\\Projects\\StimulGL documentation\\Doxygen\\DefaultQML.cfg";
		sBinairyPath = "doxywizard";		
	}	
	
	if(!bSkipStep)
	{
		if(ExecuteBinairy(sBinairyPath,tmpStringList))
		{
			Log("ExecuteBinairy started Successfully!");
			if(nCounter == sBinairySteps)
				bDoCleanup = true;
			while(!bFileProccessed)
			{
				StimulGL.processEvents();
				//Log("Waiting...");
			}
		}
		else
		{
			Log("ExecuteBinairy Failed!");
			CleanUpScript();
		}
	}
	else
	{
		Log("** STEP "  + nCounter + " ** SKIPPED");
		continue;
	}
}
