// This example shows how you can make use of the Experiment Manager.

var ExperimentManagerObj = new ExperimentManager(); 				//Here we create the Experiment Manager object that can run experiments.
var currentScriptLocation = StimulGL.getSelectedScriptFileLocation();		//Here we store the directory-path from this script file for further usage.
var tmpText;											//Just a global variabele we can (re)use inside this script
var currentEMState;										//Just a global variabele that stores the latest Experiment Manager State

function CleanupScript() //Cleanup the script
{
	//Disconnect the signal/slots
	ConnectDisconnectScriptFunctions(false);
	//Set all functions and constructed objects to null
	ConnectDisconnectScriptFunctions = null;
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;
	ExperimentStructureChanged = null;
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

function ExperimentStructureChanged(nCurrentBlock,nCurrentTrial,nCurrentTrigger)
{
//The signal where this slot is connected to contains 3 parameters (Block,Trial and Trigger) we can examine the parameters that are emitted by:
//
//	Log("--- ExperimentStructureChanged arguments count: " + arguments.length);
//	for (var i = 0; i < arguments.length; ++i)
//		Log("\t argument(" + i + "):" + arguments[i]);
//		
//Alternatively you could use the following function signature and code to make use of the parameters:
//function ExperimentStructureChanged()//No parameters defined in the function header
//{	
//	var nCurrentBlock = arguments[0];
//	var nCurrentTrial = arguments[1];
//	var nCurrentTrigger = arguments[2];
	
	
	tmpText = "->-> Block " + nCurrentBlock + ", Trial " + nCurrentTrial + ", Trigger " + nCurrentTrigger;
	// Here we make use of the function signature parameters to construct	a string containing the current experiment structure information
	Log(tmpText);
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
	if(currentEMState == 4)
	{

	}
	else if(currentEMState == 8)
	{
		CleanupScript();
		//Whenever the experiment is stopped (or aborted) we make sure here that we'll end the script nicely
	}	
}




ConnectDisconnectScriptFunctions(true); //Connect the signal/slots
ExperimentManagerObj.setExperimentFileName(currentScriptLocation + "/experiments/PolarAngle.exml"); //Set the experiment file
if(!ExperimentManagerObj.runExperiment()) //Try to run the experiment
	CleanupScript(); //If the experiment fails to run exit the script nicely