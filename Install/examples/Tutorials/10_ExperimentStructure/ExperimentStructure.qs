var cExperimentStructure_Object;
var cBlockStructure_Object0;
var cBlockStructure_Object1;
var cBlockStructure_Object2;
var cBlockStructure_Object3;
var cBlockStructure_Object4;
var cLoopStructure_Object0;
var cLoopStructure_Object1;
var cLoopStructure_Object2;
var cLoopStructure_Object3;
var cLoopStructure_Object4;
var cLoopStructure_Object;
var cExperimentStructureState_Object;
var sTimer = new TriggerTimer();
var KeyBoardCaptureObj = new KeyBoardCapture(); 							//Construct a StimulGL Plugin KeyBoard Object
var ExperimentManagerObj = new ExperimentManager(); 						//Here we create the Experiment Manager object that can run experiments.
var sCurrentScriptLocation = StimulGL.getActiveDocumentFileLocation();			//Here we store the directory-path from this script file for further usage.
var sExperimentFilePath = sCurrentScriptLocation + "/ExperimentStructure.exml";

function CleanupScript()
{
	//Pre
	sTimer.stopTimer();
	KeyBoardCaptureObj.StopCaptureThread();
	StimulGL.enableActiveDocument(true);
	ConnectDisconnectExperimentStructure(false);
	ConnectDisconnectScriptFunctions(false);
	//Functions
	CreateExperimentFromScript = null;
	ShowMenuCommands = null;
	getItem = null;
	LogState = null;	
	ExperimentStarted = null;
	ExperimentStopped = null;
	ExperimentStateChanged = null;
	KeyCaptureDetectFunction = null;
	CreateBlock = null;
	CreateLoop = null;
	EM_ExperimentStateChanged = null;
	EM_ExternalTriggerIncremented = null;	
	ConnectDisconnectScriptFunctions = null;
	ConnectDisconnectExperimentStructure = null;
	CleanupScript = null;	
	//Objects
	cLoopStructure_Object0 = null;
	cLoopStructure_Object1 = null;
	cLoopStructure_Object2 = null;
	cLoopStructure_Object3 = null;
	cLoopStructure_Object4 = null;	
	cBlockStructure_Object0 = null;
	cBlockStructure_Object1 = null;
	cBlockStructure_Object2 = null;
	cBlockStructure_Object3 = null;
	cBlockStructure_Object4 = null;
	cExperimentStructure_Object = null;	
	cExperimentStructureState_Object = null;
	ExperimentManagerObj = null;
	sTimer = null;	
	KeyBoardCaptureObj = null;
	ExperimentManagerObj = null;
	//Post
	Log("\nFinished script cleanup, ready for garbage collection!");
	Beep();
	StimulGL.cleanupScript();	
}

function ConnectDisconnectExperimentStructure(Connect)
//This function can connect or disconnect all signal/slot connections defined by the boolean parameter 
{
	if(cExperimentStructure_Object)
	{
		if(Connect) //This parameter defines whether we should connect or disconnect the signal/slots.
		{
			try 
			{	
				sTimer.timeout.connect(cExperimentStructure_Object, cExperimentStructure_Object.incrementExternalTrigger);
				cExperimentStructure_Object.experimentStarted.connect(this, ExperimentStarted);
				cExperimentStructure_Object.experimentStopped.connect(this, ExperimentStopped);
				cExperimentStructure_Object.externalTriggerRecieved.connect(this, ExperimentStateChanged);
			} 
			catch (e) 
			{
				Log(".*. Something went wrong connecting the Experiment Structure:" + e); //If a connection fails warn the user!
			}
		}
		else
		{
			try 
			{	
				sTimer.timeout.disconnect(cExperimentStructure_Object, cExperimentStructure_Object.incrementExternalTrigger);
				cExperimentStructure_Object.experimentStarted.disconnect(this, ExperimentStarted);
				cExperimentStructure_Object.experimentStopped.disconnect(this, ExperimentStopped);
				cExperimentStructure_Object.externalTriggerRecieved.disconnect(this, ExperimentStateChanged);
			} 
			catch (e) 
			{
				Log(".*. Something went wrong disconnecting the Experiment Structure:" + e); //If a disconnection fails warn the user!
			}		
		}
	}
}

function ConnectDisconnectScriptFunctions(Connect)
//This function can connect or disconnect all signal/slot connections defined by the boolean parameter 
{
	if(Connect) //This parameter defines whether we should connect or disconnect the signal/slots.
	{
		Log("... Connecting Signal/Slots");
		try 
		{	
			KeyBoardCaptureObj.CaptureThreadKeyPressed.connect(this, this.KeyCaptureDetectFunction);
			ExperimentManagerObj.ExperimentStateHasChanged.connect(this, this.EM_ExperimentStateChanged);  
			ExperimentManagerObj.WriteToLogOutput.connect(this,this.Log);			
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
			KeyBoardCaptureObj.CaptureThreadKeyPressed.disconnect(this, this.KeyCaptureDetectFunction);
			ExperimentManagerObj.ExperimentStateHasChanged.disconnect(this, this.EM_ExperimentStateChanged);  
			ExperimentManagerObj.WriteToLogOutput.disconnect(this,this.Log);	
		} 
		catch (e) 
		{
			Log(".*. Something went wrong disconnecting the Signal/Slots:" + e); //If a disconnection fails warn the user!
		}		
	}	
}

function EM_ExternalTriggerIncremented() 
{
	Log("*** EM_ExternalTriggerIncremented()");
	//LogState(cExperimentStructure_Object.getCurrentExperimentState());
}

function EM_ExperimentStateChanged(currentEMState,timestamp)
{
	Log("*** EM_ExperimentStateChanged() to:" + currentEMState + " @" + timestamp);
	if(currentEMState == ExperimentManager.ExperimentState.ExperimentManager_Stopped)	//ExperimentManager_Stopped
	{
		ConnectDisconnectExperimentStructure(false);
	}		
}

function LogState(currentExperimentState)
{
	Log("-----");
	var tmpLoopID = currentExperimentState.CurrentBlock_LoopID;
	if (tmpLoopID == -4)
		Log("CurrentBlock_LoopID: RA_UNDEFINED");
	else
		Log("CurrentBlock_LoopID:" + currentExperimentState.CurrentBlock_LoopID);
	if(cLoopStructure_Object0)
		Log("CurrentBlock_LoopCounter0:" + cLoopStructure_Object0.getLoopCounter());
	if(cLoopStructure_Object1)
		Log("CurrentBlock_LoopCounter1:" + cLoopStructure_Object1.getLoopCounter());
	if(cLoopStructure_Object2)
		Log("CurrentBlock_LoopCounter2:" + cLoopStructure_Object2.getLoopCounter());
	Log("CurrentBlock_BlockID:" + currentExperimentState.CurrentBlock_BlockID);
	Log("CurrentBlock_TrialNumber:" + currentExperimentState.CurrentBlock_TrialNumber);
	Log("CurrentBlock_InternalTrigger:" + currentExperimentState.CurrentBlock_InternalTrigger);
	Log("CurrentBlock_ExternalTrigger:" + currentExperimentState.CurrentBlock_ExternalTrigger);
	Log("Experiment_ExternalTrigger:" + currentExperimentState.Experiment_ExternalTrigger);
	Log("-----");
}

function ExperimentStateChanged()
{
	Log("-O- Experiment State Changed -O-");
	//LogState(cExperimentStructure_Object.getCurrentExperimentState());
}

function ExperimentStarted()
{
	Log("-*- Experiment Started");
}

function ExperimentStopped()
{
	Log("-*- Experiment Stopped");
}

function CreateBlock(BlockObject,ID,Trials,IntTriggers,ExtTriggers)
{	
	//BlockObject = new cBlockStructure();	
	BlockObject.BlockName = "Block " + ID;
	//Log("Block Name: " + BlockObject.BlockName);
	BlockObject.BlockID = ID;
	//Log("Block ID: " + BlockObject.BlockID);
	BlockObject.BlockNumber = ID;
	//Log("Block Number: " + BlockObject.BlockNumber);
	BlockObject.NumberOfTrials = Trials;
	//Log("Block Number Of Trials: " + BlockObject.NumberOfTrials);
	BlockObject.NumberOfInternalTriggers = IntTriggers;
	//Log("Block Number Of Internal Triggers: " + BlockObject.NumberOfInternalTriggers);
	BlockObject.NumberOfExternalTriggers = ExtTriggers;
	//Log("Block Number Of External Triggers: " + BlockObject.NumberOfExternalTriggers);
	//return BlockObject;
	Log("\t-*- new Block Created");
}

function CreateLoop(LoopObject,ID,NumberOfLoops,TargetBlockID)
{
	//LoopObject = new cLoopStructure();	
	LoopObject.LoopName = "Loop " + ID;
	//Log("Loop Name: " + LoopObject.LoopName);
	LoopObject.LoopID = ID;
	//Log("Loop ID: " + LoopObject.LoopID);
	LoopObject.LoopNumber = ID;
	//Log("Loop Number: " + LoopObject.LoopNumber);
	LoopObject.NumberOfLoops = NumberOfLoops;
	//Log("Loop Number Of Loops: " + LoopObject.NumberOfLoops);
	LoopObject.TargetBlockID = TargetBlockID;
	//Log("Loop Target Block ID: " + LoopObject.TargetBlockID);
	Log("\t-*- new Loop Created");
}

function CreateExperimentFromScript()
{
	cExperimentStructure_Object = new cExperimentStructure();
	Log("-*- Going to create a new experiment from script code");
	cExperimentStructure_Object.ExperimentName = "Test Experiment";
	Log("Experiment Name: " + cExperimentStructure_Object.ExperimentName);
	cExperimentStructure_Object.ExperimentID = 99;
	Log("Experiment ID: " + cExperimentStructure_Object.ExperimentID);
	cExperimentStructure_Object.ExperimentDebugMode = true;
	Log("Experiment Debug Mode: " + cExperimentStructure_Object.ExperimentDebugMode);

	cBlockStructure_Object0 = new cBlockStructure();
	CreateBlock(cBlockStructure_Object0,0,2,5,2);//(BlockObject,ID,Trials,IntTriggers,ExtTriggers)
	var bResult = cExperimentStructure_Object.insertBlock(cBlockStructure_Object0);
	//Log("Add a defined Block result: " + bResult);

	cBlockStructure_Object1 = new cBlockStructure();
	CreateBlock(cBlockStructure_Object1,1,1,1,1);//(BlockObject,ID,Trials,IntTriggers,ExtTriggers)
	bResult = cExperimentStructure_Object.insertBlock(cBlockStructure_Object1);
	//Log("Add a defined Block result: " + bResult);
	cBlockStructure_Object2 = new cBlockStructure();
	CreateBlock(cBlockStructure_Object2,2,1,1,1);//(BlockObject,ID,Trials,IntTriggers,ExtTriggers)
	bResult = cExperimentStructure_Object.insertBlock(cBlockStructure_Object2);
	//Log("Add a defined Block result: " + bResult);
	
	cLoopStructure_Object0 = new cLoopStructure();
	CreateLoop(cLoopStructure_Object0,0,2,cBlockStructure_Object0.getBlockID());//(LoopObject,ID,NumberOfLoops,TargetBlockID)
	bResult = cBlockStructure_Object1.insertLoop(cLoopStructure_Object0);
	//Log("Add a defined Loop result: " + bResult);
	//Log("\tLoop Count: " + cBlockStructure_Object1.getLoopCount());	

	Log("-*- Experiment Created");
}

function getItem(title,text,items,current)
{
	var item = QInputDialog.getItem(this, title, text, items, current, false, Qt.WindowFlags(0));
	return item;
}

function ShowMenuCommands()
{
	//StimulGL.clearOutputWindow();
	Log("------------------------------------");
	Log("- COMMANDS MENU");
	Log("------------------------------------");
	Log("1\t\t: Load experiment from file");
	Log("2\t\t: Create experiment (in script)");
	Log("3\t\t: Change experiment");
	Log("4\t\t: Show experiment structure (in visual editor)");
	Log("5\t\t: Show experiment state (in Output log pane)");
	Log("6\t\t: Execute experiment");

	Log("9\t\t: Show this menu");
	Log("Escape\t	: Exit");
	Log("------------------------------------\n\n");
}

function KeyCaptureDetectFunction(keyCode)
{
	Log("A key press was detected: " + keyCode); 
	if(keyCode == 27)//Escape
	{
		CleanupScript();
	}
	else if(keyCode == 49)//1 = load experiment from file
	{
		if(!ExperimentManagerObj.loadExperiment(sExperimentFilePath,false,true))
		{
			Log("~~Failed to load the Experiment file");
		}
		else
		{
			cExperimentStructure_Object = ExperimentManagerObj.getExperimentStructure();//new cExperimentStructure(ExperimentManagerObj.getExperimentStructure());//ExperimentManagerObj.getExperimentStructure();
			Log(cExperimentStructure_Object);
			Log(cExperimentStructure_Object.ExperimentName);
		}
	}	
	else if(keyCode == 50)//2 = create experiment in script
	{
		CreateExperimentFromScript();
		Log(cExperimentStructure_Object);
		Log(cExperimentStructure_Object.ExperimentName);		
	}		
	else if(keyCode == 51)//3 = change experiment in memory
	{
		if(cExperimentStructure_Object != null)
		{
			cBlockStructure_Object0.BlockName = "Custom Block Name";
			cLoopStructure_Object2 = new cLoopStructure();
			CreateLoop(cLoopStructure_Object2,2,1,cBlockStructure_Object0.getBlockID());
			Log(">> Add a defined Loop result: " + cBlockStructure_Object0.insertLoop(cLoopStructure_Object2));
		}
		else
		{
			Log("No Experiment configured...")
		}	
	}	
	else if(keyCode == 52)//4 = show experiment in memory
	{
		if(!ExperimentManagerObj.showVisualExperimentEditor(cExperimentStructure_Object)) //cExperimentStructure_Object1
		{
			Log("~~Failed to show the Experiment Structure");
		}
	}
	else if(keyCode == 53)//5 = show experiment in memory
	{
		LogState(cExperimentStructure_Object.getCurrentExperimentState());
	}	
	else if(keyCode == 54)//6 = execute experiment
	{
		if(!ExperimentManagerObj.runExperiment()) //Try to run the experiment
		{
			Log("~~Failed to execute the Experiment");
		}
		else
		{
			ConnectDisconnectExperimentStructure(true);
		}
	}
	else if(keyCode == 57)//9 = show the menubar
	{
		ShowMenuCommands();
	}
}

ConnectDisconnectScriptFunctions(true);
KeyBoardCaptureObj.StartCaptureThread(0, true,new Array(27,49,50,51,52,53,54,55,56,57));//Escape-key and key(1..9) are processed
StimulGL.enableActiveDocument(false);
sTimer.startTimer(1000);
ShowMenuCommands();
