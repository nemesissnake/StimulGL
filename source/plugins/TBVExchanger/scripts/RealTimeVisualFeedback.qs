var scriptRootPath = StimulGL.getSelectedScriptFileLocation();
var exmlFilePath = scriptRootPath + "/" + "FingerTapping.exml";
var qmlFilePath = scriptRootPath + "/" + "ActivationBar.qml";
var StimulusHeightSpan = 1050;
var StimulusWidthSpan = 1050;
var cExperimentStructure_Object = new cExperimentStructure();
var currentExperimentState;
var ExperimentManagerObj = new ExperimentManager();
var arg;
var tmpString = new String();
var retVal;
var Param1;
var functionName = new String();
var MainItem = new String();
var scannerVolumeDuration = 2000;
var arrExperimentModes = new Array("Testing_Mode", "Scanner_Mode");
var currentExperimenMode = arrExperimentModes[1];
var parPortAddress = 8209;
var parPortMask = 32;
var parPortMethod = 2;
var parPortPostLHDelay = 100;
var parPortPostHLDelay = 100;
var tmpLoopID;
var tmpLoopCounter;
var tmpBlockID;
var cLoopStructure_Object0;
var cBlockStructure_Object0;
//Construct a new Plugin object
var TBVExchangerobject = new TBVExchanger();
var fMeanROI;

function executePreStep()
{
	Log("executePreStep");
}

function executePostStep()
{
	Log("executePostStep");	
	MainItem = "functions";
	functionName = "setActivationLevel";		
	fMeanROI = TBVExchangerobject.tGetMeanOfROI(0);
	Log("tGetMeanOfROI() returned: " + fMeanROI);
	Param1 = fMeanROI;
	retVal = QML2Viewer_Object_1.invokeQml2Method(MainItem,functionName,Param1);
	Log("<<< " + functionName + "(" + Param1 + ") returned: " + retVal + " >>>");	
}

function executePostRun()
{
	Log("executePostRun");	
}

function disconnected()
{
	Log("disconnected");	
}

function connected()
{
	Log("connected");	
}

function connectionError(sError)
{
	Log("connectionError: " + sError);	
}

function initExperimentObjects()
{
	if(currentExperimenMode == "Testing_Mode")
	{
		scannerVolumeDuration = 100;
	}
	//else if(currentExperimenMode == "Scanner_Mode")
	//{
		
	//}	
}

function CleanupScript()
{
	ConnectDisconnectScriptFunctions(false);
	Log(TBVExchangerobject.deactivateAutoConnection());
	functionName = null;
	MainItem = null;
	ExperimentManagerObj = null;
	ExperimentStateChanged = null;
	NewInitBlockTrial = null;
	ConnectDisconnectScriptFunctions = null;
	CleanupScript = null;
	tmpString = null;
	NewQMLSourceLoaded = null;
	initExperimentObjects = null;
	startExperimentObjects = null;
	stopExperimentObjects = null;
	getItem = null;
	changeStimuli = null;
	externalTriggerRecieved = null;
	LogExperimentState = null;
	cExperimentStructure_Object = null;
	cLoopStructure_Object0 = null;
	cBlockStructure_Object0 = null;
	TBVExchangerobject = null;
	StimulGL.cleanupScript();
}

function externalTriggerRecieved()
{
	if(currentExperimenMode == "Testing_Mode")
	{
		Log("-----------------------------------------");
		Log("-- externalTriggerRecieved called");
		Log("-----------------------------------------");
	}
	currentExperimentState = cExperimentStructure_Object.getCurrentExperimentState();
	if(currentExperimentState.CurrentBlock_BlockID>=0)
	{
		cBlockStructure_Object0 = cExperimentStructure_Object.getBlockPointerByID(currentExperimentState.CurrentBlock_BlockID);
		if(currentExperimentState.CurrentBlock_LoopID>=0)
		{
			cLoopStructure_Object0 = cBlockStructure_Object0.getLoopPointerByID(currentExperimentState.CurrentBlock_LoopID);
		}
	}
	LogExperimentState();	
}

function LogExperimentState()
{
	tmpLoopCounter = -1;
	Log("-----");
	if(currentExperimentState.CurrentBlock_BlockID >= 0)
	{
		Log("CurrentBlock_BlockID:" + currentExperimentState.CurrentBlock_BlockID);
		Log("CurrentBlock_BlockNumber:" + cBlockStructure_Object0.getBlockNumber());
		Log("CurrentBlock_TrialNumber:" + currentExperimentState.CurrentBlock_TrialNumber);
		Log("CurrentBlock_InternalTrigger:" + currentExperimentState.CurrentBlock_InternalTrigger);
		Log("CurrentBlock_ExternalTrigger:" + currentExperimentState.CurrentBlock_ExternalTrigger);
		Log("Experiment_ExternalTrigger:" + currentExperimentState.Experiment_ExternalTrigger);		
		if (currentExperimentState.CurrentBlock_LoopID == -4)
		{
			Log("CurrentBlock_LoopID: RA_UNDEFINED");
		}
		else if (currentExperimentState.CurrentBlock_LoopID >= 0)
		{
			Log("CurrentBlock_LoopID:" + currentExperimentState.CurrentBlock_LoopID);
			tmpLoopCounter = cLoopStructure_Object0.getLoopCounter();
			Log("CurrentBlock_LoopCounter0:" + tmpLoopCounter);	
		}
		Log("-----");
	}
	if(currentExperimenMode == "Testing_Mode")
	{	
		MainItem = "DebugMode_Functions";
		functionName = "updateExperimentState";
		retVal = QML2Viewer_Object_1.invokeQml2Method(MainItem,functionName,currentExperimentState.CurrentBlock_BlockID,currentExperimentState.CurrentBlock_TrialNumber,currentExperimentState.CurrentBlock_InternalTrigger,tmpLoopCounter);
		Log("    <<< " + functionName + " returned: " + retVal + " >>>");	
	}
}

function NewInitBlockTrial()
{
	if(currentExperimenMode == "Testing_Mode")
	{
		Log("-----------------------------------------");
		Log("--- Starting to initialize a new Block Trial (" + currentExperimentState.CurrentBlock_TrialNumber + ")");
		Log("-----------------------------------------");
	}
	if(currentExperimentState.CurrentBlock_BlockID%2==0)
	{
		changeStimuli("fixation");		
	}
	else if(currentExperimentState.CurrentBlock_BlockID==1)
	{
		changeStimuli("righthand");
	}
	else// if(currentExperimentState.CurrentBlock_TrialNumber%4>0)
	{
		changeStimuli("lefthand");
	}	
}

function NewQMLSourceLoaded(sSource)
{
	changeStimuli("+");
	if(currentExperimenMode == "Testing_Mode")
	{
		Log("-----------------------------------------");
		Log("NewQMLSourceLoaded: " + sSource);
		Log("-----------------------------------------");
		MainItem = "DebugMode_Functions";
		functionName = "enableDebugMode";
		retVal = QML2Viewer_Object_1.invokeQml2Method(MainItem,functionName);
		Log("    <<< " + functionName + " returned: " + retVal + " >>>");		
	}	
}

function changeStimuli(strText)
{
	functionName = "setStimuliFunction";//"setTextFunction";
	MainItem = "functions";
	Param1 = strText;
	retVal = QML2Viewer_Object_1.invokeQml2Method(MainItem,functionName,Param1);
	Log("<<< " + functionName + "(" + Param1 + ") returned: " + retVal + " >>>");
	
	//var fMeanROI = TBVExchangerobject.tGetMeanOfROI(0);
	//Log("tGetMeanOfROI() returned: " + fMeanROI);
	//functionName = "setActivationLevel";
	//Param1 = fMeanROI;
	//retVal = QML2Viewer_Object_1.invokeQml2Method(MainItem,functionName,Param1);
	//Log("<<< " + functionName + "(" + Param1 + ") returned: " + retVal + " >>>");
}

function ExperimentStateChanged(currentState)
{
	var textToShow = "--- The Experiment State Changed to: ";
	
	if(currentState == ExperimentManager.ExperimentState.ExperimentManager_Initialized)
	{
		Log(textToShow + "ExperimentManager_Initialized");
		//Now all defined objects in the experiment file are constructed and therefore available in this script, so now we can make the connections between constructed the objects.
		cExperimentStructure_Object = ExperimentManagerObj.getExperimentStructure();
		currentExperimentState = cExperimentStructure_Object.getCurrentExperimentState();
		ConnectDisconnectScriptFunctions(true);	
	}	
	else if(currentState == ExperimentManager.ExperimentState.ExperimentManager_IsStarting)
	{	
		Log(textToShow + "ExperimentManager_IsStarting");
		initExperimentObjects();
		startExperimentObjects();
	}
	else if(currentState == ExperimentManager.ExperimentState.ExperimentManager_IsStopping)
	{	
		Log(textToShow + "ExperimentManager_IsStopping");
		stopExperimentObjects();
	}	
	else if(currentState == ExperimentManager.ExperimentState.ExperimentManager_Stopped)
	{
		Log(textToShow + "ExperimentManager_Stopped");
		CleanupScript();
	}
}

function startExperimentObjects()
{
	if(currentExperimenMode == "Testing_Mode")
	{
		Timer_Object_1.startTimer(scannerVolumeDuration);
	}
	else if(currentExperimenMode == "Scanner_Mode")
	{
		ParallelPort_Object_1.StartCaptureThread(parPortAddress,parPortMask,parPortMethod,parPortPostLHDelay,parPortPostHLDelay);
	}
}

function stopExperimentObjects()
{
	if(currentExperimenMode == "Testing_Mode")
	{	
		Timer_Object_1.stopTimer();
	}
	else if(currentExperimenMode == "Scanner_Mode")
	{
		ParallelPort_Object_1.StopCaptureThread();
	}
}

function ConnectDisconnectScriptFunctions(Connect)
{
	if(Connect)
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			QML2Viewer_Object_1.NewInitBlockTrial.connect(this, this.NewInitBlockTrial);
			QML2Viewer_Object_1.NewSourceLoaded.connect(this,this.NewQMLSourceLoaded);
			cExperimentStructure_Object.externalTriggerRecieved.connect(this, externalTriggerRecieved);
			
			TBVExchangerobject.executePreStep.connect(this, this.executePreStep);  
			TBVExchangerobject.executePostStep.connect(this, this.executePostStep);  
			TBVExchangerobject.executePostRun.connect(this, this.executePostRun);  
			TBVExchangerobject.disconnected.connect(this, this.disconnected);  
			TBVExchangerobject.connected.connect(this, this.connected);  
			TBVExchangerobject.connectionError.connect(this, this.connectionError);  			
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
			QML2Viewer_Object_1.NewInitBlockTrial.disconnect(this, this.NewInitBlockTrial);
			QML2Viewer_Object_1.NewSourceLoaded.disconnect(this,this.NewQMLSourceLoaded);
			cExperimentStructure_Object.externalTriggerRecieved.disconnect(this, externalTriggerRecieved);
			
			TBVExchangerobject.executePreStep.disconnect(this, this.executePreStep);  
			TBVExchangerobject.executePostStep.disconnect(this, this.executePostStep);  
			TBVExchangerobject.executePostRun.disconnect(this, this.executePostRun);  
			TBVExchangerobject.disconnected.disconnect(this, this.disconnected);  
			TBVExchangerobject.connected.disconnect(this, this.connected);  
			TBVExchangerobject.connectionError.disconnect(this, this.connectionError);  			
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e);
		}		
	}	
}

function getItem(title,text,items,current)
{
	var item = QInputDialog.getItem(this, title, text, items, current, false, Qt.WindowFlags(0));
	return item;
}

currentExperimenMode = getItem("Choose the Experiment mode","Experiment mode:",arrExperimentModes,0);
if(currentExperimenMode === null)
{
	bContinue = false;
	CleanupScript();
}
else
{
	Log("Choosen experiment mode = " + currentExperimenMode);
	ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.ExperimentStateChanged);
	if(ExperimentManagerObj.setExperimentFileName(exmlFilePath))
	{
		Log("activateAutoConnection() = " + TBVExchangerobject.activateAutoConnection());
		ExperimentManagerObj.runExperiment();
	}
	else
	{
		Log("*** Error: Could not locate the experiment file!");
	}
}












































//This function is called whenever the user presses a key
//function KeyCaptureDetectFunction(keyCode)
//{
//	Log("A key press was detected: " + keyCode);
	//KeyPressedCounter++;
	//Log("KeyPressedCounter incremented to: " + KeyPressedCounter + "\n");
	//if (KeyPressedCounter==4)
	//{
	//	KeyBoardCaptureObj.StopCaptureThread();		
	//}
//	if(keyCode == 27)
//	{
//		
//		CleanupScript();//escape key
//	}	
//}
//
//function CleanupScript() //Cleanup the script
//{
	//Disconnect the signal/slots
//	ConnectDisconnectScriptFunctions(false);
	//Set all functions and constructed objects to null
//	executePreStep = null;
//	executePostStep = null;
//	executePostRun = null;
//	disconnected = null;
//	connected = null;
//	connectionError = null;	
//	ConnectDisconnectScriptFunctions = null;
//	TBVExchangerobject = null;
//	KeyBoardCaptureObj = null;
//	KeyCaptureDetectFunction = null;
//	CleanupScript = null;
	//Write something to the Log Output Pane so we know that this Function executed successfully.
//	Log("Finished script cleanup, ready for garbage collection!");
//	StimulGL.cleanupScript();
//}




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//ConnectDisconnectScriptFunctions(true);

//Start the capture thread
//KeyBoardCaptureObj.StartCaptureThread(0, false);
//StartCaptureThread(const short method, bool keyForwarding)
//method == 0 --> KeyPressed
//method == 1 --> KeyReleased
//method == 2 --> KeyPressedReleased

//	Basic Project Functions
//Log(TBVExchangerobject.tGetCurrentTimePoint());
//Log(TBVExchangerobject.tGetExpectedNrOfTimePoints());
//Log(TBVExchangerobject.tGetDimsOfFunctionalData());
//Log(TBVExchangerobject.tGetProjectName());
//Log(TBVExchangerobject.tGetWatchFolder());
//Log(TBVExchangerobject.tGetTargetFolder());
//Log(TBVExchangerobject.tGetFeedbackFolder());
//	Protocol, DM, GLM Functions:
//Log(TBVExchangerobject.tGetCurrentProtocolCondition());
//Log(TBVExchangerobject.tGetFullNrOfPredictors());
//Log(TBVExchangerobject.tGetCurrentNrOfPredictors());
//Log(TBVExchangerobject.tGetNrOfConfoundPredictors());
//Log(TBVExchangerobject.tGetValueOfDesignMatrix(1, 2));
//Log(TBVExchangerobject.tGetNrOfContrasts());
//	ROI Functions
//Log(TBVExchangerobject.tGetNrOfROIs());
//Log(TBVExchangerobject.tGetMeanOfROI(2));
//Log(TBVExchangerobject.tGetNrOfVoxelsOfROI(2));
//Log(TBVExchangerobject.tGetBetaOfROI(1,2));
//Log(TBVExchangerobject.tGetCoordsOfVoxelOfROI(1, 2));
//Log(TBVExchangerobject.tGetAllCoordsOfVoxelsOfROI(1));
//	Volume Data Access Functions
//Log(TBVExchangerobject.tGetValueOfVoxelAtTime(1,2,3,4));
//Log(TBVExchangerobject.tGetTimeCourseData(1));
//Log(TBVExchangerobject.tGetRawTimeCourseData(1));
//Log(TBVExchangerobject.tGetBetaOfVoxel(1,2,3,4));
//Log(TBVExchangerobject.tGetBetaMaps());
//Log(TBVExchangerobject.tGetMapValueOfVoxel(1,2,3,4));// {return tbvNetwIntFace->tGetMapValueOfVoxel(map, x, y, z);};
//Log(TBVExchangerobject.tGetContrastMaps());
//Log(TBVExchangerobject.activateAutoConnection());
//Log(TBVExchangerobject.connectToServer("127.0.0.1",80));
//Log(TBVExchangerobject.disconnectFromServer());

//CleanupScript();