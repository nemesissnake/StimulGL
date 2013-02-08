// This example shows how you can make use of the Experiment Manager.

var cExperimentStructure_Object = new cExperimentStructure();
var ExperimentManagerObj = new ExperimentManager(); 				//Here we create the Experiment Manager object that can run experiments.
var currentScriptLocation = StimulGL.getSelectedScriptFileLocation();		//Here we store the directory-path from this script file for further usage.
var tmpText;											//Just a global variabele we can (re)use inside this script
var currentEMState;										//Just a global variabele that stores the latest Experiment Manager State
var TriggerTime = 4000;
//var CurrentQMLFile = "C:\\Qt\\5.0.0\\win32\\examples\\multimedia\\video\\qmlvideo\\qml\\qmlvideo\\main.qml";
var CurrentQMLFile = ".//experiments//qml2Example_1.qml";

function CleanupScript() //Cleanup the script
{
	//Disconnect the signal/slots
	ConnectDisconnectScriptFunctions(false);
	//Set all functions and constructed objects to null
	ConnectDisconnectScriptFunctions = null;
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;
	ExperimentStarted = null;
	ExperimentStopped = null;	
	EM_ExternalTriggerIncremented = null;
	LogState = null;
	cExperimentStructure_Object = null;
	CleanupScript = null;
	//Write something to the Log Output Pane so we know that this Function executed successfully.
	Log("Finished script cleanup, ready for garbage collection!");
	StimulGL.cleanupScript();
}

function ConnectDisconnectScriptFunctions(Connect)
//This function can connect or disconnect all signal/slot connections defined by the boolean parameter 
{
	if(Connect) //This parameter defines whether we should connect or disconnect the signal/slots.
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);  
			//This signal is emitted whenever the EM changes its state, we'll use it to detect changes of the current Block/Trial/Trigger
			ExperimentManagerObj.WriteToLogOutput.connect(this,this.Log);
			//This signal emits usefull and extensive debugging information, we'll connect it to the Log function to make sure that we can see it in the Log output Pane
		} 
		catch (e) 
		{
			Log(".*. Something went wrong connecting the Signal/Slots:" + e); //If a connection fails warn the user!
		}		
	}
	else
	{
		Log("... Disconnecting Signal/Slots");
		try 
		{	
			ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.ExperimentStateChanged);
			ExperimentManagerObj.WriteToLogOutput.disconnect(this,this.Log);
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e); //If a disconnection fails warn the user!
		}		
	}	
}

function ExperimentStarted()
{
	Log("-*- Experiment Started");
}

function ExperimentStopped()
{
	Log("-*- Experiment Stopped");
}

function EM_ExternalTriggerIncremented() 
{
	Log("*** EM_ExternalTriggerIncremented()");
	LogState(cExperimentStructure_Object.getCurrentExperimentState());
}

function LogState(currentExperimentState)
{
	Log("-----");
	var tmpLoopID = currentExperimentState.CurrentBlock_LoopID;
	if (tmpLoopID == -4)
		Log("CurrentBlock_LoopID: RA_UNDEFINED");
	else
		Log("CurrentBlock_LoopID:" + currentExperimentState.CurrentBlock_LoopID);
	Log("CurrentBlock_BlockID:" + currentExperimentState.CurrentBlock_BlockID);
	Log("CurrentBlock_TrialNumber:" + currentExperimentState.CurrentBlock_TrialNumber);
	Log("CurrentBlock_InternalTrigger:" + currentExperimentState.CurrentBlock_InternalTrigger);
	Log("CurrentBlock_ExternalTrigger:" + currentExperimentState.CurrentBlock_ExternalTrigger);
	Log("Experiment_ExternalTrigger:" + currentExperimentState.Experiment_ExternalTrigger);
	Log("-----");
}

function ExperimentStateChanged(currentState)
{
//The signal where this slot is connected to contains 1 parameters (that holds the current Experiment Manager State).
//This state is represented using a number following this table:
//
//	ExperimentManager_NoState	= 0,	This signal can't be catched in this script
//	ExperimentManager_Constructed	= 1, 	This signal can't be catched in this script
//	ExperimentManager_Loaded		= 2, 
//	ExperimentManager_Configured	= 3, 
//	ExperimentManager_Initialized	= 4, 	
//	ExperimentManager_IsStarting	= 5, 
//	ExperimentManager_Started	= 6, 
//	ExperimentManager_IsStopping	= 7, 
//	ExperimentManager_Stopped	= 8  
	
	currentEMState = currentState;
	Log(ExperimentManagerObj.getCurrentExperimentState());	
	if(currentEMState == 5)
	{
		cExperimentStructure_Object = ExperimentManagerObj.getExperimentStructure();
		cExperimentStructure_Object.experimentStarted.connect(this, ExperimentStarted);
		cExperimentStructure_Object.experimentStopped.connect(this, ExperimentStopped);
		cExperimentStructure_Object.externalTriggerRecieved.connect(this, EM_ExternalTriggerIncremented);		
	}
	else if(currentEMState == 8)
	{
		cExperimentStructure_Object.experimentStarted.disconnect(this, ExperimentStarted);
		cExperimentStructure_Object.experimentStopped.disconnect(this, ExperimentStopped);
		cExperimentStructure_Object.externalTriggerRecieved.disconnect(this, EM_ExternalTriggerIncremented);
		CleanupScript();
		//Whenever the experiment is stopped (or aborted) we make sure here that we'll end the script nicely
	}	
}




ConnectDisconnectScriptFunctions(true); //Connect the signal/slots
ExperimentManagerObj.setExperimentFileName(currentScriptLocation + "/experiments/qml2Example.exml"); //Set the experiment file
if(!ExperimentManagerObj.runExperiment()) //Try to run the experiment
	CleanupScript(); //If the experiment fails to run exit the script nicely