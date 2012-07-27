var ExperimentManagerObj = new ExperimentManager();

function CleanupScript()
{
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;
	ExperimentStructureChanged = null;
	ConnectDisconnectScriptFunctions = null;
	CleanupScript = null;
	StimulGL.cleanupScript();
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

ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
ExperimentManagerObj.setExperimentFileName("C:\\Program Files\\StimulGL\\experiments\\Polar1.exml");
ExperimentManagerObj.runExperiment();

//Polar1.exml
//Polar2.exml
//Eccentricity1.exml
//Eccentricity2.exml
//MovingBar1.exml
//MovingBar2.exml
//MovingDots1.exml
