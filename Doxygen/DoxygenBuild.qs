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
var sBinairyPath = "doxygen";
var sDoxyFileDir = sScriptPath + "/";

Include("GenerateDoxyConfigFile.qs");//Make sure to call the cleanup when not needed anymore!
Include("../js/StimulGL.js");//Make sure to call the cleanup when not needed anymore!

var StimulGL_Info = new StimulGL_Information();
var CurrentStimulGLRelease = StimulGL_Info.GetCurrentRelease();
var CurrentStimulGLReleaseIndex = StimulGL_Info.GetNumberOfReleases()-1;
var LastComponentIndex;
var LastComponentReleaseIndex;
var LastComponentVersion;
var ComponentName;

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
	StimulGL_Cleanup();
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
	Beep();
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
sBinairySteps = 7;
for(nCounter=1;nCounter<=sBinairySteps;nCounter++)
{
	bFileProccessed = false;
	bSkipStep = false;
	
	Log("** STEP "  + nCounter + " **");
	
		
	changeSet = StimulGL_CreateArray(8,2); //See the doxygen template document for parameter descriptions!!
	if(nCounter==1)
	{
		ComponentName = "ExperimentManagerPlugin";		
		Log("Component Name: " + ComponentName);
		LastComponentReleaseIndex = StimulGL_Info.GetLatestComponentReleaseByName(ComponentName);
		Log("Last Component Release Index: " + LastComponentReleaseIndex);
		LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		Log("Last Component Index: " + LastComponentIndex);
		LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"ExperimentManager script class\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The Experiment Manager script class reference.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/ExperimentManagerPlugin\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/Plugins/ExperimentManager/ExperimentManager.cpp \\\n" +
		                          "../../StimulGL/Source/Plugins/ExperimentManager/ExperimentManager.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/qmlWidget.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/qmlWidget.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/GLWidgetWrapper.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/GLWidgetWrapper.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/retinomap_glwidget.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/retinomap_glwidget.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/TriggerTimer.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/TriggerTimer.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/prtformatmanager.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/prtformatmanager.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/ImageProcessor.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/ImageProcessor.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/RandomGenerator.h \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/RandomGenerator.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ExperimentManager/Global.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";
		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
		changeSet[7][1] = "";		
	}
	else if (nCounter==2)
	{
		ComponentName = "ParallelPortPlugin";		
		Log("Component Name: " + ComponentName);
		LastComponentReleaseIndex = StimulGL_Info.GetLatestComponentReleaseByName(ComponentName);
		Log("Last Component Release Index: " + LastComponentReleaseIndex);
		LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		Log("Last Component Index: " + LastComponentIndex);
		LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"Parallel Port Device script class\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"Parallel Port Device script class reference.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/ParallelPortDevicePlugin\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/Plugins/ParallelPortDevice/ParallelPort.h \\\n" +
					  "../../StimulGL/Source/Plugins/ParallelPortDevice/ParallelPort.cpp \\\n" +
					  "../../StimulGL/Source/Plugins/ParallelPortDevice/Global.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";	
		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
		changeSet[7][1] = "";		
	}
	else if (nCounter==3)
	{
		ComponentName = "SerialPortPlugin";		
		Log("Component Name: " + ComponentName);
		LastComponentReleaseIndex = StimulGL_Info.GetLatestComponentReleaseByName(ComponentName);
		Log("Last Component Release Index: " + LastComponentReleaseIndex);
		LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		Log("Last Component Index: " + LastComponentIndex);
		LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"Serial Port Device script class\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The Serial Port Device script class reference.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/SerialPortDevicePlugin\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/Plugins/SerialPortDevice/SerialPortDevice.h \\\n" +
		                          "../../StimulGL/Source/Plugins/SerialPortDevice/SerialPortDevice.cpp \\\n" +
		                          "../../StimulGL/Source/Plugins/SerialPortDevice/Global.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";
		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
		changeSet[7][1] = "Q_OS_WIN";
	}
	else if (nCounter==4)
	{
		ComponentName = "USBHIDDevicePlugin";		
		Log("Component Name: " + ComponentName);
		LastComponentReleaseIndex = StimulGL_Info.GetLatestComponentReleaseByName(ComponentName);
		Log("Last Component Release Index: " + LastComponentReleaseIndex);
		LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		Log("Last Component Index: " + LastComponentIndex);
		LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\USB HID Device script class\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The USB HID Device script class reference.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/USBHIDDevicePlugin\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/Plugins/USBHIDDevice/USBHIDDevice.cpp \\\n" +
		                          "../../StimulGL/Source/Plugins/USBHIDDevice/USBHIDDevice.h \\\n" +
		                          "../../StimulGL/Source/Plugins/USBHIDDevice/Global.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";
		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
		changeSet[7][1] = "";
	}
	else if (nCounter==5)
	{
		ComponentName = "KeyBoardPlugin";		
		Log("Component Name: " + ComponentName);
		LastComponentReleaseIndex = StimulGL_Info.GetLatestComponentReleaseByName(ComponentName);
		Log("Last Component Release Index: " + LastComponentReleaseIndex);
		LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		Log("Last Component Index: " + LastComponentIndex);
		LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"KeyBoard Device script class\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The KeyBoard Device script class reference.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/KeyBoardDevicePlugin\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/Plugins/KeyBoardDevice/KeyBoardCapture.cpp \\\n" +
		                          "../../StimulGL/Source/Plugins/KeyBoardDevice/KeyBoardCapture.h \\\n" +
		                          "../../StimulGL/Source/Plugins/KeyBoardDevice/Global.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";	
		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
		changeSet[7][1] = "";	
	}
	else if (nCounter==6)
	{
		ComponentName = "StimulGL_QMLExtensions";		
		Log("Component Name: " + ComponentName);
		LastComponentReleaseIndex = StimulGL_Info.GetLatestComponentReleaseByName(ComponentName);
		Log("Last Component Release Index: " + LastComponentReleaseIndex);
		LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		Log("Last Component Index: " + LastComponentIndex);
		LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"StimulGL QML Extensions\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The StimulGL QML Extensions class references.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/QtQuick/StimulGL_QMLExtensions\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/QmlExtensions/Plugins/DefaultPlugin/PhononPlayer.h \\\n" +
					  "../../StimulGL/Source/QmlExtensions/Plugins/DefaultPlugin/PhononPlayer.cpp \\\n" //+
					  //"../../StimulGL/Source/QmlExtensions/Plugins/DefaultPlugin/TimeModel.h \\\n" +
		                          //"../../StimulGL/Source/QmlExtensions/Plugins/DefaultPlugin/TimeModel.cpp \\\n" +
		                          //"../../StimulGL/Source/QmlExtensions/Plugins/DefaultPlugin/defines.h \\\n";
		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
		changeSet[6][1] = "StimulGL.doxygen.Project";
		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
		changeSet[7][1] = "";
	}
	else if (nCounter==7)
	{
		ComponentName = "StimulGLMainApplication";		
		//Log("Component Name: " + ComponentName);
		LastComponentVersion = StimulGL_Info.GetCurrentRelease();
		Log("StimulGL Release Index: " + LastComponentVersion);
		//LastComponentIndex = StimulGL_Info.GetLatestComponentIndexByName(LastComponentReleaseIndex,ComponentName);
		//Log("Last Component Index: " + LastComponentIndex);
		//LastComponentVersion = StimulGL_Info.GetComponentVersionByIndexes(LastComponentReleaseIndex,LastComponentIndex);
		//Log("Last Component Version: " + LastComponentVersion);
		Log("\n");
		fileDest = ComponentName + ".cfg";
		changeSet[0][0] = preFix + "PROJECT_NAME" + postFix;
		changeSet[0][1] = "\"StimulGL Main Application\"";
		changeSet[1][0] = preFix + "PROJECT_NUMBER" + postFix;
		changeSet[1][1] = "\"" + LastComponentVersion + "\"";
		changeSet[2][0] = preFix + "PROJECT_BRIEF" + postFix;
		changeSet[2][1] = "\"The StimulGL Main Application class references.\"";
		changeSet[3][0] = preFix + "PROJECT_LOGO" + postFix;
		changeSet[3][1] = "";//No quotes for empty!
		changeSet[4][0] = preFix + "OUTPUT_DIRECTORY" + postFix;
		changeSet[4][1] = "\"../References/Script/StimulGLMainApp\"";
		changeSet[5][0] = preFix + "INPUT" + postFix;
		changeSet[5][1] = "../../StimulGL/Source/StimulGL/mainwindow.h \\\n" +
					  "../../StimulGL/Source/StimulGL/mainwindow.cpp \\\n" +
					  "../../StimulGL/Source/StimulGL/scifinddialog.h \\\n";
//		changeSet[6][0] = preFix + "QHP_NAMESPACE" + postFix;
//		changeSet[6][1] = "StimulGL.doxygen.Project";
//		changeSet[7][0] = preFix + "PREDEFINED" + postFix; //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html //see http://www.stack.nl/~dimitri/doxygen/preprocessing.html
//		changeSet[7][1] = "";		
	}
	else
	{
		bSkipStep = true;
	}
	
	if(!bSkipStep)
	{
		GDCF_ReplaceInFiles(fileSource,fileDest,changeSet);
		//doxygen -w html header.html footer.html stylesheet.css <config_file>
		tmpStringList = new Array();//Reset list
		tmpStringList[0] = sDoxyFileDir + fileDest;
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
