Include("ReplaceInFile.qs");

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
var sBinairyPath;
var changeSet;

//var sQTDirWin32 = tr("C:/Qt/4.8.0/win32/");
//var sQTDirx64 = tr("C:/Qt/4.8.0/x64/");
var sScriptPath = ToWindowsPath(StimulGL.getSelectedScriptFileLocation());
var sInstallJammerPath = StimulGL.getEnvironmentVariabele("PROGRAMFILES(x86)") + "/InstallJammer/installjammer.exe";
var strInstallConfigurationFileName = "InstallJammer_(32+64bit)";
var strInstallConfigurationFile = ToWindowsPath(StimulGL.getSelectedScriptFileLocation() + "/" + strInstallConfigurationFileName + ".mpi");
var strInstallConfigurationFileCopy = sScriptPath + "\\" + strInstallConfigurationFileName + "_tmpCopy.mpi" ;
var sInstallerAppName = tr("StimulGL");//StimulGL
var arrConfigList = new Array("win32","x64");
var sInstallerConfiguration = tr(arrConfigList[0]);//win32 or x64
var sInstallerVersion = tr("2.0.0.3");//<Major>.<Minor>.<Build>.<Revision>
var sInstallerPlatform = tr("Windows");
var sQtDirectory_default = tr("C:/Qt/5.0.0");
var sStimulGLProjectDirectory_default = tr("E:/Projects/StimulGL");
var sQtDirectory = sQtDirectory_default;
var sStimulGLProjectDirectory = sStimulGLProjectDirectory_default;
var srcFile = new QFile(strInstallConfigurationFile);
var dstFile = new QFile(strInstallConfigurationFileCopy);

QByteArray.prototype.toString = function()
{
	ts = new QTextStream( this, QIODevice.ReadOnly );
	return ts.readAll();
}

function CleanUpScript()
{
	Log("CleanUpScript started...");
	ConnectDisconnectScriptFunctions(false);
	ReplaceInFile_Cleanup();
	tr=null;
	tmpByteArray=null;
	tmpStringList=null;
	tmpString=null;
	ExecuteBinairy=null;
	ToWindowsPath = null;
	informationMessage=null;
	processReadyRead=null;
	selectItem=null;
	QByteArray.prototype.toString=null;
	InstallProcess=null;
	srcFile = null;
	dstFile = null;
	ConnectDisconnectScriptFunctions=null;
	processFinished=null;
	CleanUpScript=null;	
	Log("CleanUpScript exited successfully!");
	StimulGL.cleanupScript();
}

function getItem(title,text,items)
{
	var item = QInputDialog.getItem(this, tr(title), tr(text), items, 0, false, Qt.WindowFlags(0));
	if (item != null & item.length != 0)
		return item;
}

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

function informationMessage(message)
{
    var reply = QMessageBox.information(this, tr("Information"), tr(message));//Dialog.MESSAGE);
    if (reply == QMessageBox.Ok)
        //this.informationLabel.text = tr("OK");
	return true;
    else
        //this.informationLabel.text = tr("Escape");
    return false;
}

function ToWindowsPath(path)
{
	return path.replace( /\//gi,"\\");//  first argument = /regex/ (g=all occurrences, i=case insensitive), second = <replace_string>
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

function processReadyRead()
{
	tmpByteArray = InstallProcess.readAll();
	Log(tmpByteArray.toString());	
}

function processFinished()
{
	Log("processFinished() called!");
	//Log(__qt_sender__.workingDirectory());
	//Log(this.name);
	//Log(arguments.callee);
	//Log("processFinished() proccessed Successfully (" + __qt_sender__.name() + ", " + this.name + ", " + arguments.callee + ").");
	Log("** STEP "  + nCounter + " ** PASSED!");
	bFileProccessed = true;
	if(bDoCleanup)
	{
		CleanUpScript();
	}
}

function ExecuteBinairy(binPath, argList)
{
	InstallProcess.start(binPath,argList);
	return InstallProcess.waitForStarted();
}

ConnectDisconnectScriptFunctions(true);
//Log(getString("This is an Title", "Some Text....","default x"));

bDoCleanup = false;
sBinairySteps = 5;

for(nCounter=1;nCounter<=sBinairySteps;nCounter++)
{
	bFileProccessed = false;
	bSkipStep = false;
	
	Log("** STEP "  + nCounter + " **");
	
	if (nCounter==1)
	{
		
		sInstallerVersion = getString("Set the StimulGL version","StimulGL version:",sInstallerVersion);
		Log("StimulGL version = " + sInstallerVersion);
		
		sInstallerConfiguration = getItem("Choose the StimulGL configuration","StimulGL configuration:",arrConfigList);
		Log("StimulGL configuration = " + sInstallerConfiguration);
		
		sQtDirectory = getString("Choose the Qt directory","Qt directory:",sQtDirectory);
		Log("Qt directory = " + sQtDirectory);

		sStimulGLProjectDirectory = getString("Choose the StimulGL project directory","StimulGL project directory:",sStimulGLProjectDirectory);
		Log("StimulGL project directory = " + sStimulGLProjectDirectory);	
		
		bSkipStep = true;
	}	
	else if(nCounter==2)
	{
		if(dstFile.exists())
			dstFile.remove();//Copy Doesn't Overwrite!
		//if (srcFile.copy(strInstallConfigurationFileCopy))//Copy Doesn't Overwrite!
		//	Log("File (" + strInstallConfigurationFile + ") copied!");
		changeSet = ReplaceInFile_CreateArray(2,2);
		changeSet[0][0] = sQtDirectory_default;
		changeSet[0][1] = sQtDirectory;
		changeSet[1][0] = sStimulGLProjectDirectory_default;
		changeSet[1][1] = sStimulGLProjectDirectory;
		ReplaceInFile_ReplaceInFiles(strInstallConfigurationFile,strInstallConfigurationFileCopy,changeSet);//
		
		bSkipStep = true;
	}
	else if (nCounter==3)
	{
		informationMessage("Please remove the unwanted component(s) from the copied installer script file (except the Default and the [" + sInstallerConfiguration + "] components.");
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = strInstallConfigurationFileCopy;
		sBinairyPath = sInstallJammerPath;
		//bSkipStep = true;
	}
	else if (nCounter==4)
	{
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = "-DAppName";
		tmpStringList[1] = sInstallerAppName;
		tmpStringList[2] = "-DPlatformName";
		tmpStringList[3] = sInstallerConfiguration;
		tmpStringList[4] = "-DInstallType";
		tmpStringList[5] = sInstallerConfiguration;
		tmpStringList[6] = "-DInstallVersion";
		tmpStringList[7] = sInstallerVersion;
		tmpStringList[8] = "-DVersion"
		tmpStringList[9] = sInstallerVersion;
		tmpStringList[10] = "--platform"
		tmpStringList[11] = sInstallerPlatform;
		//Insert new options here!	
		tmpStringList[12] = "--build-for-release";
		tmpStringList[13] = "--build";
		tmpStringList[14] = strInstallConfigurationFileCopy;
		sBinairyPath = sInstallJammerPath;
		//bSkipStep = true;
	}
	else if (nCounter==5)
	{
		if(dstFile.exists())
		{
			if (dstFile.remove())//Copy Doesn't Overwrite!
				Log("Copied file removed");
			else
				Log("Couldn't remove the Copied file!");
		}
		else
		{
			Log("File doesn't exist!");
		}
		bSkipStep = true;
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
			break;
		}
	}
	else
	{
		Log("** STEP "  + nCounter + " ** SKIPPED");
		if(nCounter == sBinairySteps)
			CleanUpScript();		
		continue;
	}
}
