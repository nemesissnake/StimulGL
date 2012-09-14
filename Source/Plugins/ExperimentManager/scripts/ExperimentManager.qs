var ExperimentManagerObj = new ExperimentManager();

function CleanupScript()
{
	ConnectDisconnectScriptFunctions(false);
	ConnectDisconnectScriptFunctions = null;
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;
	ExperimentStructureChanged = null;
	CleanupScript = null;
	StimulGL.cleanupScript();
	Log("CleanupScript() exited successfully")
}

function ConnectDisconnectScriptFunctions(Connect)
{
	if(Connect)
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
			ExperimentManagerObj.WriteToLogOutput.connect(this,this.Log);//For more extensive debugging information
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e);
		}		
	}
	else
	{
		Log("... Disconnecting Signal/Slots");
		try 
		{	
			ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.ExperimentStateChanged);
			ExperimentManagerObj.WriteToLogOutput.disconnect(this,this.Log);//For more extensive debugging information
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
		}		
	}	
}

function ExperimentStructureChanged()
{
//	Log("--- ExperimentStructureChanged arguments count: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//		Log("\t argument(" + i + "):" + arguments[i]);
	
	var nCurrentBlock = arguments[0];
	var nCurrentTrial = arguments[1];
	var nCurrentTrigger = arguments[2];
	var textToShow = "->-> Block " + nCurrentBlock + ", Trial " + nCurrentTrial + ", Trigger " + nCurrentTrigger;		
	Log(textToShow);
}

function ExperimentStateChanged()
{
	//ExperimentManager_NoState	= 0,	This signal can't be catched in this script
	//ExperimentManager_Constructed	= 1, 	This signal can't be catched in this script
	//ExperimentManager_Loaded	= 2, 
	//ExperimentManager_Configured	= 3, 
	//ExperimentManager_Initialized	= 4, 	
	//ExperimentManager_IsStarting	= 5, 
	//ExperimentManager_Started	= 6, 
	//ExperimentManager_IsStopping	= 7, 
	//ExperimentManager_Stopped	= 8  
	
	//Log("mySignalFunction arguments count: " + arguments.length);
	//for (var i = 0; i < arguments.length; ++i)
	//	Log("mySignalFunction argument: " + arguments[i]); 
	
	var arg = arguments[0];
	if(arg == 4)
	{	
		Log("--- ExperimentManager_Initialized(" + arg + ")");
	}	
	if(arg == 8)
	{
		Log("--- ExperimentManager_Stopped(" + arg + ")");
		CleanupScript();
	}
}

ConnectDisconnectScriptFunctions(true);
ExperimentManagerObj.setExperimentFileName(StimulGL.getApplicationRootDirPath() + "/examples/experiments/Polar1.exml");
if(!ExperimentManagerObj.runExperiment())
	CleanupScript();

//Polar1.exml
//Polar2.exml
//Eccentricity1.exml
//Eccentricity2.exml
//MovingBar1.exml
//MovingBar2.exml
//MovingDots1.exml
